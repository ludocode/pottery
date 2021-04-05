# Glossary

This document describes the vocabulary and naming conventions used by Pottery.

Pottery enforces strong consistency in naming. This is important in order to make APIs predictable and easy to understand.

For example, all of the below functions _remove_ a value from a container, destroying it in the process:

- `remove()`
- `remove_at()`
- `remove_first()`
- `remove_last()`
- `remove_before()`
- `remove_after()`
- `remove_and_next()`
- `remove_and_previous()`

Similarly, all of the below functions access and manipulate the _last_ entry in a container:

- `last()`
- `insert_last()`
- `emplace_last()`
- `construct_last()`
- `extract_last()`
- `remove_last()`
- `displace_last()`

These functions have the same behaviour across all Pottery containers. If a container does not support the necessary functionality (by configuration or by the algorithm used by the container), the function does not exist.

Compare this to `insert` versus `push_back` in `std::vector`, or `top` of `std::stack` versus `front` of `std::deque`. These are entirely different words for the same operations. Such inconsistencies make it difficult to use the C++ STL without either following an API reference or memorizing all sorts of special cases.

Pottery strives to eliminate inconsistencies as much as possible.



## Templates

The naming conventions for templates are a bit loose, but generally follow these rules in descending order of priority:

- A **`map`** suffix indicates an associative container that owns its values and dynamically allocates its storage. An example is [`open_hash_map`](../include/pottery/open_hash_map/)).

- A **`sort`** suffix indicates a sorting algorithm. So far only array sorts are supported. Non-array sorts (such as merge sort of a linked list) may or may not carry a **sort** suffix if they are ever implemented.

- A **`tree`** suffix indicates an intrusive container. This only applies to the suffix. For example `red_black_tree` is an intrusive tree but `b_tree_map` is a dynamic map container.

- A **`table`** suffix indicates a hash table that operates on externally allocated memory. An example is [`open_hash_table`](../include/pottery/open_hash_table/)).

A few other terms are used for templates, but they are not reflected in the names of the templates:

- **Array** is used for a user-ordered container that supports direct value indexing. These typically support not only `at()` for looking up values directly, but also `emplace_at()`, `displace_at()` and others. An example is [`vector`](../include/pottery/vector/).

- **Queue** is used for a container that efficiently supports mutations on at least one end of the values in the container based on its ordering. These can be user-ordered (such as [`ring`](../include/pottery/ring/), [`vector`](../include/pottery/vector/) and [`pager`](../include/pottery/pager/)) or unordered (such as [`heap`](../include/pottery/vector/).) (If the container efficiently supports mutations on both ends, it is a **double-ended queue**.

- **Helper** is used to refer to a template that is meant mostly to be used by other templates. These generally implement functionality internal to Pottery. Examples are [`alloc`](../include/pottery/alloc/) and [`compare`](../include/pottery/compare/). These can nevertheless be used on their own if desired.



## Types

Almost all algorithms and containers in Pottery deal with the following types:


### **Value**

A **value** type is the name given to the concrete type for a value stored in a container.

This may or may not exist. If this type does not exist, we still speak of "values" conceptually, but they may not even be stored in memory; they could be entries in an external database for example.

A value type is configured by `VALUE_TYPE` and defined by templates as `value_t`. If a `VALUE_TYPE` is not given, `value_t` does not exist.

Note that this should not be confused with the expression "by value", which refers to the argument passing and return value passing mechanism in C. See also "pass" in this glossary.


### **Ref**

A **ref** is an abstract reference to a value, which may or may not be contained in a container.

If a concrete value type exists, this is a pointer to it; otherwise it's an abstract handle to some external conceptual value.

A ref type is configured by `REF_TYPE` and defined by templates as `ref_t`. If a `REF_TYPE` is not given, `ref_t` is defined as a pointer to `value_t` (which must exist.)

This is a separate concept from entry in part because for real values, a ref can refer to a temporary on the stack. This is why the [lifecycle](../include/pottery/lifecycle/) template deals with refs and not entries.

This is also a separate concept from value because some templates do not have concrete values at all, and a ref is just an abstract handle to some conceptual external value. A ref for example can be an integer, a UUID, a string, etc.


### **Entry**

An **entry** is an abstract reference to the storage location of a value within a container, with sufficient metadata to make operations like iteration and removal possible.

For some containers this is the same as ref. For example in a [vector](../include/pottery/vector/), an entry is just a pointer to a value in the vector's buffer. This is called a "trivial entry type".

In other containers, this is a struct containing additional data needed to locate the value. For example in a [pager](../include/pottery/pager/), the entry is a small struct containing the value as well as the page in which it's contained.

An entry type is configured by `ENTRY_TYPE` and defined templates as `entry_t`. If an `ENTRY_TYPE` is not given, `entry_t` is defined to be the same as `ref_t`.

If `entry_t` is not implicitly convertible to `ref_t`, you must configure an `ENTRY_REF` expression get an entry's ref (usually a pointer to the value contained in the entry.)


### **Key**

A **key** is an abstract reference used for lookup in associative arrays (trees, maps, tables.)

Keys are hashed and compared with other keys to search a container and enforce its constraints. In Pottery, values contain their own keys. Usually you will want the key type to be a pointer to some portion of the value (or to the value itself in the special case of a set.)

A key type is configured by `KEY_TYPE` and defined by the template as `key_t`. If a `KEY_TYPE` is not given, `key_t` is defined to be the same as `ref_t`.


### **Context**

A **context** is an arbitrary value used to supply a user context to configured expressions on templates.

Contexts are optional. If enabled, stateful containers typically take the context on initialization and store it within the container, while other templates take the context as the first argument to all functions.

A context type is configured by `CONTEXT_TYPE` and defined by templates as `context_t`.

Like all configured types, a single context type is used for a template and all its children. If you want different contexts for different purposes (for example separate allocation and compare contexts), you will need to pack them into a struct as the single context and separate them out again in your configured expressions.

Contexts are copied and passed by value. If your context is large, consider making the context type a pointer to it.



## Lifecycle Operations

Lifecycle operations are documentated extensively in the [Lifecycle Style](lifecycle_style.md) guide. These apply both to containers and, when wrapped by the [lifecycle template](../include/pottery/lifecycle/), to the values they contain. Here's a quick summary.

The initialization and de-initialization functions are:

- **`init()`**: Initializes an externally allocated value.
- **`destroy()`**: Deinitializes an externally allocated value.
- **`new()`**: Allocates and initializes a value.
- **`delete()`**: Deinitializes and deallocates a value.

Lifecycle mutation operations are:

- **`copy()`**: Replaces the contents of a value with a copy of the contents of another.
- **`steal()`**: Replaces the contents of a value by stealing those of another, leaving it with junk.
- **`move()`**: Moves an object to another address in memory without performing any (de)initialization. (Other libraries sometimes call this "relocation".)
- **`swap()`**: Swaps the contents of two values.

Copy and steal can be combined with initialization:

- **`init_copy`()**: Initializes an externally allocated value, copying the contents of another value into it.
- **`new_copy`()**: Allocates and initializes a value, copying the contents of another value into it.
- **`init_steal`()**: Initializes an externally allocated value stealing the contents of another into it, leaving the other initialized but containing junk.
- **`new_steal`()**: Allocates and initializes a value stealing the contents of another into it, leaving the other initialized but containing junk.



## Container Operations

- **`count()`**: The number of values in the container.

- **`is_empty()`**: True if the container is empty, false otherwise. This is equivalent to `count() == 0` but it can sometimes be implemented more efficiently.

- **`capacity()`**: The total number of values that can be stored in a container (without re-allocating, if the container dynamically allocates storage.)

- **`reserve()`**: Grow the storage of the container to fit at least the given number of values.

- **`shrink()`**: Shrink the storage of the container to fit the current number of values.



## Container Value Operations

The two fundamental mutation operations for non-intrusive containers are "emplace" and "displace". These manage unitialized storage for values in the container.

- **`emplace()`**: Create uninitialized storage for a value in the container. You must initialize the value yourself before calling any other functions on the container. (Note that this is not the same as the C++ STL operation "emplace", which is instead called "construct" in Pottery.)

- **`displace()`**: Discard uninitialized storage for a value in the container. You must de-initialize (destroy) a value yourself before displacing it, and you cannot call any other functions on the container in between.

The other core operations on non-intrusive containers wrap emplace and displace with appropriate lifecycle operations.

- **`insert()`**: Insert a value passed by value into the non-intrusive container. (Other libraries sometimes call this "push".) This requires that the value can be passed, i.e. moved by value; see the glossary for "pass".

- **`extract()`**: Extract a value, returning (passing) it to the caller by value and removing its storage from the container. (Other libraries sometimes call this "pop" or "remove".) It requires that the value can be passed, i.e. moved by value; see the glossary for "pass".

- **`remove()`**: De-initialize (destroy) a value and remove its storage from the non-intrusive container. This is essentially `lifecycle_destroy()` followed by `displace()`, and it's the usual way to remove values from a container that owns its values. It requires that the container is able to destroy its values.

- **`construct()`**: In the context of a container operation, create uninitialized storage for a value and then run its C++ constructor in-place, passing the given arguments along with perfect forwarding. (In the C++ STL this is called "emplace", which means something different in Pottery.) This operation only exists in C++.

For user-ordered containers, these are typically combined with iteration words, sometimes with a conjunction in between. Some examples:

- `insert_at()` inserts a value at a specific index;
- `emplace_last()` creates uninitialized storage for a value at the end of a container (making it the new last value);
- `displace_and_next()` displaces a value and returns the subsequent entry.

See the iteration and ordering sections below.

Note in particular that Pottery defines both `remove_all()` and `displace_all()` for clearing a non-intrusive container. Other libraries call such an operation "clear", but Pottery has two such operations. The best way to distinguish them is to follow the same naming conventions as everything else: "remove" destroys the values and "displace" does not.

Heaps also provide two additional core mutation operations. A heap operates on a starting range of an underlying generalized array, and these functions alter the range:

- **`expand()`**: Expands the container to contain additional values from the underlying array, moving them to their appropriate place in the container.

- **`contract()`**: Contracts the container to contain less values from the underlying array, moving them to the array immediately after the range of the container.



## Intrusive Container Value Operations

The two fundamental mutation operations for intrusive linked containers are "link" and "unlink". These add or remove externally owned and allocated values to and from the container.

- **`link()`**: Add an externally owned and allocated value to an intrusive container.

- **`unlink()`**: Remove an externally owned and allocated value from an intrusive container, possibly returning a ref to the removed value.

There are other operations specific to linked containers as well:

- **`replace()`**: Replaces one node with another.

- **`find_location()`**: Finds an entry in the container by key. If such an entry does not exist, an opaque location struct is populated which can be passed to `link_location()`.

- **`link_location()`** Links an entry at a location previously populated by `find_location()`. This avoids the double-lookup of calling `find()` followed by `link()`.

In both of the above cases, the values are unchanged, aside from whatever intrusive metadata they store to link them with other values. Intrusive containers never initialize, destroy, allocate or free values. All values are externally owned and externally managed. They must be initialized before insertion into the intrusive container and cannot be destroyed until after they are removed from the intrusive container.



### Ordering

Entries in a container are semantically ordered as per one of the following ordering modes. These are used for access and iteration.

- For **comparison-ordered** containers (like `tree_map`), the values are accessible in the order given by the comparison expression with which the container is configured.

- For **unordered** containers (like [`open_hash_map`](../include/pottery/open_hash_map/) or [`heap`](../include/pottery/heap/)), the values are accessible in an arbitrary order internal to the container. The order can change arbitrarily as the container is mutated.

- For **user-ordered** containers (like [`vector`](../include/pottery/vector/) or [`pager`](../include/pottery/pager/)), the values are accessible in the order inserted and managed by the user of the container. A user-ordered container preserves insertion order.

Note in particular, array containers are not unordered; they are user-ordered.



### Access

These operations or operation modifiers access specific values in a container:

- **`first()`**: The first entry in the container. When used on its own, this returns the first entry. It can also be used as a modifier; for example `insert_first()` adds a value to the start of the container (making it the new first value) and `remove_first()` removes the first value.

- **`last()`**: The last entry in the container. This only exists if it can be efficiently implemented. For example a singly-linked list without an end pointer does not support `last()` because it would have to iterate through all values in the container.

- **`any()`**: Any arbitrary entry in the container. This is usually the one that can be accessed and removed most quickly. This is useful for visiting all values in arbitrary order while incrementally clearing a container, among other things. For example on a [`vector`](../include/pottery/vector/) it returns the last entry; on queues it returns the first entry; on binary trees it returns the root entry; and on tables it does not exist because it cannot be efficiently implemented.

- **`bulk()`**: A range of values in a C array. This is used in the context of a mutation, such as `insert_bulk()`, which inserts all values from a C array into the container.

- **`all()`**: All entries in the container. This is used in e.g. `remove_all()` or `displace_all()` to clear a container.

- **`find()`**: Finds an entry with the given key. This is used for containers that support key-based lookups, for example associative arrays (maps, trees, hash tables, etc.)



### Iteration

The term **iteration** means to access entries in a container sequentially based on their ordering. These are the fundamental iteration operations:

- **`begin()`**: This is either the first entry in the container, or the end entry if the container is empty. (It is the entry from which iteration should begin.)

- **`end()`**: A non-existent entry representing the end of the container. The end entry is semantically one past the last entry in the container; calling `next()` on the last entry returns the end entry, and calling `previous()` on the end entry returns the last entry. There is only one unique end entry in a container; two instances of the end entry compare equal even if they were reached by different means (i.e. `entry_equal(end(), next(last()))` returns true.)

- **`next()`**: The entry following a given entry in the container. The next entry after the last entry is the end entry. The end entry has no next entry; calling `next()` on the end entry is undefined behaviour.

- **`previous()`**: The entry preceding a given entry in the container. The last entry is before the end entry in the container. The first entry has no previous entry; calling `previous()` on the first entry is undefined behaviour.

The following operations deal directly with the index of values. The container must support direct indexing (for example arrays and weight-balanced trees support this, but b-trees and red-black trees do not.)

- **`at()`**: Gets the entry at a given index for a value in the container. The index must be in the range `0` to `count-1`. Calling this on an empty container or with an index out of range is undefined behaviour (and asserts in debug mode.) This is the natural way to access values by index in a container.

- **`index()`**: Gets the index of an entry within the container. This ranges from `0` to `count`, where `count` is the index of the end entry.

- **`select()`**: Gets the container entry at a given index, including potentially the end entry. This ranges from `0` to `count`. This is almost identical to `at()`; the only difference is that it can also return the end entry. Usually you would not call this directly, although you would configure `ARRAY_ACCESS_SELECT` on a template that uses [`array_access`](../include/pottery/array_access/) (for example when [sorting a pager](../examples/pottery/sort_pager/).)

- **`offset()`**: Calculate the signed difference in index between two entries.

- **`shift()`**: Shift an entry forwards or backwards by the given signed offset. This can shift to and from the end entry. The index of the given entry plus the signed offset must be in the range `0` to `count`.

A few additional concepts are used for access and iteration, and for manipulating entries in general:

- **Equal**: Compares two entries for equality. Returns true if the entries refer to the same storage location in a container. This is usually called `entry_equal()`, and is separate from the concept of value equality.

- **Exists**: True if the entry is not the end entry of the container. In other words an entry exists if it refers to the storage location of a value in the container. This is usually called `entry_exists()`.

- **Base**: The reference entry for [`array_access`](../include/pottery/array_access/), used in any algorithm that operates on a generalized array. This is roughly equivalent to the begin entry of the array. They are usually equivalent in practice, but they don't have to be. It essentially functions as another arbitrary context: `array_access` functions can do anything with it including ignoring it. If a `BEGIN` expression is defined, `array_access` functions and configured expressions no longer take a base since it's assumed that begin can take its place (it is therefore said to have an "inherent base".)

In addition, the `entry_ref()` function returns a ref to the value stored in an entry.



### Miscellaneous

- **Pass**: To pass a value as a function parameter or return it from a function by value. A value can only be passed if a `VALUE_TYPE` exists and if it can be moved by value (either `LIFECYCLE_BY_VALUE` or `LIFECYCLE_MOVE_BY_VALUE` must be 1.) This enables the "insert" and "extract" operations on containers. (A C++ type can be passed even if it has a non-trivial move constructor as long as it does not throw. Pottery will properly call all constructors, destructors and operators for C++ value types declared `BY_VALUE`.)
