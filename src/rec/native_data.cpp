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


#include "native_data.hpp"
#include "../assertions.hpp"


namespace bakery {
namespace rec {


const std::string native_data_t::class_t::strings[] =
{
	"null",
	"bool",
	"char",
	"short",
	"int",
	"float",
	"double",
	"string",
	"pair",
	"tuple",
	"list",
	"map"
};


/**
 * Constructor
 */
native_data_t::native_data_t():
	class_(class_t::null)
{}


/**
 * Constructor
 *
 * @param _class_ The class of the native.
 *
 * @throw core::exceptions::InvalidParameter* if the class is not a member of
 *        the native_data::class_t::value enumeration.
 */
native_data_t::native_data_t(class_t::value _class_):
	class_(_class_)
{
	bakery_assert_message(class_t::Wrapper::has(_class_), "Bad class value.");
}


/**
 * @return A string representing the content of the native_data.
 */
std::string native_data_t::print() const
{
	return "class = " + class_t::Wrapper::to_string(class_);
}


/**
 * @return The class of the native type.
 */
native_data_t::class_t::value native_data_t::get_class() const
{
	return class_;
}


/**
 * Defines the class of the native type.
 *
 * @param value Class.
 */
void native_data_t::set_class(class_t::value value)
{
	bakery_assert_message(class_t::Wrapper::has(value), "Bad class value.");
	class_ = value;
}


} /* namespace rec */
} /* namespace bakery */

