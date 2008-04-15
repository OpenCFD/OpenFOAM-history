/*
 *  COSS Naming Service administration tool for MICO
 *  Copyright (C) 1997 Kai-Uwe Sattler
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

#include <CORBA-SMALL.h>
#include <coss/CosNaming.h>
#include <mico/util.h>
#include "NamingClient.h"
#include <signal.h>
#include <string.h>

#ifdef HAVE_LIBREADLINE
extern "C" {
    extern char *readline (const char *);
    extern void add_history (const char *);
}
#endif


using namespace std;

static const char info[] = "CosNaming administration tool for MICO(tm)";

int parse_cmd (CORBA::ORB_var& orb, NamingClient& nsclient, 
	       vector<string>& cmds)
{
  const char *arg1 = NULL, *arg2 = NULL;
  
  if (cmds.size () > 1)
    arg1 = cmds[1].c_str ();
  if (cmds.size () > 2)
    arg2 = cmds[2].c_str ();

#ifdef HAVE_EXCEPTIONS
  try {
#endif
  if (cmds[0] == "help") {
    cerr << info << "\n"
	 << "available commands:\n" 
	 << "\thelp           - print this message\n"
	 << "\tpwd            - print the current naming context\n"
	 << "\tls [context]   - list the naming context\n"
	 << "\tcd context     - change the current naming context\n"
	 << "\tmkdir context  - create a new naming context\n"
	 << "\tbind name oref - create a new binding for the object\n"
	 << "\tbind_context name oref - connect a naming context\n"
	 << "\tcat entry      - show entry\n"
	 << "\tiordump entry  - iordump entry\n"
	 << "\trepoid entry   - show repoid\n"
	 << "\turl entry      - show corbaname url for entry\n"
	 << "\tlocurl entry   - show corbaloc url for entry\n"
	 << "\trm entry       - delete the naming entry\n"
	 << "\texit           - quit nsadmin\n" << flush;
  }
  else if (cmds[0] == "ls") {
    nsclient.list_context (arg1);
  }
  else if (cmds[0] == "cd") {
    if (arg1 == NULL)
      cerr << "syntax error" << endl;
    else
      nsclient.change_context (arg1);
  }
  else if (cmds[0] == "pwd") {
    nsclient.print_current_context ();
  }
  else if (cmds[0] == "mkdir") {
    if (arg1 == NULL)
      cerr << "syntax error" << endl;
    else
      nsclient.create_context (arg1);
  }
  else if (cmds[0] == "rm") {
    if (arg1 == NULL)
      cerr << "syntax error" << endl;
    else
      nsclient.remove_name (arg1);
  }
  else if (cmds[0] == "bind") {
    if (arg1 == NULL || arg2 == NULL)
      cerr << "syntax error" << endl;
    else {
      CORBA::Object_var obj;
#ifdef HAVE_EXCEPTIONS
      try {
#endif
        obj = orb->string_to_object (arg2);
        CORBA::String_var blah = orb->object_to_string (obj);
        cout << blah.in() << endl;
        nsclient.bind (arg1, obj);
#ifdef HAVE_EXCEPTIONS
      } catch (CORBA::SystemException_catch &ex) {
	cerr << "invalid stringified IOR" << endl;
      }
#endif
    }
  }
  else if (cmds[0] == "bind_context") {
      if (arg1 == NULL || arg2 == NULL) {
	  cerr << "syntax error: bind_context name oref" << endl; 
	  return 0;
      } 
      CORBA::Object_var obj;
#ifdef HAVE_EXCEPTIONS
      try {
#endif
        obj = orb->string_to_object (arg2);
        CORBA::String_var blah = orb->object_to_string (obj);
        cout << blah.in() << endl;
	CosNaming::NamingContext_var nc = CosNaming::NamingContext::_narrow(obj);
	if(CORBA::is_nil(nc))	{
	    cerr << "Object is no Namingcontext " << endl;
            return 0;
	}
        nsclient.bind_context (arg1, nc);
#ifdef HAVE_EXCEPTIONS
      } catch (CORBA::SystemException_catch &ex) {
	  cerr << "invalid stringified IOR" << endl;
      }
#endif
  }
  else if (cmds[0] == "cat") {
    nsclient.show_ior (arg1);
  }
  else if (cmds[0] == "iordump") {
      if (arg1 == NULL) {
	  cerr << "syntax error: iordump <name>" << endl;
          return 0;
      }
      CORBA::Object_var obj = nsclient.resolve_name(arg1);
      if (CORBA::is_nil (obj)) {
	  cerr << "error: illegal object reference: " << arg1 << endl;
	  return 0;
      }
      cout << endl;
      obj->_ior()->print (cout);
      cout << endl;
  }
  else if (cmds[0] == "repoid") {
      if (arg1 == NULL) {
	  cerr << "syntax error: repoid <name>" << endl;
          return 0;
      }
      CORBA::Object_var obj = nsclient.resolve_name(arg1);
      if (CORBA::is_nil (obj)) {
	  cerr << "error: illegal object reference: " << arg1 << endl;
	  return 0;
      }
      cout << obj->_ior()->objid() << endl;
  }
  else if (cmds[0] == "locurl") {
      if (arg1 == NULL) {
	  cerr << "syntax error: locurl <name>" << endl;
          return 0;
      }
      CORBA::Object_var obj = nsclient.resolve_name(arg1);
      if (CORBA::is_nil (obj)) {
	  cerr << "error: illegal object reference: " << arg1 << endl;
	  return 0;
      }
      CORBA::IORProfile *prof = obj->_ior()->profile(CORBA::IORProfile::TAG_INTERNET_IOP);
      if(!prof)
      {
	  cerr << "No TAG_INTERNET_IIOP in url - try iordump " << endl;
	  return 0;
      }
      CORBA::Long length;
      const CORBA::Octet *okey = prof->objectkey(length);
      string addr = prof->addr()->stringify(); // Warum wuehlen ?
   
      cout << "corbaloc::" << addr.substr(addr.find(":")+1);

      if(length>0)
      {
	  CORBA::String_var urlrumpf = mico_url_encode(okey,length);
	  cout << '/' << urlrumpf;
      }
      cout << endl;
      
  }
  else if (cmds[0] == "url") {
    if (arg1 == NULL) {
      cerr << "syntax error" << endl;
    }
    else {
      nsclient.show_url (arg1);
    }
  }
  else if (cmds[0] == "exit")
    return -1;
  else 
    cerr << "unknown command: " << cmds[0] << endl;
#ifdef HAVE_EXCEPTIONS
  }
  catch (CosNaming::NamingContext::NotFound_catch &exc) {	
    cerr << "NotFound: " << arg1 << endl;                     
    switch(exc.why)                                  	
    {                                               	
    case CosNaming::NamingContext::missing_node:	
	cerr << "Missing Node";                       	
	break;                                          
    case CosNaming::NamingContext::not_context:      	
	cerr << "Not Context" ;                    	
        break;                                  	
    case CosNaming::NamingContext::not_object:		
        cerr << "Not Object";				
        break;						
    }							
    cerr << " (Rest of name: ";				
    printname(cerr,exc.rest_of_name);			
    cerr << ')' << endl;				
    cerr << endl;                                               
  }								
  catch (CosNaming::NamingContext::CannotProceed_catch &exc) {	
    cerr << "CannotProceed: " << arg1 << endl;	       	
  }								
  catch (CosNaming::NamingContext::InvalidName_catch &exc) {	
    cerr << "InvalidName: " << arg1 << endl;			
  }								
  catch (CosNaming::NamingContext::AlreadyBound_catch &exc) {	
    cerr << "AlreadyBound: " << arg1 << endl;			
  }
#endif
  return 0;
}

int main (int argc, char *argv[])
{
  signal (SIGINT, SIG_IGN);
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");
  CORBA::Object_var obj;
  CosNaming::NamingContext_var nc;

#ifdef HAVE_EXCEPTIONS
  try {
#endif
    obj = orb->resolve_initial_references ("NameService");
    nc = CosNaming::NamingContext::_narrow (obj);
#ifdef HAVE_EXCEPTIONS
  }
  catch (CORBA::ORB::InvalidName_catch & ivn) {
    nc = CosNaming::NamingContext::_nil ();
  }
  catch (CORBA::SystemException_catch & sx) {
    nc = CosNaming::NamingContext::_nil ();
  }
#endif
  if (CORBA::is_nil (nc)) {
      cerr << "no name service available, use -ORBNamingAddr ..." << endl;
      exit (1);
  }

  NamingClient nsclient (nc);
  vector<string> cmds;

  if (argc >= 2) {
    // non interactive mode
    for (int i = 1; i < argc; i++) 
      cmds.push_back (argv[i]);

    parse_cmd (orb, nsclient, cmds);
  }
  else {
    // interactive mode
    char *line, *lptr, *tok; 

    cerr << info << endl;
    while (1) {
      cmds.erase (cmds.begin (), cmds.end ());

#ifdef HAVE_LIBREADLINE
      line = readline ("nsadmin> ");
      if (line)
	add_history (line);
#else
      cout << "nsadmin> " << flush;
      char buf[1000];
      if (cin.getline(buf, sizeof(buf)).eof())
        line = NULL;
      else
        line = strdup (buf);
#endif
      if (line == NULL)
        exit(0);
      if (line[0] == '\0') {
	if (line) free (line);
	continue;
      }

      lptr = line;
      while ((tok = ::strtok (lptr, " \t"))) {
	cmds.push_back (tok);
	lptr = NULL;
      }

      if (parse_cmd (orb, nsclient, cmds) == -1)
        exit (0);
      free (line);
    }
  }
  return 0;
}
	
