
all: subs

include ../MakeVars

DIRS=hello hello2 philo events
RUNDIRS=hello hello2 events

subs:
	for i in $(DIRS); do $(MAKE) -C $$i || exit 1; done

run:
	for i in $(RUNDIRS); do $(MAKE) -C $$i run || exit 1; done

clean:
	for i in $(DIRS); do $(MAKE) -C $$i clean || exit 1; done
	rm -f *.o core *~ .depend

install:
	$(IDIRCMD) $(INSTDIR)/doc/mico/examples/ccm
	$(ISRCCMD) Makefile $(INSTDIR)/doc/mico/examples/ccm
	for i in $(DIRS); do $(MAKE) -C $$i install || exit 1; done
