#!/bin/sh

# Builds and runs a Pottery fuzz test under american fuzzy lop.
# Pass the name of the fuzz test you'd like or pass nothing to get a list.

set -e
cd "$(dirname $0)/.."

# Generate meta-templates
meta/generate.py

# Build given fuzz tester (or print usage if no $1)
make -j8 -f test/tools/afl-makefile.mk $1

if ! [ "x$1" =  "x" ]; then

    # Generate AFL test cases
    test/tools/generate-corpus.sh

    # ugly output directory, but I haven't bothered trimming the fuzz
    # test names yet so oh well
    OUT=test/build/afl/output/$1
    mkdir -p $OUT

    # Run given fuzz tester
    afl-fuzz -i test/build/corpus -o $OUT -m 2500 $1
fi
