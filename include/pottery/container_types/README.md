# Container Types

The container_types template is used to instantiate the various types used in Pottery containers and algorithms along with conversions and comparisons between them.

This defines the folowing types if applicable:

- `value_t`: A concrete value type, if values exist as C types in memory
- `ref_t`: An abstract reference to a value (a pointer to `value_t` if it exists)
- `entry_t`: An abstract reference to a storage location in a container
- `key_t`: A portion of a value used for comparisons in associative arrays (maps, tables, search trees)
- `context_t`: A user context for configuration expressions

It also defines the following conversions and comparisons:

- `entry_ref()`: Gets the ref for an entry
- `ref_key()`: Gets the key for a ref
- `ref_equal()`: Returns true if two refs are equal

Note that comparing entries is the responsibility of the [array_access](../array_access/) template and comparing keys is either the responsibility of the hash table using it or of the [compare](../compare/) template. This template only supports comparing refs for equality.

These types are also summarized in the [glossary](../../../docs/glossary.md).

Unlike most child templates, `container_types` is not usually instantiated by its parent with a suffix. It is instead instantiated directly into the parent template's namespace.



## Configuration Options

### Types

Note in the below configuration, `REF_TYPE` and `VALUE_TYPE` are mutually exclusive. For the three types value, entry and ref, you must define one of the following combinations:

- Only `VALUE_TYPE` for primitives, structs and other value types;

- Only `REF_TYPE` for unowned pointers, abstract references like UUIDs or strings, etc.;

- Both `ENTRY_TYPE` and `VALUE_TYPE` for non-contiguous value containers, like some Pottery containers;

- Both `ENTRY_TYPE` and `REF_TYPE` for more abstract containers (like a container of UUIDs to values stored in a database.)

#### `VALUE_TYPE`

A concrete value type for comparing objects in memory.

This is optional. If this is configured, it is typedef'd to `value_t`, and `ref_t` is defined as a pointer to this. If this is not configured `REF_TYPE` must be configured instead.

#### `REF_TYPE`

An abstract reference for the type to be compared.

`ref_t` is defined as a typedef to this, or to `value_t*` if this is not configured.

#### `ENTRY_TYPE`

An abstract reference type for the value to be compared, usually with the storage location of the value within its container. Two values of this type are passed to all configured comparison expressions (after the optional context.)

`entry_t` is defined as a typedef to this, or to `ref_t` if this is not configured.

#### `CONTEXT_TYPE`

An optional comparison context type. If configured, the context type is passed as the first argument to all configured comparison expressions and accepted as the first argument of all generated functions.

If this is configured, it is typedef'd to `context_t`.

### Conversions

#### `REF_EQUAL`

An expression matching `bool(context_t context, ref_t left, ref_t right)`.

Returns true if the two refs refer to the same value.

Note that this must compare the refs themselves, not their values! Refs that point to different values must compare false even if those values are equal.

#### `ENTRY_REF`

An expression matching `ref_t(context_t context, entry_t entry)`.

Returns a ref to the value contained in an entry. In other words it converts an entry to a ref.

This is optional. If not configured, the entry type must be implicitly convertible to the ref type. This is often the case since they are usually the same type.

If you are configuring a custom `ENTRY_TYPE`, you will almost certainly want to configure this as well (though technically you may not need to, for example if your ref type is implicitly convertible from entry by a non-explicit C++ constructor that takes the entry type as argument.)
