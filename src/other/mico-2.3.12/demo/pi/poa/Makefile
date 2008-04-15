
all: .depend client server collocated

DIR_PREFIX=../
include ../../MakeVars

INSTALL_DIR     = pi/poa
INSTALL_SRCS    = Makefile client.cc server.cc collocated.cc account.idl
INSTALL_SCRIPTS = account

server: account.h account.o server.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) account.o server.o $(LDLIBS) -o server

client: account.h account.o client.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) account.o client.o $(LDLIBS) -o client 

collocated: account.h account.o collocated.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) account.o collocated.o $(LDLIBS) -o collocated

account.h account.cc : account.idl $(IDLGEN)
	$(IDL) --any account.idl

run:
	./account

clean:
	rm -f account.cc account.h Bank.ref *.o core client server collocated *~ .depend
