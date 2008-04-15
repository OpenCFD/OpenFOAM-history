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
LDFLAGS  := -L../../orb $(LDFLAGS)
LDLIBS   := -lmico$(VERSION) $(CONFLIBS)
IDLFLAGS := -B../..

# generated files

GEN  = yacc.output account.cc account.h browser.cc browser.h
SRCS = account.cc browser.cc yacc.cc scanner.cc cg.cc dii.cc parser.cc \
       diiproxy.cc accountI.cc browserI.cc
OBJS = ${SRCS:.cc=.o}

JGEN = parser.java sym.java
JSRCS=$(JGEN) Node.java RelationNode.java ConceptNode.java \
  ConceptualGraph.java DIIException.java Proxy.java parser.java scanner.java \
  Node.java TransportLayer.java MyCanvas.java CanvasObject.java \
  CanvasBoxText.java DII.java CanvasHandler.java \
  CanvasArrow.java CGEditor.java CanvasLine.java EntryBox.java AlertBox.java \
  EditLinearForm.java CGParser.java View.java GlobalVars.java \
  BorderPanel.java CGTemplates.java CanvasIcon.java IRBrowser.java \
  DIIRequest.java IRBrowserProxy.java DIIString.java DIIBoolean.java \
  DIIObject.java DIISequence.java DIIFrame.java IRFrame.java StatusLine.java

JOBJS=${JSRCS:.java=.class}

GENERATED = $(GEN) $(JGEN)

# normal rules

all: .depend diiproxy $(JOBJS)

diiproxy: $(OBJS) ../../orb/$(LIBMICO)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -L../../idl $(OBJS) -lidl $(LDLIBS) -o $@
	$(POSTLD) $@

account.cc account.h: account.idl
	$(IDL) $(IDLFLAGS) --no-poa --boa $<

browser.cc browser.h: browser.idl
	$(IDL) $(IDLFLAGS) --no-poa --boa $<

# generate

generate:
	$(BISON) -d -t -v -o yacc.c yacc.yy
	mv yacc.c yacc.cc
	$(FLEX) -B -8 -oscanner.cc scanner.ll
	patch < scanner.cc.diffs
	./gentemplates templates.kb < CGTemplates.java.in > CGTemplates.java

diffs:
	mkdir orig; cp scanner.ll orig/
	cd orig; $(FLEX) -B -8 -oscanner.cc scanner.ll
	diff -c orig/scanner.cc scanner.cc > scanner.cc.diffs || true
	$(RM) -rf orig

# cleaning

install:
	:

clean:
	-$(RM) diiproxy *.o *.a *~ *.rpo *.class
	-$(RM) .depend TAGS core nohup.out $(GENERATED)


# dependencies

parser.java sym.java: parser.cup


ifeq (.depend, $(wildcard .depend))
include .depend
endif

.depend :
	echo '# Module dependencies' > .depend
	$(MKDEPEND) $(CXXFLAGS) *.cc >> .depend
