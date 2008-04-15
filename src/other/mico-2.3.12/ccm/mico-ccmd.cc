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
#include <mico/os-misc.h>
#include <mico/impl.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif
#include <signal.h>

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif


using namespace std;

/*
 * This file implements the Components::ServerActivator,
 * Components::AssemblyFactory and Components::ComponentInstallation
 * interfaces.
 *
 * "The ServerActivator interface represents a singleton object that
 * resides on each host and acts as a factory for ComponentServer
 * objects."
 *
 * A new process is started for each ComponentServer instance.
 *
 * "The AssemblyFactory interface is used to create Assembly objects.
 * A single AssemblyFactory object must be present on each host where
 * Assembly objects are to be created."
 *
 * "The ComponentInstallation object is used to install, query,
 * and remove component implementations on a single platform. There
 * is at most one ComponentInstallation object per host."
 */

CORBA::ORB_ptr orb;
bool finished;
bool verbose;

/*
 * ----------------------------------------------------------------------
 * ServerActivator implementation
 * ----------------------------------------------------------------------
 */

class ServerActivator_impl :
  virtual public POA_Components::ServerActivator,
  virtual public PortableServer::RefCountServantBase,
  virtual public MICO::ProcessCallback
{
protected:
  struct SvInf {
    bool iscs;
    Components::ComponentServer_var cs;
    MICO::UnixProcess * proc;
  };

  CORBA::ULong counter;
  typedef map<CORBA::ULong, SvInf, less<CORBA::ULong> > MapSvInf;
  MapSvInf svmap;

  string root;

public:
  ServerActivator_impl (const char * = 0);
  ~ServerActivator_impl ();

  Components::ComponentServer_ptr
  create_component_server (const Components::ConfigValues &);

  void remove_component_server (Components::ComponentServer_ptr);

  Components::ComponentServers * get_component_servers ();

  /*
   * Extension
   */

  virtual CORBA::Object_ptr exec (const char *,
				  const Components::NameList &,
				  const char *);

  /*
   * MICO Process callback
   */

  virtual void callback (MICO::Process *, MICO::ProcessCallback::Event);

  /*
   * Destroy
   */

  virtual void destroy ();
};

/*
 * ----------------------------------------------------------------------
 * ServerActivator implementation
 * ----------------------------------------------------------------------
 */

ServerActivator_impl::ServerActivator_impl (const char * _root)
{
  counter = 0;
  if (_root) {
    root = _root;
  }
  if (verbose) {
    cout << "ServerActivator: running." << endl;
  }
}

ServerActivator_impl::~ServerActivator_impl ()
{
  if (verbose) {
    cout << "ServerActivator: terminating." << endl;
  }
}

Components::ComponentServer_ptr
ServerActivator_impl::create_component_server (const Components::ConfigValues &cv)
{
  /*
   * Generate unique token for new component server
   */

  CORBA::ULong mycounter = counter++;
  string cstoken = xdec (mycounter);

  /*
   * Compose command line
   */

  CORBA::Object_var myself = _this ();
  CORBA::String_var myior = orb->object_to_string (myself);

  string cmdline = "componentserver";
  cmdline += " --ccmd ";
  cmdline += myior.in();
  cmdline += " --token ";
  cmdline += cstoken;

  if (verbose) {
    cmdline += " -v";
  }

  if (root.length() > 0) {
    cmdline += " --root ";
    cmdline += root;
  }

  /*
   * Make new entry to our table
   */

  MapSvInf::iterator it;
  SvInf & info = svmap[mycounter];
  info.cs = Components::ComponentServer::_nil ();
  info.iscs = true;

  /*
   * Start Component Server
   */

  info.proc = new MICO::UnixProcess (cmdline.c_str(), this);

  if (!info.proc->run ()) {
    if (verbose) {
      cout << "ServerActivator: exec `" << cmdline << "' failed." << endl;
    }
    it = svmap.find (mycounter);
    delete (*it).second.proc;
    svmap.erase (it);
    throw Components::CreateFailure ();
  }

  /*
   * Wait for the callback to arrive or until the process disappears
   */

  CORBA::Timeout patience (orb->dispatcher(), 60000);
  Components::ComponentServer_var cs =
    Components::ComponentServer::_nil ();

  // cout << "waiting for callback " << mycounter << " ..." << endl;

  while ((it = svmap.find (mycounter)) != svmap.end() && !patience.done()) {
    // cout << "still waiting." << endl;

    if (!CORBA::is_nil ((*it).second.cs)) {
      // cout << "got something." << endl;
      cs = Components::ComponentServer::_duplicate ((*it).second.cs);
      break;
    }

    /*
     * Wait some more for callback to arrive
     */

    orb->perform_work ();
  }

  // cout << "outta loop." << endl;

  /*
   * Decide what happened. Is the process still running? If not, the
   * callback would have removed it from our table.
   */

  if ((it = svmap.find (mycounter)) == svmap.end()) {
    if (verbose) {
      cout << "ServerActivator: crash during exec `" << cmdline << "'"
	   << endl;
    }
    throw Components::CreateFailure ();
  }

  /*
   * Do we still have an invalid IOR?
   */

  if (CORBA::is_nil (cs)) {
    if (verbose) {
      cout << "ServerActivator: component server failed to connect"
	   << endl;
    }
    MapSvInf::iterator it = svmap.find (mycounter);
    if (!(*it).second.proc->exited()) {
      // the callback will remove the element from the list
      (*it).second.proc->terminate ();
    }
    throw Components::CreateFailure ();
  }

  /*
   * Otherwise, all should be well. Bestow the new ComponentServer with
   * its ConfigValues.
   */

  MICOCCM::ComponentServer_var mcs =
    MICOCCM::ComponentServer::_narrow (cs);

  if (!CORBA::is_nil (mcs)) {
    try {
      mcs->set_config_values (cv);
    }
    catch (...) {
      if (verbose) {
	cout << "ServerActivator: unable to call ComponentServer"
	     << endl;
      }
    }
  }

  // cout << "ok." << endl;

  if (verbose) {
    cout << "ServerActivator: new ComponentServer # " << mycounter
	 << " started" << endl;
  }

  return cs._retn ();
}

void
ServerActivator_impl::remove_component_server (Components::ComponentServer_ptr server)
{
  if (CORBA::is_nil (server)) {
    throw Components::RemoveFailure ();
  }

  MapSvInf::iterator it = svmap.begin ();

  while (it != svmap.end()) {
    if (!(*it).second.iscs) {
      it++;
      continue;
    }
    try {
      if (server->_is_equivalent ((*it).second.cs)) {
	break;
      }
    } catch (...) {
    }
    it++;
  }

  if (it == svmap.end()) {
    throw Components::RemoveFailure ();
  }

  if (verbose) {
    cout << "ServerActivator: terminating ComponentServer # "
	 << (*it).first << endl;
  }

  (*it).second.cs = Components::ComponentServer::_nil ();
  (*it).second.proc->terminate ();
}

Components::ComponentServers *
ServerActivator_impl::get_component_servers ()
{
  /*
   * While creating a new component server, some references in our
   * svmap might still be nil.
   */

  Components::ComponentServers_var cs =
    new Components::ComponentServers;

  MapSvInf::iterator it = svmap.begin ();
  CORBA::ULong i=0;

  while (it != svmap.end()) {
    if (!CORBA::is_nil ((*it).second.cs)) {
      cs->length (i+1);
      cs[i++] = Components::ComponentServer::_narrow ((*it).second.cs);
    }
    it++;
  }

  return cs._retn ();
}

/*
 * exec a process
 */

CORBA::Object_ptr
ServerActivator_impl::exec (const char * prog,
			    const Components::NameList & args,
			    const char * iorfile)
{
  /*
   * Generate unique token for new server
   */

  CORBA::ULong mycounter = counter++;
  string cstoken = xdec (mycounter);

  /*
   * Compose command line
   */

  string cmdline;

  if (root.length() > 0 && *prog != '/') {
    cmdline = root;
    cmdline += "/";
    cmdline += prog;
#ifndef _WIN32
#ifdef HAVE_SYS_STAT_H
    // hack: file has been uploaded, need to set exec permission
    if (verbose) {
      cout << "ServerActivator: fixing permissions on " << cmdline << endl;
    }
    if (::chmod (cmdline.c_str(), 0700) != 0) {
      if (verbose) {
	cout << "ServerActivator: error fixing permissions on "
	     << cmdline << endl;
      }
    }
#endif
#endif
  }
  else if (*prog != '.' && *prog != '/') {
    cmdline = "./";
    cmdline += prog;
#ifndef _WIN32
#ifdef HAVE_SYS_STAT_H
    // hack: file has been uploaded, need to set exec permission
    if (verbose) {
      cout << "ServerActivator: fixing permissions on " << cmdline << endl;
    }
    if (::chmod (cmdline.c_str(), 0700) != 0) {
      if (verbose) {
	cout << "ServerActivator: error fixing permissions on "
	     << cmdline << endl;
      }
    }
#endif
#endif
  }
  else {
    cmdline = prog;
  }

  for (CORBA::ULong argc=0; argc<args.length(); argc++) {
    cmdline += " ";
    cmdline += args[argc];
  }

  /*
   * Make new entry to our table
   */

  MapSvInf::iterator it;
  SvInf & info = svmap[mycounter];
  info.cs = Components::ComponentServer::_nil ();
  info.iscs = false;

  /*
   * Remove iorfile
   */

  if (*iorfile) {
    ::remove (iorfile);
  }
  
  /*
   * Start Process
   */

  info.proc = new MICO::UnixProcess (cmdline.c_str(), this);

  if (!info.proc->run ()) {
    if (verbose) {
      cout << "ServerActivator: exec `" << cmdline << "' failed." << endl;
    }
    it = svmap.find (mycounter);
    delete (*it).second.proc;
    svmap.erase (it);
    throw Components::CreateFailure ();
  }

  /*
   * If no iorfile, return "success"
   */

  if (!*iorfile) {
    return CORBA::Object::_nil ();
  }

  /*
   * Wait for the iorfile to appear or until the process disappears
   * - or until 60 seconds have timed out
   *
   * If the file exists, try to read the IOR from it. Maybe the
   * other process is slow in writing it, so if we fail to read
   * a complete IOR, we wait some more.
   */

  CORBA::Timeout patience (orb->dispatcher(), 60000);
  CORBA::Object_var obj = CORBA::Object::_nil ();

  while ((it = svmap.find (mycounter)) != svmap.end() && !patience.done()) {
    if (OSMisc::access (iorfile, OSMisc::ACCESS_READ) == 0) {
      ifstream iorstream (iorfile);
      if (iorstream.good()) {
	CORBA::String_var ior;
	iorstream >> ior;

	try {
	  obj = orb->string_to_object (ior.in());
	}
	catch (...) {
	  obj = CORBA::Object::_nil ();
	}
      }
    }

    if (!CORBA::is_nil (obj)) {
      break;
    }

    CORBA::Timeout waitabit (orb->dispatcher(), 1000);

    while (!waitabit.done()) {
      orb->perform_work ();
    }
  }

  if ((it = svmap.find (mycounter)) == svmap.end()) {
    if (verbose) {
      cout << "ServerActivator: crash during exec `" << cmdline << "'"
	   << endl;
    }

    throw Components::CreateFailure ();
  }

  if (CORBA::is_nil (obj)) {
    if (verbose) {
      cout << "ServerActivator: no valid iorfile " << iorfile 
	   << " after exec `" << cmdline << "'" << endl;
    }

    MapSvInf::iterator it = svmap.find (mycounter);

    if (!(*it).second.proc->exited()) {
      // the callback will remove the element from the list
      (*it).second.proc->terminate ();
    }

    throw Components::CreateFailure ();
  }

  if (verbose) {
    cout << "ServerActivator: new process # " << mycounter
	 << " started: " << prog << endl;
  }

  /*
   * Don't need the ior file any longer
   */

  ::remove (iorfile);
  return obj._retn ();
}

/*
 * Process Callback
 */

void
ServerActivator_impl::callback (MICO::Process * proc,
				MICO::ProcessCallback::Event ev)
{
  /*
   * Find appropriate server
   */

  MapSvInf::iterator it;
  for (it = svmap.begin(); it != svmap.end(); it++) {
    if ((*it).second.proc == proc) {
      break;
    }
  }

  assert (it != svmap.end());

  /*
   * What's happened?
   */

  switch (ev) {
  case MICO::ProcessCallback::Exited:
    // cout << "component server " << (*it).first << " exited." << endl;
    delete ((*it).second.proc);

    if (verbose) {
      cout << "ServerActivator: ComponentServer # "
	   << (*it).first << " has exited." << endl;
    }

    svmap.erase (it);
    break;

  default:
    assert (0);
  }
}

void
ServerActivator_impl::destroy ()
{
  MapSvInf::iterator it;

  for (it=svmap.begin(); it!=svmap.end(); it++) {
    (*it).second.proc->terminate ();
    (*it).second.proc->detach ();
  }

  svmap.clear ();
}

/*
 * ----------------------------------------------------------------------
 * AssemblyFactory implementation
 * ----------------------------------------------------------------------
 */

class AssemblyFactory_impl :
  virtual public POA_Components::AssemblyFactory,
  virtual public PortableServer::RefCountServantBase,
  virtual public MICO::ProcessCallback
{
protected:
  struct AssInf {
    Components::Assembly_var ass;
    MICO::UnixProcess * proc;
  };

  CORBA::ULong counter;
  typedef map<CORBA::ULong, AssInf, less<CORBA::ULong> > MapAssInf;
  MapAssInf assmap;

public:
  AssemblyFactory_impl ();
  ~AssemblyFactory_impl ();

  Components::Cookie * create (const char *);
  Components::Assembly_ptr lookup (Components::Cookie *);
  void destroy (Components::Cookie *);

  /*
   * MICO Process callback
   */

  virtual void callback (MICO::Process *, MICO::ProcessCallback::Event);

  /*
   * Destroy
   */

  virtual void destroy ();
};

/*
 * ----------------------------------------------------------------------
 * AssemblyFactory implementation
 * ----------------------------------------------------------------------
 */

AssemblyFactory_impl::AssemblyFactory_impl ()
{
  counter = 0;
  if (verbose) {
    cout << "AssemblyFactory: running." << endl;
  }
}

AssemblyFactory_impl::~AssemblyFactory_impl ()
{
  if (verbose) {
    cout << "AssemblyFactory: terminating." << endl;
  }
}

Components::Cookie *
AssemblyFactory_impl::create (const char * assembly_loc)
{
  /*
   * Generate unique token for new assembly
   */

  CORBA::ULong mycounter = counter++;
  string cstoken = xdec (mycounter);

  /*
   * Compose command line
   */

  CORBA::Object_var myself = _this ();
  CORBA::String_var myior = orb->object_to_string (myself);

  string cmdline = "assembly";
  cmdline += " --ccmd ";
  cmdline += myior.in();
  cmdline += " --token ";
  cmdline += cstoken;

  if (verbose) {
    cmdline += " -v";
  }

  cmdline += " ";
  cmdline += assembly_loc;

  /*
   * Make new entry to our table
   */

  MapAssInf::iterator it;
  AssInf & info = assmap[mycounter];
  info.ass = Components::Assembly::_nil ();

  /*
   * Start Component Server
   */

  info.proc = new MICO::UnixProcess (cmdline.c_str(), this);

  if (!info.proc->run ()) {
    if (verbose) {
      cout << "AssemblyFactory: exec `" << cmdline << "' failed." << endl;
    }
    it = assmap.find (mycounter);
    delete (*it).second.proc;
    assmap.erase (it);
    throw Components::CreateFailure ();
  }

  /*
   * Wait for the callback to arrive or until the process disappears
   */

  CORBA::Timeout patience (orb->dispatcher(), 60000);
  Components::Assembly_var ass =
    Components::Assembly::_nil ();

  // cout << "waiting for callback " << mycounter << " ..." << endl;

  while ((it = assmap.find (mycounter)) != assmap.end() && !patience.done()) {
    // cout << "still waiting." << endl;

    if (!CORBA::is_nil ((*it).second.ass)) {
      // cout << "got something." << endl;
      ass = Components::Assembly::_duplicate ((*it).second.ass);
      break;
    }

    /*
     * Wait some more for callback to arrive
     */

    orb->perform_work ();
  }

  // cout << "outta loop." << endl;

  /*
   * Decide what happened. Is the process still running? If not, the
   * callback would have removed it from our table.
   */

  if ((it = assmap.find (mycounter)) == assmap.end()) {
    if (verbose) {
      cout << "AssemblyFactory: crash during exec `" << cmdline << "'"
	   << endl;
    }
    throw Components::CreateFailure ();
  }

  /*
   * Do we still have an invalid IOR?
   */

  if (CORBA::is_nil (ass)) {
    if (verbose) {
      cout << "AssemblyFactory: Assembly failed to connect"
	   << endl;
    }
    MapAssInf::iterator it = assmap.find (mycounter);
    if (!(*it).second.proc->exited()) {
      // the callback will remove the element from the list
      (*it).second.proc->terminate ();
    }
    throw Components::CreateFailure ();
  }

  /*
   * Otherwise, all should be well.
   */

  // cout << "ok." << endl;

  if (verbose) {
    cout << "AssemblyFactory: new Assembly # " << mycounter
	 << " started" << endl;
  }

  Components::Cookie_var ck = new MICO::CCM::Cookie_impl;
  ck->CookieValue().replace (sizeof (CORBA::ULong), sizeof (CORBA::ULong),
			     (CORBA::Octet *) &mycounter);
  return ck._retn ();
}

Components::Assembly_ptr
AssemblyFactory_impl::lookup (Components::Cookie * c)
{
  if (!c || c->CookieValue().length() != sizeof (CORBA::ULong)) {
    throw Components::InvalidAssembly ();
  }

  CORBA::ULong mycounter = * ((CORBA::ULong *) c->CookieValue().get_buffer());
  MapAssInf::iterator it = assmap.find (mycounter);

  if (it == assmap.end()) {
    throw Components::InvalidAssembly ();
  }

  return Components::Assembly::_duplicate ((*it).second.ass);
}

void
AssemblyFactory_impl::destroy (Components::Cookie * c)
{
  if (!c || c->CookieValue().length() != sizeof (CORBA::ULong)) {
    throw Components::InvalidAssembly ();
  }

  CORBA::ULong mycounter = * ((CORBA::ULong *) c->CookieValue().get_buffer());
  MapAssInf::iterator it = assmap.find (mycounter);

  if (it == assmap.end()) {
    throw Components::InvalidAssembly ();
  }

  try {
    if (!CORBA::is_nil ((*it).second.ass)) {
      Components::AssemblyState as = (*it).second.ass->get_state ();

      if (as == Components::INSERVICE) {
	if (verbose) {
	  cout << "AssemblyFactory: tearing down Assembly # "
	       << (*it).first << endl;
	}
	(*it).second.ass->tear_down ();
      }
    }
  }
  catch (...) {
  }

  if (verbose) {
    cout << "AssemblyFactory: destroying Assembly # "
	 << (*it).first << endl;
  }

  (*it).second.ass = Components::Assembly::_nil ();
  (*it).second.proc->terminate ();
}

/*
 * Process Callback
 */

void
AssemblyFactory_impl::callback (MICO::Process * proc,
				MICO::ProcessCallback::Event ev)
{
  /*
   * Find appropriate server
   */

  MapAssInf::iterator it;
  for (it = assmap.begin(); it != assmap.end(); it++) {
    if ((*it).second.proc == proc) {
      break;
    }
  }

  assert (it != assmap.end());

  /*
   * What's happened?
   */

  switch (ev) {
  case MICO::ProcessCallback::Exited:
    // cout << "component server " << (*it).first << " exited." << endl;
    delete ((*it).second.proc);

    if (verbose) {
      cout << "AssemblyFactory: Assembly # "
	   << (*it).first << " has exited." << endl;
    }

    assmap.erase (it);
    break;

  default:
    assert (0);
  }
}

void
AssemblyFactory_impl::destroy ()
{
  MapAssInf::iterator it;

  for (it=assmap.begin(); it!=assmap.end(); it++) {
    (*it).second.proc->terminate ();
    (*it).second.proc->detach ();
  }

  assmap.clear ();
}

/*
 * ----------------------------------------------------------------------
 * ComponentInstallation implementation
 * ----------------------------------------------------------------------
 */

class ComponentInstallation_impl :
  virtual public POA_Components::ComponentInstallation,
  virtual public PortableServer::RefCountServantBase
{
protected:
  // map implUUIDs to a local file name (usually the same) and a ref counter

  struct ImplInf {
    bool downloading;
    string location;
    string localname;
    CORBA::ULong refs;
  };

  typedef map<string, ImplInf, less<string> > MapImplInf;
  MapImplInf implmap;
  string root;

public:
  ComponentInstallation_impl (const char * = 0);
  ~ComponentInstallation_impl ();

  void install (const char * implUUID, const char * component_loc);
  void replace (const char * implUUID, const char * component_loc);
  void remove  (const char * implUUID);
  char * get_implementation (const char * implUUID);

  virtual void destroy ();

protected:
  bool download (const char * localname, const char * url, bool force);
  bool download_file (const char * localname, const char * url, bool force);
  bool download_http (const char * localname, const char * url, bool force);
};

/*
 * ----------------------------------------------------------------------
 * ComponentInstallation implementation
 * ----------------------------------------------------------------------
 */

ComponentInstallation_impl::ComponentInstallation_impl (const char * _root)
{
  if (_root) {
    root = _root;
  }
  if (verbose) {
    cout << "ComponentInstallation: running." << endl;
  }
}

ComponentInstallation_impl::~ComponentInstallation_impl ()
{
  if (verbose) {
    cout << "ComponentInstallation: terminating." << endl;
  }
}

void
ComponentInstallation_impl::install (const char * implUUID,
				     const char * component_loc)
{
  MapImplInf::iterator it = implmap.find (implUUID);

  if (it != implmap.end()) {
    assert (!(*it).second.downloading);
    (*it).second.refs++;
    return;
  }

  ImplInf & ii = implmap[implUUID];
  ii.downloading = 1;
  ii.localname = implUUID;
  ii.location = component_loc;
  ii.refs = 1;

  bool downloaded;

  try {
    downloaded = download (implUUID, component_loc, 0);
  }
  catch (...) {
    implmap.erase (implUUID);
    throw;
  }

  if (!downloaded) {
    implmap.erase (implUUID);
    throw Components::InstallationFailure ();
  }

  it = implmap.find (implUUID);
  (*it).second.downloading = 0;
}

void
ComponentInstallation_impl::replace (const char * implUUID,
				     const char * component_loc)
{
  MapImplInf::iterator it = implmap.find (implUUID);

  if (it == implmap.end()) {
    install (implUUID, component_loc);
    return;
  }

  (*it).second.refs += 1;
  bool downloaded;

  try {
    downloaded = download (implUUID, component_loc, 1);
  }
  catch (...) {
    it = implmap.find (implUUID);
    assert (it != implmap.end());
    (*it).second.refs -= 1;
    throw;
  }

  it = implmap.find (implUUID);
  assert (it != implmap.end());
  (*it).second.refs -= 1;

  if (!downloaded) {
    throw Components::InstallationFailure ();
  }

  (*it).second.location = component_loc;
}

void
ComponentInstallation_impl::remove (const char * implUUID)
{
  MapImplInf::iterator it = implmap.find (implUUID);

  if (it == implmap.end()) {
    throw Components::UnknownImplId ();
  }

  if ((*it).second.downloading) {
    throw Components::RemoveFailure ();
  }

  if ((*it).second.refs > 0) {
    (*it).second.refs -= 1;
  }

  if ((*it).second.refs == 0) {
    string outname, tmpname;

    if (root.length() > 0) {
      outname  = root;
      outname += "/";
      outname += (*it).second.localname;
      tmpname  = outname;
      tmpname += ".tmp";
    }
    else {
      outname  = (*it).second.localname;
      tmpname  = outname;
      tmpname += ".tmp";
    }

    if (verbose) {
      cout << "ComponentInstallation: removing implementation "
	   << implUUID << endl;
    }

    if (::remove ((*it).second.localname.c_str()) != 0) {
      throw Components::RemoveFailure ();
    }
  }
}

char *
ComponentInstallation_impl::get_implementation (const char * implUUID)
{
  MapImplInf::iterator it = implmap.find (implUUID);

  if (it == implmap.end()) {
    throw Components::UnknownImplId ();
  }

  return CORBA::string_dup ((*it).second.location.c_str());
}

void
ComponentInstallation_impl::destroy ()
{
  MapImplInf::iterator it = implmap.begin ();
  string outname, tmpname;

  while (it != implmap.end()) {
    if (root.length() > 0) {
      outname  = root;
      outname += "/";
      outname += (*it).second.localname;
      tmpname  = outname;
      tmpname += ".tmp";
    }
    else {
      outname  = (*it).second.localname;
      tmpname  = outname;
      tmpname += ".tmp";
    }

    if (OSMisc::access (outname.c_str(), OSMisc::ACCESS_READ) == 0) {
      if (verbose) {
	cout << "ComponentInstallation: removing " << outname << endl;
      }
      if (::remove (outname.c_str()) != 0) {
	if (verbose) {
	  cout << "ComponentInstallation: error removing " << outname << endl;
	}
      }
    }

    if (OSMisc::access (tmpname.c_str(), OSMisc::ACCESS_READ) == 0) {
      if (verbose) {
	cout << "ComponentInstallation: removing " << tmpname << endl;
      }
      if (::remove (tmpname.c_str()) != 0) {
	if (verbose) {
	  cout << "ComponentInstallation: error removing " << tmpname << endl;
	}
      }
    }

    it++;
  }

  implmap.clear ();
}

bool
ComponentInstallation_impl::download (const char * localname,
				      const char * url,
				      bool force)
{
  if (strncmp (url, "file://", 7) == 0) {
    return download_file (localname, url, force);
  }
  else if (strncmp (url, "http://", 7) == 0) {
    return download_http (localname, url, force);
  }

  if (verbose) {
    cout << "ComponentInstallation: unknown URL type " << url << endl;
  }

  throw Components::InvalidLocation ();
}

bool
ComponentInstallation_impl::download_file (const char * localname,
					   const char * urlptr,
					   bool force)
{
  string filename (urlptr);
  size_t pos = filename.find ("//");
  size_t pos2 = filename.find ('/', pos+2);

  if (verbose) {
    cout << "ComponentInstallation: copying "
	 << urlptr << " as " << localname << endl;
  }

  if (pos2 == (size_t) -1) {
    if (verbose) {
      cout << "ComponentInstallation: error: illegal file:// URL" << endl;
    }
    throw Components::InvalidLocation ();
  }

  string host = filename.substr (pos+2, pos2-pos-2);
  filename = filename.substr (pos2);

  /*
   * Hack: understand file://[pwd] in Windows, which expands to
   *       file://<drive>:/path. The drive letter would be read as
   *       host here.
   */

#ifdef _WIN32
  if (host.length() > 1 && host[1]==':') {
    filename = host + filename;
    host = "";
  }
#endif

  if (host.length() > 0) {
    if (verbose) {
      cout << "ComponentInstallation: error: cannot access remote file "
	   << "on host " << host << endl;
    }
    throw Components::InvalidLocation ();
  }

  CORBA::ULong len;
  CORBA::String_var fn =
    (char *) mico_url_decode (filename.c_str(), len);
  string ntfn (fn.in(), len);

  /*
   * Hack: accept //drive/path in Win32
   */

#ifdef _WIN32
  if (ntfn.length() > 3 && ntfn[0]=='/' && ntfn[1]=='/' && ntfn[3]=='/') {
    ntfn = ntfn.substr(2);
    ntfn[1] = ':';
  }
#endif

  ifstream in (ntfn.c_str(), ios::in | ios::binary);

  if (!in.good()) {
    if (verbose) {
      cout << "ComponentInstallation: error: cannot open "
	   << ntfn << " for reading" << endl;
    }
    throw Components::InvalidLocation ();
  }

  /*
   * Determine output filename
   */

  string outname, tmpname;

  if (root.length() > 0) {
    outname  = root;
    outname += "/";
    outname += localname;
    tmpname  = outname;
    tmpname += ".tmp";
  }
  else {
    outname  = localname;
    tmpname  = outname;
    tmpname += ".tmp";
  }

  if (OSMisc::access (outname.c_str(), OSMisc::ACCESS_READ) == 0 && !force) {
    if (verbose) {
      cout << "ComponentInstallation: error: target file "
	   << outname << " already exists" << endl;
    }
    throw Components::InstallationFailure ();
  }

  ofstream out (tmpname.c_str(), ios::out | ios::trunc | ios::binary);

  if (!out.good()) {
    if (verbose) {
      cout << "ComponentInstallation: error: cannot open "
	   << tmpname << " for writing" << endl;
    }
    throw Components::InstallationFailure ();
  }

  /*
   * copy file
   */

  char buf[1024];
  int blocks = 0;
  int count;

  if (verbose) {
    cout << "ComponentInstallation: copying "
	 << outname << " " << flush;
  }

  while (in.good() && out.good() && !in.eof()) {
    count = in.read (buf, 1024).gcount ();
    out.write (buf, count);

    if (++blocks == 50 && verbose) {
      cout << "." << flush;
      blocks = 0;
    }
  }

  if ((!in.eof() && !in.good()) || !out.good()) {
    if (verbose) {
      cout << endl;
      cout << "ComponentInstallation: error copying file." << endl;
    }
    ::remove (tmpname.c_str());
    throw Components::InstallationFailure ();
  }

  in.close ();
  out.close ();

  if (verbose) {
    cout << " done." << endl;
  }

  if (::rename (tmpname.c_str(), outname.c_str()) != 0) {
    if (verbose) {
      cout << "ComponentInstallation: error renaming "
	   << tmpname << " to " << outname << endl;
    }
    ::remove (tmpname.c_str());
    throw Components::InstallationFailure ();
  }

  return true;
}

bool
ComponentInstallation_impl::download_http (const char * localname,
					   const char * urlptr,
					   bool force)
{
  string url (urlptr);
  size_t pos = url.find ("//");
  string host, port, abs_path;
  size_t pos2 = url.find ('/', pos+2);

  if (verbose) {
    cout << "ComponentInstallation: downloading "
	 << urlptr << " as " << localname << endl;
  }

  if (pos2 != (size_t) -1) {
    abs_path = url.substr (pos2);
    host = url.substr (pos+2, pos2-pos-2);
  }
  else {
    abs_path = "/";
    host = url;
  }

  if ((pos = host.find (':')) != (size_t) -1) {
    port = host.substr (pos+1);
    host = host.substr (0, pos);
  }
  else {
    port = "80";
  }

  CORBA::UShort portno = atoi (port.c_str());
  MICO::InetAddress * addr = new MICO::InetAddress (host.c_str(), portno);

  if (!addr->valid()) {
    if (verbose) {
      cout << "ComponentInstallation: cannot resolve " << host << endl;
    }
    delete addr;
    throw Components::InvalidLocation ();
  }

  CORBA::Transport * trans = addr->make_transport ();

  if (!trans || trans->bad() || !trans->connect(addr)) {
    if (verbose) {
      cout << "ComponentInstallation: cannot connect to " << host << endl;
    }
    delete trans;
    delete addr;
    throw Components::InvalidLocation ();
  }

  delete addr;

  /*
   * Determine output filename
   */

  string outname, tmpname;

  if (root.length() > 0) {
    outname  = root;
    outname += "/";
    outname += localname;
    tmpname  = outname;
    tmpname += ".tmp";
  }
  else {
    outname  = localname;
    tmpname  = outname;
    tmpname += ".tmp";
  }

  if (OSMisc::access (outname.c_str(), OSMisc::ACCESS_READ) == 0 && !force) {
    if (verbose) {
      cout << "ComponentInstallation: error: target file "
	   << outname << " already exists" << endl;
    }
    throw Components::InstallationFailure ();
  }

  ofstream out (tmpname.c_str(), ios::out | ios::trunc | ios::binary);

  if (!out.good()) {
    if (verbose) {
      cout << "ComponentInstallation: error: cannot open "
	   << outname << " for writing" << endl;
    }
    delete trans;
    throw Components::InstallationFailure ();
  }

  /*
   * For simplicity, speak HTTP/0.9
   */
  CORBA::Long written = 0;
  if (trans->write ("GET ", 4) != 4
      || (written = trans->write ((void *) abs_path.c_str(),
				  abs_path.length())) < 0
      || (CORBA::ULong)written != abs_path.length()
      || trans->write ("\r\n\r\n", 4) != 4) {
    ::remove (tmpname.c_str());
    delete trans;
    throw Components::InvalidLocation ();
  }

  /*
   * Read response
   */

  char buf[1024];
  int blocks = 0;

  if (verbose) {
    cout << "ComponentInstallation: downloading "
	 << outname << " " << flush;
  }

  while (!trans->eof() && !trans->bad() && out.good()) {
    CORBA::Long got = trans->read (buf, 1024);
    if (got > 0) {
      out.write (buf, got);
    }
    if (++blocks == 50 && verbose) {
      cout << "." << flush;
      blocks = 0;
    }
  }

  if (trans->bad() || !out.good()) {
    if (verbose) {
      cout << endl;
      cout << "ComponentInstallation: error downloading file." << endl;
    }
    ::remove (outname.c_str());
    delete trans;
    throw Components::InstallationFailure ();
  }

  delete trans;
  out.close ();

  if (verbose) {
    cout << " done." << endl;
  }

  if (::rename (tmpname.c_str(), outname.c_str()) != 0) {
    if (verbose) {
      cout << "ComponentInstallation: error renaming "
	   << tmpname << " to " << outname << endl;
    }
    ::remove (tmpname.c_str());
    throw Components::InstallationFailure ();
  }

  return true;
}

/*
 * ----------------------------------------------------------------------
 * Implementation for both ServerActivator and ComponentInstallation
 * ----------------------------------------------------------------------
 */

class MicoCCMD_impl :
  virtual public POA_MICOCCM::MicoCCMD,
  virtual public ComponentInstallation_impl,
  virtual public ServerActivator_impl,
  virtual public AssemblyFactory_impl
{
public:
  MicoCCMD_impl (const char * = 0);
  ~MicoCCMD_impl ();

  /*
   * Extension
   */

  CORBA::Object_ptr exec (const char *,
			  const Components::NameList &,
			  const char *);

  /*
   * MICO Process callback
   */

  virtual void callback (MICO::Process *, MICO::ProcessCallback::Event);

  /*
   * Callback from ComponentServer or Assembly
   */

  void callback (const char *, CORBA::Object_ptr);

  /*
   * Destroy
   */

  void destroy ();
};

MicoCCMD_impl::MicoCCMD_impl (const char * _root)
  : ComponentInstallation_impl (_root),
    ServerActivator_impl (_root)
{
}

MicoCCMD_impl::~MicoCCMD_impl ()
{
}

CORBA::Object_ptr
MicoCCMD_impl::exec (const char * prog,
		     const Components::NameList & args,
		     const char * iorfilename)
{
  return ServerActivator_impl::exec (prog, args, iorfilename);
}

void
MicoCCMD_impl::destroy ()
{
  ServerActivator_impl::destroy ();
  AssemblyFactory_impl::destroy ();
  ComponentInstallation_impl::destroy ();
}

/*
 * MICO Process callback
 */

void
MicoCCMD_impl::callback (MICO::Process * proc,
			 MICO::ProcessCallback::Event ev)
{
  /*
   * Find appropriate ServerActivator
   */

  {
    MapSvInf::iterator it;
    for (it = svmap.begin(); it != svmap.end(); it++) {
      if ((*it).second.proc == proc) {
	break;
      }
    }

    if (it != svmap.end()) {
      ServerActivator_impl::callback (proc, ev);
      return;
    }
  }

  /*
   * Find appropriate AssemblyFactory
   */

  {
    MapAssInf::iterator it;
    for (it = assmap.begin(); it != assmap.end(); it++) {
      if ((*it).second.proc == proc) {
	break;
      }
    }

    if (it != assmap.end()) {
      AssemblyFactory_impl::callback (proc, ev);
      return;
    }
  }

  assert (0);
}

/*
 * Callback from ComponentServer or Assembly
 */

void
MicoCCMD_impl::callback (const char * token,
			 CORBA::Object_ptr cs)
{
  // cout << "callback token " << token << endl;

  CORBA::ULong mycounter = atoi (token);

  if (cs->_is_a ("IDL:omg.org/Components/ComponentServer:1.0")) {
    MapSvInf::iterator it = svmap.find (mycounter);
    assert (it != svmap.end());
    assert (CORBA::is_nil ((*it).second.cs));
    (*it).second.cs = Components::ComponentServer::_narrow (cs);
  }
  else if (cs->_is_a ("IDL:omg.org/Components/Assembly:1.0")) {
    MapAssInf::iterator it = assmap.find (mycounter);
    assert (it != assmap.end());
    assert (CORBA::is_nil ((*it).second.ass));
    (*it).second.ass = Components::Assembly::_narrow (cs);
  }
  else {
    assert (0);
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
  finished = true;
}

static void
usage (const char *progname)
{
  cerr << "usage: " << progname << " [<options>]" << endl;
  cerr << "possible <options> are:" << endl;
  cerr << "    --root <pkgdir>" << endl;
  cerr << "    --help" << endl;
  cerr << "    --ior <IOR ref file>" << endl;
  cerr << "    -v" << endl;
  exit (1);
}

int
main (int argc, char *_argv[])
{
  /*
   * add -POAImplName to argv if missing...
   */

  const char * implname = NULL;
  char **argv = new char*[argc+2+1];

  for (int i=0; i<argc; ++i) {
    if (strcmp (_argv[i], "-POAImplName") == 0 && i+1<argc) {
      implname = _argv[i+1];
    }
    argv[i] = _argv[i];
  }

  if (!implname) {
    argv[argc++] = "-POAImplName";
    argv[argc++] = "MicoCCMD";
  }

  argv[argc] = 0;

  /*
   * Init ORB
   */

  orb = CORBA::ORB_init (argc, argv);
  MICO::CCM::register_all_factories (orb);
  CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var RootPOA = PortableServer::POA::_narrow (poaobj);
  PortableServer::POAManager_var mgr = RootPOA->the_POAManager();
  verbose = false;

  /*
   * Process Command Line
   */

  MICOGetOpt::OptMap opts;
  opts["--root"] = "arg-expected";
  opts["--help"] = "";
  opts["--ior"]  = "arg-expected";
  opts["-v"]     = "";

  MICOGetOpt opt_parser (opts);
  if (!opt_parser.parse (argc, argv))
    usage (argv[0]);

  string reffile;
  string root;

  for (MICOGetOpt::OptVec::const_iterator i = opt_parser.opts().begin();
       i != opt_parser.opts().end(); ++i) {
    string arg = (*i).first;
    string val = (*i).second;

    if (arg == "--ior") {
      reffile = val;
    } else if (arg == "-v") {
      verbose = true;
    } else if (arg == "--root") {
      root = val;
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
   * Create persistent POA for myself
   */

  CORBA::PolicyList pl;
  pl.length (2);
  pl[0] = RootPOA->create_lifespan_policy (PortableServer::PERSISTENT);
  pl[1] = RootPOA->create_id_assignment_policy (PortableServer::USER_ID);
  PortableServer::POA_var mypoa =
    RootPOA->create_POA ("MicoCCMD", mgr, pl);

  /*
   * Create our ServerActivator singleton and activate it
   */

  MicoCCMD_impl * aiimpl =
    new MicoCCMD_impl (root.c_str());
  PortableServer::ObjectId_var oid =
    PortableServer::string_to_ObjectId ("MicoCCMD");
  mypoa->activate_object_with_id (oid.in(), aiimpl);
  CORBA::Object_var saobj = mypoa->id_to_reference (oid.in());

  /*
   * Write IOR
   */

  if (reffile.length() > 0) {
    CORBA::String_var saior = orb->object_to_string (saobj);

    if (reffile == "-") {
      cout << saior << endl;
    }
    else {
      ofstream out (reffile.c_str());
      if (!out) {
	cerr << "error: cannot open output file " << reffile << endl;
      }
      else {
	out << saior << endl;
	out.close ();
      }
    }
  }

  /*
   * Signal handling
   */

  finished = false;
  signal (SIGINT, sighandler);
  signal (SIGTERM, sighandler);

  /*
   * Go Active
   */

  mgr->activate ();

  while (!finished) {
    orb->perform_work ();
  }

  /*
   * Shutdown
   */

  RootPOA->destroy (TRUE, TRUE);
  aiimpl->destroy ();
  aiimpl->_remove_ref ();

  return 0;
}
