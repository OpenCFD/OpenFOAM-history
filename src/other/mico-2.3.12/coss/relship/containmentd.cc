/*
 *  Relationship Service for MICO
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


#include <coss/InterfaceDefsHelper.h>
#include <coss/NamedRoleTypesHelper.h>
#include <coss/CosRelationships.h>
#include <coss/RoleFactory_impl.h>
#include <coss/RelationshipFactory_impl.h>
#include <coss/CosContainment.h>
#include <coss/CosContainment_impl.h>


int main( int argc, char *argv[] )
{
  CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );

  CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
  PortableServer::POAManager_var mgr = poa->the_POAManager();


  InterfaceDefsHelper* idefs_contains = new InterfaceDefsHelper (orb);
  //idefs_contains->add ("IDL:Documents/Figure:1.0");
  //idefs_contains->add ("IDL:Documents/Text:1.0");
  idefs_contains->add ("IDL:omg.org/CosGraphs/Node:1.0");

  InterfaceDefsHelper* idefs_containedin = new InterfaceDefsHelper (orb);
  //idefs_containedin->add ("IDL:Documents/Figure:1.0");
  //idefs_containedin->add ("IDL:Documents/Text:1.0");
  idefs_containedin->add ("IDL:omg.org/CosGraphs/Node:1.0"); 

  RoleFactory_impl<ContainsRole_impl>* contains_role_factory 
    = new RoleFactory_impl<ContainsRole_impl>;

  contains_role_factory->set_max_cardinality (4294967295ul);
  contains_role_factory->set_related_object_types 
    (idefs_contains->get_interfaces ());
  contains_role_factory->set_orb (orb);

  RoleFactory_impl<ContainedInRole_impl>* containedin_role_factory 
    = new RoleFactory_impl<ContainedInRole_impl>;

  containedin_role_factory->set_max_cardinality (1);
  containedin_role_factory->set_related_object_types 
    (idefs_containedin->get_interfaces ());
  containedin_role_factory->set_orb (orb);
  

  PortableServer::ObjectId_var contains_id 
    = PortableServer::string_to_ObjectId ("ContainsRole_impl");
  PortableServer::ObjectId_var containedin_id 
    = PortableServer::string_to_ObjectId ("ContainedInRole_impl");
  


  NamedRoleTypesHelper*
    nrth_containment = new NamedRoleTypesHelper (orb);
  nrth_containment->add ("IDL:omg.org/CosContainment/ContainsRole:1.0", 
			 "ContainsRole");
  nrth_containment->add ("IDL:omg.org/CosContainment/ContainedInRole:1.0", 
			 "ContainedInRole");
  

  RelationshipFactory_impl<Containment_impl>* containment_factory 
    = new RelationshipFactory_impl<Containment_impl> (orb);
  
  containment_factory->set_orb (orb);
  containment_factory->set_named_role_types (nrth_containment
					     ->get_named_role_types ());
  containment_factory->set_degree (2);

  PortableServer::ObjectId_var containment_id 
    = PortableServer::string_to_ObjectId ("Containment_impl");
   

  poa->activate_object_with_id (*contains_id, contains_role_factory);
  poa->activate_object_with_id (*containedin_id, containedin_role_factory);
  poa->activate_object_with_id (*containment_id, containment_factory);
   
  mgr->activate ();

  orb->run ();

  poa->destroy (TRUE, TRUE);

  return 0;
}

