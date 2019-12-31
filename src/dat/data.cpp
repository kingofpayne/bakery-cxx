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


#include "data.hpp"


namespace bakery {
namespace dat {


/**
 * @return Reference to the root node.
 */
const node::sptr & data::get_root_node() const
{
    return root_node;
}


/**
 * @return A object indicating where the recipe file is located.
 */
const recipe_indication_t & data::get_recipe_indication() const
{
    return recipe_indication;
}


/**
 * @return A string representing the data.
 */
std::string data::print() const 
{
    std::string r;

    r = "recipe " + recipe_indication.fi.print() + "\n";
    r += root_node->print();

    return r;
}


/**
 * Defines the root node.
 *
 * @param value Root node.
 */
void data::set_root_node(const node::sptr & value)
{
    root_node = value;
}


/**
 * Defines where the recipe file is located.
 *
 * @param value The location of the recipe file.
 */
void data::set_recipe_indication(const recipe_indication_t & value)
{
    recipe_indication = value;
}


} /* namespace dat */
} /* namespace bakery */

