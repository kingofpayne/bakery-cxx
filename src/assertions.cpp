/**
 * Copyright (C) 2012
 * Olivier Heriveaux
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


#include "assertions.hpp"
#include <iostream>


namespace bakery {


/**
 * Abort program execution and display the location of the error.
 *
 * @param message Message describing the error.
 * @param filename Name of the file in which the assertion failed.
 * @param line Error line number.
 */
void assertion_failed(const std::string & message, const char* filename,
	int line)
{
	std::cout << "Assertion failed: " << message << " (in " << filename << ':'
		<< line << ")" << std::endl;
	std::abort();
}


} /* namespace bakery */
