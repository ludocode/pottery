#!/bin/sh

# This pulls all of the unit test registration functions out of the object
# files and generates a source file that calls them. This is used to
# auto-register unit tests when compiling under TinyCC or any other compiler
# that doesn't support static constructor functions.
#
# You don't need to call this directly. It's called by the unit test Ninja file
# after building all other object files. See test/tools/configure.py for where
# it generates a call to this script.

set -e
echo "Generating unit test registrations..."

OUT=$1
shift
OBJS=$@

LIST=$OUT.list
rm -f $LIST $OUT

for f in $OBJS; do
    objdump -t $f|grep -o '\<pottery_unit_test_register_[A-Za-z0-9_]*\>' >> $LIST || true
done

cat >>$OUT <<EOF
#ifdef __cplusplus
extern "C" {
#endif

EOF

sed 's/.*/void &(void);/' $LIST >> $OUT

cat >>$OUT <<EOF

void pottery_register_unit_tests(void);

void pottery_register_unit_tests(void) {
EOF

sed 's/.*/    &();/' $LIST >> $OUT

cat >>$OUT <<EOF
}

#ifdef __cplusplus
}
#endif
EOF

echo "Registered $(wc -l $LIST|awk '{print $1}') tests."
echo "Wrote $OUT"
