#!/bin/sh
set -e
set -v
cd "$(dirname $0)/.."
rm -rf test/build .ninja_deps .ninja_log vgcore.* core.*
