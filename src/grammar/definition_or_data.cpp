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
 * Initializes the definition_or_data rule.
 *
 * @param rules Reference over the rules container.
 */
template <typename I>
	void generic_init_definition_or_data(rule_container<I> & rules)
{
	namespace qi = boost::spirit::qi;
	using qi::_val;
	using qi::_1;

	/* Node which returns either a data or a definition. */
	rules.definition_or_data_ =
		/* Start by testing data since all data files must start by the
		 * 'definition' directive. */
		rules.data
		[
			boost::phoenix::bind(
				&definition_or_data::set_data,
				_val,
				_1
			)
		]
		|
		rules.definition_
		[
			boost::phoenix::bind(
				&definition_or_data::set_definition,
				_val,
				_1
			)
		];
}


template <>
	void init_definition_or_data<iterator>(rule_container<iterator> & rules)
{
	generic_init_definition_or_data<iterator>(rules);
}


} /* namespace grammar */
} /* namespace bakery */

