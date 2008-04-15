all: .depend junk_client_2 junk_server

DIR_PREFIX=../
include ../../MakeVars

CXXFLAGS  := $(COS_CXXFLAGS) $(CXXFLAGS)
LDLIBS    := $(COS_LDLIBS) $(LDLIBS)
LDFLAGS   := $(COS_LDFLAGS) $(LDFLAGS)
DEPS      := $(COS_DEPS) $(DEPS)

junk_client_2: Junk.h junk_client_2.o Junk.o JunkNC.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) Junk.o JunkNC.o junk_client_2.o \
                               $(LDLIBS) -o $@

junk_server: Junk.h Junk.o Junk_impl.o junk_server.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) Junk.o Junk_impl.o junk_server.o \
                               $(LDLIBS) -o $@

Junk.h Junk.cc : Junk.idl $(IDLGEN)
	$(IDL) Junk.idl

clean:
	rm -f .depend Junk.cc Junk.h *.o junk_client_2 junk_server *.ior nsd.db
	rm -rf db
