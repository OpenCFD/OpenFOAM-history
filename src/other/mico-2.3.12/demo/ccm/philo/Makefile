
all: all_target

DIR_PREFIX=../
include ../../MakeVars

INSTALL_DIR     = ccm/philo
INSTALL_SRCS    = README Makefile client.cc philo_impl.cc philo.idl \
		  tkobserver.cc
INSTALL_SCRIPTS = philo

all_target: .depend client philo.$(SOEXT)

client: client.o philo.pic.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) -o $@ client.o philo.pic.o \
		-lmicoccm$(VERSION) -lmicocoss$(VERSION) $(LDLIBS)

philo.$(SOEXT): philo_impl.pic.o philo_ccm.pic.o philo.pic.o $(DEPS)
ifeq ($(HAVE_PRELINK), yes)
	$(CXX) $(PRELINK_FLAG) $(LDFLAGS) philo_impl.pic.o philo_ccm.pic.o philo.pic.o \
		-lmicoccm$(VERSION) $(LDLIBS)
endif
	$(RM) $@
	$(LDSO) $(CXXFLAGS) $(LDFLAGS) -o philo \
		philo_impl.pic.o philo_ccm.pic.o philo.pic.o \
		-lmicoccm$(VERSION) $(LDLIBS)

tkobserver.pic.o: tkobserver.cc philo.h
	$(CXX) -c -o $@ $(CPPFLAGS) -I/local/include $(CXXFLAGS) $(PICFLAGS) $<

tkobserver.$(SOEXT): tkobserver.pic.o philo_ccm.pic.o philo.pic.o $(DEPS)
	$(RM) $@
	$(LDSO) $(CXXFLAGS) $(LDFLAGS) -o tkobserver \
		tkobserver.pic.o philo_ccm.pic.o philo.pic.o \
		-L/local/lib -L/usr/X11R6/lib -lX11 -ltk8.3 -ltcl8.3 \
		-lmicoccm$(VERSION) $(LDLIBS)

clean:
	$(RM) client philo.$(SOEXT) tkobserver.$(SOEXT)
	$(RM) philo_ccm.cc philo_ccm.h
	$(RM) philo.cc philo.h
	$(RM) *.o *.$(SOEXT) *~ core *.ior .depend

client.o: client.cc philo.h
philo.pic.o: philo.cc
philo_impl.pic.o: philo_impl.cc philo.h
philo_ccm.pic.o: philo_ccm.cc

philo_ccm.cc philo_ccm.h: philo.idl
	$(CCMGEN) $(IDLFLAGS) $^

philo.cc philo.h: philo.idl
	$(IDL) $(IDLFLAGS) $^
