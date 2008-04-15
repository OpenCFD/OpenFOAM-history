include ../../../MakeVars

CXXFLAGS := -I. -I../../../include $(CXXFLAGS) #$(EHFLAGS)
LDFLAGS  := -L../../../orb $(LDFLAGS) 
LDLIBS    = -lmico$(VERSION) $(CONFLIBS)

all: .depend client server

client: request.o z.h z.o client.o $(DEPS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) request.o z.o client.o $(LDLIBS) -o $@

server: request.o z.h z.o server.o $(DEPS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) request.o z.o server.o $(LDLIBS) -o $@

z.h z.cc : z.idl $(IDLGEN)
	$(IDL) z.idl

clean:
	rm -f *.o *.ior core client server *~ .depend z.cc z.h Z.ref


ifeq (.depend, $(wildcard .depend))
include .depend
endif

.depend:
	echo "# module dependencies" > .depend
	$(MKDEPEND) $(CXXFLAGS) *.cc >> .depend

