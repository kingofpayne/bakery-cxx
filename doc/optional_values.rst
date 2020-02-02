Optional values
===============

A variable can de defined as optional in a recipe. When a variable is optional,
assignment in the data file can be omitted.

In the compiled binary, optional values are written using a boolean header
indicating if the variable is defined or not. If the variable is defined, it is
written in the binary file, otherwise it is missing from the binary and it is up
to the serialization to assign a default value when reading the binary. Optional
values are therefore more complex to handle in the program, but the produced
binaries can be much smaller than when using default values described in the
previous page.

A variable cannot be optional and have a default value defined.

Example with basic types
------------------------

.. code-block:: c++
    :caption: settings.rec

    optional int width;
    optional int height;
    optional bool fullscreen;
    optional string name;
    optional int difficulty;

Example with structures
-----------------------

.. code-block:: c++
    :caption: line.rec

    struct point_t {
        int x;
        int y;
    };

    optional point_t a;
    optional point_b b;
