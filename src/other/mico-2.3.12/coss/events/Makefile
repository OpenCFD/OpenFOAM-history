#
#  MICO --- an Open Source CORBA implementation
#  Copyright (c) 1997-2001 by The Mico Team
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
#  For more information, visit the MICO Home Page at
#  http://www.mico.org/
#

include ../../MakeVars

LDLIBS    = $(LDMICOCOSS) $(LDMICO) $(CONFLIBS)
LDFLAGS  := -L.. -L.. -L../../orb $(LDFLAGS)
CXXFLAGS := -I. -I../../include $(CXXFLAGS) $(EHFLAGS)
IDLFLAGS := -B../..

# generated files

SRV_OBJS=CosEvent_impl.o main.o

# normal rules

all: lib prg

ifeq ($(HAVE_FINAL), no)
ifeq ($(HAVE_SHARED_EXCEPTS), yes)
ifeq ($(HAVE_STATIC), yes)
lib: .depend CosEventComm.o CosEventComm.pic.o \
             CosEventChannelAdmin.o CosEventChannelAdmin.pic.o
else
lib: .depend CosEventComm.pic.o \
             CosEventChannelAdmin.pic.o
endif
else
lib: .depend CosEventComm.o CosEventChannelAdmin.o
endif
else
lib:
endif

prg: .depend eventd

idlcompile: CosEventComm.cc CosEventChannelAdmin.cc

eventd: $(SRV_OBJS) ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(SRV_OBJS) $(LDLIBS) -o eventd
	$(POSTLD) $@

CosEventComm.h CosEventComm.cc: ../../include/coss/CosEventComm.idl $(IDL)
	$(IDL) $(IDLFLAGS) --name CosEventComm \
		../../include/coss/CosEventComm.idl
	cp CosEventComm.h ../../include/coss


CosEventChannelAdmin.h CosEventChannelAdmin.cc: \
		../../include/coss/CosEventChannelAdmin.idl $(IDL)
	$(IDL) $(IDLFLAGS) --name CosEventChannelAdmin \
		../../include/coss/CosEventChannelAdmin.idl
	cp CosEventChannelAdmin.h ../../include/coss

# cleaning

clean:
	-$(RM) *.o *.a *~ *.rpo *.objid eventd
	-$(RM) .depend TAGS core nohup.out
	-$(RM) CosEventChannelAdmin.h CosEventChannelAdmin.cc 
	-$(RM) ../../include/coss/CosEventChannelAdmin.h
	-$(RM) CosEventComm.h CosEventComm.cc 
	-$(RM) ../../include/coss/CosEventComm.h

install:
	$(IDIRCMD) $(INSTDIR)/bin
	$(IBINCMD) eventd $(INSTDIR)/bin

ifeq (.depend, $(wildcard .depend))
include .depend
endif

.depend :
	echo '# Module dependencies' > .depend
	$(MKDEPEND) $(CXXFLAGS) *.cc >> .depend
