/**
 * Copyright (C) 2011, 2012
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


#ifndef _BAKERY_DEF_STRUCTURE_DATA_HPP_
#define _BAKERY_DEF_STRUCTURE_DATA_HPP_


#include "type_instanciation.hpp"
#include <vector>


namespace bakery {
namespace rec {


/**
 * data associated to structure nodes.
 */
struct structure_data_t
{
    structure_data_t();
    void add_heritance_type(const type_instanciation_t &);

    /** types from which the structure heritates. Note: the order is
     * relevant, a std::set cannot be used. */
    std::vector<type_instanciation_t> heritance_list;
};


} /* namespace rec */
} /* namespace bakery */


#endif

