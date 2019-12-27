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


#ifndef _BAKERY_DEF_NODE_HPP_
#define _BAKERY_DEF_NODE_HPP_


#include "array_data.hpp"
#include "enum_value_data.hpp"
#include "member_data.hpp"
#include "native_data.hpp"
#include "path.hpp"
#include "structure_data.hpp"
#include "typedef_data.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>
#include "../continuous_wrapper.hpp"
#include "../compilation_log.hpp"
#include <string>
#include <vector>


namespace bakery {
namespace rec {


/**
 * Represents a node.
 *
 * A node is the base class for everything.
 */
class node
{
	public:
		/**
		 * Enumerates the different possible specializations of a node.
		 */
		struct kind
		{
			enum value
			{
				none,
				namespace_,
				structure,
				variant,
				array,
				typedef_,
				enum_,
				enum_value,
				member,
				native,
				template_type
			};

			static const std::string strings[];

			typedef continuous_wrapper_t<
					value,
					strings,
					none,
					template_type
				> Wrapper;
		};


		/**
		 * Enumerates the possible qualifiers.
		 */
		struct qualifier
		{
			enum value
			{
				unsigned_,
				optional
			};

			static const std::string strings[];

			typedef continuous_wrapper_t<
					value,
					strings,
					unsigned_,
					optional
				> Wrapper;
		};


		typedef boost::shared_ptr<node> sptr;
		typedef std::list<sptr> sptr_list;


		node();
		node(kind::value);
		node(kind::value, const std::string &);

		kind::value get_kind() const;
		const std::string & get_name() const;
		std::string print() const;
		const typedef_data_t & get_typedef_data() const;
		const member_data_t & get_member_data() const;
		const native_data_t & get_native_data() const;
		const enum_value_data_t & get_enum_value_data() const;
		enum_value_data_t & get_enum_value_data();
		const structure_data_t & get_structure_data() const;
		structure_data_t & get_structure_data();
		const array_data_t & get_array_data() const;
		array_data_t & get_array_data();
		bool has_qualifier(qualifier::value) const;
		bool is_type() const;
		sptr find_node(const std::string &) const;
		const sptr_list & get_children() const;

		template <template <typename T, typename U> class Container>
			Container<sptr, std::allocator<sptr> >
			query_nodes(kind::value) const;

		void set_kind(kind::value);
		void set_name(const std::string &);
		void add_child(node::sptr);
		void set_scope_node(node*);

		template <typename T>
			void add_child_list(const T &);

		void set_structure_data(const structure_data_t &);
		void set_array_data(const array_data_t &);
		void set_typedef_data(const typedef_data_t &);
		void set_member_data(const member_data_t &);
		void set_native_data(const native_data_t &);
		void set_enum_value_data(const enum_value_data_t &);
		void add_qualifier(qualifier::value);
        void merge(const sptr &);
        void merge_types(const sptr &);
		bool compile(compilation_log_t &);
		void merge_namespaces();
		bool compile_type_instanciation(type_instanciation_t &,
			compilation_log_t &, node*) const;

	private:
		/** kind of the node. */
		kind::value kind;
		/** List of the children. */
		sptr_list children;
		/** Parent pointer. */
		node* parent;
		/** The node used to solve types during compilation. If 'this' has a
		 * parent, scope_node == parent. */
		node* scope_node;
		/** Name of the node. May be empty. */
		std::string name;
		/** data. */
		boost::variant<
			member_data_t,
			typedef_data_t,
			native_data_t,
			enum_value_data_t,
			structure_data_t,
			array_data_t
		> data;
		/** qualifiers, such as "optional" or "unsigned". */
		std::list<qualifier::value> qualifiers;

		void init_data();
		bool check_children_names(compilation_log_t &) const;
		sptr solve_type_down(const path &) const;
		sptr solve_type(const path &) const;
		bool compile_as_structure(compilation_log_t &);
		bool compile_as_array(compilation_log_t &);
		bool compile_as_typedef(compilation_log_t &);
		bool compile_as_member(compilation_log_t &);
		bool compile_as_enum(compilation_log_t &);
		bool compile_as_enum_value(compilation_log_t &);
};


/**
 * @return A list of node which have the desired kind.
 *
 * @param kind kind of the nodes returned.
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
		[&](const sptr & x)
		{
			if (x->get_kind() == kind)
				result.push_back(x);
		}
	);

	return result;
}


/**
 * Adds a child list.
 *
 * @param list List containing the child to be added.
 */
template <typename T>
	void node::add_child_list(const T & list)
{
	std::for_each
	(
		list.begin(),
		list.end(),
		[&](const typename T::value_type & x)
		{
			add_child(x);
		}
	);
}


} /* namespace rec */
} /* namespace bakery */


#endif

