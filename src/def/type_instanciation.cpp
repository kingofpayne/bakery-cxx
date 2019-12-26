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


#include "type_instanciation.hpp"
#include "node.hpp"


namespace bakery {
namespace def {


/**
 * Constructor.
 */
type_instanciation_t::type_instanciation_t():
    type_path(),
    unsigned_flag(false),
	owning_node(false),
	type_node_ptr((node*)0)
{}


/**
 * @return A string representing the content of the type instanciation.
 */
std::string type_instanciation_t::print() const
{
	std::string r = type_path.print();

	const node* ptr = get_type_ptr();
	if(ptr != 0)
	{
		r += " - " + ptr->print();
	}

	return r;
}


/**
 * @returns The path of the type.
 */
const path & type_instanciation_t::get_type_path() const
{
    return type_path;
}


/**
 * @return true if the type is declared as unsigned.
 */
bool type_instanciation_t::is_unsigned() const
{
    return unsigned_flag;
}


/**
 * @return A const pointer to the type node, or zero if the node has not been
 *         solved.
 */
const node* type_instanciation_t::get_type_ptr() const
{
	if(owning_node)
		return boost::get<boost::shared_ptr<node> >(type_node_ptr).get();
	else
		return boost::get<node*>(type_node_ptr);
}


/**
 * @return A pointer to the type node, or zero if the node has not been
 *         solved.
 */
node* type_instanciation_t::get_type_ptr()
{
	if(owning_node)
		return boost::get<boost::shared_ptr<node> >(type_node_ptr).get();
	else
		return boost::get<node*>(type_node_ptr);
}


/**
 * @return Const reference on the list of template parameters.
 */
const type_instanciation_t::parameter_list &
	type_instanciation_t::get_parameters() const
{
	return parameters;
}


/**
 * @return Reference on the list of template parameters.
 */
type_instanciation_t::parameter_list & type_instanciation_t::get_parameters()
{
	return parameters;
}


/**
 * @return True if the type instanciation owns its type node.
 */
bool type_instanciation_t::get_owning_node() const
{
	return owning_node;
}


/**
 * Defines the path of the type.
 *
 * @param value Name of the type.
 */
void type_instanciation_t::set_type_path(const path & value)
{
    type_path = value;
}


/**
 * Set or clear the unsigned flag of the type instanciation.
 * @param value true to mark the type as unsigned.
 */
void type_instanciation_t::set_unsigned(bool value)
{
    unsigned_flag = value;
}


/**
 * Sets the type node pointer. Using this function, the type_instanciation_t
 * does not own the pointer of the node.
 *
 * @param node Pointer to the node.
 */
void type_instanciation_t::set_type_node_ptr(node* node)
{
	owning_node = false;
	type_node_ptr = node;
}


/**
 * Adds a template parameter.
 *
 * @param ti The type instanciation in place of the parameter.
 */
void type_instanciation_t::add_parameter(const type_instanciation_t & ti)
{
	parameters.push_back(ti);
}


/**
 * Sets the type node (shared) pointer. Using this function, the
 * type_instanciation_t owns the pointer of the node (more precisely, its is
 * owned by all the copies of the type_instanciation_t. The ressource will be
 * freed once no type_instanciation_t stores the pointer of the node - its a
 * shared_ptr !).
 *
 * @param node Shared pointer to the node.
 */
void type_instanciation_t::set_type_node_sptr(boost::shared_ptr<node> & node)
{
	owning_node = true;
	type_node_ptr = node;
}


} /* namespace def */
} /* namespace bakery */

