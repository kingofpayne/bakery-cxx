/**
 * Copyright (C) 2010, 2011, 2012, 2013, 2014
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


#ifndef _BAKERY_DEF_MEMBER_DATA_HPP_
#define _BAKERY_DEF_MEMBER_DATA_HPP_


#include "type_instanciation.hpp"
#include "../dat/node.hpp"


namespace bakery {
namespace def {


/**
 * Data corresponding to a member in structure of a definition.
 */
struct member_data_t
{
	member_data_t();
	std::string print() const;
	bool has_default_value() const;
		
	/** The type instanciation of the member. */
	type_instanciation_t type_instanciation;
	/** Shared pointer to a default value node. If this pointer is null, no
	 * default value is specified. */
	dat::node::sptr default_value_node;
};


} /* namespace def */
} /* namespace bakery */


#endif

