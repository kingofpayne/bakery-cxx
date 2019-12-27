/**
 * Copyright (C) 2019
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


#include "bakery.hpp"
#include <iostream>


namespace bakery {


/**
 * Default constructor.
 */
bakery_t::bakery_t():
    force_rebuild(false),
    has_rebuilt_flag(false)
{}


/**
 * Set or unset force_rebuild switch.
 *
 * @param value New value.
 */
void bakery_t::set_force_rebuild(bool value)
{
    force_rebuild = value;
}


/**
 * @return force_rebuild setting.
 */
bool bakery_t::get_force_rebuild() const
{
    return force_rebuild;
}


/**
 * @return True if previous data load has rebuild the data cache.
 */
bool bakery_t::has_rebuilt() const
{
    return has_rebuilt_flag;
}


/**
 * Add a list of include directories.
 *
 * @param dirs List of include directories.
 */
void bakery_t::include(const std::list<std::string> & dirs)
{
    std::copy(dirs.begin(), dirs.end(),
        std::back_inserter(include_directories));
}


/**
 * Includes a directory which may contain recipe files.
 *
 * @param dir The directory.
 */
void bakery_t::include(const std::string & dir)
{
	include_directories.push_back(dir);
}


/**
 * @return List of directories which may contain recipe files.
 */
const std::list<std::string> & bakery_t::get_include_directories() const
{
	return include_directories;
}


} /* namespace bakery */
