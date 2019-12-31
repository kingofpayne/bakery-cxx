/**
 * Copyright (C) 2012
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


#ifndef _BAKERY_DEF_GRAMMAR_MAIN_HPP_
#define _BAKERY_DEF_GRAMMAR_MAIN_HPP_


#include "util.hpp"
#include "grammar.hpp"


namespace bakery {
namespace grammar {


namespace qi = boost::spirit::qi;


/**
 * Main grammar which parses a recipe or data code.
 */
template <typename I>
    struct main:
    qi::grammar<
        I,
        recipe_or_data(),
        skipper
    >
{
    main():
        main::base_type(rules.recipe_or_data_)
    {
        /* Quoted string symbols */
        rules.quoted_string_symbols.add
            ("\\\"", '\"')
            ("\\\\", '\\')
            ("\\n", '\n')
            ("\\t", '\t');

        init_recipe_or_data(rules);
        init_identifier(rules);
        init_quoted_string(rules);
        init_integer(rules);
        init_unsigned_integer(rules);
        init_floating_number(rules);
        init_path(rules);
        init_recipe(rules);
        init_data(rules);
        init_def_composite_content(rules);
        init_def_namespace(rules);
        init_def_template_argument_declaration(rules);
        init_def_structure(rules);
        init_def_array(rules);
        init_def_variant(rules);
        init_def_typedef(rules);
        init_def_enum(rules);
        init_def_enum_value(rules);
        init_def_member(rules);
        init_def_type_instanciation(rules);
        init_def_type_instanciation_no_array(rules);
        init_recipe_indication(rules);
        init_recipe_indication_new(rules);
        init_dat_structure_content(rules);
        init_dat_assignment(rules);
        init_dat_value(rules);
        init_dat_variant_value(rules);
        init_dat_structure(rules);
        init_dat_bool_value(rules);
        init_dat_string_value(rules);
        init_dat_floating_value(rules);
        init_dat_identifier_value(rules);
        init_dat_list(rules);
        init_dat_map(rules);
        init_dat_map_assignment(rules);

        /*boost::spirit::qi::on_error<boost::spirit::qi::fail>
        (
               global
          , std::cout
                << val("Syntax error, expecting ")
                << _4
                << val(" here: \"")
                << construct<std::string>(_3, _2)
                << val("\"")
                << std::endl
        );*/
    }

    rule_container<I> rules;
};


} /* namespace grammar */
} /* namespace bakery */


#endif

