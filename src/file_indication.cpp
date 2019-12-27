/**
 * Copyright (C) 2010, 2011, 2012
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


#include "file_indication.hpp"
#include <iostream>


namespace bakery {


/**
 * @return the path to the recipe file. This path may be absolute or relative.
 */
const std::string & file_indication::get_path() const
{
	return path;
}


/**
 * @return True if the path is absolute, false if relative.
 */
bool file_indication::get_absolute() const
{
	return absolute;
}


/**
 * @return A string representing the content of the object.
 */
std::string file_indication::print() const
{
	return
		(absolute?"<":"\"")
		+ path
		+ (absolute?">":"\"");
}


/**
 * Defines the path.
 *
 * @param value path. May be relative or absolute.
 */
void file_indication::set_path(const std::string & value)
{
	path = value;
}


/**
 * Defines if the path is relative or absolute.
 *
 * @param value True if the path is absolute, false otherwise.
 */
void file_indication::set_absolute(bool value)
{
	absolute = value;
}


} /* namespace bakery */

