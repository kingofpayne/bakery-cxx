/**
 * Copyright (C) 2010, 2011, 2012, 2013
 * Olivier Hériveaux.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @author Olivier Hériveaux
 */


#include "compiler.hpp"
#include "util.hpp"
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/if.hpp>
#include <boost/lambda/lambda.hpp>
#include "util.hpp"
#include "str.hpp"
#include "rec/util.hpp"
#include "parser.hpp"
#include <limits>


namespace bakery {
namespace compiler {


/**
 * Echos indentation in output.
 */
void decompilation_state_t::write_indent()
{
    for (int i=0; i<indent_level; ++i)
        output << "  ";
}


/**
 * Compiles a dat file into a bin file.
 *
 * @param dat_path data file path.
 * @param bin_path Destination binary file path.
 * @param include_directories Directories in which the files may be searched.
 */
void compile(
    const std::string & dat_path,
    const std::string & bin_path,
    const std::list<std::string> & include_directories,
    compilation_log_t & log)
{
    /* Make dat_path and bin_path absolute. */
    boost::filesystem::path abs_dat_path = make_absolute(dat_path);
    boost::filesystem::path abs_bin_path = make_absolute(bin_path);

    /* Opens the output file for writing. */
    std::ofstream output(abs_bin_path.string(), std::ios::binary);

    if (!output.is_open())
    {
        log.add_error("Failed to open file '" + abs_bin_path.string()
            + "' for output writing.");
        return;
    }

    recipe_or_data recipe_or_data;
    parser::load_from_file(abs_dat_path.string(), recipe_or_data, log);

    if (log.get_error_count() != 0)
    {
        log.add_error("Error in data file '" + abs_dat_path.string() + "'.");
        return;
    }

    /* Check that the parser returned a data and not a recipe. */
    if (!recipe_or_data.is_data())
    {
        log.add_error("File '" + abs_dat_path.string()
            + "' could not be parsed as a data.");
        return;
    }

    dat::data & data = recipe_or_data.get_data();
    compilation_state_t state = { output, tti_stack_t(), log };

    write_data(state, data, abs_dat_path.parent_path().string() + "/",
        include_directories);
}


/**
 * Decompiles a binary file to generate a data file, given a recipe file.
 * This is the inverse operation of the compilation.
 *
 * @param bin_path Destination binary file path.
 * @param def_path Definition file path. The recipe file cannot be guessed
 *        from the binary file. This may be an absolute path or a relative path.
 *        If this path starts with '<' and ends with '>' the include directories
 *        of the context are scanned to find the file.
 * @param dat_path data file path.
 * @param include_directories Directories in which the files may be searched.
 * @param log The object in which error messages will be pushed.
 */
void decompile(
    const std::string & bin_path,
    const std::string & def_path,
    const std::string & dat_path,
    const std::list<std::string> & include_directories,
    compilation_log_t & log)
{
    bool scan_inc_dirs = (def_path.size() >= 2) && (def_path[0] == '<') &&
        (def_path[def_path.size()-1] == '>');

    file_indication_t def_path_fi;

    def_path_fi.set_path(scan_inc_dirs ? def_path.substr(1, def_path.size()-2) :
        def_path);

    def_path_fi.set_absolute(scan_inc_dirs);

    /* Make all paths absolute. */
    boost::filesystem::path abs_bin_path = make_absolute(bin_path);
    boost::filesystem::path abs_dat_path = make_absolute(dat_path);

    /* Opens the output file for writing. */
    std::ofstream output_file(abs_dat_path.string().c_str());

    if (!output_file.is_open())
    {
        log.add_error("Failed to open file '" + abs_dat_path.string()
            + "' for output writing.");

        return;
    }

    /* Open the input binary file. */
    std::ifstream input_file(abs_bin_path.string().c_str(), std::ios::binary);

    if (!input_file.is_open())
    {
        log.add_error("Failed to open input file '" + abs_bin_path.string()
            + "'.");

        return;
    }

    /* Compute the size of the binary file. */
    input_file.seekg(0, std::ios::end);
    size_t size = input_file.tellg();
    input_file.seekg(0, std::ios::beg);

    decompilation_state_t state = { input_file, output_file, size,
        tti_stack_t(), 0, log };

    std::string current_dir = abs_bin_path.parent_path().string() + "/";
    read_data(state, def_path_fi, current_dir, include_directories);
}


/**
 * Writes data to a output.
 *
 * @param state Compilation state.
 * @param data The data to write. This is issued from the data parser.
 * @param current_directory The base directory to use for all relative path
 *        calculations - should be the directory of the compiled data file.
 * @param include_directories Directories in which the recipe files may be
 *        searched.
 */
bool write_data(
    compilation_state_t & state,
    const dat::data & data,
    const std::string & current_directory,
    const std::list<std::string> & include_directories)
{
    /* Firstly, get the recipe file path. */
    std::string def_path;

    if ( !resolve_file_indication (
        data.get_recipe_indication().fi,
        current_directory,
        include_directories,
        def_path,
        state.log) )
    {
        state.log.add_error("recipe file not found.");
        return false;
    }

    /* Loads the recipe */
    recipe_or_data recipe_or_data;
    parser::load_from_file(def_path, recipe_or_data, state.log);

    if (state.log.get_error_count() != 0)
    {
        state.log.add_error("Failed to parse main recipe file '"
            + def_path + "'.");

        return false;
    }

    /* Check the grammar returned a recipe. */
    if (!recipe_or_data.is_recipe())
    {
        state.log.add_error("File '" + def_path
            + "' could not be parsed as a recipe.");

        return false;
    }

    rec::recipe & recipe = recipe_or_data.get_recipe();

    /* Handle included recipe files. */
    std::list<std::string> loaded_recipe_files;

    if (!merge_included_recipe_files(
        recipe,
        boost::filesystem::path(def_path).parent_path().string() + "/",
        include_directories,
        loaded_recipe_files,
        state.log ) )
    {
        state.log.add_error(
            "Failed to include some files into main recipe.");
        return false;
    }

    if (state.log.get_error_count() != 0)
    {
        state.log.add_error(
            "Error occured while parsing included recipe files.");
        return false;
    }

    /* Populate the recipe. */
    populate_node(recipe.get_node());

    /* Compile the recipe. */
    if (!recipe.compile(state.log))
        return false;

    /* If errors are encounterred during the recipe file parsing, it is not
     * necessary to continue... */
    if (state.log.get_error_count() != 0)
        return false;


    /* The recipe can be either the members defined at the root of the
     * recipe file, or a struct type declared or imported in the recipe
     * file itself.
     * The first case corresponds to data files with recipe defined like:
     * recipe "some_file.def";
     * And for the second case:
     * recipe "some_file.def" some_type; */
    tti_stack_t tti_stack;
    if (data.get_recipe_indication().ti.has_value())
    {
        /* The type is specified in the recipe declaration (second case). */
        rec::type_instanciation_t ti = data.get_recipe_indication().ti.value();
        /* note: ti is a copy we can edit. */
        rec::node::sptr def_root_node = recipe.get_node();
        def_root_node->compile_type_instanciation(ti, state.log, def_root_node.get());
        write_structure(state, ti, *data.get_root_node());
    }
    else
    {
        /* All the members at the root of the recipe file should be written
         * in the binary file (first case).
         * Write the root structure node, which will write the whole content. */
        rec::type_instanciation_t root_structure_instanciation;
        root_structure_instanciation.set_type_node_ptr(recipe.get_node().get());
        write_structure(state, root_structure_instanciation, *data.get_root_node());
    }
    bakery_assert_debug(tti_stack.size() == 0); /* sanity check */

    return true;
}


/**
 * Writes a data node, according to a recipe node.
 *
 * @param state Compilation state.
 * @param type_inst The type instanciation.
 * @param data_node The data node.
 *
 * @return True if no error occurs.
 */
bool write_node(compilation_state_t & state,
    const rec::type_instanciation_t & type_inst, const dat::node & data_node)
{
    rec::node::kind::value kind = type_inst.get_type_ptr()->get_kind();

    /* Verify that the type is declared as 'unsigned' only for native types. */
    if (type_inst.is_unsigned() && (kind != rec::node::kind::native))
    {
        state.log.add_error("Type '" + type_inst.print() + "' of node '"
            + data_node.print() + "' cannot be declared as unsigned.");
        return false;
    }

    switch (kind)
    {
        case rec::node::kind::structure:
            return write_structure(state, type_inst, data_node);

        case rec::node::kind::variant:
            return write_variant(state, type_inst, data_node);

        case rec::node::kind::array:
            return write_array(state, type_inst, data_node);

        case rec::node::kind::typedef_:
            return write_typedef(state, type_inst, data_node);

        case rec::node::kind::enum_:
            return write_enum(state, type_inst, data_node);

        case rec::node::kind::native:
            return write_native(state, type_inst, data_node);

        case rec::node::kind::template_type:
            return write_template_type(state, type_inst, data_node);

        /* Other cases should not happen. */
        default:
            bakery_unexpected_case();
    }
}


/**
 * Write a data node, according to a recipe template type.
 *
 * @param state Compilation state.
 * @param type_inst The type instanciation.
 * @param data_node The data node.
 *
 * @return True if no error occurs.
 */
bool write_template_type(compilation_state_t & state,
    const rec::type_instanciation_t & type_inst, const dat::node & data_node)
{
    /* template type must be matched to a real type. For this, tti stack must be
     * explored to find the latest instanciation of the template. */
    tti_stack_t::const_reverse_iterator it = std::find_if
    (
        state.tti_stack.rbegin(),
        state.tti_stack.rend(),
        [&](const std::pair<rec::node::sptr, rec::type_instanciation_t> & p)
            -> bool
        {
            return p.first.get() == type_inst.get_type_ptr();
        }
    );

    bakery_assert(it != state.tti_stack.rend());
    return write_node(state, it->second, data_node);
}


/**
 * Writes a data node, according to a recipe structure.
 *
 * @param state Compilation state.
 * @param type_inst The type instanciation.
 * @param data_node The data node.
 *
 * @return True if no error occurs.
 */
bool write_structure(compilation_state_t & state,
    const rec::type_instanciation_t & type_inst, const dat::node & data_node)
{
    /* Check data node's type. */
    if (data_node.get_kind() != dat::node::kind::group)
    {
        state.log.add_error("expected a group node.");
        return false;
    }

    const rec::node* type_ptr = type_inst.get_type_ptr();

    /* Check template parameter count. */
    if (!check_template_parameter_count(type_inst, state.log))
        return false;

    /* Push in the tty stack the instancied types for template types. As we use
     * a tti_stacker, we are sure the elements will be poped correctly when this
     * function returns. */
    std::list<rec::node::sptr> template_types = type_ptr->query_nodes<std::list>
        (rec::node::kind::template_type);

    tti_stacker_t<
        std::list<rec::node::sptr>,
        rec::type_instanciation_t::parameter_list
    > stacker(
        state.tti_stack,
        template_types.begin(),
        template_types.end(),
        type_inst.get_parameters().begin(),
        type_inst.get_parameters().end()
    );

    /* Get the data of the structure */
    const rec::structure_data_t & the_structure_data =
        type_ptr->get_structure_data();

    /* Firstly, we must write the data of the herited types. */
    for (const rec::type_instanciation_t & herited_type_inst :
        the_structure_data.heritance_list)
    {
        if (!write_node(state, herited_type_inst, data_node))
        {
            state.log.add_error("failed to write inherited data of type '"
                + herited_type_inst.print() + "'.");

            return false;
        }
    }

    /* Get the assignment list. */
    std::list<dat::node::sptr> assignments = data_node.query_nodes<std::list>(
        dat::node::kind::assignment);

    /* For each member of the structure. */
    for (const rec::node::sptr & node :
        type_ptr->query_nodes<std::list>(rec::node::kind::member))
    {
        const std::string & name = node->get_name();

        /* Retrive the member data */
        const rec::member_data_t & member_data = node->get_member_data();

        /* Checks if the node is optional or not. */
        bool node_is_optional =
            node->has_qualifier(rec::node::qualifier::optional);

        /* Does the node has a default value ? */
        bool has_default_value = member_data.has_default_value();

        /* A node cannot be optional if it has a default value. */
        if (has_default_value && node_is_optional)
        {
            state.log.add_error("member '" + name + "' cannot be optional and "
                "have a default value at the same time.");

            return false;
        }

        /* Find the data node which has the same name. */
        std::list<dat::node::sptr>::const_iterator it_data_node = std::find_if(
            assignments.begin(),
            assignments.end(),
            boost::bind(
                std::equal_to<std::string>(),
                boost::ref(name),
                boost::bind(&dat::node::get_name, _1)
            )
        );

        bool data_is_defined = it_data_node != assignments.end();

        if ((!data_is_defined) && (!node_is_optional)
            && (!has_default_value))
        {
            state.log.add_error("data node '" + name + "' is missing.");
            return false;
        }

        /* Ok, node has been found or is optional, or has a default value.
         * If node is optional, write the presence flag (a bool which is true
         * is the data has been defined). */
        if (node_is_optional)
            stream_write_binary(state.output, data_is_defined);

        if (data_is_defined)
        {
            if (!write_node(state, member_data.type_instanciation,
                *((*it_data_node)->get_children().front())))
            {
                return false;
            }
        }
        else
        {
            if (!node_is_optional)
            {
                if (!write_node(state, member_data.type_instanciation,
                    *(member_data.default_value_node)))
                {
                    return false;
                }
            }
        }
    }

    return true;
}


/**
 * Writes a data node, according to a recipe variant node.
 *
 * @param state Compilation state.
 * @param type_inst The type instanciation.
 * @param data_node The data node.
 *
 * @return True if no error occurs.
 */
bool write_variant(compilation_state_t & state,
    const rec::type_instanciation_t & type_inst, const dat::node & data_node)
{
    /* Check data node's type. */
    if (data_node.get_kind() != dat::node::kind::variant)
    {
        state.log.add_error("expected a variant node, got a "
            + dat::node::kind::wrapper::to_string(data_node.get_kind())
            + " node.");

        return false;
    }

    const rec::node* type_ptr = type_inst.get_type_ptr();

    /* Template argument count must be equal to template types. */
    const rec::type_instanciation_t::parameter_list & template_parameters =
        type_inst.get_parameters();

    std::list<rec::node::sptr> template_types = type_ptr->query_nodes<std::list>
        (rec::node::kind::template_type);

    if (template_types.size() != template_parameters.size())
    {
        state.log.add_error("wrong number of template parameters, "
            + str::from(template_types.size()) + " expected, got "
            + str::from(template_parameters.size()) + ".");

        return false;
    }

    /* Push in the tty stack the instancied types for template types. As we use
     * a tti_stacker, we are sure the elements will be poped correctly when this
     * function returns. */
    tti_stacker_t<
        std::list<rec::node::sptr>,
        rec::type_instanciation_t::parameter_list
    > stacker(
        state.tti_stack,
        template_types.begin(),
        template_types.end(),
        template_parameters.begin(),
        template_parameters.end()
    );

    /* Get the node of the assigned member. */
    const std::string & assigned_member_name = data_node.get_name();

    /* Get the member list. */
    std::list<rec::node::sptr> members = type_ptr->query_nodes<std::list>(
        rec::node::kind::member);

    /* Find the corresponding member. */
    unsigned int member_index = 0;
    std::list<rec::node::sptr>::const_iterator it_member_node = members.begin();

    const std::list<rec::node::sptr>::const_iterator end_it_member_node =
        members.end();

    while (it_member_node != end_it_member_node)
    {
        if ((*it_member_node)->get_name() == assigned_member_name)
            break;

        member_index++;
        it_member_node++;
    }

    /* Check that the member name is ok. */
    if (it_member_node == members.end())
    {
        state.log.add_error("variant member '" + assigned_member_name
            + "' does not exist.");

        return false;
    }

    /* Transform it_member_node into a node (for code clarity). */
    rec::node::sptr member_node = *it_member_node;

    /* Verify that the member is not marked as optional */
    if (member_node->has_qualifier(rec::node::qualifier::optional))
    {
        state.log.add_error("variant member '" + assigned_member_name
            + "' cannot be marked as optional since it belongs to a variant.");

        return false;
    }

    /* Verify that the member has no default value. */
    if (member_node->get_member_data().has_default_value())
    {
        state.log.add_error("variant member '" + assigned_member_name
            + "' cannot have a default value since it belongs to a variant.");

        return false;
    }

    /* Get the data node. */
    dat::node::sptr sub_data_node = *data_node.get_children().begin();

    /* Write the variant index. */
    stream_write_binary(state.output, member_index);

    /* Write the data. */
    if (!write_node(state, member_node->get_member_data().type_instanciation,
        *sub_data_node))
    {
        state.log.add_error("failed to write data of variant member '"
            + assigned_member_name + "'.");

        return false;
    }

    return true;
}


/**
 * Writes a data node, according to a recipe array node.
 *
 * @param state Compilation state.
 * @param type_inst The type instanciation.
 * @param data_node The data node.
 *
 * @return True if no error occurs.
 */
bool write_array(compilation_state_t & state,
    const rec::type_instanciation_t & type_inst, const dat::node & data_node)
{
    /* Get array data */
    const rec::array_data_t & array_data =
        type_inst.get_type_ptr()->get_array_data();

    /* This should not fail, it the grammar does its job correctly. */
    bakery_assert_debug(array_data.dims.size());

    if (!write_array_dim(state, type_inst, 0, data_node))
    {
        std::string dim_desc;

        std::cout << "dim desc size: " << array_data.dims.size() << std::endl;

        for (size_t dim : array_data.dims)
        {
            dim_desc.push_back('[');

            if (dim)
                dim_desc += str::from(dim);

            dim_desc.push_back(']');
        }

        state.log.add_error("failed to write array of dimension " + dim_desc
            + ".");

        return false;
    }

    return true;
}


/**
 * Writes a data node, according to one array node dimension.
 *
 * @param state Compilation state.
 * @param type_inst The type instanciation.
 * @param dim Dimension index.
 * @param data_node The data node.
 *
 * @return True if no error occurs.
 */
bool write_array_dim(
    compilation_state_t & state,
    const rec::type_instanciation_t & type_inst,
    size_t dim,
    const dat::node & data_node)
{
    /* Check data node's type. */
    if (data_node.get_kind() != dat::node::kind::group)
    {
        state.log.add_error("expected a group node.");
        return false;
    }

    const dat::node::node_sptr_list & children = data_node.get_children();

    /* Get array data */
    const rec::array_data_t & array_data =
        type_inst.get_type_ptr()->get_array_data();

    const size_t dim_size = array_data.dims[dim];

    if (dim_size == 0)
    {
        /* Array is dynamic, write the number of elements. */
        stream_write_binary(state.output, children.size());
    }
    else
    {
        /* Array size is fixed. Check that we have the right element count. */
        if (children.size() != dim_size)
        {
            state.log.add_error("array dimension expected size is "
                + str::from(dim_size) + " but got "
                + str::from(children.size()) + " elements.");

            return false;
        }
    }

    /* Index used used only in error messages. */
    size_t index = 0;

    if ((dim + 1) == array_data.dims.size())
    {
        /* We are writting the last dimension of the array. */
        for (const dat::node::sptr & n : children)
        {
            if (!write_node(state, array_data.type_instanciation, *n))
            {
                state.log.add_error("failed to write array element number "
                    + str::from(index) + " (in dimension " + str::from(dim + 1)
                    + ").");

                return false;
            }

            index++;
        }
    }
    else
    {
        /* We are not writting the last dimension. Call recursively this
         * function to write next dimension. */
        for (const dat::node::sptr & n : children)
        {
            if (!write_array_dim(state, type_inst, dim + 1, *n))
            {
                state.log.add_error("failed to write array element number "
                    + str::from(index) + ".");

                return false;
            }

            index++;
        }
    }

    return true;
}


/**
 * Writes a data node, according to a recipe typedef node.
 *
 * @param state Compilation state.
 * @param type_inst The type instanciation.
 * @param data_node The data node.
 *
 * @return True if no error occurs.
 */
bool write_typedef(compilation_state_t & state,
    const rec::type_instanciation_t & type_inst, const dat::node & data_node)
{
    if (!check_template_parameter_count(type_inst, state.log))
        return false;

    const rec::node* type_ptr = type_inst.get_type_ptr();

    /* Push in the tty stack the instancied types for template types. As we use
     * a tti_stacker, we are sure the elements will be poped correctly when this
     * function returns. */
    std::list<rec::node::sptr> template_types =
        type_ptr->query_nodes<std::list>(rec::node::kind::template_type);

    tti_stacker_t<
        std::list<rec::node::sptr>,
        rec::type_instanciation_t::parameter_list
    > stacker(
        state.tti_stack,
        template_types.begin(),
        template_types.end(),
        type_inst.get_parameters().begin(),
        type_inst.get_parameters().end()
    );

    return write_node(state, type_ptr->get_typedef_data().type_instanciation,
        data_node);
}


/**
 * Writes a data node, according to a enumeration node.
 *
 * @param state Compilation state.
 * @param type_inst The type instanciation.
 *
 * @return True if no error occurs.
 */
bool write_enum(compilation_state_t & state,
    const rec::type_instanciation_t & type_inst, const dat::node & data_node)
{
    /* Get the enumeration recipe node. */
    const rec::node* type_ptr = type_inst.get_type_ptr();

    /* Check data node's type. */
    if (data_node.get_kind() != dat::node::kind::identifier)
    {
        state.log.add_error("expected an identifier for the enumeration '"
            + type_ptr->get_name() + "'.");

        return false;
    }

    /* Template argument count must be equal to zero. */
    if (type_inst.get_parameters().size() != 0)
    {
        state.log.add_error("an enumeration cannot have template parameters.");
        return false;
    }

    /* Get the enumeration identifier. */
    const std::string & identifier = data_node.get_identifier();

    /* Find the enumeration member. */
    rec::node::sptr enum_value_node = type_ptr->find_node(identifier);
    if (enum_value_node.get() != 0)
    {
        /* Found. Write the value. */
        int enum_value = enum_value_node->get_enum_value_data().value;
        stream_write_binary(state.output, enum_value);
    }
    else
    {
        /* Not found. */
        state.log.add_error("'" + identifier
            + "' is not a member of the enumeration '" + type_ptr->get_name()
            + "'.");

        return false;
    }

    return true;
}


/**
 * Writes a data node, according to a recipe native node.
 *
 * @param state Compilation state.
 * @param type_inst The type instanciation.
 * @param data_node The data node.
 *
 * @return True if no error occurs.
 */
bool write_native(compilation_state_t & state,
    const rec::type_instanciation_t & type_inst, const dat::node & data_node)
{
    rec::native_data_t::class_t::value cls =
        type_inst.get_type_ptr()->get_native_data().get_class();

    /* Some types cannot be declared as unsigned. Check this. */
    if (type_inst.is_unsigned())
    {
        switch (cls)
        {
            case rec::native_data_t::class_t::int_:
            case rec::native_data_t::class_t::char_:
            case rec::native_data_t::class_t::short_:
                break;
            default:
                state.log.add_error("Type '" + type_inst.print() +
                    "' of member '" + data_node.print() + "' cannot be declared"
                    " as unsigned.");
                return false;
        }
    }

    switch (cls)
    {
        case rec::native_data_t::class_t::bool_:
            return write_native_bool(state, type_inst, data_node);

        case rec::native_data_t::class_t::int_:
        case rec::native_data_t::class_t::char_:
        case rec::native_data_t::class_t::short_:
            return write_native_integer(state, type_inst, data_node);

        case rec::native_data_t::class_t::string:
            return write_native_string(state, type_inst, data_node);

        case rec::native_data_t::class_t::float_:
        case rec::native_data_t::class_t::double_:
            return write_native_floating(state, type_inst, data_node);

        case rec::native_data_t::class_t::pair:
            return write_native_pair(state, type_inst, data_node);

        case rec::native_data_t::class_t::tuple:
            return write_native_tuple(state, type_inst, data_node);

        case rec::native_data_t::class_t::list:
            return write_native_list(state, type_inst, data_node);

        case rec::native_data_t::class_t::map:
            return write_native_map(state, type_inst, data_node);

        default:
            bakery_unexpected_case();
    }
}


/**
 * Writes a data node, according to a recipe pair node.
 *
 * @param state Compilation state.
 * @param type_inst The type instanciation.
 * @param data_node The data node.
 *
 * @return True if no error occurs.
 */
bool write_native_pair(compilation_state_t & state,
    const rec::type_instanciation_t & type_inst, const dat::node & data_node)
{
    /* Check data node's type. */
    if (data_node.get_kind() != dat::node::kind::group)
    {
        state.log.add_error("expected a group node.");
        return false;
    }

    const dat::node::node_sptr_list & children = data_node.get_children();

    /* Children must have only two elements, no more. */
    if (children.size() != 2)
    {
        state.log.add_error("nwrong number of elements in pair, 2 expected"
            ", got " + str::from(children.size()) + ".");

        return false;
    }

    /* pair is a template parametized type. We need to retrieve the type of the
     * elements. */
    const rec::type_instanciation_t::parameter_list & template_parameters =
        type_inst.get_parameters();

    size_t size = template_parameters.size();

    /* Up to 1 or 2 template parameters are expected. */
    if(! ((size == 1) || (size == 2)) )
    {
        state.log.add_error("wrong number of template parameters, 1 or 2 "
            "expected, got " + str::from(size) + ".");

        return false;
    }

    /* Get the type of the first element. */
    const rec::type_instanciation_t & type_of_a = template_parameters[0];

    /* Get the type of the second element. If only one template parameter was
     * specified, then the type is the same as the first element. With this
     * feature it is possible to reduce syntax: instead of writing
     * pair<int, int>
     * just write:
     * pair<int> */
    const rec::type_instanciation_t & type_of_b = (size == 2)?
        template_parameters[1]:
        type_of_a;

    /* Write the first element of the pair */
    dat::node::node_sptr_list::const_iterator it_child = children.begin();

    if (!write_node(state, type_of_a, **it_child))
    {
        state.log.add_error("failed to write pair first element.");
        return false;
    }

    /* Write the second element of the pair */
    if (!write_node(state, type_of_b, **(++it_child)))
    {
        state.log.add_error("failed to write pair second element.");
        return false;
    }

    return true;
}


/**
 * Writes a data node, according to a recipe tuple node.
 *
 * @param state Compilation state.
 * @param type_inst The type instanciation.
 * @param data_node The data node.
 *
 * @return True if no error occurs.
 */
bool write_native_tuple(compilation_state_t & state,
    const rec::type_instanciation_t & type_inst, const dat::node & data_node)
{
    /* Check data node's type. */
    if (data_node.get_kind() != dat::node::kind::group)
    {
        state.log.add_error("expected a group node.");
        return false;
    }

    const dat::node::node_sptr_list & children = data_node.get_children();

    /* tuple is a template parametized type. We need to retrieve the type of the
     * elements. */
    const rec::type_instanciation_t::parameter_list & template_parameters =
        type_inst.get_parameters();

    size_t size = template_parameters.size();

    /* At least one template parameter should be present. */
    if (size == 0)
    {
        state.log.add_error("wrong number of template parameters, at least "
            "1 expected, got 0.");

        return false;
    }

    /* Children size must be equal to tuple size. */
    if (children.size() != size)
    {
        state.log.add_error("wrong number of elements in tuple, "
            + str::from(size) + " expected, got " + str::from(children.size())
            + ".");

        return false;
    }

    /* Write the elements. */
    size_t index = 0;

    for (const dat::node::sptr & child : children)
    {
        if (!write_node(state, template_parameters[index], *child))
        {
            state.log.add_error("failed to write tuple element number "
                + str::from(index) + ".");

            return false;
        }

        index++;
    }

    return true;
}


/**
 * Writes a data node, according to a recipe list node.
 *
 * @param state Compilation state.
 * @param type_inst The type instanciation.
 * @param data_node The data node.
 *
 * @return True if no error occurs.
 */
bool write_native_list(compilation_state_t & state,
    const rec::type_instanciation_t & type_inst, const dat::node & data_node)
{
    /* Check data node's type. */
    if (data_node.get_kind() != dat::node::kind::group)
    {
        state.log.add_error("expected a group node.");
        return false;
    }

    const dat::node::node_sptr_list & children = data_node.get_children();

    /* list is a template parametized type. We need to retrieve the type of the
     * elements. */
    const rec::type_instanciation_t::parameter_list & template_parameters =
        type_inst.get_parameters();

    if (template_parameters.size() != 1)
    {
        state.log.add_error("wrong number of template parameters, 1 "
            "expected, got " + str::from(template_parameters.size()) + ".");

        return false;
    }

    /* First, write the element count. */
    size_t count = children.size();
    stream_write_binary(state.output, count);

    unsigned int index = 0;

    for (const dat::node::sptr & child : children)
    {
        if (!write_node(state, template_parameters[0], *child))
        {
            state.log.add_error("failed to write list element number "
                + str::from(index) + ".");

            return false;
        }

        index++;
    }

    return true;
}


/**
 * Writes a data node, according to a recipe map node.
 *
 * @param state Compilation state.
 * @param type_inst The type instanciation.
 * @param data_node The data node.
 *
 * @return True if no error occurs.
 */
bool write_native_map(compilation_state_t & state,
    const rec::type_instanciation_t & type_inst, const dat::node & data_node)
{
    /* Check data node's type. */
    if (data_node.get_kind() != dat::node::kind::group)
    {
        state.log.add_error("expected a group node.");
        return false;
    }

    const dat::node::node_sptr_list & children = data_node.get_children();

    /* map is a template parametized type. We need to retrieve the type of the
     * key and the type of the elements. */
    const rec::type_instanciation_t::parameter_list & template_parameters =
        type_inst.get_parameters();

    if (template_parameters.size() != 2)
    {
        state.log.add_error("wrong number of template parameters, 2 "
            "expected, got " + str::from(template_parameters.size()) + ".");

        return false;
    }

    /* First, write the element count. */
    size_t count = children.size();
    stream_write_binary(state.output, count);

    unsigned int index = 0;

    for (const dat::node::sptr & child : children)
    {
        /* Child node's kind must be map_assignment, which contains two child
         * nodes. If the following tests fails, an exception is thrown since the
         * parser should not allow anything else. */
        bakery_assert(child->get_kind() == dat::node::kind::map_assignment);
        bakery_assert(child->get_children().size() == 2);

        dat::node::node_sptr_list::const_iterator it_data =
            child->get_children().begin();

        /* Write the key */
        if (!write_node(state, template_parameters[0], **it_data))
        {
            state.log.add_error("failed to write map element "
                + str::from(index) + " key.");

            return false;
        }

        /* Go to the second child data node. */
        it_data++;

        /* Write the value */
        if (!write_node(state, template_parameters[1], **it_data))
        {
            state.log.add_error("failed to write map element "
                + str::from(index) + " value.");

            return false;
        }

        index++;
    }

    return true;
}


/**
 * Writes a data node as a native bool.
 *
 * @param state Compilation state.
 * @param type_inst The type instanciation.
 * @param data_node The data node.
 *
 * @return True if no error occurs.
 */
bool write_native_bool(compilation_state_t & state,
    const rec::type_instanciation_t & type_inst, const dat::node & data_node)
{
    if (data_node.get_kind() != dat::node::kind::bool_)
    {
        state.log.add_error("value must be a bool.");
        return false;
    }

    stream_write_binary(state.output, data_node.get_bool());
    return true;
}


/**
 * Writes a data node as a native int, char or short.
 *
 * @param state Compilation state.
 * @param type_inst The type instanciation.
 * @param data_node The data node.
 *
 * @return True if no error occurs.
 */
bool write_native_integer(compilation_state_t & state,
    const rec::type_instanciation_t & type_inst, const dat::node & data_node)
{
    const rec::native_data_t::class_t::value the_class =
        type_inst.get_type_ptr()->get_native_data().get_class();

    if (data_node.get_kind() != dat::node::kind::floating)
    {
        state.log.add_error("value must be a number.");
        return false;
    }

    /* floating value must only have an integer part. */
    const dat::floating & floating = data_node.get_floating();

    if (floating.has_decimal_part())
    {
        state.log.add_error("integer cannot have a decimal part.");
        return false;
    }

    if (floating.has_exponent_part())
    {
        state.log.add_error("integer cannot have an exponent.");
        return false;
    }

    /* Safety check. If the parser did his job well, this should not happen
     * (a floating cannot have all its three parts empty, and the two previous
     * are empty, so the last one should not !). */
    bakery_assert(floating.has_integer_part());

    /* The parser garantees that the string matches -?[0-9]+
     * However, it does not check if the integer can be represented by an
     * integer (for example, "90000000000000000000000000" is not possible if int
     * size is 32bits). The value will be loaded in a mpz_class of the GMP
     * library, which support arbitrary long integers. Then a size test will be
     * performed to determine if the integer is small enought to be written as
     * an int. */
    mpz_class val(floating.get_integer_string());

    /* Apply the minus, if present.
     * Raise an error if the type is unsigned. */
    bool is_unsigned = type_inst.is_unsigned();
    if (floating.get_negative())
    {
        if (is_unsigned)
        {
            state.log.add_error("value " + str::from(val) + " is negative, but"
                " the type is unsigned.");
            return false;
        }

        val = -val;
    }

    switch (the_class)
    {
        case rec::native_data_t::class_t::int_:
        {
            if ( (!is_unsigned && !val.fits_sint_p())
                || (is_unsigned && !val.fits_uint_p()) )
            {
                state.log.add_error("value " + str::from(val)
                    + " does not fit an " + (is_unsigned ? "unsigned " : "")
                    + "int.");
                return false;
            }
            break;
        }

        case rec::native_data_t::class_t::short_:
        {
            if ( (!is_unsigned && !val.fits_sshort_p()) ||
                (is_unsigned && !val.fits_ushort_p()) )
            {
                state.log.add_error("value " + str::from(val)
                    + " does not fit a " + (is_unsigned ? "unsigned " : "")
                    + "short.");
                return false;
            }
            break;
        }

        case rec::native_data_t::class_t::char_:
        {
            bool fits = false;
            if (val.fits_sint_p())
            {
                int x = val.get_si();
                int the_min = is_unsigned ? 0 :
                    std::numeric_limits<char>::min();
                int the_max = is_unsigned ?
                    std::numeric_limits<unsigned char>::max() :
                    std::numeric_limits<char>::max();
                fits = (x <= the_max) && (x >= the_min);
            }

            if (!fits)
            {
                state.log.add_error("value " + str::from(val)
                    + " does not fit a " + (is_unsigned ? "unsigned " : "")
                    + "char.");
                return false;
            }

            break;
        }

        default:
            bakery_unexpected_case();
    }

    /* Output the value in the stream */
    if (!is_unsigned)
    {
        int x = val.get_si();

        switch (the_class)
        {
            case rec::native_data_t::class_t::int_:
                stream_write_binary(state.output, x);
                break;
            case rec::native_data_t::class_t::short_:
                stream_write_binary(state.output, (short)x);
                break;
            case rec::native_data_t::class_t::char_:
                stream_write_binary(state.output, (char)x);
                break;
            default:
                bakery_unexpected_case();
        }
    }
    else
    {
        unsigned int x = val.get_ui();

        switch (the_class)
        {
            case rec::native_data_t::class_t::int_:
                stream_write_binary(state.output, x);
                break;
            case rec::native_data_t::class_t::short_:
                stream_write_binary(state.output, (unsigned short)x);
                break;
            case rec::native_data_t::class_t::char_:
                stream_write_binary(state.output, (unsigned char)x);
                break;
            default:
                bakery_unexpected_case();
        }
    }

    return true;
}


/**
 * Writes a data node as a native float or double.
 *
 * @param state Compilation state.
 * @param type_inst The type instanciation.
 * @param data_node The data node.
 *
 * @return True if no error occurs.
 */
bool write_native_floating(compilation_state_t & state,
    const rec::type_instanciation_t & type_inst, const dat::node & data_node)
{
    const rec::native_data_t::class_t::value the_class =
        type_inst.get_type_ptr()->get_native_data().get_class();

    if (data_node.get_kind() != dat::node::kind::floating)
    {
        state.log.add_error("value must be a floating number.");
        return false;
    }

    mpf_class mpf;

    if (!floating_to_mpf(mpf, data_node.get_floating(), state.log))
    {
        state.log.add_error("failed to synthesize the floating value.");
        return false;
    }

    /* Verify that the value is not bigger than the maximal possible value. */
    switch(the_class)
    {
        case rec::native_data_t::class_t::float_:
        {
            if (mpf > mpf_class(std::numeric_limits<float>::max()))
            {
                state.log.add_error("floating value is bigger than the "
                    "maximal value that a float can store.");

                return false;
            }
            break;
        }

        case rec::native_data_t::class_t::double_:
        {
            if (mpf > mpf_class(std::numeric_limits<double>::max()))
            {
                state.log.add_error("floating value is bigger than the "
                    "maximal value that a double can store.");

                return false;
            }
            break;
        }

        default:
            bakery_unexpected_case();
    }

    /* Verify that the value is not smaller than the minimal possible value. */
    switch (the_class)
    {
        case rec::native_data_t::class_t::float_:
        {
            if (mpf < mpf_class(-std::numeric_limits<float>::max()))
            {
                state.log.add_error("floating value is smaller than the minimal"
                    " value that a float can store.");

                return false;
            }
            break;
        }

        case rec::native_data_t::class_t::double_:
        {
            if (mpf < mpf_class(-std::numeric_limits<double>::max()))
            {
                state.log.add_error("floating value is smaller than the minimal"
                    " value that a double can store.");

                return false;
            }
            break;
        }

        default:
            bakery_unexpected_case();
    }

    double dou = mpf.get_d();

    /* Now output the value in the stream. */
    switch (the_class)
    {
        case rec::native_data_t::class_t::float_:
            stream_write_binary(state.output, (float)dou);
            break;
        case rec::native_data_t::class_t::double_:
            stream_write_binary(state.output, dou);
            break;
        default:
            bakery_unexpected_case();
    }

    return true;
}


/**
 * Writes a data node as a native string.
 *
 * @param state Compilation state.
 * @param type_inst The type instanciation.
 * @param data_node The data node.
 *
 * @return True if no error occurs.
 */
bool write_native_string(compilation_state_t & state,
    const rec::type_instanciation_t & type_inst, const dat::node & data_node)
{
    if (data_node.get_kind() != dat::node::kind::string)
    {
        state.log.add_error("value must be a string.");
        return false;
    }

    const std::string & s = data_node.get_string();
    stream_write_binary(state.output, s.size());
    state.output.write(s.c_str(), s.size());

    return true;
}


/**
 * Decompiles a binary file, following a recipe file.
 *
 * @param state Decompilation state.
 * @param def_path Path to the recipe file.
 * @param include_directories Directories in which the recipe files may be
 *        searched.
 */
bool read_data(decompilation_state_t & state,
    const file_indication_t & def_fi,
    const std::string & current_directory,
    const std::list<std::string> & include_directories)
{
    /* Open the recipe file and parse it. */
    recipe_or_data recipe_or_data;
    std::string def_path;

    if (!resolve_file_indication(def_fi, current_directory, include_directories,
        def_path, state.log))
    {
        state.log.add_error("Definition file not found.");
        return false;
    }

    parser::load_from_file(def_path, recipe_or_data, state.log);

    if (state.log.get_error_count())
    {
        state.log.add_error("Error in recipe file '" + def_path + "'.");
        return false;
    }

    /* Verify that the recipe file is really a recipe file and not a
     * data file. */
    if (recipe_or_data.is_data())
    {
        state.log.add_error("File '" + def_path
            + "' is not a recipe file.");

        return false;
    }

    rec::recipe & recipe = recipe_or_data.get_recipe();

    /* Handle included recipe files. */
    std::list< std::string > loaded_recipe_files;

    if (!merge_included_recipe_files(recipe,
        boost::filesystem::path(def_path).parent_path().string() + "/",
        include_directories, loaded_recipe_files, state.log))
    {
        state.log.add_error("Failed to include some files into main "
            "recipe.");

        return false;
    }

    if (state.log.get_error_count())
    {
        state.log.add_error("Error occured while parsing included recipe "
            "files.");

        return false;
    }

    /* At this point, the complete recipe has been parsed. */

    /* Populate the recipe and compile it. */
    populate_node(recipe.get_node());

    if (!recipe.compile(state.log))
        return false;

    if (state.log.get_error_count() != 0)
        return false;

    /* Write in the output the link to the recipe file. */
    boost::filesystem::path rel_def_path = make_relative(
        boost::filesystem::current_path(), def_path);

    state.output << "recipe " << def_fi.print() << ";" << std::endl
        << std::endl;

    /* Next process is similar to compilation. The recipe will be travelled
     * and for each data, the binary file will be read and the data (in text
     * format) will be generated in the output file.
     *
     * Note: each time data must be decompiled, we must verify that there is
     * enough bytes remaining in the binary file. */

    rec::type_instanciation_t root_structure_instanciation;
    root_structure_instanciation.set_type_node_ptr(recipe.get_node().get());
    read_structure(state, root_structure_instanciation);

    return true;
}


/**
 * Decompiles a structure.
 *
 * @param state Decompilation state.
 * @param type_inst The type instanciation.
 *
 * @return True if no error occurs.
 */
bool read_structure(decompilation_state_t & state,
    const rec::type_instanciation_t & type_inst)
{
    const rec::node* type_ptr = type_inst.get_type_ptr();

    /* Verify template argument count. */
    if (!check_template_parameter_count(type_inst, state.log)) return false;

    std::list<rec::node::sptr> template_types = type_ptr->query_nodes<std::list>
        (rec::node::kind::template_type);

    tti_stacker_t<
        std::list<rec::node::sptr>,
        rec::type_instanciation_t::parameter_list
    > stacker(
        state.tti_stack,
        template_types.begin(),
        template_types.end(),
        type_inst.get_parameters().begin(),
        type_inst.get_parameters().end()
    );

    /* Read the data of the inherited types first. */
    for (const rec::type_instanciation_t & inherited_type_inst :
        type_ptr->get_structure_data().heritance_list)
    {
        if (!read_type(state, inherited_type_inst))
        {
            state.log.add_error("failed to read inherited data of type '"
                + inherited_type_inst.print() + "'.");

            return false;
        }
    }

    /* For each member of the structure */
    for (rec::node::sptr node :
        type_ptr->query_nodes<std::list>(rec::node::kind::member))
    {
        const rec::member_data_t & member_data = node->get_member_data();
        bool has_default_value = member_data.has_default_value();

        bool node_is_optional =
            node->has_qualifier(rec::node::qualifier::optional);

        /* A node cannot be optional if it has a default value. */
        if (has_default_value && node_is_optional)
        {
            state.log.add_error("member '" + node->get_name() + "' cannot be "
                "optional and have a default value at the same time.");

            return false;
        }

        bool read_member = true;

        if (node_is_optional)
        {
            /* Read the binary flag which tells if the member has been written
             * or not into the file. */
            bool data_is_defined;

            if (!read_native_type(state, data_is_defined))
            {
                state.log.add_error("failed to read optional flag of member '"
                    + node->get_name() + "'.");

                return false;
            }

            if (!data_is_defined)
                read_member = false;
        }

        if (has_default_value)
        {
            /* A default value is defined for this member. To avoid writing
             * unecessary data in the output data file, we must check if the
             * next bytes in the input binary stream are equal to the default
             * value of the member. */

            compilation_log_t default_value_compilation_log;

            std::stringstream default_value_stream(std::ios::in | std::ios::out
                | std::ios::binary);

            compilation_state_t default_value_compilation_state = {
                default_value_stream,state.tti_stack,
                default_value_compilation_log };

            if (!write_node(default_value_compilation_state,
                member_data.type_instanciation,
                *(member_data.default_value_node)))
            {
                state.log.add_error("failed to compile default value for "
                    "member '" + node->get_name() + "'.");

                return false;
            }

            /* Once the default value data has been built, we compare it with
             * the input binary stream. */

            default_value_stream.seekg(0, std::ios::end);
            size_t default_value_size = default_value_stream.tellg();
            default_value_stream.seekg(0, std::ios::beg);

            if (state.size >= default_value_size)
            {
                size_t start = state.input.tellg();
                bool identical = true;

                for (size_t i = 0; (i < default_value_size) && identical; ++i)
                {
                    char a, b;
                    a = state.input.get();
                    b = default_value_stream.get();
                    identical = (a == b);
                }

                if (identical)
                {
                    read_member = false;
                    state.size -= default_value_size;
                }
                else
                {
                    state.input.seekg(start);
                }
            }
        }

        if (read_member)
        {
            state.write_indent();
            state.output << node->get_name() << " = ";

            if (!read_type(state, member_data.type_instanciation))
            {
                state.log.add_error("failed to read data of member '"
                    + node->get_name() + "'.");

                return false;
            }

            state.output << ";" << std::endl;
        }
    }

    return true;
}


/**
 * Decompiles a variant.
 *
 * @param state Decompilation state.
 * @param type_inst The type instanciation.
 *
 * @return True if no error occurs.
 */
bool read_variant(decompilation_state_t & state,
    const rec::type_instanciation_t & type_inst)
{
    /* Get the variant type index. */
    unsigned int member_index;

    if (!read_native_type(state, member_index))
        return false;

    std::vector<rec::node::sptr> members = type_inst.get_type_ptr()
        ->query_nodes<std::vector>(rec::node::kind::member);

    /* Verify that the index has a correct value. */
    if (member_index >= members.size())
        return false;

    /* Get the type which is set for the variant. */
    rec::node::sptr member = members[member_index];
    const std::string & member_name = member->get_name();

    /* Member must not be marked as optional (this does not affect the
     * decompilation process, but it seems legit to show the error). */
    if (member->has_qualifier(rec::node::qualifier::optional))
    {
        state.log.add_error("variant member '" + member_name
            + "' cannot be marked as optional since it belongs to a variant.");

        return false;
    }

    /* Verify that the member has no default value (this does not affect the
     * decompilation process, but it seems legit to show the error). */
    if (member->get_member_data().has_default_value())
    {
        state.log.add_error("variant member '" + member_name
            + "' cannot have a default value since it belongs to a variant.");

        return false;
    }

    state.output << member_name << ": ";

    if (!read_type(state, member->get_member_data().type_instanciation))
        return false;

    return true;
}


/**
 * Decompiles an array.
 *
 * @param state Decompilation state.
 * @param type_inst The type instanciation.
 * @param dim Dimension index.
 *
 * @return True if no error occurs.
 */
bool read_array_dim(decompilation_state_t & state,
    const rec::type_instanciation_t & type_inst, size_t dim)
{
    /* Get array data */
    const rec::array_data_t & array_data =
        type_inst.get_type_ptr()->get_array_data();

    size_t dim_count = array_data.dims.size();
    bakery_assert_debug(dim < dim_count);
    size_t dim_size = array_data.dims[dim];
    bool last_dim = (dim + 1 == dim_count);

    if (dim_size == 0)
    {
        /* Array is dynamic. Read the written element count from the binary
         * input file. */
        if (!read_native_type(state, dim_size))
            return false;
    }

    state.output << std::endl;
    state.write_indent();
    state.indent_level++;
    state.output << "{" << std::endl;

    for (size_t i = 0; i < dim_size; ++i)
    {
        state.write_indent();

        if (last_dim)
        {
            if (!read_type(state, array_data.type_instanciation))
                return false;
        }
        else
        {
            if (!read_array_dim(state, type_inst, dim + 1))
                return false;
        }

        if (i + 1 < dim_size)
            state.output << ",";

        state.output << std::endl;
    }

    state.indent_level--;
    state.write_indent();
    state.output << "}";

    return true;
}


/**
 * Decompiles a typedef.
 *
 * @param state Decompilation state.
 * @param type_inst The type instanciation.
 *
 * @return True if no error occurs.
 */
bool read_typedef(decompilation_state_t & state,
    const rec::type_instanciation_t & type_inst)
{
    if (!check_template_parameter_count(type_inst, state.log))
        return false;

    const rec::node* type_ptr = type_inst.get_type_ptr();

    /* Push in the tty stack the instancied types for template types. As we use
     * a tti_stacker, we are sure the elements will be poped correctly when this
     * function returns. */
    std::list<rec::node::sptr> template_types = type_ptr->query_nodes<std::list>
        (rec::node::kind::template_type);

    tti_stacker_t<
        std::list<rec::node::sptr>,
        rec::type_instanciation_t::parameter_list
    > stacker(
        state.tti_stack,
        template_types.begin(),
        template_types.end(),
        type_inst.get_parameters().begin(),
        type_inst.get_parameters().end()
    );

    return read_type(state, type_ptr->get_typedef_data().type_instanciation);
};


/**
 * Decompiles an enumeration.
 *
 * @param state Decompilation state.
 * @param type_inst The type instanciation.
 *
 * @return True if no error occurs.
 */
bool read_enum(decompilation_state_t & state,
    const rec::type_instanciation_t & type_inst)
{
    /* Template argument count must be equal to zero. */
    if (type_inst.get_parameters().size() != 0)
    {
        state.log.add_error("an enumeration cannot have template parameters.");
        return false;
    }

    /* Read the value from the binary file. */
    int enum_value;

    if (!read_native_type(state, enum_value))
        return false;

    /* Try to match one of the enumeration members. */
    bool matched = false;

    for (rec::node::sptr n : type_inst.get_type_ptr()->query_nodes<std::list>
        (rec::node::kind::enum_value))
    {
        const enum_value_data_t & evd = n->get_enum_value_data();

        if (evd.value == enum_value)
        {
            matched = true;
            state.output << n->get_name();
            break;
        }
    }

    if (!matched)
        state.output << str::from(enum_value);

    return true;
}


/**
 * Read a type from an input file and write the decompiled data.
 *
 * @param state Decompilation state.
 * @param type_inst Type instanciation.
 *
 * @return True if the data could be decompiled, false otherwise.
 */
bool read_type(decompilation_state_t & state,
    const rec::type_instanciation_t & type_inst)
{
    switch (type_inst.get_type_ptr()->get_kind())
    {
        case rec::node::kind::structure:
        {
            state.output << std::endl;
            state.write_indent();
            state.output << "{" << std::endl;
            state.indent_level++;

            if (!read_structure(state, type_inst))
                return false;

            state.indent_level--;
            state.write_indent();
            state.output << "}";
            return true;
        }

        case rec::node::kind::variant:
            return read_variant(state, type_inst);

        case rec::node::kind::array:
            return read_array_dim(state, type_inst, 0);

        case rec::node::kind::typedef_:
            return read_typedef(state, type_inst);

        case rec::node::kind::enum_:
            return read_enum(state, type_inst);

        case rec::node::kind::native:
            return read_native(state, type_inst);

        case rec::node::kind::template_type:
            return read_template_type(state, type_inst);

        default: bakery_unexpected_case();
    }
}


/**
 * Decompiles a template type.
 *
 * @param state Decompilation state.
 * @param type_inst Type instanciation.
 *
 * @return True if the data could be decompiled, false otherwise.
 */
bool read_template_type(decompilation_state_t & state,
    const rec::type_instanciation_t & type_inst)
{
    /* template type must be matched to a real type. We explore tti stack to
     * find the latest instanciation of the template. */
    tti_stack_t::const_reverse_iterator it = std::find_if
    (
        state.tti_stack.rbegin(),
        state.tti_stack.rend(),
        [&](const std::pair<rec::node::sptr, rec::type_instanciation_t> & p)
            -> bool
        {
            return p.first.get() == type_inst.get_type_ptr();
        }
    );

    bakery_assert(it != state.tti_stack.rend());
    return read_type(state, it->second);
}


/**
 * Decompiles a native type.
 *
 * @param state Decompilation state.
 * @param type_inst Type instanciation.
 *
 * @return True if the data could be decompiled, false otherwise.
 */
bool read_native(decompilation_state_t & state,
    const rec::type_instanciation_t & type_inst)
{
    switch (type_inst.get_type_ptr()->get_native_data().get_class())
    {
        case rec::native_data_t::class_t::bool_:
            return read_native_bool(state, type_inst);

        case rec::native_data_t::class_t::int_:
        case rec::native_data_t::class_t::char_:
        case rec::native_data_t::class_t::short_:
            return read_native_integer(state, type_inst);

        case rec::native_data_t::class_t::string:
            return read_native_string(state, type_inst);

        case rec::native_data_t::class_t::float_:
        case rec::native_data_t::class_t::double_:
            return read_native_floating(state, type_inst);

        case rec::native_data_t::class_t::pair:
        case rec::native_data_t::class_t::tuple:
            return read_native_tuple(state, type_inst);

        case rec::native_data_t::class_t::list:
            return read_native_list(state, type_inst);

        case rec::native_data_t::class_t::map:
            return read_native_map(state, type_inst);

        default: bakery_unexpected_case();
    }
}


/**
 * Decompiles a native boolean.
 *
 * @param state Decompilation state.
 * @param type_inst Type instanciation.
 *
 * @return True if the data could be decompiled, false otherwise.
 */
bool read_native_bool(decompilation_state_t & state,
    const rec::type_instanciation_t & type_inst)
{
    bool dest;

    if (!read_native_type(state, dest))
        return false;

    state.output << (dest?"true":"false");
    return true;
}


/**
 * Decompiles a native integer.
 *
 * @param state Decompilation state.
 * @param type_inst Type instanciation.
 *
 * @return True if the data could be decompiled, false otherwise.
 */
bool read_native_integer(decompilation_state_t & state,
    const rec::type_instanciation_t & type_inst)
{
    const rec::native_data_t::class_t::value the_class =
        type_inst.get_type_ptr()->get_native_data().get_class();

    switch (the_class)
    {
        case rec::native_data_t::class_t::int_:
        {
            int dest;

            if (!read_native_type(state, dest))
                return false;

            state.output << str::from(dest);
            return true;
        }

        case rec::native_data_t::class_t::char_:
        {
            char dest;

            if (!read_native_type(state, dest))
                return false;

            state.output << str::from(dest);
            return true;
        }

        case rec::native_data_t::class_t::short_:
        {
            short dest;

            if (!read_native_type(state, dest))
                return false;

            state.output << str::from(dest);
            return true;
        }

        default: bakery_unexpected_case();
    }
}


/**
 * Decompiles a native floating value.
 *
 * @param state Decompilation state.
 * @param type_inst Type instanciation.
 *
 * @return True if the data could be decompiled, false otherwise.
 */
bool read_native_floating(decompilation_state_t & state,
    const rec::type_instanciation_t & type_inst)
{
    switch (type_inst.get_type_ptr()->get_native_data().get_class())
    {
        case rec::native_data_t::class_t::float_:
        {
            float dest;

            if (!read_native_type(state, dest))
                return false;

            state.output << str::floating_to_string(dest);
            return true;
        }

        case rec::native_data_t::class_t::double_:
        {
            double dest;

            if (!read_native_type(state, dest))
                return false;

            state.output << str::floating_to_string(dest);
            return true;
        }

        default: bakery_unexpected_case();
    }
}


/**
 * Decompiles a string.
 *
 * @param state Decompilation state.
 * @param type_inst Type instanciation.
 *
 * @return True if the data could be decompiled, false otherwise.
 */
bool read_native_string(decompilation_state_t & state,
    const rec::type_instanciation_t & type_inst)
{
    /* Read the size of the string. */
    size_t len;

    if (!read_native_type(state, len))
        return false;

    /* Verify that the input stream has enough bytes remaining. */
    if (len > state.size)
    {
        state.log.add_error("eof before end of string.");
        return false;
    }

    std::string str;
    str.reserve(len);

    /* Note: this string extraction is not very efficient, it may be improved,
     * later... */
    for (size_t i = 0; i < len; ++i)
        str.push_back(state.input.get());

    /* Decrement the bytes read from the string. Do not count the size of the
     * string since it is automatically done in the read_native_type
     * function. */
    state.size -= len;

    /* Note: for the moment, the string is written "as is" in the output file -
     * but it should be escaped if there are any special character. */
    state.output << '"' << str << '"';

    return true;
}


/**
 * Decompiles a native pair.
 *
 * @param state Decompilation state.
 * @param type_inst Type instanciation.
 *
 * @return True if the data could be decompiled, false otherwise.
 */
bool read_native_tuple(decompilation_state_t & state,
    const rec::type_instanciation_t & type_inst)
{
    state.output << std::endl;
    state.write_indent();
    state.indent_level++;
    state.output << "{" << std::endl;

    bool first_element = true;

    for (const rec::type_instanciation_t & i: type_inst.get_parameters())
    {
        if (first_element)
            first_element = false;
        else
            state.output << "," << std::endl;

        state.write_indent();

        if (!read_type(state, i))
            return false;
    }

    state.output << std::endl;
    state.indent_level--;
    state.write_indent();
    state.output << "}";

    return true;
};


/**
 * Decompiles a native list.
 *
 * @param state Decompilation state.
 * @param type_inst Type instanciation.
 *
 * @return True if the data could be decompiled, false otherwise.
 */
bool read_native_list(decompilation_state_t & state,
    const rec::type_instanciation_t & type_inst)
{
    /* Read the size of the list. */
    size_t size;

    if (!read_native_type(state, size))
        return false;

    /* Get the type of the list elements. */
    const rec::type_instanciation_t::parameter_list & template_parameters =
        type_inst.get_parameters();

    if (template_parameters.size() != 1)
    {
        state.log.add_error("wrong number of template parameters, 1 expected, "
            "got " + str::from(template_parameters.size()) + ".");

        return false;
    }

    state.output << std::endl;
    state.write_indent();
    state.indent_level++;
    state.output << "{" << std::endl;

    while (size--)
    {
        state.write_indent();

        if (!read_type(state, template_parameters.front()))
            return false;

        if (size)
            state.output << ",";

        state.output << std::endl;
    }

    state.indent_level--;
    state.write_indent();
    state.output << "}";

    return true;
}


/**
 * Decompiles a native map.
 *
 * @param state Decompilation state.
 * @param type_inst Type instanciation.
 *
 * @return True if the data could be decompiled, false otherwise.
 */
bool read_native_map(decompilation_state_t & state,
    const rec::type_instanciation_t & type_inst)
{
    /* Read the size of the map. */
    size_t size;

    if (!read_native_type(state, size))
        return false;

    /* Get the type of the map elements. */
    const rec::type_instanciation_t::parameter_list & template_parameters =
        type_inst.get_parameters();

    if (template_parameters.size() != 2)
    {
        state.log.add_error("wrong number of template parameters, 2 expected, "
            "got " + str::from(template_parameters.size()) + ".");

        return false;
    }

    state.output << std::endl;
    state.write_indent();
    state.indent_level++;
    state.output << "{" << std::endl;

    while (size--)
    {
        state.write_indent();

        if (!read_type(state, template_parameters[0]))
            return false;

        state.output << " = ";

        if (!read_type(state, template_parameters[1]))
            return false;

        if (size)
            state.output << ",";

        state.output << std::endl;
    }

    state.indent_level--;
    state.write_indent();
    state.output << "}";

    return true;
}


/**
 * Creates the nodes for all the primitive types.
 *
 * @param node The node to populate.
 */
void populate_node(rec::node::sptr node)
{
    node->add_child(
        rec::util::make_native_node(
            "bool",
            rec::native_data_t::class_t::bool_
        )
    );

    node->add_child(
        rec::util::make_native_node(
            "char",
            rec::native_data_t::class_t::char_
        )
    );

    node->add_child(
        rec::util::make_native_node(
            "short",
            rec::native_data_t::class_t::short_
        )
    );

    node->add_child(
        rec::util::make_native_node(
            "int",
            rec::native_data_t::class_t::int_
        )
    );

    node->add_child(
        rec::util::make_native_node(
            "float",
            rec::native_data_t::class_t::float_
        )
    );

    node->add_child(
        rec::util::make_native_node(
            "double",
            rec::native_data_t::class_t::double_
        )
    );

    node->add_child(
        rec::util::make_native_node(
            "string",
            rec::native_data_t::class_t::string
        )
    );

    node->add_child(
        rec::util::make_native_node(
            "pair",
            rec::native_data_t::class_t::pair
        )
    );

    node->add_child(
        rec::util::make_native_node(
            "tuple",
            rec::native_data_t::class_t::tuple
        )
    );

    node->add_child(
        rec::util::make_native_node(
            "list",
            rec::native_data_t::class_t::list
        )
    );

    node->add_child(
        rec::util::make_native_node(
            "map",
            rec::native_data_t::class_t::map
        )
    );

}


/**
 * Synthesizes a mpf_class from a dat::floating. Eventual errors are stored in
 * the compilation log.
 *
 * @param out The mpf_class destination.
 * @param floating The floating value to convert.
 * @param log The object in which error messages will be pushed.
 *
 * @return True if no error occurs.
 */
bool floating_to_mpf(
    mpf_class & out,
    const dat::floating & floating,
    compilation_log_t & log)
{
    /* Get the integer value */
    mpf_class integer_part(0);

    if (floating.has_integer_part())
        integer_part.set_str(floating.get_integer_string(), 10);

    /* Get the decimal value */
    mpf_class decimal_part(0);

    if (floating.has_decimal_part())
    {
        decimal_part.set_str(floating.get_decimal_string(), 10);
        decimal_part /= make_mpz_10_power(floating.get_decimal_string().size());
    }

    /* Get the exponent value. */
    mpz_class exponent_part(0);

    if (floating.has_exponent_part())
        exponent_part.set_str(floating.get_exponent_string(), 10);

    if (!exponent_part.fits_sint_p())
    {
        log.add_error("exponent is too big.");
        return false;
    }

    /* If there are no integer_part and no decimal_part, then we can consider
     * the floating value is 1.0. This allow to consider the expression "e-9" as
     * "1.0e-9" and not 0. */
    if ((!floating.has_integer_part()) && (!floating.has_decimal_part()))
        out = 1;
    else
        out = integer_part + decimal_part;

    /* Consider the sign. */
    if (floating.get_negative())
        out = -out;

    /* Apply the exponent. */
    if (exponent_part > 0)
    {
        out *= make_mpz_10_power(exponent_part.get_ui());
    }
    else if (exponent_part < 0)
    {
        mpz_class neg = -exponent_part;
        out /= make_mpz_10_power(neg.get_ui());
    }

    return true;
}


/**
 * Verifies the number of template arguments of a type instanciation.
 *
 * @param type_inst Type instanciation.
 * @param log Objects in which error messages will be pushed.
 *
 * @return True if template argument count if right, false otherwise.
 */
bool check_template_parameter_count(
    const rec::type_instanciation_t & type_inst, compilation_log_t & log)
{
    std::list<rec::node::sptr> template_types = type_inst.get_type_ptr()
        ->query_nodes<std::list>(rec::node::kind::template_type);

    size_t template_parameters_size = type_inst.get_parameters().size();

    if (template_types.size() != template_parameters_size)
    {
        log.add_error("wrong number of template parameters, "
            + str::from(template_types.size())
            + " expected, got "
            + str::from(template_parameters_size)
            + ".");

        return false;
    }

    return true;
}


/**
 * Loads included recipe files and merge their content. Since included files
 * may also include other files, this method is recursive.
 *
 * @param def Reference to the recipe containing the included files.
 * @param current_directory Current directory of the recipe, this path will
 *        be used to calculate the relative paths of included files.
 * @param include_directories Directories in which the files may be searched.
 * @param loaded_defs List of already loaded recipe files. This is to avoid
 *        loading multiple times the same files (which would cause compilation
 *        error due to symbol redefinition) and circular references. It also
 *        avoid to use some #idndef #define C-like triks.
 * @param log The object in which error messages will be pushed.
 */
bool merge_included_recipe_files(
    rec::recipe & def,
    const std::string & current_directory,
    const std::list<std::string> & include_directories,
    std::list<std::string> & loaded_defs,
    compilation_log_t & log)
{
    /* Retrieve the liste of included files by recipe. */
    const std::list<file_indication_t> & included_defs = def.get_included_files();

    /* For each included file. */
    for (const file_indication_t & included_def : included_defs)
    {
        /* Solve path. */
        std::string p;

        if (!resolve_file_indication(included_def, current_directory,
            include_directories, p, log))
        {
            log.add_error("Included file not found.");
            return false;
        }

        /* Find if this recipe file has already been loaded. */
        if (
            std::find_if(
                loaded_defs.begin(),
                loaded_defs.end(),
                [&](const std::string & x) -> bool
                {
                    return boost::filesystem::equivalent(p, x);
                }
            ) != loaded_defs.end() )
        {
            /* File already loaded. */
        }
        else
        {
            /* File not yet loaded
             * Store the file path to avoid reloading the same recipe file
             * later. */
            loaded_defs.push_back(p);

            /* Loads the recipe. */
            recipe_or_data recipe_or_data;
            parser::load_from_file(p, recipe_or_data, log);

            if (log.get_error_count() != 0)
            {
                log.add_error("Failed to parse included recipe '" + p +
                    "'.");

                return false;
            }

            /* Check that the parser produced a rec::recipe. */
            if (!recipe_or_data.is_recipe())
            {
                log.add_error("Included file '" + p
                    + "' could not be parsed as a recipe.");

                return false;
            }

            rec::recipe & def_2 = recipe_or_data.get_recipe();

            /* Included recipe may also include recipes files. */
            if (!merge_included_recipe_files(def_2,
                boost::filesystem::path(p).parent_path().string() + "/",
                include_directories, loaded_defs, log))
            {
                return false;
            }

            /* Merge the recipe content.
             * This takes all namespaces and types. Member nodes are
             * discarded. */
            def.get_node()->merge_types(def_2.get_node());
        }
    }

    return true;
}


/**
 * @return The real path from a file indication. If fi path is absolute, then
 *         its path is directly returned.
 *
 * @param fi The file indication.
 * @param current_directory Current directory used to compute the relatives
 *        paths.
 * @param include_directories Directories in which the files may be searched.
 * @param dest Reference on the path where the result should be written.
 * @param log The object in which error messages will be pushed.
 */
bool resolve_file_indication(
    const file_indication_t & fi,
    const std::string & current_directory,
    const std::list<std::string> & include_directories,
    std::string & dest,
    compilation_log_t & log)
{
    boost::filesystem::path the_path(fi.get_path());

    /* Whenever path is absolute or relative, if it is a complete path, we just
     * return it. These are examples :
     * include </home/pouet/toto.def>
     * include "/home/pouet/toto.def" */
    if (the_path.is_complete())
    {
        if (boost::filesystem::exists(the_path))
        {
            dest = the_path.string();
            return true;
        }
        else
        {
            /* File does not exists. */
            log.add_error("File \"" + the_path.string() + "\" does not exists.");
            return false;
        }
    }

    if (!fi.get_absolute())
    {
        boost::filesystem::path p = current_directory + fi.get_path();

        if (boost::filesystem::exists(p))
        {
            dest = p.string();
            return true;
        }
        else
        {
            /* File does not exists. */
            log.add_error("File \"" + p.string() + "\" does not exists.");
            return false;
        }
    }
    else
    {
        for (const std::string & dir: include_directories)
        {
            boost::filesystem::path p = boost::filesystem::path(dir) / fi.get_path();

            if (boost::filesystem::exists(p))
            {
                dest = p.string();
                return true;
            }
        }

        /* path not found. */
        log.add_error("Cannot find file <" + fi.get_path() + ">.");
        return false;
    }
}


/**
 * Creates a mpz_class equal to 10^power.
 *
 * @param power The power.
 */
mpz_class make_mpz_10_power(unsigned int power)
{
    /* 12/06/2010
     * There is no C++ binding for the function mpz_pow_ui, so we do the
     * calculation with the classic C interface, and then convert the result to
     * a C++ class mpz_class. */
    mpz_t x;
    mpz_init(x);
    mpz_ui_pow_ui(x, 10, power);
    mpz_class result(x);
    mpz_clear(x);
    return result;
}


} /* namespace compiler */
} /* namespace bakery */

