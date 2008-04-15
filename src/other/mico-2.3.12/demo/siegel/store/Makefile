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

all: .depend store_server

DIR_PREFIX=../
include ../../MakeVars

INSTALL_DIR     = siegel/store
INSTALL_SRCS    = Makefile StoreAccessImpl.cc StoreImpl.cc TaxImpl.cc \
  store_server.cc StoreAccessImpl.h StoreImpl.h TaxImpl.h
INSTALL_SCRIPTS =

OBJS            = StoreImpl.o TaxImpl.o StoreAccessImpl.o
AUX_OBJS        = ../idl/POS.o ../idl/Store.o ../idl/Central.o \
                  ../primer_utils/NsPublisher.o

CXXFLAGS  := $(COS_CXXFLAGS) $(CXXFLAGS) -I../idl -I../primer_utils
LDLIBS    := $(COS_LDLIBS) $(LDLIBS)
LDFLAGS   := $(COS_LDFLAGS) $(LDFLAGS)
DEPS      := $(COS_DEPS) $(DEPS)


store_server: store_server.o $(OBJS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) store_server.o $(OBJS) \
              $(AUX_OBJS) $(LDLIBS) -o store_server

clean:
	rm -f *.o core store_server *~ .depend
