
all: .depend client server

DIR_PREFIX=../
include ../../MakeVars

INSTALL_DIR     = mt/callback
INSTALL_SRCS    = Makefile client.cc server.cc callback.idl
INSTALL_SCRIPTS = run

server:  callback.h callback.cc callback.o server.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) callback.o server.o $(LDLIBS) -o server

client: callback.h callback.cc callback.o client.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) callback.o client.o $(LDLIBS) -o client 


callback.cc callback.h: callback.idl $(IDLGEN)
	$(IDL) callback.idl

clean:
	rm -f callback.cc callback.h *.o core client server *~ .depend
