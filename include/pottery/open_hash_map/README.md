# Open Hash Map

A dynamically growable hash table that uses open addressing.

The open hash map allocates an array of buckets in a single contiguous allocation, where each bucket stores exactly one value. Hash collisions are resolved by storing colliding elements in alternate buckets based on a configurable probing sequence.

Like all Pottery associative containers, keys are contained within values rather than stored separately by the map. You must be careful never to change the key for a value while it is stored in an associative container, otherwise it will corrupt the container.

Documentation still needs to be written. In the meantime check out the examples, e.g. [String Set](../../../examples/pottery/string_set/).
