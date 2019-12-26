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


/* Must be included before spirit and phoenix. */
#include "../enable_phoenix_v3.hpp"

#include "grammar.hpp"
#include "util.hpp"
#include <boost/spirit/include/phoenix.hpp>


namespace bakery {
namespace grammar {


/**
 * Initializes the def_typedef rule.
 *
 * @param rules Reference over the rules container.
 */
template <typename I> void generic_init_def_typedef(rule_container<I> & rules)
{
	namespace qi = boost::spirit::qi;
	using qi::_val;
	using qi::_1;
	using boost::phoenix::construct;

	rules.def_typedef =
		qi::string("typedef")
		>>
		qi::eps
		[
			_val = create_def_node_sptr(def::node::kind::typedef_)
		]
		>>
		rules.def_type_instanciation
		[
			boost::phoenix::bind(
				&def::node::set_typedef_data,
				*_val,
				construct<def::typedef_data_t>(_1)
			)
		]
		>>
		rules.identifier
		[
			boost::phoenix::bind(&def::node::set_name, *_val, _1)
		]
		>>
		-rules.def_template_argument_declaration
		[
			boost::phoenix::bind(
				&def::node::add_child_list<std::list<def::node::sptr> >,
				*_val,
				_1
			)
		];
}


template <> void init_def_typedef<iterator>(rule_container<iterator> & rules)
{
	generic_init_def_typedef<iterator>(rules);
}


} /* namespace grammar */
} /* namespace bakery */

