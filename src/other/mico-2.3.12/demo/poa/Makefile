
all: subs

include ../MakeVars

DIRS=hello-1 hello-2 inherit account-1 account-2 account-3

subs:
	for i in $(DIRS); do $(MAKE) -C $$i || exit 1; done

clean:
	for i in $(DIRS); do $(MAKE) -C $$i clean || exit 1; done
	rm -f *.o core *~ .depend

run:
	for i in $(DIRS); do $(MAKE) -C $$i run || exit 1; done

install:
	$(IDIRCMD) $(INSTDIR)/doc/mico/examples/poa
	$(ISRCCMD) Makefile $(INSTDIR)/doc/mico/examples/poa
	$(ISRCCMD) README $(INSTDIR)/doc/mico/examples/poa
	for i in $(DIRS); do $(MAKE) -C $$i install || exit 1; done
