/**
 * Copyright (C) 2012
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


#ifndef _BAKERY_ASSERTIONS_HPP_
#define _BAKERY_ASSERTIONS_HPP_


#include <string>


namespace bakery {


void assertion_failed(const std::string &, const char*, int)
	__attribute__ ((noreturn));


} /* namespace bakery */


#define bakery_assert(condition) \
	if (!(condition)) \
		::bakery::assertion_failed("("#condition")", __FILE__, __LINE__)

#define bakery_assert_message(condition, message) \
	if (!(condition)) \
		::bakery::assertion_failed(message, __FILE__, __LINE__)

#define bakery_abort() \
	::bakery::assertion_failed("Execution aborted.", __FILE__, __LINE__)

#define bakery_abort_message(message) \
	::bakery::assertion_failed(message, __FILE__, __LINE__)

#define bakery_unexpected_case() \
	::bakery::assertion_failed("Unexpected case.", __FILE__, __LINE__)

#define bakery_not_implemented() \
	::bakery::assertion_failed("Not implemented.", __FILE__, __LINE__)

/**
 * This assertion should be used when you want to check some information
 * coming from internal data.
 *
 * The assertion of bakery_assert_debug will be removed if you compile the code
 * with the optimisations. (in non debug mode).
 */
#ifdef NDEBUG
	#define bakery_assert_debug(condition) ((void)0)
	#define bakery_assert_message_debug(condition, message) ((void)0)
#else
	#define bakery_assert_debug(condition) \
		bakery_assert(condition)

	#define bakery_assert_message_debug(condition, message) \
		bakery_assert_message(condition, message)
#endif


#endif
