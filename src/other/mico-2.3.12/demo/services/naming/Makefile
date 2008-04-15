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

DIR_PREFIX=../
include ../../MakeVars

CXXFLAGS  := $(COS_CXXFLAGS) $(CXXFLAGS)
LDLIBS    := $(COS_LDLIBS) $(LDLIBS)
LDFLAGS   := $(COS_LDFLAGS) $(LDFLAGS)
DEPS      := $(COS_DEPS) $(DEPS)

INSTALL_DIR     = services/naming
INSTALL_SRCS    = Makefile account.idl client.cc server.cc
INSTALL_SCRIPTS = account_test

client: account.h account.o client.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) account.o client.o $(LDLIBS) -o $@

server: account.h account.o server.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) account.o server.o $(LDLIBS) -o $@

account.h account.cc : account.idl $(IDLGEN)
	$(IDL) account.idl

clean:
	rm -f .depend account.cc account.h *.o core client server *~
