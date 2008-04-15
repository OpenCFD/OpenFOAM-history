all:	mtio

BASEDIR = ../../..

include $(BASEDIR)/MakeVars

CXXFLAGS := -I$(BASEDIR)/include $(CXXFLAGS) $(EHFLAGS)
LDFLAGS  := -L$(BASEDIR)/orb $(LDFLAGS)
LDLIBS    = -lmico$(VERSION) $(CONFLIBS)

mtio:	mtio.o $(BASEDIR)/orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) mtio.o $(LDLIBS) -o $@
	$(POSTLD) $@

clean:	
	rm -f *~ *.o mtio


ifeq (.depend, $(wildcard .depend))
include .depend
endif

.depend:
	echo "# module dependencies" > .depend
	$(MKDEPEND) $(CXXFLAGS) *.cc >> .depend
