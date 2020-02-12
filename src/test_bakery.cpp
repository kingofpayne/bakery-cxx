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


class test_class_t
{
    public:
        test_class_t(): a(0), b(){}

        /* Prevent copies: we want bakery to serialize using references. */
        test_class_t(const test_class_t &) = delete;
        test_class_t & operator = (const test_class_t &) = delete;

        int get_a() const
        {
            return a;
        }

        void set_a(int value)
        {
            a = value;
        }

        const std::string & get_b() const
        {
            return b;
        }

        void set_b(const std::string & value)
        {
            b = value;
        }

    private:
        int a;
        std::string b;
};


namespace bakery {
BAKERY_BASIC_SERIALIZER_BEGIN(test_class_t)
    io.template getter<test_class_t, int, &test_class_t::get_a>(x)
        .template setter<test_class_t, int, &test_class_t::set_a>(x)
        .template getter<test_class_t, const std::string &,
            &test_class_t::get_b>(x)
        .template setter<test_class_t, const std::string &,
            &test_class_t::set_b>(x);
BAKERY_BASIC_SERIALIZER_END
}


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
        bakery::bakery_t bk({"aaa"});
        REQUIRE( bk.get_include_directories()
            == std::list<std::string>({"aaa"}) );
        REQUIRE( bk.get_include_directories().size() == 1 );
        bk.include({"zzz", "bbb"});
        REQUIRE( bk.get_include_directories().size() == 3 );
        REQUIRE( bk.get_include_directories()
            == std::list<std::string>({"aaa", "zzz", "bbb"}) );
        bk.include("ccc");
        REQUIRE( bk.get_include_directories().size() == 4 );
        REQUIRE( bk.get_include_directories()
            == std::list<std::string>({"aaa", "zzz", "bbb", "ccc"}) );
        bk.include(std::list<std::string>({"uuu", "jjj"}));
        REQUIRE( bk.get_include_directories().size() == 6 );
        REQUIRE( bk.get_include_directories()
            == std::list<std::string>(
            {"aaa", "zzz", "bbb", "ccc", "uuu", "jjj"}) );
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
            bakery::log_t log = bk.load("tests/int_42.dat", a);
            REQUIRE( (bool)log == true );
            REQUIRE( log.good() == true );
            REQUIRE( log.has_rebuilt() == true );
            REQUIRE( a == 42 );
        }
        /* Load again, verify that build occured again (force_rebuild flag is
         * set). */
        {
            a = 0;
            bakery::log_t log = bk.load("tests/int_42.dat", a);
            REQUIRE( (bool)log == true );
            REQUIRE( log.good() == true );
            REQUIRE( log.has_rebuilt() == true );
            REQUIRE( a == 42 );
        }
        /* Disable force_rebuild switch, load again and verify it did not
         * rebuild this time. */
        bk.set_force_rebuild(false);
        {
            a = 0;
            bakery::log_t log = bk.load("tests/int_42.dat", a);
            REQUIRE( (bool)log == true );
            REQUIRE( log.good() == true );
            REQUIRE( log.has_rebuilt() == false );
            REQUIRE( a == 42 );
        }
    }

    /* Test loading without bakery_t instanciation */
    SECTION("load 1")
    {
        int a;
        bakery::log_t log = bakery::load("tests/int_42.dat", a);
        REQUIRE( log.good() == true );
        REQUIRE( a == 42 );
    }

    /* Load methods with multiple variadic template arguments. */
    SECTION("load 2")
    {
        int a = 0;
        std::string b;
        bool result = bakery::load("tests/int_21_string_hello.dat", a, b);
        REQUIRE( result == true );
        REQUIRE( a == 21 );
        REQUIRE( b == "Hello world!" );
    }

    /* Load with a const reference to a bakery */
    SECTION("load with const ref")
    {
        bakery::bakery_t bak;
        bak.set_force_rebuild(true);
        const bakery::bakery_t & bak2 = bak;
        int a;
        bakery::log_t log = bak2.load("tests/int_42.dat", a);
        REQUIRE( log.good() == true );
        REQUIRE( a == 42 );
    }

    /* Test all native types. */
    SECTION("load native types")
    {
        bakery::bakery_t bak;
        bak.set_force_rebuild(true);
        int a;
        short b;
        char c;
        float d;
        double e;
        std::string f;
        std::pair<int, float> g;
        std::tuple<int, float, std::string> h;
        std::list<int> i;
        std::vector<int> i2;
        std::map<std::string, float> j;
        enum k_t { first, second };
        k_t k;
        bakery::log_t log = bak.load("tests/types.dat", a, b, c, d, e, f, g, h,
            i, i2, j, k);
        REQUIRE( log.good() == true );
        REQUIRE( a == -42 );
        REQUIRE( b == 101 );
        REQUIRE( c == 127 );
        REQUIRE( d == Approx(3.14159265f) );
        REQUIRE( e == Approx(-3.14159265) );
        REQUIRE( f == "Hello world!" );
        REQUIRE( g.first == 99 );
        REQUIRE( g.second == 2.0 );
        REQUIRE( std::get<0>(h) == 123456 );
        REQUIRE( std::get<1>(h) == -8.88f );
        REQUIRE( std::get<2>(h) == "tomato" );
        REQUIRE( i == std::list<int>({5, 4, 3, 2, 1, 0}) );
        REQUIRE( i2 == std::vector<int>({11, 10, 9, 8, 7, 6}) );
        REQUIRE( j.size() == 2 );
        REQUIRE( j["a"] == 6 );
        REQUIRE( j["b"] == 7 );
        REQUIRE( k == 1 );
    };

    /* Test optional qualifier */
    SECTION("load optional")
    {
        bakery::bakery_t bak;
        bak.set_force_rebuild(true);
        int a = -1;
        std::string b = "default";
        int c = -1;
        std::string d = "default";
        bakery::log_t log;
        bakery::input_t input = bak.load_input("tests/optional.dat", log);
        REQUIRE( log.good() == true );
        input.optional(a).optional(b).optional(c).optional(d);
        REQUIRE( a == 99 );
        REQUIRE( b == "hello" );
        REQUIRE( c == -1 );
        REQUIRE( d == "default" );
    }

    /* Load using setters */
    SECTION("load setter")
    {
        test_class_t x;
        bool result = bakery::load("tests/int_21_string_hello.dat", x);
        REQUIRE( result == true );
        REQUIRE( x.get_a() == 21 );
        REQUIRE( x.get_b() == "Hello world!" );
    }

    /* Test include directory effectiveness */
    SECTION("include")
    {
        bakery::bakery_t bak;
        bak.set_force_rebuild(true);
        bak.include("./tests/dir");
        std::string value;
        bakery::log_t log = bak.load("tests/aaa.dat", value);
        REQUIRE( log.good() );
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

    /* Save with a const reference to a bakery */
    SECTION("save with const ref")
    {
        bakery::bakery_t bak;
        const bakery::bakery_t & bak2 = bak;

        std::string dat_out_path = "tests/out.dat";
        boost::filesystem::remove(dat_out_path);
        REQUIRE( boost::filesystem::exists(dat_out_path) == false );
        bakery::log_t log = bak2.save(dat_out_path, "int_string.rec", 10,
            std::string("wololo"));
    }

    /* Test getters when saving */
    SECTION("save getters")
    {
        test_class_t x;
        x.set_a(451);
        x.set_b("Fahrenheit");

        std::string dat_out_path = "tests/out.dat";
        boost::filesystem::remove(dat_out_path);
        REQUIRE( boost::filesystem::exists(dat_out_path) == false );
        bakery::bakery_t bak;
        bakery::log_t log = bak.save(dat_out_path, "int_string.rec", x);
        REQUIRE( log.good() == true );
        std::string dat;
        bakery::str::load_from_file(dat_out_path, dat);
        REQUIRE( dat ==
            "recipe \"int_string.rec\";\n"
            "\n"
            "the_int = 451;\n"
            "the_string = \"Fahrenheit\";\n");
    }
}
