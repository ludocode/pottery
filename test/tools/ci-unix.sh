#!/bin/sh

# This script is run by the continuous integration server to build and run the
# Pottery unit test suite on UNIX systems.

set -e
cd "$(dirname $0)/../.."

[ -z "$CI" ] && export CI=1

# The CI uses a variant with CC=scan-build in order to scan the project.
if [ "$CC" == "scan-build" ]; then
    exec test/scan-build.sh
fi

test/unit.sh all
