# How It Works

Most of the Pottery source code is in _template headers_. They have the extension `.t.h` and do not have include guards. A Pottery template is configured by macros and then `#include`d to instantiate it. The macros within a template mostly just forward configuration to child templates and rename all of its identifiers to make it unique.

This document is a work in progress. It needs to be expanded.



## Basic Headers

To understand the basics, follow along with the source code to Pottery's [`compare` template](../include/pottery/compare/) since it's probably Pottery's simplest template. It's mostly intended as a helper for other templates, but you can instantiate and use it directly, like this:

```c
#define POTTERY_COMPARE_PREFIX string
#define POTTERY_COMPARE_REF_TYPE const char*
#define POTTERY_COMPARE_THREE_WAY strcmp
#include "pottery/compare/pottery_compare_static.t.h"
```

A template instantiation here is performed by [`pottery_compare_static.t.h`](../include/pottery/compare/pottery_compare_static.t.h). This performs three steps, separated into different headers:

- Configuring and renaming identifiers. This is [`impl/pottery_compare_macros.t.h`](../include/pottery/compare/impl/pottery_compare_macros.t.h). It checks the user's configuration, then performs a set of simple `#define`s to rename all of the template's types and functions.

- Instantiating the code. The code is in [`impl/pottery_compare_declarations.t.h`](../include/pottery/compare/impl/pottery_compare_declarations.t.h). The compare template mostly just wraps the user's configuration so it's not very interesting. However you can see the `min()`, `max()`, `clamp()`  and `median()` functions at the bottom look somewhat like idiomatic C.

- Cleaning up. This is in [`impl/pottery_compare_unmacros.t.h`](../include/pottery/compare/impl/pottery_compare_unmacros.t.h). This undefines all macros used to instantiate the template, including the renaming macros and user's configuration macros. The template can be instantiated again with a different configuration.


## Separate Header and Source Files

The compare template is simple because all of its functions are `static inline`. Larger Pottery functions are not inline. Instead, Pottery templates with large functions have a separate `definitions` header.

Take a look at the [`alloc` template](../include/pottery/alloc/) template for an example. This has two separate implementation headers: [`impl/pottery_alloc_declarations.t.h`](../include/pottery/alloc/impl/pottery_alloc_declarations.t.h) and [`impl/pottery_alloc_definitions.t.h`](../include/pottery/alloc/impl/pottery_alloc_definitions.t.h). These two files correspond more or less to the header and source files of normal C code. They are separate files so that the declarations can be instantiated into a normal C header file with external linkage, and the definitions can be instantiated into a single translation unit.



## Template Composition

Pottery's templates are composable. For example the `intro_sort` template internally instantiates all of the following templates, some of which instantiate their own templates and some of which depend on others:

- [`intro_sort`](../include/pottery/intro_sort/)
    - [`quick_sort`](../include/pottery/quick_sort/)
    - [`insertion_sort`](../include/pottery/insertion_sort/)
    - [`heap_sort`](../include/pottery/heap_sort/)
        - [`heap`](../include/pottery/heap/)
    - [`lifecycle`](../include/pottery/lifecycle/)
    - [`compare`](../include/pottery/compare/)

Configuration of child templates is done by `config` headers. For example `intro_sort` configures its child `quick_sort` in [`pottery_intro_sort_config_quick_sort.t.h`](../include/pottery/intro_sort/impl/pottery_intro_sort_config_quick_sort.t.h). Template composition is accomplished by forwarding along whatever configuration is necessary, instantiating dependent templates, and then renaming the functions they would use in those templates. Some of the forwarding and un-forwarding code is generated using [meta-templates](../meta/).

In many cases, a parent template needs to declare some identifiers before instantiating its child template. This is accomplished using a `forward` implementation header. For example, [`shell_sort`](../include/pottery/shell_sort/) must forward-declare its state in order to use it as a context for its child [`insertion_sort`](../include/pottery/insertion_sort/). This happens in [`impl/pottery_shell_sort_forward.t.h`](../include/pottery/shell_sort/impl/pottery_shell_sort_forward.t.h) before `insertion_sort` is instantiated. You can see the include order in [`pottery_shell_static.t.h`](../include/pottery/shell_sort/pottery_shell_sort_static.t.h)

Some containers can depend on an external instantiation of its child templates. For example, with `intro_sort` above, all of the various sort algorithms can share a single instantiation of `lifecycle`. Rather than letting each one instantiate its own copy, `intro_sort` instantiates it, then configures it as an external template.



## Exported Configuration

Some templates export macros to allow parent templates to detect their capabilities after they are instantiated. For example the lifecycle template can instantiate a `swap()` function if given a `SWAP` expression. But it can also instantiate a `swap()` function if given a `MOVE` expression and a `VALUE_TYPE`.

In order to allow the parent to detect whether swap is possible, the lifecycle template exports the macro `POTTERY_LIFECYCLE_CAN_SWAP`. This is defined to either 1 or 0, corresponding to whether or not `swap()` exists.

When these identifiers are no longer needed, they must be cleaned up. This is done by including the corresponding `cleanup` header. For example the lifecycle template provides [`pottery_lifecycle_cleanup.t.h`](../include/pottery/lifecycle/pottery_lifecycle_cleanup.t.h) to undefine these macros. All templates that instantiate `lifecycle` perform its cleanup after they are done.

See for example [`pottery_insertion_sort_static.t.h`](../include/pottery/insertion_sort/pottery_insertion_sort_static.t.h). The `insertion_sort` template instantiates lifecycle functions, instantiates its own functions based on the capabilities of the lifecycle template, then includes the lifecycle cleanup header to clean up. In [`pottery_insertion_sort_definitions.t.h`](../include/pottery/insertion_sort/pottery_insertion_sort_definitions.t.h), you can see where it checks `POTTERY_LIFECYCLE_CAN_MOVE` to decide whether to move or swap.

`POTTERY_LIFECYCLE_CAN_DESTROY` is another macro that is checked often. You can see this macro throughout the implementation of [`vector`](../include/pottery/vector/) for example. This changes its behaviour and capabilities based on whether its contained values can be destroyed.



## Contexts

Most templates support a context to be passed to configured expressions. For dynamic containers, the container stores the context. For algorithms, the context is passed in as an argument to every function.

Since the context is optional, every function involving a context has an optional argument. To simplify this, templates typically define macros:

- `CONTEXT_ARG` evaluates to a context function parameter within a function prototype if a context is defined, such as "`pottery_context_t context,`" (with the comma.) It evaluates to nothing otherwise.

- `CONTEXT_VAL` evaluates to a context value to be passed as a function parameter if a context is defined, such as "`context,`" (with the comma.) It evaluates to nothing otherwise.
