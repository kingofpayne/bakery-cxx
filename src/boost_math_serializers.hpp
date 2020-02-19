/**
 * Copyright (C) 2017-2020
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


#ifndef _BAKERY_BOOST_MATH_SERIALIZERS_HPP_
#define _BAKERY_BOOST_MATH_SERIALIZERS_HPP_


#include "serializers.hpp"
#include <boost/math/quaternion.hpp>


namespace bakery {


/**
 * serializer for all boost::math::quaternion<T>.
 */
template <typename T>
	struct serializer<boost::math::quaternion<T> >
{
	/**
	 * Serialization operator, for input.
	 *
	 * @param u Object to be serialized.
	 * @param io Reference to the end-point serializer.
	 *
	 * Template parameters:
	 * - U: boost::math::quaternion<T> &.
	 * - IO: Input.
	 */
	template <typename U, typename IO>
		void operator()(boost::math::quaternion<T> & u, IO & io)
	{
		T a, b, c, d;
		io(a)(b)(c)(d);
		u = boost::math::quaternion<T>(a, b, c, d);
	}
};


} /* namespace bakery */


#endif

