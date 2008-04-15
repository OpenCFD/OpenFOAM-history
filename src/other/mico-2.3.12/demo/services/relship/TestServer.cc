/*
 *  Test of COSS Relationship Service for MICO
 *  Copyright (C) 1998-99 Karel Gardas
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  Send comments and/or bug reports to:
 *                 mico@informatik.uni-frankfurt.de
 *  or to my private e-mail:
 *                 gardask@alpha.inf.upol.cz
 */


//#define DEBUG 1

#include <coss/InterfaceDefsHelper.h>
#include <coss/NamedRoleTypesHelper.h>
#include <coss/CosRelationships.h>
#include <coss/RoleFactory_impl.h>
#include <coss/RelationshipFactory_impl.h>
#include "Test.h"
#include "Test_impl.h"


int main( int argc, char *argv[] )
{
  CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );

  CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
  PortableServer::POAManager_var mgr = poa->the_POAManager();


  InterfaceDefsHelper* idefs_base = new InterfaceDefsHelper (orb);
  idefs_base->add ("IDL:Documents/Figure:1.0");
  idefs_base->add ("IDL:Documents/Text:1.0");
  idefs_base->add ("IDL:omg.org/CosGraphs/Node:1.0");

  InterfaceDefsHelper* idefs_derived1 = new InterfaceDefsHelper (orb);
  idefs_derived1->add ("IDL:omg.org/CosGraphs/Node:1.0"); 
  idefs_derived1->add ("IDL:Documents/Figure:1.0");
  idefs_derived1->add ("IDL:Documents/Text:1.0");

  InterfaceDefsHelper* idefs_derived2 = new InterfaceDefsHelper (orb);
  idefs_derived2->add ("IDL:omg.org/CosGraphs/Node:1.0"); 
  idefs_derived2->add ("IDL:Documents/Figure:1.0");
  idefs_derived2->add ("IDL:Documents/Text:1.0");

  RoleFactory_impl<BaseRole_impl>* base_role_factory 
    = new RoleFactory_impl<BaseRole_impl>;

  base_role_factory->set_max_cardinality (1000);
  base_role_factory->set_related_object_types 
    (idefs_base->get_interfaces ());
  base_role_factory->set_orb (orb);

  RoleFactory_impl<DerivedRole1_impl>* derived_role1_factory 
    = new RoleFactory_impl<DerivedRole1_impl>;

  derived_role1_factory->set_max_cardinality (1);
  derived_role1_factory->set_related_object_types 
    (idefs_derived1->get_interfaces ());
  derived_role1_factory->set_orb (orb);

  RoleFactory_impl<DerivedRole2_impl>* derived_role2_factory 
    = new RoleFactory_impl<DerivedRole2_impl>;

  derived_role2_factory->set_max_cardinality (1);
  derived_role2_factory->set_related_object_types 
    (idefs_derived2->get_interfaces ());
  derived_role2_factory->set_orb (orb);
  

  PortableServer::ObjectId_var base_id 
    = PortableServer::string_to_ObjectId ("BaseRole_impl");
  PortableServer::ObjectId_var derived1_id 
    = PortableServer::string_to_ObjectId ("DerivedRole1_impl");
  PortableServer::ObjectId_var derived2_id 
    = PortableServer::string_to_ObjectId ("DerivedRole2_impl");
  


  NamedRoleTypesHelper*
    nrth_TestRelationship = new NamedRoleTypesHelper (orb);
  nrth_TestRelationship->add ("IDL:omg.org/Test/BaseRole:1.0", 
			 "BaseRole");
  nrth_TestRelationship->add ("IDL:omg.org/Test/DerivedRole1:1.0", 
			      "DerivedRole1");
  

  RelationshipFactory_impl<TestRelationship_impl>* test_relationship_factory 
    = new RelationshipFactory_impl<TestRelationship_impl> (orb);
  
  test_relationship_factory->set_orb (orb);
  test_relationship_factory->set_named_role_types (nrth_TestRelationship
						   ->get_named_role_types ());
  test_relationship_factory->set_degree (2);

  PortableServer::ObjectId_var test_relationship_id 
    = PortableServer::string_to_ObjectId ("TestRelationship_impl");
    
  poa->activate_object_with_id (*base_id, base_role_factory);
  poa->activate_object_with_id (*derived1_id, derived_role1_factory);
  poa->activate_object_with_id (*derived2_id, derived_role2_factory);
  poa->activate_object_with_id (*test_relationship_id, 
				test_relationship_factory);
  
  mgr->activate ();
#if DEBUG
  cout << "  <TestServer> Ready\n";
#endif
  orb->run ();

  poa->destroy (TRUE, TRUE);
#if DEBUG
  cout << "  <TestServer> END!\n";
#endif
  return 0;
}

