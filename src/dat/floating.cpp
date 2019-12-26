/**
 * Copyright (C) 2010, 2011, 2012, 2013
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


#include <algorithm>
#include <functional>
#include "floating.hpp"
#include "../assertions.hpp"
#include <boost/bind.hpp>


namespace bakery {
namespace dat {


/**
 * Constructor. All parts strings are initialized to empty strings.
 */
floating::floating():
	negative(false),
	integer_string(),
	decimal_string(),
	exponent_string()
{}


/**
 * @return True if the floating is negative, false otherwise.
 */
bool floating::get_negative() const
{
	return negative;
}


/**
 * @return The string representing the integer part of the floating number, or
 *         an empty string (meaning 0).
 */
const std::string & floating::get_integer_string() const
{
	return integer_string;
}


/**
 * @return The string representing the decimal part of the floating number, or
 *         an empty string (meaning 0).
 */
const std::string & floating::get_decimal_string() const
{
	return decimal_string;
}


/**
 * @return The string representing the exponent part of the floating number, or
 *         an empty string (meaning 0).
 */
const std::string & floating::get_exponent_string() const
{
	return exponent_string;
}


/**
 * @return True if the integer part string is not empty.
 */
bool floating::has_integer_part() const
{
	return !integer_string.empty();
}


/**
 * @return True if the decimal part string is not empty.
 */
bool floating::has_decimal_part() const
{
	return !decimal_string.empty();
}


/**
 * @return True if the exponent part string is not empty.
 */
bool floating::has_exponent_part() const
{
	return !exponent_string.empty();
}


/**
 * Defines wheter the floating is negative or not.
 *
 * @param value True for making the floating negative.
 */
void floating::set_negative(bool value)
{
	negative = value;
}


/**
 * Defines the string representing the integer part of the floating number.
 *
 * @param value A string matching the regex '-?[0-9]*'.
 */
void floating::set_integer_string(const std::string & value)
{
	bakery_assert(valid_format(value, true));
	integer_string = value;
}


/**
 * Defines the string representing the decimal part of the floating number.
 *
 * @param value A string matching the regex '[0-9]*'.
 *
 * @throw core::exceptions::invalid_parameter* if value has a wrong format.
 */
void floating::set_decimal_string(const std::string & value)
{
	bakery_assert(valid_format(value, false));
	decimal_string = value;
}


/**
 * Defines the string representing the exponent of the floating number.
 *
 * @param value A string matching the regex '-?[0-9]*'.
 *
 * @throw core::exceptions::invalid_parameter* if value has a wrong format.
 */
void floating::set_exponent_string(const std::string & value)
{
	bakery_assert(valid_format(value, true));
	exponent_string = value;
}


/**
 * Checks the format of a string. An empty string always matches.
 *
 * @param s The string to check.
 * @param n True if the number represented by s can be negative.
 *
 * @return True if s has the right format.
 */
bool floating::valid_format(const std::string & s, bool n)
{
	if(s.length() == 0)
		return true;

	if(n)
	{
		char c0 = s[0];
		if(!std::isdigit(c0) && !(c0=='-'))
			return false;
	}

	return std::find_if(
		s.begin()+1,
		s.end(),
		boost::bind(
			std::logical_not<bool>(),
			boost::bind(
				(int(*)(int))
				&std::isdigit,
				_1
			)
		)
	) == s.end();
}


} /* namespace dat */
} /* namespace bakery */

