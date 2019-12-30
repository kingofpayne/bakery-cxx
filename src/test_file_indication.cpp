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
#include "file_indication.hpp"


TEST_CASE("file_indication_t")
{
    bakery::file_indication_t f;
    REQUIRE( f.get_path() == "" );
    REQUIRE( f.get_absolute() == false );
    REQUIRE( f.print() == "\"\"" );

    f.set_absolute(true);
    REQUIRE( f.get_absolute() == true );
    REQUIRE( f.print() == "<>" );

    f.set_path("test/test");
    REQUIRE( f.get_path() == "test/test" );
    REQUIRE( f.print() == "<test/test>" );

    f.set_absolute( false);
    REQUIRE( f.print() == "\"test/test\"" );
}

