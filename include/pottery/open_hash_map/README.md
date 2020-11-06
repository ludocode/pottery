# Open Hash Map

A dynamically growable hash table that uses open addressing.

The open hash map allocates an array of buckets in a single contiguous allocation, where each bucket stores exactly one value. Hash collisions are resolved by storing colliding elements in alternate buckets based on a configurable probing sequence.

Documentation still needs to be written. In the meantime check out the examples, e.g. [String Set](../../../examples/pottery/string_set/).
