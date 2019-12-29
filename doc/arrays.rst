Arrays
======

Bakery supports fixed and dynamic arrays, as shown in the example *recipe* below:

.. code-block:: c++

   /* Fixed array */
   int[3] a;

   /* Dynamic array */
   int[] b;

   /* Multi-dimensional array */
   int[2][3] c;

Values are assigned as shown in the following *data* file below:

.. code-block:: c++

   recipe "arrays.rec";
   a = {1, 2, 3};
   b = {1, 2, 3, 4, 5, 6};
   c = {{1, 2}, {3, 4}, {5, 6}};

Dynamic arrays are equivalent to the ``list<T>`` type.
