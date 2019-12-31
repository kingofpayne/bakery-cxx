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


#include "member_data.hpp"


namespace bakery {
namespace rec {


/**
 * Default constructor.
 */
member_data_t::member_data_t():
    type_instanciation(),
    default_value_node()
{}


/**
 * @return A string representing the data.
 */
std::string member_data_t::print() const
{
    return type_instanciation.print();
}


/**
 * @return True if a default value has been specified.
 */
bool member_data_t::has_default_value() const
{
    return default_value_node.get() != 0;
}


} /* namespace rec */
} /* namespace bakery */

