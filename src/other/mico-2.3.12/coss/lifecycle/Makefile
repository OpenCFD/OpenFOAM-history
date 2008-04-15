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

OMG_SRCS =  CosLifeCycle.cc \
	    CosCompoundLifeCycle.cc \
	    CosLifeCycleReference.cc \
	    CosLifeCycleContainment.cc \
	    FactoryFinder_impl.cc \
	    GenericFactory_impl.cc \
	    LifeRole_impl.cc \
	    LifeContainment_impl.cc \
	    LifeNode_impl.cc \
	    LifePropagationCriteriaFactory_impl.cc \
	    LifeReference_impl.cc \
	    LifeRelationship_impl.cc \
	    LifeTraversalCriteria_impl.cc \
	    Operations_impl.cc \
		RegisterHelper.cc

DAEMONS_SRCS = lifenoded.cc \
	       lifecontainmentd.cc \
	       lifereferenced.cc \
	       operationd.cc \
	       lifecriteriad.cc \
	       factoryfinderd.cc \
	       genericfactory.cc


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

prg: .depend lifenoded lifecontainmentd lifereferenced operationd lifecriteriad factoryfinderd genericfactory

operationd: operationd.o ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) operationd.o $(LDLIBS) -o operationd
	$(POSTLD) $@

lifenoded: lifenoded.o ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) lifenoded.o $(LDLIBS) -o lifenoded
	$(POSTLD) $@

lifecontainmentd: lifecontainmentd.o ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) lifecontainmentd.o $(LDLIBS) -o lifecontainmentd
	$(POSTLD) $@

lifereferenced: lifereferenced.o ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) lifereferenced.o  $(LDLIBS) -o lifereferenced
	$(POSTLD) $@

lifecriteriad: lifecriteriad.o ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) lifecriteriad.o $(LDLIBS) -o lifecriteriad
	$(POSTLD) $@

factoryfinderd: factoryfinderd.o ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) factoryfinderd.o $(LDLIBS) -o factoryfinderd
	$(POSTLD) $@

genericfactory: genericfactory.o ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) genericfactory.o $(LDLIBS) -o genericfactory
	$(POSTLD) $@

idlcompile: CosLifeCycle.cc CosCompoundLifeCycle.cc CosLifeCycleContainment.cc CosLifeCycleReference.cc

CosLifeCycle.h CosLifeCycle.cc: \
		../../include/coss/CosLifeCycle.idl $(IDL)
	$(IDL) $(IDLFLAGS) --name CosLifeCycle \
		../../include/coss/CosLifeCycle.idl
	cp CosLifeCycle.h ../../include/coss

CosCompoundLifeCycle.h CosCompoundLifeCycle.cc: \
		../../include/coss/CosCompoundLifeCycle.idl $(IDL)
	$(IDL) $(IDLFLAGS) --name CosCompoundLifeCycle \
		../../include/coss/CosCompoundLifeCycle.idl
	cp CosCompoundLifeCycle.h ../../include/coss

CosLifeCycleContainment.h CosLifeCycleContainment.cc: \
		../../include/coss/CosLifeCycleContainment.idl $(IDL)
	$(IDL) $(IDLFLAGS) --name CosLifeCycleContainment \
		../../include/coss/CosLifeCycleContainment.idl
	cp CosLifeCycleContainment.h ../../include/coss

CosLifeCycleReference.h CosLifeCycleReference.cc: \
		../../include/coss/CosLifeCycleReference.idl $(IDL)
	$(IDL) $(IDLFLAGS) --name CosLifeCycleReference \
		../../include/coss/CosLifeCycleReference.idl
	cp CosLifeCycleReference.h ../../include/coss


# cleaning

clean:
	-$(RM) *.o *.a *~ *.rpo *.objid lifenoded lifecontainmentd lifereferenced operationd lifecriteriad factoryfinderd genericfactory
	-$(RM) .depend TAGS core nohup.out
	-$(RM) CosLifeCycle.h CosLifeCycle.cc
	-$(RM) ../../include/coss/CosLifeCycle.h
	-$(RM) CosCompoundLifeCycle.h CosCompoundLifeCycle.cc
	-$(RM) ../../include/coss/CosCompoundLifeCycle.h
	-$(RM) CosLifeCycleContainment.h CosLifeCycleContainment.cc
	-$(RM) ../../include/coss/CosLifeCycleContainment.h
	-$(RM) CosLifeCycleReference.h CosLifeCycleReference.cc
	-$(RM) ../../include/coss/CosLifeCycleReference.h

install:
	$(IDIRCMD) $(INSTDIR)/bin
	$(IBINCMD) operationd $(INSTDIR)/bin
	$(IDIRCMD) $(INSTDIR)/bin
	$(IBINCMD) lifenoded $(INSTDIR)/bin
	$(IDIRCMD) $(INSTDIR)/bin
	$(IBINCMD) lifecontainmentd $(INSTDIR)/bin
	$(IDIRCMD) $(INSTDIR)/bin
	$(IBINCMD) lifereferenced $(INSTDIR)/bin


ifeq (.depend, $(wildcard .depend))
include .depend
endif

.depend :
	echo '# Module dependencies' > .depend
	$(MKDEPEND) $(CXXFLAGS) *.cc >> .depend

