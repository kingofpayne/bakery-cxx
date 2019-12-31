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
 * Initializes the def_enum rule.
 *
 * @param rules Reference over the rules container.
 */
template <typename I> void generic_init_def_enum(rule_container<I> & rules)
{
    namespace qi = boost::spirit::qi;
    using qi::_val;
    using qi::_1;

    /* Example:
     *
     * enum X {
     *     toto,
     *     titi,
     *     pouet = 4
     * }
     */
    rules.def_enum =
        qi::eps
        [
            _val = create_def_node_sptr(rec::node::kind::enum_)
        ]
        >>
        qi::string("enum")
        >>
        rules.identifier
        [
            boost::phoenix::bind(&rec::node::set_name, *_val, _1)
        ]
        >>
        '{'
        >>
        -(
            (
                rules.def_enum_value
                [
                    boost::phoenix::bind(&rec::node::add_child, *_val, _1)
                ]
                %
                qi::char_(',')
            )
            >>
            -qi::char_(',')
        )
        >>
        '}';
}


template <> void init_def_enum<iterator>(rule_container<iterator> & rules)
{
    generic_init_def_enum<iterator>(rules);
}


} /* namespace grammar */
} /* namespace bakery */

