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
#include "str.hpp"


TEST_CASE("bakery_t")
{
    /* Test state after default constructor. */
    SECTION("constructor")
    {
        bakery::bakery_t bk;
        REQUIRE( bk.get_include_directories().size() == 0 );
        REQUIRE( bk.get_include_directories() == std::list<std::string>() );
        REQUIRE( bk.get_force_rebuild() == false );
        REQUIRE( bk.get_verbose() == false );
        REQUIRE( bk.get_abort_on_error() == false );
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

    /* Test loading without bakery_t instanciation */
    SECTION("load with input")
    {
        bakery::input_t input = bakery::load("tests/int_42.dat");
        REQUIRE( input.good() == true );
        int a;
        input >> a;
        REQUIRE( a == 42 );
    }

    /* Load methods with variadic template arguments. */
    SECTION("load with args")
    {
        int a = 0;
        std::string b;
        bool result = bakery::load("tests/int_21_string_hello.dat", a, b);
        REQUIRE( result == true );
        REQUIRE( a == 21 );
        REQUIRE( b == "Hello world!" );
    }

    /* Test all native types. */
    SECTION("load native types")
    {
        bakery::bakery_t bak;
        bak.set_force_rebuild(true);
        bakery::input_t input = bak.load("tests/types.dat");
        REQUIRE( input.good() == true );

        int a;
        input >> a;
        REQUIRE( a == -42 );
        short b;
        input >> b;
        REQUIRE( b == 101 );
        char c;
        input >> c;
        REQUIRE( c == 127 );
        float d;
        input >> d;
        REQUIRE( d == Approx(3.14159265f) );
        double e;
        input >> e;
        REQUIRE( e == Approx(-3.14159265) );
        std::string f;
        input >> f;
        REQUIRE( f == "Hello world!" );
        std::pair<int, float> g;
        input >> g;
        REQUIRE( g.first == 99 );
        REQUIRE( g.second == 2.0 );
        std::tuple<int, float, std::string> h;
        input >> h;
        REQUIRE( std::get<0>(h) == 123456 );
        REQUIRE( std::get<1>(h) == -8.88f );
        REQUIRE( std::get<2>(h) == "tomato" );
        std::list<int> i;
        input >> i;
        REQUIRE( i == std::list<int>({5, 4, 3, 2, 1, 0}) );
        std::map<std::string, float> j;
        input >> j;
        REQUIRE( j.size() == 2 );
        REQUIRE( j["a"] == 6 );
        REQUIRE( j["b"] == 7 );
    };

    /* Test include directory effectiveness */
    SECTION("include")
    {
        bakery::bakery_t bak;
        bak.set_force_rebuild(true);
        bak.include("./tests/dir");
        bakery::input_t input = bak.load("tests/aaa.dat");
        REQUIRE( input.good() );
        std::string value;
        input >> value;
        REQUIRE( value == "aaa" );
    }

    SECTION("verbose getter/setter")
    {
        bakery::bakery_t bak;
        bak.set_verbose(false);
        REQUIRE( bak.get_verbose() == false );
        bak.set_verbose(true);
        REQUIRE( bak.get_verbose() == true );
        bak.set_verbose(false);
        REQUIRE( bak.get_verbose() == false );
    }

    SECTION("abort_on_error getter/setter")
    {
        bakery::bakery_t bak;
        bak.set_abort_on_error(false);
        REQUIRE( bak.get_abort_on_error() == false );
        bak.set_abort_on_error(true);
        REQUIRE( bak.get_abort_on_error() == true );
        bak.set_abort_on_error(false);
        REQUIRE( bak.get_abort_on_error() == false );
    }

    /* Test saving basic data and decompilation */
    SECTION("save")
    {
        std::string dat_out_path = "tests/out.dat";
        boost::filesystem::remove(dat_out_path);
        REQUIRE( boost::filesystem::exists(dat_out_path) == false );

        bakery::bakery_t bak;
        bakery::log_t log = bak.save(dat_out_path, "int_string.rec", 10,
            std::string("wololo"));
        std::string dat;
        bakery::str::load_from_file(dat_out_path, dat);
        REQUIRE( dat ==
            "recipe \"int_string.rec\";\n"
            "\n"
            "the_int = 10;\n"
            "the_string = \"wololo\";\n");
    }
}
