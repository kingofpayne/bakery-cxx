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
 * Initializes the integer rule.
 *
 * @param rules Reference over the rules container.
 */
template <typename I> void generic_init_integer(rule_container_t<I> & rules)
{
    namespace qi = boost::spirit::qi;
    using qi::_val;
    using qi::_1;

    /* Example of matched strings:
     * 1
     * 465
     * -45
     * 6405646540654103210498406510 */
    rules.integer = qi::lexeme
    [
        -qi::char_('-')[_val += _1]
        >>
        +(
            qi::digit[_val += _1]
        )
    ];
}


template <> void init_integer<iterator>(rule_container_t<iterator> & rules)
{
    generic_init_integer<iterator>(rules);
}


} /* namespace grammar */
} /* namespace bakery */

