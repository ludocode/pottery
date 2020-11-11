#!/bin/sh
set -e
for f in examples/pottery/*; do

    # skip common since it's not a real example
    if [ $(basename $f) == "common" ]; then
        continue
    fi

    echo -e "\n\n\nBuilding $f"
    ( cd $f ; make )
done
