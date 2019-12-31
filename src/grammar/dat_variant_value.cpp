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
 * Initializes the dat_variant_value rule.
 *
 * @param rules Reference over the rules container.
 */
template <typename I>
    void generic_init_dat_variant_value(rule_container<I> & rules)
{
    namespace qi = boost::spirit::qi;
    using qi::_val;
    using qi::_1;

    rules.dat_variant_value =
        qi::eps
        [
            _val = create_dat_node_sptr(dat::node::kind::variant)
        ]
        >>
        rules.identifier
        [
            boost::phoenix::bind(&dat::node::set_name, *_val, _1)
        ]
        >>
        qi::char_(':')
        >>
        rules.dat_value
        [
            boost::phoenix::bind(&dat::node::add_child, *_val, _1)
        ];
}


template <>
    void init_dat_variant_value<iterator>(rule_container<iterator> & rules)
{
    generic_init_dat_variant_value<iterator>(rules);
}


} /* namespace grammar */
} /* namespace bakery */

