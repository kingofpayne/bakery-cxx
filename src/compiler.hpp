/**
 * Copyright (C) 2010, 2011, 2012, 2013, 2014
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


#ifndef _BAKERY_COMPILER_HPP_
#define _BAKERY_COMPILER_HPP_


#include "log.hpp"
#include <algorithm>
#include <fstream>
#include "dat/data.hpp"
#include "rec/recipe.hpp"
#include "file_indication.hpp"
#include "tti_stack.hpp"
#include <gmpxx.h>
#include <string>


namespace bakery {
namespace compiler {


/**
 * Structure helping passing data through the decompilation process. This helps
 * reducing the number of arguments passed in the functions.
 */
struct decompilation_state_t
{
    /** Reference to the input binary stream. */
    std::ifstream & input;
    /** Reference to the output data file. */
    std::ofstream & output;
    /** Bytes available from the input binary file. This is decreased during the
     * decompilation process. */
    size_t size;
    /** Stack of template instanciations. */
    tti_stack_t tti_stack;
    /** Indentation level. */
    int indent_level;
    /** Log, for info, warning and error messages. */
    log_t & log;

    void write_indent();
};


/**
 * Structure helping passing data through the compilation process. This helps
 * reducing the number of arguments passed in the functions.
 */
struct compilation_state_t
{
    /** Reference to the output data stream. */
    std::ostream & output;
    /** Stack of the template instanciations. */
    tti_stack_t tti_stack;
    /** Log, for info, warning and error messages. */
    log_t & log;
};


void compile(const std::string &, const std::string &,
    const std::list<std::string> &, log_t &);

void decompile(const std::string &, const std::string &, const std::string &,
    const std::list<std::string> &, log_t &);

void populate_node(rec::node::sptr);

bool write_data(compilation_state_t &, const dat::data &, const std::string &,
    const std::list<std::string> &);

bool write_node(compilation_state_t &, const rec::type_instanciation_t &,
    const dat::node &);

bool write_template_type(compilation_state_t &,
    const rec::type_instanciation_t &, const dat::node &);

bool write_structure(compilation_state_t &, const rec::type_instanciation_t &,
    const dat::node &);

bool write_variant(compilation_state_t &, const rec::type_instanciation_t &,
    const dat::node &);

bool write_array(compilation_state_t &, const rec::type_instanciation_t &,
    const dat::node &);

bool write_array_dim(compilation_state_t &, const rec::type_instanciation_t &,
    size_t, const dat::node &);

bool write_typedef(compilation_state_t &, const rec::type_instanciation_t &,
    const dat::node &);

bool write_enum(compilation_state_t &, const rec::type_instanciation_t &,
    const dat::node &);

bool write_native(compilation_state_t &, const rec::type_instanciation_t &,
    const dat::node &);

bool write_native_pair(compilation_state_t &, const rec::type_instanciation_t &,
    const dat::node &);

bool write_native_tuple(compilation_state_t &,
    const rec::type_instanciation_t &, const dat::node &);

bool write_native_list(compilation_state_t &, const rec::type_instanciation_t &,
    const dat::node &);

bool write_native_map(compilation_state_t &, const rec::type_instanciation_t &,
    const dat::node &);

bool write_native_bool(compilation_state_t &, const rec::type_instanciation_t &,
    const dat::node &);

bool write_native_integer(compilation_state_t &,
    const rec::type_instanciation_t &, const dat::node &);

bool write_native_string(compilation_state_t &,
    const rec::type_instanciation_t &, const dat::node &);

bool write_native_floating(compilation_state_t &,
    const rec::type_instanciation_t &, const dat::node &);

bool read_data(decompilation_state_t &, const file_indication_t &,
    const std::string &, const std::list<std::string> &);

bool read_structure(decompilation_state_t &, const rec::type_instanciation_t &);
bool read_varian(decompilation_state_t &, const rec::type_instanciation_t &);

bool read_array_dim(decompilation_state_t &, const rec::type_instanciation_t &,
    size_t);

bool read_enum(decompilation_state_t &, const rec::type_instanciation_t &);
bool read_typedef(decompilation_state_t &, const rec::type_instanciation_t &);
bool read_type(decompilation_state_t &, const rec::type_instanciation_t &);


/**
 * Reads a native type from the input binary file of a decompilation.
 *
 * @param state Decompilation state. When this function is successfull,
 *        the remaining byte count is reduced and the input file stream
 *        position is advanced.
 * @param dest Where the result is stored.
 *
 * @return True if the value has been read, false if there is not enough
 *         remaining bytes.
 */
template <typename T> static bool read_native_type(
    decompilation_state_t & state, T & dest)
{
    const size_t size = sizeof(T);

    if (size > state.size)
    {
        state.log.add_message(
            log_message_type_t::error,
            "end of file reached, not enough bytes available.");

        return false;
    }
    else
    {
        state.input.read((char*)&dest, size);
        state.size -= size;
        return true;
    }
}


bool read_template_type(decompilation_state_t &,
    const rec::type_instanciation_t &);

bool read_native(decompilation_state_t &, const rec::type_instanciation_t &);

bool read_native_bool(decompilation_state_t &,
    const rec::type_instanciation_t &);

bool read_native_integer(decompilation_state_t &,
    const rec::type_instanciation_t &);

bool read_native_floating(decompilation_state_t &,
    const rec::type_instanciation_t &);

bool read_native_string(decompilation_state_t &,
    const rec::type_instanciation_t &);

bool read_native_tuple(decompilation_state_t &,
    const rec::type_instanciation_t &);

bool read_native_list(decompilation_state_t &,
    const rec::type_instanciation_t &);

bool read_native_map(decompilation_state_t &,
    const rec::type_instanciation_t &);

bool floating_to_mpf(mpf_class &, const dat::floating &, log_t &);

bool check_template_parameter_count(const rec::type_instanciation_t &,
    log_t &);

bool merge_included_recipe_files(rec::recipe &, const std::string &,
    const std::list<std::string> &, std::list<std::string> &, log_t &);

bool resolve_file_indication(const file_indication_t &, const std::string &,
    const std::list<std::string> &, std::string &, log_t &);

mpz_class make_mpz_10_power(unsigned int power);


} /* namespace compiler */
} /* namespace bakery */


#endif

