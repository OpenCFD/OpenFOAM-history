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

#define MICO_CONF_IR
#define MICO_CONF_POA
#define MICO_CONF_INTERCEPT
#include <CORBA-SMALL.h>
#include <coss/CosNaming.h>
#include <coss/PropertyService_impl.h>
#include <mico/util.h>


using namespace std;

void usage (const char *progname)
{
  cerr << "usage: " << progname << " [<options>]" << endl;
  cerr << "possible <options> are:" << endl;
  cerr << "    --regname1 <naming service name for property set factory>"
       << endl;
  cerr << "    --regname2 <naming service name for property set def factory>"
       << endl;
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
  // ORB initialization
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");
  CORBA::Object_ptr obj = orb->resolve_initial_references("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);
  PortableServer::POAManager_var mgr = poa->the_POAManager();

  string regname1 = "PropertySetFactory";
  string regname2 = "PropertySetDefFactory";

  MICOGetOpt::OptMap opts;
  opts["--regname1"] = "arg-expected";
  opts["--regname2"] = "arg-expected";

  MICOGetOpt opt_parser (opts);
  if (!opt_parser.parse (argc, argv))
    usage (argv[0]);

  for (MICOGetOpt::OptVec::const_iterator i = opt_parser.opts().begin();
       i != opt_parser.opts().end(); ++i) {
    string arg = (*i).first;
    string val = (*i).second;

    if (arg == "--regname1") {
      regname1 = val;
    } else if (arg == "--regname2") {
      regname2 = val;
    } else {
      usage( argv[ 0 ] );
    }
  }

  PropertySetFactory_impl* factory1 = new PropertySetFactory_impl;
  PropertyService::PropertySetFactory_ptr f1_ref = factory1->_this();
  register_ns (orb, regname1.c_str(), f1_ref);

  PropertySetDefFactory_impl* factory2 = new PropertySetDefFactory_impl;
  PropertyService::PropertySetDefFactory_ptr f2_ref = factory2->_this();
  register_ns (orb, regname2.c_str(), f2_ref);

  mgr->activate();
  orb->run ();
  return 0;
}
