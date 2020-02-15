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


namespace bakery {
namespace grammar {


/**
 * Initializes the def_variant rule.
 *
 * @param rules Reference over the rules container.
 */
template <typename I> void generic_init_def_variant(rule_container_t<I> & rules)
{
    namespace qi = boost::spirit::qi;
    using qi::_val;
    using qi::_1;
    using qi::_a;
    using qi::_b;

    /* _a : name of the node
     *_b : list of template arguments */
    rules.def_variant =
        qi::string("variant")
        >>
        rules.identifier[_a = _1]
        >>
        -rules.def_template_argument_declaration[_b = _1]
        >>
        '{'
        >>
        -(
            rules.def_composite_content
            [
                _val = _1,
                boost::phoenix::bind(&rec::node::set_name, *_val, _a),
                boost::phoenix::bind(&rec::node::set_kind, *_val,
                    rec::node::kind::variant),
                boost::phoenix::bind(
                    &rec::node::add_child_list<std::list<rec::node::sptr> >,
                    *_val,
                    _b
                )
            ]
        )
        >>
        '}';
}


template <> void init_def_variant<iterator>(rule_container_t<iterator> & rules)
{
    generic_init_def_variant<iterator>(rules);
}


} /* namespace grammar */
} /* namespace bakery */

