#!/bin/sh
set -e
for f in examples/pottery/*/; do
    echo -e "\n\n\nBuilding $f"

    # We don't run clayfish since it's interactive.
    if [ "$f" == "examples/pottery/clayfish/" ]; then
        ( cd $f ; make build )
    else
        ( cd $f ; make run )
    fi
done

echo -e "\n\n\nDone!"
