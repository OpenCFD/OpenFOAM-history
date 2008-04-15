
all: .depend client client2 server

DIR_PREFIX=../
include ../../MakeVars

INSTALL_DIR     = security/acl-unaware-ca-check
INSTALL_SRCS    = Makefile client.cc client2.cc server.cc  account.idl s_cert.pem s_key.pem \
		manager_cert.pem manager_key.pem owner_cert.pem owner_key.pem wife_cert.pem \
		wife_key.pem wife_self_cert.pem wife_self_key.pem ca_cert.pem audit.cnf ODM.cnf \
		access.cnf \
		rights.cnf README
INSTALL_SCRIPTS = rss rss1 rcs rcs1 rcs2 run

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
