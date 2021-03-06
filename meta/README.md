# Meta-templates

This folder contains "meta-templates", i.e. templates that are used to generate parts of Pottery's templates.

Many of Pottery's `macros` and `unmacros` headers contain sections that say `AUTOGENERATED`. These are generated from the meta-templates here. The generator replaces these sections in-place in the headers.

Meta-templates are used only for renaming identifiers. They emit only preprocessor defines for this purpose; they do not actually generate any non-preprocessor code.

Functions and types are renamed by concatenating the user prefix with the suffix of the identifier. For example if the user defines `POTTERY_VECTOR_PREFIX` to foo, `pottery_vector_init` is renamed to `foo_init`. Since these macros are the same across many container templates, they are generated from [`container/rename.m.h`](container/rename.m.h).

Macros are renamed by meta-templates as well. This is called "forwarding". For example when you instantiate a [vector](../include/pottery/vector/), you configure lifecycle expressions on the vector template, like `POTTERY_VECTOR_LIFECYCLE_DESTROY`. But vector uses the [lifecycle](../include/pottery/lifecycle/) template to actually implement this, so it needs to rename (or forward along) this configuration to `POTTERY_LIFECYCLE_DESTROY`. Since these macros are the same across many containers, they are generated from [`lifecycle/forward.m.h`](lifecycle/forward.m.h).

## Generating the code

The unit test suite and fuzz testing scripts regenerate all headers automatically. You can just run them as normal while making changes to Pottery. If the meta-template generator script notices any changes during CI builds, it will fail. This ensures that all meta-templates are correctly generated in any commit that passed CI.

If you want to run the generator manually (e.g. when editing one of the [examples](../examples/)), you can run it from the command line:

```sh
meta/generate.py
```

## Clean

If you want to see the Pottery code without meta-templates, you can run the generator in clean mode:

```sh
meta/generate.py -c
```

This sweeps out all autogenerated blocks leaving them empty. This can make it easier to understand the `macros` files since the only things left over are those unique to the template.

Of course Pottery doesn't compile this way; to fix it just run the generator normally again.

## Committing the code

The partially generated headers are committed to the repository. This is unconventional of course, as generated code should usually never be committed. But there are good reasons to do this for Pottery:

- The `macros` and `unmacros` headers are only partially generated; some parts are necessarily hand-written. I've tried splitting the generated portions into their own headers and there ends up being just way too many header files. There are already too many headers per template algorithm/container as it is. This is as simple as I can make it (short of abandoning generation and writing the macros by hand.)

- Having the generated code in the repository means the repository can be used as-is without needing to perform a code generation step. This way we can write the generator in any language and not require that users of the library be able to run it. You only need to run the generator if you change the meta-templates or change some template's use of them.

This makes Pottery trivially usable as a header-only library. You can just take a tarball from any commit of Pottery, dump it onto your include path and start using it. See the [integration documentation](../docs/integration.md) to see how easy this is in practice.
