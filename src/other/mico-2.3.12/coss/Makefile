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

CXXFLAGS := -I../include -I../include/coss $(CXXFLAGS) $(EHFLAGS)

PLATFORM_LIBS =
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

ifeq ($(USE_NAMING), yes)
FINAL_FLAG := -DNAMING
COSS_INCLUDE := -Inaming
SUBDIRS := $(SUBDIRS) naming
endif

ifeq ($(USE_EVENTS), yes)
FINAL_FLAG += -DEVENTS
COSS_INCLUDE += -Ievents
SUBDIRS := $(SUBDIRS) events
endif

ifeq ($(USE_STREAMS), yes)
FINAL_FLAG += -DSTREAMS
COSS_INCLUDE += -Istreams
SUBDIRS := $(SUBDIRS) streams
endif

ifeq ($(USE_RELSHIP), yes)
FINAL_FLAG += -DRELSHIP
COSS_INCLUDE += -Irelship
SUBDIRS := $(SUBDIRS) relship
endif

ifeq ($(USE_PROPERTY), yes)
FINAL_FLAG += -DPROPERTY
COSS_INCLUDE += -Iproperty
SUBDIRS := $(SUBDIRS) property
endif

ifeq ($(USE_TRADER), yes)
FINAL_FLAG += -DTRADER
COSS_INCLUDE += -Itrader
SUBDIRS := $(SUBDIRS) trader
endif

ifeq ($(USE_TIME), yes)
FINAL_FLAG += -DTIME
COSS_INCLUDE += -Itime
SUBDIRS := $(SUBDIRS) time
endif

ifeq ($(USE_LIFE), yes)
FINAL_FLAG += -DLIFE
COSS_INCLUDE += -Ilifecycle
SUBDIRS := $(SUBDIRS) lifecycle
endif

ifeq ($(USE_EXTERN), yes)
FINAL_FLAG += -DEXTERN
COSS_INCLUDE += -Iexternalization
SUBDIRS := $(SUBDIRS) externalization
endif

ifeq ($(USE_WIRELESS), yes)
FINAL_FLAG += -DWIRELESS
COSS_INCLUDE += -Iwireless
SUBDIRS := $(SUBDIRS) wireless
endif

ifeq ($(HAVE_FINAL), yes)
CXXFLAGS := $(CXXFLAGS) $(FINAL_FLAG) $(COSS_INCLUDE)
endif

CLEANDIRS = naming events streams relship property trader time lifecycle externalization wireless

LDLIBS    = -L../orb $(LDFLAGS) $(LDMICO) $(CONFLIBS)

#STATIC_OBJS = \

ifeq ($(USE_NAMING), yes)
  STATIC_OBJS += naming/CosNaming.o
endif

ifeq ($(USE_EVENTS), yes)
  STATIC_OBJS += events/CosEventComm.o events/CosEventChannelAdmin.o
endif

ifeq ($(USE_STREAMS), yes)
  STATIC_OBJS += streams/stream.o streams/stream_impl.o
endif

ifeq ($(USE_RELSHIP), yes)
  STATIC_OBJS += relship/CosObjectIdentity.o relship/CosRelationships.o
  STATIC_OBJS += relship/CosGraphs.o
  STATIC_OBJS += relship/CosContainment.o relship/CosReference.o
  STATIC_OBJS += relship/CosGraphsExtension.o
  STATIC_OBJS += relship/IdentifiableObject_impl.o relship/Role_impl.o
  STATIC_OBJS += relship/Relationship_impl.o relship/RelationshipIterator_impl.o 
  STATIC_OBJS += relship/TraversalFactory_impl.o relship/Traversal_impl.o
  STATIC_OBJS += relship/TraversalCriteria_impl.o relship/Node_impl.o
  STATIC_OBJS += relship/GraphsRole_impl.o relship/EdgeIterator_impl.o
  STATIC_OBJS += relship/CosContainment_impl.o relship/CosReference_impl.o
  STATIC_OBJS += relship/GenericCriteriaFactory_impl.o
  STATIC_OBJS += relship/RandomGenerator.o relship/random.o
  STATIC_OBJS += relship/RandomGenerator_impl.o
  STATIC_OBJS += relship/InterfaceDefsHelper.o relship/NamedRoleTypesHelper.o
endif

ifeq ($(USE_PROPERTY), yes)
  STATIC_OBJS +=   property/PropertyService_impl.o property/PropertyService.o
endif

ifeq ($(USE_TRADER), yes)
  STATIC_OBJS += trader/CosTrading.o trader/CosTradingRepos.o
endif

ifeq ($(USE_TIME), yes)
  STATIC_OBJS += time/CosTime.o
endif

ifeq ($(USE_LIFE), yes)
  STATIC_OBJS += lifecycle/CosLifeCycle.o
  STATIC_OBJS += lifecycle/CosCompoundLifeCycle.o
  STATIC_OBJS += lifecycle/CosLifeCycleContainment.o lifecycle/CosLifeCycleReference.o
  STATIC_OBJS += lifecycle/FactoryFinder_impl.o
  STATIC_OBJS += lifecycle/GenericFactory_impl.o
  STATIC_OBJS += lifecycle/LifeRole_impl.o
  STATIC_OBJS += lifecycle/LifeContainment_impl.o
  STATIC_OBJS += lifecycle/LifeNode_impl.o
  STATIC_OBJS += lifecycle/LifePropagationCriteriaFactory_impl.o
  STATIC_OBJS += lifecycle/LifeReference_impl.o
  STATIC_OBJS += lifecycle/LifeRelationship_impl.o
  STATIC_OBJS += lifecycle/LifeTraversalCriteria_impl.o
  STATIC_OBJS += lifecycle/Operations_impl.o
  STATIC_OBJS += lifecycle/RegisterHelper.o
endif

ifeq ($(USE_EXTERN), yes)
  STATIC_OBJS += externalization/CosExternalization.o
  STATIC_OBJS += externalization/CosExternalizationContainment.o externalization/CosExternalizationReference.o
  STATIC_OBJS += externalization/CompoundExternalization_impl.o
  STATIC_OBJS += externalization/ExternalizationPropagationCriteriaFactory_impl.o
  STATIC_OBJS += externalization/ExternalizationTraversalCriteria_impl.o
  STATIC_OBJS += externalization/Externalization_impl.o
  STATIC_OBJS += externalization/Stream_impl.o
  STATIC_OBJS += externalization/ExternalizationContainment_impl.o
  STATIC_OBJS += externalization/ExternalizationReference_impl.o
endif

ifeq ($(USE_WIRELESS), yes)
  STATIC_OBJS += wireless/MobileTerminal.o wireless/GTP.o
  STATIC_OBJS += wireless/MobileTerminalNotification.o
endif

ifeq ($(HAVE_FINAL), yes)
STATIC_OBJS := coss_all.o
endif

SHARED_OBJS = $(STATIC_OBJS:.o=.pic.o)

# mingw hack

ifeq ($(OSTYPE), windows)
ifeq ($(HAVE_SHARED), yes)
HAVE_SHARED_EXCEPTS := no
HAVE_SHARED := no
HAVE_STATIC := yes
endif
endif


all: lib prg

lib:

ifneq ($(strip $(SUBDIRS)),)
ifeq ($(HAVE_SHARED_EXCEPTS), yes)
ifeq ($(HAVE_STATIC), yes)
prg .NOTPARALLEL: sublib libmicocoss$(VERSION).a libmicocoss$(VERSION).$(SOEXT) subprg
else
prg .NOTPARALLEL: sublib libmicocoss$(VERSION).$(SOEXT) subprg
endif
else
prg .NOTPARALLEL: sublib libmicocoss$(VERSION).a subprg
endif
else
prg:
endif

sublib: idlcompile
	for i in $(SUBDIRS); do $(MAKE) -C $$i lib || exit 1; done

subprg:
	for i in $(SUBDIRS); do $(MAKE) -C $$i prg || exit 1; done


# shared 

ifeq ($(HAVE_SHARED_EXCEPTS), yes)
libmicocoss$(VERSION).$(SOEXT): $(SHARED_OBJS)
ifeq ($(HAVE_REPO), yes)
	-$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(SHARED_OBJS) $(LDFLAGS) $(CONFLIBS)
endif
ifeq ($(HAVE_PRELINK), yes)
	$(CXX) $(PRELINK_FLAG) $(SHARED_OBJS) $(LDFLAGS) $(CONFLIBS)
endif
	$(RM) $@
	$(LDSO) -o libmicocoss$(VERSION) $(SHARED_OBJS) \
		-L../libs $(LDMICO) $(PLATFORM_LIBS)
	$(IDIRCMD) ../libs
	cd ../libs; $(RM) $@; $(LN) ../coss/$@ .
endif

# static

libmicocoss$(VERSION).a: $(STATIC_OBJS)
ifeq ($(HAVE_REPO), yes)
	-$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(STATIC_OBJS) $(LDFLAGS) $(CONFLIBS)
endif
ifeq ($(HAVE_PRELINK), yes)
	$(CXX) $(PRELINK_FLAG) $(CXXFLAGS) $(CPPFLAGS) $(STATIC_OBJS) \
		$(LDFLAGS) -L../libs $(LDMICO) 
endif
	$(AR) $(ARFLAGS) $@ $(STATIC_OBJS)
	$(RANLIB) $@
	$(IDIRCMD) ../libs
	cd ../libs; $(RM) $@; $(LN) ../coss/$@ .


clean:
	for i in $(CLEANDIRS); do $(MAKE) -C $$i clean || exit 1; done
	-$(RM) *.o *.o0 *.a *~ *.$(SOEXT) *.exe *.rpo *.objid
	-$(RM) .depend TAGS core nohup.out

idlcompile:
	for i in $(SUBDIRS); do $(MAKE) -C $$i idlcompile || exit 1; done

.depend:
	for i in $(SUBDIRS); do $(MAKE) -C $$i .depend || exit 1; done
install:
	$(IDIRCMD) $(INSTDIR)/lib
ifneq ($(strip $(SUBDIRS)),)
ifeq ($(HAVE_SHARED_EXCEPTS), yes)
	$(ILIBCMD) libmicocoss$(VERSION).$(SOEXT) $(INSTDIR)/lib
ifeq ($(HAVE_STATIC), yes)
	$(ILIBCMD) libmicocoss$(VERSION).a $(INSTDIR)/lib
endif
else
	$(ILIBCMD) libmicocoss$(VERSION).a $(INSTDIR)/lib
endif
	for i in $(SUBDIRS); do $(MAKE) -C $$i install || exit 1; done	
endif

install-cd: install

