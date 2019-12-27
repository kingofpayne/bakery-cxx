/**
 * Copyright (C) 2010, 2011, 2012, 2013
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


/* To be included before spirit and phoenix. */
#include "../enable_phoenix_v3.hpp"

#include "../assertions.hpp"
#include "../util.hpp"
#include "node.hpp"
#include <algorithm>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/if.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/spirit/include/phoenix.hpp>


namespace bakery {
namespace dat {


const std::string node::kind::strings[] =
{
	"none",
	"assignment",
	"string",
	"floating", 
	"identifier",
	"bool_",
	"group",
	"variant",
	"map_assignment"
};


/**
 * Default constructor.
 */
node::node():
	name(),
	kind(kind::none),
	children(),
	data()
{}


/**
 * Constructor
 *
 * @param _kind kind of the node. Must be a member of the enumeration
 *        node::kind::value.
 *
 * @throw core::exceptions::invalid_enum_parameter* if _kind is not a member of
 *        the enumeration node::kind::value.
 */
node::node(kind::value _kind):
	name(),
	kind(_kind),
	children()
{
	bakery_assert(kind::wrapper::has(_kind));

	switch(_kind)
	{
		case kind::group:
		case kind::assignment:
		case kind::map_assignment:
		case kind::variant:
			break;

		case kind::string:
			data = std::string();
			break;

		case kind::floating:
			data = floating();
			break;

		case kind::identifier:
			data = std::string();
			break;

		case kind::bool_:
			data = false;
			break;

		default:
			bakery_unexpected_case();
	}
}


/**
 * @return kind of the node.
 */
node::kind::value node::get_kind() const
{
	return kind;
}


/**
 * @return Const reference on the list of children.
 */
const node::node_sptr_list & node::get_children() const
{
	return children;
}


/**
 * @return The name of the node (may be an empty string).
 */
const std::string & node::get_name() const
{
	return name;
}


/**
 * @return The string stored by the node. This function is available only when
 *         the nodes's kind is kind::string.
 */
const std::string & node::get_string() const
{
	bakery_assert(kind == kind::string);
	return boost::get<const std::string>(data);
}


/**
 * @return Const reference to the floating stored by the node. This function is
 *         available only when the node's kind is kind::floating.
 *
 * @throw core::exceptions::unexpected_call if node's kind is not kind::floating.
 */
const floating & node::get_floating() const
{
	bakery_assert(kind == kind::floating);
	return boost::get<const floating>(data);
}


/**
 * @return The identifier stored by the node. This function is available only
 *         when the nodes's kind is kind::string.
 *
 * @throw core::exceptions::unexpected_call if node's kind is node
 *        kind::identifier.
 */
const std::string & node::get_identifier() const
{
	bakery_assert(kind == kind::identifier);
	return boost::get<const std::string>(data);
}


/**
 * @return The bool stored by the node. This function is available only when the
 *         node's kind is kind::bool_.
 *
 * @throw core::exceptions::unexpected_call if node's kind is not kind::bool_.
 */
bool node::get_bool() const
{
	bakery_assert(kind == kind::bool_);
	return boost::get<bool>(data);
}


/**
 * @return A string representing the content of the node.
 */
std::string node::print() const
{
	std::string r;

	if(name.empty())
		r = "anonymous";
	else
		r = name;

	r += " is " + kind::wrapper::to_string(kind);

	if(!children.empty())
	{
		using boost::phoenix::ref;
		using boost::phoenix::if_;
		using namespace boost::phoenix::arg_names;
		using boost::phoenix::lambda;

		r += " {\n";
		std::string r2;
		bool first = true;

		std::for_each(
			children.begin(),
			children.end(),
			(
				[&](const node::sptr & x)
				{
					if (first)
						first = false;
					else
						r2 += "\n" + x->print();
				}
			)
		);

		r += indent(r2);
		r += "\n}";
	}

	switch(kind)
	{
		case kind::string:
			r += " (" + get_string() + ")";
			break;

		default:;
	}

	return r;
}


/**
 * Defines the name of the node.
 *
 * @param value Name of the node, or an empty string.
 */
void node::set_name(const std::string & value)
{
	name = value;
}


/**
 * Adds a child node.
 *
 * @param c Child node.
 */
void node::add_child(const node::sptr & c)
{
	children.push_back(c);
}


/**
 * Sets the string stored by the node. This function is available only when the
 * node's kind is kind::string.
 *
 * @param value The string.
 *
 * @throw core::exceptions::unexpected_call if node's kind is not kind::string.
 */
void node::set_string(const std::string & value)
{
	bakery_assert(kind == kind::string);
	data = value;
}


/**
 * Defines the floating stored by the node. This function is available only when
 * the node's kind is kind::floating.
 *
 * @param value The floating.
 *
 * @throw core::exceptions::unexpected_call if node's kind is not kind::floating.
 */
void node::set_floating(const floating & value) 
{
	bakery_assert(kind == kind::floating);
	data = value;
}


/**
 * Sets the identifier stored by the node. This function is available only when
 * the node's kind is kind::identifier.
 *
 * @param value The identifier.
 *
 * @throw core::exceptions::unexpected_call if node's kind is not
 *        kind::identifier.
 */
void node::set_identifier(const std::string & value)
{
	bakery_assert(kind == kind::identifier);
	data = value;
}


/**
 * Defines the bool stored by the node. This function is available only when the
 * node's kind is kind::bool_.
 *
 * @param value The bool.
 *
 * @throw core::exceptions::unexpected_call* if the node's kind is
 *        not kind::bool_.
 */
void node::set_bool(bool value)
{
	bakery_assert(kind == kind::bool_);
	data = value;
}


} /* namespace dat */
} /* namespace bakery */

