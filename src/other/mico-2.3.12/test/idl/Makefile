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

#
# Omit test/idl/25 from compiling. This test is broken for a long time now.
# For more infomrmation about it and its status, please look at PR#64
#
#DIRS = 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 18 19 20 21 22 23 24 25 26 27 29 30
DIRS = 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 18 19 20 21 22 23 24 26 27 29 30 31 32 33 34 35 36 37

ifeq ($(HAVE_EXCEPTIONS), yes)
DIRS := $(DIRS) 17
endif

ifeq ($(HAVE_NAMESPACE), yes)
DIRS := $(DIRS) 28
endif


all:
	for i in $(DIRS); do $(MAKE) -C $$i || exit 1; done

clean:
	for i in $(DIRS); do $(MAKE) -C $$i clean || exit 1; done

check:
	@for dir in $(DIRS); do \
	  if test -x $$dir/demo -a -f $$dir/expected-stdout; then \
	    echo Testing $$dir...; \
	    cd $$dir; \
	    if ./demo | cmp expected-stdout - >/dev/null; then :; \
	    else \
	      echo ==== test $$dir failed =====================; \
	      ./demo | diff -c expected-stdout -; \
	      echo ========================================; \
	    fi; \
	    cd ..; \
	  fi; \
	done
ifeq ($(HAVE_EXCEPTIONS), yes)
	$(MAKE) -C error check
endif
