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

SRC=$1
shift
OBJS=$@

LIST=test/build/pottery_unit_test_list
rm -f $LIST $SRC

for f in $OBJS; do
    objdump -t $f|grep 'F \.text.*\<pottery_unit_test_register_'|awk '{print $6}' >> $LIST
done

cat >>$SRC <<EOF
#ifdef __cplusplus
extern "C" {
#endif

EOF

sed 's/.*/void &(void);/' $LIST >> $SRC

cat >>$SRC <<EOF

#ifdef __cplusplus
}
#endif

void pottery_register_unit_tests(void) {
EOF

sed 's/.*/    &();/' $LIST >> $SRC

cat >>$SRC <<EOF
}
EOF

echo "Registered $(wc -l $LIST|awk '{print $1}') tests."
echo "Wrote $SRC"
