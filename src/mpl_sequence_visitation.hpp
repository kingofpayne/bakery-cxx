/**
 * Copyright (C) 2011
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


#ifndef _BAKERY_SEQUENCE_VISITATION_HPP_
#define _BAKERY_SEQUENCE_VISITATION_HPP_


#include "assertions.hpp"
#include <boost/mpl/at.hpp>


namespace bakery {


/**
 * Performs visitation over the types of index in range [A, B].
 *
 * @param <S> MPL sequence (for example, a boost::mpl::vector<...>).
 * @param <A> Lower index of range to visit.
 * @param <B> Higher index of range to visit.
 */
template <typename S, int A, int B>
	struct mpl_sequence_range_visitor;


/**
 * Specialization of mpl_sequence_range_visitor when the range is reduced to
 * only one element.
 */
template <typename S, int A>
	struct mpl_sequence_range_visitor<S, A, A>
{
	/**
	 * Process visitation.
	 *
	 * @param i Type index. For the current specialization, this parameter
	 *        should be equal to A, so this may not be used...
	 * @param v Visitation object.
	 *
	 * @param <V> Type of the visitation object.
	 */
	template <typename V>
		static void visit(int i, V & v)
	{
		v.template visit<
			typename boost::mpl::at<S, boost::mpl::int_<A>>::type
		>();
	}
};


template <typename S, int A, int B>
	struct mpl_sequence_range_visitor
{
	static const int mid = (A+B)/2;

	/**
	 * Process visitation.
	 *
	 * @param i Type index.
	 * @param v Visitation object.
	 *
	 * @param <V> Type of the visitation object.
	 */
	template <typename V>
		static void visit(int i, V & v)
	{
		if(i <= mid)
			mpl_sequence_range_visitor<S, A, mid>::visit(i, v);
		else
			mpl_sequence_range_visitor<S, mid + 1, B>::visit(i, v);
	}
};


/**
 * Apply visitation over v of a mpl sequence, given the type index. In other
 * words, the function visit(T) of v is called, where T is the type of index i
 * in the given MPL sequence.
 *
 * @remarq Visitation is not done via the operator() since there are no
 *         parameters, so templatization of this operator may be non-trivial.
 *
 * @param i Type index in MPL sequence. If this parameter is out of bounds an
 *        exception is raised.
 * @param Visitation functor.
 *
 * @param <S> MPL sequence.
 * @param <V> Visitation functor type.
 */
template <typename S, typename V>
	void apply_mpl_sequence_visitor(int i, V & v)
{
	bakery_assert_message((i >= 0) && (i < S::size::value),
		"invalid mpl sequence visitation index.");

	mpl_sequence_range_visitor<S, 0, S::size::value - 1>::visit(i, v);
}


} /* namespace bakery */


#endif

