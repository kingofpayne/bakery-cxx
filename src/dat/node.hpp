/**
 * Copyright (C) 2010, 2011, 2012, 2013
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


#ifndef _BAKERY_DAT_NODE_HPP_
#define _BAKERY_DAT_NODE_HPP_


/* To be included before phoenix. */
#include "../enable_phoenix_v3.hpp"

#include "../continuous_wrapper.hpp"
#include "floating.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/variant.hpp>
#include <list>


namespace bakery {
namespace dat {


/**
 * A node can represent any primitive in the data file: assignments, values,
 * subnodes...
 */
class node
{
	public:
		typedef boost::shared_ptr<node> sptr;
		typedef std::list<sptr> node_sptr_list;

		/**
		 * Enumerates the different kind of nodes.
		 *
		 * 'group' nodes are used for structures, arrays... They generally are
		 * nodes with children.
		 */
		struct kind
		{
			enum value
			{
				none,
				assignment,
				string,
				floating,
				identifier,
				bool_,
				group,
				variant,
				map_assignment
			};

			static const std::string strings[];

			typedef continuous_wrapper_t<
					value,
				   	strings,
					none,
					map_assignment
				> wrapper;
		};

		node();
		node(kind::value);
		kind::value get_kind() const;
		const node_sptr_list & get_children() const;
		const std::string & get_name() const;
		const std::string & get_string() const;
		const std::string & get_identifier() const;
		const floating & get_floating() const;
		bool get_bool() const;
		template <template <typename T, typename U> class Container>
			Container<sptr, std::allocator<sptr> >
			query_nodes(kind::value) const;
		std::string print() const;
		void set_name(const std::string &);
		void add_child(const sptr &);
		void set_string(const std::string &);
		void set_floating(const floating &);
		void set_identifier(const std::string &);
		void set_bool(bool);

	private:
		/** Name of the node */
		std::string name;
		/** kind of the node. */
		kind::value kind;
		/** Sub nodes */
		node_sptr_list children;
		/** data, depending on the node kind. */
		boost::variant<
			std::string,
			floating,
			bool
		> data;
};


/**
 * @return A list of node which have the desired kind.
 *
 * @param kind Kind of the nodes returned.
 *
 * @template Container A container class, such as std::list or std::vector.
 *           Default class is std::list.
 *
 * Note: Container takes two template arguments, like std::list.
 */
template <template <typename T, typename U> class Container>
	Container<node::sptr, std::allocator<node::sptr> >
	node::query_nodes(kind::value kind) const
{
	using namespace boost::phoenix::arg_names;

	Container<sptr, std::allocator<sptr> > result;

	std::for_each
	(
		children.begin(),
		children.end(),
		[&](const sptr & s)
		{
			if (s->get_kind() == kind)
			{
				result.push_back(s);
			}
		}
	);

	return result;
}


} /* namespace dat */
} /* namespace bakery */


#endif

