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


#include "grammar.hpp"


namespace bakery {
namespace grammar {


/**
 * Initializes the data rule.
 *
 * @param rules Reference over the rules container.
 */
template <typename I> void generic_init_data(rule_container<I> & rules)
{
	namespace qi = boost::spirit::qi;
	using qi::_val;
	using qi::_1;

	rules.data =
		"recipe"
		>> rules.recipe_indication_new
		[
			bind(&dat::data::set_recipe_indication, _val, _1)
		]
		>> ';'
		>> rules.dat_structure_content
		[	
			boost::phoenix::bind(&dat::data::set_root_node, _val, _1)
		];
}


template <> void init_data<iterator>(rule_container<iterator> & rules)
{
	generic_init_data<iterator>(rules);
}


} /* namespace grammar */
} /* namespace bakery */

