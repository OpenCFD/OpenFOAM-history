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


DIR_PREFIX=../
include ../../MakeVars

INSTALL_DIR     = siegel/idl
INSTALL_SRCS    = Makefile Central.idl POS.idl Store.idl
INSTALL_SCRIPTS =

all: POS.o Store.o Central.o

Central.h Central.cc : Central.idl $(IDLGEN)
	$(IDL) Central.idl

POS.h POS.cc : POS.idl $(IDLGEN)
	$(IDL) POS.idl

Store.h Store.cc : Store.idl $(IDLGEN)
	$(IDL) Store.idl

clean:
	rm -f *.h *.cc *.o core *~ .depend
