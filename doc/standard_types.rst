Standard types
==============

The following table lists all the types natively supported by the Bakery
library. It includes standard native types such as ``int``, ``float``,
``bool``, more complex types such as ``string``, and also generic types
like ``list<T>``.

The first column shows the type names to be used in the *recipe* files.
The second column shows the equivalent type which can be used for
deserialization in the C++ program. Note that one bakery type can have
multiple equivalent C++ types: for instance ``list`` can be deserialized
into ``std::list<T>`` or ``std::vector<T>``.

+------------------------+------------------------+
| Bakery type            | C++ types              |
+========================+========================+
| ``int``                | ``int``                |
+------------------------+------------------------+
| ``short``              | ``short``              |
+------------------------+------------------------+
| ``char``               | ``char``               |
+------------------------+------------------------+
| ``float``              | ``float``              |
+------------------------+------------------------+
| ``double``             | ``double``             |
+------------------------+------------------------+
| ``string``             | ``std::string``        |
+------------------------+------------------------+
| ``pair<A, B>``         | ``std::pair<A, B>``    |
+------------------------+------------------------+
| ``tuple<T0, T1, ...>`` | ``std::tuple<T...>``   |
+------------------------+------------------------+
| ``list<T>``            | | ``std::list<T>``     |
|                        | | ``std::vector<T>``   |
+------------------------+------------------------+
| ``map<K, V>``          | ``std::map<K, V>``     |
+------------------------+------------------------+

The binary data size and endianess of the primitive types are architecture
dependent, just like C/C++ is. This means you don't have to worry about this
when loading data with Bakery, but this also means a compiled binary may not
work as intended when copied on another architecture. The recommendation is to
always copy the data and recipe files, but not the generated binaries.

Architecture independant types may be added in future versions of Bakery.
