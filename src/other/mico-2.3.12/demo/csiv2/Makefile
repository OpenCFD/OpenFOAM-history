
all: subs

include ../MakeVars

DIRS=hello-1 tls-hello-1 tls-hello-2 identity-1 tls-identity-1 interop
RUNDIRS=hello-1 tls-hello-1 tls-hello-2 identity-1 tls-identity-1

subs:
	for i in $(DIRS); do $(MAKE) -C $$i || exit 1; done

clean:
	for i in $(DIRS); do $(MAKE) -C $$i clean || exit 1; done
	rm -f *.o core *~ .depend

run:
	for i in $(RUNDIRS); do $(MAKE) -C $$i run || exit 1; done

install:
	$(IDIRCMD) $(INSTDIR)/doc/mico/examples/csiv2
	$(ISRCCMD) Makefile $(INSTDIR)/doc/mico/examples/csiv2
	$(ISRCCMD) README $(INSTDIR)/doc/mico/examples/csiv2
	for i in $(DIRS); do $(MAKE) -C $$i install || exit 1; done
