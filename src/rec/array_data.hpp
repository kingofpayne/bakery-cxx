/**
 * Copyright (C) 2011, 2012, 2013
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


#ifndef _BAKERY_DEF_ARRAY_DATA_HPP_
#define _BAKERY_DEF_ARRAY_DATA_HPP_


#include "type_instanciation.hpp"


namespace bakery {
namespace rec {


/**
 * An array type node. Array can be simple or multi-dimensional.
 */
struct array_data_t
{
    array_data_t();
    void add_dim(size_t);
        
    /** The type of the array. */
    type_instanciation_t type_instanciation;
    /** Size of each dimension of the array. The size of this list is the
     * dimension of the array. First element is the first dimension. For
     * instance, if the type is declared as int[1][2][3], then the list will
     * be {1, 2, 3}.
     * For each dimension, if the size if zero, then the size is not fixed
     * and the dimension is dynamic - the size of the dimension will be
     * written in the binary output file. If the size is strictly positive,
     * then the dimension is fixed - the size of the dimension won't be
     * written in the binary output file. */
    std::vector<size_t> dims;
};


} /* namespace rec */
} /* namespace bakery */


#endif

