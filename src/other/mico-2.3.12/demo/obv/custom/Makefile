
all: .depend test

DIR_PREFIX=../
include ../../MakeVars

INSTALL_DIR     = obv/custom
INSTALL_SRCS    = README Makefile test.cc custom.idl custom_impl.cc \
		  custom_impl.h
INSTALL_SCRIPTS = custom

test: custom.h custom_impl.h custom.o test.o custom_impl.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) custom.o test.o custom_impl.o $(LDLIBS) -o test


custom.h custom.cc : custom.idl $(IDLGEN)
	$(IDL) custom.idl

run:
	custom

clean:
	rm -f custom.cc custom.h
	rm -f *.o core test *~ .depend

