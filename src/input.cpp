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
 * Default constructor. Set stream to null. The input cannot be deserialized
 * after default construction.
 */
input_t::input_t(): stream(0) {}


/**
 * Move constructor.
 *
 * @param other Moved instance.
 */
input_t::input_t(input_t && other):
    stream(other.stream)
{
    other.stream = 0;
}


/**
 * Destructor. Closes the stream.
 */
input_t::~input_t()
{
    delete stream;
}


/**
 * Move assignment
 *
 * @param other Moved instance.
 */
input_t & input_t::operator = (input_t&& other)
{
    delete stream;
    this->stream = other.stream;
    other.stream = 0;
    return *this;
}


/**
 * @return True if Bakery successfully opened the file.
 */
bool input_t::good() const
{
    return (stream != 0) && ((bool)*stream);
}


/**
 * @return True if Bakery successfully opened the file.
 */
input_t::operator bool() const
{
    return good();
}


/**
 * Sets the stream used for deserialization.
 *
 * @param stream Stream pointer. This class takes ownership of the pointer.
 */
void input_t::set_stream(std::istream* stream)
{
    delete this->stream;
    this->stream = stream;
}


} /* namespace bakery */
