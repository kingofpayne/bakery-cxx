/**
 * Copyright (C) 2010, 2012, 2013, 2014
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


#ifndef _BAKERY_ERROR_CODE_HPP_
#define _BAKERY_ERROR_CODE_HPP_


#include "assertions.hpp"


namespace bakery {


/**
 * Class which may be used by functions to return an error code.
 *
 * This class provides a good alternative to exceptions by providing safe error
 * code checking. If an instance of error_code_t holds an error (code != 0), the
 * user has to read the value before the instance and all its copies are
 * destroyed. This way, any error occuring during execution must be checked, or
 * the program will be aborted by an exception.
 *
 * If a function call cannot return any error (because, for instance, its
 * parameters have been checked previously), then reading the code can be
 * ommited.
 */
class error_code_t
{
	public:
		error_code_t(int);
		error_code_t(const error_code_t &);
		~error_code_t();
		error_code_t & operator = (const error_code_t &);
		bool fail() const;
		bool good() const;

		/**
		 * Cast to bool operator. Using this cast operator marks the error code
		 * as checked.
		 *
		 * Allows tests with a minimum of code:
		 *
		 * error_code_t x = foo();
		 *
		 * if (x)
		 *     cerr << "Faillure." << endl;
		 */
		inline operator bool () const
		{
			return fail();
		}

		/**
		 * Cast to int (return the value). Using this cast operator marks the
		 * error code as checked.
		 */
		inline operator int () const
		{
            make_checked();
			return value;
		}

		/**
		 * @return Value of the error code.
		 *
		 * Calling this method marks the error code as checked.
		 */
		inline int get() const
		{
			make_checked();
			return value;
		}

	private:
		/** Error state. If no error, this value is 0. */
		int value;
		/** Reference counter. Positive when the error code has not been
		 * checked, otherwise negative. The absolute value of (*ref) gives the
		 * number of instances which shares this error code. */
		mutable int* ref;

		/**
		 * Increments reference count.
		 */
		inline void inc_ref() const
		{
			if (*ref > 0)
				(*ref)++;
			else
				(*ref)--;
		}

		/**
		 * Decrements reference count. Throws if the error is non zero and the
		 * last reference is destroyed.
		 */
		inline void dec_ref() const
		{
			if (*ref > 0)
			{
				if (--(*ref) == 0)
				{
					delete ref;
					bakery_assert(value == 0);
				}
			}
			else
			{
				if (++(*ref) == 0)
					delete ref;
			}
		}

		/**
		 * Marks the error code as checked.
		 */
		inline void make_checked() const
		{
			if (*ref > 0)
				*ref = - *ref;
		}
};


} /* namespace bakery */


#endif

