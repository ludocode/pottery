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

fetch_file() {
    if ! [ -e $1 ]; then
        echo "Fetching $1 from $2"
        curl -Lo "$1" "$2"
    fi
}

LIB=test/build/lib
mkdir -p $LIB
cd $LIB
fetch_lib stb https://github.com/nothings/stb/archive/b42009b3b9d4ca35bc703f5310eedc74f584be58.tar.gz
fetch_lib pqsort https://github.com/Freaky/pqsort/archive/aba8f65af1839554ac8ed4fe2cb5a5a685e3564a.tar.gz
fetch_lib swenson_sort https://github.com/swenson/sort/archive/749d33e4aea4cd5508edc5a13b8501dc1bc29bfa.tar.gz
fetch_lib svpv_qsort https://github.com/svpv/qsort/archive/32667a20d3725f62ab2c3def00fcaa96ce2a5508.tar.gz
#fetch_lib justinow_introsort_c https://github.com/justinow/introsort-c/archive/eee26ae2e95f959debe821f030159ac49851b3d6.tar.gz
fetch_file freebsd_qsort.c "https://raw.githubusercontent.com/freebsd/freebsd/9e4440ca5ba7072e6b8c4d0d9799fb9d82c5ad49/lib/libc/stdlib/qsort.c"
fetch_file musl_qsort.c "https://git.musl-libc.org/cgit/musl/plain/src/stdlib/qsort.c?id=30f55067a6dcba602797c24e020a6a9c8ba22180"
fetch_file glibc_qsort.c "https://sourceware.org/git/?p=glibc.git;a=blob_plain;f=stdlib/qsort.c;hb=682cdd6e1aa4c7a84737f85b9e6612cac10da959"
fetch_file glibc_msort.c "https://sourceware.org/git/?p=glibc.git;a=blob_plain;f=stdlib/msort.c;hb=682cdd6e1aa4c7a84737f85b9e6612cac10da959"
cd ../../..
echo

# Make some empty files to work around includes in the various libc qsorts
make_empty_file() {
    if ! [ -e $LIB/$1 ]; then
        touch $LIB/$1
    fi
}
make_empty_file atomic.h
make_empty_file libc_private.h
make_empty_file memcopy.h

if ! [ -e /usr/include/boost/sort/sort.hpp ] && ! [ -e /usr/local/include/boost/sort/sort.hpp ]; then
    echo
    echo "WARNING: Install Boost to include it in benchmarks."
    echo
fi

# Build benchmarks
make -j8 -f test/tools/benchmark-makefile.mk

# Run them
test/build/benchmark/runner
