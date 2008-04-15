
all: .depend prodcons

DIR_PREFIX=../
include ../../MakeVars

INSTALL_DIR     = mt/prodcons-1
INSTALL_SRCS    = Makefile prodcons.cc

prodcons: prodcons.cc prodcons.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) prodcons.o $(LDLIBS) -o prodcons

clean:
	rm -f *.o core prodcons *~ .depend
