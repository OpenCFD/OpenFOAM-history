
all: subs

include ../MakeVars

DIRS=dii poa poa2 current

subs:
	for i in $(DIRS); do $(MAKE) -C $$i || exit 1; done

run:
	for i in $(DIRS); do $(MAKE) -C $$i run || exit 1; done

clean:
	for i in $(DIRS); do $(MAKE) -C $$i clean || exit 1; done
	rm -f *.o core *~ .depend

install:
	$(IDIRCMD) $(INSTDIR)/doc/mico/examples/pi
	$(ISRCCMD) Makefile $(INSTDIR)/doc/mico/examples/pi
	$(ISRCCMD) README $(INSTDIR)/doc/mico/examples/pi
	for i in $(DIRS); do $(MAKE) -C $$i install || exit 1; done
