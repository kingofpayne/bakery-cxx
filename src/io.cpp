/**
 * Copyright (C) 2019
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


#include "io.hpp"


namespace bakery {


/**
 * Constructor.
 *
 * @param stream Stream used for deserialization. Ownership is taken.
 */
input_t::input_t(std::istream* stream):
    stream(stream)
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


} /* namespace bakery */
