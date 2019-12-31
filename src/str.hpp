/**
 * Copyright (C) 2010, 2011
 * Sofien Bouaziz, Olivier Hériveaux, Adrien Boussicault.
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
 * @author Clément Iatrides
 */


#ifndef _BAKERY_STR_HPP_
#define _BAKERY_STR_HPP_


#include "error_code.hpp"
#include <limits>
#include <list>
#include <sstream> 
#include <string>


namespace bakery {
namespace str {


std::string indent(const std::string & s);
std::string assert_load_from_file(const std::string &);
error_code_t load_from_file(const std::string &, std::string &);


/**
 * Converts an hexadecimal string to a numeric type. This function is safe: if
 * the input string is invalid, an exception is thrown, if the number
 * represented by the input string is greater than the max possible value which
 * can be stored in the return type, an exception is thrown.
 *
 * @param <T> Integral numeric type.
 *
 * @param hex_string Input hexadecimal string. All characters must belong to
 *        [0-9a-fA-F]. An empty string is considered 0.
 *
 * @return Value represented by the hexadecimal string.
 */
template <typename T> T assert_hex_to(const std::string & hex_string)
{
    T result = 0;
    const size_t s = hex_string.size();

    for (size_t i=0; i<s; ++i)
    {
        /* Fetch char. */
        char c = hex_string[i];

        /* Convert char to a semi-octet. */
        unsigned char x;

        if ((c >= '0') && (c <= '9'))
            x = c - '0';
        else if ((c >= 'a') && (c <= 'f'))
            x = c - 'a' + 0x10;
        else if ((c >= 'A') && (c <= 'F'))
            x = c - 'A' + 0x10;
        else
            bakery_abort_message("invalid hexadecimal string '" + hex_string
                + "'.");

        /* Verify that the value will fit the numeric type. */
        z_assert_message(
            result <= (std::numeric_limits<T>::max() >> 4),
            "hexadecimal string '" + hex_string + "' is too big.");

        result = (result << 4) | ((T)x);
    }

    return result;
}


/**
 * Converts a data of type T into text. This function uses stl streams for
 * conversion.
 *
 * Example:
 * float x = 3.14159;
 * std::string test = bakery::str::from<float>(x);
 *
 * @param <T> Type of the data to be converted to a string.
 * @param x Data to be converted to a string.
 *
 * @return Conversion result.
 */
template <typename T> std::string from(const T& x)
{
    std::ostringstream stm;
    stm << x;
    return stm.str();
}


/**
 * Get the size of a data of type T into text. This function uses stl 
 * streams for conversion.
 *
 * @param <T> Type of the data to be converted to a string.
 * @param x Data to be converted to a string.
 *
 * @return Conversion result.
 */
template <typename T> int size(const T& x)
{
    std::string s = from(x);
    return s.size();
}


/**
 * Converts a list of datas of type T into text. This function uses stl 
 * streams for conversion.
 *
 * Example:
 * std::list<std::string> l;
 * l.push_back( "club" );
 * l.push_back( "pc" );
 * std::string test = bakery::str::from( l );
 *
 * @param <T> Type of the data inside the list.
 * @param l list to be converted to a string.
 *
 * @return Conversion result.
 */
template <typename T> std::string from(const std::list<T> & l)
{
    std::ostringstream stm;
    stm << "[";
    for( const T & v : l ){
        stm << v;
        stm << ", ";
    }
    stm << "]";
    return stm.str();
}


/**
 * Converts a floating numeric value (float or double) to a string, with full
 * digits precision.
 *
 * @param x Value.
 *
 * @tparam T Type of the numeric value. Must be float or double.
 *
 * @return A string.
 */
template <typename T> std::string floating_to_string(T x)
{
    std::ostringstream s;
    /* digits10 is the number of decimal digits the floating type can represent
     * without any loss in precision. We add +1 for rounding for best
     * approximation. */
    s.precision(std::numeric_limits<T>::digits10+1);
    s << x;
    return s.str();
}


/**
 * Converts a string to any type, and throw an exception if conversion fails.
 *
 * @param <T> Type of the result.
 * @param s Input string to be converted.
 *
 * @return Conversion result.
 */
template <typename T> T assert_to(std::string s)
{
    std::istringstream stm(s);
    T result;
    stm >> result;

    bakery_assert_message(
        (stm.tellg() == s.size()),
        "String conversion failed.");

    return result;
}


/**
 * Converts a string to any type.
 *
 * @param <T> Type of the result.
 * @param s Input string to be converted.
 * @param out Variable where the result is set in case of success. May be
 *        partially written in case of error.
 *
 * @return 0 in case of success, an error code otherwise.
 */
template <typename T> error_code_t to(std::string s, T & out)
{
    std::istringstream stm(s);
    T result;
    stm >> out;
    return (stm.tellg() == s.size())?0:1;
}


} /* namespace str */
} /* namespace bakery */


#endif

