#!/bin/sh
rm -f ref
cp Johnson_TLS_CSIv2_UP_POA.ior ref
./client -ORBSSLCAfile ca_cert.pem -ORBSSLcert c_cert.pem -ORBSSLkey c_key.pem -ORBSSLverify 1 -ORBCSIv2 -ORBCSSNoAttr -ORBGSSClientUser guest,password
