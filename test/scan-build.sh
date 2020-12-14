#!/bin/sh

# Runs Clang Static Analyzer on the unit test suite.

set -e
cd "$(dirname $0)/.."
unset CC
export CCACHE_DISABLE=true
export POTTERY_SCAN_BUILD=true
test/clean.sh
scan-build -v -analyze-headers --status-bugs test/unit.sh -v "$@"
