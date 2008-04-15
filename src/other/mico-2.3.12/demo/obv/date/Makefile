
all: .depend client server

DIR_PREFIX=../
include ../../MakeVars

INSTALL_DIR     = obv/date
INSTALL_SRCS    = Makefile client.cc server.cc date.idl calendar.idl \
		  date_impl.cc date_impl.h
INSTALL_SCRIPTS = date

server: date.h date_impl.h calendar.h date.o date_impl.o calendar.o server.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) date.o date_impl.o calendar.o server.o $(LDLIBS) -o server

client: date.h date_impl.h calendar.h date.o date_impl.o calendar.o client.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) date.o date_impl.o calendar.o client.o $(LDLIBS) -o client 


date.h date.cc : date.idl $(IDLGEN)
	$(IDL) date.idl

calendar.h calendar.cc : calendar.idl $(IDLGEN)
	$(IDL) calendar.idl

run:
	date

clean:
	rm -f date.cc date.h calendar.cc calendar.h
	rm -f *.o core client server *~ .depend

