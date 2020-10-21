#!/bin/sh

# Builds and runs the unit test suite.
# Set CC before calling this to use a different compiler.
# Pass a configuration to run or pass "all" to run all configurations.

set -e
cd "$(dirname $0)/.."

# Generate meta-templates
meta/generate.py

# Fail CI builds if the above generate script changed anything
if [ -n "$CI" ] && [ -e test/build/meta_changed ]; then
    echo "Failing CI build due to generated meta-template changes."
    exit 1
fi

# Configure and run unit tests
test/tools/configure.py
ninja -f test/build/build.ninja $@
