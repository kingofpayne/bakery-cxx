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
#include "util.hpp"
#include <boost/spirit/include/phoenix.hpp>


namespace bakery {
namespace grammar {


/**
 * Initializes the def_enum_value rule.
 *
 * @param rules Reference over the rules container.
 */
template <typename I> void
    generic_init_def_enum_value(rule_container<I> & rules)
{
    namespace qi = boost::spirit::qi;
    using qi::_val;
    using qi::_1;
    using qi::_a;
    using boost::phoenix::construct;

    /* Examples:
     *
     * toto
     * toto = 4 */
    rules.def_enum_value =
        /* Create the node. */
        qi::eps
        [
            _val = create_def_node_sptr(rec::node::kind::enum_value)
        ]
        >>
        rules.identifier
        [
            boost::phoenix::bind(&rec::node::set_name, *_val, _1)
        ]
        >>
        -(
            qi::char_('=')
            >>
            rules.integer
            [
                /* Initialize an enum value data. */
                _a = construct<enum_value_data_t>(),
                /* Sets the value. */
                boost::phoenix::bind(&enum_value_data_t::set_fixed_value, _a,
                    _1),
                /* Save the data in the node. */
                boost::phoenix::bind(&rec::node::set_enum_value_data, *_val, _a)
            ]
        );
}


template <> void init_def_enum_value<iterator>(rule_container<iterator> & rules)
{
    generic_init_def_enum_value<iterator>(rules);
}


} /* namespace grammar */
} /* namespace bakery */

