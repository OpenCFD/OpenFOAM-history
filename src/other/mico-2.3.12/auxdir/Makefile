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

PLATFORM_LIBS = -lSM -lICE
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

LDFLAGS  := -L../orb $(LDFLAGS)
CXXFLAGS := -I. -I../include $(CPPFLAGS) $(CXXFLAGS) $(X_CFLAGS)
LDLIBS   := $(LDMICO) $(CONFLIBS)

AUX_SRCS=dynany_impl.cc
STATIC_AUX_OBJS = $(AUX_SRCS:.cc=.o)
SHARED_AUX_OBJS = $(AUX_SRCS:.cc=.pic.o)

STATIC_LIBS = libmicoaux$(VERSION).a

ifeq ($(HAVE_X11), yes)
STATIC_LIBS := $(STATIC_LIBS) libmicox$(VERSION).a
X11_SRCS := x11.cc
STATIC_X11_OBJS = $(X11_SRCS:.cc=.o)
SHARED_X11_OBJS = $(X11_SRCS:.cc=.pic.o)
endif

ifeq ($(HAVE_QT), yes)
STATIC_LIBS := $(STATIC_LIBS) libmicoqt$(VERSION).a
CXXFLAGS := $(CXXFLAGS) $(QT_CXXFLAGS)
QT_SRCS := qtmico.cc
STATIC_QT_OBJS = $(QT_SRCS:.cc=.o)
SHARED_QT_OBJS = $(QT_SRCS:.cc=.pic.o)
endif

ifeq ($(HAVE_GTK), yes)
STATIC_LIBS := $(STATIC_LIBS) libmicogtk$(VERSION).a
CXXFLAGS := $(CXXFLAGS) $(GTK_CFLAGS)
GTK_SRCS := gtkmico.cc
STATIC_GTK_OBJS = $(GTK_SRCS:.cc=.o)
SHARED_GTK_OBJS = $(GTK_SRCS:.cc=.pic.o)
endif

ifeq ($(HAVE_TCL), yes)
STATIC_LIBS := $(STATIC_LIBS) libmicotcl$(VERSION).a
TCL_SRCS := tclmico.cc
STATIC_TCL_OBJS = $(TCL_SRCS:.cc=.o)
SHARED_TCL_OBJS = $(TCL_SRCS:.cc=.pic.o)
endif

SHARED_LIBS = $(STATIC_LIBS:.a=.$(SOEXT))

# mingw hack

ifeq ($(OSTYPE), windows)
ifeq ($(HAVE_SHARED), yes)
HAVE_SHARED := no
HAVE_STATIC := yes
endif
endif

# normal rules

all: lib prg

prg:

ifeq ($(HAVE_SHARED), yes)
ifeq ($(HAVE_STATIC), yes)
lib: .depend $(STATIC_LIBS) $(SHARED_LIBS)
else
lib: .depend $(SHARED_LIBS)
endif
else
lib: .depend $(STATIC_LIBS)
endif

qtmico.moc: ../include/mico/qtmico.h
	$(MOC) $< -o $@

dynany_impl.o: dynany_impl.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(EHFLAGS) -c $< -o $@

dynany_impl.pic.o: dynany_impl.cc
	$(CXX) $(PICFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(EHFLAGS) -c $< -o $@

qtmico.cc: qtmico.moc


# shared

libmicoaux$(VERSION).$(SOEXT): $(SHARED_AUX_OBJS)
ifeq ($(HAVE_REPO), yes)
	-$(CXX) $(CXXFLAGS) $(SHARED_AUX_OBJS) $(LDFLAGS) $(LDLIBS)
endif
	rm -f $@
	$(LDSO) -o libmicoaux$(VERSION) $(SHARED_AUX_OBJS) \
		-L../libs $(LDMICO)
	$(IDIRCMD) ../libs
	cd ../libs; rm -f $@; $(LN) ../auxdir/$@ .

libmicox$(VERSION).$(SOEXT): $(SHARED_X11_OBJS)
ifeq ($(HAVE_REPO), yes)
	-$(CXX) $(CXXFLAGS) $(SHARED_X11_OBJS) $(LDFLAGS) $(X_LIBS) $(LDLIBS)
endif
	rm -f $@
	$(LDSO) -o libmicox$(VERSION) $(SHARED_X11_OBJS) $(X_LIBS) \
		$(PURE_LDFLAGS) -lXt -lX11 -L../libs $(LDMICO) $(PLATFORM_LIBS)
	$(IDIRCMD) ../libs
	cd ../libs; rm -f $@; $(LN) ../auxdir/$@ .

libmicoqt$(VERSION).$(SOEXT): $(SHARED_QT_OBJS)
ifeq ($(HAVE_REPO), yes)
	-$(CXX) $(CXXFLAGS) $(SHARED_QT_OBJS) $(LDFLAGS) $(X_LIBS) $(LDLIBS)
endif
	rm -f $@
	$(LDSO) -o libmicoqt$(VERSION) $(SHARED_QT_OBJS) $(X_LIBS) \
		$(PURE_LDFLAGS) $(QT_LIBS) -L../libs $(LDMICO)
	$(IDIRCMD) ../libs
	cd ../libs; rm -f $@; $(LN) ../auxdir/$@ .

libmicogtk$(VERSION).$(SOEXT): $(SHARED_GTK_OBJS)
ifeq ($(HAVE_REPO), yes)
	-$(CXX) $(CXXFLAGS) $(SHARED_GTK_OBJS) $(LDFLAGS) $(X_LIBS) $(LDLIBS)
endif
	rm -f $@
	$(LDSO) -o libmicogtk$(VERSION) $(SHARED_GTK_OBJS) $(X_LIBS) \
		$(PURE_LDFLAGS) $(GTK_LIBS) -L../libs $(LDMICO)
	$(IDIRCMD) ../libs
	cd ../libs; rm -f $@; $(LN) ../auxdir/$@ .

libmicotcl$(VERSION).$(SOEXT): $(SHARED_TCL_OBJS)
ifeq ($(HAVE_REPO), yes)
	-$(CXX) $(CXXFLAGS) $(SHARED_TCL_OBJS) $(LDFLAGS) $(LDLIBS)
endif
	rm -f $@
	$(LDSO) -o libmicotcl$(VERSION) $(SHARED_TCL_OBJS) \
		$(PURE_LDFLAGS) $(TCL_LIBS) -L../libs $(LDMICO)
	$(IDIRCMD) ../libs
	cd ../libs; rm -f $@; $(LN) ../auxdir/$@ .


# static

libmicoaux$(VERSION).a: $(STATIC_AUX_OBJS)
ifeq ($(HAVE_REPO), yes)
	-$(CXX) $(CXXFLAGS) $(STATIC_AUX_OBJS) $(LDFLAGS) $(LDLIBS)
endif
	$(AR) $(ARFLAGS) $@ $(STATIC_AUX_OBJS)
	$(RANLIB) $@
	$(IDIRCMD) ../libs
	cd ../libs; rm -f $@; $(LN) ../auxdir/$@ .

libmicox$(VERSION).a: $(STATIC_X11_OBJS)
ifeq ($(HAVE_REPO), yes)
	-$(CXX) $(CXXFLAGS) $(STATIC_X11_OBJS) $(LDFLAGS) $(LDLIBS)
endif
	$(AR) $(ARFLAGS) $@ $(STATIC_X11_OBJS)
	$(RANLIB) $@
	$(IDIRCMD) ../libs
	cd ../libs; rm -f $@; $(LN) ../auxdir/$@ .

libmicoqt$(VERSION).a: $(STATIC_QT_OBJS)
ifeq ($(HAVE_REPO), yes)
	-$(CXX) $(CXXFLAGS) $(STATIC_QT_OBJS) $(LDFLAGS) $(LDLIBS)
endif
	$(AR) $(ARFLAGS) $@ $(STATIC_QT_OBJS)
	$(RANLIB) $@
	$(IDIRCMD) ../libs
	cd ../libs; rm -f $@; $(LN) ../auxdir/$@ .

libmicogtk$(VERSION).a: $(STATIC_GTK_OBJS)
ifeq ($(HAVE_REPO), yes)
	-$(CXX) $(CXXFLAGS) $(STATIC_GTK_OBJS) $(LDFLAGS) $(LDLIBS)
endif
	$(AR) $(ARFLAGS) $@ $(STATIC_GTK_OBJS)
	$(RANLIB) $@
	$(IDIRCMD) ../libs
	cd ../libs; rm -f $@; $(LN) ../auxdir/$@ .

libmicotcl$(VERSION).a: $(STATIC_TCL_OBJS)
ifeq ($(HAVE_REPO), yes)
	-$(CXX) $(CXXFLAGS) $(STATIC_TCL_OBJS) $(LDFLAGS) $(LDLIBS)
endif
	$(AR) $(ARFLAGS) $@ $(STATIC_TCL_OBJS)
	$(RANLIB) $@
	$(IDIRCMD) ../libs
	cd ../libs; rm -f $@; $(LN) ../auxdir/$@ .


# cleaning

clean:
	-$(RM) *.o *.o0 *.a *~ *.$(SOEXT) *.exe *.rpo *.ii *.ti *.objid *.moc
	-$(RM) .depend TAGS core nohup.out
	-$(RM) *.orig

install:
	$(IDIRCMD) $(INSTDIR)/lib
ifeq ($(HAVE_SHARED), yes)
	@for i in $(SHARED_LIBS); do $(ILIBCMD) $$i $(INSTDIR)/lib; done
endif
ifeq ($(HAVE_STATIC), yes)
	@for i in $(STATIC_LIBS); do $(ILIBCMD) $$i $(INSTDIR)/lib; done
endif

install-cd: install

ifeq (.depend, $(wildcard .depend))
include .depend
endif

.depend :
	echo '# Module dependencies' > .depend
	$(MKDEPEND) $(CXXFLAGS) *.cc >> .depend
