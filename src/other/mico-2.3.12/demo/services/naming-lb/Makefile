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

all: .depend Server Client

DIR_PREFIX=../
include ../../MakeVars

CXXFLAGS  := $(COS_CXXFLAGS) $(CXXFLAGS)
LDLIBS    := $(COS_LDLIBS) $(LDLIBS)
LDFLAGS   := $(COS_LDFLAGS) $(LDFLAGS)
DEPS      := $(COS_DEPS) $(DEPS)

INSTALL_DIR     = services/naming-lb
INSTALL_SRCS    = Makefile Printer.idl Printer_impl.h Printer_impl.cc Client.cc Server.cc
INSTALL_SCRIPTS = printer_test

Printer.o : Printer.h Printer.cc

Printer_impl.o : Printer_impl.h Printer_impl.cc Printer.h Printer.cc
	$(CXX) $(CXXFLAGS) -c Printer_impl.cc -o Printer_impl.o

Server.o : Printer.h Server.cc

Client.o : Printer.h Client.cc

Server : Printer.o Printer_impl.o Server.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) Printer.o Printer_impl.o Server.o $(LDLIBS) $(COS_LDLIBS) -o Server

Client : Printer.o Client.o
	$(LD) $(CXXFLAGS) $(LDFLAGS) Printer.o Client.o $(LDLIBS) $(COS_LDLIBS) -o Client

Printer.h Printer.cc : Printer.idl $(IDLGEN)
	$(IDL) Printer.idl

clean:
	rm -f Printer.cc Printer.h *.o core Server Client *~ .depend
