# Array Map

A map that stores its values in a growable array. A linear or binary search is used to find a value by key.

This makes it ideal for maps that store only with a small handful of values. It can be much faster than hash tables or binary search trees as long as the total number of values is small, and it has a smaller compiled code size and memory usage than other maps. Use this when you know that the number of unique keys is limited so you don't need performance that scales to huge numbers of values.

By default the array map instantiates a [`vector`](../vector/) to store its values. This means values are stored contiguously in memory, and growing the map allocates new storage and moves all values. You can instead configure the array map to use an arbitrary growable array by defining `POTTERY_LINEAR_MAP_EXTERNAL_ARRAY`.

The map supports two ordering modes: comparison-ordered or unordered.

- The array map is **comparison-ordered** if it is configured with an ordering `COMPARE` expression, e.g. if one of `COMPARE_LESS`, `COMPARE_THREE_WAY`, etc. are defined. In this mode the values are stored in sorted order. Finding values is O(logn) by binary search. Insertion and removal is O(n) for a vector, or whatever the arbitrary insertion and removal complexity is of the backing array.

- The array map is **unordered** if it is configured without an ordering expression, e.g. if only `COMPARE_EQUAL` or `COMPARE_NOT_EQUAL` are defined. In this mode the values are sorted in arbitrary order. Finding values is done by an O(n) linear search. Insertion is done by append, and removal is done by swapping the last value into the removed value's place and truncating; these are amortized O(1) for a vector, or whatever the append and truncate insertion complexity is of the backing array.

See the [glossary](../../../docs/glossary.md#ordering) for more information on these modes.

## Examples

Here's an array map of values of a struct type `foo_t` with string keys:

```c
typedef struct foo_t {
    char* key;
    // other stuff
} foo_t;

#define POTTERY_ARRAY_MAP_PREFIX foo_map
#define POTTERY_ARRAY_MAP_VALUE_TYPE foo_t
#define POTTERY_ARRAY_MAP_KEY_TYPE const char*
#define POTTERY_ARRAY_MAP_REF_KEY(foo) foo->key
#define POTTERY_ARRAY_MAP_LIFECYCLE_MOVE_BY_VALUE 1
#define POTTERY_ARRAY_MAP_COMPARE_THREE_WAY strcmp
#include "pottery/array_map/pottery_array_map_declare.t.h"
```

Note that this uses `const char*` as the key type rather than `char*` so you can search it with const strings.
