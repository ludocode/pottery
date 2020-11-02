# Open Hash Table

A hash table that uses open addressing on an external generalized array.

The `open_hash_table` template is composed of a set of algorithms that operate on an external power-of-two-sized array of buckets (similar to the [`heap`](../heap/) template for example.) It does not require the storage of any persistent state other than the array itself. All functions take an `open_hash_table_t` struct as a parameter, but this struct need not be persistent; you can generate it as an argument to each function call.

If you want an open hash table that automatically allocates its backing and grows or re-hashes as necessary, see [`open_hash_map`](../open_hash_map/).

The hash table supports a configurable probing sequence. The default is linear probing. Quadratic probing and double hashing are also available.

### Bucket States

Each bucket in the hash table must support an empty state to denote empty buckets and may support a tombstone state to denote deleted buckets.

You can configure these states however you like. You can use in-band values like null or -1, or you can use separate flags or a state enum. You configure the hash table with query and set expressions for each state so you can store these however you like.

Buckets in the empty or tombstone state are considered uninitialized. When initializing a bucket with an element or moving an element into a bucket, you must clear the empty and tombstones states yourself. The table is not capable of clearing the empty or tombstone state of a bucket.

### Tombstones

The tombstone state is optional for linear probing but required for other probing sequences.

If a tombstone state is not configured, a move expression is required. The hash table will move colliding elements around when an element is removed from the hash table.

If a tombstone state is configured, the hashtable will use tombstones where necessary to replace deleted elements, and elements in the hash table are never moved (so they are "pointer stable".) Tombstones slow down all hash table operations. A hash table may eventually fill up with tombstones, so you may want to have a mechanism for rehashing. This is not necessary without tombstones.
