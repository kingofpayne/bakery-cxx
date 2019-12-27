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


#ifndef _BAKERY_DEF_NATIVE_DATA_HPP_
#define _BAKERY_DEF_NATIVE_DATA_HPP_


#include "../continuous_wrapper.hpp"


namespace bakery {
namespace rec {


/**
 * data for nodes representing a native type. 
 */
class native_data_t
{
	public:
		/**
		 * Enumerates the different native types.
		 */
		struct class_t
		{
			enum value
			{
				null,
				bool_,
				char_,
				short_,
				int_,
				float_,
				double_,
				string,
				pair,
				tuple,
				list,
				map
			};

			static const std::string strings[];

			typedef continuous_wrapper_t<
					value,
					strings,
					null,
					map
				> Wrapper;
		};
		
		native_data_t();
		native_data_t(class_t::value);
		std::string print() const;
		class_t::value get_class() const;
		void set_class(class_t::value);
		
	private:
		/** Class */
		class_t::value class_;
};


} /* namespace rec */
} /* namespace bakery */


#endif

