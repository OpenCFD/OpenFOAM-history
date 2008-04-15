
all: subs

include ../MakeVars

DIRS=abstract custom date tree tricky

subs:
	for i in $(DIRS); do $(MAKE) -C $$i || exit 1; done

run:
	for i in $(DIRS); do $(MAKE) -C $$i run || exit 1; done

clean:
	for i in $(DIRS); do $(MAKE) -C $$i clean || exit 1; done
	rm -f *.o core *~ .depend

install:
	$(IDIRCMD) $(INSTDIR)/doc/mico/examples/obv
	$(ISRCCMD) Makefile $(INSTDIR)/doc/mico/examples/obv
	$(ISRCCMD) README $(INSTDIR)/doc/mico/examples/obv
	for i in $(DIRS); do $(MAKE) -C $$i install || exit 1; done
