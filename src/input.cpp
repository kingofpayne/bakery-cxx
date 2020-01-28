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
 * Default constructor. Set stream to null and rebuilt flag to false. The input
 * cannot be deserialized after default construction.
 */
input_t::input_t(): stream(0), rebuilt_flag(false) {}


/**
 * Move constructor.
 *
 * @param other Moved instance.
 */
input_t::input_t(input_t && other):
    stream(other.stream),
    rebuilt_flag(other.rebuilt_flag),
    log(std::move(other.log))
{
    other.stream = 0;
    other.rebuilt_flag = false;
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
    rebuilt_flag = other.rebuilt_flag;
    other.rebuilt_flag = false;
    log = std::move(other.log);
    return *this;
}


/**
 * @return True if Bakery successfully opened the file. False if any error
 *         occured. Errors can be retrieved in the log.
 */
bool input_t::good() const
{
    return (stream != 0) && ((bool)*stream) && (log.get_error_count() == 0);
}


/**
 * @return True if Bakery successfully opened the file. False if any error
 *         occured. Errors can be retrieved in the log.
 */
input_t::operator bool() const
{
    return good();
}


/**
 * Set the rebuilt flag value. Called by bakery when loading a data file.
 */
void input_t::set_rebuilt(bool value)
{
    rebuilt_flag = value;
}


/**
 * @return True if the binary has been rebuilt. False if it has been loaded from
 *         cache.
 */
bool input_t::has_rebuilt() const
{
    return rebuilt_flag;
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


/**
 * @return Compilation log.
 */
log_t & input_t::get_log()
{
    return log;
}


/**
 * @return Compilation log.
 */
const log_t & input_t::get_log() const
{
    return log;
}


} /* namespace bakery */
