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


#ifndef _BAKERY_DEF_PARSER_HPP_
#define _BAKERY_DEF_PARSER_HPP_


#include "grammar/main.hpp"
#include <boost/filesystem.hpp>
#include "compilation_log.hpp"
#include "compilation_message.hpp"


namespace bakery {


/**
 * parser class.
 */
class parser
{
	public:
		static void load_from_file(const std::string &, recipe_or_data &,
            compilation_log_t &);
};


} /* namespace bakery */


#endif

