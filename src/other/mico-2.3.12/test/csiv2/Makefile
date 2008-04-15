
include ../../MakeVars

CXXFLAGS := -I. -I../../include $(CXXFLAGS) $(EHFLAGS)
LDLIBS    = -lmico$(VERSION) $(CONFLIBS)
LDFLAGS  := -L../../orb $(LDFLAGS)

all: .depend codec

codec:  codec.o ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) codec.o $(LDLIBS) -o codec
	$(POSTLD) $@

clean:
	rm -f .depend *.o core codec *~

ifeq (.depend, $(wildcard .depend))
include .depend
endif

.depend :
	echo '# Module dependencies' > .depend
	$(MKDEPEND) $(CXXFLAGS) *.cc >> .depend

