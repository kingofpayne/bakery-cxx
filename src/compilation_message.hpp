/**
 * Copyright (C) 2010, 2011, 2012, 2013
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


#ifndef _BAKERY_COMPILATION_MESSAGE_HPP_
#define _BAKERY_COMPILATION_MESSAGE_HPP_


#include <string>


namespace bakery {


/**
 * Compilation message types.
 */
enum class compilation_message_type_t
{
	warning,
	error
};


/**
 * Holds a message resulting from a compilation (error message, warning
 * message...).
 */
struct compilation_message_t
{
	compilation_message_t();
	compilation_message_t(compilation_message_type_t, const std::string &);

	std::string to_string() const;

	/** Type of the message. */
	compilation_message_type_t type;
	/** Message. */
	std::string text;
};


} /* namespace bakery */


#endif

