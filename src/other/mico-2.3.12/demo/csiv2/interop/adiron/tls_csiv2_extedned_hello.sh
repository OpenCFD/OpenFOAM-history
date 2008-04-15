#!/bin/sh
rm -f ref
rm -f peter.ref
cp Johnson_TLS_CSIv2_UP_POA.ior ref
# run Peter server
./server -ORBSSLCAfile ca_cert.pem -ORBSSLcert s_cert.pem -ORBSSLkey s_key.pem -ORBSSLverify 1 -ORBIIOPAddr ssl:inet:`uname -n`:11500 -ORBNoResolve -ORBNoCodeSets -ORBCSIv2 -ORBCSIv2Realm '@objectsecurity.com' -ORBGSSServerUser karel,cobalt & 
server_pid=$!

trap "kill $server_pid > /dev/null 2> /dev/null" 0
for i in 0 1 2 3 4 5 6 7 8 9 ; do if test -r peter.ref ; then break ; else sleep 1 ; fi ; done

./client2 -ORBSSLCAfile ca_cert.pem -ORBSSLcert c_cert.pem -ORBSSLkey c_key.pem -ORBSSLverify 1 -ORBCSIv2 -ORBCSSNoAttr -ORBGSSClientUser guest,password
