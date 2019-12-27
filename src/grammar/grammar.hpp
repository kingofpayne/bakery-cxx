/**
 * Copyright (C) 2012, 2013
 * Olivier Heriveaux.
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
 * @author Olivier Heriveaux
 */


#ifndef _BAKERY_GRAMMAR_GRAMMAR_HPP_
#define _BAKERY_GRAMMAR_GRAMMAR_HPP_


/* Must be included before spirit and phoenix. */
#include "../enable_phoenix_v3.hpp"

#include "../dat/data.hpp"
#include "../dat/node.hpp"
#include "../rec/recipe.hpp"
#include "../rec/node.hpp"
#include "../rec/type_instanciation.hpp"
#include "../recipe_indication.hpp"
#include "../recipe_or_data.hpp"
#include "../file_indication.hpp"
#include "custom_skipper.hpp"
#include <boost/spirit/include/qi.hpp>


namespace bakery {
namespace grammar {


typedef std::string::const_iterator iterator;


/** Skipper to be used during parsing.
 *
 * boost::spirit::ascii::space was used initially. In order to allow C-style
 * comments, a custom skipper has been written in custom_skipper.hpp. */
typedef custom_skipper<iterator> skipper;


/**
 * This structure contains all the rules which are part of the grammar. By
 * passing the reference to an instance of this structure, rule initialization
 * functions are able to uses the other rules.
 *
 * This is quite nag, but giving only the required rules references to the
 * initialization functions is really horrible and hard to maintain.
 *
 * @param <I> Grammar iterator type.
 */
template <typename I>
	struct rule_container
{
	boost::spirit::qi::symbols<char, char> quoted_string_symbols;

	boost::spirit::qi::rule<I, std::string(), skipper>
		identifier,
		integer,
		unsigned_integer,
		quoted_string;

	boost::spirit::qi::rule<I, dat::floating(), skipper> floating_number;

	boost::spirit::qi::rule<I, rec::path(), skipper> path;

	/** recipe or data. */
	boost::spirit::qi::rule<I, recipe_or_data(), skipper>
		recipe_or_data_;

	/** recipe. */
	boost::spirit::qi::rule<I, rec::recipe(), skipper>
		recipe_;

	/** Array type definition. */
	boost::spirit::qi::rule<I, boost::spirit::qi::locals<rec::array_data_t>,
		rec::node::sptr(), skipper> def_array;

	boost::spirit::qi::rule<I, rec::node::sptr(), skipper>
		def_enum;

	/** Template argument list. */
	boost::spirit::qi::rule<I, boost::spirit::qi::locals<rec::node::sptr>,
		std::list<rec::node::sptr>(), skipper>
		def_template_argument_declaration;

	boost::spirit::qi::rule<I, boost::spirit::qi::locals<rec::member_data_t>,
		rec::node::sptr(), skipper> def_member;

	boost::spirit::qi::rule<I, rec::type_instanciation_t(), skipper>
		/* type instanciation for recipes. Includes array definition. */
		def_type_instanciation,
		/** type instanciation for recipes. Does not include array
		 * definitions. */
		def_type_instanciation_no_array;

	boost::spirit::qi::rule<I, rec::node::sptr(), skipper>
		def_typedef,
		def_composite_content;

	/* Namespace definition. */
	boost::spirit::qi::rule<I, boost::spirit::qi::locals<std::string>,
		rec::node::sptr(), skipper> def_namespace;

	/* Structure definition. */
	boost::spirit::qi::rule
	<
		I,
		boost::spirit::qi::locals
		<
			std::string,
			std::list<rec::node::sptr>,
			rec::structure_data_t
		>,
		rec::node::sptr(),
		skipper
	> def_structure;

	/* Variant definition. */
	boost::spirit::qi::rule<I, boost::spirit::qi::locals<std::string,
		std::list<rec::node::sptr> >, rec::node::sptr(), skipper>
		def_variant;

	/* data rule. */
	boost::spirit::qi::rule<I, dat::data(), skipper> data;

	boost::spirit::qi::rule<I, dat::node::sptr(), skipper>
		dat_bool_value,
		dat_floating_value,
		dat_identifier_value,
		dat_string_value,
		dat_structure_content,
		dat_assignment,
		dat_value,
		dat_variant_value,
		dat_list,
		dat_map,
		dat_map_assignment,
		dat_structure;

	boost::spirit::qi::rule<I, boost::spirit::qi::locals<enum_value_data_t>,
		rec::node::sptr(), skipper> def_enum_value;

	/** recipe indication. */
	boost::spirit::qi::rule<I, boost::spirit::qi::locals<std::string>,
		file_indication(), skipper>
		recipe_indication;

    boost::spirit::qi::rule<I, recipe_indication_t(), skipper>
        recipe_indication_new;
};


/* Below are the prototypes of the functions which initializes the different
 * rules of the grammar.
 *
 * In a previous revision, all the grammar was written in a single .hpp file. As
 * a result, the compilation of the grammar was requiring a lot of memory
 * (approximately 2 gigabytes !), which was a big problem. Grammar has been
 * splitted in many files to have as many compilation units as rules. This
 * reduces memory requirements at the cost of source complexity, but this was
 * absolutely necessary. */


template <typename I> void init_path(rule_container<I> &);
template <typename I> void init_integer(rule_container<I> &);
template <typename I> void init_unsigned_integer(rule_container<I> &);
template <typename I> void init_quoted_string(rule_container<I> &);
template <typename I> void init_floating_number(rule_container<I> &);
template <typename I> void init_identifier(rule_container<I> &);
template <typename I> void init_recipe_or_data(rule_container<I> &);
template <typename I> void init_recipe(rule_container<I> &);
template <typename I> void init_recipe_indication(rule_container<I> &);
template <typename I> void init_recipe_indication_new(rule_container<I> &);
template <typename I> void init_def_type_instanciation(rule_container<I> &);
template <typename I> void init_def_array(rule_container<I> &);
template <typename I> void init_def_member(rule_container<I> &);
template <typename I> void init_def_typedef(rule_container<I> &);
template <typename I> void init_def_variant(rule_container<I> &);
template <typename I> void init_def_namespace(rule_container<I> &);
template <typename I> void init_def_structure(rule_container<I> &);
template <typename I> void init_def_composite_content(rule_container<I> &);
template <typename I>
	void init_def_type_instanciation_no_array(rule_container<I> &);
template <typename I>
	void init_def_template_argument_declaration(rule_container<I> &);
template <typename I> void init_data(rule_container<I> &);
template <typename I> void init_dat_bool_value(rule_container<I> &);
template <typename I> void init_dat_floating_value(rule_container<I> &);
template <typename I> void init_dat_identifier_value(rule_container<I> &);
template <typename I> void init_dat_string_value(rule_container<I> &);
template <typename I> void init_dat_assignment(rule_container<I> &);
template <typename I> void init_dat_list(rule_container<I> &);
template <typename I> void init_dat_map(rule_container<I> &);
template <typename I> void init_dat_map_assignment(rule_container<I> &);
template <typename I> void init_dat_structure(rule_container<I> &);
template <typename I> void init_dat_variant_value(rule_container<I> &);
template <typename I> void init_dat_structure_content(rule_container<I> &);
template <typename I> void init_dat_value(rule_container<I> &);
template <typename I> void init_def_enum_value(rule_container<I> &);
template <typename I> void init_def_enum(rule_container<I> &);


} /* namespace grammar */
} /* namespace bakery */


#endif

