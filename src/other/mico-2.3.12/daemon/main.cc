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

#include <CORBA.h>
#include <mico/util.h>
#include <mico/template_impl.h>
#include <sys/types.h>
#include <signal.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <iomanip>
#include <fstream>
#else
#include <iostream.h>
#include <iomanip.h>
#include <fstream.h>
#endif

#include "poamediator.h"
#include "imr_impl.h"

#endif // FAST_PCH


using namespace std;

CORBA::ORB_ptr orb;
CORBA::POAMediator_ptr pmd;
string thedb;

int
save_imr (const char *fn)
{
    ofstream out (fn);
    if (!out) {
	cerr << "error: cannot open output file " << fn << endl;
	return 1;
    }
    CORBA::Object_var obj =
      orb->resolve_initial_references ("ImplementationRepository");
    CORBA::ImplRepository_var imr = CORBA::ImplRepository::_narrow (obj);

    /*
     * Save Imr entries
     */

    CORBA::ImplRepository::ImplDefSeq_var impls = imr->find_all();
    CORBA::ULong length=0;

    for (CORBA::ULong i0 = 0; i0<impls->length(); ++i0) {
      CORBA::String_var s = impls[i0]->command();
      if (strlen (s) > 0) {
	length++;
      }
    }

    out << length << endl;

    for (CORBA::ULong i = 0; i < impls->length(); ++i) {
	CORBA::String_var s = impls[i]->command();
	if (strlen (s) > 0) {
	    // it is not an automatically created entry
	    s = impls[i]->tostring();
	    out << s.in() << endl;
	}
    }

    return 0;
}

int
restore_imr (const char *fn)
{
    ifstream in (fn);
    if (!in) {
	cerr << "warning: cannot open input file " << fn << endl;
	return 1;
    }
    CORBA::Object_var obj =
      orb->resolve_initial_references ("ImplementationRepository");
    CORBA::ImplRepository_var imr = CORBA::ImplRepository::_narrow (obj);

    /*
     * Restore Imr entries
     */

    char line[10000];
    if (in.getline(line, sizeof (line)).eof()) {
      return 0;
    }

    CORBA::ULong entries = atoi (line);

    for (CORBA::ULong i=0; i<entries; i++) {
      if (in.getline (line, sizeof (line)).eof()) {
	break;
      }
      CORBA::ImplementationDef_var impl = imr->restore (line);
      assert (!CORBA::is_nil (impl));
    }

    return 0;
}

void
sighandler (int sig)
{
    if (!CORBA::is_nil (pmd)) {
      pmd->shutdown_server ();
    }
    orb->shutdown (TRUE);
    if (thedb.length() > 0)
	save_imr (thedb.c_str());
    if (!CORBA::is_nil (pmd))
	CORBA::release (pmd);
    signal (sig, SIG_DFL);
    raise (sig);
}

void
usage (const char *progname)
{
    cerr << "usage: " << progname << " [<options>]" << endl;
    cerr << "possible <options> are:" << endl;
    cerr << "    --help" << endl;
    cerr << "    --forward" << endl;
    cerr << "    --db <database file>" << endl;
    cerr << "    --ior <IOR ref file>" << endl;
    exit (1);
}

int
main (int argc, char *argv[])
{
    CORBA::Boolean forward_opt = TRUE;

    //
    // Make sure that we'll run with at most 1 thread
    // in thread pool - so in fact in single-threaded
    // mode
    //
    char** t_argv = new char*[argc+1+2+1];

    for (int i=0; i<argc; ++i) {
	t_argv[i] = argv[i];
    }
    t_argv[argc++] = "-ORBThreadPool";

    t_argv[argc++] = "-ORBRequestLimit";
    t_argv[argc++] = "1";    

    t_argv[argc] = 0;

    orb = CORBA::ORB_init (argc, t_argv, "mico-local-orb");
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);
    PortableServer::POAManager_var mgr = poa->the_POAManager();

#ifdef _WIN32
    MICO::UnixProcess::win32_process_init();
#endif

    MICOGetOpt::OptMap opts;
    opts["--help"]    = "";
    opts["--forward"] = "";
    opts["--dont-forward"] = "";
    opts["--db"]      = "arg-expected";
    opts["--ior"]     = "arg-expected";

    MICOGetOpt opt_parser (opts);
    if (!opt_parser.parse (argc, t_argv))
	usage (t_argv[0]);

    string reffile;

    for (MICOGetOpt::OptVec::const_iterator i = opt_parser.opts().begin();
	 i != opt_parser.opts().end(); ++i) {
	string arg = (*i).first;
	string val = (*i).second;

	if (arg == "--db") {
	    thedb = val;
	} else if (arg == "--forward") {
	    forward_opt = TRUE;
	} else if (arg == "--dont-forward") {
	    forward_opt = FALSE;
	} else if (arg == "--ior") {
	    reffile = val;
	} else if (arg == "--help") {
	    usage (t_argv[0]);
	} else {
	    usage (t_argv[0]);
	}
    }
    if (argc != 1) {
	usage (t_argv[0]);
    }

    /*
     * Provide Mediators and Implementation Repository
     */

    ImplRepository_impl* imr_serv = new ImplRepository_impl;
    CORBA::Object_var imr = imr_serv->_this();
    orb->set_initial_reference ("ImplementationRepository", imr);
    POAMediatorImpl* poa_med = new POAMediatorImpl(orb, forward_opt);
    pmd = poa_med->_this();
    poa_med->set_own_ref(pmd);
    mgr->activate();
    if (thedb.length() > 0)
	restore_imr (thedb.c_str());

    if (reffile.length() > 0) {
      CORBA::String_var imrior = orb->object_to_string (imr);

      if (reffile == "-") {
	cout << imrior << endl;
      }
      else {
	ofstream out (reffile.c_str());
	if (!out) {
	  cerr << "error: cannot open output file " << reffile << endl;
	}
	else {
	  out << imrior << endl;
	  out.close ();
	}
      }
    }

    signal (SIGTERM, sighandler);
    signal (SIGINT, sighandler);
#ifndef _WIN32
    signal (SIGHUP, sighandler);
#endif
    orb->run ();
    // notreached
    return 0;
}
