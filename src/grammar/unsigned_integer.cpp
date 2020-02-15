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


/* Must be included before spirit and phoenix. */
#include "../enable_phoenix_v3.hpp"

#include "grammar.hpp"
#include <boost/spirit/include/phoenix.hpp>


namespace bakery {
namespace grammar {


/**
 * Initializes an unsigned_integer rule.
 *
 * @param dest Reference over the rule to be initialized.
 */
template <typename I>
    void generic_init_unsigned_integer(rule_container_t<I> & rules)
{
    namespace qi = boost::spirit::qi;

    rules.unsigned_integer = qi::lexeme
    [
        +( qi::digit[qi::_val += qi::_1] )
    ];
}


template <> void
    init_unsigned_integer<iterator>(rule_container_t<iterator> & rules)
{
    generic_init_unsigned_integer<iterator>(rules);
}


} /* namespace grammar */
} /* namespace bakery */

