
all: all_target

DIR_PREFIX=../
include ../../MakeVars

INSTALL_DIR     = ccm/hello2
INSTALL_SRCS    = README Makefile client.cc hello.idl hello_impl.cc
INSTALL_SCRIPTS = hello

all_target: .depend client hello.$(SOEXT)

client: client.o hello.pic.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) -o $@ client.o hello.pic.o -lmicoccm$(VERSION) -lmicocoss$(VERSION) $(LDLIBS)

hello.$(SOEXT): hello_impl.pic.o hello_ccm.pic.o hello.pic.o $(DEPS)
ifeq ($(HAVE_PRELINK), yes)
	$(CXX) $(PRELINK_FLAG) $(LDFLAGS) hello_impl.pic.o hello_ccm.pic.o hello.pic.o -lmicoccm$(VERSION) $(LDLIBS)
endif
	$(RM) $@
	$(LDSO) $(CXXFLAGS) $(LDFLAGS) -o hello hello_impl.pic.o hello_ccm.pic.o hello.pic.o -lmicoccm$(VERSION) $(LDLIBS)

run:
	hello

clean:
	$(RM) client hello.$(SOEXT)
	$(RM) hello_ccm.cc hello_ccm.h
	$(RM) hello.cc hello.h
	$(RM) *.o *.$(SOEXT) *~ core *.ior .depend

client.o: client.cc hello.h
hello.pic.o: hello.cc
hello_impl.pic.o: hello_impl.cc hello_ccm.h hello.h
hello_ccm.pic.o: hello_ccm.cc

hello_ccm.cc hello_ccm.h: hello.idl
	$(CCMGEN) $^

hello.cc hello.h: hello.idl
	$(IDL) $(IDLFLAGS) $^

