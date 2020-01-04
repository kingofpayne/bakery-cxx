/**
 * Copyright (C) 2010-2020
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


#include "log.hpp"
#include <algorithm>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/if.hpp>
#include <boost/lambda/lambda.hpp>
#include <iostream>


namespace bakery {


/**
 * Constructor
 */
log_t::log_t():
    error_count(0)
{}


/**
 * @return Count of error messages.
 */
size_t log_t::get_error_count() const
{
    return error_count;
}


/**
 * @return A string representing the status. It contains all messages.
 */
std::string log_t::to_string() const
{
    std::string r;
    bool add_nl = false;

    using boost::lambda::var;

    std::for_each(
        messages.begin(),
        messages.end(),
        (
            boost::lambda::if_then_else(
                var(add_nl),
                var(r) += "\n",
                var(add_nl) = true
            ),
            var(r)
                += boost::lambda::bind(
                    &log_message_t::to_string,
                    boost::lambda::_1
                )
        )
    );

    return r;
}


/**
 * Print to std::cout all the messages.
 */
void log_t::print() const
{
    std::cout << to_string() << std::endl;
}


/**
 * Adds a message.
 *
 * @param message The message.
 */
void log_t::add_message(const log_message_t & message)
{
    messages.push_back(message);

    if (message.type == log_message_type_t::error)
        error_count++;
}


/**
 * Adds a message.
 *
 * @param type type of message.
 * @param text Text of the message.
 */
void log_t::add_message(
    log_message_type_t type,
    const std::string & text)
{
    add_message(log_message_t(type, text));
}


/**
 * Adds an error message.
 *
 * @param text Text of the message.
 */
void log_t::error(const std::string & text)
{
    add_message(log_message_type_t::error, text);
}


/**
 * Adds a warning message.
 *
 * @param text Text of the message.
 */
void log_t::warning(const std::string & text)
{
    add_message(log_message_type_t::warning, text);
}


/**
 * @return List of compilation messages.
 */
const std::list<log_message_t> &
    log_t::get_messages() const
{
    return messages;
}


/**
 * Deletes all the messages from the log.
 */
void log_t::clear()
{
    messages.clear();
    error_count = 0;
}


/**
 * @return Number of messages in the log. To get the number of error messages,
 *     use get_error_count.
 */
size_t log_t::size() const
{
    return messages.size();
}


} /* namespace bakery */

