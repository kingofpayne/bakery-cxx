/**
 * Copyright (C) 2014
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


#include "error_code.hpp"


namespace bakery {


/**
 * Constructor.
 *
 * @param value_ Code - non zero for errors.
 */
error_code_t::error_code_t(int value_):
    value(value_),
    ref(new int(1))
{}


/**
 * Copy constructor. This function does not do a deep copy of the error_code,
 * but creates an error_code pointing to the same code.
 *
 * @param ec Error code top copy.
 */
error_code_t::error_code_t(const error_code_t & ec):
    value(ec.value),
    ref(ec.ref)
{
    inc_ref();
}


/**
 * Destructor.
 */
error_code_t::~error_code_t()
{
    dec_ref();
}


/**
 * Assignment operator.
 *
 * The assigned error_code_t will point to the same error as the source.
 *
 * @param source Source error code.
 *
 * @return This.
 */
error_code_t & error_code_t::operator = (const error_code_t & source)
{
    if (this != &source)
    {
        dec_ref();
        value = source.value;
        ref = source.ref;
        inc_ref();
    }
    return *this;
}


/**
 * @return true if there is an error.
 * 
 * Calling this function marks the error code as checked.
 */
bool error_code_t::fail() const
{
    make_checked();
    return value != 0;
}
        

/**
 * @return true if there is no error.
 *
 * Calling this function marks the error code as checked.
 */
bool error_code_t::good() const
{
    make_checked();
    return value == 0;
}


} /* namespace bakery */
