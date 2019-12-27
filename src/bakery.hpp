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


#include "compilation_log.hpp"
#include "compiler.hpp"
#include "error_code.hpp"
#include "io.hpp"
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
        template <typename T> error_code_t load(const std::string &, T*);
		void include(const std::string &);
        void include(const std::list<std::string> &);
        const std::list<std::string> & get_include_directories() const;
        void set_force_rebuild(bool);
        bool get_force_rebuild() const;
        bool has_rebuilt() const;
	
    private:
        /** List of directories which may contain recipe files. */	
		std::list<std::string> include_directories;
        /** Switch to force recompilation. */
        bool force_rebuild;
        /** Set to true if previous data load has called the compiler to rebuild
         * the cache. */
        bool has_rebuilt_flag;
};


/**
 * Load and deserialize a data file into a destination variable.
 *
 * @param path Path to the datafile.
 * @param dest Destination variable for deserialization.
 * @return Error code if it fails.
 */
template <typename T>
    error_code_t bakery_t::load(const std::string & path, T* dest)
{
    has_rebuilt_flag = false;

    bakery_assert(dest != 0);
    std::cout << "Loading resource " << path << "...";

	/* Check that the data file exists. */
	bakery_assert_message(boost::filesystem::exists(path),
		"File '" + path + "' does not exist.");

    boost::filesystem::path bin_path(path);
    bin_path.replace_extension(".bin");
	
	/* Will get all compilation errors */
	compilation_log_t log;

	/* Check dates */
	bool recompile = (!boost::filesystem::exists(bin_path)) || force_rebuild;
	if (!recompile)
	{
		std::time_t dat_time = boost::filesystem::last_write_time(path);
		std::time_t bin_time = boost::filesystem::last_write_time(bin_path);
		if (dat_time > bin_time)
			recompile = true;
	}

	if (recompile)
	{
		/* Recompilation needed. */
		std::cout << " rebuilding cache...";
        std::flush(std::cout);

		/* Compile */
		compiler::compile(path, bin_path.c_str(), include_directories, log);
        has_rebuilt_flag = true;

		/* Check for any error */
		if (log.get_error_count() != 0)
		{
			std::cout << " failed." << std::endl;

			std::cerr << "An error occured during compilation of ressource "
				<< path 
				<< ", below are listed the error messages reported during "
				<< "compilation."
				<< std::endl
				<< log.print()
				<< std::endl;

			/* Remove file since it is not correct
			 * (compiler writes in it during all the compilation process and
			 * does not use a buffer before outputing the result). */
			boost::filesystem::remove(bin_path);
			/* Stop program execution, this is unrecoverable ! */
			bakery_abort_message("Cannot load ressource " + path + ".");
		}
	}

    std::ifstream* stream =
        new std::ifstream(bin_path.string(), std::ios::binary);

	if (!stream->is_open())
	{
		std::cout << " failed." << std::endl;
        delete stream;
		bakery_abort_message("Failed to open file " + bin_path.string() + ".");
	}

	std::cout << " ok." << std::endl;
    input_t(stream) >> (*dest); /* input_t takes pointer ownership of stream */
    return 0;
}


} /* namespace bakery */


#endif
