#include <stdio.h>
#include <stdint.h>

typedef struct {
    const char* key;
    int some_other_field;
} foo_t;

// FNV-1a hash function for strings
static inline size_t fnv1a(const char* p) {
    uint32_t hash = 2166136261;
    for (; *p != 0; ++p)
        hash = (hash ^ (uint8_t)*p) * 16777619;
    return hash;
}

// instantiate the hash table algorithm
#define POTTERY_OPEN_HASH_TABLE_PREFIX foo_table
#define POTTERY_OPEN_HASH_TABLE_VALUE_TYPE foo_t
#define POTTERY_OPEN_HASH_TABLE_KEY_TYPE const char*
#define POTTERY_OPEN_HASH_TABLE_KEY_FOR_VALUE(foo) foo->key
#define POTTERY_OPEN_HASH_TABLE_KEY_HASH fnv1a
#define POTTERY_OPEN_HASH_TABLE_LIFECYCLE_BY_VALUE 1
#define POTTERY_OPEN_HASH_TABLE_IS_EMPTY(x) x->key == NULL
#define POTTERY_OPEN_HASH_TABLE_SET_EMPTY(x) x->key = NULL
#define POTTERY_OPEN_HASH_TABLE_LINEAR_PROBING 1
#define POTTERY_OPEN_HASH_TABLE_KEY_EQUAL 0 == strcmp
#include "pottery/open_hash_table/pottery_open_hash_table_static.t.h"

// our hash table
#define FOO_TABLE_SIZE_BITS 10
static foo_t foo_table_array[1 << FOO_TABLE_SIZE_BITS];

int main(void) {
    foo_table_t table;
    foo_table_configure(&table, foo_table_array, FOO_TABLE_SIZE_BITS, 0);

    // TODO add insert by pass

    foo_t first = {"first", 1};
    *foo_table_emplace(&table, first.key, pottery_null) = first;

    foo_t second = {"second", 2};
    *foo_table_emplace(&table, second.key, pottery_null) = second;

    foo_t third = {"third", 3};
    *foo_table_emplace(&table, third.key, pottery_null) = third;

    foo_t* foo = foo_table_find(&table, "second");
    foo_table_displace(&table, foo);

    size_t i;
    for (i = 0; i < pottery_array_count(foo_table_array); ++i) {
        if (foo_table_array[i].key != NULL)
            printf("%zi/%i %s\n",i,1<<FOO_TABLE_SIZE_BITS, foo_table_array[i].key);
    }

    return EXIT_SUCCESS;
}
