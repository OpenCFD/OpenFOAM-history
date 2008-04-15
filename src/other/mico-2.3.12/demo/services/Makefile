
all: subs

include ../MakeVars

DIRS=

ifeq ($(USE_NAMING), yes)
DIRS := $(DIRS) naming naming-lb naming-mt
endif

ifeq ($(USE_EVENTS), yes)
DIRS := $(DIRS) events
endif

ifeq ($(USE_STREAMS), yes)
DIRS := $(DIRS) stream-bench
endif

ifeq ($(USE_RELSHIP), yes)
DIRS := $(DIRS) relship
endif

ifeq ($(USE_PROPERTY), yes)
DIRS := $(DIRS) property-daemon
endif

ifeq ($(USE_TRADER), yes)
DIRS := $(DIRS) trader
endif

ifeq ($(USE_LIFE), yes)
DIRS := $(DIRS) lifecycle
endif

ifeq ($(USE_EXTERN), yes)
DIRS := $(DIRS) externalize 
endif

ifeq ($(USE_WIRELESS), yes)
DIRS := $(DIRS) wireless
endif

subs:
	for i in $(DIRS); do $(MAKE) -C $$i || exit 1; done

clean:
	for i in $(DIRS); do $(MAKE) -C $$i clean || exit 1; done
	rm -f *.o core *~ .depend

install:
	$(IDIRCMD) $(INSTDIR)/doc/mico/examples/services
	$(ISRCCMD) Makefile $(INSTDIR)/doc/mico/examples/services
	$(ISRCCMD) README $(INSTDIR)/doc/mico/examples/services
	for i in $(DIRS) stream-sound; do $(MAKE) -C $$i install || exit 1; done
