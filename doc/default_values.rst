Default values
==============

Recipes allows defining default values for any variable. When a default value
exists, variable assignment in the data file can be omitted and the default
value will be written in the compiled binary file.

Note: Although they can serve the same purpose, default values are different
from optional values described in next page.

Example with basic types
------------------------

.. code-block:: c++
    :caption: settings.rec

    int width = 1024;
    int height = 768;
    bool fullscreen = false;
    string name = "Player 1";
    int difficulty = 1;

Example with structures
-----------------------

.. code-block:: c++
    :caption: line.rec

    struct point_t {
        int x;
        int y;
    };

    point_t a = {0, 0};
    point_b b = {1, 1};
