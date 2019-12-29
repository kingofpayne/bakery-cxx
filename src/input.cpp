/**
 * Copyright (C) 2019
 * Olivier Hériveaux
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


#include "input.hpp"


namespace bakery {


/**
 * Constructor.
 *
 * @param stream Stream used for deserialization. Ownership is taken. Null is
 *        allowed, and this means bakery failed to build or load the file.
 * @param rebuilt_flag Indicates if the binary has been rebuilt or not.
 */
input_t::input_t(std::istream* stream, bool rebuilt_flag):
    stream(stream),
    rebuilt_flag(rebuilt_flag)
{}


/**
 * Move constructor.
 *
 * @param Moved instance.
 */
input_t::input_t(input_t && rhs): stream(rhs.stream) {}


/**
 * Destructor. Closes the stream.
 */
input_t::~input_t()
{
    delete stream;
}


/**
 * @return True if Bakery successfully opened the file. False if any error
 *         occured.
 */
bool input_t::good() const
{
    return (stream != 0) && ((bool)*stream);
}


/**
 * @return True if Bakery successfully opened the file. False if any error
 *         occured.
 */
input_t::operator bool() const
{
    return good();
}


/**
 * @return True if the binary has been rebuilt. False if it has been loaded from
 *         cache.
 */
bool input_t::has_rebuilt() const
{
    return rebuilt_flag;
}


} /* namespace bakery */
