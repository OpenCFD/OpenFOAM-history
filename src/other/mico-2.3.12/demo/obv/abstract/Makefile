
all: .depend client server

DIR_PREFIX=../
include ../../MakeVars

INSTALL_DIR     = obv/abstract
INSTALL_SRCS    = README Makefile client.cc server.cc abstract.idl \
		  abstract_impl.h
INSTALL_SCRIPTS = abstract

server: abstract.h abstract_impl.h abstract.o server.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) abstract.o server.o $(LDLIBS) -o server

client: abstract.h abstract_impl.h abstract.o client.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) abstract.o client.o $(LDLIBS) -o client 


abstract.h abstract.cc : abstract.idl $(IDLGEN)
	$(IDL) abstract.idl

run:
	abstract

clean:
	rm -f abstract.cc abstract.h
	rm -f *.o core client server *~ *.ref .depend

