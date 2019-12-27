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


#include "grammar.hpp"


namespace bakery {
namespace grammar {


/**
 * Initializes the recipe rule.
 *
 * @param rules Reference over the rules container.
 */
template <typename I> void generic_init_recipe(rule_container<I> & rules)
{
	namespace qi = boost::spirit::qi;
	using qi::_val;
	using qi::_1;
		
	rules.recipe_ =
		*(
			"include"
			>>
			rules.recipe_indication
			[
				bind(&def::recipe::add_include_file, _val, _1)
			]
			>>
			';'
		)
		>>
		rules.def_composite_content
		[
			boost::phoenix::bind(&def::node::set_kind, *_1,
				def::node::kind::structure),
			boost::phoenix::bind(&def::recipe::set_node, _val, _1)
		];
}


template <> void init_recipe<iterator>(rule_container<iterator> & rules)
{
	generic_init_recipe<iterator>(rules);
}


} /* namespace grammar */
} /* namespace bakery */

