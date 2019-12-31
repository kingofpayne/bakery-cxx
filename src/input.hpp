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


#ifndef _BAKERY_INPUT_HXX_
#define _BAKERY_INPUT_HXX_


#include <istream>
#include "serializers.hpp"


namespace bakery {


/**
 * Deserialization class.
 * Non-copyable. Movable.
 */
class input_t
{
    public:
        input_t(std::istream*, bool);
        ~input_t();
        input_t(const input_t &) = delete;
        input_t & operator = (const input_t &) = delete;
        input_t(input_t &&);
        operator bool() const;
        bool good() const;
        bool has_rebuilt() const;

        /**
         * Reads input into t using bakery deserialization.
         *
         * @param t Destination data.
         * @return this
         */
        template <typename T> input_t & operator >> (T & t)
        {
            return (*this)(t);
        }

        /**
         * Reads input into t using bakery deserialization.
         *
         * @param t Destination data.
         * @return this
         */
         template <typename T> input_t & operator()(T & t)
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

            selected_serializer_t().template operator()<
                /* Type tranformation:
                 * from (int) to (int &)
                 * from (float) to (float &)
                 * etc. */
                typename boost::call_traits<T>::reference,
                input_t
            >(t, *this);
            return *this;
         }

         /**
          * Trivially loads data by direct stream read.
          */
         template <typename T> void trivial(T & dest)
         {
            stream->read((char*)(&dest), (std::streamsize)sizeof(T));
         }

    private:
        /** Input stream for deserialization. Owned. */
        std::istream* stream;
        /** When true, indicates the binary has been rebuilt. */
        bool rebuilt_flag;
};


} /* namespace bakery */


#endif
