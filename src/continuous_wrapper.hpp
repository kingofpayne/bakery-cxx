/**
 * Copyright (C) 2010, 2011, 2012, 2013
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


#ifndef _BAKERY_CONTINUOUS_WRAPPER_HPP_
#define _BAKERY_CONTINUOUS_WRAPPER_HPP_


#include "assertions.hpp"


namespace bakery {


/**
 * Class to wrap an enumeration with continuous values.
 *
 * Usage example:
 *
 * struct my_enum_t
 * {
 *     enum type
 *     {
 *         one,
 *         two,
 *         three
 *     };
 *
 *     static const std::string strings[]; // to be defined in cpp file.
 *
 *     typedef continuous_wrapper_t< type, strings, one, three > wrapper_t;
 * };
 */
template <typename T, const std::string strings[], T min, T max>
    struct continuous_wrapper_t
{
    static bool has(T);
    static const std::string & to_string(T);
};


/**
 * @return True is the value is a member of the enumeration.
 *
 * @param value Value to test.
 */
template <typename T, const std::string strings[], T min, T max>
    bool continuous_wrapper_t<T, strings, min, max>::has(T value)
{
    return (value >= min) && (value <= max);
}


/**
 * @return A string representing the value.
 *
 * @param value Must be a member of the enumeration.
 *
 * @throws core::exceptions::invalid_enum_parameter* if value is not a member of
 *         the enumeration.
 */
template <typename T, const std::string strings[], T min, T max>
    const std::string &
    continuous_wrapper_t<T, strings, min, max>::to_string(T value)
{
    bakery_assert_message(has(value), "Invalid enumeration value.");
    return strings[value - min];
}


} /* namespace bakery */


#endif

