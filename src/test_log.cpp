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


TEST_CASE("compilation_log_t")
{
    SECTION("constructor")
    {
        bakery::log_t log;
        REQUIRE( log.get_error_count() == 0 );
        REQUIRE( log.get_messages() == std::list<bakery::log_message_t>() );
        REQUIRE( log.print() == "" );
    }

    SECTION("add messages")
    {
        bakery::log_t log;
        log.add_error("abcd");
        REQUIRE( log.get_error_count() == 1 );
        log.add_warning("defg");
        log.add_error("ijkl");
        REQUIRE( log.get_error_count() == 2 );
        REQUIRE( log.print() ==
            "Error: abcd\n"
            "Warning: defg\n"
            "Error: ijkl" );
    }
}
