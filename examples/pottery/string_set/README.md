This demonstrates how to implement a set of unique strings using Pottery.

The `string_set` container provides functions to add a string, remove a string, and query whether a string exists in the set.

This example uses an [`open_hash_map`](include/pottery/open_hash_map/) with [FNV-1a, a simple hash function](https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function#FNV-1a_hash). We've declared that the map should hash keys with `fnv1a()`, compare keys with `strcmp()`, move values by simple assignment, and destroy values with `free()`. The map will manage our string memory for us.

We've also declared `EMPTY_IS_ZERO`. This means that zero (null) is a sentinel value that can be used to mark an empty bucket, making the map more efficient. We could instead (or also) have provided custom `IS_EMPTY` and `SET_EMPTY` expressions to provide some other means of marking an empty bucket (for example if the map value was a struct, we could use a field in the struct.) We could also declare nothing about empty buckets, in which case the map will allocate its own metadata bits to store which buckets are empty.

The map uses linear probing by default. We could instead have declared `QUADRATIC_PROBING` or `DOUBLE_HASHING` to change the probing strategy. Alternate probing algorithms require the use of tombstones: we could declare `IS_TOMBSTONE` and `SET_TOMBSTONE` to store those in-band like our empty state, or just let the map store this metadata on its own. Pottery's hash tables can be extensively configured to get the exact behaviour you want.

`string_set` hides its implementation details, demonstrating how you can use Pottery within a component or a shared library without exposing Pottery in your API. The hash map contains allocated copies of strings and frees them on removal or when it's destroyed.

With this example you can start to see why this library is called "Pottery". Pottery doesn't just give you a generic set in the way you might type `Set<String>` in other languages. Instead you use Pottery to build your own containers, exercising a level of control over algorithms and memory layout that you can only dream of in other programming languages.

This is a minimal example. You could extend it for example to iterate over the strings in the set, to copy or clear the set, to use a better hash function or a different map, etc.
