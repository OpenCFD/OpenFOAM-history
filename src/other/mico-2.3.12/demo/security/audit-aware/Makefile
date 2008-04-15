
all: .depend client client2 server

DIR_PREFIX=../
include ../../MakeVars

INSTALL_DIR     = security/audit-aware
INSTALL_SRCS    = Makefile client.cc client2.cc server.cc  account.idl ServerCert.pem ServerKey.pem \
		key.pem manager.pem owner.pem wife.pem config.cnf audit.cnf ODM.cnf access.cnf \
		rights.cnf README README.Audit
INSTALL_SCRIPTS = rss rst rsnd rcs rcs run

server: account.h account.o server.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) account.o server.o $(LDLIBS) -o server

client: account.h account.o client.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) account.o client.o $(LDLIBS) -o client 

client2: account.h account.o client2.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) account.o client2.o $(LDLIBS) -o client2 

account.h account.cc : account.idl $(IDLGEN)
	$(IDL) account.idl

clean:
	rm -f account.cc account.h *.ref *.o *.core client client2 server *~ .depend *.log ODM.map *.ref
