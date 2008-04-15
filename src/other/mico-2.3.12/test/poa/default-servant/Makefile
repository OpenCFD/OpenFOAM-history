
include ../../../MakeVars

CXXFLAGS := -I. -I../../../include $(CXXFLAGS) #$(EHFLAGS)
LDFLAGS  := -L../../../orb $(LDFLAGS) 
LDLIBS   := -lmico$(VERSION) $(CONFLIBS)

all: .depend client server

client:	hello.o client.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@
	$(POSTLD) $@                                                            

server:	hello.o server.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@
	$(POSTLD) $@                                                            

hello.cc hello.h : hello.idl
	$(IDL) hello.idl                                            

clean:
	$(RM) -f *.o core server client hello.h hello.cc *.ref *~ .depend

check:
	@echo "Testing ./default-servant..."
	@if ./hello|cmp expected-output - >/dev/null; then : ; \
	else echo "FAILED:"; echo "===============================" \
	./hello|diff -u expected-output - ; \
	echo "==============================="; fi

ifeq (.depend, $(wildcard .depend))
include .depend
endif

.depend:
	echo "# module dependencies" > .depend
	$(MKDEPEND) $(CXXFLAGS) *.cc >> .depend
