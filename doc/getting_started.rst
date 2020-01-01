Introduction
============

Bakery is a binary data asset loading library. It allows easy creation of data
files using a C++-like syntax, provides just-in-time binary compilation and
C++ deserialization API.

Bakery has been designed for taking both perks of data description langages, such
as XML or JSON, and binary files loading speed. Loading a bakery data file only
requires support for binary deserialization in the final program, which is easy
to implement and maintain - no need to parse the data files (bakery does it for
you).


Getting started
===============

In the following very quick example, we want a video game program to load
settings from a configuration file: screen resolution, fullscreen option,
player name and game difficulty. We want the configuration file to be editable
with any simple text editor. Also, we don't want to have complex code to parse
and load this file in our program code.

Using the Bakery library, the first step is to write a **recipe** file which
will specify what fields are to be expected in the settings file:

.. code-block:: c++
    :caption: settings.rec

    int width;
    int height;
    bool fullscreen;
    string name;
    int difficulty;

The **recipe** will tell Bakery what fields must be set in the config **data**
file. Each field has a defined type. The **data** file can have the following
content:

.. code-block:: c++
    :caption: settings.dat

    recipe "settings.rec";
    name = "Gordon";
    difficulty = 3;
    width = 1024;
    height = 768;
    fullscreen = true;

Once this **data** file has been written, loading is very simple and
straightforward:

.. code-block:: c++
    :caption: demo.cpp

    #include <bakery/bakery.hpp>
    ...
    int width, height;
    bool fullscreen;
    std::string name;
    int difficulty;

    bakery::input_t input = bakery::load("settings.dat");
    input >> width >> height >> fullscreen >> name >> difficulty;

What's happening ?
------------------

In the example above, the fields defined in the **data** file are not in the
same order as specified in the **recipe**. It's not a bug, it's a feature.

In the code loading the data file, the fields are deserialized in the same
order as they are declared in the **recipe**, and this is very important! When
calling the ``load`` method, the bakery library builds a binary file
*settings.bin* from the **data** file and the **recipe**. The order of the
fields written in the binary file is defined by the recipe. The generated
binary will look as defined below (for a x64 architecture):

.. code-block::
   :caption: hexdump -Cv settings.bin

   00000000  00 04 00 00 00 03 00 00  01 06 00 00 00 00 00 00  |................|
   00000010  00 47 6f 72 64 6f 6e 03  00 00 00                 |.Gordon....|

The first 4 bytes ``00 04 00 00`` store the width field value (1024 in little
endian). The next 4 bytes ``00 03 00 00`` store the height field value. The next
byte ``01`` is the fullscreen option bool. Then comes the player name: it
starts with the value length 6 followed by 6 ASCII characters. Finally, the last
4 bytes ``03 00 00 00`` are for the difficulty setting.

The builded binary is then open by the program and deserialized into the local
variables using the ``>>`` operator. Bakery defines deserialization operations
for many types, such as ``std::string`` here.

If the *settings.dat* file is not modified, running the program a second time
will not rebuild the binary file and will directly deserialize it. This means
loading the data will be really fast since no grammar parsing will happen this
time. For this small example the difference won't be noticable, but when using
large data files, such as a 3D animated model, this caching mechanism is really
efficient.
