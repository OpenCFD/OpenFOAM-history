/*
 *  Mico --- a free CORBA implementation
 *  Copyright Frank Pilhofer
 *
 *  This file is part of MicoCCM, written by Frank Pilhofer.
 *  The MICO CORBA Component Project was sponsored by Alcatel.
 */

#include <CORBA.h>
#include <mico/CCM.h>
#include <coss/CosNaming.h>
#include <mico/util.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif


using namespace std;

/*
 * This is just a simple tool that
 *
 *   - contacts a Server Activator to create a ComponentServer
 *   - creates a Container
 *   - loads a component from a shared library into the Container
 *   - activates the component's home
 *   - optionally registers the component's home in the NamingService
 */

static void
usage (const char *progname)
{
  cerr << "usage: " << progname << " [<options>] <home name> <library file>" << endl;
  cerr << "possible <options> are:" << endl;
  cerr << "    --help" << endl;
  cerr << "    --ccmd <IOR>" << endl;
  cerr << "    --host <host>[:port]" << endl;
  cerr << "    --ns <string>" << endl;
  cerr << "    --ior <IOR ref file>" << endl;
  cerr << "    -v" << endl;
  exit (1);
}

int
main (int argc, char *argv[])
{
  /*
   * Init ORB
   */

  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

  /*
   * Process Command Line
   */

  MICOGetOpt::OptMap opts;
  opts["--help"] = "";
  opts["--sa"]   = "arg-expected";
  opts["--ccmd"] = "arg-expected";
  opts["--host"] = "arg-expected";
  opts["--ns"]   = "arg-expected";
  opts["--ior"]  = "arg-expected";
  opts["-v"]     = "";

  MICOGetOpt opt_parser (opts);
  if (!opt_parser.parse (argc, argv))
    usage (argv[0]);

  string saior;
  string host;
  string nsname;
  string reffile;
  bool verbose = false;

  for (MICOGetOpt::OptVec::const_iterator i = opt_parser.opts().begin();
       i != opt_parser.opts().end(); ++i) {
    string arg = (*i).first;
    string val = (*i).second;

    if (arg == "--sa") {
      saior = val;
    } else if (arg == "--ccmd") {
      saior = val;
    } else if (arg == "--host") {
      host = val;
    } else if (arg == "--ns") {
      nsname = val;
    } else if (arg == "--ior") {
      reffile = val;
    } else if (arg == "-v") {
      verbose = true;
    } else if (arg == "--help") {
      usage (argv[0]);
    } else {
      usage (argv[0]);
    }
  }

  if (argc != 3) {
    usage (argv[0]);
  }

  string homename = argv[1];
  string libfile = argv[2];

  /*
   * Contact ServerActivator
   */

  Components::ServerActivator_var sa =
    Components::ServerActivator::_nil ();
  CORBA::Object_var obj;

  if (host.length() > 0) {
    saior  = "corbaloc::";
    saior += host;
    if (host.find (":") == (CORBA::ULong) -1) {
      saior += ":1234";
    }
    saior += "/MicoCCMD";
  }

  if (saior.length() > 0) {
    try {
      obj = orb->string_to_object (saior.c_str());
      sa = Components::ServerActivator::_narrow (obj);
    } catch (...) {
      sa = Components::ServerActivator::_nil ();
    }

    if (CORBA::is_nil (sa)) {
      cerr << "error: illegal IOR for --sa: " << saior << endl;
    }
  }
  else {
    try {
      obj = orb->resolve_initial_references ("ServerActivator");
    } catch (...) {
      obj = CORBA::Object::_nil ();
    }

    sa = Components::ServerActivator::_narrow (obj);

    if (CORBA::is_nil (sa)) {
      cerr << "error: need ServerActivator, use --host or --ccmd" << endl;
      exit (1);
    }
  }

  /*
   * Contact NamingService if necessary
   */

  CosNaming::NamingContextExt_var ns;
  CosNaming::Name_var n;

  if (nsname.length()) {
    try {
      obj = orb->resolve_initial_references ("NameService");
    }
    catch (...) {
      cerr << "oops: need a NamingService initial reference for --ns" << endl;
      exit (1);
    }

    ns = CosNaming::NamingContextExt::_narrow (obj);

    if (CORBA::is_nil (ns)) {
      cerr << "oops: cannot contact Naming Service" << endl;
      exit (1);
    }

    n = ns->to_name (nsname.c_str());
  }

  /*
   * Create ComponentServer
   */

  if (verbose) {
    cout << "Creating ComponentServer ... " << flush;
  }

  Components::ComponentServer_var cs;
  Components::ConfigValues cv;

  try {
    cs = sa->create_component_server (cv);
  }
  catch (...) {
    if (!verbose) {
      cerr << "oops: exception creating ComponentServer" << endl;
    }
    throw;
  }

  if (verbose) {
    cout << "ok." << endl;
  }

  /*
   * Create Container
   */

  if (verbose) {
    cout << "Creating Container ... " << flush;
  }

  Components::Container_var co;

  try {
    co = cs->create_container (cv);
  }
  catch (...) {
    if (!verbose) {
      cerr << "oops: exception creating Container" << endl;
    }
    throw;
  }

  if (verbose) {
    cout << "ok." << endl;
  }

  // determine name of entry point from home name

  string entrypt = homename;
  CORBA::ULong idx;

  if (entrypt[0] == ':') {
    entrypt = entrypt.substr (2);
  }

  while ((idx = entrypt.find ("::")) != (CORBA::ULong) -1) {
    entrypt.replace (idx, 2, "_");
  }

  entrypt.insert (0, "create_");

  /*
   * Install Home
   */

  if (verbose) {
    cout << "Installing Home ... " << flush;
  }

  Components::CCMHome_var home;

  try {
    home = co->install_home (libfile.c_str(), entrypt.c_str(), cv);
  }
  catch (...) {
    if (!verbose) {
      cerr << "oops: exception installing home" << endl;
    }
    throw;
  }

  if (verbose) {
    cout << "ok." << endl;
  }

  bool writeior = true;

  /*
   * Register with Naming Service
   */

  if (nsname.length()) {
    if (verbose) {
      cout << "Registering Home with Naming Service ... " << flush;
    }

    ns->rebind (n.in(), home.in());

    if (verbose) {
      cout << "done." << endl;
    }

    writeior = false;
  }

  /*
   * Write Home's IOR
   */

  if (reffile.length() > 0) {
    CORBA::String_var homeior = orb->object_to_string (home.in());

    if (reffile == "-") {
      cout << homeior << endl;
    }
    else {
      ofstream out (reffile.c_str());
      if (!out) {
	cerr << "error: cannot open output file " << reffile << endl;
      }
      else {
	out << homeior << endl;
	out.close ();
      }
    }

    writeior = false;
  }

  /*
   * If the IOR has not been exported to the Naming Service or dumped
   * to a file, print it.
   */

  if (verbose || writeior) {
    CORBA::String_var homeior = orb->object_to_string (home.in());
    cout << homeior << endl;
  }

  /*
   * Done.
   */

  return 0;
}

