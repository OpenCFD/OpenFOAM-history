
all: subs

include ../MakeVars

DIRS=

ifeq ($(HAVE_X11), yes)
DIRS := $(DIRS) x11
endif

ifeq ($(HAVE_QT), yes)
DIRS := $(DIRS) qtmico
endif

ifeq ($(HAVE_GTK), yes)
DIRS := $(DIRS) gtkmico
endif

subs:
	test -z "$(DIRS)" || \
	  eval 'for i in $(DIRS); do $(MAKE) -C $$i || exit 1; done'

clean:
	test -z "$(DIRS)" || \
	  eval 'for i in $(DIRS); do $(MAKE) -C $$i clean || exit 1; done'
	rm -f *.o core *~ .depend

install:
	$(IDIRCMD) $(INSTDIR)/doc/mico/examples/dispatcher
	$(ISRCCMD) Makefile $(INSTDIR)/doc/mico/examples/dispatcher
	$(ISRCCMD) README $(INSTDIR)/doc/mico/examples/dispatcher
	test -z "$(DIRS)" || \
	  eval 'for i in $(DIRS); do $(MAKE) -C $$i install || exit 1; done'
