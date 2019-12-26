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


#ifndef _BAKERY_DEF_TYPE_INSTANCIATION_HPP_
#define _BAKERY_DEF_TYPE_INSTANCIATION_HPP_


#include "path.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>


namespace bakery {
namespace def {


class node;


/**
 * Represents a type instanciation
 */
class type_instanciation_t
{
	public:
		typedef std::vector<type_instanciation_t> parameter_list;

		type_instanciation_t();
		std::string print() const;
        const path & get_type_path() const;
        bool is_unsigned() const;
		const node* get_type_ptr() const;
		node* get_type_ptr();
		const parameter_list & get_parameters() const;
		parameter_list & get_parameters();
		bool get_owning_node() const;
        void set_type_path(const path &);
        void set_unsigned(bool);
		void set_type_node_ptr(node*);
		void set_type_node_sptr(boost::shared_ptr<node> &);
		void add_parameter(const type_instanciation_t &);

	private:
		/** Name of the type. Used to resolve the type. */
        path type_path;
        /** Set to true when the type is declared as 'unsigned'. */
        bool unsigned_flag;
		/** Flag indicating if the type_instanciation_t owns the pointer of its
		 * node (true), or just hold a pointer to a node having a parent
		 * (false). */
		bool owning_node;
		/** Pointer to the type, when resolved. If the type is owned by the
		 * instanciation, it is stored as a shared_ptr. Otherwise, it is stored
		 * as a simple pointer to avoid circular references, which are not
		 * compatible with boost::shared_ptr. */
		boost::variant<
			node*,
			boost::shared_ptr<node>
		> type_node_ptr;
		/** Template parameters */
		parameter_list parameters;
};


} /* namespace def */
} /* namespace bakery */


#endif

