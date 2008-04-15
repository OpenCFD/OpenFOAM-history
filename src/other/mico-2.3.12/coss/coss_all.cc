#include <CORBA.h>

#ifdef RELSHIP
#include "relship/CosObjectIdentity.cc"
#include "relship/CosRelationships.cc"
#include "relship/CosGraphs.cc"
#include "relship/CosContainment.cc"
#include "relship/CosReference.cc"
#include "relship/CosGraphsExtension.cc"
#include "relship/IdentifiableObject_impl.cc"
#include "relship/Role_impl.cc"
#include "relship/Relationship_impl.cc"
#include "relship/RelationshipIterator_impl.cc"
#include "relship/TraversalFactory_impl.cc"
#include "relship/Traversal_impl.cc"
#include "relship/TraversalCriteria_impl.cc"
#include "relship/Node_impl.cc"
#include "relship/GraphsRole_impl.cc"
#include "relship/EdgeIterator_impl.cc"
#include "relship/CosContainment_impl.cc"
#include "relship/CosReference_impl.cc"
#include "relship/GenericCriteriaFactory_impl.cc"
#include "relship/RandomGenerator.cc"
#include "relship/random.cc"
#include "relship/RandomGenerator_impl.cc"
#include "relship/InterfaceDefsHelper.cc"
#include "relship/NamedRoleTypesHelper.cc"
#endif // RELSHIP

#ifdef TIME
#include "time/CosTime.cc"
#endif // TIME

#ifdef NAMING
#include "naming/CosNaming.cc"
#endif // NAMING

#ifdef EVENTS
#include "events/CosEventComm.cc"
#include "events/CosEventChannelAdmin.cc"
#endif // EVENTS

#ifdef PROPERTY
#include "property/PropertyService.cc"
#include "property/PropertyService_impl.cc"
#endif // PROPERTY

#ifdef TRADER
#include "trader/CosTrading.cc"
#include "trader/CosTradingRepos.cc"
#endif // TRADER

#ifdef STREAMS
#include "streams/stream.cc"
#include "streams/stream_impl.cc"
#endif // STREAMS

#ifdef WIRELESS
#include "wireless/MobileTerminal.cc"
#include "wireless/GTP.cc"
#include "wireless/MobileTerminalNotification.cc"
#endif // WIRELESS
