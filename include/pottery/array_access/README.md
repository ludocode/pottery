# Array Access

This is a helper template for other Pottery containers. It is not intended to be used directly.

The array access template wraps a configured set of array access expressions over a generalized array. These make it possible for Pottery algorithms to operate efficiently on abstract arrays that do not need to be contiguous in memory and do not need to be in memory at all.

The configured expressions must implement a selection algorithm with optional iteration optimizations. The functions instantiated by this template provide abstract array access and iteration. The main functions are:

- `select()` -- Get the entry at an index, or the end entry if the index is the end of the array
- `index()` -- Get the index of an entry
- `shift()` -- Shifts an entry forwards or backwards by a signed offset
- `offset()` -- Calculates the offset (difference in index) between two entries
- `next()` -- Get the entry following a given entry
- `previous()` -- Get the entry preceding a given entry
- `equal()` -- Compare entries for equality (true if they refer to the same value)

In addition, if the context has an inherent range of values (like a container for example), the template can instantiate functions to access that range:

- `begin()` -- Get an entry to the first value, or the end entry if the context is empty
- `end()` -- Get the end entry
- `count()` -- Get the total number of values
- `exists()` -- Check whether an entry exists (false if it is the end entry, true otherwise)

(This wraps what in C++ might be called a _RandomAccessIterator_, which is a terrible name because randomness has nothing to do with it and it isn't necessarily used for iteration.)



## Description



### Inherent Base and Count

The array access context may have an inherent base entry and/or an inherent value count. For example, if the context is a container, it has an inherent range of values. In this case a separate base entry does not need to be passed to array access functions because it can be retrieved from the context.

Defining `BEGIN` enables "inherent base" mode and defines `POTTERY_ARRAY_ACCESS_INHERENT_BASE` to 1. When this is disabled (the default), all configuration options and all instantiated functions take an additional base entry parameter (after the context, if any.)

Defining `COUNT` or `END` (or both) enables "inherent count" mode and defines `POTTERY_ARRAY_ACCESS_INHERENT_COUNT` to 1. This instantiates additional functions `end()`, `count()` and `exists()` for interacting with the inherent count of the context.

If you want to instantiate a sort template to sort a [vector](../vector) for example, you can make the vector the context and define `BEGIN` and `END`/`COUNT`, giving it an inherent range and enabling both of the above modes. This will instantiate a sort function that takes only the vector itself as argument and sorts its entire contents.

Note that the context is still optional even if there is an inherent base and count. You could use this for example to define a hash table or sort function that operates on static storage.



### End Ref

The configured entry type must support an "end entry" for any valid range of values. This is an entry whose index is one past the end of the range. All configured operations must work on such an entry.

The entry doesn't actually have to "be" one past the end. The end entry of a container can be null for example. It just has to work as though it is one past the end:

- `SELECT` on the total number of values must return the end entry;
- `INDEX` on the end entry must return the total number of values;
- `NEXT` on the last value must return the end entry;
- `PREVIOUS` on the end entry must return the last entry;
- `EXISTS` must return false on the end entry and true on all other entries;
- `EQUAL` must return true if both entries are the end entry and false if only one is the end entry.

This is why the template expects `SELECT` and provides a `select()` function rather than an `at()` function. A `select()` on the total number of values is valid and returns the (unique) non-existent end entry.



### Ref Comparisons

The array access template must be able to compare entries for equality. You can define a custom comparison expression via `EQUAL`. If this is not defined, the template will compare entries by their index in the array.

Any two entries that have the same index must compare equal. This includes the end entry. Equality on two end entry values must be true, no matter how the end entry is obtained (for example if an entry is repeatedly shifted via `next()`, it must eventually compare equal to the entry obtained by `end()`.)



## Configuration Options

Many of these can be synthesized automatically if not defined. For example if `EQUAL` is not defined it will be synthesized by comparing the `INDEX` of the entries. However you can often improve performance by defining more of them.

For example if the array is a weight-balanced tree, you should define `NEXT` and `PREVIOUS` (especially if the tree has next and previous pointers.) Otherwise `next()` and `previous()` will have to walk up to the root of the tree and back down again just to shift the absolute index of an entry by 1.

Documentation of configuration options and API reference is incomplete.
