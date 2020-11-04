The `platform/` include folder is not a template. It contains portability headers to support various operating systems and compilers.

Don't include any of these headers directly. They are included automatically by templates (unless you're using Pottery templates inside C++ classes, namespaces or extern "C" blocks, in which case you should include [`pottery/pottery_dependencies.h`](../pottery_dependencies.h) at file scope.)
