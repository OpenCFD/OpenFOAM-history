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

all: .depend dynany dynanytest

include ../MakeVars

CXXFLAGS  := -g $(AUX_CXXFLAGS) $(CXXFLAGS)
LDLIBS    := $(AUX_LDLIBS) $(LDLIBS)
LDFLAGS   := $(AUX_LDFLAGS) $(LDFLAGS)
DEPS      := $(AUX_DEPS) $(DEPS)

INSTALL_DIR     = dynany
INSTALL_SRCS    = Makefile test.idl dynany.cc dynanytest.cc
INSTALL_SCRIPTS =

dynany: test.o dynany.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) test.o dynany.o $(LDLIBS) -o $@

dynanytest: test.o dynanytest.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) test.o dynanytest.o $(LDLIBS) -o $@

test.h test.cc : test.idl $(IDLGEN)
	$(IDL) --any test.idl

run:
	dynanytest

clean:
	rm -f test.cc test.h *.o core dynany dynanytest .depend
