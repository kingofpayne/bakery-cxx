/**
 * Copyright (C) 2010, 2011, 2012, 2013, 2014
 * Olivier Hériveaux.
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


#ifndef _BAKERY_DEF_TYPEDEF_DATA_HPP_
#define _BAKERY_DEF_TYPEDEF_DATA_HPP_


#include "type_instanciation.hpp"
#include "../dat/node.hpp"


namespace bakery {
namespace rec {


/**
 * Data for a typedef node.
 */
struct typedef_data_t
{
    typedef_data_t();
    typedef_data_t(const type_instanciation_t &);
    std::string print() const;

    /** The type instanciation of the typedef. */
    type_instanciation_t type_instanciation;
};


} /* namespace rec */
} /* namespace bakery */


#endif

