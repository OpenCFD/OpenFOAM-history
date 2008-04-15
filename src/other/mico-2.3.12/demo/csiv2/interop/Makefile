
all: subs

DIR_PREFIX=../

include ../../MakeVars

DIRS=adiron

subs:
	for i in $(DIRS); do $(MAKE) -C $$i || exit 1; done

clean:
	for i in $(DIRS); do $(MAKE) -C $$i clean || exit 1; done
	rm -f *.o core *~ .depend


install:
	$(IDIRCMD) $(INSTDIR)/doc/mico/examples/csiv2/interop
	$(ISRCCMD) Makefile $(INSTDIR)/doc/mico/examples/csiv2/interop
	$(ISRCCMD) README $(INSTDIR)/doc/mico/examples/csiv2/interop
	for i in $(DIRS); do $(MAKE) -C $$i install || exit 1; done
