#
# MICO --- a free CORBA implementation
# Copyright (C) 1997 Kay Roemer & Arno Puder
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# Send comments and/or bug reports to:
#                mico@informatik.uni-frankfurt.de
#

include ../../MakeVars

LDLIBS    = -lmicocoss$(VERSION) -lmico$(VERSION) $(CONFLIBS)
LDFLAGS  := -L.. -L../../orb $(LDFLAGS)
CXXFLAGS := -I. -I../../include $(CXXFLAGS) $(EHFLAGS)
IDLFLAGS := -B../.. --poa --no-boa --any

# generated files

OMG_SRCS =  CosExternalization.cc \
	    CosExternalizationReference.cc \
	    CosExternalizationContainment.cc \
	    CompoundExternalization_impl.cc \
	    ExternalizationPropagationCriteriaFactory_impl.cc \
	    ExternalizationTraversalCriteria_impl.cc \
	    Externalization_impl.cc \
	    Stream_impl.cc \
	    ExternalizationContainment_impl.cc \
	    ExternalizationReference_impl.cc \

DAEMONS_SRCS = extnoded.cc \
	       extcontainmentd.cc \
	       extreferenced.cc \
	       streamd.cc \
	       extcriteria.cc

STATIC_OBJS = $(OMG_SRCS:.cc=.o)
SHARED_OBJS = $(OMG_SRCS:.cc=.pic.o)

# normal rules

all: lib prg

ifeq ($(HAVE_SHARED_EXCEPTS), yes)
ifeq ($(HAVE_STATIC), yes)
lib: .depend $(STATIC_OBJS) $(SHARED_OBJS)
else
lib: .depend $(SHARED_OBJS)
endif
else
lib: .depend $(STATIC_OBJS)
endif

prg: .depend extnoded extcontainmentd extreferenced streamd extcriteriad

streamd: streamd.o ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) streamd.o $(LDLIBS) -o streamd
	$(POSTLD) $@

extnoded: extnoded.o ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) extnoded.o $(LDLIBS) -o extnoded
	$(POSTLD) $@

extcriteriad: extcriteriad.o ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) extcriteriad.o $(LDLIBS) -o extcriteriad
	$(POSTLD) $@

extcontainmentd: extcontainmentd.o ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) extcontainmentd.o $(LDLIBS) -o extcontainmentd
	$(POSTLD) $@

extreferenced: extreferenced.o ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) extreferenced.o $(LDLIBS) -o extreferenced
	$(POSTLD) $@

idlcompile: CosExternalization.cc CosExternalizationContainment.cc CosExternalizationReference.cc

CosExternalization.h CosExternalization.cc: \
		../../include/coss/CosExternalization.idl $(IDL)
	$(IDL) $(IDLFLAGS) --name CosExternalization \
		../../include/coss/CosExternalization.idl
	cp CosExternalization.h ../../include/coss

CosExternalizationContainment.h CosExternalizationContainment.cc: \
		../../include/coss/CosExternalizationContainment.idl $(IDL)
	$(IDL) $(IDLFLAGS) --name CosExternalizationContainment \
		../../include/coss/CosExternalizationContainment.idl
	cp CosExternalizationContainment.h ../../include/coss

CosExternalizationReference.h CosExternalizationReference.cc: \
		../../include/coss/CosExternalizationReference.idl $(IDL)
	$(IDL) $(IDLFLAGS) --name CosExternalizationReference \
		../../include/coss/CosExternalizationReference.idl
	cp CosExternalizationReference.h ../../include/coss


# cleaning

clean:
	-$(RM) *.o *.a *~ *.rpo *.objid extnoded extcontainmentd extreferenced streamd extcriteriad
	-$(RM) .depend TAGS core nohup.out
	-$(RM) CosExternalization.h CosExternalization.cc
	-$(RM) ../../include/coss/CosExternalization.h
	-$(RM) CosExternalizationContainment.h CosExternalizationContainment.cc
	-$(RM) ../../include/coss/CosExternalizationContainment.h
	-$(RM) CosExternalizationReference.h CosExternalizationReference.cc
	-$(RM) ../../include/coss/CosExternalizationReference.h

install:
	$(IDIRCMD) $(INSTDIR)/bin
	$(IBINCMD) streamd $(INSTDIR)/bin
	$(IDIRCMD) $(INSTDIR)/bin
	$(IBINCMD) extnoded $(INSTDIR)/bin
	$(IDIRCMD) $(INSTDIR)/bin
	$(IBINCMD) extcontainmentd $(INSTDIR)/bin
	$(IDIRCMD) $(INSTDIR)/bin
	$(IBINCMD) extreferenced $(INSTDIR)/bin
	$(IDIRCMD) $(INSTDIR)/bin
	$(IBINCMD) extcriteriad $(INSTDIR)/bin


ifeq (.depend, $(wildcard .depend))
include .depend
endif

.depend :
	echo '# Module dependencies' > .depend
	$(MKDEPEND) $(CXXFLAGS) *.cc >> .depend

