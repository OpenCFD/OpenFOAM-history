
all: .depend client server

DIR_PREFIX=../
include ../../MakeVars

INSTALL_DIR     = security/audit-book
INSTALL_SRCS    = Makefile client.cc server.cc hello.idl ServerCert.pem ServerKey.pem \
		ClientCert.pem ClientKey.pem README
INSTALL_SCRIPTS = rss rcs runit ssl run

server: hello.h hello.o server.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) hello.o server.o $(LDLIBS) -o server

client: hello.h hello.o client.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) hello.o client.o $(LDLIBS) -o client 

hello.h hello.cc : hello.idl $(IDLGEN)
	$(IDL) hello.idl

clean:
	rm -f hello.cc hello.h *.ref *.o *.core client client2 server *~ .depend *.log ODM.map
