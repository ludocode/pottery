#!/bin/bash

# Generates some basic inputs for AFL fuzzing.

# We don't bother providing realistic test cases for each algorithm and data
# structure. Not only would this be an annoying number of files to manage, but
# it would get invalidated any time we change the test harness. Instead we just
# generate some randomness and supply a couple of degenerate cases.
echo "Generating test corpus for AFL fuzzing"

set -e
cd "$(dirname "$0")"/../..

CORPUS=test/.build/corpus
rm -rf $CORPUS
mkdir $CORPUS

# The random test cases just do some random stuff. We supply several of them
# since AFL seems to want them, although I'm not convinced having many of these
# actually helps.
for i in $(seq 1 2); do
    echo " Generating $CORPUS/random$i"
    dd if=/dev/urandom of=$CORPUS/random$i bs=512 count=1 >/dev/null 2>&1
done

# An insert/emplace command is mapped to zero so the zero test case causes the
# container to repeatedly grow.
echo " Generating $CORPUS/zero"
dd if=/dev/zero of=$CORPUS/zero bs=2048 count=1 >/dev/null 2>&1

# A remove command is mapped to one so the zero_then_one test case grows
# the container then shrinks it back down to nothing.
echo " Generating $CORPUS/zero_then_one"
dd if=/dev/zero of=$CORPUS/zero_then_one bs=1024 count=1 >/dev/null 2>&1
for i in $(seq 1 1024); do printf '\x01' >> $CORPUS/zero_then_one; done

echo "Done"
