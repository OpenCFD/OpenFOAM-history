#!/bin/sh
echo -n "Testing $1..."
rm -f ird.ior

../../ir/ird -ORBGIOPVersion 1.2 -ORBIIOPVersion 1.2 --ior ird.ior &
ird_pid=$!

trap "kill $ird_pid > /dev/null 2> /dev/null" 0
for i in 0 1 2 3 4 5 6 7 8 9 ; do if test -r ird.ior ; then break ; else sleep 1 ; fi ; done

../../idl/idl -ORBGIOPVersion 1.2 -ORBIIOPVersion 1.2 -ORBInitRef InterfaceRepository=`cat ird.ior ` --feed-ir -I. $1
retval=$?
if test $retval -ne 0; then
  echo error: $retval
else
  echo
fi
