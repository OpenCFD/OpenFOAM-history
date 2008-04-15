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
ALLLIBS   = $(LDMICOCOSS) $(LDMICO) $(CURSESLIBS)
LDFLAGS  := -L.. -L../../orb $(LDFLAGS)
CXXFLAGS := -I. -I../../include $(CXXFLAGS) $(EHFLAGS)
IDLFLAGS := -B../..

# generated files

CLNT_OBJS = NamingClient.o nsadmin.o
SRV_OBJS  = Naming_impl.o nsd.o CosNaming_skel.o

# normal rules

all: lib prg

ifeq ($(HAVE_FINAL), no)
ifeq ($(HAVE_SHARED_EXCEPTS), yes)
ifeq ($(HAVE_STATIC), yes)
lib: .depend CosNaming.o CosNaming.pic.o
else
lib: .depend CosNaming.pic.o
endif
else
lib: .depend CosNaming.o
endif
else
lib:
endif

prg: .depend nsd nsadmin

idlcompile: CosNaming.cc CosNaming_skel.cc

nsd: $(SRV_OBJS) ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(SRV_OBJS) $(LDLIBS) -o nsd
	$(POSTLD) $@

nsadmin: $(CLNT_OBJS) ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(CLNT_OBJS) $(ALLLIBS) -o nsadmin
	$(POSTLD) $@


CosNaming.h CosNaming.cc CosNaming_skel.cc .NOTPARALLEL: ../../include/coss/CosNaming.idl $(IDL)
	$(IDL) $(IDLFLAGS) --c++-skel --name CosNaming \
		../../include/coss/CosNaming.idl
	cp CosNaming.h ../../include/coss

# cleaning

clean:
	-$(RM) *.o *.a *~ *.rpo *.ii *.ti *.objid nsd nsadmin
	-$(RM) .depend TAGS core nohup.out
	-$(RM) CosNaming.h CosNaming.cc ../../include/coss/CosNaming.h \
		CosNaming_skel.cc

install:
	$(IDIRCMD) $(INSTDIR)/bin
	$(IBINCMD) nsd $(INSTDIR)/bin
	$(IBINCMD) nsadmin $(INSTDIR)/bin

ifeq (.depend, $(wildcard .depend))
include .depend
endif

.depend :
	echo '# Module dependencies' > .depend
	$(MKDEPEND) $(CXXFLAGS) *.cc >> .depend
