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

# delete rules for .y defined in ../MakeVars
%.c: %.y


LDFLAGS  := -L. $(LDFLAGS)
LDLIBS   :=
CFLAGS   := -DHAVE_CONFIG_H -I. -I../include $(CFLAGS)

# generated files

GENERATED=cexp.h cexp.output

OBJS=cexp.o cccp.o config.o alloca.o

# normal rules

all: lib prg

prg: .depend mico-cpp

lib: .depend

mico-cpp: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@
	$(POSTLD) $@

# cleaning

clean:
	-$(RM) *.o *.a *~ *.rpo mico-cpp
	-$(RM) .depend TAGS core nohup.out $(GENERATED)


install: all
	$(IDIRCMD) $(INSTDIR)/bin
	$(IBINCMD) mico-cpp $(INSTDIR)/bin

install-cd: install

generate:
	$(BISON) -d -t -v -o cexp.c cexp.y


# dependencies

ifeq (.depend, $(wildcard .depend))
include .depend
endif

.depend :
	echo '# Module dependencies' > .depend
	$(MKDEPEND) $(CPPFLAGS) $(CXXFLAGS) *.c >> .depend

