# POSIX `hsearch()` with Pottery

This is an implementation of the mostly terrible POSIX `hcreate()`. `hsearch()` and `hdestroy()` APIs and their various extensions using Pottery.

- [Linux `search(0P)` man page](https://man7.org/linux/man-pages/man0/search.h.0p.html)
- [Linux `hsearch()` man page](https://linux.die.net/man/3/hsearch)
- [GNU `hsearch()` documentation](https://www.gnu.org/software/libc/manual/html_node/Hash-Search-Function.html)
- [FreeBSD `hsearch()` man page](https://www.unix.com/man-page/freebsd/3/hsearch_r/)
- [NetBSD `hsearch()` man page](https://man.netbsd.org/hcreate.3)
- [OpenBSD `hsearch()` man page](https://man.openbsd.org/hcreate.3)

This is very nearly a drop-in replacement. There are some incompatibilities between systems that make it impossible to provide one implementation to match the behaviour of all systems. In particular:

- GNU glibc requires that the hash table be zeroed before calling `hcreate_r()` so it can detect a double-create and return an error. musl, FreeBSD and NetBSD do not have this requirement and so cannot detect double-create. This implementation follows the musl/BSD behaviour.

- OpenBSD frees all keys with `free()` on a call to `hdestroy()`. No other implementation seems to do this (at least not a modern one.) This implementation provides `pottery_hdestroy_openbsd()` with the OpenBSD behaviour.

- IBM z/OS uses an internal mutex to synchronize all calls to `hsearch()`, and QNX also classifies it as thread safe. This implementation does not, nor does glibc, musl, FreeBSD or NetBSD. (You can trivially write an `hsearch()` wrapper that locks a mutex. Mutexes are platform-specific so we don't bother here.)

Hopefully having yet another free permissive implementation of this will encourage future POSIX standards to deprecate and remove it. Implementations that need to preserve backwards compatibility forever can use something like this to easily provide it (though you'll need to fix it up for this, e.g. hiding structure contents and not using inline functions.)

Use this to reduce your code's dependence on POSIX or GNU, to port your code to other platforms with incompatible `hsearch()`, or to implement a standard library that provides backward compatibility for ancient POSIX quirks. Avoid this in new code. By default this prefixes identifiers with `pottery_` or `POTTERY_` but you can configure it to define the real identifiers to it for old code.

In general this shows how easy it is to use Pottery to provide fully-featured alternate implementations of traditional containers. We don't need to rely on our operating systems or squabble over their implementation differences anymore. We can just ignore what the platforms provide and use our own.

This also demonstrates how you might use your own `.t.h` file to avoid repeating the configuration of a template when using separate declare/define instantiations.
