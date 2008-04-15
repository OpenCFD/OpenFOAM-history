#
# MICO --- an Open Source CORBA implementation
# Copyright (c) 1997-2002 by The Mico Team
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
# For more information, visit the MICO Home Page at
# http://www.mico.org/
#

include ../MakeVars

LDFLAGS  := -L../orb $(LDFLAGS)
CXXFLAGS := -I. -I../include $(PCHFLAGS) $(CXXFLAGS)
LDLIBS   := $(LDMICO) $(CONFLIBS)
IDLFLAGS := -B..

#ifeq ($HAVE_THREADS), yes)
MICODOBJS=poamediator.o main.o imr_impl.o
#else
#MICODOBJS=mediator.o poamediator.o main.o imr_impl.o
#endif
IMRCLOBJS=imr_client.o

# normal rules

all: prg

prg: .depend micod imr

micod: $(MICODOBJS) ../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(MICODOBJS) $(LDLIBS) -o $@
	$(POSTLD) $@

imr: $(IMRCLOBJS) ../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(IMRCLOBJS) $(LDLIBS) -o $@
	$(POSTLD) $@


generate:
	cd ../orb; mv imr.cc imr.cc.backup
	cd ../include/mico; mv imr.h imr.h.backup
	$(IDL) $(IDLFLAGS) --any --name imr --windows-dll mico \
		--mico-core ../include/mico/imr.idl
	mv imr.h ../include/mico
	mv imr.cc ../orb

ungenerate:
	mv ../orb/imr.cc.backup ../orb/imr.cc
	mv ../include/mico/imr.h.backup ../include/mico/imr.h

diffs:
	mkdir orig; cd orig; $(IDL) $(IDLFLAGS) --poa --any \
		--name imr --windows-dll mico --mico-core \
		../../include/mico/imr.idl
	$(RM) imr.cc.diffs
	cd ../include/mico; diff -c ../../daemon/orig/imr.h imr.h \
	  > ../../daemon/imr.cc.diffs || true
	cd ../orb; diff -c ../daemon/orig/imr.cc imr.cc \
	  >> imr.cc.diffs || true
	$(RM) -rf orig

# cleaning

clean:
	-$(RM) micod imr
	-$(RM) *.o *.o0 *.a *~ *.$(SOEXT) *.exe *.rpo *.ii *.ti *.objid
	-$(RM) .depend TAGS core nohup.out


install: all
	$(IDIRCMD) $(INSTDIR)/bin
	$(IBINCMD) micod $(INSTDIR)/bin
	$(IBINCMD) imr $(INSTDIR)/bin

install-cd: install

# dependencies

ifeq (.depend, $(wildcard .depend))
include .depend
endif

.depend:
	echo '# Module dependencies' > .depend
	$(MKDEPEND) $(CPPFLAGS) $(CXXFLAGS) *.cc >> .depend

