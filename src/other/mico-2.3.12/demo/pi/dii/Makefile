
DIR_PREFIX=../
include ../../MakeVars

INSTALL_DIR     = pi/dii
INSTALL_SRCS    = Makefile diiclient.cc diiserver.cc
INSTALL_SCRIPTS = rundiic rundiis runit

all: .depend diiclient diiserver

diiclient: diiclient.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) diiclient.o $(LDLIBS) -o diiclient

diiserver: diiserver.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) diiserver.o $(LDLIBS) -o diiserver

run:
	./runit

clean:
	rm -f diiclient diiserver .depend *.o *.ior core
