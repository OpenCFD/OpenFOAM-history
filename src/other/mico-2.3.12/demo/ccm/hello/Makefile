
all: .depend client server

DIR_PREFIX=../
include ../../MakeVars

INSTALL_DIR     = ccm/hello
INSTALL_SRCS    = README Makefile client.cc hello.idl hello_impl.cc
INSTALL_SCRIPTS = hello

client: client.o hello.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) -o $@ client.o hello.o -lmicoccm$(VERSION) -lmicocoss$(VERSION) $(LDLIBS)

server: hello_impl.o hello_ccm.o hello.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) -o $@ hello_impl.o hello_ccm.o hello.o -lmicoccm$(VERSION) -lmicocoss$(VERSION) $(LDLIBS)

run:
	hello

clean:
	$(RM) client server
	$(RM) hello_ccm.cc hello_ccm.h
	$(RM) hello.cc hello.h
	$(RM) *.o *~ core *.ior .depend

client.o: client.cc hello.h
hello.o: hello.cc
hello_impl.o: hello_impl.cc hello_ccm.h hello.h
hello_ccm.o: hello_ccm.cc

hello_ccm.cc hello_ccm.h: hello.idl
	$(CCMGEN) --standalone $^

hello.cc hello.h: hello.idl
	$(IDL) $(IDLFLAGS) $^

