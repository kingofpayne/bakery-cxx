/**
 * Copyright (C) 2019
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


#include <catch2/catch.hpp>
#include "grammar.hpp"


TEST_CASE("grammar integer rule")
{
    bakery::grammar::rule_container_t<bakery::grammar::iterator> rules;
    bakery::grammar::init_integer(rules);

    struct test_vector_t
    {
        std::string code;
        bool good;
    };

    std::vector<test_vector_t> vectors =
    {
        {"0", true},
        {"1", true},
        {"2", true},
        {"3", true},
        {"4", true},
        {"5", true},
        {"6", true},
        {"7", true},
        {"8", true},
        {"9", true},
        {"10", true},
        {"11", true},
        {"12", true},
        {"13", true},
        {"14", true},
        {"15", true},
        {"16", true},
        {"17", true},
        {"18", true},
        {"19", true},
        {"20", true},
        {"1234", true},
        {"123456789123456789123456789123456789", true},
        {"-0", true},
        {"-1", true},
        {"-2", true},
        {"-3", true},
        {"-4", true},
        {"-5", true},
        {"-6", true},
        {"-7", true},
        {"-8", true},
        {"-9", true},
        {"-10", true},
        {"-11", true},
        {"-12", true},
        {"-13", true},
        {"-14", true},
        {"-15", true},
        {"-16", true},
        {"-17", true},
        {"-18", true},
        {"-19", true},
        {"-20", true},
        {"-1234", true},
        {"-123456789123456789123456789123456789", true},
        {"1234a567", false},
        {"--1234", false},
        {"+1234", false},
        {"123 456", false}
    };

    for (const auto & vector: vectors)
    {
        std::string::const_iterator it = vector.code.begin();
        std::string::const_iterator it_end = vector.code.end();
        std::string result = "";
        REQUIRE( result == "" );
        boost::spirit::qi::phrase_parse(it, it_end, rules.integer,
            bakery::grammar::skipper_t(), result);
        if (vector.good)
        {
            REQUIRE( result == vector.code );
            REQUIRE( it == it_end );
        }
        else
        {
            REQUIRE( it != it_end );
        }
    }
}
