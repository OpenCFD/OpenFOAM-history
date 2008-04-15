
all: .depend client server

DIR_PREFIX=../
include ../../MakeVars

INSTALL_DIR     = security/l2
INSTALL_SRCS    = Makefile client.cc server.cc account.idl ClientCert.pem ClientKey.pem \
		ServerCert.pem ServerKey.pem list.pem
INSTALL_SCRIPTS = rss rcs ssl run

server: account.h account.o server.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) account.o server.o $(LDLIBS) -o server

client: account.h account.o client.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) account.o client.o $(LDLIBS) -o client 


account.h account.cc : account.idl $(IDLGEN)
	$(IDL) account.idl

clean:
	rm -f account.cc account.h *.o core client server *~ .depend *.ref
