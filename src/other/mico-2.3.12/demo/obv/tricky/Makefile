
all: .depend client server

DIR_PREFIX=../
include ../../MakeVars

INSTALL_DIR     = obv/tricky
INSTALL_SRCS    = Makefile client.cc server.cc tricky.idl \
		  tricky_impl.cc tricky_impl.h
INSTALL_SCRIPTS = tricky

server: tricky.h tricky_impl.h tricky.o tricky_impl.o server.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) tricky.o tricky_impl.o server.o $(LDLIBS) -o server

client: tricky.h tricky_impl.h tricky.o tricky_impl.o client.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) tricky.o tricky_impl.o client.o $(LDLIBS) -o client 


tricky.h tricky.cc : tricky.idl $(IDLGEN)
	$(IDL) tricky.idl

run:
	tricky

clean:
	rm -f tricky.cc tricky.h calendar.cc calendar.h
	rm -f *.o core client server *~ .depend

