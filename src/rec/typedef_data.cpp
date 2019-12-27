/**
 * Copyright (C) 2010, 2011, 2012, 2013, 2014
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


#include "typedef_data.hpp"


namespace bakery {
namespace rec {


/**
 * Default constructor.
 */
typedef_data_t::typedef_data_t():
	type_instanciation()
{}


/**
 * Constructor.
 *
 * @param type_instanciation_ The type instanciation.
 */
typedef_data_t::typedef_data_t
	(const type_instanciation_t & type_instanciation_)
:
	type_instanciation(type_instanciation_)
{}


/**
 * @return A string representing the data.
 */
std::string typedef_data_t::print() const
{
	return type_instanciation.print();
}


} /* namespace rec */
} /* namespace bakery */

