#
#  MICO --- an Open Source CORBA implementation
#  Copyright (c) 1997-2001 by The Mico Team
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Library General Public
#  License as published by the Free Software Foundation; either
#  version 2 of the License, or (at your option) any later version.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Library General Public License for more details.
#
#  You should have received a copy of the GNU Library General Public
#  License along with this library; if not, write to the Free
#  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
#  For more information, visit the MICO Home Page at
#  http://www.mico.org/
#

include ../MakeVars

LDFLAGS  := -L. -L../idl -L../orb $(LDFLAGS)
CXXFLAGS := -I. -I../idl -I../include $(PCHFLAGS) $(CXXFLAGS)
LDLIBS   := -lidl $(LDMICOIR) $(LDMICO) $(CONFLIBS)

PLATFORM_LIBS =
ifeq ($(HAVE_CYGWIN), yes)
PLATFORM_LIBS := $(PLATFORM_LIBS) ../orb/libmico$(VERSION).$(SOEXT)
endif

ifeq ($(HAVE_EXCEPTIONS), yes)
NOPICFLAGS := $(NOPICFLAGS) $(EHOPTFLAGS)
else
NOPICFLAGS := $(NOPICFLAGS) $(NOEHFLAGS)
endif
ifeq ($(HAVE_SHARED_EXCEPTS), yes)
PICFLAGS := $(PICFLAGS) $(EHOPTFLAGS)
else
PICFLAGS := $(PICFLAGS) $(NOEHFLAGS)
endif

STATIC_OBJS=ir_skel.o ir_impl.o

ifeq ($(USE_CCM), yes)
STATIC_OBJS := $(STATIC_OBJS) ir3.o ir3_skel.o
endif

ifeq ($(HAVE_FINAL), yes)
STATIC_OBJS := ir_all.o
endif

SHARED_OBJS=$(STATIC_OBJS:.o=.pic.o)

# mingw hack

ifeq ($(OSTYPE), windows)
ifeq ($(HAVE_SHARED), yes)
HAVE_SHARED := no
HAVE_STATIC := yes
endif
endif

# normal rules

all: lib prg

prg: ird

ifeq ($(HAVE_SHARED), yes)
ifeq ($(HAVE_STATIC), yes)
lib: .depend libmicoir$(VERSION).$(SOEXT) libmicoir$(VERSION).a
else
lib: .depend libmicoir$(VERSION).$(SOEXT)
endif
else
lib: .depend libmicoir$(VERSION).a
endif

ird: main.o ../idl/libidl.a ../orb/$(LIBMICO) $(LIBMICOIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) main.o -L. $(LDLIBS) -o ird
	$(POSTLD) $@

libmicoir$(VERSION).$(SOEXT): $(SHARED_OBJS)
	$(RM) $@
ifeq ($(HAVE_REPO), yes)
	-$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(SHARED_OBJS) $(LDFLAGS) \
		$(LDMICO) $(CONFLIBS)
endif
ifeq ($(HAVE_PRELINK), yes)
	$(CXX) $(PRELINK_FLAG) $(SHARED_OBJS) $(LDFLAGS) \
		$(LDMICO) $(CONFLIBS)
endif
	$(LDSO) -o libmicoir$(VERSION) $(SHARED_OBJS) -L../libs $(LDMICO) $(PLATFORM_LIBS)
	$(IDIRCMD) ../libs
	cd ../libs; $(RM) $@; $(LN) ../ir/$@ .

libmicoir$(VERSION).a: $(STATIC_OBJS)
ifeq ($(HAVE_REPO), yes)
	-$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(STATIC_OBJS) $(LDFLAGS) \
		$(LDMICO) $(CONFLIBS)
endif
ifeq ($(HAVE_PRELINK), yes)
	$(CXX) $(PRELINK_FLAG) $(CXXFLAGS) $(CPPFLAGS) $(STATIC_OBJS) \
		$(LDFLAGS) $(LDMICO) 
endif
	$(AR) $(ARFLAGS) $@ $(STATIC_OBJS)
	$(RANLIB) $@
	$(IDIRCMD) ../libs
	cd ../libs; $(RM) $@; $(LN) ../ir/$@ .

generate:
	-mv ir_skel.cc ir_skel.cc.backup
	-cd ../orb; mv ir.cc ir.cc.backup
	-cd ../orb; mv ir_base.cc ir_base.cc.backup
	-cd ../include/mico; mv ir.h ir.h.backup
	-cd ../include/mico; mv ir_base.h ir_base.h.backup
	$(IDL) --no-poa-ties --windows-dll mico -B.. \
		--any --mico-core --name ir_base ../include/mico/ir_base.idl
	$(IDL) --no-poa-ties --c++-skel --windows-dll mico --any --name ir \
		-B.. --mico-core ../include/mico/ir.idl
	$(IDL) --no-poa-ties --c++-skel --any --name ir3 -B.. \
		--hh-prefix mico ../include/mico/ir3.idl
	sleep 1
	patch < ir.cc.diffs
	$(RM) ir.*.orig ir_base.*.orig
	mv ir.h ir_base.h ir3.h ../include/mico
	mv ir.cc ir_base.cc ../orb

ungenerate:
	-mv ir_skel.cc.backup ir_skel.cc
	-mv ../orb/ir.cc.backup ../orb/ir.cc
	-mv ../orb/ir_base.cc.backup ../orb/ir_base.cc
	-mv ../include/mico/ir.h.backup ../include/mico/ir.h
	-mv ../include/mico/ir_base.h.backup ../include/mico/ir_base.h

diffs:
	mkdir orig; cd orig; \
		idl --no-poa-ties --windows-dll mico -B../.. \
		--any --mico-core --name ir_base ../../include/mico/ir_base.idl
	cd orig; $(IDL) --no-poa-ties --c++-skel --windows-dll mico --any \
		-B../.. --mico-core --name ir ../../include/mico/ir.idl
	$(RM) ir.cc.diffs
	cd ../include/mico; diff -c ../../ir/orig/ir.h ir.h \
	  > ../../ir/ir.cc.diffs || true
	cd ../include/mico; diff -c ../../ir/orig/ir_base.h ir_base.h \
	  >> ../../ir/ir.cc.diffs || true
	diff -c orig/ir.cc ir.cc >> ir.cc.diffs || true
	diff -c orig/ir_skel.cc ir_skel.cc >> ir.cc.diffs || true
	diff -c orig/ir_base.cc ir_base.cc >> ir.cc.diffs || true
	$(RM) -rf orig

# cleaning

clean:
	-$(RM) libs libmicoir* ird
	-$(RM) *.o *.o0 *.a *~ *.$(SOEXT) *.exe *.rpo *.ii *.ti *.objid
	-$(RM) .depend TAGS core nohup.out

install:
	$(IDIRCMD) $(INSTDIR)/bin
	$(IBINCMD) ird $(INSTDIR)/bin
ifeq ($(HAVE_STATIC), yes)
	$(ILIBCMD) libmicoir$(VERSION).a $(INSTDIR)/lib
endif
ifeq ($(HAVE_SHARED), yes)
	$(ILIBCMD) libmicoir$(VERSION).$(SOEXT) $(INSTDIR)/lib
endif

install-cd: install

ifeq (.depend, $(wildcard .depend))
include .depend
endif

.depend :
	echo '# Module dependencies' > .depend
	$(MKDEPEND) $(CXXFLAGS) $(CPPFLAGS) *.cc >> .depend




