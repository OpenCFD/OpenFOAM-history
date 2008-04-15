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

all: .depend set_properties get_properties

DIR_PREFIX=../
include ../../MakeVars

CXXFLAGS  := $(COS_CXXFLAGS) $(CXXFLAGS)
LDLIBS    := $(COS_LDLIBS) $(LDLIBS)
LDFLAGS   := $(COS_LDFLAGS) $(LDFLAGS)
DEPS      := $(COS_DEPS) $(DEPS)

INSTALL_DIR     = services/property-daemon
INSTALL_SRCS    = Makefile set_properties.cc get_properties.cc bank.idl README
INSTALL_SCRIPTS = start_demo

bank.h bank.cc: bank.idl
	$(IDL) $(IDLFLAGS) --any bank.idl

set_properties: set_properties.o bank.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) set_properties.o bank.o $(LDLIBS) -o $@

get_properties: get_properties.o bank.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) get_properties.o bank.o $(LDLIBS) -o $@

clean:
	rm -f .depend *.o bank.cc bank.h core get_properties set_properties *~

set_properties.o get_properties.o: bank.h
