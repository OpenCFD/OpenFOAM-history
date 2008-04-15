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

VER = $(shell cat ../VERSION)
BIN_VER1 = $(shell echo $(VERSION) | cut -d. -f1)
BIN_VER2 = $(shell echo $(VERSION) | cut -d. -f2)
BIN_VER3 = $(shell echo $(VERSION) | cut -d. -f3)
BIN_VER = $(shell printf '0x%02x%02x%02x' $(BIN_VER1) $(BIN_VER2) $(BIN_VER3))

DIRS = mico ministl coss

MICO_DIRS = os-thread transport security

adm: mico/version.h

mico/version.h: ../VERSION
	echo "#define MICO_VERSION \"$(VER)\"" > mico/version.h
	echo "#define MICO_BIN_VERSION $(BIN_VER)" >> mico/version.h

install:
	$(IDIRCMD) $(INSTDIR)/include
	for i in *.h; do \
	  $(IINCCMD) $$i $(INSTDIR)/include; \
	done
	for i in $(DIRS); do \
	  $(IDIRCMD) $(INSTDIR)/include/$$i; \
	  for j in $$i/*; do \
	    $(IINCCMD) $$j $(INSTDIR)/include/$$i/; \
	  done; \
	done
	for i in $(MICO_DIRS); do \
	  $(IDIRCMD) $(INSTDIR)/include/mico/$$i; \
	  for j in mico/$$i/*; do \
	    $(IINCCMD) $$j $(INSTDIR)/include/mico/$$i/; \
	  done; \
	done

install-cd: install

