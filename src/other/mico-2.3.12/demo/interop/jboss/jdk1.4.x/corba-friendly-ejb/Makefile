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

all: .depend CorbaFriendlySession.jar client

DIR_PREFIX=../../../

include ../../../../MakeVars
CXXFLAGS = -I. -I$(MICO_HOME)/include
LDFLAGS  = -L$(MICO_HOME)/libs
IDLFLAGS = -I. -I$(MICO_HOME)/include

.SUFFIXES: .idl

%.h %.cc: %.idl
	$(IDL) -I. $(IDL_INCL) -DprimaryKey=_primaryKey \
               --ifdef-prefix=$(subst /,_,$(<D)) $<
	mv $(*F).cc $(<D)
	mv $(*F).h $(<D)

CXXFLAGS  := $(COS_CXXFLAGS) $(CXXFLAGS)
LDLIBS    := $(COS_LDLIBS) $(LDLIBS)
LDFLAGS   := $(COS_LDFLAGS) $(LDFLAGS)
DEPS      := $(COS_DEPS) $(DEPS)

IDL_INCL="-I$(MICO_HOME)/include/mico -I$(MICO_HOME)/include"

IDL_FILES= examples/CorbaFriendlySession.idl \
           examples/CorbaFriendlySessionHome.idl \
           examples/corba/DataObject.idl \
           org/omg/boxedIDL/examples/corba/DataObject.idl \
           org/omg/boxedRMI/org/omg/boxedIDL/examples/corba/seq1_DataObject.idl \
           javax/ejb/EJBHome.idl \
           javax/ejb/EJBMetaData.idl \
           javax/ejb/HomeHandle.idl \
           javax/ejb/RemoveException.idl \
           javax/ejb/RemoveEx.idl \
           javax/ejb/Handle.idl \
           javax/ejb/EJBObject.idl \
           javax/ejb/CreateException.idl \
           javax/ejb/CreateEx.idl \
           java/lang/Throwable.idl \
           java/lang/ThrowableEx.idl \
           java/lang/Exception.idl \
           java/lang/Ex.idl \
           java/lang/Object.idl \
           java/lang/StackTraceElement.idl \
           org/omg/boxedRMI/java/lang/seq1_StackTraceElement.idl

examples/corba/DataObject.java \
examples/corba/DataObjectHelper.java \
examples/corba/DataObjectHolder.java: examples/corba/DataObject.idl
	idlj examples/corba/DataObject.idl

examples/CorbaFriendlySession.class: examples/CorbaFriendlySession.java \
                                 examples/corba/DataObject.class

examples/CorbaFriendlySession.idl: examples/CorbaFriendlySession.class
	rmic -classpath $(JBOSS_HOME)/client/jboss-j2ee.jar:. \
             -idl -noValueMethods examples.CorbaFriendlySession
	cp jdk1.4-Throwable/Throwable.idl java/lang
	cp jdk1.4-Throwable/StackTraceElement.idl java/lang
	test -d org/omg/boxedRMI/java || mkdir org/omg/boxedRMI/java
	test -d org/omg/boxedRMI/java/lang || mkdir org/omg/boxedRMI/java/lang
	cp jdk1.4-Throwable/seq1_StackTraceElement.idl \
            org/omg/boxedRMI/java/lang/

examples/CorbaFriendlySessionHome.idl: examples/CorbaFriendlySessionHome.class
	rmic -classpath $(JBOSS_HOME)/client/jboss-j2ee.jar:. \
             -idl -noValueMethods examples.CorbaFriendlySessionHome
	cp jdk1.4-Throwable/Throwable.idl java/lang
	cp jdk1.4-Throwable/StackTraceElement.idl java/lang
	test -d org/omg/boxedRMI/java || mkdir org/omg/boxedRMI/java
	test -d org/omg/boxedRMI/java/lang || mkdir org/omg/boxedRMI/java/lang
	cp jdk1.4-Throwable/seq1_StackTraceElement.idl \
             org/omg/boxedRMI/java/lang/

CorbaFriendlySession.jar: examples/CorbaFriendlySession.class \
                          examples/CorbaFriendlySessionHome.class \
                          examples/CorbaFriendlySessionBean.class \
                          examples/corba/DataObject.class \
                          examples/corba/DataObjectHelper.class \
                          examples/corba/DataObjectHolder.class \
                          META-INF/ejb-jar.xml \
                          META-INF/jboss.xml
	jar cf CorbaFriendlySession.jar $+

client: client.o $(subst .idl,.o,$(IDL_FILES)) $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) $(subst .idl,.o,$(IDL_FILES)) $< -o $@

client.o: client.cc $(subst .idl,.h,$(IDL_FILES))

clean:
	rm -f .depend core client *.o *.jar `find . -name '*~' -print`
	rm -f examples/*.idl examples/*.cc examples/*.h examples/*.o
	rm -f examples/corba/*.java examples/corba/*.class examples/corba/*.o
	rm -f examples/corba/*.h examples/corba/*.cc examples/*.class 
	rm -rf java javax org

install:
	$(IDIRCMD) $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.4.x/corba-friendly-ejb/META-INF
	$(IDIRCMD) $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.4.x/corba-friendly-ejb/examples
	$(IDIRCMD) $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.4.x/corba-friendly-ejb/examples/corba
	$(IDIRCMD) $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.4.x/corba-friendly-ejb/jdk1.4-Throwable
	$(ISRCCMD) Makefile $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.4.x/corba-friendly-ejb
	$(ISRCCMD) client.cc $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.4.x/corba-friendly-ejb
	$(ISCRIPTCMD) run-client $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.4.x/corba-friendly-ejb
	$(ISRCCMD) META-INF/ejb-jar.xml $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.4.x/corba-friendly-ejb/META-INF
	$(ISRCCMD) META-INF/jboss.xml $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.4.x/corba-friendly-ejb/META-INF
	$(ISRCCMD) examples/CorbaFriendlySession.java $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.4.x/corba-friendly-ejb/examples
	$(ISRCCMD) examples/CorbaFriendlySessionBean.java $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.4.x/corba-friendly-ejb/examples
	$(ISRCCMD) examples/CorbaFriendlySessionHome.java $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.4.x/corba-friendly-ejb/examples
	$(ISRCCMD) examples/CorbaFriendlySession.cc.saved $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.4.x/corba-friendly-ejb/examples
	$(ISRCCMD) examples/CorbaFriendlySession.h.saved $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.4.x/corba-friendly-ejb/examples
	$(ISRCCMD) examples/corba/DataObject.idl $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.4.x/corba-friendly-ejb/examples/corba
	$(ISRCCMD) jdk1.4-Throwable/StackTraceElement.idl $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.4.x/corba-friendly-ejb/jdk1.4-Throwable
	$(ISRCCMD) jdk1.4-Throwable/Throwable.idl $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.4.x/corba-friendly-ejb/jdk1.4-Throwable
	$(ISRCCMD) jdk1.4-Throwable/seq1_StackTraceElement.idl $(INSTDIR)/doc/mico/examples/interop/jboss/jdk1.4.x/corba-friendly-ejb/jdk1.4-Throwable
