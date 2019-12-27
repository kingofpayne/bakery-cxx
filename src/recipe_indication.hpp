/**
 * Copyright (C) 2019
 * Olivier Heriveaux
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
 * @author Olivier Heriveaux
 */


#ifndef _BAKERY_RECIPE_INDICATION_HPP_
#define _BAKERY_RECIPE_INDICATION_HPP_


#include "def/type_instanciation.hpp"
#include "file_indication.hpp"


namespace bakery {


/**
 * Stores recipe specification of a data file.
 */
struct recipe_indication_t
{
    file_indication fi;
    std::optional<def::type_instanciation_t> ti;
};


} /* namespace bakery */


#endif
