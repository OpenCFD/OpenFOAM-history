
all: .depend client server

DIR_PREFIX=../
include ../../MakeVars

INSTALL_DIR     = csiv2/tls-hello-2
INSTALL_SRCS    = Makefile client.cc server.cc hello.idl s_cert.pem s_key.pem c_cert.pem c_key.pem ca_cert.pem
INSTALL_SCRIPTS = hello

server: hello.h hello.o server.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) hello.o server.o $(LDLIBS) -o server

client: hello.h hello.o client.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) hello.o client.o $(LDLIBS) -o client 


hello.h hello.cc : hello.idl $(IDLGEN)
	$(IDL) hello.idl

run:
	hello

clean:
	rm -f hello.cc hello.h hello.ref *.o core client server *~ .depend
