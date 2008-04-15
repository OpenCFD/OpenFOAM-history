#!/bin/sh
rm -f Johnson_*.ior
IOR_DIR="http://www.cis.syr.edu/%7Epolar/CSIv2InterOpTestBed/"
wget $IOR_DIR/Johnson_IIOP_POA.ior
wget $IOR_DIR/Johnson_TLS_POA.ior
wget $IOR_DIR/Johnson_IIOP_CSIv2_UP_POA.ior
wget $IOR_DIR/Johnson_TLS_CSIv2_UP_POA.ior
