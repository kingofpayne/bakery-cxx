/**
 * Copyright (C) 2011, 2012
 * Olivier Heriveaux.
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


#include "array_data.hpp"


namespace bakery {
namespace def {


/**
 * Constructor.
 */
array_data_t::array_data_t()
{}


/**
 * Adds a dimension to the array.
 *
 * @param dim Dimension size. If 0, dimension is fixed.
 */
void array_data_t::add_dim(size_t dim)
{
	/* Note: this function is not usefull since the dims member is public.
	 * However, it is convenient for the grammar parser. */
	dims.push_back(dim);
}


} /* namespace def */
} /* namespace bakery */

