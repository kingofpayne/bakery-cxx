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
#include "floating.hpp"


TEST_CASE("dat::floating_t")
{
    SECTION("default constructor")
    {
        bakery::dat::floating_t f;
        REQUIRE( f.get_negative() == false );
        REQUIRE( f.get_integer_string() == "" );
        REQUIRE( f.get_decimal_string() == "" );
        REQUIRE( f.get_exponent_string() == "" );
    }

    SECTION("negative setter/getter")
    {
        bakery::dat::floating_t f;
        f.set_negative(true);
        REQUIRE( f.get_negative() == true );
        f.set_negative(false);
        REQUIRE( f.get_negative() == false );
    }

    SECTION("integer_string setter/getter")
    {
        bakery::dat::floating_t f;
        f.set_integer_string("1234");
        REQUIRE( f.get_integer_string() == "1234" );
    }
    
    SECTION("decimal_string setter/getter")
    {
        bakery::dat::floating_t f;
        f.set_decimal_string("4567");
        REQUIRE( f.get_decimal_string() == "4567" );
    }
    
    SECTION("exponent_string setter/getter")
    {
        bakery::dat::floating_t f;
        f.set_exponent_string("12");
        REQUIRE( f.get_exponent_string() == "12" );
        /* negative allowed for exponent */
        f.set_exponent_string("-42");
        REQUIRE( f.get_exponent_string() == "-42" );
    }

    SECTION("valid_format")
    {
        using bakery::dat::floating_t;
        REQUIRE( floating_t::valid_format("0", false) == true );
        REQUIRE( floating_t::valid_format("1", false) == true );
        REQUIRE( floating_t::valid_format("2", false) == true );
        REQUIRE( floating_t::valid_format("3", false) == true );
        REQUIRE( floating_t::valid_format("4", false) == true );
        REQUIRE( floating_t::valid_format("5", false) == true );
        REQUIRE( floating_t::valid_format("6", false) == true );
        REQUIRE( floating_t::valid_format("7", false) == true );
        REQUIRE( floating_t::valid_format("8", false) == true );
        REQUIRE( floating_t::valid_format("9", false) == true );
        REQUIRE( floating_t::valid_format("10", false) == true );
        REQUIRE( floating_t::valid_format("11", false) == true );
        REQUIRE( floating_t::valid_format("12", false) == true );
        REQUIRE( floating_t::valid_format("13", false) == true );
        REQUIRE( floating_t::valid_format("14", false) == true );
        REQUIRE( floating_t::valid_format("15", false) == true );
        REQUIRE( floating_t::valid_format("16", false) == true );
        REQUIRE( floating_t::valid_format("17", false) == true );
        REQUIRE( floating_t::valid_format("18", false) == true );
        REQUIRE( floating_t::valid_format("19", false) == true );
        REQUIRE( floating_t::valid_format("20", false) == true );
        REQUIRE( floating_t::valid_format("123456789123456789", false) == true );
        REQUIRE( floating_t::valid_format("-0", true) == true );
        REQUIRE( floating_t::valid_format("-1", true) == true );
        REQUIRE( floating_t::valid_format("-2", true) == true );
        REQUIRE( floating_t::valid_format("-3", true) == true );
        REQUIRE( floating_t::valid_format("-4", true) == true );
        REQUIRE( floating_t::valid_format("-5", true) == true );
        REQUIRE( floating_t::valid_format("-6", true) == true );
        REQUIRE( floating_t::valid_format("-7", true) == true );
        REQUIRE( floating_t::valid_format("-8", true) == true );
        REQUIRE( floating_t::valid_format("-9", true) == true );
        REQUIRE( floating_t::valid_format("-10", true) == true );
        REQUIRE( floating_t::valid_format("-11", true) == true );
        REQUIRE( floating_t::valid_format("-12", true) == true );
        REQUIRE( floating_t::valid_format("-13", true) == true );
        REQUIRE( floating_t::valid_format("-14", true) == true );
        REQUIRE( floating_t::valid_format("-15", true) == true );
        REQUIRE( floating_t::valid_format("-16", true) == true );
        REQUIRE( floating_t::valid_format("-17", true) == true );
        REQUIRE( floating_t::valid_format("-18", true) == true );
        REQUIRE( floating_t::valid_format("-19", true) == true );
        REQUIRE( floating_t::valid_format("-20", true) == true );
        REQUIRE( floating_t::valid_format("-123456789123456789", true) == true );
        REQUIRE( floating_t::valid_format("-1234", false) == false );
        REQUIRE( floating_t::valid_format("123a456", true) == false );
        REQUIRE( floating_t::valid_format("--1234", true) == false );
        REQUIRE( floating_t::valid_format("+1234", true) == false );
        REQUIRE( floating_t::valid_format("12 34", true) == false );
        REQUIRE( floating_t::valid_format("-", true) == false );
        REQUIRE( floating_t::valid_format("", false) == true );
    }
}
