/**
 * Copyright (C) 2010, 2011, 2012
 * Olivier Heriveaux.
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


#ifndef _BAKERY_DAT_FLOATING_HPP_
#define _BAKERY_DAT_FLOATING_HPP_


#include <string>


namespace bakery {
namespace dat {


/**
 * Stores a parsed floating number.
 *
 * The floating number is stored in three parts:
 * - an integer number before the '.' separator,
 * - an integer number after the '.' separator,
 * - an integer number after the exponent.
 *
 * Each part can be an empty string.
 *
 * For example, the string '1.45e-4' will be stored as {'1', '45', '-4'}. The
 * string '1' will be stored as {'1', '', ''}.
 */
class floating
{
	public:
		floating();
		
		bool get_negative() const;
		const std::string & get_integer_string() const;
		const std::string & get_decimal_string() const;
		const std::string & get_exponent_string() const;
		bool has_integer_part() const;
		bool has_decimal_part() const;
		bool has_exponent_part() const;
		void set_negative(bool);
		void set_integer_string(const std::string &);
		void set_decimal_string(const std::string &);
		void set_exponent_string(const std::string &);

	private:
		/** True for a negative floating. */
		bool negative;
		/** Integer number before the '.' separator. */
		std::string integer_string;
		/** Integer number after the '.' separator. */
		std::string decimal_string;
		/** Integer string of the exponent. */
		std::string exponent_string;

		static bool valid_format(const std::string &, bool);
};


} /* namespace dat */
} /* namespace bakery */


#endif

