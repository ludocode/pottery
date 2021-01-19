This demonstrates how to build a map with Pottery. In this example we're building a map of string names to person pointers, i.e. `const char*` to `person_t*`.

The template instantiation gives us a type called `person_map_t`. You can initialize it with `person_map_init()`; add to it with `person_map_insert()` and friends; access the contents by `person_map_find()`; and so forth.

We use `const char*` as the key, not `char*`, that way we can search it with const strings. This means we can pass C string literals to `find()`, `emplace_key()`, etc. We provide an ordering comparison by `strcmp()` so the map will keep our values sorted.

We've given the map a `DESTROY` expression so the map can destroy values for us. This means we can call `remove()` to remove an entry or `remove_key()` to remove a value by key. The map will call `person_delete()` on them automatically. We can also destroy the whole map even if it's not empty; the map will call `person_delete()` on all remaining values.

(A `DESTROY` expression is optional. Without it we would not be able to call `remove()` or destroy a non-empty map, but we could still call `displace()` or `extract()` so we could destroy values and empty the map ourselves.)

In this example, we're storing our persons as pointers within the map. You could instead store structs by value within the map, but if they aren't bitwise-movable you won't be able to use `insert()` or `extract()`. See the [lifecycle_style](../lifecycle_style/) example for a map that contains non-bitwise-movable structs by value.
