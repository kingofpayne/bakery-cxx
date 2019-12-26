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


#include "definition.hpp"


namespace bakery {
namespace def {


/**
 * @return A string representing the content of the definition.
 */
std::string definition::print() const
{
	return "definition " + the_node->print();
}


/**
 * Returns the root node.
 */
node::sptr definition::get_node() const
{
	return the_node;
}


/**
 * @return Const reference to the list of included definition files.
 */
const std::list<file_indication> & definition::get_included_files() const
{
	return included_files;
}


/**
 * Defines the root node.
 *
 * @param value Shared pointer to the node.
 */
void definition::set_node(node::sptr value)
{
	the_node = value;
}


/**
 * Compiles the definition.
 *
 * @param compil_status The object in which error messages will be pushed.
 *
 * @return False if an error occurs during compilation.
 */
bool definition::compile(compilation_log_t & compil_status)
{
	/* Similar namespaces must be merge before compiling. */
	the_node->merge_namespaces();

	/* Now compile the root node, which recursively compiles all the tree, and
	 * return the result. */
	return the_node->compile(compil_status);
}


/**
 * Declares a file to be included as definition during compilation.
 *
 * @param file_indication Indicates the path of the file, and its relativity.
 */
void definition::add_include_file(const file_indication & file_indication)
{
	included_files.push_back(file_indication);
}


} /* namespace def */
} /* namespace bakery */

