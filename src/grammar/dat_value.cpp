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


namespace bakery {
namespace grammar {


/**
 * Initializes the dat_value rule.
 *
 * @param rules Reference over the rules container.
 */
template <typename I> void generic_init_dat_value(rule_container<I> & rules)
{
    namespace qi = boost::spirit::qi;
    using qi::_val;
    using qi::_1;

    rules.dat_value =
    (
         rules.dat_variant_value[_val = _1]
        | rules.dat_bool_value[_val = _1]
        | rules.dat_string_value[_val = _1]
        | rules.dat_floating_value[_val = _1]
        | rules.dat_identifier_value[_val = _1]
        | rules.dat_structure[_val = _1]
        | rules.dat_list[_val = _1]
        | rules.dat_map[_val = _1]
    );
}


template <> void init_dat_value<iterator>(rule_container<iterator> & rules)
{
    generic_init_dat_value<iterator>(rules);
}


} /* namespace grammar */
} /* namespace bakery */

