
all: .depend client client2 server

DIR_PREFIX=../../
include ../../../MakeVars

INSTALL_DIR     = csiv2/interop/adiron
INSTALL_SRCS    = Makefile client.cc client2.cc server.cc InterOpTest.idl \
  ca_cert.pem c_cert.pem c_key.pem s_cert.pem s_key.pem
INSTALL_SCRIPTS = run.sh get_iors.sh iiop_csiv2_extedned_hello.sh \
  iiop_csiv2_hello.sh iiop_hello.sh tls_csiv2_extedned_hello.sh \
  tls_csiv2_hello.sh tls_hello.sh

server: InterOpTest.h InterOpTest.cc InterOpTest.o server.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) InterOpTest.o server.o $(LDLIBS) -o server

client: InterOpTest.h InterOpTest.cc InterOpTest.o client.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) InterOpTest.o client.o $(LDLIBS) -o client 

client2: InterOpTest.h InterOpTest.cc InterOpTest.o client2.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) InterOpTest.o client2.o $(LDLIBS) -o client2 


InterOpTest.h InterOpTest.cc : InterOpTest.idl $(IDLGEN)
	$(IDL) InterOpTest.idl

run:
	hello

clean:
	rm -f InterOpTest.cc InterOpTest.h ref *.ref *.ior *.o core client client2 server *~ .depend
