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

DIRS = bench dynany interop henning interceptor \
  codesets poa random obv services dispatcher mcast generic pi

RUNDIRS = bench dynany interceptor codesets poa obv mcast generic pi

INST_DIRS = $(DIRS) shlib ssl

# shlib needs to be ported to POA
#ifeq ($(HAVE_DYNAMIC), yes)
#DIRS := $(DIRS) shlib
#endif

ifeq ($(HAVE_SSL), yes)
DIRS := $(DIRS) ssl
RUNDIRS := $(RUNDIRS) ssl
endif

ifeq ($(USE_CCM), yes)
DIRS := $(DIRS) ccm
RUNDIRS := $(RUNDIRS) ccm
endif

ifeq ($(USE_CSL2), yes)
DIRS := $(DIRS) security
RUNDIRS := $(RUNDIRS) security
endif

ifeq ($(USE_CSIV2), yes)
DIRS := $(DIRS) csiv2
RUNDIRS := $(RUNDIRS) csiv2
endif

ifeq ($(HAVE_THREADS), yes)
DIRS := $(DIRS) mt
endif

ifeq ($(USE_LIFE), yes)
DIRS := $(DIRS) lifecycle
endif

ifeq ($(HAVE_BLUETOOTH), yes)
DIRS := $(DIRS) ltp
endif

all: prg

prg:
	for i in $(DIRS); do $(MAKE) -C $$i || exit 1; done

run:
	for i in $(RUNDIRS); do $(MAKE) -C $$i run || exit 1; done

clean:
	for i in $(DIRS); do $(MAKE) -C $$i clean || exit 1; done

install:
	$(IDIRCMD) $(INSTDIR)/doc/mico/examples
	sed -e s/@MICO_VERSION@/$(VERSION)/g \
		< MakeVars.install > MakeVars.install.ver
	$(ISRCCMD) MakeVars.install.ver $(INSTDIR)/doc/mico/examples/MakeVars
	rm -f MakeVars.install.ver
	$(ISRCCMD) README $(INSTDIR)/doc/mico/examples
	for i in $(INST_DIRS); do $(MAKE) -C $$i install || exit 1; done

install-cd: install

