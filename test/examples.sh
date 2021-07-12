#!/bin/sh
set -e
for f in examples/pottery/*/; do

    # Skip qsort/ and clayfish/ since they've been moved.
    [ "$f" = "examples/pottery/qsort/" ] && continue
    [ "$f" = "examples/pottery/clayfish/" ] && continue

    echo -e "\n\n\nBuilding $f"
    ( cd $f ; make run )
done

echo -e "\n\n\nDone!"
