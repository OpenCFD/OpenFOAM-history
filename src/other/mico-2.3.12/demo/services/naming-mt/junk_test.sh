#!/bin/sh

PATH=../../../daemon:../../../coss/naming:../../../imr:../../../ir:$PATH
export PATH
export LB_LIBRARY_PATH=../../../libs

export MICORC=/dev/null


if test $# -eq 0
    then
        hname=`uname -n`
    else
        hname=localhost
fi

mkdir db
rm -f db/*

rm -f nsd.db
rm -f micod.ior junk_factory.ior nsd.ior

nsd --db nsd.db --ior nsd.ior &
nsd_pid=$!

for i in 0 1 2 3 4 5 6 7 8 9 ; do if test -r nsd.ior ; then break ; else sleep 1 ; fi ; done

micod -ORBIIOPAddr inet:${hname}:12456\
                    --forward --ior micod.ior &
micod_pid=$!
#trap "kill $daemon_pid > /dev/null 2> /dev/null" 0
for i in 0 1 2 3 4 5 6 7 8 9 ; do if test -r micod.ior ; then break ; else sleep 1 ; fi ; done
                                                                                
# Add imr info
imr -ORBImplRepoIOR file://`pwd`/micod.ior\
    create JunkServer poa `pwd`/junk_server IDL:Junk:1.0
imr -ORBImplRepoIOR file://`pwd`/micod.ior activate JunkServer
for i in 0 1 2 3 4 5 6 7 8 9 ; do if test -r junk_factory.ior ; then break ; else sleep 1 ; fi ; done
                                                                                
./junk_client_2 1 -ORBInitRef NameService=`cat nsd.ior` &
c1_pid=$!
./junk_client_2 2 -ORBInitRef NameService=`cat nsd.ior` &
c2_pid=$!
./junk_client_2 3 -ORBInitRef NameService=`cat nsd.ior` &
c3_pid=$!
./junk_client_2 4 -ORBInitRef NameService=`cat nsd.ior` &
c4_pid=$!

trap "kill $micod_pid $nsd_pid $c1_pid $c2_pid $c3_pid $c4_pid > /dev/null 2> /dev/null" 0
echo
echo
echo "Press Ctrl-C for finishing test."
echo
echo
wait $c1_pid $c2_pid $c3_pid $c4_pid
