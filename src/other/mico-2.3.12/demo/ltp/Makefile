
all: subs

include ../MakeVars

DIRS=hello

subs:
	for i in $(DIRS); do $(MAKE) -C $$i || exit 1; done

clean:
	for i in $(DIRS); do $(MAKE) -C $$i clean || exit 1; done
	rm -f *.o core *~ .depend

install:
	$(IDIRCMD) $(INSTDIR)/doc/mico/examples/ltp
	$(ISRCCMD) Makefile $(INSTDIR)/doc/mico/examples/ltp
	$(ISRCCMD) README $(INSTDIR)/doc/mico/examples/ltp
	for i in $(DIRS); do $(MAKE) -C $$i install || exit 1; done
