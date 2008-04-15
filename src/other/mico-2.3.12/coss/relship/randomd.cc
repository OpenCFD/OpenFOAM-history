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


#include <coss/RandomGenerator_impl.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif
#include <unistd.h>


using namespace std;

void usage (char* args[])
{
  cout << "usage: " << args[0] << " --generate | --proxy <cache size>\n\n";
}


int main (int argc, char *argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );
  
  CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
  PortableServer::POAManager_var mgr = poa->the_POAManager();

  if (argc < 2 || argc > 3) {
    usage (argv);
    exit (1);
  }
  RandomGenerator_impl* generator;

  if (argc == 2) {
    if (strcmp (argv[1], "--generate") == 0) {
      generator = new RandomGenerator_impl (RandomGenerator::generate);
    }
    else {
      usage (argv);
      exit (1);
    }
  }

  long size = -1;
  if (argc == 3) {
    if (strcmp (argv[1], "--proxy") == 0) {
      size = atol (argv[2]);
      if (size > 0) {
	generator = new RandomGenerator_impl (RandomGenerator::proxy);
	generator->cache_size (size);
      }
      else {
	usage (argv);
	exit (1);
      }
    }
    else {
      usage (argv);
      exit (1);
    }
  }

  PortableServer::ObjectId_var oid = poa->activate_object (generator);

  mgr->activate ();
  
  orb->run ();

  poa->destroy (TRUE, TRUE);
  
  return 0;
}







