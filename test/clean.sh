#!/bin/sh
set -e
cd "$(dirname $0)/.."
rm -rf test/build .ninja_deps .ninja_log vgcore.* core.*
