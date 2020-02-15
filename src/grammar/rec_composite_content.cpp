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
#include "util.hpp"


namespace bakery {
namespace grammar {


/**
 * Initializes the def_composite_content rule.
 *
 * @param rules Reference over the rules container.
 */
template <typename I>
    void generic_init_def_composite_content(rule_container_t<I> & rules)
{
    namespace qi = boost::spirit::qi;
    using qi::_val;
    using qi::_1;

    /* A composite is a list of member and type nodes. */
    rules.def_composite_content =
        qi::eps[ _val = create_def_node_sptr() ]
        >>
        -(
            (
                 rules.def_namespace
                [
                    boost::phoenix::bind(&rec::node::add_child, *_val, _1)
                ]
                |
                 rules.def_structure
                [
                    boost::phoenix::bind(&rec::node::add_child, *_val, _1)
                ]
                |
                 rules.def_variant
                [
                    boost::phoenix::bind(&rec::node::add_child, *_val, _1)
                ]
                |
                rules.def_typedef
                [
                    boost::phoenix::bind(&rec::node::add_child, *_val, _1)
                ]
                |
                rules.def_enum
                [
                    boost::phoenix::bind(&rec::node::add_child, *_val, _1)
                ]
                |
                 rules.def_member
                [
                    boost::phoenix::bind(&rec::node::add_child, *_val, _1)
                ]
            ) % ';'
            >>
            -qi::char_(';')
        )
        ;
}


template <>
    void init_def_composite_content<iterator>(rule_container_t<iterator> & rules)
{
    generic_init_def_composite_content<iterator>(rules);
}


} /* namespace grammar */
} /* namespace bakery */

