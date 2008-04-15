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

all: .depend client server

include ../MakeVars

CXXFLAGS := $(CXXFLAGS) -O

INSTALL_DIR     = generic
INSTALL_SRCS    = Makefile client.cc server.cc test.idl test_cases
INSTALL_SCRIPTS = run_test

client: client.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) client.o $(LDLIBS) -o $@

server: server.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) server.o $(LDLIBS) -o $@

run:
	run_test

clean:
	rm -f *.o *.ior core client server *~ .depend
