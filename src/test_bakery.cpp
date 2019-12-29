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
#include "bakery.hpp"


TEST_CASE("bakery_t")
{
    /* Test state after default constructor. */
    SECTION("constructor")
    {
        bakery::bakery_t bk;
        REQUIRE( bk.get_include_directories().size() == 0 );
        REQUIRE( bk.get_include_directories() == std::list<std::string>() );
        REQUIRE( bk.get_force_rebuild() == false );
    }

    /* Test include directory list management */
    SECTION("include")
    {
        bakery::bakery_t bk;
        bk.include({"aaa", "zzz", "bbb"});
        REQUIRE( bk.get_include_directories().size() == 3 );
        REQUIRE( bk.get_include_directories()
            == std::list<std::string>({"aaa", "zzz", "bbb"}) );
        bk.include("ccc");
        REQUIRE( bk.get_include_directories().size() == 4 );
        REQUIRE( bk.get_include_directories()
            == std::list<std::string>({"aaa", "zzz", "bbb", "ccc"}) );
    }

    /* Test force_rebuild switch getter/setter */
    SECTION("force_rebuild")
    {
        bakery::bakery_t bk;
        REQUIRE(bk.get_force_rebuild() == false );
        bk.set_force_rebuild(true);
        REQUIRE(bk.get_force_rebuild() == true );
        bk.set_force_rebuild(false);
        REQUIRE(bk.get_force_rebuild() == false );
    }

    /* Load basic data.
     * Test effectiveness of force_rebuild option. */
    SECTION("load force_rebuild")
    {
        int a;
        bakery::bakery_t bk;
        bk.set_force_rebuild(true);
        {
            bakery::input_t input = bk.load("tests/int_42.dat");
            REQUIRE( (bool)input == true );
            REQUIRE( input.good() == true );
            REQUIRE( input.has_rebuilt() == true );
            input >> a;
            REQUIRE( a == 42 );
        }
        /* Load again, verify that build occured again (force_rebuild flag is
         * set). */
        {
            a = 0;
            bakery::input_t input = bk.load("tests/int_42.dat");
            REQUIRE( (bool)input == true );
            REQUIRE( input.good() == true );
            REQUIRE( input.has_rebuilt() == true );
            input >> a;
            REQUIRE( a == 42 );
        }
        /* Disable force_rebuild switch, load again and verify it did not
         * rebuild this time. */
        bk.set_force_rebuild(false);
        {
            a = 0;
            bakery::input_t input = bk.load("tests/int_42.dat");
            REQUIRE( (bool)input == true );
            REQUIRE( input.good() == true );
            REQUIRE( input.has_rebuilt() == false );
            input >> a;
            REQUIRE( a == 42 );
        }
    }

    /* Test all native types. */
    SECTION("load native types")
    {
        int a;
        short b;
        char c;
        float d;
        double e;
        std::string f;
        std::pair<int, float> g;
        std::tuple<int, float, std::string> h;
        std::list<int> i;
        std::map<std::string, float> j;
    };
}
