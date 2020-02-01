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
#include "log.hpp"


TEST_CASE("log_t")
{
    SECTION("constructor")
    {
        bakery::log_t log;
        REQUIRE( log.get_error_count() == 0 );
        REQUIRE( (bool)log == true );
        REQUIRE( log.good() == true );
        REQUIRE( log.get_messages() == std::list<bakery::log_message_t>() );
        REQUIRE( log.to_string() == "" );
        REQUIRE( log.has_rebuilt() == false );
    }

    SECTION("add messages")
    {
        bakery::log_t log;
        log.error("abcd");
        REQUIRE( (bool)log == false );
        REQUIRE( log.good() == false );
        REQUIRE( log.get_error_count() == 1 );
        log.warning("defg");
        log.error("ijkl");
        REQUIRE( log.get_error_count() == 2 );
        REQUIRE( log.to_string() ==
            "Error: abcd\n"
            "Warning: defg\n"
            "Error: ijkl" );
    }

    SECTION("clear")
    {
        bakery::log_t log;
        log.warning("efgh");
        REQUIRE( (bool)log == true );
        REQUIRE( log.good() == true );
        log.error("abcd");
        log.error("ijkl");
        REQUIRE( log.get_error_count() == 2 );
        REQUIRE( (bool)log == false );
        REQUIRE( log.good() == false );
        REQUIRE( log.get_messages().size() == 3 );
        log.clear();
        REQUIRE( log.get_error_count() == 0 );
        REQUIRE( (bool)log == true );
        REQUIRE( log.good() == true );
        REQUIRE( log.get_messages().size() == 0 );
    }

    SECTION("size")
    {
        bakery::log_t log;
        REQUIRE( log.size() == 0 );
        log.error("abcd");
        REQUIRE( log.size() == 1 );
        log.warning("efgh");
        REQUIRE( log.size() == 2 );
        log.error("ijkl");
        REQUIRE( log.size() == 3 );
    }

    SECTION("rebuilt setter/getter")
    {
        bakery::log_t x;
        x.set_rebuilt(false);
        REQUIRE( x.has_rebuilt() == false );
        x.set_rebuilt(true);
        REQUIRE( x.has_rebuilt() == true );
        x.set_rebuilt(false);
        REQUIRE( x.has_rebuilt() == false );
    }

}
