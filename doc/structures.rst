Structures
==========

Structures can be defined in *recipe* files:

.. code-block:: c++

   struct example_t {
       int a;
       float b;
   };

   example_t data;

... and set in *data* files:

.. code-block:: c++

   recipe "example.rec";
   data = {
       a = 1;
       b = 3.14159265;
   };


Inheritance
-----------

Structure inheritance can be used to add more fields to an existing structure:

.. code-block:: c++

   struct aircraft_t {
       string name;
       int wings;
   };

   struct jet_t: aircraft_t {
       int reactors;
   };

   aircraft_t data;

.. code-block:: c++

   recipe "planes.rec";
   data = {
       name = "A380";
       wings = 2;
       reactors = 4;
   };


Multiple inheritance
--------------------

A structure can inherits multiple parents. In that case, the order of
declaration is very important because is defines the order of deserialization.
In the following structure declaration, the members of ``something_t`` will be
written first in the binary, the members of ``aircraft_t`` second, and the
member ``reactors`` of ``jet_t`` third.

.. code-block:: c++

   struct jet_t: something_t, aircraft_t {
       int reactors;
   };


Templates
---------

Structures supports template type parametization, like C++ does (although the
syntax for Bakery is simplified).

.. code-block:: c++

   struct point_t<T> {
       T x;
       T y;
   };
   
   point_t<float> point_a;
   point_t<int> point_b;

Setting the values for template types in the *data* file is transparent:

.. code-block:: c++

   recipe "point.rec";
   point_a = { x = 1.5; y = 3.6; };
   point_b = { x = 0; y = 10; };

Multiple template parameters are also supported by adding more typenames
separated with commas. Variadic template parameters are not supported.
