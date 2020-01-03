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


#ifndef _BAKERY_LOG_HPP_
#define _BAKERY_LOG_HPP_


#include "log_message.hpp"
#include <list>


namespace bakery {


/**
 * Log filled during the compilation or decompilation process.
 */
class log_t
{
    public:
        log_t();

        size_t get_error_count() const;
        std::string print() const;
        void add_message(const log_message_t &);
        void add_message(log_message_type_t, const std::string &);
        void error(const std::string &);
        void warning(const std::string &);
        const std::list<log_message_t> & get_messages() const;

    private:
        /** List of the messages. */
        std::list<log_message_t> messages;
        /** Boolean indicating the error count. This member is calculated when
         * adding the messages to the status. */
        size_t error_count;
};


} /* namespace bakery */


#endif

