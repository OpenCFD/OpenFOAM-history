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
IDLFLAGS := -B../..

# generated files

OMG_SRCS = CosObjectIdentity.cc CosRelationships.cc CosGraphs.cc \
  CosContainment.cc CosReference.cc CosGraphsExtension.cc

IMPL_SRCS = IdentifiableObject_impl.cc Role_impl.cc Relationship_impl.cc \
  RelationshipIterator_impl.cc TraversalFactory_impl.cc Traversal_impl.cc \
  TraversalCriteria_impl.cc Node_impl.cc GraphsRole_impl.cc EdgeIterator_impl.cc \
  CosContainment_impl.cc CosReference_impl.cc \
  GenericCriteriaFactory_impl.cc

EXT_SRCS = CosGraphsExtension.cc RandomGenerator.cc random.cc

EXT_IMPL_SRCS = GenericCriteriaFactory_impl.cc RandomGenerator_impl.cc

UTIL_SRCS = InterfaceDefsHelper.cc NamedRoleTypesHelper.cc

#DAEMONS_SRCS = traversald.cc noded.cc containmentd.cc referenced.cc randomd.cc \
#  proxyd.cc

SRCS = $(OMG_SRCS) $(EXT_SRCS) $(IMPL_SRCS) $(EXT_IMPL_SRCS) $(UTIL_SRCS)

STATIC_OBJS = $(SRCS:.cc=.o)
SHARED_OBJS = $(SRCS:.cc=.pic.o)

# normal rules

all: lib prg

ifeq ($(HAVE_FINAL), no)
ifeq ($(HAVE_SHARED_EXCEPTS), yes)
ifeq ($(HAVE_STATIC), yes)
lib: .depend $(STATIC_OBJS) $(SHARED_OBJS)
else
lib: .depend $(SHARED_OBJS)
endif
else
lib: .depend $(STATIC_OBJS)
endif
else
lib:
endif

prg: .depend traversald noded containmentd referenced randomd proxyd

idlcompile: $(OMG_SRCS) $(EXT_SRCS)

traversald: traversald.o ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) traversald.o $(LDLIBS) -o traversald
	$(POSTLD) $@

noded: noded.o ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) noded.o $(LDLIBS) -o noded
	$(POSTLD) $@

containmentd: containmentd.o ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) containmentd.o $(LDLIBS) -o containmentd
	$(POSTLD) $@

referenced: referenced.o ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) referenced.o $(LDLIBS) -o referenced
	$(POSTLD) $@

randomd: randomd.o ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) randomd.o $(LDLIBS) -o randomd
	$(POSTLD) $@

proxyd: proxyd.o ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) proxyd.o $(LDLIBS) -o proxyd
	$(POSTLD) $@

CosObjectIdentity.h CosObjectIdentity.cc: \
		../../include/coss/CosObjectIdentity.idl $(IDL)
	$(IDL) $(IDLFLAGS) --name CosObjectIdentity \
		../../include/coss/CosObjectIdentity.idl
	cp CosObjectIdentity.h ../../include/coss

CosRelationships.h CosRelationships.cc: \
		../../include/coss/CosRelationships.idl $(IDL)
	$(IDL) $(IDLFLAGS) --name CosRelationships \
		../../include/coss/CosRelationships.idl
	cp CosRelationships.h ../../include/coss

CosGraphs.h CosGraphs.cc: \
		../../include/coss/CosGraphs.idl $(IDL)
	$(IDL) $(IDLFLAGS) --name CosGraphs \
		../../include/coss/CosGraphs.idl
	cp CosGraphs.h ../../include/coss

CosContainment.h CosContainment.cc: \
		../../include/coss/CosContainment.idl $(IDL)
	$(IDL) $(IDLFLAGS) --name CosContainment \
		../../include/coss/CosContainment.idl
	cp CosContainment.h ../../include/coss

CosReference.h CosReference.cc: \
		../../include/coss/CosReference.idl $(IDL)
	$(IDL) $(IDLFLAGS) --name CosReference \
		../../include/coss/CosReference.idl
	cp CosReference.h ../../include/coss

CosGraphsExtension.h CosGraphsExtension.cc: \
		../../include/coss/CosGraphsExtension.idl $(IDL)
	$(IDL) $(IDLFLAGS) --name CosGraphsExtension \
		../../include/coss/CosGraphsExtension.idl
	cp CosGraphsExtension.h ../../include/coss

RandomGenerator.h RandomGenerator.cc: \
		../../include/coss/RandomGenerator.idl $(IDL)
	$(IDL) $(IDLFLAGS) --name RandomGenerator \
		../../include/coss/RandomGenerator.idl
	cp RandomGenerator.h ../../include/coss

random.h random.cc: \
		../../include/coss/random.idl $(IDL)
	$(IDL) $(IDLFLAGS) --name random \
		../../include/coss/random.idl
	cp random.h ../../include/coss


# cleaning

clean:
	-$(RM) *.o *.a *~ *.rpo *.objid traversald noded containmentd referenced randomd proxyd
	-$(RM) .depend TAGS core nohup.out
	-$(RM) CosObjectIdentity.h CosObjectIdentity.cc
	-$(RM) ../../include/coss/CosObjectIdentity.h
	-$(RM) CosRelationships.h CosRelationships.cc
	-$(RM) ../../include/coss/CosRelationships.h
	-$(RM) CosGraphs.h CosGraphs.cc
	-$(RM) ../../include/coss/CosGraphs.h
	-$(RM) CosContainment.h CosContainment.cc
	-$(RM) ../../include/coss/CosContainment.h
	-$(RM) CosReference.h CosReference.cc
	-$(RM) ../../include/coss/CosReference.h
	-$(RM) CosGraphsExtension.h CosGraphsExtension.cc
	-$(RM) ../../include/coss/CosGraphsExtension.h
	-$(RM) RandomGenerator.h RandomGenerator.cc
	-$(RM) ../../include/coss/RandomGenerator.h
	-$(RM) random.h random.cc
	-$(RM) ../../include/coss/random.h

install:
	$(IDIRCMD) $(INSTDIR)/bin
	$(IBINCMD) traversald $(INSTDIR)/bin
	$(IDIRCMD) $(INSTDIR)/bin
	$(IBINCMD) noded $(INSTDIR)/bin
	$(IDIRCMD) $(INSTDIR)/bin
	$(IBINCMD) containmentd $(INSTDIR)/bin
	$(IDIRCMD) $(INSTDIR)/bin
	$(IBINCMD) referenced $(INSTDIR)/bin
	$(IDIRCMD) $(INSTDIR)/bin
	$(IBINCMD) randomd $(INSTDIR)/bin
	$(IDIRCMD) $(INSTDIR)/bin
	$(IBINCMD) proxyd $(INSTDIR)/bin


ifeq (.depend, $(wildcard .depend))
include .depend
endif

.depend :
	echo '# Module dependencies' > .depend
	$(MKDEPEND) $(CXXFLAGS) *.cc >> .depend

