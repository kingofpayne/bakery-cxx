/**
 * Copyright (C) 2012, 2013
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
#include "util.hpp"


namespace bakery {
namespace grammar {


/**
 * Initializes the def_template_argument_declaration rule.
 *
 * @param rules Reference over the rules container.
 */
template <typename I>
	void generic_init_def_template_argument_declaration
	(rule_container<I> & rules)
{
	namespace qi = boost::spirit::qi;
	using qi::_val;
	using qi::_1;
	using qi::_a;

	rules.def_template_argument_declaration =
		qi::char_('<')[ _val ]
		>>
		(
			rules.identifier
			[
				/* Create a node and store it into _a */
				_a = create_def_node_sptr(rec::node::kind::template_type),
				/* Set the name of the node to the identifier _1 */
				boost::phoenix::bind(&rec::node::set_name, *_a, _1),
				/* Push the node in the list */
				boost::phoenix::push_back(_val, _a)	
			]
			%
			','
		)
		>>
		'>';
}


template <>
	void init_def_template_argument_declaration<iterator>
	(rule_container<iterator> & rules)
{
	generic_init_def_template_argument_declaration<iterator>(rules);
}


} /* namespace grammar */
} /* namespace bakery */

