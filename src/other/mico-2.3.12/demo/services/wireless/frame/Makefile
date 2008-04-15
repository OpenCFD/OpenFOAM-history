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

all: .depend client server cliser gremlin frame consumer

DIR_PREFIX=../../
include ../../../MakeVars

CXXFLAGS := $(COS_CXXFLAGS) $(CXXFLAGS)
LDLIBS   := $(COS_LDLIBS) $(LDLIBS)
LDFLAGS  := $(COS_LDFLAGS) $(LDFLAGS)
DEPS     := $(COS_DEPS) $(DEPS)

INSTALL_DIR     = services/wireless/frame
INSTALL_SRCS    = Makefile demo.idl client.cc server.cc cliser.cc gremlin.cc frame.cc consumer.cc
INSTALL_SCRIPTS = ab0 ab1 demoevd demohla demons demotb netcli netcons netproxy netser tercli tercons terproxy terser hlarc micorc

server: demo.o server.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) demo.o server.o $(LDLIBS) -o server

cliser: demo.o cliser.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) demo.o cliser.o $(LDLIBS) -o cliser

client: demo.o client.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) demo.o client.o $(LDLIBS) -o client

frame: demo.o frame.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) demo.o frame.o $(LDLIBS) -o frame

gremlin: gremlin.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) gremlin.o $(LDLIBS) -o gremlin

consumer: consumer.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) consumer.o $(LDLIBS) -o consumer

%.h %.cc: %.idl
	$(IDL) $(IDLFLAGS) $<

clean:
	rm -f demo.cc demo.h *.ior *.ref *.o core *~ .depend
	rm -f consumer client server cliser gremlin frame
