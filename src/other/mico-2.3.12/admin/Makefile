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

include ../MakeVars

CXXFLAGS := -I../include $(CXXFLAGS)


all: adm

adm: mkdepend

mkdepend: mkdepend.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $< $(CONFLIBS) -o $@

install:
	$(IDIRCMD) $(INSTDIR)/lib
	$(ISCRIPTCMD) mico-setup.sh $(INSTDIR)/lib
	$(ISCRIPTCMD) mico-setup.csh $(INSTDIR)/lib
	$(IDIRCMD) $(INSTDIR)/bin
	test -f mico-c++   && $(ISCRIPTCMD) mico-c++   $(INSTDIR)/bin
	test -f mico-ld    && $(ISCRIPTCMD) mico-ld    $(INSTDIR)/bin
	test -f mico-ar    && $(ISCRIPTCMD) mico-ar    $(INSTDIR)/bin
ifeq ($(HAVE_DYNAMIC), yes)
	test -f mico-shc++ && $(ISCRIPTCMD) mico-shc++ $(INSTDIR)/bin
	test -f mico-shld  && $(ISCRIPTCMD) mico-shld  $(INSTDIR)/bin 
endif
	$(ISCRIPTCMD) mkdepend $(INSTDIR)/bin
	test -f mico-config && $(ISCRIPTCMD) mico-config $(INSTDIR)/bin

install-cd: install
	$(ISCRIPTCMD) mico-setup.sh.in $(INSTDIR)/lib
	$(ISCRIPTCMD) mico-setup.csh.in $(INSTDIR)/lib

clean:
#	rm -f mico-setup.sh mico-setup.csh
#	rm -f mico-c++ mico-ld mico-ar mico-shc++ mico-shld
	rm -f mkdepend *.o *.rpo *.ii *.ti
