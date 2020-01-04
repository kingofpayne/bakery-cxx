/**
 * Copyright (C) 2019
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


#ifndef _BAKERY_BAKERY_HPP_
#define _BAKERY_BAKERY_HPP_


#include "log.hpp"
#include "compiler.hpp"
#include "error_code.hpp"
#include "input.hpp"
#include <boost/filesystem.hpp>
#include <iostream>


namespace bakery {


/**
 * Can load or save bakery data files.
 *
 * Before loading data, options in the bakery can be configured. Directories to
 * be included for recipe files can be added.
 *
 * After loading data using the bakery, extra compilation information can be
 * retrieved in the state.
 */
class bakery_t
{
    public:
        bakery_t();
        void include(const std::string &);
        void include(const std::list<std::string> &);
        const std::list<std::string> & get_include_directories() const;
        void set_force_rebuild(bool);
        bool get_force_rebuild() const;
        void set_verbose(bool);
        bool get_verbose() const;
        void set_abort_on_error(bool);
        bool get_abort_on_error() const;
        input_t load(const std::string &);

    private:
        /** List of directories which may contain recipe files. */
        std::list<std::string> include_directories;
        /** Switch to force recompilation. */
        bool force_rebuild;
        /** Verbose option. When enabled, bakery directly prints to stdout
         * information when loading data and errors. */
        bool verbose;
        /** Abort on error option. When enabled, bakery calls std::abort when an
         * error is encountered during data loading. */
        bool abort_on_error;
};


input_t load(const std::string &);


} /* namespace bakery */


#endif
