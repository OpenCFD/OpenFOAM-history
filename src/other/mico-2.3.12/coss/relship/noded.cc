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
#include <coss/CosGraphs.h>
#include <coss/NodeFactory_impl.h>
#include <coss/Node_impl.h>


int main( int argc, char *argv[] )
{
  CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );

  CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
  PortableServer::POAManager_var mgr = poa->the_POAManager();


  NodeFactory_impl<Node_impl>* node_factory 
    = new NodeFactory_impl<Node_impl> (orb);

  PortableServer::ObjectId_var node_factory_id 
    = PortableServer::string_to_ObjectId ("Node_impl");

  poa->activate_object_with_id (*node_factory_id, node_factory);

  mgr->activate ();
  orb->run ();

  poa->destroy (TRUE, TRUE);

  return 0;
}

