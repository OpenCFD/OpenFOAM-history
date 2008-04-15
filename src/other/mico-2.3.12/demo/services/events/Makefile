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

all: .depend server server2 server3 client client2

DIR_PREFIX=../
include ../../MakeVars

CXXFLAGS  := $(COS_CXXFLAGS) $(CXXFLAGS)
LDLIBS    := $(COS_LDLIBS) $(LDLIBS)
LDFLAGS   := $(COS_LDFLAGS) $(LDFLAGS)
DEPS      := $(COS_DEPS) $(DEPS)

INSTALL_DIR     = services/events
INSTALL_SRCS    = Makefile client.cc client2.cc server.cc server2.cc server3.cc
INSTALL_SCRIPTS = runit

server: $(OBJS) server.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) server.o $(LDLIBS) -o $@

server2: $(OBJS) server2.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) server2.o $(LDLIBS) -o $@

server3: $(OBJS) server3.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) server3.o $(LDLIBS) -o $@

client: $(OBJS) client.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) client.o $(LDLIBS) -o $@

client2: $(OBJS) client2.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) client2.o $(LDLIBS) -o $@

clean:
	rm -f .depend *.o core server server2 client client2 server3 *~
