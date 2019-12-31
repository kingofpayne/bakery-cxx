/**
 * Copyright (C) 2010, 2011, 2012
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


#ifndef _BAKERY_ENUM_VALUE_DATA_HPP_
#define _BAKERY_ENUM_VALUE_DATA_HPP_


#include <string>


namespace bakery {


/**
 * Data associated to an enumeration value.
 */
class enum_value_data_t
{
    public:
        enum_value_data_t();
        const std::string & get_value_str() const;
        bool get_fixed_value_flag() const;
        void set_fixed_value(const std::string &);

        /** Value of the enumeration member. */
        int value;
                
    private:
        /** Flag indicating if a fixed value has been specified. */
        bool fixed_value_flag;
        /** The string value given in the recipe file. During compilation,
         * this is converted as an int (if fits) and stored in value. */
        std::string value_str;
};


} /* namespace bakery */


#endif

