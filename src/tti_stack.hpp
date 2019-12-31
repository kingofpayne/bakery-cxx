/**
 * Copyright (C) 2013
 * Olivier Hériveaux
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


#ifndef _BAKERY_TTI_STACK_HPP_
#define _BAKERY_TTI_STACK_HPP_


namespace bakery {


/**
 * Template Type Instanciation stack. This type is used for recording how
 * template types are associated to real types when parsing a recipe.
 */
typedef std::list<std::pair<rec::node::sptr, rec::type_instanciation_t> >
    tti_stack_t;


/**
 * Class used to push elements in a tti_stack_t and pop them automatically
 * during the destructor.
 */
template <typename T, typename U> class tti_stacker_t
{
    public:
        /**
         * Constructor.
         *
         * @param _tti The tti in which elements are stacked by the instance.
         * @param first_node Iterator to the first element node to be stacked.
         * @param last_node End iterator.
         * @param first_ti Iterator to the first type instanciation to be
         *        stacked.
         * @param last_ti End iterator.
         */
        tti_stacker_t(
            tti_stack_t & _tti,
            typename T::const_iterator first_node,
            const typename T::const_iterator & last_node,
            typename U::const_iterator first_ti,
            const typename U::const_iterator & last_ti)
        :
            tti(_tti),
            size(_tti.size())
        {
            while (first_node != last_node)
            {
                bakery_assert_debug(first_ti != last_ti);
                tti.push_back(
                    std::pair<rec::node::sptr, rec::type_instanciation_t>(
                        *first_node,
                        *first_ti
                    )
                );
                first_node++;
                first_ti++;
            }
        }


        /**
         * Destructor. Pops the stacked elements.
         */
        ~tti_stacker_t()
        {
            tti.resize(size);
        }

    private:
        /** Reference to the tti_stack_t. */
        tti_stack_t & tti;

        /** Size of tti stack before elements are pushed in the stack. */
        size_t size;
};


} /* namespace bakery */


#endif

