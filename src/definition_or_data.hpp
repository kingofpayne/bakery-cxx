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


#ifndef _BAKERY_DEFINITION_OR_DATA_HPP_
#define _BAKERY_DEFINITION_OR_DATA_HPP_


#include "def/definition.hpp"
#include "dat/data.hpp"


namespace bakery {


/**
 * Class which holds either a definition or a data, or nothing.
 */
class definition_or_data
{
	public:
		definition_or_data();

		void set_definition(const def::definition &);
		void set_data(const dat::data &);
		bool is_definition() const;
		bool is_data() const;
		def::definition & get_definition();
		dat::data & get_data();
		
	private:
		boost::variant<
			int, /* int to hold nothing. */
			def::definition,
			dat::data
		> value;	
};


} /* namespace bakery */


#endif

