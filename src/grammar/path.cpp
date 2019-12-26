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
#include <boost/spirit/include/phoenix_operator.hpp>
//#include <boost/spirit/home/phoenix/bind/bind_member_function.hpp> //DEPRECATED
#include <boost/phoenix/bind/bind_member_function.hpp>


namespace bakery {
namespace grammar {


/**
 * Initializes the path rule.
 *
 * @param rules Reference over the rules container.
 */
template <typename I> void generic_init_path(rule_container<I> & rules)
{
	namespace qi = boost::spirit::qi;
	using qi::_val;
	using qi::_1;

	/* Examples:
	 *
	 * ::toto::tutu::Mytype
	 * tadam::waou::x
	 * toto */
	rules.path =
	(
		-(
			qi::string("::")
			[
				boost::phoenix::bind(&def::path::set_absolute, _val, true)
			]
		)
		>>
		rules.identifier
		[
			boost::phoenix::bind(&def::path::push_back, _val, _1)
		]
		>>
		*(
			qi::string("::")
			>>
			rules.identifier
			[
				boost::phoenix::bind(&def::path::push_back, _val, _1)
			]
		)
	);	
}


template <> void init_path<iterator>(rule_container<iterator> & rules) 
{
	generic_init_path<iterator>(rules);
}


} /* namespace grammar */
} /* namespace bakery */

