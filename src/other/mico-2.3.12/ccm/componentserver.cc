/*
 *  Mico --- a free CORBA implementation
 *  Copyright Frank Pilhofer
 *
 *  This file is part of MicoCCM, written by Frank Pilhofer.
 *  The MICO CORBA Component Project was sponsored by Alcatel.
 */

#include <CORBA.h>
#include <mico/CCM.h>
#include <mico/CCMContainer.h>
#include <mico/util.h>
#include <mico/shlib.h>
#include <mico/shlib_impl.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif
#include <signal.h>


using namespace std;

/*
 * This file implements the Components::ComponentServer and Container
 * interfaces. A ComponentServer can host multiple Containers, and a
 * container can host multiple Homes.
 *
 * Each instance of this server has a ComponentServer singleton.
 */

class ComponentServer_impl;

CORBA::ORB_var orb;
PortableServer::POA_var poa;
#ifdef HAVE_THREADS
MICOMT::Mutex mutex;
MICOMT::CondVar cond(&mutex);
#else // HAVE_THREADS
bool finished;
#endif // HAVE_THREADS
bool verbose;
string ccmdtoken;
string root;
ComponentServer_impl* csimpl;

class ComponentServer_impl :
  virtual public POA_MICOCCM::ComponentServer,
  virtual public PortableServer::RefCountServantBase
{
private:
  Components::ConfigValues _configuration;
  Components::ServerActivator_var _server_activator;
  PortableServer::POA_var _container_poa;
  Components::Containers _containers;

public:
  ComponentServer_impl (Components::ServerActivator_ptr,
			PortableServer::POA_ptr);

  void set_config_values (const Components::ConfigValues &);

  Components::ConfigValues * configuration ();
  Components::ServerActivator_ptr get_server_activator ();
  Components::Container_ptr create_container (const Components::ConfigValues &);
  void remove_container (Components::Container_ptr);
  Components::Containers * get_containers ();
  void remove ();
};

class Container_impl :
  virtual public POA_Components::Container,
  virtual public PortableServer::RefCountServantBase
{
private:
  Components::ConfigValues _configuration;
  Components::ComponentServer_var _component_server;

  struct ShlibInfo {
    CORBA::ULong count;
    MICO::SharedLib * shlib;
  };

  typedef map<string, ShlibInfo, less<string> > ShlibMap;
  ShlibMap _loaded_shlibs;

  struct HomeInfo {
    string file;
    MICO::CCM::ContainerBase * cont;
    Components::CCMHome_ptr home;
  };

  list<HomeInfo> _homes;

private:
  MICO::SharedLib * open_shlib (const char *);
  void close_shlib (const char *);

public:
  Container_impl (Components::ComponentServer_ptr,
		  const Components::ConfigValues &);

  Components::ConfigValues * configuration ();
  Components::ComponentServer_ptr get_component_server ();
  Components::CCMHome_ptr install_home (const char *, const char *,
					const Components::ConfigValues &);
  void remove_home (Components::CCMHome_ptr);
  Components::CCMHomes * get_homes ();
  void remove ();
};

class SharedLibraryCloser : public CORBA::DispatcherCallback
{
private:
  MICO::SharedLib * _lib;

public:
  SharedLibraryCloser (MICO::SharedLib *);
  ~SharedLibraryCloser ();
  void callback (CORBA::Dispatcher *, CORBA::DispatcherCallback::Event);
};

/*
 * ----------------------------------------------------------------------
 * Component Server Implementation
 * ----------------------------------------------------------------------
 */

ComponentServer_impl::ComponentServer_impl (Components::ServerActivator_ptr sa,
					    PortableServer::POA_ptr cp)
{
  _server_activator = Components::ServerActivator::_duplicate (sa);
  _container_poa = PortableServer::POA::_duplicate (cp);
}

void
ComponentServer_impl::set_config_values (const Components::ConfigValues & cv)
{
  _configuration = cv;
}

Components::ConfigValues *
ComponentServer_impl::configuration ()
{
  return new Components::ConfigValues (_configuration);
}

Components::ServerActivator_ptr
ComponentServer_impl::get_server_activator ()
{
  return Components::ServerActivator::_duplicate (_server_activator);
}

Components::Container_ptr
ComponentServer_impl::create_container (const Components::ConfigValues & cv)
{
  Components::ComponentServer_var me = _this ();
  Container_impl * nc = new Container_impl (me, cv);
  PortableServer::ObjectId_var oid = _container_poa->activate_object (nc);
  CORBA::Object_var obj = _container_poa->id_to_reference (oid.in());
  Components::Container_var co = Components::Container::_narrow (obj);
  CORBA::ULong idx = _containers.length ();
  _containers.length (idx+1);
  _containers[idx] = Components::Container::_duplicate (co);
  return co._retn ();
}

void
ComponentServer_impl::remove_container (Components::Container_ptr co)
{
  PortableServer::ObjectId_var oid;
  PortableServer::ObjectId_var iid;
  CORBA::ULong i;

  try {
    oid = _container_poa->reference_to_id (co);
  }
  catch (PortableServer::POA::WrongAdapter &) {
    throw Components::RemoveFailure ();
  }

  for (i=0; i<_containers.length(); i++) {
    iid = _container_poa->reference_to_id (_containers[i]);
    if (iid->length() == oid->length() &&
	memcmp (iid->get_buffer(), oid->get_buffer(), iid->length()) == 0) {
      break;
    }
  }

  if (i >= _containers.length()) {
    throw Components::RemoveFailure ();
  }

  co->remove ();

  for (; i<_containers.length()-1; i++) {
    _containers[i] = _containers[i+1];
  }

  _containers.length (_containers.length()-1);
  _container_poa->deactivate_object (oid.in());
}

Components::Containers *
ComponentServer_impl::get_containers ()
{
  return new Components::Containers (_containers);
}

void
ComponentServer_impl::remove ()
{
  Components::Containers_var all = get_containers ();

  for (CORBA::ULong i=0; i<all->length(); i++) {
    remove_container (all[i]);
  }
#ifdef HAVE_THREADS
  cond.signal();
#else // HAVE_THREADS
  finished = true;
#endif // HAVE_THREADS
}

/*
 * ----------------------------------------------------------------------
 * Container
 * ----------------------------------------------------------------------
 */

Container_impl::Container_impl (Components::ComponentServer_ptr cs,
				const Components::ConfigValues & cv)
{
  _configuration = cv;
  _component_server = Components::ComponentServer::_duplicate (cs);
}

Components::ConfigValues *
Container_impl::configuration ()
{
  return new Components::ConfigValues (_configuration);
}

Components::ComponentServer_ptr
Container_impl::get_component_server ()
{
  return Components::ComponentServer::_duplicate (_component_server);
}

Components::CCMHome_ptr
Container_impl::install_home (const char * id,
			      const char * entrypt,
			      const Components::ConfigValues & cv)
{
  MICO::SharedLib * shlib = open_shlib (id);

  if (!shlib) {
    if (verbose) {
      cout << "ComponentServer # " << ccmdtoken << ": "
	   << "failure opening " << id << endl;
    }
    throw Components::InstallationFailure ();
  }

  if (strncmp (entrypt, "create_", 7) != 0) {
    if (verbose) {
      cout << "ComponentServer # " << ccmdtoken << ": "
	   << "unexpected entry point" << entrypt << endl;
    }
    close_shlib (id);
    throw Components::InstallationFailure ();
  }

  string deployentrypt (entrypt);
  deployentrypt.replace (0, 7, "deploy_");

  void * createfn = shlib->symbol (entrypt);
  void * deployfn = shlib->symbol (deployentrypt.c_str());

  if (!createfn) {
    if (verbose) {
      cout << "ComponentServer # " << ccmdtoken << ": "
	   << "cannot find entry point " << entrypt << " in " << id << endl;
    }
    close_shlib (id);
    throw Components::InstallationFailure ();
  }

  if (!deployfn) {
    if (verbose) {
      cout << "ComponentServer # " << ccmdtoken << ": "
	   << "cannot find  " << deployentrypt << " in " << id << endl;
    }
    close_shlib (id);
    throw Components::InstallationFailure ();
  }

  Components::HomeExecutorBase_ptr (*creator) (CORBA::ORB_ptr) =
    (Components::HomeExecutorBase_ptr (*) (CORBA::ORB_ptr)) createfn;

  MICO::CCM::ContainerBase * (*deployor) (CORBA::ORB_ptr, Components::HomeExecutorBase_ptr) =
    (MICO::CCM::ContainerBase * (*) (CORBA::ORB_ptr, Components::HomeExecutorBase_ptr)) deployfn;

  /*
   * Install Home
   */

  Components::HomeExecutorBase_var he = (*creator) (orb);

  if (CORBA::is_nil (he)) {
    if (verbose) {
      cout << "ComponentServer # " << ccmdtoken << ": "
	   << "oops: entry point returned nil" << endl;
    }
    close_shlib (id);
    throw Components::InstallationFailure ();
  }

  MICO::CCM::ContainerBase * cb = (*deployor) (orb, he);
  assert (cb);

  Components::CCMHome_var home = cb->get_CCM_home ();
  assert (!CORBA::is_nil (home));

  /*
   * Store info
   */

  HomeInfo hi;
  hi.file  = id;
  hi.cont  = cb;
  hi.home  = Components::CCMHome::_duplicate (home);

  _homes.push_back (hi);

  /*
   * Seems to have worked so far
   */

  if (verbose) {
    string homename (entrypt);
    homename = homename.substr (7);
    cout << "ComponentServer # " << ccmdtoken << ": " << homename
	 << " is active." << endl;
  }

  cb->activate ();
  return home._retn ();
}

void
Container_impl::remove_home (Components::CCMHome_ptr home)
{
  list<HomeInfo>::iterator it;

  for (it=_homes.begin(); it!=_homes.end(); it++) {
    if ((*it).cont->compare (home)) {
      break;
    }
  }

  if (it == _homes.end()) {
    throw Components::RemoveFailure ();
  }

  CORBA::release ((*it).home);
  (*it).cont->remove ();
  delete (*it).cont;
  close_shlib ((*it).file.c_str());
  _homes.erase (it);

}

Components::CCMHomes *
Container_impl::get_homes ()
{
  Components::CCMHomes_var homes = new Components::CCMHomes;
  homes->length (_homes.size ());

  list<HomeInfo>::iterator it;
  CORBA::ULong i;

  for (i=0, it=_homes.begin(); it!=_homes.end(); it++) {
    homes[i++] = Components::CCMHome::_duplicate ((*it).home);
  }

  return homes._retn ();
}

void
Container_impl::remove ()
{
  Components::CCMHomes_var homes = get_homes ();

  for (CORBA::ULong i=0; i<homes->length(); i++) {
    remove_home (homes[i]);
  }
}

MICO::SharedLib *
Container_impl::open_shlib (const char * id)
{
  string filename;

  if (root.length() > 0 && *id != '/') {
    filename = root;
    filename += "/";
    filename += id;
  }
  else if (*id != '.' && *id != '/') {
    filename = "./";
    filename += id;
  }
  else {
    filename = id;
  }

  /*
   * See if this shared library is already open
   */

  ShlibMap::iterator it = _loaded_shlibs.find (filename);

  if (it != _loaded_shlibs.end()) {
    (*it).second.count++;
    return (*it).second.shlib;
  }

  if (verbose) {
    cout << "ComponentServer # " << ccmdtoken << ": loading "
	 << filename << endl;
  }

  MICO::UnixSharedLib * thelib = new MICO::UnixSharedLib (filename.c_str());

  if (!thelib) {
    return 0;
  }
  else if (!*thelib) {
    cerr << "Cannot open `" << filename << "': " << thelib->error () << endl;
    delete thelib;
    return 0;
  }
  
  thelib->init ();
  
  ShlibInfo & sinfo = _loaded_shlibs[filename];
  sinfo.count = 1;
  sinfo.shlib = thelib;

  return thelib;
}

void
Container_impl::close_shlib (const char * filename)
{
  ShlibMap::iterator it = _loaded_shlibs.find (filename);

  if (it == _loaded_shlibs.end()) {
    return;
  }

  if (--(*it).second.count) {
    return;
  }

  if (verbose) {
    cout << "ComponentServer # " << ccmdtoken << ": closing "
	 << filename << endl;
  }

  MICO::SharedLib * lib = (*it).second.shlib;
  lib->exit ();

  /*
   * We defer closing the shared library. The problem is that the home
   * reference that has been passed to us (in remove_home, which has
   * called us) contains a CLP (Collocated POA) pointer to the servant.
   * That's normally a Good Thing, as it allows us to bypass marshalling
   * in collocated invocations, but that pointer still points to data in
   * the shared library; precisely, it references the home's virtual table 
   * (__vt_18HelloHome_stub_clp.Q210Components7CCMHome).
   *
   * So if we close the shared library, the skeleton for remove_home will
   * dump core trying to release the home reference.
   *
   * So we must wait for this invocation to complete before closing the
   * shared library. We do that by installing a timeout that closes the
   * shared library right after this invocation has finished (and the
   * event loop gains control again).
   */

  SharedLibraryCloser * slc = new SharedLibraryCloser (lib);
  orb->dispatcher()->tm_event (slc, 0);

  _loaded_shlibs.erase (it);
}

/*
 * ----------------------------------------------------------------------
 * Helper
 * ----------------------------------------------------------------------
 */

SharedLibraryCloser::SharedLibraryCloser (MICO::SharedLib * lib)
{
  _lib = lib;
}

SharedLibraryCloser::~SharedLibraryCloser ()
{
  delete _lib;
}

void
SharedLibraryCloser::callback (CORBA::Dispatcher *,
			       CORBA::DispatcherCallback::Event ev)
{
  if (ev == CORBA::Dispatcher::Timer) {
    delete this;
  }
}

/*
 * ----------------------------------------------------------------------
 * Main
 * ----------------------------------------------------------------------
 */

void
sighandler (int signal)
{
#ifdef HAVE_THREADS
  cond.signal();
#else // HAVE_THREADS
  finished = true;
#endif // HAVE_THREADS
}

static void
usage (const char *progname)
{
  cerr << "usage: " << progname << " [<options>]" << endl;
  cerr << "possible <options> are:" << endl;
  cerr << "    --help" << endl;
  cerr << "    --ior <IOR ref file>" << endl;
  cerr << "    --ccmd <Servant Activator IOR>" << endl;
  cerr << "    --token <Servant Activator Token>" << endl;
  cerr << "    --root <directory>" << endl;
  cerr << "    -v" << endl;
  exit (1);
}

#ifdef HAVE_THREADS
class ORBRunner
    : public virtual MICOMT::Thread
{
public:
    void _run(void*)
    {
	try {
	    orb->run();
	} catch (CORBA::SystemException& ex) {
	    cerr << "caught: " << ex._repoid() << endl;
	}
    }
};

class ORBKiller
    :  public virtual MICOMT::Thread
{
public:
    void _run(void*)
    {
	MICOMT::AutoLock t_lock(mutex);
	cond.wait();
	csimpl->remove();
	poa->destroy (1, 1);
	csimpl->_remove_ref ();
	orb->shutdown(TRUE);
    }
};

#endif // HAVE_THREADS

int
main (int argc, char *argv[])
{
  orb = CORBA::ORB_init (argc, argv);
  MICO::CCM::register_all_factories (orb);
  CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
  poa = PortableServer::POA::_narrow (poaobj);
  PortableServer::POAManager_var mgr = poa->the_POAManager();
  verbose = false;

  /*
   * Process Command Line
   */

  MICOGetOpt::OptMap opts;
  opts["--help"]   = "";
  opts["--ior"]    = "arg-expected";
  opts["--ccmd"]   = "arg-expected";
  opts["--token"]  = "arg-expected";
  opts["--root"]   = "arg-expected";
  opts["-v"]       = "";

  MICOGetOpt opt_parser (opts);
  if (!opt_parser.parse (argc, argv))
    usage (argv[0]);

  string reffile;
  string ccmdior;

  for (MICOGetOpt::OptVec::const_iterator i = opt_parser.opts().begin();
       i != opt_parser.opts().end(); ++i) {
    string arg = (*i).first;
    string val = (*i).second;

    if (arg == "--ior") {
      reffile = val;
    } else if (arg == "--ccmd") {
      ccmdior = val;
    } else if (arg == "--token") {
      ccmdtoken = val;
    } else if (arg == "--root") {
      root = val;
    } else if (arg == "-v") {
      verbose = true;
    } else if (arg == "--help") {
      usage (argv[0]);
    } else {
      usage (argv[0]);
    }
  }

  if (argc != 1) {
    usage (argv[0]);
  }

  /*
   * Create POA for the Containers
   */

  CORBA::PolicyList pl;
  PortableServer::POA_var copoa = poa->create_POA ("Container", mgr, pl);

  /*
   * Do we have a ServerActivator IOR on the command line?
   */

  Components::ServerActivator_var ccmd = Components::ServerActivator::_nil ();

  if (ccmdior.length()) {
    try {
      CORBA::Object_var ccmdobj = orb->string_to_object (ccmdior.c_str());
      ccmd = Components::ServerActivator::_narrow (ccmdobj);
    }
    catch (...) {
    }
    
    if (CORBA::is_nil (ccmd)) {
      cerr << "error: illegal value for --ccmd: " << ccmdior << endl;
      exit (1);
    }
  }

  /*
   * Create our ComponentServer singleton and activate it
   */

  ComponentServer_impl * csimpl = new ComponentServer_impl (ccmd, copoa);
  Components::ComponentServer_var csobj = csimpl->_this ();

  /*
   * Write IOR
   */

  if (reffile.length() > 0) {
    CORBA::String_var nsior = orb->object_to_string (csobj);

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
   * Signal handling
   */
#ifndef HAVE_THREADS
  finished = false;
#endif // HAVE_THREADS
  signal (SIGINT, sighandler);
  signal (SIGTERM, sighandler);

  /*
   * Go Active
   */

  mgr->activate ();

#ifdef HAVE_THREADS
  ORBRunner* runner = new ORBRunner;
  runner->start();
  ORBKiller* killer = new ORBKiller;
  killer->start();
#endif // HAVE_THREADS

  /*
   * Say Hello to our ServerActivator
   */

  MICOCCM::MicoCCMD_var mccmd =
    MICOCCM::MicoCCMD::_narrow (ccmd);

  if (!CORBA::is_nil (mccmd)) {
    try {
      mccmd->callback (ccmdtoken.c_str(), csobj);
    } catch (...) {
      cerr << "ComponentServer # " << ccmdtoken << ": oops: "
	   << "unable to call back MicoCCMD"
	   << endl;
    }
  }

  /*
   * Do something useful
   */

  if (verbose) {
    cout << "ComponentServer # " << ccmdtoken << ": running." << endl;
  }
#ifndef HAVE_THREADS
  while (!finished) {
    orb->perform_work ();
  }
#else // HAVE_THREADS
  runner->wait();
#endif // HAVE_THREADS
  if (verbose) {
    cout << "ComponentServer # " << ccmdtoken << ": terminating." << endl;
  }

#ifndef HAVE_THREADS
  csimpl->remove ();

  /*
   * Shutdown
   */

  poa->destroy (1, 1);
  csimpl->_remove_ref ();
#endif // HAVE_THREADS
  return 0;
}
