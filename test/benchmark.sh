#!/bin/sh

# Builds and runs Pottery benchmarks.

set -e
cd "$(dirname $0)/.."

# Generate meta-templates
meta/generate.py

# Fetch comparison libraries
fetch_lib() {
    if ! [ -e $1 ]; then
        echo "Fetching $1 from $2"
        (rm -rf $1 && mkdir $1 && cd $1 && curl -Lo- "$2" | tar -xzf- --strip-components=1)
    fi
}
LIB=test/build/lib
mkdir -p $LIB
cd $LIB
fetch_lib stb https://github.com/nothings/stb/archive/b42009b3b9d4ca35bc703f5310eedc74f584be58.tar.gz
fetch_lib pqsort https://github.com/Freaky/pqsort/archive/aba8f65af1839554ac8ed4fe2cb5a5a685e3564a.tar.gz
fetch_lib swenson_sort https://github.com/swenson/sort/archive/749d33e4aea4cd5508edc5a13b8501dc1bc29bfa.tar.gz
fetch_lib musl https://musl.libc.org/releases/musl-1.2.1.tar.gz
#fetch_lib justinow_introsort_c https://github.com/justinow/introsort-c/archive/eee26ae2e95f959debe821f030159ac49851b3d6.tar.gz
cd ../../..
echo

if ! [ -e /usr/include/boost/sort/sort.hpp ] && ! [ -e /usr/local/include/boost/sort/sort.hpp ]; then
    echo
    echo "WARNING: Install Boost to include it in benchmarks."
    echo
fi

if ! [ -e $LIB/musl/src/stdlib/atomic.h ]; then
    # Put an empty atomic.h file in musl stdlib to work around
    # qsort.c's #include "atomic.h"
    touch $LIB/musl/src/stdlib/atomic.h
fi

# Build benchmarks
make -j8 -f test/tools/benchmark-makefile.mk

# Run them
test/build/benchmark/runner
