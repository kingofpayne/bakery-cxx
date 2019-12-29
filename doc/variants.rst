Variants
========

Bakery supports variant types, which can be deserialized to ``std::variant`` or
``boost::variant``. The following *recipe* shows a variant definition example:

.. code-block:: c++

   variant numeric_t {
       int a;
       float b;
       bool c;
   };

   numeric_t x;
   numeric_t y;
   numeric_t z;

Assignment in the *data* files is as follows:

.. code-block:: c++

   recipe "variants.rec";
   x = a: 5;
   y = b: 3.0;
   z = c: true;

