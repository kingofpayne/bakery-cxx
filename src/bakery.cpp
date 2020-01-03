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


#include "bakery.hpp"
#include <iostream>


namespace bakery {


/**
 * Default constructor.
 */
bakery_t::bakery_t():
    force_rebuild(false)
{}


/**
 * Set or unset force_rebuild switch.
 *
 * @param value New value.
 */
void bakery_t::set_force_rebuild(bool value)
{
    force_rebuild = value;
}


/**
 * @return force_rebuild setting.
 */
bool bakery_t::get_force_rebuild() const
{
    return force_rebuild;
}


/**
 * Add a list of include directories.
 *
 * @param dirs List of include directories.
 */
void bakery_t::include(const std::list<std::string> & dirs)
{
    std::copy(dirs.begin(), dirs.end(),
        std::back_inserter(include_directories));
}


/**
 * Includes a directory which may contain recipe files.
 *
 * @param dir The directory.
 */
void bakery_t::include(const std::string & dir)
{
    include_directories.push_back(dir);
}


/**
 * @return List of directories which may contain recipe files.
 */
const std::list<std::string> & bakery_t::get_include_directories() const
{
    return include_directories;
}


/**
 * Load a bakery data file. Rebuilds the binary cache if necessary, or if the
 * force_build option is enabled.
 *
 * @param path Path to the datafile.
 * @return input_t for deserialization.
 */
input_t bakery_t::load(const std::string & path)
{
    bool has_rebuilt_flag = false;
    std::cout << "Loading resource " << path << "...";

    /* Check that the data file exists. */
    bakery_assert_message(boost::filesystem::exists(path),
        "File '" + path + "' does not exist.");

    boost::filesystem::path bin_path(path);
    bin_path.replace_extension(".bin");

    /* Will get all compilation errors */
    log_t log;

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
        return input_t(0, false);
    }

    return input_t(stream, has_rebuilt_flag); /* input_t takes pointer ownership of stream */
}


/**
 * Load a bakery data file. Rebuilds the binary cache if necessary, or if the
 * force_build option is enabled.
 *
 * @param path Path to the datafile.
 * @return input_t for deserialization.
 */
input_t load(const std::string & path)
{
    bakery_t b;
    return b.load(path);
}


} /* namespace bakery */
