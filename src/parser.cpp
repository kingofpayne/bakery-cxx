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


#include "parser.hpp"
#include "str.hpp"
#include "util.hpp"


namespace bakery {


/**
 * Loads a recipe or data from a file.
 *
 * If this function fails to read the file, an error message is pushed in the
 * parse status.
 *
 * @param path path to the file containing the recipe.
 * @param recipe_or_data Reference to a recipe_or_data object in which
 *        the result will be stored.
 * @param log Where the error messages will be stored.
 */
void parser::load_from_file(
	const std::string & path,
	recipe_or_data & recipe_or_data,
	compilation_log_t & log)
{
	/* Load the file */
	std::string content;
	error_code_t ec = str::load_from_file(path, content);

	if (ec)
	{
		log.add_error("Failed to read file '" + path + "'.");
		return;
	}

	std::string::const_iterator it = content.begin();
	const std::string::const_iterator it_end = content.end();

	grammar::main<std::string::const_iterator> grammar;

	boost::spirit::qi::phrase_parse(it, it_end, grammar,
		grammar::skipper(), recipe_or_data);

	/* Check parse result. */
	if (it != it_end)
	{
		log.add_error("in file '" + path + "' line "
			+ str::from(calculate_line_number(content, it))
			+ ", syntax error : " + show_string_position(content, it));
	}
}


} /* namespace bakery */

