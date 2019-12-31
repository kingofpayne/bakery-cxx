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
    SECTION("constructor a")
    {
        bakery::input_t x(0, false);
        REQUIRE( (bool)x == false );
        REQUIRE( x.has_rebuilt() == false );
    }

    SECTION("constructor b")
    {
        bakery::input_t x(new std::ifstream("README.md"), false);
        REQUIRE( (bool)x == true );
        REQUIRE( x.has_rebuilt() == false );
    }

    SECTION("constructor c")
    {
        bakery::input_t x(new std::ifstream("README.md"), true);
        REQUIRE( (bool)x == true );
        REQUIRE( x.has_rebuilt() == true );
    }
}
