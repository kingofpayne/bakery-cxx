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
 * Initializes the def_array rule.
 *
 * @param rules Reference over the rules container.
 */
template <typename I> void generic_init_def_array(rule_container<I> & rules)
{
	namespace qi = boost::spirit::qi;
	using qi::_val;
	using qi::_1;
	using qi::_a;
	using boost::phoenix::construct;

		
	rules.def_array =
		qi::eps
		[
			_val = create_def_node_sptr(rec::node::kind::array)
		]
		>>
		rules.def_type_instanciation_no_array
		[
			/* Initializes the array data */
			_a = construct<rec::array_data_t>(),
			/* Sets the type instanciation */
			boost::phoenix::bind(&rec::array_data_t::type_instanciation, _a)
				= _1
		]
		>>
		+
		(
			qi::char_('[')
			>>
			(
				qi::int_
				[
					boost::phoenix::bind(&rec::array_data_t::add_dim, _a, _1)
				]
				|
				qi::eps
					[ boost::phoenix::bind(&rec::array_data_t::add_dim, _a,
					boost::phoenix::val(0)) ]
			)
			>>
			qi::char_(']')
		)
		>>
		/* Save the array data in the node */
		qi::eps
		[
			boost::phoenix::bind(&rec::node::set_array_data, *_val, _a)
		];
}


template <> void init_def_array<iterator>(rule_container<iterator> & rules)
{
	generic_init_def_array<iterator>(rules);
}


} /* namespace grammar */
} /* namespace bakery */

