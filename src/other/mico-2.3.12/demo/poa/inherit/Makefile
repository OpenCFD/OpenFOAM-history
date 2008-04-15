
all: .depend client server

DIR_PREFIX=../
include ../../MakeVars

INSTALL_DIR     = poa/inherit
INSTALL_SRCS    = Makefile client.cc server.cc inherit.idl
INSTALL_SCRIPTS = inherit

server: inherit.h inherit.o server.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) inherit.o server.o $(LDLIBS) -o server

client: inherit.h inherit.o client.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) inherit.o client.o $(LDLIBS) -o client 


inherit.h inherit.cc : inherit.idl $(IDLGEN)
	$(IDL) --poa-ties inherit.idl

run:
	inherit

clean:
	rm -f inherit.cc inherit.h *.o core client server *~ .depend
