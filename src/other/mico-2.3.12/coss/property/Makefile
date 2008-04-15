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

STATIC_OBJS = PropertyService.o PropertyService_impl.o
SHARED_OBJS = PropertyService.pic.o PropertyService_impl.pic.o

SRV_OBJS = main.o

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

prg: .depend propertyd

idlcompile: PropertyService.cc

propertyd: $(SRV_OBJS) ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(SRV_OBJS) $(LDLIBS) -o propertyd
	$(POSTLD) $@

PropertyService.h PropertyService.cc: ../../include/coss/PropertyService.idl $(IDL)
	$(IDL) $(IDLFLAGS) --any --name PropertyService \
	       ../../include/coss/PropertyService.idl
	cp PropertyService.h ../../include/coss


# cleaning

clean:
	-$(RM) *.o *.a *~ *.rpo *.objid
	-$(RM) .depend TAGS core nohup.out
	-$(RM) PropertyService.h PropertyService.cc ../../include/coss/PropertyService.h
	-$(RM) propertyd

install:
	$(IDIRCMD) $(INSTDIR)/bin
	$(IBINCMD) propertyd $(INSTDIR)/bin

ifeq (.depend, $(wildcard .depend))
include .depend
endif

.depend :
	echo '# Module dependencies' > .depend
	$(MKDEPEND) $(CXXFLAGS) *.cc >> .depend
