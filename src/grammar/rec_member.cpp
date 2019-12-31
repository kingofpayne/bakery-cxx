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
 * Initializes the def_member rule.
 *
 * @param rules Reference over the rules container.
 */
template <typename I> void generic_init_def_member(rule_container<I> & rules)
{
    namespace qi = boost::spirit::qi;
    using qi::_val;
    using qi::_1;
    using qi::_a;
    using boost::phoenix::construct;

    /* A member is a type_instanciation followed by a name.
     * It may have qualifiers, such as "optional".
     * It may have a default value (for example: "int x = 4"). */
    rules.def_member =
        /* Create the node */
        qi::eps
        [
            _val = create_def_node_sptr(rec::node::kind::member)
        ]
        >>
        -(
            qi::string("optional")
            [
                boost::phoenix::bind(
                    &rec::node::add_qualifier,
                    *_val,
                    rec::node::qualifier::optional
                )
            ]
        )
        >>
        rules.def_type_instanciation
        [
            /* Initialise the member data */
            _a = construct<rec::member_data_t>(),
            /* Sets the type instanciation returned by the parser */
            boost::phoenix::bind(&rec::member_data_t::type_instanciation, _a)
                = _1
        ]
        >>
        rules.identifier
        [
            boost::phoenix::bind(
                &rec::node::set_name,
                *_val,
                _1
            )
        ]
        >> -(
            qi::char_('=')
            >>
            rules.dat_value
            [
                boost::phoenix::bind(&rec::member_data_t::default_value_node,
                    _a) =_1
            ]
        )
        >>
        qi::eps
        [
            /* Save the member data in the node */
            boost::phoenix::bind(&rec::node::set_member_data, *_val, _a)
        ];
}


template <> void init_def_member<iterator>(rule_container<iterator> & rules)
{
    generic_init_def_member<iterator>(rules);
}


} /* namespace grammar */
} /* namespace bakery */

