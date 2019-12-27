/**
 * Copyright (C) 2010, 2011, 2012
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


#include "enum_value_data.hpp"


namespace bakery {


/**
 * Constructor.
 */
enum_value_data_t::enum_value_data_t():
	value(0),
	fixed_value_flag(false),
	value_str()
{}


/**
 * @return The fixed value string found in the recipe file.
 */
const std::string & enum_value_data_t::get_value_str() const
{
	return value_str;
}


/**
 * @return True if a fixed value has been specified.
 */
bool enum_value_data_t::get_fixed_value_flag() const
{
	return fixed_value_flag;
}


/**
 * Sets a fixed value for the enumeration member.
 *
 * @param v The value.
 */
void enum_value_data_t::set_fixed_value(const std::string & v)
{
	fixed_value_flag = true;
	value_str = v;
}


} /* namespace bakery */

