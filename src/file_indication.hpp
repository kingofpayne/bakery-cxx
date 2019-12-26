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


#ifndef _BAKERY_FILE_INDICATION_HPP_
#define _BAKERY_FILE_INDICATION_HPP_


#include <string>


namespace bakery {


/**
 * Represents the definition file specification in a data file.
 */
class file_indication
{
	public:
		const std::string & get_path() const;
		bool get_absolute() const;
		std::string print() const;

		void set_path(const std::string &);
		void set_absolute(bool);
	private:
		/** path to the definition file. */
		std::string path;

		/** True is the path is absolute, false if the path is relative. */
		bool absolute;
};


} /* namespace bakery */


#endif

