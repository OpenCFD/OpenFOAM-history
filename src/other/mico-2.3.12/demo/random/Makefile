# Makefile for a little mico client that reads random numbers from a
# Corba-server. See client.cc for details.

all: .depend client

include ../MakeVars
INSTALL_DIR     = random
INSTALL_SRCS    = Makefile client.cc random.idl


client: random.h random.o client.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) random.o client.o $(LDLIBS) -o $@

random.h random.cc : random.idl $(IDLGEN)
	$(IDL) random.idl

clean:
	rm -f random.cc random.h *.o core client *~ .depend
