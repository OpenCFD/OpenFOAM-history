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

all: .depend account_svr Account.class Client.class ClientApplet.class

DIR_PREFIX=../
include ../../MakeVars

INSTALL_DIR     = interop/visibroker
INSTALL_SRCS    = Makefile Account.idl Client.java ClientApplet.html.in \
  ClientApplet.java README main.cc
INSTALL_SCRIPTS = account run_applet

account_svr: Account.h Account.o main.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) Account.o main.o $(LDLIBS) -o account_svr

Account.h Account.cc : Account.idl $(IDLGEN)
	$(IDL) --no-poa --boa Account.idl

Account.java : Account.idl
	idl2java Account.idl

clean:
	rm -f Account.cc Account.h .depend *.o core account_svr *~ \
	      Account.java *Helper.* *Holder.* *.class \
	      ClientApplet.html account.ref *Operations.* _*

