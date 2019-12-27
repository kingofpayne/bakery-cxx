/**
 * Copyright (C) 2010, 2011, 2012, 2013
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


#include "path.hpp"
#include <algorithm>
#include <string>
#include "../assertions.hpp"


namespace bakery {
namespace rec {


/**
 * Default constructor.
 */
path::path():
	absolute(false),
	tokens()
{}


/**
 * @return True if the path is absolute, false if it is relative.
 */
bool path::get_absolute() const
{
	return absolute;
}


/**
 * @return The list of tokens.
 */
const path::token_list & path::get_tokens() const
{
	return tokens;
}


/**
 * @return A string representing the content of the path.
 */
std::string path::print() const
{
	std::string result;

	bool dotdot = absolute;

	std::for_each(
		tokens.begin(),
		tokens.end(),
		[&]( const std::string & val ){
			if(dotdot){
				result.append("::");
			}
			dotdot = true;
			result.append(val);
		}
	);

	return result;
}


/**
 * Pushes a token in the path.
 *
 * @param token The token.
 */
void path::push_back(const std::string & token)
{
	tokens.push_back(token);
}


/**
 * Removes the first token of the path.
 *
 * @throw core::exceptions::InvalidCall* if the path is empty.
 */
void path::pop_front()
{
	bakery_assert_message(!tokens.empty(), "path is empty.");
	tokens.erase(tokens.begin());
}


/**
 * Defines if the path is relative or absolute.
 *
 * @param value True to make the path absolute, false to make it relative.
 */
void path::set_absolute(bool value)
{
	absolute = value;
}


} /* namespace rec */
} /* namespace bakery */

