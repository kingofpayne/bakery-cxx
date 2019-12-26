/**
 * Copyright (C) 2010
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


#include "util.hpp"
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/if.hpp>


namespace bakery {


/**
 * @return A copy of s where each line have been indented.
 *
 * @param s Original string (line separator must be '\n').
 */
std::string indent(const std::string & s)
{
	std::string k = "    ";
	std::string r = k;

	for(unsigned int i=0; i<s.length(); ++i){
		char c = s[i];
		r.push_back(c);
		if(c == '\n'){
			r += k;
		}
	}

	return r;
}


/**
 * @return A string showing a position in a line.
 *
 * @param s The string.
 * @param i Iterator indicating the position to show.
 *
 * Example of output:
 *
 * void f(inth test)
 *            ^
 */
std::string show_string_position(
	const std::string & s,
	const std::string::const_iterator i)
{
	const std::string::const_iterator it_begin = s.begin();
	const std::string::const_iterator it_end = s.end();

	/* Firstly, we go backward to find the beginning of the line. */
	std::string::const_iterator it_start = i;
	while((*it_start != '\n') && (it_start != it_begin))
		it_start--;
	if(*it_start == '\n'){
		/* Algorithm stopped at the end of a previous line, step one character
		 * forward. */
		it_start++;
	}

	/* Now, find the end of the line (or string) */
	std::string::const_iterator it_stop = i;
	while((*it_stop != '\n') && (it_stop != it_end))
		it_stop++;

	std::string result;
	std::for_each(
		it_start,
		it_stop,
		boost::bind(
			&std::string::push_back,
			&result,
			_1
		)
	);

	result.insert(i-it_start, "[!]");

	return result;
}


/**
 * @return The line number at which the given iterator stands.
 *
 * Lines are detected which the character '\n'. The result 1 corresponds to the
 * first line (value 0 should not be returned).
 *
 * @param s The string.
 * @param i The iterator.
 */
unsigned int calculate_line_number(
	const std::string & s,
	const std::string::const_iterator i)
{
	unsigned int line = 1;
	std::string::const_iterator i_begin = s.begin();
	std::for_each(
		i_begin,
		i,
		boost::lambda::if_then(
			boost::lambda::_1 == '\n',
			boost::lambda::var(line)++
		)
	);

	return line;
}


/**
 * @return Absolute path corresponding to the given relative or absolute path.
 *
 * @param path Input path.
 */
boost::filesystem::path make_absolute(const boost::filesystem::path & path)
{
	return path.is_complete()
		?path
		:(boost::filesystem::current_path() / path);
}


/**
 * Computes relative of b path to a path.
 *
 * @param a First path.
 * @param b Second path.
 *
 * @return A path p, such as a/p corresponds to b.
 */
boost::filesystem::path make_relative(boost::filesystem::path a,
	boost::filesystem::path b)
{
	using namespace boost::filesystem;

	/* The result. */
	path p;

	/* Make a and b absolutes. */
	a = absolute(a);
	b = absolute(b);

	path::const_iterator
		it_a(a.begin()),
		it_b(b.begin()),
		it_a_end(a.end()),
		it_b_end(b.end());

	/* Find common directories by advancing iterators while the elements are
	 * identical. */
	while ((it_a != it_a_end) && (it_b != it_b_end) && (*it_a == *it_b))
	{
		it_a++;
		it_b++;
	}

	/* Walk up directories. */
	while (it_a != it_a_end)
	{
		if ((*it_a) != ".")
			p /= "..";
		it_a++;
	}

	/* Append the non-common part. */
	while (it_b != it_b_end)
	{
		p /= *it_b;
		it_b++;
	}

	return p;
}


/**
 * Given a base directory, deduce a relative path. Considering base is
 * '/home/pouet/test' :
 * - if path is 'toto.ogg', result is '/home/pouet/test/toto.ogg',
 * - if path is '/something/toto.ogg', result is path.
 *
 * @param base Base path.
 * @param path The path.
 *
 * @return Deduced path.
 */
boost::filesystem::path path_from_base(const boost::filesystem::path & base,
	const boost::filesystem::path & path)
{
	return path.is_complete()
		?path
		:base/path;
}


} /* namespace bakery */

