/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
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
 *  For more information, visit the MICO Home Page at
 *  http://www.mico.org/
 */

#ifdef FAST_PCH
#include "imr_pch.h"
#endif // FAST_PCH
#ifdef __COMO__
#pragma hdrstop
#endif // __COMO__

#ifndef FAST_PCH

#define MICO_CONF_IMR

#include <CORBA-SMALL.h>
#include <string.h>
#include <stdlib.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <iomanip>
#else
#include <iostream.h>
#include <iomanip.h>
#endif
#include <mico/template_impl.h>

#endif // FAST_PCH


using namespace std;

static char *prog_name = 0;

static int
usage ()
{
    cerr << "usage: " << prog_name << " <command> ..." << endl;
    cerr << "where <command> is one of the following:" << endl;
    cerr << "\tlist ....................................... "
	 << "show entries"
	 << endl;
    cerr << "\tinfo <name> ................................ "
	 << "info for entry <name>"
	 << endl;
    cerr << "\tdelete <name> .............................. "
	 << "remove entry <name>"
	 << endl;
    cerr << "\tcreate <name> <mode> <command> <repoids> ... "
	 << "create new entry"
	 << endl;
    cerr << "\tactivate <name> [<address>] ................ "
	 << "activate implementation"
	 << endl;
    cerr << "\tstop <name> [<address>] .................... "
	 << "stop server and hold requests"
	 << endl;
    cerr << "\tcontinue <name> [<address>] ................ "
	 << "restart server when stopped"
	 << endl;
    cerr << "\tlocate <name> ....... ...................... "
	 << "locate tagged implementation of repoid"
	 << endl;
    return 1;
}

static int
imr_list (const vector<string> &args, CORBA::ImplRepository_ptr imr)
{
    if (args.size() != 0)
	return usage ();

    CORBA::ImplRepository::ImplDefSeq_var impls = imr->find_all();
    for (CORBA::ULong i = 0; i < impls->length(); ++i) {
	CORBA::String_var name = impls[i]->name();
	cout << name.in() << endl;
    }
    return 0;
}

static int
imr_info (const vector<string> &args, CORBA::ImplRepository_ptr imr)
{
    if (args.size() != 1)
	return usage ();

    CORBA::ImplRepository::ImplDefSeq_var impls
	= imr->find_by_name (args[0].c_str());
    if (impls->length() == 0) {
	cout << "no such server: " << args[0] << endl;
	return 1;
    }
    assert (impls->length() == 1);
    CORBA::ImplementationDef_ptr impl = impls[(CORBA::ULong)0];

    cout << setw(20) << "server name: " << args[0] << endl;
    cout << setw(20) << "activation mode: ";
    switch (impl->mode()) {
    case CORBA::ImplementationDef::ActivatePOA:
        cout << "poa";
	break;
    default:
	assert (0);
    }
    cout << endl;

    CORBA::String_var command = impl->command();
    cout << setw(20) << "activation command: " << command.in() << endl;

    CORBA::ImplementationDef::ObjectInfoList_var objs = impl->objs();
    for (CORBA::ULong i = 0; i < objs->length(); ++i) {
	cout << setw(17) << "object #" << i << ": "
	     << objs[i].repoid.in();
	if (objs[i].tag.length() > 0) {
	    CORBA::String_var tag = CORBA::ORB::tag_to_string (objs[i].tag);
	    cout << "#" << tag.in();
	}
	cout << endl;
    }
    return 0;
}

static int
imr_locate (const vector<string> &args, CORBA::ImplRepository_ptr imr)
{
    if (args.size() != 1)
	return usage ();

    CORBA::ImplRepository::ImplDefSeq_var impls = imr->find_all();
    for (CORBA::ULong i = 0; i < impls->length(); ++i) {
      CORBA::String_var name = impls[i]->name();
      
      CORBA::ImplRepository::ImplDefSeq_var impls1 = imr->find_by_name (name.in());
      if (impls1->length() == 0) {
	cout << "no such server: " << name.in() << endl;
	continue;
      }
      assert (impls1->length() == 1);
      CORBA::ImplementationDef_ptr impl = impls1[(CORBA::ULong)0];
           
      CORBA::ImplementationDef::ObjectInfoList_var objs = impl->objs();
      for (CORBA::ULong j = 0; j < objs->length(); ++j) {
	if (args[0] == objs[j].repoid.in()) {
	  CORBA::String_var tag = CORBA::ORB::tag_to_string (objs[j].tag);
	  if (strlen(tag.in())) {
	    cout << tag.in();
	    cout << endl;
	  } 
	}
      }
    }
    return 0;
}

static int
imr_delete (const vector<string> &args, CORBA::ImplRepository_ptr imr)
{
    if (args.size() < 1)
	return usage ();

    CORBA::ImplRepository::ImplDefSeq_var impls
	= imr->find_by_name (args[0].c_str());
    if (impls->length() == 0) {
	cout << "no such server: " << args[0] << endl;
	return 1;
    }
    assert (impls->length() == 1);
    imr->destroy (impls[(CORBA::ULong)0]);
    return 0;
}

static int
imr_create (const vector<string> &args, CORBA::ImplRepository_ptr imr)
{
    if (args.size() < 2)
	return usage ();

    CORBA::ImplRepository::ImplDefSeq_var impls
	= imr->find_by_name (args[0].c_str());
    if (impls->length() > 0) {
	cout << "there is already a server called '"
	     << args[0] << "', please choose a unique name!"
	     << endl;
	return 1;
    }

    CORBA::ImplementationDef::ActivationMode mode;
    if (args[1] == "poa")
	mode = CORBA::ImplementationDef::ActivatePOA;
    else {
	cout << "invalid activation mode: " << args[1] << endl;
	cout << "valid modes are: poa"
	     << endl;
	return 1;
    }

    string command;
    if (args.size() < 2) {
	cout << prog_name << "create needs a command" << endl;
	return 1;
    }
    else {
      command = args[2];
    }

    CORBA::ImplementationDef::ObjectInfoList objs;

    if (args.size() > 3) {
	objs.length (args.size() - 3);
	for (vector<string>::size_type i = 3; i < args.size(); ++i) {
	    int pos = args[i].rfind ("#");
	    if (pos >= 0) {
		objs[i-3].repoid =
		    CORBA::string_dup (args[i].substr(0,pos).c_str());

		CORBA::ORB::ObjectTag_var tag =
		    CORBA::ORB::string_to_tag (args[i].substr(pos+1).c_str());
		objs[i-3].tag = *tag;
	    } else {
		objs[i-3].repoid = CORBA::string_dup (args[i].c_str());
	    }
	}
    }

    CORBA::ImplementationDef_var impl
	= imr->create (mode, objs, args[0].c_str(), command.c_str());
    assert (!CORBA::is_nil (impl));
    return 0;
}

static int
imr_activate (const vector<string> &args, CORBA::ImplRepository_ptr imr)
{
    if (args.size() < 1)
	return usage ();

    CORBA::ImplRepository::ImplDefSeq_var impls
	= imr->find_by_name (args[0].c_str());
    if (impls->length() == 0) {
	cout << "no such server: " << args[0] << endl;
	return 1;
    }
    assert (impls->length() == 1);

    CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
    CORBA::Object_var obj = CORBA::Object::_nil ();

    if (args.size() > 1) {
	// try given address
	obj = orb->bind ("IDL:omg.org/CORBA/POAMediator:1.0",
			 args[1].c_str());
    }
    if (CORBA::is_nil (obj)) {
	// try address of the impl repo
	const CORBA::Address *addr = imr->_ior()->addr();
	obj = orb->bind ("IDL:omg.org/CORBA/POAMediator:1.0",
			 addr->stringify().c_str());
    }
    if (CORBA::is_nil (obj)) {
	// try default addresses 
	obj = orb->bind ("IDL:omg.org/CORBA/POAMediator:1.0");
    }
    if (CORBA::is_nil (obj)) {
	cout << "error: cannot connect to micod" << endl;
	return 1;
    }
    CORBA::POAMediator_var poamed = CORBA::POAMediator::_narrow (obj);
    if (!poamed->force_activation (impls[(CORBA::ULong)0])) {
	cout << "error: cannot activate server " << args[0] << endl;
	return 1;
    }

    return 0;
}

static int
imr_stop (const vector<string> &args, CORBA::ImplRepository_ptr imr)
{
    if (args.size() < 1)
	return usage ();

    CORBA::ImplRepository::ImplDefSeq_var impls
	= imr->find_by_name (args[0].c_str());
    if (impls->length() == 0) {
	cout << "no such server: " << args[0] << endl;
	return 1;
    }
    assert (impls->length() == 1);

    CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
    CORBA::Object_var obj = CORBA::Object::_nil ();

    if (args.size() > 1) {
	// try given address
	obj = orb->bind ("IDL:omg.org/CORBA/POAMediator:1.0",
			 args[1].c_str());
    }
    if (CORBA::is_nil (obj)) {
	// try address of the impl repo
	const CORBA::Address *addr = imr->_ior()->addr();
	obj = orb->bind ("IDL:omg.org/CORBA/POAMediator:1.0",
			 addr->stringify().c_str());
    }
    if (CORBA::is_nil (obj)) {
	// try default addresses 
	obj = orb->bind ("IDL:omg.org/CORBA/POAMediator:1.0");
    }
    if (CORBA::is_nil (obj)) {
	cout << "error: cannot connect to micod" << endl;
	return 1;
    }
    CORBA::POAMediator_var poamed = CORBA::POAMediator::_narrow (obj);
    if (!poamed->stop (impls[(CORBA::ULong)0])) {
	cout << "error: cannot stop server " << args[0] << endl;
	return 1;
    }

    return 0;
}

static int
imr_hold (const vector<string> &args, CORBA::ImplRepository_ptr imr)
{
    if (args.size() < 1)
	return usage ();

    CORBA::ImplRepository::ImplDefSeq_var impls
	= imr->find_by_name (args[0].c_str());
    if (impls->length() == 0) {
	cout << "no such server: " << args[0] << endl;
	return 1;
    }
    assert (impls->length() == 1);

    CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
    CORBA::Object_var obj = CORBA::Object::_nil ();

    if (args.size() > 1) {
	// try given address
	obj = orb->bind ("IDL:omg.org/CORBA/POAMediator:1.0",
			 args[1].c_str());
    }
    if (CORBA::is_nil (obj)) {
	// try address of the impl repo
	const CORBA::Address *addr = imr->_ior()->addr();
	obj = orb->bind ("IDL:omg.org/CORBA/POAMediator:1.0",
			 addr->stringify().c_str());
    }
    if (CORBA::is_nil (obj)) {
	// try default addresses 
	obj = orb->bind ("IDL:omg.org/CORBA/POAMediator:1.0");
    }
    if (CORBA::is_nil (obj)) {
	cout << "error: cannot connect to micod" << endl;
	return 1;
    }
    CORBA::POAMediator_var poamed = CORBA::POAMediator::_narrow (obj);
    if (!poamed->hold (impls[(CORBA::ULong)0])) {
	cout << "error: cannot hold server " << args[0] << endl;
	return 1;
    }

    return 0;
}

static int
imr_continue (const vector<string> &args, CORBA::ImplRepository_ptr imr)
{
    if (args.size() < 1)
	return usage ();

    CORBA::ImplRepository::ImplDefSeq_var impls
	= imr->find_by_name (args[0].c_str());
    if (impls->length() == 0) {
	cout << "no such server: " << args[0] << endl;
	return 1;
    }
    assert (impls->length() == 1);

    CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
    CORBA::Object_var obj = CORBA::Object::_nil ();

    if (args.size() > 1) {
	// try given address
	obj = orb->bind ("IDL:omg.org/CORBA/POAMediator:1.0",
			 args[1].c_str());
    }
    if (CORBA::is_nil (obj)) {
	// try address of the impl repo
	const CORBA::Address *addr = imr->_ior()->addr();
	obj = orb->bind ("IDL:omg.org/CORBA/POAMediator:1.0",
			 addr->stringify().c_str());
    }
    if (CORBA::is_nil (obj)) {
	// try default addresses 
	obj = orb->bind ("IDL:omg.org/CORBA/POAMediator:1.0");
    }
    if (CORBA::is_nil (obj)) {
	cout << "error: cannot connect to micod" << endl;
	return 1;
    }
    CORBA::POAMediator_var poamed = CORBA::POAMediator::_narrow (obj);
    if (!poamed->_cxx_continue (impls[(CORBA::ULong)0])) {
	cout << "error: cannot continue server " << args[0] << endl;
	return 1;
    }

    return 0;
}

int
main (int argc, char **argv)
{
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

    prog_name = argv[0];
    if (argc < 2)
	return usage ();

    CORBA::Object_var obj =
	orb->resolve_initial_references ("ImplementationRepository");
    CORBA::ImplRepository_var imr = CORBA::ImplRepository::_narrow (obj);

    if (CORBA::is_nil (imr)) {
      cout << "error: cannot contact Implementation Repository" << endl;
      return 1;
    }

    string cmd = argv[1];
    vector<string> args;
    for (int i = 2; i < argc; ++i)
	args.push_back (argv[i]);
    
    if (cmd == "list") {
	return imr_list (args, imr);
    } else if (cmd == "info") {
	return imr_info (args, imr);
    } else if (cmd == "delete") {
	return imr_delete (args, imr);
    } else if (cmd == "create") {
	return imr_create (args, imr);
    } else if (cmd == "activate") {
      return imr_activate (args, imr);
    } else if (cmd == "locate") {
      return imr_locate(args, imr);
    } else if (cmd == "hold") {
      return imr_hold (args, imr);
    } else if (cmd == "stop") {
      return imr_stop (args, imr);
    } else if (cmd == "continue") {
      return imr_continue (args, imr);
    } else {
	return usage ();
    }
}
