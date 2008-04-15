set ADDR=ssl:inet:localhost:7788
server -ORBIIOPAddr %ADDR% -ORBSSLCAfile ca_cert.pem -ORBSSLcert s_cert.pem -ORBSSLkey s_key.pem -ORBSSLverify 1 %1 %2 %3 %4 %5 %6 %7 %8 %9 > ref 
