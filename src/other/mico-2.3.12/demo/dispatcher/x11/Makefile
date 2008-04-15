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

DIR_PREFIX=../
include ../../MakeVars

ifeq ($(HAVE_CYGWIN), yes)
PLATFORM_LIBS = -lSM -lICE -lXpm   
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

all: .depend client server

CXXFLAGS  := $(X11_CXXFLAGS) $(CXXFLAGS)
LDLIBS    := $(X11_LDLIBS) $(LDLIBS)
LDFLAGS   := $(X11_LDFLAGS) $(LDFLAGS)
DEPS      := $(X11_DEPS) $(DEPS)

INSTALL_DIR     = dispatcher/x11
INSTALL_SRCS    = Makefile hello.idl client.cc server.cc
INSTALL_SCRIPTS = xhello

client: hello.o client.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) hello.o client.o $(LDLIBS) -o $@ $(PLATFORM_LIBS)

server: hello.o server.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) hello.o server.o $(LDLIBS) -o $@ $(PLATFORM_LIBS)

hello.h hello.cc : hello.idl $(IDLGEN)
	$(IDL) hello.idl

clean:
	rm -f hello.cc hello.h *.o core client server *~ .depend
