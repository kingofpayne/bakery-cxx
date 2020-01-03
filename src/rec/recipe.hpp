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


#ifndef _BAKERY_DEF_RECIPE_HPP_
#define _BAKERY_DEF_RECIPE_HPP_


#include "../log.hpp"
#include "node.hpp"
#include "../file_indication.hpp"
#include <vector>


namespace bakery {
namespace rec {


/**
 * Class representing the recipe defined in a rec file.
 */
class recipe
{
    public:
        std::string print() const;
        node::sptr get_node() const;
        const std::list<file_indication_t> & get_included_files() const;
        void set_node(node::sptr);
        bool compile(log_t &);
        void add_include_file(const file_indication_t &);

    private:
        /** Root node of the recipe. */
        node::sptr the_node;
        /** Included files. */
        std::list<file_indication_t> included_files;
};


} /* namespace rec */
} /* namespace bakery */


#endif

