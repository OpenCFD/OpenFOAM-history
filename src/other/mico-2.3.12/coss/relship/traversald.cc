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


#include <coss/CosGraphs.h>
#include <coss/TraversalFactory_impl.h>
// extension
#include <coss/CosGraphsExtension.h>
#include <coss/GenericCriteriaFactory_impl.h>

#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif
#include <unistd.h>

//#define DEBUG 0

int main( int argc, char *argv[] )
{
  CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );

  CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
  PortableServer::POAManager_var mgr = poa->the_POAManager();
  
  TraversalFactory_impl* trf = new TraversalFactory_impl;
  PortableServer::ObjectId_var oid = poa->activate_object (trf);

  // extension
  GenericCriteriaFactory_impl* gcf = new GenericCriteriaFactory_impl;
  PortableServer::ObjectId_var oid2 = poa->activate_object (gcf);

  mgr->activate ();
  orb->run ();

  poa->destroy (TRUE, TRUE);

  return 0;
}





