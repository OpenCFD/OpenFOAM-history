#!/bin/sh
rm -f ref
cp Johnson_IIOP_CSIv2_UP_POA.ior ref
./client -ORBCSIv2 -ORBCSSNoAttr -ORBGSSClientUser guest,password
