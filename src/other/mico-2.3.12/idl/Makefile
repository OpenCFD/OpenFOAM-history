#
# MICO --- an Open Source CORBA implementation
# Copyright (c) 1997-2003 by The Mico Team
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
# For more information, visit the MICO Home Page at
# http://www.mico.org/
#

include ../MakeVars

# delete rules for .ll and .yy defined in ../MakeVars
%.cc: %.ll
%.cc: %.yy


LDFLAGS  := -L. -L../orb -L../ir $(LDFLAGS)
CXXFLAGS := -I. -I../include $(PCHFLAGS) $(CXXFLAGS) $(EHFLAGS)
CFLAGS   := -DHAVE_CONFIG_H -I. -I../cpp -I../include $(CFLAGS)
LDLIBS   := $(LDMICOIR) $(LDMICO) $(CONFLIBS)

# generated files

GENERATED=yacc.output

ifeq ($(HAVE_FINAL), no)
OBJS=params.o parser.o yacc.o scanner.o parsenode.o idlparser.o \
     ir-copy.o codegen.o codegen-c++-util.o codegen-c++-common.o \
     codegen-c++-stub.o codegen-c++-skel.o codegen-c++-impl.o \
     codegen-c++.o codegen-idl.o codegen-midl.o dep.o error.o const.o \
     db.o prepro.o keymap.o codegen-wsdl.o ../cpp/alloca.o
else
OBJS=idl_all.o yacc.o ../cpp/alloca.o
endif

ifeq ($(HAVE_FINAL), no)
ifeq ($(USE_CCM), yes)
OBJS := $(OBJS) ccm-transform.o
endif
endif

# normal rules

all: lib prg

prg: .depend idl

lib: .depend libidl.a $(OBJS)

libidl.a: $(OBJS)
ifeq ($(HAVE_REPO), yes)
	-$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(OBJS) $(LDFLAGS) $(LDLIBS)
endif
ifeq ($(HAVE_PRELINK), yes)
	$(CXX) $(PRELINK_FLAG) $(CXXFLAGS) $(CPPFLAGS) $(OBJS) \
		$(LDFLAGS) $(LDLIBS)
endif
	$(AR) $(ARFLAGS) $@ $(OBJS)
	$(RANLIB) $@

idl: libidl.a main.o ../orb/$(LIBMICO) ../ir/$(LIBMICOIR) $(IR_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) main.o $(IR_OBJS) -lidl $(LDLIBS) -o idl
	$(POSTLD) $@

main.o:

# cleaning

clean:
	-$(RM) idl idl.install
	-$(RM) *.o *.o0 *.a *~ *.$(SOEXT) *.exe *.rpo *.ii *.ti *.objid
	-$(RM) .depend TAGS core nohup.out $(GENERATED)


install: all
	$(IDIRCMD) $(INSTDIR)/bin
	$(IBINCMD) idl $(INSTDIR)/bin

install-cd: install


generate:
	$(BISON) -d -t -v -o yacc.c yacc.yy
	mv yacc.c yacc.cc
	$(FLEX) -B -8 -oscanner.cc scanner.ll
	sed -e 's/extern int isatty YY_PROTO(( int ));/#include <unistd.h>/g' < scanner.cc > scanner.cc.new
	mv scanner.cc.new scanner.cc

# dependencies

ifeq (.depend, $(wildcard .depend))
include .depend
endif

.depend :
	echo '# Module dependencies' > .depend
	$(MKDEPEND) $(CXXFLAGS) $(CPPFLAGS) *.cc >> .depend

