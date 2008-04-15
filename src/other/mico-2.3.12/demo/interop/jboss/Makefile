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

DIRS=jdk1.3.x jdk1.4.x

all:

DIR_PREFIX=../

include ../../MakeVars

clean:
	for i in $(DIRS); do $(MAKE) -C $$i clean || exit 1; done

install:
	$(IDIRCMD) $(INSTDIR)/doc/mico/examples/interop/jboss
	$(ISRCCMD) Makefile $(INSTDIR)/doc/mico/examples/interop/jboss
	$(ISRCCMD) README $(INSTDIR)/doc/mico/examples/interop/jboss
	for i in $(DIRS); do $(MAKE) -C $$i install || exit 1; done
