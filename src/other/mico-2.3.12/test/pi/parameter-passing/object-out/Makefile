
include ../../../../MakeVars

CXXFLAGS := -I. -I../../../../include $(CXXFLAGS) #$(EHFLAGS)
LDFLAGS  := -L../../../../orb $(LDFLAGS) 
LDLIBS   := -lmico$(VERSION) $(CONFLIBS)

all: .depend client server

client:	foo.o client.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@
	$(POSTLD) $@                                                            

server: foo.o server.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@
	$(POSTLD) $@                                                            

foo.cc foo.h : foo.idl
	$(IDL) foo.idl                                            

clean:
	$(RM) -f *.o core server client foo.h foo.cc *.ref *~ .depend

ifeq (.depend, $(wildcard .depend))
include .depend
endif

.depend:
	echo "# module dependencies" > .depend
	$(MKDEPEND) $(CXXFLAGS) *.cc >> .depend
