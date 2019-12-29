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

A bakery data file is a text file with C++ like syntax, which sets the value of
defined field to be loaded. When loading a file in a program, the bakery libray
parses the text datafile and compiles it into a binary file which can be loaded
and deserialized. The compilation process requires an extra file, called recipe,
which tells bakery how to build the text data file into a binary representation.

The following *demo.dat* file shows the content of a Bakery data file:

.. code-block:: c++

    recipe "demo.rec";
    firstname = "Emmett";
    lastname = "Brown";
    age = 70;

The first line of the data file indicates what *recipe* should be used to build
the binary file *demo.bin*. The recipe *demo.rec* has the following definition:

.. code-block:: c++

    string firstname;
    string lastname;
    int age;

The *demo.dat* data file can then be loaded and deserialized easily in the
program using the following C++ code:

.. code-block:: c++

    std::string firstname, lastname;
    int age;
    
    bakery::input_t input = bakery::load("demo.dat");
    input >> firstname >> lastname >> age;
