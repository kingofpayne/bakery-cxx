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


#ifndef _BAKERY_DAT_DATA_HPP_
#define _BAKERY_DAT_DATA_HPP_


#include "node.hpp"
#include "../recipe_indication.hpp"
#include <string>


namespace bakery {
namespace dat {


/**
 * Result of the parsing of a data file.
 */
class data
{
	public:
		const node::sptr & get_root_node() const;
		const recipe_indication_t & get_recipe_indication() const;
		std::string print() const;
		void set_root_node(const node::sptr &);
		void set_recipe_indication(const recipe_indication_t &);

	private:
		/** Root node. */
		node::sptr root_node;
		/** Structure indicating where is located the recipe file. */
		recipe_indication_t recipe_indication;
};


} /* namespace dat */
} /* namespace bakery */


#endif

