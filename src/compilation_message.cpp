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


#include "compilation_message.hpp"
#include "assertions.hpp"


namespace bakery {


/**
 * Default constructor.
 */
compilation_message_t::compilation_message_t()
{}


/**
 * Constructor.
 *
 * @param type_ Type of the message.
 * @param text_ Text of the message.
 */
compilation_message_t::compilation_message_t(compilation_message_type_t type_,
	const std::string & text_)
:
	type(type_),
	text(text_)
{}


/**
 * @return A string representing the message.
 */
std::string compilation_message_t::to_string() const
{
	std::string h;
	switch (type)
	{
		case compilation_message_type_t::error:
			h = "Error";
			break;

		case compilation_message_type_t::warning:
			h = "Warning";
			break;

		default:
			bakery_unexpected_case();
	}
	return h + ": " + text;
}


} /* namespace bakery */

