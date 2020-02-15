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
 * Initializes the def_type_instanciation rule.
 *
 * @param rules Reference over the rules container.
 */
template <typename I>
    void generic_init_def_type_instanciation(rule_container_t<I> & rules)
{
    namespace qi = boost::spirit::qi;
    using qi::_val;
    using qi::_1;

    /* Examples:
     *
     * int
     * point<int>
     * list<point<double>>
     */
    rules.def_type_instanciation =
        rules.def_array
        [
            boost::phoenix::bind(
                &rec::type_instanciation_t::set_type_node_sptr, _val, _1)
        ]
        |
        rules.def_type_instanciation_no_array[_val = _1];
}


template <>
    void init_def_type_instanciation<iterator>(rule_container_t<iterator> & rules)
{
    generic_init_def_type_instanciation<iterator>(rules);
}


} /* namespace grammar */
} /* namespace bakery */

