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

CXXFLAGS := -I. -I../../include $(CXXFLAGS) $(EHFLAGS)
LDFLAGS  := -L../../orb $(LDFLAGS)
LDLIBS   := -lmico$(VERSION) $(CONFLIBS)

ifeq ($(HAVE_EXCEPTIONS), yes)
all: .depend diitest anytest transp diiclient diiserver siiclient siiserver \
  exc exs static
else
all: .depend diitest anytest transp diiclient diiserver  siiclient siiserver \
  static
endif

diitest: diitest.o ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) diitest.o $(LDLIBS) -o $@
	$(POSTLD) $@

diiclient: diiclient.o ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) diiclient.o $(LDLIBS) -o $@
	$(POSTLD) $@

diiserver: diiserver.o ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) diiserver.o $(LDLIBS) -o $@
	$(POSTLD) $@

siiclient: siiclient.o ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) siiclient.o $(LDLIBS) -o $@
	$(POSTLD) $@

siiserver: siiserver.o ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) siiserver.o $(LDLIBS) -o $@
	$(POSTLD) $@

anytest: anytest.o ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) anytest.o $(LDLIBS) -o $@
	$(POSTLD) $@

static: types.o static.o ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) static.o types.o $(LDLIBS) -o $@
	$(POSTLD) $@

subtype.cc subtype.h : subtype.idl
	$(IDL) subtype.idl

types.cc types.h : types.idl
	$(IDL) --typecode types.idl

ex.cc ex.h : ex.idl
	$(IDL) ex.idl

transp: transp.o ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) transp.o $(LDLIBS) -o $@
	$(POSTLD) $@

exc: ex.o ex_client.o ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) ex.o ex_client.o $(LDLIBS) -o $@
	$(POSTLD) $@

exs: ex.o ex_server.o ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) ex.o ex_server.o $(LDLIBS) -o $@
	$(POSTLD) $@

clean:
	rm -f transp diiclient diiserver diitest anytest exc exs siiclient \
	  siiserver static *.o *~ .depend *.objid subtype.cc subtype.h \
	  types.cc types.h ex.cc ex.h

check:
	@echo
	@echo "NOTE: dii tests _still_ need to be reorganized and many of them"
	@echo "      need to be fixed, although some are runable. This is just"
	@echo "      the first step on the long way to have them all running."
	@echo
	@echo
	-anytest
	@echo
	-static
	@echo
	-diirun
	@echo
	-exrun
	@echo
	-siirun
	@echo
	-transprun

ifeq (.depend, $(wildcard .depend))
include .depend
endif

.depend:
	echo "# module dependencies" > .depend
	$(MKDEPEND) $(CXXFLAGS) *.cc >> .depend
