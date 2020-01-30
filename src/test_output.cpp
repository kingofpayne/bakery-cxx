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
#include "output.hpp"
#include <fstream>


TEST_CASE("output_t")
{
    SECTION("default constructor")
    {
        bakery::output_t x;
        REQUIRE( (bool)x == false );
        REQUIRE( x.good() == false );
    }

    SECTION("move constructor")
    {
        bakery::output_t a;
        a.set_stream(new std::ofstream("test-out"));
        a.get_log().warning("abcd");
        REQUIRE( a.good() == true );

        bakery::output_t b(std::move(a));
        REQUIRE( a.good() == false );
        REQUIRE( a.get_log().size() == 0 );
        REQUIRE( b.good() == true );
        REQUIRE( b.get_log().size() == 1 );

        bakery::output_t c(std::move(b));
        REQUIRE( b.good() == false );
        REQUIRE( b.get_log().size() == 0 );
        REQUIRE( c.good() == true );
        REQUIRE( c.get_log().size() == 1 );
    }

    SECTION("move assignment")
    {
        bakery::output_t a;
        a.set_stream(new std::ofstream("test-out"));
        a.get_log().warning("abcd");
        REQUIRE( a.good() == true );
        
        bakery::output_t b;
        REQUIRE( b.good() == false );
        b = std::move(a);
        REQUIRE( a.good() == false );
        REQUIRE( a.get_log().size() == 0 );
        REQUIRE( b.good() == true );
        REQUIRE( b.get_log().size() == 1 );

        bakery::output_t c;
        REQUIRE( c.good() == false );
        c = std::move(b);
        REQUIRE( b.good() == false );
        REQUIRE( b.get_log().size() == 0 );
        REQUIRE( c.good() == true );
        REQUIRE( c.get_log().size() == 1 );
    }
    
    SECTION("stream setter and good")
    {
        bakery::output_t x;
        /* case where good is false already tested previously with default
         * constructor */
        x.set_stream(new std::ofstream("test-out"));
        REQUIRE( (bool)x == true );
        REQUIRE( x.good() == true );
    }
    
    SECTION("log getter")
    {
        bakery::output_t x;
        bakery::log_t & log = x.get_log();
        /* avoid unused variable warning */
        REQUIRE( log.get_error_count() == 0 );
    }

    SECTION("log const getter")
    {
        const bakery::output_t x;
        const bakery::log_t & log = x.get_log();
        /* avoid unused variable warning */
        REQUIRE( log.get_error_count() == 0 );
    }

    SECTION("log and good")
    {
        bakery::output_t x;
        x.set_stream(new std::ofstream("test-out"));
        REQUIRE( x.good() == true );
        x.get_log().warning("abcd");
        REQUIRE( x.good() == true );
        x.get_log().error("abcd");
        REQUIRE( x.good() == false );
    }
}
