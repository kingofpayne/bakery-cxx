/**
 * Copyright (C) 2010, 2011
 * Sofien Bouaziz, Olivier Hériveaux, Adrien Boussicault.
 *
 * Copyright (C) 2012
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
 * @author Sofien Bouaziz
 * @author Olivier Hériveaux
 * @author Adrien Boussicault
 */


#include "assertions.hpp"
#include "str.hpp"
#include <boost/regex.hpp>
#include <fstream>
#include <iostream>
#include <string>


namespace bakery {
namespace str {


/**
 * @return The given string with all '\t' occurences replaced by the input
 *         parameter string.
 *
 * @param s Indentation string.
 */
std::string indent(const std::string & s)
{
	return boost::regex_replace( s, boost::regex("^"), "\t" );
}


/**
 * Loads a file and pushes its content into a string.
 *
 * If the file cannot be opened, the program is aborted.
 *
 * @param path Path to the file
 *
 * @see load_from_file
 */
std::string assert_load_from_file(const std::string & path)
{
	std::string x;
	error_code_t ec = load_from_file(path, x);
	bakery_assert_message(!ec, "Failed to open file '" + path + "'.");
	return x;
}


/**
 * Loads a file and pushes its content into a string.
 *
 * @param path Path to the file.
 * @param out A string where the result is written in case of success.
 *
 * @return 0 in case of success, or an error code.
 *
 * @see assert_load_from_file
 */
error_code_t load_from_file(const std::string & path, std::string & out)
{
	std::ifstream stream(path.c_str(), std::ios::binary);

	/* Check if open failed */
	if (stream.fail())
		return 1;

	stream.seekg(0, std::ios::end);
	unsigned int length = stream.tellg();
	stream.seekg(0, std::ios::beg);

	/* Copy the file into a buffer of characters. Do not forget to add an extra
	 * character to mark the end of the string with a zero. */
	char* buffer = new char[length+1];
	stream.read(buffer, length);
	stream.close();
	buffer[length] = 0;

	out = buffer;
	delete[] buffer;
	return 0;
}


} /* namespace str */
} /* namespace bakery */

