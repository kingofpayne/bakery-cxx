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
#include "output.hpp"
#include <boost/filesystem.hpp>
#include <iostream>


namespace bakery {


/**
 * Deserialize an input into a destination variable.
 * Commodity for bakery_t class.
 *
 * @param input Input stream.
 * @param t Reference to the destination variable.
 */
template <typename T> void deserialize_many(input_t & input, T& t)
{
    input >> t;
}


/**
 * Deserialize an input into multiple destination variables.
 * Commodity for bakery_t class.
 *
 * @param input input stream.
 * @param t Reference to the first destination variable.
 * @param u... Others destination variables.
 */
template <typename T, typename ... U>
    void deserialize_many(input_t & input, T& t, U&... u)
{
    input >> t;
    deserialize_many(input, u...);
}


/**
 * Serialize data into an output stream.
 * Commodity for bakery_t class.
 *
 * @param output Output stream.
 * @param t Const reference to the source variable.
 */
template <typename T> void serialize_many(output_t & output, const T& t)
{
    output << t;
}


/**
 * Serialize multiple variables into an output stream.
 *
 * @param output Output stream.
 * @param t Const reference to the first source variable.
 * @param u... Const reference to the other source variables.
 */
template <typename T, typename ... U>
    void serialize_many(output_t & output, const T & t, const U&... u)
{
    output << t;
    serialize_many(output, u...);
}


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
        input_t load_input(const std::string &, log_t &) const;

        /**
         * Load a bakery data file and deserialize it in destination variables.
         *
         * @param path Path to the data file.
         * @param dest Reference to destination variable.
         * @return Log object containing potential error messages.
         */
        template <typename ... T>
            log_t load(const std::string & path, T&... dest) const
        {
            log_t log;
            input_t input = load_input(path, log);
            if (input)
                deserialize_many(input, dest...);
            return log;
        }

        /**
         * Save a bakery data in binary using serialization, and then decompiles
         * it to regenerate a text data file.
         *
         * @param dat_path Path to the data file.
         * @param rec_path Path to the recipe file to be used for decompilation.
         * @return false in case of error (if abort_on_error is disabled), true
         *     if the binary and data files have been written.
         */
        template <typename ... T> log_t save(const std::string & dat_path,
            const std::string & rec_path, const T & ... src) const
        {
            log_t log;
            boost::filesystem::path bin_path(dat_path);
            bin_path.replace_extension(".bin");
            {
                output_t output;
                output.set_stream(new std::ofstream(bin_path.c_str()));
                if (output)
                {
                    serialize_many(output, src...);
                }
                else
                {
                    std::string msg = "Failed to open output file "
                        + std::string(bin_path.c_str()) + ".";
                    if (abort_on_error)
                        bakery_abort_message(msg);
                    else if (verbose)
                        std::cout << msg << std::endl;
                    log.error(msg);
                    return log;
                }
                /* file will be closed here */
            }
            compiler::decompile(bin_path.c_str(), rec_path, dat_path,
                include_directories, log);
            if (!log)
            {
                if (verbose)
                {
                    std::cout << "An error occured during decompilation of "
                        "resource " << dat_path << ", below are listed the "
                        "error messages reported during decompilation."
                        << std::endl;
                    log.print();
                }

                if (abort_on_error)
                    bakery_abort_message("Cannot decompile resource " + dat_path + ".");
            }
            return log;
        }

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


/**
 * Load a bakery data file and deserialize it in destination variables.
 * Rebuilds the binary cache if necessary.
 * If options for loading data has to be set, use the bakery_t class instead.
 *
 * @param path Path to the datafile.
 * @param dest Reference to destination variables.
 * @return false in case of error, true if data has been written into dest
 *     variables.
 */
template <typename ...T> log_t load(const std::string & path, T&... dest)
{
    bakery_t b;
    return b.load(path, dest...);
}


} /* namespace bakery */


#endif
