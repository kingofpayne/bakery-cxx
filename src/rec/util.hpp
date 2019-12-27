/**
 * Copyright (C) 2010, 2011, 2012
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


#ifndef _BAKERY_DEF_UTIL_HPP_
#define _BAKERY_DEF_UTIL_HPP_


#include "node.hpp"
#include "native_data.hpp"


namespace bakery {
namespace rec {
namespace util {


node::sptr make_native_node(const std::string &, native_data_t::class_t::value);
node::sptr make_namespace_node(const std::string &);


} /* namespace util */
} /* namespace rec */
} /* namespace bakery */


#endif

