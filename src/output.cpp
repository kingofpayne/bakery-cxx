/**
 * Copyright (C) 2020
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


#include "output.hpp"


namespace bakery {


/**
 * Default constructor. Set stream to null. The output cannot be serialized
 * after default construction.
 */
output_t::output_t(): stream(0) {}


/**
 * Move constructor.
 *
 * @param other Moved instance.
 */
output_t::output_t(output_t && other):
    stream(other.stream),
    log(std::move(other.log))
{
    other.stream = 0;
}


/**
 * Destructor. Closes the stream.
 */
output_t::~output_t()
{
    delete stream;
}


/**
 * Move assignment
 *
 * @param other Moved instance.
 */
output_t & output_t::operator = (output_t&& other)
{
    delete stream;
    stream = other.stream;
    other.stream = 0;
    log = std::move(other.log);
    return *this;
}


/**
 * @return True if Bakery successfully opened the file. False if any error
 *         occured. Errors can be retrieved in the log.
 */
bool output_t::good() const
{
    return (stream != 0) && ((bool)*stream) && (log.get_error_count() == 0);
}


/**
 * @return True if Bakery successfully opened the file. False if any error
 *         occured. Errors can be retrieved in the log.
 */
output_t::operator bool() const
{
    return good();
}


/**
 * Sets the stream used for serialization.
 *
 * @param stream Stream pointer. This class takes ownership of the pointer.
 */
void output_t::set_stream(std::ostream* stream)
{
    delete this->stream;
    this->stream = stream;
}


/**
 * @return Compilation log.
 */
log_t & output_t::get_log()
{
    return log;
}


/**
 * @return Compilation log.
 */
const log_t & output_t::get_log() const
{
    return log;
}


} /* namespace bakery */
