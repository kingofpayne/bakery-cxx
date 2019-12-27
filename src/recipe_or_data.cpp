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


#include "recipe_or_data.hpp"


namespace bakery {


/**
 * Default constructor.
 */
recipe_or_data::recipe_or_data():
	value(0)
{}


/**
 * Defines the recipe_or_data as a Definition.
 *
 * @param value The recipe.
 */
void recipe_or_data::set_recipe(const def::recipe & value)
{
	this->value = value;
}


/**
 * Defines the recipe_or_data as a Data.
 *
 * @param value The data.
 */
void recipe_or_data::set_data(const dat::data & value)
{
	this->value = value;
}


/**
 * @return True if the instance holds a recipe.
 */
bool recipe_or_data::is_recipe() const
{
	return value.which() == 1;
}


/**
 * @return True if the instance holds a data.
 */
bool recipe_or_data::is_data() const
{
	return value.which() == 2;
}


/**
 * @return A reference to the recipe.
 *
 * This function must be called only if is_recipe() == true.
 */
def::recipe & recipe_or_data::get_recipe()
{
	bakery_assert_debug(is_recipe());
	return boost::get<def::recipe>(value);
}


/**
 * @return A reference to the data.
 *
 * This function must be called only if is_data() == true.
 */
dat::data & recipe_or_data::get_data()
{
	bakery_assert_debug(is_data());
	return boost::get<dat::data>(value);
}


} /* namespace bakery */

