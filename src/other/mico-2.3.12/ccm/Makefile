#
# MICO --- an Open Source CORBA implementation
# Copyright (c) 1997-2002 by The Mico Team
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

PLATFORM_LIBS =
ifeq ($(HAVE_CYGWIN), yes)
PLATFORM_LIBS := $(PLATFORM_LIBS) ../orb/libmico$(VERSION).$(SOEXT)
endif


LDFLAGS  := -L. -L../ir -L../idl -L../orb -L../coss $(LDFLAGS)
CXXFLAGS := -I../include -I../idl $(CXXFLAGS) $(EHFLAGS)
LDLIBS   := -lidl $(LDMICO) $(CONFLIBS)

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

STATIC_OBJS=CCM.o CCMContainer.o
SHARED_OBJS=$(STATIC_OBJS:.o=.pic.o)

# mingw hack

ifeq ($(OSTYPE), windows)
ifeq ($(HAVE_SHARED), yes)
HAVE_SHARED := no
HAVE_STATIC := yes
endif
endif

# normal rules

ifeq ($(USE_CCM), yes)

all: prg
lib:
ifeq ($(HAVE_SHARED), yes)
LIBMICOCCM := libmicoccm$(VERSION).$(SOEXT)
ifeq ($(HAVE_STATIC), yes)
prg: .depend libmicoccm$(VERSION).$(SOEXT) libmicoccm$(VERSION).a \
	mico-ccm componentserver mico-ccmd ccmload
else
prg: .depend libmicoccm$(VERSION).$(SOEXT) \
	mico-ccm componentserver mico-ccmd ccmload
endif
else
LIBMICOCCM := libmicoccm$(VERSION).a
prg: .depend libmicoccm$(VERSION).a \
	mico-ccm componentserver mico-ccmd ccmload
endif
else
all:
lib:
prg:
endif

mico-ccm: ../idl/libidl.a ../orb/$(LIBMICO) ../ir/$(LIBMICOIR) $(LIBMICOCCM) mico-ccm.o
	$(CXX) -o $@ $(LDFLAGS) mico-ccm.o -L. $(LDMICOCCM) $(LDMICOIR) $(LDLIBS)
	$(POSTLD) $@

componentserver: ../orb/$(LIBMICO) $(LIBMICOCCM) componentserver.o
	$(CXX) -o $@ $(LDFLAGS) componentserver.o -L. $(LDMICOCCM) $(LDLIBS)

mico-ccmd: ../orb/$(LIBMICO) $(LIBMICOCCM) mico-ccmd.o
	$(CXX) -o $@ $(LDFLAGS) mico-ccmd.o -L. $(LDMICOCCM) $(LDLIBS)

ccmload: ../orb/$(LIBMICO) $(LIBMICOCCM) ccmload.o
	$(CXX) -o $@ $(LDFLAGS) ccmload.o -L. $(LDMICOCCM) $(LDMICOCOSS) $(LDLIBS)

libmicoccm$(VERSION).$(SOEXT): $(SHARED_OBJS)
	$(RM) $@
ifeq ($(HAVE_REPO), yes)
	-$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(SHARED_OBJS) $(LDFLAGS) $(LDLIBS)
endif
ifeq ($(HAVE_PRELINK), yes)
	$(CXX) $(PRELINK_FLAG) $(SHARED_OBJS) $(LDFLAGS) $(LDLIBS)
endif
	$(LDSO) -o libmicoccm$(VERSION) $(SHARED_OBJS) -L../libs $(LDMICO) $(PLATFORM_LIBS)
	$(IDIRCMD) ../libs
	cd ../libs; $(RM) $@; $(LN) ../ccm/$@ .

libmicoccm$(VERSION).a: $(STATIC_OBJS)
ifeq ($(HAVE_REPO), yes)
	-$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(STATIC_OBJS) $(LDFLAGS) $(LDLIBS)
endif
ifeq ($(HAVE_PRELINK), yes)
	$(CXX) $(PRELINK_FLAG) $(STATIC_OBJS) $(LDFLAGS) $(LDLIBS)
endif
	$(AR) $(ARFLAGS) $@ $(STATIC_OBJS)
	$(RANLIB) $@
	$(IDIRCMD) ../libs
	cd ../libs; $(RM) $@; $(LN) ../ccm/$@ .

# cleaning

clean:
	-$(RM) mico-ccmd ccmload componentserver mico-ccm libmicoccm*
	-$(RM) *.o *.o0 *.a *~ *.$(SOEXT) *.exe *.rpo *.ii *.ti *.objid
	-$(RM) .depend TAGS core nohup.out

install: all
ifeq ($(USE_CCM), yes)
	$(IDIRCMD) $(INSTDIR)/bin
	$(IBINCMD) mico-ccm $(INSTDIR)/bin
	$(IBINCMD) componentserver $(INSTDIR)/bin
	$(IBINCMD) mico-ccmd $(INSTDIR)/bin
	$(IBINCMD) ccmload $(INSTDIR)/bin
ifeq ($(HAVE_STATIC), yes)
	$(ILIBCMD) libmicoccm$(VERSION).a $(INSTDIR)/lib
endif
ifeq ($(HAVE_SHARED), yes)
	$(ILIBCMD) libmicoccm$(VERSION).$(SOEXT) $(INSTDIR)/lib
endif
endif

install-cd: install


# dependencies

ifeq (.depend, $(wildcard .depend))
include .depend
endif

generate:
	$(IDL) --name CCM --any -B.. --hh-prefix mico \
		--windows-dll ccm \
		../include/mico/CCM.idl
	mv CCM.h ../include/mico/CCM.h

.depend :
	echo '# Module dependencies' > .depend
	$(MKDEPEND) $(CPPFLAGS) $(CXXFLAGS) *.cc >> .depend

