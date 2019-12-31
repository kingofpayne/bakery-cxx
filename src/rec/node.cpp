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


#include "node.hpp"
#include "../assertions.hpp"
#include "../str.hpp"
#include "../util.hpp"
#include <boost/bind.hpp>
#include <cstdio>
#include <gmpxx.h>
#include <iostream>
#include <limits>


namespace bakery {
namespace rec {


const std::string node::kind::strings[] =
{
    "none",
    "namespace",
    "structure",
    "variant",
    "array",
    "typedef",
    "enum",
    "enum_value",
    "member",
    "native",
    "template_type"
};


const std::string node::qualifier::strings[] =
{
    "unsigned",
    "optional"
};


/**
 * Constructor.
 */
node::node():
    kind(kind::none),
    children(),
    parent(0),
    scope_node(0),
    name(),
    qualifiers()
{
    init_data();
}


/**
 * Constructor
 *
 * @param _kind kind of node.
 */
node::node(kind::value _kind):
    kind(_kind),
    children(),
    parent(0),
    scope_node(0),
    name(),
    qualifiers()
{
    bakery_assert_message(kind::Wrapper::has(_kind), "Invalid enumeration value.");
    init_data();
}


/**
 * @return The kind of the node.
 */
node::kind::value node::get_kind() const
{
    return kind;
}


/**
 * @returns The name of the node. May be empty.
 */
const std::string & node::get_name() const
{
    return name;
}


/**
 * @return A string representing the content of the node.
 */
std::string node::print() const
{
    std::string r;

    if(name.empty())
        r = "anonymous";
    else
        r = name;

    r += " is " + kind::Wrapper::to_string(kind);

    if(!children.empty())
    {
        r += " {\n";
        std::string r2;
        bool first = true;
        std::for_each(
            children.begin(),
            children.end(),
            [&](const sptr & x)
            {
                if (first)
                    first = false;
                else
                    r2 += "\n";

                r2 += x->print();
            }
        );

        r += indent(r2);
        r += "\n}";
    }

    switch(kind)
    {
        case kind::native:
            r += " (" + get_native_data().print() + ")";
            break;
        case kind::member:
            r += " (" + get_member_data().print() += ")";
            break;
        default:;
    }

    return r;
}


/**
 * @return Const reference to the typedef data of the node.
 *
 * This function may only be called when the kind of the node is kind::typedef_.
 */
const typedef_data_t & node::get_typedef_data() const
{
    bakery_assert(kind == kind::typedef_);
    return boost::get<typedef_data_t>(data);
}


/**
 * @return Const reference to the member data of the node.
 *
 * This function may only be called when the kind of the node is kind::member.
 */
const member_data_t & node::get_member_data() const
{
    bakery_assert(kind == kind::member);
    return boost::get<member_data_t>(data);
}


/**
 * @return Const reference to the native data of the node.
 *
 * This function may only be called when the kind of the node is kind::native.
 */
const native_data_t & node::get_native_data() const
{
    bakery_assert(kind == kind::native);
    return boost::get<native_data_t>(data);
}


/**
 * @return Const reference to the enum value data of the node.
 *
 * This function may only be called when the kind of the node is
 * kind::enum_value.
 */
const enum_value_data_t & node::get_enum_value_data() const
{
    bakery_assert(kind == kind::enum_value);
    return boost::get<enum_value_data_t>(data);
}


/**
 * @return Reference to the enum value data of the node.
 *
 * This function may only be called when the kind of the node is
 * kind::enum_value.
 */
enum_value_data_t & node::get_enum_value_data()
{
    bakery_assert(kind == kind::enum_value);
    return boost::get<enum_value_data_t>(data);
}


/**
 * @return Const reference to the structure data of the node.
 *
 * This function may only be called when the kind of the node is
 * kind::structure.
 */
const structure_data_t & node::get_structure_data() const
{
    bakery_assert(kind == kind::structure);
    return boost::get<structure_data_t>(data);
}


/**
 * @return Reference to the structure data of the node.
 *
 * This function may only be called when the kind of the node is
 * kind::structure.
 */
structure_data_t & node::get_structure_data()
{
    bakery_assert(kind == kind::structure);
    return boost::get<structure_data_t>(data);
}


/**
 * @return Const reference to the array data of the node.
 *
 * This function may only be called when the kind of the node is kind::array.
 */
const array_data_t & node::get_array_data() const
{
    bakery_assert(kind == kind::array);
    return boost::get<array_data_t>(data);
}


/**
 * @return Reference to the array data of the node.
 *
 * This function may only be called when the kind of the node is kind::array.
 */
array_data_t & node::get_array_data()
{
    bakery_assert(kind == kind::array);
    return boost::get<array_data_t>(data);
}


/**
 * @return True if the node has the given qualifier.
 *
 * @param q Tested qualifier.
 */
bool node::has_qualifier(qualifier::value q) const
{
    return std::find(qualifiers.begin(), qualifiers.end(), q)
        != qualifiers.end();
}


/**
 * @return True if the node represents a type.
 */
bool node::is_type() const
{
    switch(kind)
    {
        case kind::structure:
        case kind::variant:
        case kind::array:
        case kind::typedef_:
        case kind::enum_:
        case kind::native:
        case kind::template_type:
            return true;

        default:
            return false;
    }
}


/**
 * Finds a node in the direct children of the node.
 *
 * @param name Name of the node.
 *
 * @return A shared_ptr to the node, or a empty shared_ptr if no node is found.
 */
node::sptr node::find_node(const std::string & name) const
{
    sptr_list::const_iterator it = std::find_if(
        children.begin(),
        children.end(),
        boost::bind(
            std::equal_to<std::string>(),
            boost::ref(name),
            bind(&node::get_name,_1)
        )
    );

    if(it != children.end())
        return *it;
    else
        // Return an empty shared_ptr
        return sptr();
}


/**
 * @return Const referenc on the list of children nodes (which are all shared
 *         pointers which can be modified)
 */
const node::sptr_list & node::get_children() const
{
    return children;
}


/**
 * Defines the kind of the node.
 *
 * @param value kind of the node. Must be a member of the enumeration
 *        node::kind.
 */
void node::set_kind(kind::value value)
{
    bakery_assert_message(kind::Wrapper::has(value), "Invalid enumeration value.");
    kind = value;
    init_data();
}


/**
 * Defines the name of the node.
 *
 * @param value Name, may be an empty string.
 */
void node::set_name(const std::string & value)
{
    name = value;
}


/**
 * Adds a node as child. This automatically defines the members parent and
 * scope_node to the parent pointer value.
 *
 * @param n The node.
 */
void node::add_child(sptr n)
{
    children.push_back(n);
    bakery_assert(n->parent == 0);
    n->parent = this;
    n->scope_node = this;
}


/**
 * Defines the scope node. This function cannot be called if node has already a
 * parent.
 *
 * @param value The scope node.
 */
void node::set_scope_node(node* value)
{
    bakery_assert(parent == 0);
    scope_node = value;
}


/**
 * Defines the structure data of the node. This function may be called only if
 * the kind of the node is kind::structure.
 *
 * @param value The data.
 */
void node::set_structure_data(const structure_data_t & value)
{
    bakery_assert(kind == kind::structure);
    data = value;
}


/**
 * Defines the array data of the node. This function may be called only if the
 * kind of the node is kind::array.
 *
 * @param value The data.
 */
void node::set_array_data(const array_data_t & value)
{
    bakery_assert(kind == kind::array);
    data = value;
}


/**
 * Defines the typdef data of the node. This function may be called only if the
 * kind of the node is kind::typedef_.
 *
 * @param value The data.
 */
void node::set_typedef_data(const typedef_data_t & value)
{
    bakery_assert(kind == kind::typedef_);
    data = value;
}


/**
 * Defines the member data of the node. This function may be called only if the
 * kind of the node is kind::member.
 *
 * @param value The data.
 */
void node::set_member_data(const member_data_t & value)
{
    bakery_assert(kind == kind::member);
    data = value;
}


/**
 * Defines the native data of the node. This function may be called only if the
 * kind of the node is kind::native.
 *
 * @param value The data.
 */
void node::set_native_data(const native_data_t & value)
{
    bakery_assert(kind == kind::native);
    data = value;
}


/**
 * Defines the enum value data of the node. This function may only be called if
 * the kind of the node is kind::enum_value.
 *
 * @param value The data.
 */
void node::set_enum_value_data(const enum_value_data_t & value)
{
    bakery_assert(kind == kind::enum_value);
    data = value;
}


/**
 * Adds a qualifier.
 *
 * @param q The qualifier.
 */
void node::add_qualifier(node::qualifier::value q)
{
    bakery_assert_debug(qualifier::Wrapper::has(q));
    qualifiers.push_back(q);
}


/**
 * Transfert the children of b to this.
 *
 * @param b Second node from which nodes are extracted.
 */
void node::merge(const sptr & b)
{
    for (node::sptr & n: b->children)
    {
        n->parent = this;
        n->scope_node = this;
        children.push_back(n);
    }

    b->children.clear();
}


/**
 * Moves the types and namespaces nodes of b to this. Member nodes are
 * discarded. This function is used when loading an included recipe file.
 *
 * @param b Second node from which nodes are moved.
 */
void node::merge_types(const sptr & b)
{
    for (node::sptr & n: b->children)
    {
        if (n->get_kind() != kind::member)
        {
            n->parent = this;
            n->scope_node = this;
            children.push_back(n);
        }
    }

    b->children.clear();
}


/**
 * Compiles the tree node.
 *
 * This function is recursive.
 *
 * @param compil_status The object in which error messages will be pushed.
 *
 * @return False if a compilation error occurs.
 */
bool node::compile(compilation_log_t & compil_status)
{
    // There may be multiple namespace node having the same name (so they
    // represent the same namespace). The merge_namespaces must have been called
    // before any compilation.

    // Verify that there are no two child node with the same identifier. The
    // parser cannot check that, or it may be complicated... whatever its
    // not the job of the parser !
    if(!check_children_names(compil_status))
        return false;

    // Call compilation of the children
    sptr_list::iterator it_end = children.end();
    for(sptr_list::iterator it = children.begin(); it != it_end; ++it)
        if(!(*it)->compile(compil_status))
            return false;

    // Self compile
    switch(kind)
    {
        case kind::none:
        case kind::namespace_:
        case kind::variant:
        case kind::native:
        case kind::template_type:
            break;

        case kind::structure:
            if(!compile_as_structure(compil_status))
                return false;
            break;

        case kind::array:
            if(!compile_as_array(compil_status))
                return false;
            break;

        case kind::typedef_:
            if(!compile_as_typedef(compil_status))
                return false;
            break;

        case kind::member:
            if(!compile_as_member(compil_status))
                return false;
            break;

        case kind::enum_:
            if(!compile_as_enum(compil_status))
                return false;
            break;

        case kind::enum_value:
            if(!compile_as_enum_value(compil_status))
                return false;
            break;

        default:
            bakery_unexpected_case();
    }

    return true;
}


/**
 * Initialize the member data depending on the kind of the node.
 */
void node::init_data()
{
    switch(kind)
    {
        case kind::none:
        case kind::namespace_:
        case kind::variant:
        case kind::enum_:
        case kind::template_type:
            break;

        case kind::array:
            data = array_data_t();
            break;

        case kind::typedef_:
            data = typedef_data_t();
            break;

        case kind::enum_value:
            data = enum_value_data_t();
            break;

        case kind::member:
            data = member_data_t();
            break;

        case kind::native:
            data = native_data_t();
            break;

        case kind::structure:
            data = structure_data_t();
            break;

        default:
            bakery_unexpected_case();
    }
}


/**
 * Merge the namespaces nodes having the same name (and thus representing the
 * same namespace).
 */
void node::merge_namespaces()
{
    if(children.size() <= 1)
        return;

    sptr_list::iterator it_end = children.end();

    for(sptr_list::iterator it1 = children.begin();
        it1 != it_end;
        ++it1)
    {
        if( (*it1)->get_kind() == kind::namespace_ )
        {
            sptr_list::iterator it2 = it1;
            it2++;

            // Try to find a similare namespace node
            while(it2 != it_end)
            {
                if(
                    ( (*it2)->get_kind() == kind::namespace_ )
                    && ( (*it1)->get_name() == (*it2)->get_name() )
                ){
                    // Ok, these namespaces are similar, merge them ! To do it,
                    // merge the second node into the first, and remove the
                    // second node from the children list.
                    (*it1)->merge(*it2);
                    sptr_list::iterator it_to_erase = it2;
                    it2++;
                    children.erase(it_to_erase);
                }
                else
                {
                    it2++;
                }
            }
        }
    }

    // Recursively, merge namespaces of children.
    std::for_each(
        children.begin(),
        children.end(),
        boost::bind(&node::merge_namespaces, _1)
    );
}


/**
 * Verifies that there are no two child node with the same (non empty) name.
 * This check must be performed once the namespace have been merged.
 *
 * @param compil_status The object in which error messages will be pushed.
 *
 * @return True if there are no collisions.
 */
bool node::check_children_names(compilation_log_t & compil_status) const
{
    bool result = true;

    const sptr_list::const_iterator it_end = children.end();
    for(sptr_list::const_iterator it1 = children.begin();
        it1 != it_end;
        ++it1)
    {
        sptr_list::const_iterator it2 = it1;
        for(it2++;
            it2 != it_end;
            ++it2)
        {
            const std::string & name1 = (*it1)->get_name();
            const std::string & name2 = (*it2)->get_name();

            if((!name1.empty()) && (name1 == name2))
            {
                compil_status.add_message(
                    compilation_message_t(
                        compilation_message_type_t::error,
                        "a node with the name '"
                            + (*it1)->get_name()
                            + "' is already declared."
                    )
                );
                result = false;
            }
        }
    }

    return result;
}


/**
 * Searches for a node given a path. The scope of the search is tree of the
 * nodes, starting from this node.
 *
 * @param path path to the node. The absolute flag of the path has no influence
 *        on the result of this function.
 *
 * @return The shared_ptr to the node, or an empty shared_ptr if there is no
 *         node with the given path.
 */
/*node::sptr node::solve_down(const path & path) const
{
    // Copy the path for local modifications
    path p = path;

    const path::token_list & tokens = path.get_tokens();

    // Verify that the path is not empty.
    if(tokens.empty())
        throw core::exceptions::invalid_parameter("path", "the path is empty.",
            HERE);

    // Search for a node with the first token as name.
    sptr first_node = find_node(tokens[0]);

    if((tokens.size() == 1) || (first_node.get() == 0))
    {
        return first_node;
    }

    p.pop_front();
    return first_node->solve_down(p);
}*/


/**
 * Searches for a type node given a path. The scope of the search is tree of the
 * nodes, starting from this node.
 *
 * @param path path to the node. The absolute flag of the path has no influence
 *        on the result of this function.
 *
 * @return The shared_ptr to the node, or an empty shared_ptr if there is no
 *         type node with the given path.
 */
node::sptr node::solve_type_down(const path & a_path) const
{
    // Copy the path for local modifications
    path p = a_path;

    const path::token_list & tokens = a_path.get_tokens();

    // Verify that the path is not empty.
    bakery_assert_message(!tokens.empty(), "path is empty.");

    // Search for a type node with the first token as name.
    // Note: we cache token[0] to avoid quering multiple time the element 0
    // during the search loop.
    const std::string & token_zero = tokens[0];

    // If there is only one token in the node, search verifies that the node is
    // a type. Otherwise, search applies on all nodes (the type node may be
    // included in a non-type node).
    sptr_list::const_iterator it;

    if(tokens.size() == 1)
    {
        it = std::find_if(
            children.begin(),
            children.end(),
            [&](const sptr & x) -> bool
            {
                return x->is_type() && (x->get_name() == token_zero);
            }
        );
    }
    else
    {
        it = std::find_if(
            children.begin(),
            children.end(),
            [&](const sptr & x) -> bool
            {
                return x->get_name() == token_zero;
            }
        );
    }

    if(it == children.end())
    {
        // Return null pointer to indicate search didn't find anything.
        return sptr();
    }
    else
    {
        if(tokens.size() == 1)
        {
            return *it;
        }
        else
        {
            p.pop_front();
            return (*it)->solve_type_down(p);
        }
    }
}


/**
 * Searches for a node given a path. The scope of search is the complete tree of
 * the nodes.
 *
 * @path path path to the node.
 *
 * @return The shared_ptr to the node, or an empty shared_ptr if there is no
 *         node with the given path.
 */
/*node::sptr node::solve(const path & path) const
{
    if(path.get_absolute())
    {
        if(scope_node == 0)
        {
            path p = path;
            p.set_absolute(false);
            return solve_down(p); // throw 1
        }
        else
        {
            return scope_node->solve(path); // throw 1
        }
    }
    else
    {
        sptr s = solve_down(path);
        if(s.get() == 0)
        {
            if(scope_node == 0)
            {
                return node::sptr();
            }
            else
            {
                return scope_node->solve(path); // throw 1
            }
        }
        else
        {
            return s;
        }
    }

}*/


/**
 * Searches for a type node given a path. The scope of search is the complete
 * tree of the nodes.
 *
 * @path path path to the node.
 *
 * @return The shared_ptr to the node, or an empty shared_ptr if there is no
 *         type node with the given path.
 */
node::sptr node::solve_type(const path & a_path) const
{
    if(a_path.get_absolute())
    {
        if(scope_node == 0)
        {
            path p = a_path;
            p.set_absolute(false);
            return solve_type_down(p);
        }
        else
        {
            return scope_node->solve_type(a_path);
        }
    }
    else
    {
        sptr s = solve_type_down(a_path);
        if(s.get() == 0)
        {
            if(scope_node == 0)
            {
                return node::sptr();
            }
            else
            {
                return scope_node->solve_type(a_path);
            }
        }
        else
        {
            return s;
        }
    }
}


/**
 * Compiles the node as a structure.
 *
 * @param compil_status The object in which error messages will be pushed.
 *
 * @return False if a compilation error occurs.
 *
 * Precondition:
 *
 * this->kind == kind::structure
 */
bool node::compile_as_structure(compilation_log_t & compil_status)
{
    for (type_instanciation_t & ti :
        boost::get<structure_data_t>(data).heritance_list)
    {
        if (!compile_type_instanciation(ti, compil_status, this))
        {
            compil_status.add_message(
                compilation_message_type_t::error,
                "failed to compile the herited type '"
                    + ti.print()
                    + "' of the node '"
                    + name
                    + "'."
                );

            return false;
        }
        else
        {
            // type has been solved. We need to verify that a structure can
            // heritate from this type: this type must be a structure or a
            // typedef.
            kind::value hkind = ti.get_type_ptr()->get_kind();
            bool correct_kind;

            switch(hkind)
            {
                case kind::structure:
                case kind::typedef_:
                    correct_kind = true;
                    break;

                default:
                    correct_kind = false;
            }

            if(!correct_kind)
            {
                compil_status.add_message(
                    compilation_message_type_t::error,
                    "the structure '"
                        + name
                        + "' cannot heritate from the type '"
                        + ti.print()
                        + "'."
                    );

                return false;
            }
        }
    }

    return true;
}


/**
 * Compiles the node as an array.
 *
 * @param compil_status The object in which error messages will be pushed.
 *
 * @return False if a compilation error occurs.
 *
 * Precondition:
 *
 * this->kind == kind::array
 */
bool node::compile_as_array(compilation_log_t & compil_status)
{
    array_data_t & ad = boost::get<array_data_t>(data);

    if (!compile_type_instanciation(ad.type_instanciation, compil_status, this))
    {
        compil_status.add_error("failed to compile array type of node '" + name
            + "'.");

        return false;
    }

    return true;
}


/**
 * Compiles the node as a typedef.
 *
 * @param compil_status The object in which error messages will be pushed.
 *
 * @return False if a compilation error occurs.
 *
 * Precondition:
 *
 * this->kind == kind::typedef
 */
bool node::compile_as_typedef(compilation_log_t & compil_status)
{
    if(!compile_type_instanciation(
        boost::get<typedef_data_t>(data).type_instanciation,
        compil_status, this))
    {
        compil_status.add_message(
            compilation_message_type_t::error,
            "failed to compile the type of the node " + name + "."
        );
        return false;
    }

    return true;
}


/**
 * Compile the node as a member.
 *
 * @param compil_status The object in which error messages will be pushed.
 *
 * @return False if a compilation error occurs.
 *
 * Precondition:
 *
 * this->kind == kind::member
 */
bool node::compile_as_member(compilation_log_t & compil_status)
{
    if (!compile_type_instanciation(boost::get<member_data_t>(data)
        .type_instanciation, compil_status, this))
    {
        compil_status.add_message(
            compilation_message_type_t::error,
            "failed to compile the type of the node " + name + "."
        );
        return false;
    }

    return true;
}


/**
 * Compile the node as an enumeration.
 *
 * @param compil_status The object in which error messages will be pushed.
 *
 * @return False if a compilation error occurs.
 *
 * Precondition:
 *
 * this->kind == kind::enum_
 */
bool node::compile_as_enum(compilation_log_t & compil_status)
{
    const sptr_list::iterator it_end = children.end();

    // Firstly, find the first fixed value.
    int lowest_fixed = 0;
    size_t lowest_fixed_index = 0;
    sptr_list::iterator it;
    sptr_list::iterator itx = children.begin();

    size_t index = 0;
    for(it = children.begin(); it != it_end; ++it)
    {
        const enum_value_data_t & evd = (*it)->get_enum_value_data();
        if(evd.get_fixed_value_flag())
        {
            lowest_fixed = evd.value;
            lowest_fixed_index = index;
            itx = it;
            break;
        }

        index++;
    }

    // Computes the decreasing value of the previous enumeration members.
    int x = (lowest_fixed < (int)lowest_fixed_index)
        ?lowest_fixed:(int)lowest_fixed_index;

    sptr_list::iterator it2 = it;
    index = lowest_fixed_index;
    while(index > 0)
    {
        it2--;
        index--;

        // Verify that the numeric limit is not reached, before decrementing x.
        if(x == std::numeric_limits<int>::min())
        {
            compil_status.add_message(
                compilation_message_type_t::error,
                "enumeration constraints results in a member value lowest than "
                    "the minimum possible int value."
            );
            return false;
        }

        (*it2)->get_enum_value_data().value = --x;
    }

    // Computes the increasing value of the next enumeration members.
    x = lowest_fixed;
    bool do_increment = false;

    while(itx != it_end)
    {
        if(do_increment)
        {
            // Verify that the numeric limit is not reached, before incrementing
            // x.
            if(x == std::numeric_limits<int>::max())
            {
                compil_status.add_message(
                    compilation_message_type_t::error,
                    "enumeration constaints results in a member value higher "
                        "than the maximal possible int value."
                );
                return false;
            }

            // Increase x.
            x++;
        }
        else
        {
            do_increment = true;
        }

        // The member may define a fixed value.
        const enum_value_data_t & evd = (*itx)->get_enum_value_data();
        if(evd.get_fixed_value_flag())
        {
            // The fixed value must be higher or equal to x.
            int fixed_value = evd.value;
            if(fixed_value < x)
            {
                compil_status.add_message(
                    compilation_message_type_t::error,
                    "enumeraion member '" + (*itx)->get_name() + "' has a fixed "
                        "value " + str::from(fixed_value) + ", but this "
                        "value has already been reached by a previous node."
                );
                return false;
            }

            x = fixed_value;
        }

        (*itx)->get_enum_value_data().value = x;
        ++itx;
    }

    return true;
}


/**
 * Compile the node as an enumeration value.
 *
 * @param compil_status The object in which error messages will be pushed.
 *
 * @return False if a compilation error occurs.
 *
 * Precondition:
 *
 * this->kind == kind::enum_value
 */
bool node::compile_as_enum_value(compilation_log_t & compil_status)
{
    enum_value_data_t & evd = get_enum_value_data();
    if(evd.get_fixed_value_flag())
    {
        mpz_class mpz_value(evd.get_value_str());

        // Check that the requested value has an acceptable value.
        if(!mpz_value.fits_sint_p())
        {
            compil_status.add_message(
                compilation_message_type_t::error,
                "enumeration value " + str::from(mpz_value) + " does not "
                    "fit an int."
            );
            return false;
        }

        evd.value = mpz_value.get_si();
    }

    return true;
}


/**
 * Compiles a given type instanciation.
 *
 * @param ti The type instanciation to compile, may be modified.
 * @param compil_status The object in which error messages will be pushed.
 * @param scope_node Node to be used for type resolution.
 *
 * @return False is an error occurs during compilation.
 */
bool node::compile_type_instanciation(
    type_instanciation_t & ti,
    compilation_log_t & compil_status,
    node* scope_node
)
const
{
    // First step: solve the path to the base type.
    if(ti.get_type_ptr() == 0)
    {
        const path & a_path = ti.get_type_path();

        // Try to find the type from the path.
        sptr type_sptr = solve_type(a_path);

        if(type_sptr.get() == 0)
        {
            // type not found... bouhouoouou.
            compil_status.add_message(
                compilation_message_t(
                    compilation_message_type_t::error,
                    "type '" + a_path.print() + "' not found."
                )
            );

            return false;
        }
        else
        {
            // A node has been found ! by wait wait... it may not be a type !
            // Lets check that.
            if(!type_sptr->is_type())
            {
                // Ha Haaa ! There is a problem with that node !
                compil_status.add_message(
                    compilation_message_t(
                        compilation_message_type_t::error,
                        "the node '" + a_path.print() + "' is not a type, but a "
                            + kind::Wrapper::to_string(type_sptr->get_kind())
                            + "."
                    )
                );
                return false;
            }

            // Everything is ok !
            // Be carefull, give a simple pointer, not a shared_ptr.
            ti.set_type_node_ptr(type_sptr.get());
        }
    }

    if(ti.get_owning_node())
    {
        // Must compile the embeded node. Since this node has no parent, we must
        // give it the scope node for type resolution.
        ti.get_type_ptr()->set_scope_node(scope_node);
        ti.get_type_ptr()->compile(compil_status);
    }

    /* Second step: solve each template argument. */
    type_instanciation_t::parameter_list & pl = ti.get_parameters();
    const type_instanciation_t::parameter_list::iterator it_end = pl.end();
    for (type_instanciation_t::parameter_list::iterator it = pl.begin();
        it != it_end;
        it++
    ){
        if(!compile_type_instanciation(*it, compil_status, scope_node))
        {
            return false;
        }
    }

    return true;
}


} /* namespace rec */
} /* namespace bakery */

