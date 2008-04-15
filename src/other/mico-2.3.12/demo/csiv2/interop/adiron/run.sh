#!/bin/sh
echo ""
echo "IIOP Hello test"
echo ""
./iiop_hello.sh
echo
echo "TLS Hello test"
echo
./tls_hello.sh
echo ""
echo "IIOP CSIv2 test"
echo ""
./iiop_csiv2_hello.sh
echo ""
echo "TLS CSIv2 test"
echo ""
./tls_csiv2_hello.sh
echo ""
echo "IIOP CSIv2 extended test"
echo ""
./iiop_csiv2_extedned_hello.sh
echo
echo "TLS CSIv2 extended test"
echo
./tls_csiv2_extedned_hello.sh
