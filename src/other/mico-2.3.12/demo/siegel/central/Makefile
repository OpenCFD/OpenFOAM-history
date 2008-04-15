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

all: .depend depot_server

DIR_PREFIX=../
include ../../MakeVars

INSTALL_DIR     = siegel/central
INSTALL_SRCS    = Makefile DepotData.h DepotImpl.h TreeNode.h depot_server.cc \
  DepotData.cc DepotImpl.cc Makefile depot.dat
INSTALL_SCRIPTS =

AUX_OBJS        = ../idl/POS.o ../idl/Store.o ../idl/Central.o \
                  ../primer_utils/NsPublisher.o

CXXFLAGS  := $(COS_CXXFLAGS) $(CXXFLAGS) -I../idl -I../primer_utils
LDLIBS    := $(COS_LDLIBS) $(LDLIBS)
LDFLAGS   := $(COS_LDFLAGS) $(LDFLAGS)
DEPS      := $(COS_DEPS) $(DEPS)


depot_server: depot_server.o DepotData.o DepotImpl.o
	$(LD) $(CXXFLAGS) $(LDFLAGS) depot_server.o DepotData.o DepotImpl.o \
              $(AUX_OBJS) $(LDLIBS) -o depot_server

clean:
	rm -f *.o core depot_server *~ .depend
