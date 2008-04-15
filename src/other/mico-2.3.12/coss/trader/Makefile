#
# MICO --- a free CORBA implementation
# Copyright (C) 1998 by Torben Weis
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

LDLIBS    = -lmicocoss$(VERSION) -lmico$(VERSION) $(CONFLIBS)
LDFLAGS  := -L.. -L../../orb $(LDFLAGS)
CXXFLAGS := -I. -I../../include $(CXXFLAGS) $(EHFLAGS)
IDLFLAGS := --no-poa --boa --c++-skel -B../..
CFLAGS   := -DHAVE_CONFIG_H -I. -I../../cpp -I../../include $(CFLAGS)

# generated files
GENERATED = yacc.output
%.cc: %.ll
%.cc: %.yy


LIB_SRCS = CosTrading.cc CosTradingRepos.cc
SRV_SRCS  = main.cc link.cc parse.cc parse_tree.cc yacc.cc lex.cc lookup.cc \
  misc.cc register.cc typerepo_impl.cc proxy.cc kany.cc trader_main.cc \
  CosTrading_skel.cc CosTradingRepos_skel.cc \
  ../../cpp/alloca.o

SHARED_LIB_OBJS = $(LIB_SRCS:.cc=.pic.o)
STATIC_LIB_OBJS = $(LIB_SRCS:.cc=.o)
SRV_OBJS = $(SRV_SRCS:.cc=.o)

# normal rules

all: lib prg

ifeq ($(HAVE_FINAL), no)
ifeq ($(HAVE_SHARED_EXCEPTS), yes)
ifeq ($(HAVE_STATIC), yes)
lib: .depend $(SHARED_LIB_OBJS) $(STATIC_LIB_OBJS)
else
lib: .depend $(SHARED_LIB_OBJS)
endif
else
lib: .depend $(STATIC_LIB_OBJS)
endif
else
lib:
endif

prg: .depend traderd

idlcompile: CosTrading.cc CosTrading_skel.cc CosTradingRepos.cc CosTradingRepos_skel.cc

traderd: $(SRV_OBJS) ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(SRV_OBJS) $(LDLIBS) -o traderd
	$(POSTLD) $@


CosTrading.h CosTrading.cc CosTrading_skel.cc .NOTPARALLEL: ../../include/coss/CosTrading.idl $(IDL)
	$(IDL) $(IDLFLAGS) --any --name CosTrading ../../include/coss/CosTrading.idl
	cp CosTrading.h ../../include/coss

CosTradingRepos.h CosTradingRepos.cc CosTradingRepos_skel.cc .NOTPARALLEL: ../../include/coss/CosTradingRepos.idl $(IDL)
	$(IDL) $(IDLFLAGS) --name CosTradingRepos ../../include/coss/CosTradingRepos.idl
	cp CosTradingRepos.h ../../include/coss


# cleaning

clean:
	-$(RM) *.o *.a *~ *.rpo *.objid traderd
	-$(RM) .depend TAGS core nohup.out
	-$(RM) CosTrading.h CosTrading.cc CosTradingRepos.h CosTradingRepos.cc
	-$(RM) ../../include/coss/CosTrading.h ../../include/coss/CosTradingRepos.h
	-$(RM) $(GENERATED) CosTrading_skel.cc CosTradingRepos_skel.cc

install:
	$(IDIRCMD) $(INSTDIR)/bin
	$(IBINCMD) traderd $(INSTDIR)/bin

generate:
	$(BISON) -d -t -v -o yacc.c yacc.yy
	mv yacc.c yacc.cc
	$(FLEX) -B -8 -olex.cc lex.ll
	sed -e 's/extern int isatty YY_PROTO(( int ));/#include <unistd.h>/g' < lex.cc > lex.cc.new
	mv lex.cc.new lex.cc

ifeq (.depend, $(wildcard .depend))
include .depend
endif

.depend :
	echo '# Module dependencies' > .depend
	$(MKDEPEND) $(CXXFLAGS) *.cc >> .depend
