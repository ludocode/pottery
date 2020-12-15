#!/bin/bash

# This script is run by the continuous integration server to build and run the
# Pottery unit test suite on UNIX systems.

set -e
cd "$(dirname $0)/../.."

[ -z "$CI" ] && export CI=1

# The CI uses a variant with CC=scan-build in order to scan the project.
if [ "$CC" = "scan-build" ]; then
    exec test/scan-build.sh
fi

# Run the "more" variant of unit tests
test/unit.sh more

# Also build and run all examples standalone to make sure they work
test/examples.sh
