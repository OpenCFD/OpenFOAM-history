//
// Corba TimeService (according to basic specs)
//
// Copyright (C) 1999 C. Zerbst
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// Send comments and/or bug reports to:
//                zerbst@tu-harburg.de
//

#include "CosTime.h"
#include "TimeService_impl.h"
#include "TimeService_help.h"
#include <coss/CosNaming.h>
#include <mico/util.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif


using namespace std;

//taken from MICO PropertyService
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

void usage (const char *progname)
{
  cerr << "usage: " << progname << " [<options>]" << endl;
  cerr << "possible <options> are:" << endl;
  cerr << "    --ior" << endl;
  cerr << "    --regname <naming service name>" << endl;
  exit (1);
}


int main(int argc, char ** argv) {
   
   CORBA::ORB_var orb = CORBA::ORB_init(argc,argv);
   CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
   PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);
   PortableServer::POAManager_var mgr = poa->the_POAManager();

   TimeService_impl *ts = new TimeService_impl;
   CosTime::TimeService_var tsV = ts->_this();
   
   string regname = "TimeService";
   CORBA::Boolean printior = FALSE;

   MICOGetOpt::OptMap opts;
   opts["--regname"] = "arg-expected";
   opts["--ior"]     = "";

   MICOGetOpt opt_parser (opts);
   if (!opt_parser.parse (argc, argv))
       usage (argv[0]);

   for (MICOGetOpt::OptVec::const_iterator i = opt_parser.opts().begin();
        i != opt_parser.opts().end(); ++i) {
       string arg = (*i).first;
       string val = (*i).second;
       
       if (arg == "--regname") {
           regname = val;
       } else if (arg == "--ior") {
           printior = TRUE;
       } else {
           usage( argv[ 0 ] );
       }
   }

   if (printior) {
       CORBA::String_var str = orb->object_to_string(tsV);
       cout << str << endl;
   }

   register_ns (orb, regname.c_str(), tsV.in());


   mgr->activate ();
   orb->run();
   
   return 1;
}
