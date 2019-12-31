/**
 * Copyright (C) 2013
 * Olivier Hériveaux
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


#ifndef _BAKERY_GRAMMAR_CUSTOM_SKIPPER_HPP_
#define _BAKERY_GRAMMAR_CUSTOM_SKIPPER_HPP_


/* Must be included before spirit and phoenix. */
#include "../enable_phoenix_v3.hpp"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/repository/include/qi_confix.hpp>


namespace bakery {
namespace grammar {


/**
 * Custom parser skipper.
 *
 * Necessary to skip C-style comments.
 */
template <typename Iterator>
    struct custom_skipper: boost::spirit::qi::grammar<Iterator>
{
    custom_skipper():
        boost::spirit::qi::grammar<Iterator>::base_type(main_rule)
    {
        namespace qi = boost::spirit::qi;
        namespace ascii = boost::spirit::ascii;

        main_rule =
            boost::spirit::ascii::space
            |
            boost::spirit::repository::confix("/*", "*/")
                [*(qi::char_ - "*/")]
            ;
    }

    /** Main skipping rule. */
    boost::spirit::qi::rule<Iterator> main_rule;
};


} /* namespace grammar */
} /* namespace bakery */


#endif

