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


#include "util.hpp"


namespace bakery {
namespace rec {
namespace util {


/**
 * Creates a native node.
 *
 * @param name Name of the node.
 * @param class_ Class of the native type.
 */
node::sptr make_native_node(
	const std::string & name,
	native_data_t::class_t::value class_
){
	node::sptr a_node(new node(node::kind::native));
	a_node->set_name(name);
	a_node->set_native_data(native_data_t(class_));
	return a_node;
}


/**
 * Creates a namespace node.
 *
 * @param name Name of the node.
 */
node::sptr make_namespace_node(const std::string & name)
{
	node::sptr a_node(new node(node::kind::namespace_));
	a_node->set_name(name);
	return a_node;
}


} /* namespace util */
} /* namespace rec */
} /* namespace bakery */

