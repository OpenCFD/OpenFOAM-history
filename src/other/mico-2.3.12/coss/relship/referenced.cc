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
#include <coss/CosReference.h>
#include <coss/CosReference_impl.h>


int main( int argc, char *argv[] )
{
  CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );

  CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
  PortableServer::POAManager_var mgr = poa->the_POAManager();


  InterfaceDefsHelper* idefs_references = new InterfaceDefsHelper (orb);
  idefs_references->add ("IDL:omg.org/CosGraphs/Node:1.0");

  InterfaceDefsHelper* idefs_referencedby = new InterfaceDefsHelper (orb);
  idefs_referencedby->add ("IDL:omg.org/CosGraphs/Node:1.0");


  RoleFactory_impl<ReferencesRole_impl>* references_role_factory 
    = new RoleFactory_impl<ReferencesRole_impl>;

  references_role_factory->set_max_cardinality (4294967295ul);
  references_role_factory->set_related_object_types 
    (idefs_references->get_interfaces ());
  references_role_factory->set_orb (orb);

  RoleFactory_impl<ReferencedByRole_impl>* referencedby_role_factory 
    = new RoleFactory_impl<ReferencedByRole_impl>;

  referencedby_role_factory->set_max_cardinality (4294967295ul);
  referencedby_role_factory->set_related_object_types 
    (idefs_referencedby->get_interfaces ());
  referencedby_role_factory->set_orb (orb);
  

  PortableServer::ObjectId_var references_id 
    = PortableServer::string_to_ObjectId ("ReferencesRole_impl");
  PortableServer::ObjectId_var referencedby_id 
    = PortableServer::string_to_ObjectId ("ReferencedByRole_impl");
  

  NamedRoleTypesHelper*
    nrth_reference = new NamedRoleTypesHelper (orb);
  nrth_reference->add ("IDL:omg.org/CosReference/ReferencesRole:1.0", 
			 "ReferencesRole");
  nrth_reference->add ("IDL:omg.org/CosReference/ReferencedByRole:1.0", 
			 "ReferencedByRole");
  

   RelationshipFactory_impl<Reference_impl>* reference_factory 
     = new RelationshipFactory_impl<Reference_impl> (orb);
  
   reference_factory->set_orb (orb);
   reference_factory->set_named_role_types (nrth_reference
 					   ->get_named_role_types ());
   reference_factory->set_degree (2);

   PortableServer::ObjectId_var reference_id 
     = PortableServer::string_to_ObjectId ("Reference_impl");


  poa->activate_object_with_id (*references_id, references_role_factory);
  poa->activate_object_with_id (*referencedby_id, referencedby_role_factory);
  poa->activate_object_with_id (*reference_id, reference_factory);

  mgr->activate ();
  orb->run ();

  poa->destroy (TRUE, TRUE);

  return 0;
}

