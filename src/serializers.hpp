/**
 * Copyright (C) 2012
 * Olivier Hériveaux, Adrien Boussicault.
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
 * @author Adrien Boussicault
 */


#ifndef _BAKERY_SERIALIZERS_HPP_
#define _BAKERY_SERIALIZERS_HPP_


#include "assertions.hpp"
#include "mpl_sequence_visitation.hpp"
#include <algorithm>
#include <boost/call_traits.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/type_traits.hpp>
#include <boost/variant.hpp>
#include <boost/variant/get.hpp>
#include <list>
#include <map>
#include <vector>


namespace bakery {


class input_t;
class output_t;


/** Sugar macro to define a serializer for a given type T. */
#define BAKERY_BASIC_SERIALIZER_BEGIN(T) template <> struct serializer<T> { \
    template <typename X, typename IO> void operator()(X x, IO & io) {

/** Sugar macro to define a serializer for a given type. Closes any began
 * serializer started with BAKERY_BASIC_SERIALIZER_BEGIN macro. */
#define BAKERY_BASIC_SERIALIZER_END } };


/** Internal class helping arithmetic types serialization. */
template <typename T> struct arithmetic_wrapping_t {};


template <typename T> struct serializer;


/**
 * Serializer for all arithmetic types.
 * This serializer is selected by the compiler using the arithmetic_wrapping_t class.
 */
template <typename E> struct serializer< arithmetic_wrapping_t<E> >
{
    /**
     * Serialization operator.
     *
     * @param u Object to be serialized.
     * @param io Reference to the end-point serializer.
     *
     * Template parameters:
     * - U: reference or const reference to T.
     * - IO: Input or Output.
     */
    template <typename U, typename IO> void operator()(U u, IO & io)
    {
        io.trivial(u);
    }
};


/** Internal class helping enumerations serialization. */
template <typename E> struct enum_wrapping_t {};


/**
 * Serializer for all enumerations.
 * This serializer is selected by the compiler using the enum_wrapping_t class.
 */
template <typename E> struct serializer< enum_wrapping_t<E> >
{
    /**
     * Serialization operator.
     *
     * @param u Object to be serialized.
     * @param io Reference to the end-point serializer.
     * @tparam IO Input or Output.
     */
    template <typename U, typename IO> void operator()(E & u, IO & io)
    {
        int x;
        io.trivial(x);
        u = (E)x;
    }
};


/**
 * Serializer for std::string.
 */
template <> struct serializer<std::string>
{
    /**
     * Serialization operator, for input.
     *
     * @param u Object to be serialized.
     * @param io Reference to the end-point serializer.
     * @tparam U std::string &
     * @tparam IO input_t
     */
    template <typename U, typename IO> void operator()(std::string & u, IO & io)
    {
        size_t size = 0;
        io(size);
        u.resize(size);
        for (size_t i = 0; i < size; ++i)
            io(u[i]);
    }

    /**
     * Serialialization operator, for output.
     *
     * @param u Object to be serialized.
     * @param io Reference to the end-point serializer.
     * @tparam U const std::sttring<T> &
     * @tparam IO output_t.
     */
    template <typename U, typename IO>
        void operator()(const std::string & u, IO & io)
    {
        io(u.size());
        for (char c: u)
            io(c);
    }
};


/**
 * serializer for all std::vector<R>.
 */
template <typename T>
    struct serializer<std::vector<T> >
{
    /**
     * Serialization operator, for input.
     *
     * @param u Object to be serialized.
     * @param io Reference to the end-point serializer.
     * @tparam U std::vector<T> &
     * @tparam IO Input
     */
    template <typename U, typename IO>
        void operator()(std::vector<T> & u, IO & io)
    {
        size_t size = 0;
        io(size);
        u.clear();
        u.resize(size);
        for (auto & item: u)
            io(item);
    }


    /**
     * Serialialization operator, for output.
     *
     * @param u Object to be serialized.
     * @param io Reference to the end-point serializer.
     * @tparam U const std::vector<T> &
     * @tparam IO Output.
     */
    template <typename U, typename IO>
        void operator()(const std::vector<T> & u, IO & io)
    {
        io(u.size());
        std::for_each(u.begin(), u.end(), io);
    }
};


/**
 * serializer for all std::list<R>.
 */
template <typename T>
    struct serializer<std::list<T> >
{
    /**
     * Serialization operator, for input.
     *
     * @param u Object to be serialized.
     * @param io Reference to the end-point serializer.
     *
     * Template parameters:
     * - U: std::list<T> &.
     * - IO: Input.
     */
    template <typename U, typename IO>
        void operator()(std::list<T> & u, IO & io)
    {
        size_t size = 0;
        io(size);
        u.clear();
        u.resize(size);
        for (auto & item: u)
            io(item);
    }


    /**
     * Serialialization operator, for output.
     *
     * @param u Object to be serialized.
     * @param io Reference to the end-point serializer.
     *
     * Template parameters:
     * - U: const std::list<T> &.
     * - IO: Output.
     */
    template <typename U, typename IO>
        void operator()(const std::list<T> & u, IO & io)
    {
        io(u.size());
        std::for_each(u.begin(), u.end(), io);
    }
};


/**
 * serializer for all std::pair<A, B>.
 */
template <typename A, typename B>
    struct serializer<std::pair<A, B> >
{
    /**
     * Serialization operator.
     *
     * @param u Object to be serialized.
     * @param io Reference to the end-point serializer.
     *
     * Template parameters:
     * - U: const std::pair<A, B> & or std::pair<A, B> &.
     * - IO: Input or Output.
     */
    template <typename U, typename IO>
        void operator()(U u, IO & io)
    {
        io(u.first)(u.second);
    }
};


template <typename T, size_t Index> struct tuple_item_serializer_t
{
    void read_tuple(T & u, input_t & input)
    {
        tuple_item_serializer_t<T, Index - 1> x;
        x.read_tuple(u, input);
        input(std::get<Index>(u));
    }
};


template <typename T> struct tuple_item_serializer_t<T, 0>
{
    void read_tuple(T & u, input_t & input)
    {
        input(std::get<0>(u));
    }
};


/**
 * Serializer for std::tuple<...>.
 */
template <typename ... T> struct serializer<std::tuple<T ...>>
{
    /**
     * Serialization operator, for input.
     *
     * @param u Object to be serialized.
     * @param io Reference to the end-point serializer.
     * @tparam U std::tuple<T ...> &
     * @tparam IO input_t
     */
    template <typename U, typename IO>
        void operator()(std::tuple<T ...> & u, IO & io)
    {
        tuple_item_serializer_t<
            std::tuple<T ...>,
            std::tuple_size<std::tuple<T ...>>::value - 1> x;
        x.read_tuple(u, io);
    }
};


/**
 * serializer for all std::map<K, V>.
 */
template <typename K, typename V>
    struct serializer<std::map<K, V> >
{
    /**
     * Serialization operator, for input.
     *
     * @param u Object to be serialized.
     * @param io Reference to the end-point serializer.
     *
     * Template parameters:
     * - U: std::map<K, T> &.
     * - IO: Input.
     */
    template <typename U, typename IO>
        void operator()(std::map<K, V> & u, IO & io)
    {
        size_t size = 0;
        io(size);
        for(size_t x=0; x<size; ++x)
        {
            // Retrieve the key and the value
            std::pair<K, V> kv;
            io(kv);

            // Store the element in the map
            u[kv.first] = kv.second;
        }

        // Verify that there were no two identical keys in the binary stream.
        // During insertion, if a key is already present, the value is simply
        // overwritten. In case of duplicate keys, the final size of the map
        // will not match the expected size.
        bakery_assert_message(size == u.size(),
            "Map deserialization error: stream contains (key, value) couples "
            "with identical keys.");
    }


    /**
     * Serialialization operator, for output.
     *
     * @param u Object to be serialized.
     * @param io Reference to the end-point serializer.
     *
     * Template parameters:
     * - U: const std::map<K, V> &
     * - IO: Output.
     */
    template <typename U, typename IO>
        void operator()(const std::map<K, V> & u, IO & io)
    {
        io(u.size());
        // map iterators return std::pair<K, V>, we can directly use the
        // serializer<std::pair<A, B> > upper.
        std::for_each(u.begin(), u.end(), io);
    }
};


/**
 * @param U Variant type.
 * @param IO Input/output object type.
 */
template <typename U, typename IO>
    struct boost_variant_mpl_sequence_visitor_in
{
    /**
     * Constructor.
     *
     * @param u_ Serialized object.
     * @param io_ Input/output object.
     */
    boost_variant_mpl_sequence_visitor_in(U & u_, IO & io_):
        u(u_),
        io(io_)
    {}

    /** Serialized object. */
    U & u;

    /** Input/output object. */
    IO & io;

    template <typename T>
        void visit()
    {
        u = T();
        io( boost::get<T>(u) );
    }
};


/**
 * @tparam U Variant type.
 * @tparam O Output object type.
 */
template <typename U, typename O> struct variant_visitor_out_t
{
    /** Visitor result type. Required by bakery::variant visitation mechanism. */
    typedef void result_type;

    /** Serialized object. */
    const U & u;

    /** Output object. */
    O & o;

    template <typename T> void operator()(const T &) const
        { o(boost::get<T>(u)); }
};


/**
 * Serializer for any boost::variant built over a MPL sequence.
 *
 * If a variant is defined from a MPL sequence like this:
 *
 * boost::make_variant_over<boost::mpl::vector<T0, T1, ...>>::type
 *
 * It behaves like a boost::variant<T0, T1, ...>, but in internals it
 * corresponds to a:
 *
 * boost::variant<
 *     boost::detail::variant::over_sequence<
 *         boost::mpl::vector<T0, T1, ...>
 *     >
 * >
 *
 * To handle such type, we need to specialize the serializer for variants where
 * T0 is a over_sequence<T> where T is a MPL sequence.
 */
template <typename T>
    struct serializer<
        boost::variant<
            boost::detail::variant::over_sequence<T>
        >
    >
{
    typedef typename boost::variant<
        boost::detail::variant::over_sequence<T>
    > variant_type;

    template <typename U, typename IO>
        void operator()(
            U & u,
            IO & io
        )
    {
        unsigned int which;
        io(which);

        boost_variant_mpl_sequence_visitor_in<U, IO> v(u, io);
        apply_mpl_sequence_visitor<
            T,
            boost_variant_mpl_sequence_visitor_in<U, IO>
        >((int)which, v);
    }
};



template <typename ... T>
struct serializer< boost::variant<T ...> >{

    typedef typename boost::variant<T ...> VariantType;

    template <typename U, typename IO>
    void operator()(
        VariantType & u,
        IO & io
    ){
        unsigned int which;
        io(which);

        /* Verify the type index. */
        bakery_assert_message(
            which < sizeof ... (T),
            "Invalid variant type id."
        );

        boost_variant_mpl_sequence_visitor_in<U, IO> v(u, io);
        apply_mpl_sequence_visitor<
            typename boost::variant<T ...>::types,
            boost_variant_mpl_sequence_visitor_in<U, IO>
        >((int)which, v);
    }

    template <typename U, typename IO> void
        operator ()(const boost::variant<T ...> & u, IO & io)
    {
        io((unsigned int)u.which());
        variant_visitor_out_t<U, IO> v = {u, io};
        u.apply_visitor(v);
    }
};


} /* namespace bakery */


#endif

