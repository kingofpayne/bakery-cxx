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


#ifndef _BAKERY_DEF_DEFINITION_HPP_
#define _BAKERY_DEF_DEFINITION_HPP_


#include "../compilation_log.hpp"
#include "node.hpp"
#include "../file_indication.hpp"
#include <vector>


namespace bakery {
namespace def {


/**
 * Class representing the definition defined in a def file.
 */
class definition
{
	public:
		std::string print() const;
		node::sptr get_node() const;
		const std::list<file_indication> & get_included_files() const;
		void set_node(node::sptr);
		bool compile(compilation_log_t &);
		void add_include_file(const file_indication &);

	private:
		/** Root node of the definition. */
		node::sptr the_node;
		/** Included files. */
		std::list<file_indication> included_files;
};


} /* namespace def */
} /* namespace bakery */


#endif

