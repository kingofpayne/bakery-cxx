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
//#include <boost/spirit/home/phoenix/bind/bind_member_function.hpp> //DEPRECATED
#include <boost/phoenix/bind/bind_member_function.hpp>


namespace bakery {
namespace grammar {


/**
 * Initializes the floating_number rule.
 *
 * @param rules Reference over the rules container.
 */
template <typename I>
	void generic_init_floating_number(rule_container<I> & rules)
{
	namespace qi = boost::spirit::qi;
	using qi::_val;
	using qi::_1;

	/* floating numbers. May have different forms:
	 * -45.12
	 * -45
	 * .12
	 * -10.6e-78
	 * .6e-78
	 * e-78 (means 1.0e-78)
	 * -.10.e-78
	 *
	 * The spirit || operator is very helpful. Ommiting the eventual first
	 * minus symbol, differents possibilities are:
	 *
	 * N
	 * N.N
	 *  .N
	 * N  eZ
	 * N.NeZ
	 *  .NeZ
	 *    eZ
	 *
	 * We can reduce de first three possibilities to: X = N||(.N). Then we
	 * have:
	 *
	 * X
	 * XeZ
	 *  eZ
	 *
	 * which can be reduced to W = X||(eZ). So finally, we have the
	 * expression:
	 *
	 * ( Z || (.N) ) || (e Z) */
	rules.floating_number = 
		-(
			qi::char_('-')
			[
				boost::phoenix::bind(&dat::floating::set_negative, _val,
					true)
			]
		)
		>>
		(
			rules.unsigned_integer
			[
				boost::phoenix::bind(&dat::floating::set_integer_string,
					_val, _1)
			]
			||
			(
				qi::char_('.')
				>>
				rules.unsigned_integer
				[
					boost::phoenix::bind(&dat::floating::set_decimal_string,
						_val, _1)
				]
			)
		)
		||
		(
			qi::char_('e')
			>>
			rules.integer
			[
				boost::phoenix::bind(&dat::floating::set_exponent_string,
					_val, _1)
			]
		);
}


template <>
	void init_floating_number<iterator>(rule_container<iterator> & rules)
{
	generic_init_floating_number<iterator>(rules);
}


} /* namespace grammar */
} /* namespace bakery */

