@echo off
@rem  SSL addresses are usual MICO addresses prefixed by 'ssl:', e.g.
@rem   ssl:inet:0.0:0
@rem   ssl:unix:/tmp/socket

set ADDR=ssl:inet:localhost:12124

@rem  SSL options:
@rem   -ORBSSLcert <certificate file>
@rem     .pem file that holds your certificate, defaults to default.pem.
@rem   -ORBSSLkey <key file>
@rem     .pem file that holds your key pair, defaults to same file as the
@rem     certificate file.
@rem   -ORBSSLcipher <colon separated list of preferred ciphers>
@rem     cipher(s) that can be used, eg:
@rem     NULL-MD5     RC4-MD5      EXP-RC4-MD5
@rem     IDEA-CBC-MD5 RC2-CBC-MD5  EXP-RC2-CBC-MD5
@rem     DES-CBC-MD5  DES-CBC-SHA  DES-CBC3-MD5
@rem     DES-CBC3-SHA DES-CFB-M1
@rem   -ORBSSLverify <verify depth>
@rem     if specified the peer must supply a valid certificate, otherwise the
@rem     connection setup will fail. <verify depth> specifies how many
@rem     hops of the chain of certification authorities should be checked.
@rem     by default the validity of the peer's certificate is not checked.
@rem   -ORBSSLCAfile <CA filename>
@rem     .pem file that holds certificates of CA's
@rem   -ORBSSLCApath <CA pathname>
@rem     directory that contains .pem files holding certificates of CA's,
@rem     defaults to /usr/local/ssl/certs.
@rem 
@rem  the standard OpenSSL environment variables work as well, but the -ORBSSL*
@rem  options override them
@rem 
@rem  the server key file with the servers private key is created by
@rem    openssl genrsa -out s_key.pem
@rem  a self signed certificate for for server is created by
@rem    openssl req -days 1000 -new -x509 -key s_key.pem -out s_cert.pem
@rem  the client key and certificate are created analogous.
@rem  see the OpenSSL documentation for details.


start ./server.exe -ORBIIOPAddr %ADDR% -ORBSSLcert s_cert.pem -ORBSSLkey s_key.pem -ORBSSLverify 0 -ORBDebug All -ORBNoResolve
sleep 2

set IOR=IOR:010000001400000049444c3a53656375726548656c6c6f3a312e30000200000001000000240000000100000001000000010000001400000001000000010001000000000009010100000000000000000058000000010101001f000000736f72696e6d326b2e78326b2e6765636164736f6674776172652e636f6d00005c2f0000130000002f313536382f313034333038323033322f5f30000100000014000000080000000100660000005c2f

start ./client.exe %IOR% -ORBSSLcert c_cert.pem -ORBSSLkey c_key.pem  -ORBSSLverify 0 -ORBDebug All -ORBNoResolve

