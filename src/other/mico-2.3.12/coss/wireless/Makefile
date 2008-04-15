#
# MIWCO --- Wireless CORBA support for MICO (OMG document formal/2004-04-02)
# Copyright (C) 2001 Jaakko Kangasharju
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

include ../../MakeVars
TARGETS =

ifeq ($(USE_WIRELESS_HOME), yes)
TARGETS := $(TARGETS) hla
endif

ifeq ($(USE_WIRELESS_TERMINAL), yes)
TARGETS := $(TARGETS) tadmin tb
endif

ifeq ($(USE_WIRELESS_ACCESS), yes)
TARGETS := $(TARGETS) callback nadmin ab
endif

LDLIBS    = $(LDMICOCOSS) $(LDMICO) $(CONFLIBS)
LDFLAGS  := -L.. -L../../orb $(LDFLAGS)
CXXFLAGS := -I. -I../../include $(CXXFLAGS) $(EHFLAGS)
IDLFLAGS  = -I../../include

HLA_OBJS = HomeLocationAgent_impl.o hla.o
TB_OBJS  = TerminalBridge_impl.o glue.o proxy.o bridge.o tb.o
AB_OBJS  = AccessBridge_impl.o glue.o proxy.o bridge.o ab.o
TAD_OBJS = tadmin.o
NAD_OBJS = nadmin.o
CB_OBJS  = callback.o

all: lib prg

ifeq ($(HAVE_FINAL), no)
ifeq ($(HAVE_SHARED_EXCEPTS), yes)
ifeq ($(HAVE_STATIC), yes)
lib: .depend MobileTerminal.o MobileTerminal.pic.o GTP.o GTP.pic.o \
		MobileTerminalNotification.o MobileTerminalNotification.pic.o
else
lib: .depend MobileTerminal.pic.o GTP.pic.o MobileTerminalNotification.pic.o
endif
else
lib: .depend MobileTerminal.o GTP.o MobileTerminalNotification.pic.o
endif
else
lib:
endif

prg: .depend $(TARGETS)

idlcompile: MobileTerminal.cc GTP.cc MobileTerminalNotification.cc

hla: $(HLA_OBJS) ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(HLA_OBJS) $(LDLIBS) -o hla
	$(POSTLD) $@

tb: $(TB_OBJS) ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(TB_OBJS) $(LDLIBS) -o tb
	$(POSTLD) $@

ab: $(AB_OBJS) ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(AB_OBJS) $(LDLIBS) -o ab
	$(POSTLD) $@

callback: $(CB_OBJS) ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(CB_OBJS) $(LDLIBS) -o callback
	$(POSTLD) $@

tadmin: $(TAD_OBJS) ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(TAD_OBJS) $(LDLIBS) -o tadmin
	$(POSTLD) $@

nadmin: $(NAD_OBJS) ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(NAD_OBJS) $(LDLIBS) -o nadmin
	$(POSTLD) $@

MobileTerminal.cc MobileTerminal.h: \
		../../include/coss/MobileTerminal.idl $(IDL)
	$(IDL) $(IDLFLAGS) --name MobileTerminal ../../include/coss/MobileTerminal.idl
	cp MobileTerminal.h ../../include/coss

MobileTerminalNotification.cc MobileTerminalNotification.h: \
		../../include/coss/MobileTerminalNotification.idl $(IDL)
	$(IDL) $(IDLFLAGS) --any --name MobileTerminalNotification \
		../../include/coss/MobileTerminalNotification.idl
	cp MobileTerminalNotification.h ../../include/coss

GTP.cc GTP.h: ../../include/coss/GTP.idl $(IDL)
	$(IDL) $(IDLFLAGS) --name GTP ../../include/coss/GTP.idl
	cp GTP.h ../../include/coss

clean:
	$(RM) *.o $(TARGETS)
	$(RM) .depend TAGS core nohup.out
	$(RM) ref.* *.ref
	$(RM) MobileTerminal.cc MobileTerminal.h
	$(RM) ../../include/coss/MobileTerminal.h
	$(RM) MobileTerminalNotification.cc MobileTerminalNotification.h
	$(RM) ../../include/coss/MobileTerminalNotification.h
	$(RM) GTP.cc GTP.h
	$(RM) ../../include/coss/GTP.h

install:
	$(IDIRCMD) $(INSTDIR)/bin
	for i in $(TARGETS); do $(IBINCMD) $$i $(INSTDIR)/bin; done

ifeq (.depend, $(wildcard .depend))
include .depend
endif

.depend:
	echo '# Module dependencies' >.depend
	$(MKDEPEND) $(CXXFLAGS) *.cc >>.depend
