/**
 * Copyright (C) 2012
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
 * Initializes the recipe_indication rule.
 *
 * @param rules Reference over the rules container.
 */
template <typename I>
	void generic_init_recipe_indication(rule_container<I> & rules)
{
	namespace qi = boost::spirit::qi;
	using qi::_val;
	using qi::_1;
	using qi::_a;
	using qi::char_;

	/* Relative path recipe file:
	 * recipe "toto.def"
	 *
	 * Absolute path recipe file:
	 * recipe <toto.def> */
	rules.recipe_indication =
		(
			(
				'"'
				>>
				(	
					qi::eps[_a = std::string()]
					>>
					*((char_ - char_('"'))[ _a += _1 ])
				)
				[
					bind(&file_indication::set_absolute, _val, false)
				]
				>>
				'"'
			)
			|
			(
				'<'
				>>
				(	
					qi::eps[_a = std::string()]
					>>
					*((char_ - char_('>'))[ _a += _1 ])
				)
				[
					bind(&file_indication::set_absolute, _val, true)
				]
				>>
				'>'
			)
		)
		[
			bind(&file_indication::set_path, _val, _a)
		];
}


template <>
	void init_recipe_indication<iterator>(rule_container<iterator> & rules)
{
	generic_init_recipe_indication<iterator>(rules);
}


} /* namespace grammar */
} /* namespace bakery */

