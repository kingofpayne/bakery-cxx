/**
 * Copyright (C) 2012, 2013
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


#ifndef _BAKERY_GRAMMAR_UTIL_HPP_
#define _BAKERY_GRAMMAR_UTIL_HPP_


#include <boost/shared_ptr.hpp>
#include "../rec/node.hpp"
#include "../dat/node.hpp"


namespace bakery {
namespace grammar {


struct def_node_sptr_creator
{
    typedef boost::shared_ptr<rec::node> result_type;

    boost::shared_ptr<rec::node> operator()() const
    {
        return boost::shared_ptr<rec::node>(
            new rec::node(rec::node::kind::none));
    }

    boost::shared_ptr<rec::node> operator()(rec::node::kind::value kind) const
    {
        return boost::shared_ptr<rec::node>(new rec::node(kind));
    }
};


/** Phoenix function to create easily initialized rec::node::sptr. */
extern const boost::phoenix::function<def_node_sptr_creator>
    create_def_node_sptr;


struct dat_node_sptr_creator
{
    typedef dat::node::sptr result_type;

    dat::node::sptr operator()(dat::node::kind::value kind) const
    {
        return dat::node::sptr(new dat::node(kind));
    }
};


/** Phoenix function to create easily initialized dat::node::sptr. */
extern const boost::phoenix::function<dat_node_sptr_creator>
    create_dat_node_sptr;


} /* namespace grammar */
} /* namespace bakery */


#endif

