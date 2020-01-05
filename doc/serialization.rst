Serialization
-------------

Bakery comes with deserialization/serialization methods in order to load/save
data from/to binary streams. Standard types and classes serialization is already
defined by the Bakery library.

For types defined by the library users, serialization operation has to be
defined by implementing the template specialization of ``bakery::serializer<T>``
struct.

Basic serialization implementation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This section shows how to implement serialization and deserialization for basic
structures using a common code for both operations.

.. code-block:: c++
    :caption: demo.rec Recipe

    struct demo_t {
        int x;
        int y;
        string label;
    };

.. code-block:: c++

    #include <bakery/serializers.hpp>

    struct demo_t {
        int x;
        int y;
        std::string label;
    };

    template <> struct bakery::serializer<demo_t> {
        template <typename U, typename IO> void operator()(U u, IO & io) {
            io(u.x)(u.y)(u.label);
        }
    };

This code both implements serialization and deserialization operator, thanks to
the template parameters ``U`` and ``IO``. The C++ code defines the ``demo_t``
structure identically as in the **recipe** file, but this is not mandatory as
long as the binary to data mapping is consistent (for example, the names of the
members in the **recipe** could be different).

Alternatively, the serialization implementation can be written using some Bakery
macros, which hides a little bit the template magics:


.. code-block:: c++

    BAKERY_BASIC_SERIALIZATION_BEGIN(demo_t)
        io(u.x)(u.y)(u.label)
    BAKERY_BASIC_SERIALIZATION_END

Advanced serialization implementation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When serialization and deserialization code cannot be the same, the two
operators can be defined separately. The following snippet shows how
``std::string`` serialization operators are defined in Bakery:

.. code-block:: c++

    template <> struct bakery::serializer<std::string> {
        // Deserialization
        template <typename U, typename IO>
            void operator()(std::string & u, IO & io) {
            size_t size = 0;
            io(size);
            u.resize(size)
            for (size_t i = 0; i < size; ++i)
                io(u[i]);
        }

        // Serialization
        // u is const reference
        template typename U, typename IO>
            void operator()(const std::string & u, IO & io) {
            io(u.size());
            for (char c: u)
                io(c);
        }
    };

