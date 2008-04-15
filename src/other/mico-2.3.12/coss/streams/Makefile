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

include ../../MakeVars

CXXFLAGS := -I. -I../../include $(CXXFLAGS) $(EHFLAGS)
IDLFLAGS := -B../..

# normal rules

all: lib prg

ifeq ($(HAVE_FINAL), no)
ifeq ($(HAVE_SHARED_EXCEPTS), yes)
ifeq ($(HAVE_STATIC), yes)
lib: .depend stream.o stream_impl.o stream.pic.o stream_impl.pic.o
else
lib: .depend stream.pic.o stream_impl.pic.o
endif
else
lib: .depend stream.o stream_impl.o
endif
else
lib:
endif

prg: .depend

idlcompile: stream.cc

stream.h stream.cc: ../../include/coss/stream.idl $(IDL)
	$(IDL) $(IDLFLAGS) --no-poa --boa --name stream \
		../../include/coss/stream.idl
	cp stream.h ../../include/coss

# cleaning

clean:
	-$(RM) *.o *.a *~ *.rpo *.objid eventd
	-$(RM) .depend TAGS core nohup.out
	-$(RM) stream.cc stream.h
	-$(RM) ../../include/coss/stream.h

install:

ifeq (.depend, $(wildcard .depend))
include .depend
endif

.depend :
	echo '# Module dependencies' > .depend
	$(MKDEPEND) $(CXXFLAGS) *.cc >> .depend
