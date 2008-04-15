/*
 *  Implementation of COSS Naming Service for MICO
 *  Copyright (C) 1997 Kai-Uwe Sattler
 *  Interoperable Naming Service extension by Frank Pilhofer
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

#include <CORBA.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif
#include <Naming_impl.h>
#include <mico/util.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif


using namespace std;

/*
 * NamingContext
 */

CORBA::ORB_ptr NamingContext_impl::useorb;
PortableServer::POA_ptr NamingContext_impl::usepoa;
NamingContext_impl * NamingContext_impl::root;
NamingContext_impl::ServantMap NamingContext_impl::svmap;

NamingContext_impl::LoadBalanceApproach NamingContext_impl::staticLoadBalanceApproach;

/*
 * Constructor for Root Naming Context
 */

NamingContext_impl::NamingContext_impl (CORBA::ORB_ptr orb,
					PortableServer::POA_ptr poa,
                                        LoadBalanceApproach eLoadBalanceApproach)
{
  useorb = CORBA::ORB::_duplicate (orb);
  usepoa = PortableServer::POA::_duplicate (poa);
  root = this;
  iDebug = FALSE;

  staticLoadBalanceApproach = eLoadBalanceApproach;
}

/*
 * Constructor for other Naming Contexts
 */

NamingContext_impl::NamingContext_impl ()
{
  iDebug = FALSE;
}

NamingContext_impl::~NamingContext_impl ()
{
  if (this == root) {
    root = NULL;
    CORBA::release (usepoa);
    CORBA::release (useorb);
    usepoa = PortableServer::POA::_nil ();
    useorb = CORBA::ORB::_nil ();
  }
}

/*
 * Save and restore a Naming Context
 * First line contains a count of the entries to come.
 * Each entry is two lines wide, one containing the stringified relative
 * NameComponent, the second contains the CosName::Binding information.
 */

void
NamingContext_impl::save (ostream & out)
{
  out << table.size() << endl;

  BindingTable::iterator i;
  for (i=table.begin(); i != table.end (); i++) {
    string compname =
      NamingContextExt_impl::StringifyComponent ((*i).first);
    out << compname.c_str() << endl;

    /*
     * For a collocated object (i.e. a NamingContext object in the local
     * process), we store the ObjectId, so that it will still work if the
     * Name Server process is restarted on a different host/port.
     */

    if ((*i).second.btype == CosNaming::ncontext) {
      CORBA::String_var objname;
      assert (!CORBA::is_nil ((*i).second.obj));

#ifdef HAVE_EXCEPTIONS
      try {
	PortableServer::ObjectId_var oid =
	  usepoa->reference_to_id ((*i).second.obj);
	objname = mico_url_encode (oid->get_buffer(), oid->length());
      } catch (PortableServer::POA::WrongAdapter &) {
	objname = useorb->object_to_string ((*i).second.obj);
      }
#else
      objname = useorb->object_to_string ((*i).second.obj);
#endif
      out << "ncontext: " << objname.in() << endl;
    }
    else {
      CORBA::String_var objname;

      if (!CORBA::is_nil ((*i).second.obj)) {
	objname = useorb->object_to_string ((*i).second.obj);
      }
      else {
	objname = (const char *) "(null)";
      }
      out << "nobject: " << objname.in() << endl;
    }
  }
}

void
NamingContext_impl::restore (istream & in)
{
  char buf[256];

  if (!in.getline (buf, 256)) {
    return;
  }

  int count = atoi (buf);

  for (int i=0; i<count; i++) {
    CosNaming::NameComponent nc;
    NmCtxBinding binding;

    /*
     * Read the stringified name
     */

    in.getline (buf, 256);
    const char * ptr = buf;
    if (!NamingContextExt_impl::ScanComponent (ptr, nc)) {
      /* oops */
      assert (0);
    }
    if (*ptr) {
      /* oops */
      assert (0);
    }

    /*
     * Read type, ior and then discard line feed
     */

    string type, ior;
    in >> type >> ior;
 
    in.getline (buf, 256);

    /*
     * If ior starts with IOR:, then it is indeed an object.
     * Else, it is an ObjectId.
     */

    if (strncmp (ior.c_str(), "IOR:", 4) == 0) {
      binding.obj = useorb->string_to_object (ior.c_str());
    }
    else {
      CORBA::ULong length;
      CORBA::Octet * data = mico_url_decode (ior.c_str(), length);
      PortableServer::ObjectId oid (length, length, data, TRUE);
#ifdef HAVE_EXCEPTIONS
      try {
          binding.obj = usepoa->id_to_reference(oid); // We have already activated
      } catch ( const PortableServer::POA::ObjectNotActive& ona) {
          cerr << endl << "NamingContext_impl::restore-ObjectNotActive-" <<
                                          ptr << endl;
          cerr << "                                           -" << ior.c_str()
                                                      << endl;
          break;
      }
#else
      binding.obj = usepoa->create_reference_with_id (oid, 
                                "IDL:omg.org/CosNaming/NamingContextExt:1.0");
#endif
                                                  // all naming contexts
    }

    if (strcmp (type.c_str(), "ncontext:") == 0) {
      binding.btype = CosNaming::ncontext;
    }
    else if (strcmp (type.c_str(), "nobject:") == 0) {
      binding.btype = CosNaming::nobject;
    }

    /*
     * Insert into table
     */

    table[nc] = binding;

    if (staticLoadBalanceApproach != NONE)
      AddLoadBalanceEntry (&nc);
  }
}

void NamingContext_impl::_do_bind (const CosNaming::Name& n, 
				   CORBA::Object_ptr obj,
				   CosNaming::BindingType btype,
				   CORBA::Boolean rebind)
{
  MICOMT::AutoLock lock(table);
  if (n.length () == 0) 
    mico_throw (CosNaming::NamingContext::InvalidName ());

  BindingTable::iterator i = table.find (n[0]);
  if (n.length () == 1) {
    if (i != table.end () && !rebind) {
      mico_throw (CosNaming::NamingContext::AlreadyBound ());
    }
    else {
      NmCtxBinding binding;

      binding.btype = btype;
      binding.obj = CORBA::Object::_duplicate (obj);
      table[n[0]] = binding;

      if (staticLoadBalanceApproach != NONE)
        AddLoadBalanceEntry (&(n[0]));

      /*
      cout << "bound name = " << n[0].id 
           << " to object = " << _orbnc()->object_to_string (obj) << endl;
       */
    }
  }
  else {
    if (i == table.end ()) {
      CosNaming::NamingContext::NotFound exc;
      exc.why = CosNaming::NamingContext::missing_node;
      exc.rest_of_name = n;
      mico_throw (exc);
    }

    NmCtxBinding binding = (*i).second;
    if (binding.btype != CosNaming::ncontext)
      mico_throw (CosNaming::NamingContext::CannotProceed ());

    CosNaming::NamingContext_var nc = 
      CosNaming::NamingContext::_narrow (binding.obj);
    assert (!CORBA::is_nil (nc));

    CosNaming::Name rest;
    rest.length (n.length () - 1);

    for (CORBA::ULong k = 0; k < rest.length (); k++)
      rest[k] = n[k + 1];

    switch (btype) {
    case CosNaming::nobject:
      if (rebind)
	nc->rebind (rest, obj);
      else
	nc->bind (rest, obj);
      break;

    case CosNaming::ncontext:
      CosNaming::NamingContext_var nc2 =
	CosNaming::NamingContext::_narrow (obj);

      if (rebind)
	nc->rebind_context (rest, nc2);
      else
	nc->bind_context (rest, nc2);
      break;
    }
  }
}

CosNaming::NamingContext_ptr
NamingContext_impl::_force_local(CosNaming::NamingContext_ptr nc_in)
{
  PortableServer::ObjectId* oid;
  CORBA::Object_var obj;
  CORBA::Boolean remote = FALSE;
#ifdef HAVE_EXCEPTIONS
  try {
     oid = usepoa->reference_to_id(nc_in);
  } catch ( const PortableServer::POA::WrongPolicy& wp) {
     std::cerr << wp << endl;
     assert(0);     // cannot get here
  } catch ( const PortableServer::POA::WrongAdapter& wa) {
     remote = TRUE;
  }
  if ( remote ) return nc_in;
#else
  return nc_in;
#endif

#ifdef HAVE_EXCEPTIONS
  try {
#endif
     obj = usepoa->id_to_reference(*oid); // This ref comes from ActiveObjectMap
#ifdef HAVE_EXCEPTIONS
  } catch ( const PortableServer::POA::WrongPolicy& wp) {
     std::cerr << wp << endl;
     assert(0);     // cannot get here
  } catch ( const PortableServer::POA::ObjectNotActive& ona) {
     std::cerr << ona << endl;
     assert(0);     // should never happen, all NamingContexts are active
  }
#endif
  CosNaming::NamingContext_ptr nc = CosNaming::NamingContext::_narrow(obj);
  if ( CORBA::is_nil(nc) ) assert(0);  // cannot happen
  //CORBA::release(nc_in);
  return nc;
}

void NamingContext_impl::bind (const CosNaming::Name& n, 
			       CORBA::Object_ptr obj)
{
  _do_bind (n, obj, CosNaming::nobject, FALSE);
}

void NamingContext_impl::rebind (const CosNaming::Name& n, 
				 CORBA::Object_ptr obj)
{
  _do_bind (n, obj, CosNaming::nobject, TRUE);
}

void NamingContext_impl::bind_context (const CosNaming::Name& n, 
				       CosNaming::NamingContext_ptr nc)
{
  nc = _force_local(nc); 
  _do_bind (n, nc, CosNaming::ncontext, FALSE);
}

void NamingContext_impl::rebind_context (const CosNaming::Name& n, 
					 CosNaming::NamingContext_ptr nc)
{
  nc = _force_local(nc); 
  _do_bind (n, nc, CosNaming::ncontext, TRUE);
}

CORBA::Object_ptr NamingContext_impl::resolve 
(const CosNaming::Name& nOriginal)
{
  MICOMT::AutoLock lock(table);
  CosNaming::Name n;
  CosNaming::NameComponent ncNew;

  n = nOriginal;

  if (n.length () == 0)
    mico_throw (CosNaming::NamingContext::InvalidName ());

  if (staticLoadBalanceApproach != NONE)
  {
    if (n.length () == 1)
    {
      if (staticLoadBalanceApproach == ROUND_ROBIN)
        ncNew = RoundRobinResolve (&(n[0]));
      else if (staticLoadBalanceApproach == RANDOM)
        ncNew = RandomResolve (&(n[0]));
      else
        cout << "NamingContext_impl::resolve() cannot determine "
             << "load balancing approach !!!" << endl;

      n.length (1);
      n[0] = ncNew;
    }
  }

  BindingTable::iterator i = table.find (n[0]);
  if (i == table.end ()) {
    CosNaming::NamingContext::NotFound exc;
    exc.why = CosNaming::NamingContext::missing_node;
    exc.rest_of_name = n;
    mico_throw (exc);
  }

  NmCtxBinding binding = (*i).second;

  if (n.length () == 1) {
    /*
    cout << "resolved objref = " 
	 << _orbnc()->object_to_string (binding.obj) << endl;
    */
    return CORBA::Object::_duplicate (binding.obj);
  }
  else {
    if (binding.btype != CosNaming::ncontext)
      mico_throw (CosNaming::NamingContext::CannotProceed ());

    CosNaming::Name rest;
    rest.length (n.length () - 1);
    for (CORBA::ULong k = 0; k < rest.length (); k++)
      rest[k] = n[k + 1];

    CosNaming::NamingContext_var nc = 
      CosNaming::NamingContext::_narrow (binding.obj);
    assert (!CORBA::is_nil (nc));
    return nc->resolve (rest);
  }
  // never reached - just to avoid warning
  assert(0);
  return CORBA::Object::_nil();
}

void NamingContext_impl::unbind (const CosNaming::Name& n)
{
  MICOMT::AutoLock lock(table);
  if (n.length () == 0)
    mico_throw (CosNaming::NamingContext::InvalidName ());

  BindingTable::iterator i = table.find (n[0]);
  if (i == table.end ()) {
    CosNaming::NamingContext::NotFound exc;
    exc.why = CosNaming::NamingContext::missing_node;
    exc.rest_of_name = n;
    mico_throw (exc);
  }

  NmCtxBinding binding = (*i).second;

  if (n.length () == 1) {
    table.erase (i);

    if (staticLoadBalanceApproach != NONE)
      RemoveLoadBalanceEntry (&(n[0]));
  }
  else {
    if (binding.btype != CosNaming::ncontext) 
      mico_throw (CosNaming::NamingContext::CannotProceed ());

    CosNaming::Name rest;
    rest.length (n.length () - 1);
    for (CORBA::ULong k = 0; k < rest.length (); k++)
      rest[k] = n[k + 1];

    CosNaming::NamingContext_var nc = 
      CosNaming::NamingContext::_narrow (binding.obj);
    assert (!CORBA::is_nil (nc));
    nc->unbind (rest);
  }
}

CosNaming::NamingContext_ptr 
NamingContext_impl::new_context ()
{
  NamingContext_impl * nci = new NamingContext_impl;
  PortableServer::ObjectId_var oid = usepoa->activate_object (nci);
  CORBA::Object_var obj = usepoa->id_to_reference (oid.in());
  CosNaming::NamingContext_ptr nc = CosNaming::NamingContext::_narrow (obj);
  MICOMT::AutoLock lock(svmap);
  svmap.insert (nci);
  return nc;
}

CosNaming::NamingContext_ptr
NamingContext_impl::bind_new_context (const CosNaming::Name& n)
{
  NamingContext_impl * nci = new NamingContext_impl;
  PortableServer::ObjectId_var oid = usepoa->activate_object (nci);
  CORBA::Object_var obj = usepoa->id_to_reference (oid.in());
  CosNaming::NamingContext_ptr nc = CosNaming::NamingContext::_narrow (obj);
  {
     MICOMT::AutoLock lock(svmap);
     svmap.insert (nci);
  }
  bind_context (n, nc);
  return nc;
}

void NamingContext_impl::destroy ()
{
  {
      MICOMT::AutoLock lock(table);
      if (table.size () > 0)
        mico_throw (CosNaming::NamingContext::NotEmpty ());
  }

  CORBA::Object_var obj = useorb->resolve_initial_references ("POACurrent");
  PortableServer::Current_var pc = PortableServer::Current::_narrow (obj);
  PortableServer::POA_var poa = pc->get_POA ();
  PortableServer::ObjectId_var oid = pc->get_object_id ();
  poa->deactivate_object (oid.in());
  MICOMT::AutoLock lock(svmap);
  svmap.erase (this);
  _remove_ref ();
}

void NamingContext_impl::list (CORBA::ULong how_many, 
			       CosNaming::BindingList_out bl, 
			       CosNaming::BindingIterator_out bi)
{
  MICOMT::AutoLock lock(table);
  CORBA::ULong num = table.size () < how_many ? table.size () : how_many;
  bl = new CosNaming::BindingList (num);
  bl->length (num);
  bi = CosNaming::BindingIterator::_nil ();
  CORBA::ULong l = 0;
  BindingTable::iterator i = table.begin ();

  for (; l < num; l++, i++) {
    CosNaming::Binding b;
    (*bl)[l].binding_name.length (1);
    (*bl)[l].binding_name[0] = (*i).first;
    (*bl)[l].binding_type = (*i).second.btype;
  }
  if (how_many < table.size ()) {
    CORBA::ULong rn = table.size () - how_many;
    CosNaming::BindingList rest (rn);
    rest.length (rn);
    for (l = 0; l < rn; l++, i++) {
      rest[l].binding_name.length (1);
      rest[l].binding_name[0] = (*i).first;
      rest[l].binding_type = (*i).second.btype;
    }
    BindingIterator_impl * bii = new BindingIterator_impl (useorb, rest);
    bi = bii->_this ();
    bii->_remove_ref ();
  }
}

/*
 * Global Save and Restore
 */

void
NamingContext_impl::SaveNamingService (const char * dbfile)
{
  ofstream out (dbfile);

  if (!out.good()) {
    cerr << "warning: trouble saving NamingService to " << dbfile << endl;
    return;
  }

  cerr << "saving to database " << dbfile << " ... " << flush;

  out << "MICO Naming Service DB - v2003_11_08" << endl;
  out << svmap.size() << endl;

  for (ServantMap::iterator it = svmap.begin(); it != svmap.end(); it++) {
    PortableServer::ObjectId_var oid = usepoa->servant_to_id (*it);
    CORBA::String_var objname = mico_url_encode (oid->get_buffer(),
						 oid->length());
    out << objname.in() << endl;
  }

  root->save (out);
  for (ServantMap::iterator it = svmap.begin(); it != svmap.end(); it++) {
    (*it)->save (out);
  }

  cerr << "done." << endl;
}

void
NamingContext_impl::RestoreNamingService (const char * dbfile)
{
  ifstream in (dbfile);

  if (!in.good()) {
    cerr << "warning: trouble restoring NamingService from " << dbfile << endl;
    return;
  }

  char buf[256];
  in.getline (buf, 256);

  if (strcmp (buf, "MICO Naming Service DB - v2003_11_08") == 0) {
      cerr << "reading in database " << dbfile << " ... " << flush;

      int nc_count;
      in >> nc_count;
      for ( int i = 0; i < nc_count; i++) {
        if (!in.good() || in.eof()) {
          cerr << "oops: " << dbfile << " appears to be corrupted" << endl;
          break;
        }

        string objid;
        in >> objid;
        in.getline (buf, 256);
        if (in.eof() || objid.length() == 0) {
          break;
        }

        CORBA::ULong length;
        CORBA::Octet * data = mico_url_decode (objid.c_str(), length);
        PortableServer::ObjectId oid (length, length, data, TRUE);
        NamingContext_impl * nci = new NamingContext_impl;
        usepoa->activate_object_with_id (oid, nci);
        svmap.insert (nci);
      }

      root->restore (in);
      for (ServantMap::iterator it = svmap.begin(); it != svmap.end(); it++) {
        (*it)->restore (in);
      }
  } else if (strcmp (buf, "MICO Naming Service DB") == 0) {
      int count;
      in >> count;            // skip the root binding table
      in.getline(buf, 256);   // clear the end-of-line
      for ( int i =0; i < count; i++) {
          in.getline(buf, 256);
          in.getline(buf, 256);
      }
      while (in.good() && !in.eof()) {
        string objid;
        in >> objid;
        in.getline (buf, 256);
        if (in.eof() || objid.length() == 0) {
          break;
        }
        CORBA::ULong length;
        CORBA::Octet * data = mico_url_decode (objid.c_str(), length);
        PortableServer::ObjectId oid (length, length, data, TRUE);
        NamingContext_impl * nci = new NamingContext_impl;
        usepoa->activate_object_with_id (oid, nci);
        svmap.insert (nci);
        in >> count;            // skip the binding table
        in.getline(buf, 256);   // clear the end-of-line
        for ( int i =0; i < count; i++) {
            in.getline(buf, 256);
            in.getline(buf, 256);
        }
      }
      in.close();
      ifstream in (dbfile);
      in.getline(buf, 256);  // skip "MICO Naming Service DB"
      root->restore(in);
      for (ServantMap::iterator it = svmap.begin(); it != svmap.end(); it++) {
        in.getline(buf,256); // skip object id
        (*it)->restore(in);
      }
  } else {
      cerr << "oops: " << dbfile << " does not look like new NamingService db" << endl;
      return;
  }
  cerr << "done." << endl;
}

/*
 * BindingIterator
 */

BindingIterator_impl::BindingIterator_impl (CORBA::ORB_ptr orb,
					    const CosNaming::BindingList& bl) 
  : idx (0), blist (bl), useorb (orb)
{
}

CORBA::Boolean BindingIterator_impl::next_one (CosNaming::Binding_out b)
{
  MICOMT::AutoLock l(blist_lock_);
  if (idx < blist.length ()) {
    b = new CosNaming::Binding (blist[idx++]);
    return TRUE;
  }
  else {
    b = new CosNaming::Binding;
    b->binding_type = CosNaming::nobject;
    return FALSE;
  }
}

CORBA::Boolean 
BindingIterator_impl::next_n (CORBA::ULong how_many, 
			      CosNaming::BindingList_out bl)
{
  MICOMT::AutoLock l(blist_lock_);
  CORBA::ULong rlen = blist.length () - idx;
  if (rlen > 0) {
    CORBA::ULong num = how_many < rlen ? how_many : rlen;
    bl = new CosNaming::BindingList (num);
    bl->length (num);
    for (CORBA::ULong i = 0; i < num; i++, idx++)
      (*bl)[i] = blist[idx];
    return TRUE;
  }
  else {
    bl = new CosNaming::BindingList (0);
    bl->length (0);
    return FALSE;
  }
}

void BindingIterator_impl::destroy ()
{
  CORBA::Object_var obj = useorb->resolve_initial_references ("POACurrent");
  PortableServer::Current_var pc = PortableServer::Current::_narrow (obj);
  PortableServer::POA_var poa = pc->get_POA ();
  PortableServer::ObjectId_var oid = pc->get_object_id ();
  poa->deactivate_object (oid.in());
}

/*
 * ----------------------------------------------------------------------
 * NamingContextExt
 * ----------------------------------------------------------------------
 */

NamingContextExt_impl::NamingContextExt_impl (CORBA::ORB_ptr orb,
					      PortableServer::POA_ptr poa,
                                              LoadBalanceApproach eLoadBalanceApproach)
  : NamingContext_impl (orb, poa, eLoadBalanceApproach)
{
}

/*
 * Helpers
 *
 * These are static and also used by NamingContext's save and restore
 */

string
NamingContextExt_impl::StringifyEscape (const char * str)
{
  string res;
  while (*str) {
    switch (*str) {
    case '/':
    case '.':
    case '\\':
      res += '\\';
    default:
      res += *str;
    }
    str++;
  }
  return res;
}

string
NamingContextExt_impl::StringifyComponent (const CosNaming::NameComponent &nc)
{
  string res = StringifyEscape (nc.id.in());

  if (!*nc.kind.in() && *nc.id.in()) {
    return res;
  }

  res += '.';
  res += StringifyEscape (nc.kind.in());
  return res;
}

bool
NamingContextExt_impl::ScanComponent (const char *& ptr,
				      CosNaming::NameComponent & nc)
{
  bool inid=true, escaped=false;
  const char * bp = ptr;
  CORBA::ULong count=0;

  /*
   * Scan id.kind. The loop loops over every character, and inid denotes
   * whether we're scanning the id (true) or the kind (false) field. bp
   * points to the beginning of the field, and count counts the effective
   * number of characters (without escaping).
   * We stop at any character we don't recognize and return true with ptr
   * pointing to that char. If anything unexpected happens, we return false,
   * and contents of ptr and nc are undefined.
   */

  while (42) {
    /*
     * Ignore escaped character
     */

    if (escaped) {
      if (!*ptr) {
	return false;
      }
      escaped = false;
      count++;
      ptr++;
      continue;
    }

    /*
     * Handle escape
     */

    if (*ptr == '\\') {
      escaped = true;
      ptr++;
      continue;
    }

    /*
     * An unescaped dot is illegal within the kind field
     */

    if (*ptr == '.' && !inid) {
      return false;
    }

    /*
     * When a boundary character is reached, assign the field
     */

    if (!*ptr || *ptr == '.' || *ptr == '/') {
      char * data = CORBA::string_alloc (count);
      for (CORBA::ULong i=0; bp < ptr; i++, bp++) {
	if (*bp == '\\') {
	  bp++;
	}
	data[i] = *bp;
      }
      data[count] = 0;

      /*
       * When inid, prepare to scan kind field, else we are done.
       */

      if (inid) {
	nc.id = data;
	inid = false;
	count = 0;
	if (*ptr == '.') {
	  ptr++;
	  bp++;
	  continue;
	}
	else if (!*ptr || *ptr == '/') {
	  nc.kind = CORBA::string_dup ("");
	  break;
	}
      }
      else {
	nc.kind = data;
	break;
      }
    }

    /*
     * Just an ordinary character
     */

    count++;
    ptr++;
  }

  /*
   * Done.
   */

  return true;
}

/*
 * Exported interface
 */

char *
NamingContextExt_impl::to_string (const CosNaming::Name & name)
{
  string res;

  for (CORBA::ULong i=0; i<name.length(); i++) {
    if (i) {
      res += '/';
    }
    res += StringifyComponent (name[i]);
  }

  return CORBA::string_dup (res.c_str());
}

CosNaming::Name *
NamingContextExt_impl::to_name (const char * sn)
{
  CosNaming::Name * res = new CosNaming::Name;
  CORBA::ULong components = 0;

  while (*sn) {
    res->length (components+1);
    if (!ScanComponent (sn, (*res)[components])) {
      mico_throw (CosNaming::NamingContext::InvalidName());
    }
    if (*sn && *sn != '/') {
      mico_throw (CosNaming::NamingContext::InvalidName());
    }
    if (*sn == '/') {
      sn++;
    }
    components++;
  }

  return res;
}

char *
NamingContextExt_impl::to_url (const char * addr,
			       const char * sn)
{
  CORBA::String_var uesn = mico_url_encode ((CORBA::Octet *) sn, strlen(sn));
  string res = "corbaname:";
  res += addr;
  res += '#';
  res += uesn.in();
  return CORBA::string_dup (res.c_str());
}

CORBA::Object_ptr
NamingContextExt_impl::resolve_str (const char * sn)
{
  CosNaming::Name_var n = to_name (sn);
  return resolve (n.in());
}

string
NamingContext_impl::FindNameComponent
(const CosNaming::NameComponent *pNameComponent,
LoadBalanceData *pstLoadBalanceData)
{
  map<string, CosNaming::NameComponent, less<string> >::iterator itIterator;
  CosNaming::NameComponent stNameComponent;
  string sNumber;
  int iTotalNameComponents;
  int iFound;
  int iCount;


  // Initialize the variables
  iTotalNameComponents = pstLoadBalanceData->iCount;
  iFound = FALSE;

  // Check each element in the map
  for (iCount = 0; iCount <= (iTotalNameComponents - 1); ++iCount)
  {
    // Look for the element of number iCount
    sNumber = iCount;
    itIterator = (pstLoadBalanceData->tbSubTable).find (sNumber);

    if (itIterator != (pstLoadBalanceData->tbSubTable).end ())
    {
      stNameComponent = (*itIterator).second;
      
      // See if the name component in that element is what we are
      // looking for
      if ((strcmp (stNameComponent.id, (*pNameComponent).id.in() ) == 0) &&
          (strcmp (stNameComponent.kind, (*pNameComponent).kind.in()) == 0))
      {
        iFound = TRUE;
        break;
      }
    }
  }

  // Return a NULL value if the name component cannot be found
  if (iFound == FALSE)
    sNumber = NULL_NUMBER;

  return (sNumber);
}

void 
NamingContext_impl::AddLoadBalanceEntry 
(const CosNaming::NameComponent *pNameComponent)
{
  MICOMT::AutoLock lock(tbLoadBalanceTable);
  map<string, LoadBalanceData, less<string> >::iterator itIterator;
  string sKind;
  LoadBalanceData stLoadBalanceData;
  LoadBalanceData *pstLoadBalanceData;
  string sNumber;
  string sNumberNULL;


  sKind = pNameComponent->kind;

  // Look for an entry with the target kind
  itIterator = (this->tbLoadBalanceTable).find (sKind);

  // The target kind is not found, so add the kind to the table  
  if (itIterator == (this->tbLoadBalanceTable).end ())
  {
    stLoadBalanceData.iCount = 0;
    stLoadBalanceData.iCurrentNumber = 0;
    (this->tbLoadBalanceTable)[sKind] = stLoadBalanceData;
  }

  // Add the name component to the sub-table of the corresponding entry
  itIterator = (this->tbLoadBalanceTable).find (sKind);

  pstLoadBalanceData = &((*itIterator).second);

  // Determine whether to overwrite an existing entry or to add a
  // new entry
  sNumber = FindNameComponent (pNameComponent, pstLoadBalanceData);
  
  sNumberNULL = NULL_NUMBER;

  if (sNumber == sNumberNULL)
  {
    sNumber = (pstLoadBalanceData->iCount);
    ++(pstLoadBalanceData->iCount);
  }

  // Proceed to overwrite or add an entry
  (pstLoadBalanceData->tbSubTable)[sNumber] = (*pNameComponent);

  if (iDebug == TRUE)
  {
    cout << "load balance entry added" << endl;
    cout << "  - name component id   : " 
         << (const char *) (pNameComponent->id) << endl;
    cout << "  - name component kind : "
         << (const char *) (pNameComponent->kind) << endl;
  }
}

void 
NamingContext_impl::RemoveLoadBalanceEntry 
(const CosNaming::NameComponent *pNameComponent)
{
  MICOMT::AutoLock lock(tbLoadBalanceTable);
  map<string, LoadBalanceData, less<string> >::iterator itIterator;
  map<string, CosNaming::NameComponent, less<string> >::iterator itIteratorSub;
  string sKind;
  LoadBalanceData stLoadBalanceData;
  LoadBalanceData *pstLoadBalanceData;
  string sNumber;
  string sNumberNULL;
  string sMaximumNumber;
  CosNaming::NameComponent stNameComponent;


  sKind = pNameComponent->kind;

  // Look for an entry with the target kind
  itIterator = (this->tbLoadBalanceTable).find (sKind);
 
  // Proceed if an entry with the target kind if found
  if (itIterator != (this->tbLoadBalanceTable).end ())
  {
    // Check if an entry with the target name component exists
    pstLoadBalanceData = &((*itIterator).second);

    sNumber = FindNameComponent (pNameComponent, pstLoadBalanceData);

    // Proceed if such an entry exists
    sNumberNULL = NULL_NUMBER;
    if (sNumber != sNumberNULL)
    {
      sMaximumNumber = (pstLoadBalanceData->iCount - 1);

      if (sNumber == sMaximumNumber)
      {      
        // Remove the entry, since it is the last entry in the list
        itIteratorSub = (pstLoadBalanceData->tbSubTable).find (sNumber);    
        (pstLoadBalanceData->tbSubTable).erase (itIteratorSub);
      }
      else
      {
        // Replace the entry with the last entry in the list, just to
        // avoid having to move up all the entries that follow
        sMaximumNumber = (pstLoadBalanceData->iCount - 1);

        itIteratorSub = (pstLoadBalanceData->tbSubTable).find (sMaximumNumber);
        stNameComponent = (*itIteratorSub).second;
        (pstLoadBalanceData->tbSubTable)[sNumber] = stNameComponent;

        (pstLoadBalanceData->tbSubTable).erase (itIteratorSub);
      }

      // An entry has been removed from the list, so reduce the total
      // number of entries
      --(pstLoadBalanceData->iCount);
      
      if (iDebug == TRUE)
      {
        cout << "Name component removed :" << endl;
        cout << "  id   - " << (const char *) pNameComponent->id << endl;
        cout << "  kind - " << (const char *) pNameComponent->kind << endl;
      }
    }
  }      
}

CosNaming::NameComponent
NamingContext_impl::RoundRobinResolve 
(const CosNaming::NameComponent *pNameComponent)
{
  MICOMT::AutoLock lock(tbLoadBalanceTable);
  map<string, LoadBalanceData, less<string> >::iterator itIterator;
  map<string, CosNaming::NameComponent, less<string> >::iterator itIteratorSub;
  string sKind;
  LoadBalanceData stLoadBalanceData;
  LoadBalanceData *pstLoadBalanceData;
  string sNumber;
  int iTargetNumber;
  CosNaming::NameComponent stTargetNameComponent;


  sKind = pNameComponent->kind;

  // Look for an entry with the target kind
  itIterator = (this->tbLoadBalanceTable).find (sKind);

  // Proceed to look for the target name component
  if (itIterator != (this->tbLoadBalanceTable).end ())
  {
    pstLoadBalanceData = &((*itIterator).second);

    // Determine the target name component to return
    if (pstLoadBalanceData->iCount > 0)
    {
      if (pstLoadBalanceData->iCurrentNumber <=
          (pstLoadBalanceData->iCount - 1))
        ;
      else
        pstLoadBalanceData->iCurrentNumber = 0;

      iTargetNumber = pstLoadBalanceData->iCurrentNumber;
      ++(pstLoadBalanceData->iCurrentNumber);

      // Get the target name component
      sNumber = iTargetNumber;
      itIteratorSub = (pstLoadBalanceData->tbSubTable).find (sNumber);    
      stTargetNameComponent = (*itIteratorSub).second;
    }
    else
    {
      if (iDebug == TRUE)
        cout << "RoundRobinResolve () : iCount is 0" << endl;
      mico_throw (CosNaming::NamingContext::NotFound ());
    }
  }
  else
  {
    if (iDebug == TRUE)
      cout << "RoundRobinResolve () : Kind not found in tbLoadBalanceTable"
           << endl;
    mico_throw (CosNaming::NamingContext::NotFound ());
  }

  if (iDebug == TRUE)
  {
    cout << "round robin resolve done" << endl;
    cout << "  . name component id   : " 
         << stTargetNameComponent.id.in() << endl;
    cout << "  . name component kind : "
         << stTargetNameComponent.kind.in() << endl;
  }

  return (stTargetNameComponent);
}

CosNaming::NameComponent
NamingContext_impl::RandomResolve 
(const CosNaming::NameComponent *pNameComponent)
{
  MICOMT::AutoLock lock(tbLoadBalanceTable);
  map<string, LoadBalanceData, less<string> >::iterator itIterator;
  map<string, CosNaming::NameComponent, less<string> >::iterator itIteratorSub;
  string sKind;
  LoadBalanceData stLoadBalanceData;
  LoadBalanceData *pstLoadBalanceData;
  string sNumber;
  int iTargetNumber;
  CosNaming::NameComponent stTargetNameComponent;


  sKind = pNameComponent->kind;

  // Look for an entry with the target kind
  itIterator = (this->tbLoadBalanceTable).find (sKind);

  // Proceed to look for the target name component
  if (itIterator != (this->tbLoadBalanceTable).end ())
  {
    pstLoadBalanceData = &((*itIterator).second);

    // Determine the target name component to return
    if (pstLoadBalanceData->iCount > 0)
    {
      // Generate a random number 
      // from 0 to (pstLoadBalanceData->iCount - 1)
      srand (rand ());
      iTargetNumber = rand () % (pstLoadBalanceData->iCount);

      // Get the target name component
      sNumber = iTargetNumber;
      itIteratorSub = (pstLoadBalanceData->tbSubTable).find (sNumber);    
      stTargetNameComponent = (*itIteratorSub).second;
    }
    else
    {
      if (iDebug == TRUE)
        cout << "RandomResolve () : iCount is 0" << endl;
      mico_throw (CosNaming::NamingContext::NotFound ());
    }
  }
  else
  {
    if (iDebug == TRUE)
      cout << "RandomResolve () : Kind not found in tbLoadBalanceTable"
           << endl;
    mico_throw (CosNaming::NamingContext::NotFound ());
  }
  
  if (iDebug == TRUE)
  {
    cout << "random resolve done" << endl;
    cout << "  . name component id   : " 
         << stTargetNameComponent.id.in() << endl;
    cout << "  . name component kind : "
         << stTargetNameComponent.kind.in()   << endl;
  }

  return (stTargetNameComponent);
}

