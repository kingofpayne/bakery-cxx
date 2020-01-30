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


#ifndef _BAKERY_OUTPUT_HXX_
#define _BAKERY_OUTPUT_HXX_


#include <istream>
#include "serializers.hpp"
#include "log.hpp"


namespace bakery {


/**
 * Serialization class.
 * Non-copyable. Movable.
 */
class output_t
{
    public:
        output_t();
        output_t(output_t &&);
        ~output_t();
        output_t(const output_t &) = delete;
        output_t & operator = (const output_t &) = delete;
        output_t & operator = (output_t &&);
        operator bool() const;
        bool good() const;
        void set_stream(std::ostream*);
        log_t & get_log();
        const log_t & get_log() const;

    private:
        /** Output stream for serialization. Owned. */
        std::ostream* stream;
        /** Decompilation log, to store error messages. */
        log_t log;
};


} /* namespace bakery */


#endif
