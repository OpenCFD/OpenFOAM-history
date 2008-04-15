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

#include <CORBA.h>
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
#include <coss/CosNaming.h>
#include "CosEvent_impl.h"


using namespace std;

void sighandler (int sig)
{
  EventChannelFactory_impl::_shutdown ();
}

void usage (const char *progname)
{
  cerr << "usage: " << progname << " [<options>]" << endl;
  cerr << "possible <options> are:" << endl;
  cerr << "    --regname <naming service name>" << endl;
  cerr << "    --max-queue-size <max event queue size>" << endl;
  exit (1);
}

void register_ns (CORBA::ORB_ptr orb, const char *_name, CORBA::Object_ptr obj)
{
  CORBA::ORB::ObjectIdList_var svlist = orb->list_initial_services ();
  {
      CORBA::ULong i;
      for (i = 0; i < svlist->length(); ++i) {
	  if (!strcmp (svlist[i].in(), "NameService"))
	      break;
      }
      if (i == svlist->length())
	  return;
  }
  CORBA::Object_var nsobj =
      orb->resolve_initial_references ("NameService");

  assert (! CORBA::is_nil (nsobj));
   
   if ((strchr(_name, '.') != NULL) || (strchr(_name, '/') != NULL)) {
      // Try to use the CosNaming::NamingContextExt

      CosNaming::NamingContextExt_var nce;
#ifdef HAVE_EXCEPTIONS      
      try {
	 nce = CosNaming::NamingContextExt::_narrow (nsobj);
      } catch (...) {
	 cerr << "Found no CosNaming::NamingContextExt, use a simple regname." << endl ;
	 exit(1);
      }
#else 
      nce = CosNaming::NamingContextExt::_narrow (nsobj);
#endif
      
      CosNaming::Name_var tmp;
      CosNaming::Name_var name;
#ifdef HAVE_EXCEPTIONS      
      try {
	 name = nce->to_name(_name);
      } catch (CosNaming::NamingContextExt::InvalidAddress) {
	 cerr << "Wrong format for regname!" << endl;
	 exit(1);
      }
#else
      name = nce->to_name(_name);
#endif
      for(CORBA::ULong i=0; i < (name->length()-1); i++) {
	 tmp = name;
	 tmp->length(i+1);
	
#ifdef HAVE_EXCEPTIONS      	 
	 try { 
	    nce->resolve(tmp.in()); 
	 } catch (CosNaming::NamingContext::NotFound) {
	    nce->bind_new_context(tmp.in());
	 }
#else 	 
	 // No idea how to solve this
#endif
      }
      nce->rebind(name.in(), obj);

   } else {
      // Use the ordinary CosNaming::NamingContext

      CosNaming::NamingContext_var nc = 
      CosNaming::NamingContext::_narrow (nsobj);

      CosNaming::Name name;
      name.length (1);
      name[0].id = _name;
      name[0].kind = (const char *)"";
      
#ifdef HAVE_EXCEPTIONS
      try {
	 nc->rebind (name, obj);
      } catch (...) {
	 cerr << "Cannot register with name service !" << endl;
	 exit (1);
      }
#else
      nc->rebind (name, obj);
#endif
   }
}

int main (int argc, char *argv[])
{
  signal (SIGINT, sighandler);
#if 0
  /*
   * when exiting micod sends all running servers a SIGTERM. that is
   * we can't save on SIGTERM ...
   */
  signal (SIGTERM, sighandler);
#endif

  // ORB initialization
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");
  PortableServer::POA_var poa = PortableServer::POA::_narrow(orb->resolve_initial_references("RootPOA"));

  string regname = "EventChannelFactory";
  CORBA::Long max_queue_size = 0x7ffffffful;

  MICOGetOpt::OptMap opts;
  opts["--regname"] = "arg-expected";
  opts["--max-queue-size"] = "arg-expected";

  MICOGetOpt opt_parser (opts);
  if (!opt_parser.parse (argc, argv))
    usage (argv[0]);

  for (MICOGetOpt::OptVec::const_iterator i = opt_parser.opts().begin();
       i != opt_parser.opts().end(); ++i) {
    string arg = (*i).first;
    string val = (*i).second;

    if (arg == "--regname") {
      regname = val;
    } else if (arg == "--max-queue-size") {
      max_queue_size = atoi (val.c_str());
    } else {
      usage( argv[ 0 ] );
    }
  }

  CORBA::PolicyList_var policy_list = new CORBA::PolicyList(2); 
  policy_list->length(2);
  policy_list[(CORBA::ULong)0] = poa->create_lifespan_policy(PortableServer::TRANSIENT);
  policy_list[(CORBA::ULong)1] = poa->create_id_assignment_policy(PortableServer::SYSTEM_ID);

  PortableServer::POAManager_var poa_manager = poa->the_POAManager();
  PortableServer::POA_var factory_poa = poa->create_POA("EventChannelFactoryPOA", poa_manager, policy_list);

  EventChannelFactory_impl* factory_impl =  new EventChannelFactory_impl (max_queue_size);

  factory_poa->activate_object(factory_impl);
  SimpleEventChannelAdmin::EventChannelFactory_var factory = 
      SimpleEventChannelAdmin::EventChannelFactory::_narrow(factory_poa->servant_to_reference(factory_impl));

  poa_manager->activate();

  register_ns (orb, regname.c_str(), factory.in());

  orb->run ();
  return 0;
}
