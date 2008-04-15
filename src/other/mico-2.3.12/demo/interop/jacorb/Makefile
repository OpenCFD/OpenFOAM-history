
IDLTARGET=Account

all: .depend $(IDLTARGET).class Server.class $(IDLTARGET)Skeleton.class \
  Client.class ClientApplet.class client server

DIR_PREFIX=../
include ../../MakeVars

IDL2J=idl2j
JGEN=jgen
INSTALL_DIR     = interop/jacorb

ARCHIVE=jacorb
TAR=gtar
INSTALL_SRCS    = Makefile $(IDLTARGET).idl Server.java Client.java 	\
		  ClientApplet.html.in ClientApplet.java README 	\
		  server.cc client.cc

INSTALL_SCRIPTS = account run_applet

server: $(IDLTARGET).h $(IDLTARGET).o server.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) $(IDLTARGET).o $@.o $(LDLIBS) -o $@

client: $(IDLTARGET).o client.o
	$(LD) $(CXXFLAGS) $(LDFLAGS) $(IDLTARGET).o client.o $(LDLIBS) -o $@

$(IDLTARGET).h $(IDLTARGET).cc : $(IDLTARGET).idl $(IDLGEN)
	$(IDL) --no-poa --boa $(IDLTARGET).idl

$(IDLTARGET).java : $(IDLTARGET).idl
	$(IDL2J) $(IDLTARGET).idl

$(IDLTARGET)Skeleton.java $(IDLTARGET)Stub.java: $(IDLTARGET).class
	$(JGEN)  $(IDLTARGET).class

clean:
	$(RM) -f	$(IDLTARGET)Skeleton.java  $(IDLTARGET)Stub.java\
		$(IDLTARGET)Holder.java	$(IDLTARGET).cc $(IDLTARGET).h 	\
		.depend *.o core server client *~ $(IDLTARGET).java 	\
		$(IDLTARGET)Helper.java $(IDLTARGET)Holder.java 	\
		$(IDLTARGET)OutHolder.java *.class ClientApplet.html 	\
		account.ref $(ARCHIVE).tgz ../$(ARCHIVE).tgz

$(ARCHIVE).tgz: .
	make clean
	(	cd ..;						\
		$(TAR) zcf $(ARCHIVE).tgz $(ARCHIVE);		\
		mv $(ARCHIVE).tgz $(ARCHIVE)) 
