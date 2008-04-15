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

all: .depend TestServer DocumentsServer simple-test

DIR_PREFIX=../
include ../../MakeVars

CXXFLAGS  := $(COS_CXXFLAGS) $(CXXFLAGS)
LDLIBS    := $(COS_LDLIBS) $(LDLIBS)
LDFLAGS   := $(COS_LDFLAGS) $(LDFLAGS)
DEPS      := $(COS_DEPS) $(DEPS)

INSTALL_DIR     = services/externalize
INSTALL_SRCS    = Makefile Test.idl Documents.idl Test_impl.cc Test_impl.h \
  Documents_impl.cc Documents_impl.h README simple-test.cc DocumentsServer.cc \
  TestServer.cc
INSTALL_SCRIPTS = daemons-start daemons-stop servers-start stop-servers test \
  rundemo

Test.o: Test.cc Test.h

Test.cc Test.h: Test.idl
	$(IDL) $(IDLFLAGS) --no-boa --poa Test.idl

Documents.cc Documents.h: Documents.idl
	$(IDL) $(IDLFLAGS) --no-boa --poa Documents.idl


TestServer: Test.o Test_impl.o TestServer.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) TestServer.o Test.o Test_impl.o \
		$(LDLIBS) -o $@

DocumentsServer: Documents.o Documents_impl.o DocumentsServer.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) DocumentsServer.o Documents.o \
		Documents_impl.o $(LDLIBS) -o $@

simple-test: Test.o Documents.o simple-test.o $(DEPS)
	 $(LD) $(CXXFLAGS) $(LDFLAGS) simple-test.o Test.o Documents.o \
		$(LDLIBS) -o $@

clean:
	-$(RM) .depend *.o *.ext core *~ TestServer DocumentsServer Documents.cc
	-$(RM) Documents.h Test.cc Test.h ir-db.idl ird.pid simple-test





