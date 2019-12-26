/**
 * Copyright (C) 2010
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


#ifndef _BAKERY_UTIL_HPP_
#define _BAKERY_UTIL_HPP_


#include <ostream>
#include <string>
#include <boost/filesystem.hpp>


namespace bakery {


std::string indent(const std::string &);

std::string show_string_position (
	const std::string &,
	const std::string::const_iterator);

unsigned int calculate_line_number(
	const std::string &,
	const std::string::const_iterator);


/**
 * Writes a data in a stream.
 *
 * @param s An output stream.
 * @param d Data to be written.
 *
 * @tparam T Type of the data to be written. Can be any native type or POD.
 */
template <typename T> void stream_write_binary(std::ostream & s, const T & d)
{
	s.write((char*)(&d), sizeof(d));
}


/* These functions are a complement of boost::filesystem library. */
boost::filesystem::path make_absolute(const boost::filesystem::path &);
boost::filesystem::path make_relative(boost::filesystem::path,
	boost::filesystem::path);
boost::filesystem::path path_from_base(const boost::filesystem::path &,
	const boost::filesystem::path &);


} /* namespace bakery */


#endif

