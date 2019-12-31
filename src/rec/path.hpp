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


#ifndef _BAKERY_DEF_PATH_HPP_
#define _BAKERY_DEF_PATH_HPP_


#include <string>
#include <vector>


namespace bakery {
namespace rec {


/**
 * Represents a path identifing a type (or maybe something else).
 *
 * Example of paths:
 *
 * toto::tutu::my_type
 * ::super::toc::type
 *
 * A path can be absolute or relative (paths starting with "::" are absolutes).
 */
class path
{
    public:
        typedef std::vector<std::string> token_list;

        path();
        bool get_absolute() const;
        const token_list & get_tokens() const;
        std::string print() const;
        void push_back(const std::string &);
        void pop_front();
        void set_absolute(bool);
    private:
        /** True if the path is absolute. False by default. */
        bool absolute;
        /** Elements composing the path. */
        token_list tokens;
};


} /* namespace rec */
} /* namespace bakery */


#endif

