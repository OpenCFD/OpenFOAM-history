#
# MICO --- a CORBA 2.0 implementation
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

include ../../../MakeVars

IDLFILE1 = derived
IDLFILE2 = base

CXXFLAGS := -I. -I../../../include $(CXXFLAGS) $(EHFLAGS)
LDLIBS    = -lmico$(VERSION) $(CONFLIBS)
LDFLAGS  := -L../../../orb $(LDFLAGS)

all: .depend client server

client: $(IDLFILE1).h $(IDLFILE1).o $(IDLFILE2).h $(IDLFILE2).o \
          client.o base_impl.o derived_impl.o ../../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(IDLFILE1).o $(IDLFILE2).o \
            client.o base_impl.o derived_impl.o $(LDLIBS) -o client
	$(POSTLD) $@

server: $(IDLFILE2).h $(IDLFILE2).o server.o base_impl.o ../../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(IDLFILE2).o server.o \
            base_impl.o $(LDLIBS) -o server
	$(POSTLD) $@

base_impl.o : $(IDLFILE2).h base_impl.cc
derived_impl.o : $(IDLFILE1).h derived_impl.cc

$(IDLFILE1).h $(IDLFILE1).cc : $(IDLFILE1).idl $(IDLFILE2).h $(IDLGEN)
	$(IDL) $(IDLFILE1).idl

$(IDLFILE2).h $(IDLFILE2).cc : $(IDLFILE2).idl $(IDLGEN)
	$(IDL) $(IDLFILE2).idl

clean:
	rm -f $(IDLFILE1).cc $(IDLFILE1).h $(IDLFILE2).cc $(IDLFILE2).h \
	      .depend *.o core client server *~

ifeq (.depend, $(wildcard .depend))
include .depend
endif

.depend :
	echo '# Module dependencies' > .depend
	$(MKDEPEND) $(CXXFLAGS) *.cc >> .depend

