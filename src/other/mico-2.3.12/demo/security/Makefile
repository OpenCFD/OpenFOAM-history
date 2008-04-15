all: subs

include ../MakeVars

DIRS=l1 l2 tutorial acl-aware acl-unaware audit-unaware audit-aware audit-book acl-unaware-ca-check

subs:
	for i in $(DIRS); do $(MAKE) -C $$i || exit 1; done

clean:
	for i in $(DIRS); do $(MAKE) -C $$i clean || exit 1; done
	rm -f *.o core *~ .depend

run:
	for i in $(DIRS); do cd $$i; pwd; ./run; cd ..; sleep 2; done

install:
	$(IDIRCMD) $(INSTDIR)/doc/mico/examples/security
	$(ISRCCMD) Makefile $(INSTDIR)/doc/mico/examples/security
	$(ISRCCMD) README $(INSTDIR)/doc/mico/examples/security
	for i in $(DIRS); do $(MAKE) -C $$i install || exit 1; done
