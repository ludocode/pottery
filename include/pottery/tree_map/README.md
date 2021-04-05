# Tree Map

A dynamically growable map implemented as a self-balancing binary search tree with individually allocated nodes. (This is similar to `std::map` in C++ or `TreeMap` in Java.)

By default the `tree_map` uses a [`red_black_tree`](../red_black_tree/). (Other binary tree types are planned to be supported some time in the future.)

Like all Pottery associative containers, keys are contained within values rather than stored separately by the map. You must be careful never to change the key for a value while it is stored in an associative container, otherwise it will corrupt the container.

Documentation still needs to be written. In the meantime check out the examples, e.g. [Person Map](../../../examples/pottery/person_map/). The map API is the same as any other map; see the [glossary](../../../docs/glossary.md) for naming conventions.
