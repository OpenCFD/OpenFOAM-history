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

include ../MakeVars

PLATFORM_LIBS =
ifeq ($(HAVE_CYGWIN), yes)
ifeq ($(USE_CSL2), yes)
PLATFORM_LIBS := $(PLATFORM_LIBS) -lssl -lcrypto
else
ifeq ($(USE_CSIV2), yes)
PLATFORM_LIBS := $(PLATFORM_LIBS) -lssl -lcrypto
else
ifeq ($(HAVE_SSL), yes)
PLATFORM_LIBS := $(PLATFORM_LIBS) -lssl -lcrypto
endif
endif
endif
endif

all: all_target

all_target: .depend client server.$(SOEXT)

INSTALL_DIR     = shlib
INSTALL_SRCS    = Makefile client.cc server.cc hello.idl
INSTALL_SCRIPTS = hello

client: hello.pic.o client.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) hello.pic.o client.o $(LDLIBS) -o $@ $(PLATFORM_LIBS)

server.$(SOEXT): server.pic.o hello.pic.o $(DEPS)
	rm -f $@
	$(LDSO) -o server hello.pic.o server.pic.o $(LDFLAGS) -lmico$(VERSION) $(PLATFORM_LIBS)

hello.h hello.cc : hello.idl $(IDLGEN)
	$(IDL) --no-poa --boa hello.idl

clean:
	rm -f hello.cc hello.h *.o *.$(SOEXT) core client *~ .depend
