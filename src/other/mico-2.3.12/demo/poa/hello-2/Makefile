
all: .depend client server

DIR_PREFIX=../
include ../../MakeVars

INSTALL_DIR     = poa/hello-2
INSTALL_SRCS    = Makefile client.cc server.cc hello.idl
INSTALL_SCRIPTS = hello

server: hello.h hello.o server.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) hello.o server.o $(LDLIBS) -o server

client: hello.h hello.o client.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) hello.o client.o $(LDLIBS) -o client 


hello.h hello.cc : hello.idl $(IDLGEN)
	$(IDL) --poa-ties hello.idl

run:
	hello

clean:
	rm -f hello.cc hello.h hello.ref *.o core client server *~ .depend
