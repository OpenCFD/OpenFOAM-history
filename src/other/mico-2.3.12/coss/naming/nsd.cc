/*
 *  MICO --- a free CORBA implementation
 *  Copyright (C) 1997-98 Kay Roemer & Arno Puder
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
 */

#define MICO_CONF_IMR
#define MICO_CONF_POA
#define MICO_CONF_IR
#define MICO_CONF_INTERCEPT
#include <CORBA-SMALL.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif
#include <mico/template_impl.h>
#include <mico/util.h>
#include "Naming_impl.h"


using namespace std;

bool finished;

void sighandler (int sig)
{
  finished = true;
}

void usage (const char *progname)
{
  cerr << "usage: " << progname << " [<options>]" << endl;
  cerr << "possible <options> are:" << endl;
  cerr << "    --help" << endl;
  cerr << "    --ior <IOR ref file>" << endl;
  cerr << "    --db <db file>" << endl;
  cerr << "    --lb <load balancing : round_robin or random>" << endl;
  exit (1);
}

int main (int argc, char *_argv[])
{
  NamingContext_impl::LoadBalanceApproach eLoadBalanceApproach;

  CORBA::ORB_var orb = CORBA::ORB::_nil ();
  signal (SIGINT, sighandler);
  signal (SIGTERM, sighandler);

  /*
   * prevent a bind() to naming service...
   * also add -POAImplName if missing...
   */

  const char * implname = NULL;
  char **argv = new char*[argc+2+2+1];

  for (int i=0; i<argc; ++i) {
    if (strcmp (_argv[i], "-POAImplName") == 0 && i+1<argc) {
      implname = _argv[i+1];
    }
    argv[i] = _argv[i];
  }

  if (!implname) {
    argv[argc++] = "-POAImplName";
    argv[argc++] = "NameService";
  }

  argv[argc++] = "-ORBNamingAddr";
  argv[argc++] = "";

  argv[argc] = 0;

  // ORB initialization
  orb = CORBA::ORB_init (argc, argv, "mico-local-orb");
  CORBA::Object_var po = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var RootPOA = PortableServer::POA::_narrow (po);
  PortableServer::POAManager_var RootMgr = RootPOA->the_POAManager ();

  MICOGetOpt::OptMap opts;
  opts["--help"] = "";
  opts["--ior"]  = "arg-expected";
  opts["--db"]   = "arg-expected";
  opts["--lb"]   = "arg-expected";

  MICOGetOpt opt_parser (opts);
  if (!opt_parser.parse (argc, argv))
    usage (argv[0]);

  string reffile;
  string dbfile;

  // Default load-balancing approach
  eLoadBalanceApproach = NamingContext_impl::NONE;

  {
  for (MICOGetOpt::OptVec::const_iterator i = opt_parser.opts().begin();
       i != opt_parser.opts().end(); ++i) {
    string arg = (*i).first;
    string val = (*i).second;

    if (arg == "--ior") {
      reffile = val;
    } else if (arg == "--db") {
      dbfile = val;
    } else if (arg == "--help") {
      usage (argv[0]);
    } else if (arg == "--lb") {
      if (val == "round_robin")
        eLoadBalanceApproach = NamingContext_impl::ROUND_ROBIN;
      else if (val == "random")
        eLoadBalanceApproach = NamingContext_impl::RANDOM;
    } else {
      usage (argv[0]);
    }
  }
  }
  if (argc != 1) {
    usage (argv[0]);
  }

  /*
   * We create two new Persistent POAs. One for the root NamingContext
   * with the USER_ID policy so that we can select the `NameService' id,
   * and one for all child contexts with the SYSTEM_ID policy
   */

  CORBA::PolicyList pl;
  pl.length (1);
  pl[0] = RootPOA->create_lifespan_policy (PortableServer::PERSISTENT);
  PortableServer::POA_var ncpoa =
    RootPOA->create_POA ("NamingContexts",
			 PortableServer::POAManager::_nil (),
			 pl);
  PortableServer::POAManager_var rcmgr = ncpoa->the_POAManager ();

  pl.length (2);
  pl[1] = RootPOA->create_id_assignment_policy (PortableServer::USER_ID);
  PortableServer::POA_var rcpoa =
    RootPOA->create_POA ("NameService", rcmgr.in(), pl);

  /*
   * Create Root Naming Context
   */

  NamingContextExt_impl * root =
    new NamingContextExt_impl (orb, ncpoa.in(), eLoadBalanceApproach);

  /*
   * Restore.
   */

  if (dbfile.length() > 0) {
    if (OSMisc::access (dbfile.c_str(), OSMisc::ACCESS_READ)) {
      cerr << "warning: database " << dbfile.c_str() << " does not exist."
	   << endl;
    }
    else {
      NamingContext_impl::RestoreNamingService (dbfile.c_str());
    }
  }

  /*
   * Activate
   */

  PortableServer::ObjectId_var rncid =
    PortableServer::string_to_ObjectId ("NameService");
  rcpoa->activate_object_with_id (rncid.in(), root);

  if (reffile.length() > 0) {
    CORBA::Object_var nsobj = rcpoa->servant_to_reference (root);
    CORBA::String_var nsior = orb->object_to_string (nsobj);

    if (reffile == "-") {
      cout << nsior << endl;
    }
    else {
      ofstream out (reffile.c_str());
      if (!out) {
	cerr << "error: cannot open output file " << reffile << endl;
      }
      else {
	out << nsior << endl;
	out.close ();
      }
    }
  }

  /*
   * Go Active
   */

  rcmgr->activate ();
  RootMgr->activate ();

  finished = false;
  while (!finished) {
    orb->perform_work ();
  }

  /*
   * Save
   */

  if (dbfile.length() > 0) {
    NamingContext_impl::SaveNamingService (dbfile.c_str());
  }

  /*
   * Shutdown
   */

  RootPOA->destroy (TRUE, TRUE);
  root->_remove_ref ();
  CORBA::release (orb);

  return 0;
}
