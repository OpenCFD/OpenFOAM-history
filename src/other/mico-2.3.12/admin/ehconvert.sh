#!/bin/sh

for i in $*; do
    sed -e 's/\(catch[ \t\n]*.*\)_var[ \t\n]*\&/\1_catch \&/g' < $i > $i.tmp
    mv -f $i $i.bak
    mv -f $i.tmp $i
done
