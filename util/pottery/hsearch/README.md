# POSIX `hsearch()` with Pottery

This is an implementation of the mostly terrible [POSIX hash table](https://pubs.opengroup.org/onlinepubs/9699919799/functions/hcreate.html) API and its various extensions using a Pottery [`open_hash_map`](../../../include/pottery/open_hash_map/).

## Compatibility Details

This is very nearly a drop-in replacement for real-world implementations of POSIX `hsearch()`. There are some incompatibilities between systems that make it impossible to provide one implementation to match the behaviour of all of them. In particular:

- GNU glibc and uClibc require that the hash table be zeroed by the user before calling `hcreate_r()` so they can detect a double-create and return an error. musl, FreeBSD and NetBSD do not have this requirement and so cannot detect double-create. This implementation follows the musl/BSD behaviour.

- OpenBSD frees all keys with `free()` on a call to `hdestroy()`. No other implementation seems to do this (at least not a modern one.) This implementation provides `hdestroy_freekeys()` with the OpenBSD behaviour.

- Implementations differ on whether the table will grow if more entries are inserted than the number given to `hcreate()`. NetBSD is explicit that the table will grow; GNU is explicit that it will not. Others are more vague. This implementation ignores the number given to `hcreate()` and grows as needed.

- IBM z/OS uses an internal mutex to synchronize all calls to `hsearch()` and QNX also classifies it as thread safe. This implementation does not, nor do any of the Linux libc or the BSDs. If you need this you can trivially write an `hsearch()` wrapper that locks a mutex. Mutexes are platform-specific so we don't bother here.

Hopefully having yet another free permissive implementation of this will encourage future POSIX standards to deprecate and remove it. Implementations that need to preserve backwards compatibility forever can use something like this to easily provide it (though you'll need to fix it up for this, e.g. hiding structure contents and not using inline functions.)

## Design Flaws

The POSIX hash table API has many flaws in addition to the incompatibilities mentioned above. Here's a short list:

- There is only one global hash table for the whole process! The `hcreate_r()` extension allows independent tables but it's non-standard.

- There is no way to remove a key from a hash table. No known implementation provides an extension to do it. If you want to truly remove a key you must destroy and rebuild the table.

- There is no requirement for a means to grow the table. On some platforms it's possible to run out of space. If you want to truly grow you must destroy and rebuild the table.

- There is no way to iterate through the items in the table. No known implementation provides an extension to do this. If you want to walk the table you have to keep a list of keys yourself.

- There is no standard way to free keys or data stored in the table. Destroying the table leaks all contents; you must keep track of keys and data externally and free them yourself. Only OpenBSD frees keys on destroy, and only NetBSD (and Pottery) have extensions to call callbacks to free keys and data.

- Keys must be strings. You cannot provide custom types or void pointers as keys. There is no way to specify a custom hash function. The quality of the hash function is unspecified.

- When using `ENTER`, you may not want to allocate the key unless it is necessary to insert a new entry. Since the given key is inserted directly, it's not necessarily safe to use a stack-allocated buffer for lookups. It's awkward to replace the key with an allocation after insertion and it's unclear whether it's allowed.

- There are no higher-level operations on tables, such as copying tables, merging tables, etc. The only operations are find and insert, both implemented through the `hsearch()` function.

Use this to reduce your code's dependence on POSIX or GNU/BSD extensions, to port your code to other platforms with incompatible `hsearch()`, or to implement a standard library that provides backward compatibility for ancient POSIX quirks. Avoid this in new code. You are much better off using Pottery's [`open_hash_map`](../../../include/pottery/open_hash_map/) directly.

## API Reference

Note that all types and functions will have a `POTTERY_` or `pottery_` prefix unless `POTTERY_HSEARCH_REAL_API` is defined.

On success, functions returning `int` return non-zero and functions returning `ENTRY*` return a non-null entry. On error, the error code is placed in `errno`; functions returning `int` return zero and functions returning `ENTRY*` return `NULL`. Functions returning `void` cannot fail.

### Types

```c
typedef enum {
    FIND,
    ENTER,
} ACTION;

typedef struct {
    char *key;
    void *data;
} ENTRY;

struct hsearch_data {
    // opaque
};
```

### Global table

- `int hcreate(size_t number_of_elements);`

Creates the global table.

- `ENTRY* hsearch(ENTRY item, ACTION action);`

Searches the global table for an entry by key, returning it if found. Otherwise, if the given action is `ENTER`, the given entry is inserted and returned. Otherwise `NULL` is returned.

- `void hdestroy(void);`

Destroys the global table.

- `void hdestroy1(void (*free_key)(void*), void (*free_data)(void*));`

Destroys the global table, calling the given callbacks on each key and data in the table in arbitrary order. The given callbacks may be `NULL`. Use this to free keys or data when destroying the table.

- `void hdestroy_freekeys(void);`

Destroys the global table, calling `free()` on each key in the table.

### Custom tables

- `int hcreate_r(size_t number_of_elements, struct hsearch_data* table);`

Creates the given table. The table does not need to be zeroed before calling this.

- `int hsearch_r(ENTRY item, ACTION action, ENTRY** out_entry, struct hsearch_data* table);`

Searches the given table for an entry by key, placing it in `out_entry` if found. Otherwise, if the given action is `ENTER`, the given entry is inserted and placed in `out_entry`. Otherwise `NULL` is placed in `out_entry`.

- `void hdestroy_r(struct hsearch_data* table);`

Destroys the given table.

- `void hdestroy1_r(struct hsearch_data *htab, void (*free_key)(void*), void (*free_data)(void*));`

Destroys the given table, calling the given callbacks on each key and data in the table in arbitrary order. The given callbacks may be `NULL`. Use this to free keys or data when destroying the table.

## External Documentation

- [POSIX `hsearch()` specification](https://pubs.opengroup.org/onlinepubs/9699919799/)
- [Linux `search(0P)` man page](https://man7.org/linux/man-pages/man0/search.h.0p.html)
- [Linux `hsearch()` man page](https://linux.die.net/man/3/hsearch)
- [GNU glibc `hsearch()` documentation](https://www.gnu.org/software/libc/manual/html_node/Hash-Search-Function.html)
- [FreeBSD `hsearch_r()` man page](https://www.unix.com/man-page/freebsd/3/hsearch_r/)
- [NetBSD `hcreate()` man page](https://man.netbsd.org/hcreate.3)
- [OpenBSD `hcreate()` man page](https://man.openbsd.org/hcreate.3)
- [Solaris `hsearch(3C)` man page](https://docs.oracle.com/cd/E88353_01/html/E37843/hsearch-3c.html)
