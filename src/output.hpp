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

        /**
         * Writes t into the output stream using bakery serialization.
         *
         * @param t Source data.
         * @return this
         */
        template <typename T> output_t & operator << (const T & t)
        {
            return (*this)(t);
        }

        /**
         * Writes t into the output stream using bakery serialization.
         *
         * @param t Source data.
         * @return this
         */
        template <typename T> output_t & operator () (const T & t)
        {
            typedef typename std::conditional<
                std::is_enum<T>::value,
                serializer< enum_wrapping_t<T> >,
                typename std::conditional<
                    std::is_arithmetic<T>::value,
                    serializer< arithmetic_wrapping_t<T> >,
                    serializer<T>
                >::type
            >::type selected_serializer_t;
            selected_serializer_t().template operator()<T, output_t>(t, *this);
            return *this;
        }

        /**
         * Trivially stores data by direct stream write.
         *
         * @param data Source data to be written (without transformation) into
         *     the stream.
         */
        template <typename T> void trivial(const T & data)
        {
            stream->write((const char*)(&data), (std::streamsize)sizeof(T));
        }

    private:
        /** Output stream for serialization. Owned. */
        std::ostream* stream;
        /** Decompilation log, to store error messages. */
        log_t log;
};


} /* namespace bakery */


#endif
