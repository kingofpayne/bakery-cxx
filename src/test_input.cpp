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
#include "input.hpp"
#include <fstream>


TEST_CASE("input_t")
{
    SECTION("default constructor")
    {
        bakery::input_t x;
        REQUIRE( (bool)x == false );
        REQUIRE( x.good() == false );
        REQUIRE( x.has_rebuilt() == false );
    }

    SECTION("stream setter and good")
    {
        bakery::input_t x;
        x.set_stream(new std::ifstream("README.md"));
        REQUIRE( (bool)x == true );
        REQUIRE( x.good() == true );
    }

    SECTION("rebuilt setter/getter")
    {
        bakery::input_t x;
        x.set_rebuilt(false);
        REQUIRE( x.has_rebuilt() == false );
        x.set_rebuilt(true);
        REQUIRE( x.has_rebuilt() == true );
        x.set_rebuilt(false);
        REQUIRE( x.has_rebuilt() == false );
    }

    SECTION("log getter")
    {
        bakery::input_t x;
        bakery::log_t & log = x.get_log();
        /* avoid unused variable warning */
        REQUIRE( log.get_error_count() == 0 );
    }

    SECTION("log const getter")
    {
        const bakery::input_t x;
        const bakery::log_t & log = x.get_log();
        /* avoid unused variable warning */
        REQUIRE( log.get_error_count() == 0 );
    }

    SECTION("log and good")
    {
        bakery::input_t x;
        x.set_stream(new std::ifstream("README.md"));
        REQUIRE( x.good() == true );
        x.get_log().warning("abcd");
        REQUIRE( x.good() == true );
        x.get_log().error("abcd");
        REQUIRE( x.good() == false );
    }
}
