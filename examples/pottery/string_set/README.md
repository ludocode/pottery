This demonstrates how to implement a set of unique strings using Pottery.

The `string_set` container provides functions to add a string, remove a string, and query whether a string exists in the set.

`string_set` hides its implementation details, demonstrating how you can use Pottery within a component or a shared library without exposing Pottery in your API. Internally, it uses a Pottery [`open_hash_map`](../../../include/pottery/open_hash_map/) with the [FNV-1a](https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function#FNV-1a_hash) hash function to store its strings. The hash map contains allocated copies of strings and frees them on removal or when it's destroyed.

This is a minimal example. You could extend it for example to iterate over the strings in the set, to copy or clear the set, to use a better hash function or a different map, etc.

A walkthrough of this example is in the main [README](../../../#string-set).
