/**
 * Copyright (C) 2020
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
#include "log_message.hpp"


TEST_CASE("log_message_t")
{
    SECTION("default constructor")
    {
        bakery::log_message_t m;
        REQUIRE( m.type == bakery::log_message_type_t::info );
        REQUIRE( m.text == "" );
    }

    SECTION("constructor")
    {
        {
            bakery::log_message_t
                m(bakery::log_message_type_t::info, "hello");
            REQUIRE( m.type == bakery::log_message_type_t::info );
            REQUIRE( m.text == "hello" );
        }
        {
            bakery::log_message_t
                m(bakery::log_message_type_t::warning, "world");
            REQUIRE( m.type == bakery::log_message_type_t::warning );
            REQUIRE( m.text == "world" );
        }
        {
            bakery::log_message_t m(bakery::log_message_type_t::error, "err");
            REQUIRE( m.type == bakery::log_message_type_t::error );
            REQUIRE( m.text == "err" );
        }
    }

    SECTION("comparison operator")
    {
        bakery::log_message_t m1(bakery::log_message_type_t::info, "abcd");
        bakery::log_message_t m2(bakery::log_message_type_t::info, "abcd");
        bakery::log_message_t m3(bakery::log_message_type_t::warning, "abcd");
        bakery::log_message_t m4(bakery::log_message_type_t::info, "efgh");
        bakery::log_message_t m5(bakery::log_message_type_t::error, "efgh");
        REQUIRE( m1 == m2 );
        REQUIRE( m1 != m3 );
        REQUIRE( m1 != m4 );
        REQUIRE( m1 != m5 );
    }
}
