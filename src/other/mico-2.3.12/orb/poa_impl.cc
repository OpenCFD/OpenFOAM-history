/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (C) 1998 Frank Pilhofer
 *  Copyright (c) 1999-2005 by The Mico Team
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  For more information, visit the MICO Home Page at
 *  http://www.mico.org/
 */

/*
 * Portable Server Module
 */

#ifdef FAST_PCH
#include "orb_pch.h"
#endif // FAST_PCH
#ifdef __COMO__
#pragma hdrstop
#endif // __COMO__

#ifndef FAST_PCH

#define MICO_CONF_INTERCEPT
#include <CORBA.h>
#include <mico/impl.h>
#include <mico/throw.h>
#include <mico/util.h>
#include <mico/os-net.h>
#include <mico/poa_impl.h>
#include <mico/template_impl.h>
#include <mico/pi_impl.h>

#endif // FAST_PCH


using namespace std;

/*
 * Static data
 */

namespace MICOPOA {
  POAOptions poaopts;
}

MICOPOA::POA_impl::POAMap MICOPOA::POA_impl::AllPOAs;
MICOPOA::UniqueIdGenerator MICOPOA::POA_impl::poauid;
MICOPOA::UniqueIdGenerator MICOPOA::POA_impl::idfactory ("_");
MICOPOA::POACurrent_impl * MICOPOA::POA_impl::current = NULL;

string MICOPOA::POA_impl::oaprefix;
string MICOPOA::POA_impl::impl_name;
CORBA::IOR MICOPOA::POA_impl::poamed_ior;
CORBA::POAMediator_var MICOPOA::POA_impl::poamed;
CORBA::Boolean MICOPOA::POA_impl::ever_been_active;
MICOMT::Mutex MICOPOA::POA_impl::S_global_invoke_lock;
MICOMT::Mutex MICOPOA::POA_impl::S_servant_manager_lock;
/*
 * ----------------------------------------------------------------------
 *
 * Initialization via Init Interceptor
 *
 * ----------------------------------------------------------------------
 */

static class POAInit : public Interceptor::InitInterceptor {
public:
  POAInit ()
    : Interceptor::InitInterceptor(0)
  {
  }

  ~POAInit ()
  {
    deactivate ();
  }

  Interceptor::Status initialize (CORBA::ORB_ptr orb, const char *orbid,
				  int &argc, char *argv[])
  {
    if (!MICOPOA::poaopts.parse (orb, argc, argv))
      return Interceptor::INVOKE_ABORT;
    return Interceptor::INVOKE_CONTINUE;
  }
} InitPOA;

void
MICOPOA::_init ()
{
  (void) InitPOA;
}

/*
 * Options parser
 */

CORBA::Boolean
MICOPOA::POAOptions::parse (CORBA::ORB_ptr orb, int &argc, char *argv[])
{
  MICOGetOpt::OptMap opts;
  opts["-POARemoteIOR"]  = "arg-expected";
  opts["-POAImplName"]   = "arg-expected";
  opts["-POARemoteAddr"] = "arg-expected";

  MICOGetOpt opt_parser (opts);
  CORBA::Boolean r = opt_parser.parse (orb->rcfile(), TRUE);
  if (!r) return FALSE;
  r = opt_parser.parse (argc, argv, TRUE);
  if (!r) return FALSE;
  const MICOGetOpt::OptVec &o = opt_parser.opts();

  for (MICOGetOpt::OptVec::const_iterator i = o.begin(); i != o.end(); ++i) {
    string arg = (*i).first;
    string val = (*i).second;
    options[arg] = val;
  }
  return TRUE;
}

const char *
MICOPOA::POAOptions::operator[] (const char * opt)
{
  map<string,string,less<string> >::iterator it = options.find (opt);
  if (it == options.end()) {
    return NULL;
  }
  return (*it).second.c_str();
}

/*
 * ----------------------------------------------------------------------
 *
 * POAManager interface
 *
 * ----------------------------------------------------------------------
 */

MICOPOA::POAManager_impl::POAManager_impl ()
  : managed_lock(FALSE, MICOMT::Mutex::Recursive)
{
  _state = HOLDING;
}

MICOPOA::POAManager_impl::~POAManager_impl ()
{
}

PortableServer::POAManager::State
MICOPOA::POAManager_impl::get_state ()
{
  return _state;
}

void
MICOPOA::POAManager_impl::add_managed_poa (PortableServer::POA_ptr poa)
{
  MICOMT::AutoLock l(managed_lock);
  managed.push_back (poa);
}

void
MICOPOA::POAManager_impl::del_managed_poa (PortableServer::POA_ptr poa)
{
  MICOMT::AutoLock l(managed_lock);
  vector<PortableServer::POA_ptr>::iterator it = managed.begin ();

  while (it != managed.end()) {
    if (*it == poa) {
      managed.erase (it);
      return;
    }
    it++;
  }
}

void
MICOPOA::POAManager_impl::change_state (State s,
					CORBA::Boolean etherealize_objects,
					CORBA::Boolean wait_for_completion)
{
  if (_state == INACTIVE) {
    if (s != INACTIVE)
      mico_throw (PortableServer::POAManager::AdapterInactive());
    return; // Manager is already in inactive state
  }

  if (_state == s) {
    return;
  }
  
  _state = s;

  managed_lock.lock();

  vector<PortableServer::POA_ptr>::iterator it = managed.begin();

  while (it != managed.end()) {
    (*it)->poa_manager_callback (_state,
				 etherealize_objects,
				 wait_for_completion);
    it++;
  }

  managed_lock.unlock();
}

void
MICOPOA::POAManager_impl::activate ()
{
  change_state (ACTIVE);
}

void
MICOPOA::POAManager_impl::hold_requests (CORBA::Boolean wait_for_completion)
{
  change_state (HOLDING, FALSE, wait_for_completion);
}

void
MICOPOA::POAManager_impl::discard_requests (CORBA::Boolean wait_for_completion)
{
  change_state (DISCARDING, FALSE, wait_for_completion);
}

void
MICOPOA::POAManager_impl::deactivate (CORBA::Boolean etherealize_objects,
				      CORBA::Boolean wait_for_completion)
{
  change_state (INACTIVE,
		etherealize_objects,
		wait_for_completion);
}

/*
 * ----------------------------------------------------------------------
 *
 * Current interface
 *
 * ----------------------------------------------------------------------
 */

MICOPOA::POACurrent_impl::CurrentState::CurrentState ()
{
  // This method exists to make HP aCC happy
  assert (0);
}

MICOPOA::POACurrent_impl::CurrentState::CurrentState (PortableServer::POA_ptr _poa, POAObjectReference * _por, PortableServer::Servant _serv)
{
  poa = _poa;
  por = _por;
  serv = _serv;
  serv->_add_ref ();
}

MICOPOA::POACurrent_impl::CurrentState::CurrentState (const CurrentState & o)
{
  poa = o.poa;
  por = o.por;
  serv = o.serv;
  serv->_add_ref ();
}

MICOPOA::POACurrent_impl::CurrentState::~CurrentState ()
{
  serv->_remove_ref ();
}

#ifdef HAVE_THREADS
static void __current_cleanup(void *css)
{
  MICOPOA::POACurrent_impl::CurrentStateStack *current = 
    static_cast<MICOPOA::POACurrent_impl::CurrentStateStack *>(css);

  if (current)
    delete current;
}
#endif

MICOPOA::POACurrent_impl::POACurrent_impl (CORBA::ORB_ptr _orb)
{
#ifndef HAVE_THREADS
  state_stack_ = NULL;
#else // HAVE_THREADS
  // we get exactly one POACurrent object
  //  therefor no care is nesesary for creating the key
  MICOMT::Thread::create_key(current_key_, &__current_cleanup);
#endif // HAVE_THREADS
  assert (CORBA::is_nil (PortableServer::_the_poa_current));
  PortableServer::_the_poa_current = this;
  orb = _orb;
  orb->set_initial_reference ("POACurrent", this);
}

MICOPOA::POACurrent_impl::~POACurrent_impl ()
{
  PortableServer::_the_poa_current = NULL;
#ifndef HAVE_THREADS
  delete state_stack_;
#else // HAVE_THREADS
  MICOMT::Thread::delete_key(current_key_);
#endif // HAVE_THREADS
  //FIXME: shouldn't we remove the initial_reference ??
}

PortableServer::POA_ptr
MICOPOA::POACurrent_impl::get_POA ()
{
  if (!this->iscurrent()) {
    mico_throw(PortableServer::Current::NoContext());
  }
  return PortableServer::POA::_duplicate (this->get_current()->back().poa);
}

PortableServer::ObjectId *
MICOPOA::POACurrent_impl::get_object_id ()
{
  if (!this->iscurrent()) {
    mico_throw(PortableServer::Current::NoContext());
  }
  return this->get_current()->back().por->id();
}

CORBA::Object_ptr
MICOPOA::POACurrent_impl::get_reference ()
{
  if (!this->iscurrent()) {
    mico_throw(PortableServer::Current::NoContext());
  }
  return this->get_current()->back().por->ref();
}

PortableServer::Servant
MICOPOA::POACurrent_impl::get_servant ()
{
  if (!this->iscurrent()) {
    mico_throw(PortableServer::Current::NoContext());
  }
  return this->get_current()->back().serv;
}

CORBA::Boolean
MICOPOA::POACurrent_impl::iscurrent ()
{
  CurrentStateStack* curr = this->get_current();
  if (curr != NULL && !curr->empty())
    return TRUE;
  return FALSE;
}

MICOPOA::POAObjectReference *
MICOPOA::POACurrent_impl::get_por ()
{
  MICOPOA::POACurrent_impl::CurrentStateStack *current = get_current();
  assert (current);
  return current->back().por;
}

void
MICOPOA::POACurrent_impl::set (PortableServer::POA_ptr _poa,
			       POAObjectReference * _por,
			       PortableServer::Servant _serv)
{
  if (MICO::Logger::IsLogged (MICO::Logger::POA)) {
    MICOMT::AutoDebugLock __lock;
    MICO::Logger::Stream (MICO::Logger::POA)
      << "void MICOPOA::POACurrent_impl::set( "
      << "poa=" << _poa
      << ", POAObjectReference=" << _por
      << ", Servant=" << _serv
      << " )" << endl;
  }
  MICOPOA::POACurrent_impl::CurrentStateStack *current = get_current();

  if (!current) {
    current = new CurrentStateStack;
    this->set_current(current);
  }
  current->push_back (CurrentState (_poa, _por, _serv));
}

void
MICOPOA::POACurrent_impl::unset ()
{
  if (MICO::Logger::IsLogged (MICO::Logger::POA)) {
    MICOMT::AutoDebugLock __lock;
    MICO::Logger::Stream (MICO::Logger::POA)
      << "void MICOPOA::POACurrent_impl::unset()" << endl;
  }
  MICOPOA::POACurrent_impl::CurrentStateStack *current = get_current();
  assert (current);
  assert (!current->empty());
  current->pop_back();
}

//  #endif // HAVE_THREADS
/*
 * ----------------------------------------------------------------------
 *
 * Helper Classes
 *
 * ----------------------------------------------------------------------
 */

/*
 * Object Id
 */

MICOPOA::ObjectId::ObjectId ()
{
  octets = NULL;
  oid = NULL;
  own = true;
}

MICOPOA::ObjectId::ObjectId (const ObjectId & id, bool copy)
{
  oid = NULL;
  idlength = id.idlength;
  if ((own = copy)) {
    octets = CORBA::string_alloc (idlength);
    memcpy (octets, id.octets, idlength);
  }
  else {
    octets = (char *) id.octets;
  }
}

MICOPOA::ObjectId::ObjectId (const PortableServer::ObjectId & id)
{
  own = true;
  oid = NULL;
  idlength = id.length ();
  octets = CORBA::string_alloc (idlength);

  for (CORBA::ULong i=0; i<idlength; i++) {
    octets[i] = (char) id[i];
  }
}

MICOPOA::ObjectId::ObjectId (const char * id, CORBA::ULong len, bool copy)
{
  oid = NULL;
  idlength = len;
  if ((own = copy)) {
    octets = CORBA::string_alloc (idlength);
    memcpy (octets, id, idlength);
  }
  else {
    octets = (char *) id;
  }
}

MICOPOA::ObjectId::~ObjectId ()
{
  if (own) {
    CORBA::string_free (octets);
  }
  delete oid;
}

MICOPOA::ObjectId &
MICOPOA::ObjectId::operator= (const ObjectId & id)
{
  if (own) {
    CORBA::string_free (octets);
  }
  delete oid;
  oid = NULL;
  own = true;
  idlength = id.idlength;
  octets = CORBA::string_alloc (idlength);
  memcpy (octets, id.octets, idlength);
  return *this;
}

CORBA::Boolean
MICOPOA::ObjectId::operator== (const ObjectId & o)
{
  if (idlength != o.idlength) {
    return FALSE;
  }

  return (memcmp (octets, o.octets, idlength) == 0);
}

bool
MICOPOA::ObjectId::operator< (const ObjectId & o) const
{
  if (idlength != o.idlength) {
    return (idlength < o.idlength);
  }

  return (memcmp (octets, o.octets, idlength) < 0);
}

const char *
MICOPOA::ObjectId::get_data (CORBA::ULong & len) const
{
  len = idlength;
  return octets;
}

const PortableServer::ObjectId &
MICOPOA::ObjectId::get_id ()
{
  if (!oid) {
    oid = new PortableServer::ObjectId;
    oid->length (idlength);

    for (CORBA::ULong i=0; i<idlength; i++) {
      (*oid)[i] = (CORBA::Octet) octets[i];
    }
  }

  return *oid;
}

PortableServer::ObjectId *
MICOPOA::ObjectId::id ()
{
  return new PortableServer::ObjectId (get_id());
}

/*
 * Object References
 */

MICOPOA::POAObjectReference::POAObjectReference (POA_impl * _poa,
						 const PortableServer::ObjectId &_i,
						 const char * _repoid,
						 PortableServer::Servant _serv)
  : poa (_poa), repoid (_repoid), oid (_i), servant (_serv)
{
  assert (_poa);
  assert (_repoid);

  poa->_ref();
  poaname = poa->get_oaid();
  obj = CORBA::Object::_nil ();
  iddirty = false;

  if (servant) {
    servant->_add_ref();
  }
}

MICOPOA::POAObjectReference::POAObjectReference (POA_impl * _poa,
						 CORBA::Object_ptr _obj)
  : poa (_poa)
{
  assert (_poa);

  poa->_ref();
  obj = CORBA::Object::_duplicate (_obj);
  iddirty = true;
  servant = NULL;
}

MICOPOA::POAObjectReference::POAObjectReference (const POAObjectReference &o)
  : poa (o.poa), iddirty (o.iddirty), poaname (o.poaname),
    repoid (o.repoid),oid (o.oid),servant (o.servant)
{
  poa->_ref();
  obj = CORBA::Object::_duplicate (o.obj);

  if (servant) {
    servant->_add_ref();
  }
}

MICOPOA::POAObjectReference::~POAObjectReference ()
{
  CORBA::release (obj);
  CORBA::release (poa);
  obj = NULL;
  if (servant) {
    servant->_remove_ref();
  }
}

void
MICOPOA::POAObjectReference::make_ref ()
{
  if (!CORBA::is_nil (obj)) {
      /*
       * obj might be a "local" reference. We might need one pointing
       * to the Mediator.
       * This happens when the IOR contains a single GIOPSimpleProf, which
       * has the ID 10000, which is unfortunately hardcoded. So verify
       * with iop.cc.
       */

    CORBA::IORProfile* prof = NULL;
    if (obj->_ior() && (prof = obj->_ior()->profile()) &&
	prof->id() == 10000) {
      const CORBA::Octet * key;
      CORBA::Long length;
      key = prof->objectkey (length);
    
      CORBA::IOR * ior = new CORBA::IOR (*poa->ior_template());
      ior->objectkey ((CORBA::Octet *) key, length);
      // obj->_repoid() is empty ...
      ior->objid (obj->_ior()->objid());

      CORBA::release (obj);
      obj = new CORBA::Object (ior);
      assert (!CORBA::is_nil (obj));
      obj->_setup_domains (CORBA::Object::_nil());
    }

    return;
  }

  if (iddirty) {
    // We manage a nil reference
    return;
  }

  /*
   * key = <poa-name> / <id>
   *
   * We escape any slashes in <id> (slashes in poa-name are already
   * quoted). If poa-name == id, then we collapse both.
   */

  CORBA::ULong idlength, length = poaname.length();
  CORBA::Long i, j = poaname.length();
  CORBA::Boolean samename = FALSE;
  const char * iddata = oid.get_data (idlength);
  
  if (idlength == length) {
    for (i=0; i < (CORBA::Long) idlength; i++) {
      if (iddata[i] != poaname[i]) {
	break;
      }
    }
    if (i == (CORBA::Long) idlength) {
      samename = TRUE;
    }
  }

  if (!samename) {
    for (i=0; i < (CORBA::Long) idlength; i++) {
      if (iddata[i] == '/' || iddata[i] == '\\') {
	length++;
      }
      length++;
    }
    length++;
  }

  CORBA::Octet * key = (CORBA::Octet *) CORBA::string_alloc (length);
  memcpy (key, poaname.c_str(), j);

  if (!samename) {
    key[j++] = '/';
    for (i=0; i < (CORBA::Long) idlength; i++, j++) {
      if (iddata[i] == '/' || iddata[i] == '\\') {
	key[j++] = '\\';
      }
      key[j] = (CORBA::Octet) iddata[i];
    }
  }

  assert (j == (CORBA::Long) length);

  /*
   * Generate a Mobile Object Key if necessary.
   */
  CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
  if (!orb->plugged()) {
    CORBA::ULong align;
    MICO::CDREncoder ec;
    CORBA::ULong tlen;
    const CORBA::Octet *tid = orb->terminal_id(tlen);
    /*
     * Do encaps by hand since we don't want length in here
     */
    align = ec.buffer()->walign_base();
    ec.put_octet(ec.byteorder() == CORBA::LittleEndian ? TRUE : FALSE);

    ec.put_chars_raw ("MIOR", 4);
    ec.struct_begin();
    {
      ec.struct_begin();
      {
	ec.put_octet(0x01);
	ec.put_octet(0x00);
      }
      ec.struct_end();
      ec.put_octet(0x00);
      ec.seq_begin(tlen);
      {
	ec.put_octets(tid, tlen);
      }
      ec.seq_end();
      ec.seq_begin(length);
      {
	ec.put_octets((CORBA::Octet*)key, length);
      }
      ec.seq_end();
    }
    ec.struct_end();
    ec.buffer()->walign_base(align);

    length = ec.buffer()->length();
    CORBA::string_free((char *) key);
    key = (CORBA::Octet *) CORBA::string_alloc (length);
    memcpy (key, ec.buffer()->data(), length);
  }

  CORBA::IOR * ior = new CORBA::IOR (*poa->ior_template());
  ior->objectkey (key, length);
  ior->objid (repoid.c_str());
  CORBA::string_free ((char *) key);

  obj = new CORBA::Object (ior);
  assert (!CORBA::is_nil (obj));
  obj->_setup_domains (CORBA::Object::_nil());

  if (servant) {
    CORBA::Object_ptr stub = servant->_make_stub (poa, obj);
    if (!CORBA::is_nil (stub)) {
      CORBA::release (obj);
      obj = stub;
    }
  }
}

bool
MICOPOA::POAObjectReference::decompose_ref ()
{
  MICOMT::AutoLock l(_ref_lock);

  /*
   * The object key that we are about to decompose might be complete
   * garbage in case we didn't generate it ourselves.
   */

  if (!iddirty) {
    // nothing to do
    return true;
  }

  CORBA::IORProfile* prof = NULL;
  if (CORBA::is_nil (obj) || !obj->_ior() || !(prof = obj->_ior()->profile())) {
    return false;
  }

  CORBA::Long i, j, length, idlength;
  const CORBA::Octet * key = prof->objectkey (length);

  if (!key) {
    return false;
  }

  /*
   * Strip away the beginning if it was a Mobile Object Key
   */
  if (CORBA::ORB::is_mobile_key (key)) {
    CORBA::Octet bo;
    CORBA::Buffer *buf = new CORBA::Buffer (const_cast<CORBA::Octet*> (key));
#ifdef _WIN32
    MICO::CDRDecoder dc(buf, TRUE, CORBA::DefaultEndian,
			0, TRUE, 0, TRUE);
#else
    MICO::CDRDecoder dc(buf);
#endif
#define check(exp) if (!(exp)) return false
    char crap[4];
    check(dc.get_octet(bo));
    CORBA::ByteOrder sbo = dc.byteorder();
    dc.byteorder(bo == 0 ? CORBA::BigEndian : CORBA::LittleEndian);

    check(dc.get_chars(crap, 4));
    check(dc.struct_begin());
    {
      check(dc.get_octets(crap, 2));
    }
    check(dc.struct_end());
    CORBA::Octet oc;
    check(dc.get_octet(oc));

    CORBA::ULong l;
    check(dc.seq_begin(l));
    {
      CORBA::Octet oc;
      for (CORBA::ULong i = 0; i < l; ++i)
	check(dc.get_octet(oc));
    }
    check(dc.seq_end());
    check(dc.get_ulong(l));
#undef check
    dc.byteorder(sbo);
    key = dc.buffer()->data();
    length = l;
  }

  /*
   * The poaname is everything up to the last unescaped slash
   */

  for (i=length-1; i>0; i--) {
    if (key[i] == '/') {
      for (j=0; j<i; j++) {
	if (key[i-j-1] != '\\') {
	  break;
	}
      }
      if ((j%2) == 0) {
	break;
      }
    }
  }

  if (key[i] != '/') {  // in case of collapsed poaname/id
    i = length;
  }

  for (j=0; j<i; j++) {
    if (!key[j]) {
      return false;
    }
    poaname += key[j];
  }

  /*
   * If j == length, the id is the same as the POA name
   */

  if (j == length) {
    oid = ObjectId (poaname.c_str(), (CORBA::ULong) length, false);
  }
  else {
    j++;

    for (i=j, idlength=length-j; i<length; i++) {
      if (key[i] == '\\') {
	idlength--, i++;
      }
    }

    char * iddata = CORBA::string_alloc (idlength);

    for (i=0; j<length; i++, j++) {
      if (key[j] == '\\') {
	j++;
      }
      iddata[i] = key[j];
    }

    oid = ObjectId (iddata, (CORBA::ULong) idlength, false);
    CORBA::string_free (iddata);
  }
  
  iddirty = FALSE;
  repoid = obj->_repoid ();
  return true;
}

bool
MICOPOA::POAObjectReference::is_legal ()
{
  if (!iddirty) {
    return true;
  }
  if (!CORBA::is_nil (obj)) {
    return decompose_ref ();
  }
  return false;
}

const MICOPOA::ObjectId &
MICOPOA::POAObjectReference::get_oid ()
{
  if (iddirty) {
    bool r = decompose_ref ();
    assert (r);
  }
  return oid;
}

CORBA::Object_ptr
MICOPOA::POAObjectReference::get_ref ()
{
  MICOMT::AutoLock l(_ref_lock);
  make_ref ();
  return obj;
}

void
MICOPOA::POAObjectReference::set_ref (CORBA::Object_ptr o)
{
    MICOMT::AutoLock l(_ref_lock);
    CORBA::release (obj);
    obj = CORBA::Object::_duplicate (o);
}

const PortableServer::ObjectId &
MICOPOA::POAObjectReference::get_id ()
{
  if (iddirty) {
    bool r = decompose_ref ();
    assert (r);
  }
  return oid.get_id();
}

CORBA::Object_ptr
MICOPOA::POAObjectReference::ref ()
{
  MICOMT::AutoLock l(_ref_lock);
  make_ref();
  return CORBA::Object::_duplicate (obj);
}

PortableServer::ObjectId *
MICOPOA::POAObjectReference::id ()
{
  return new PortableServer::ObjectId (get_id());
}

MICOPOA::POAObjectReference &
MICOPOA::POAObjectReference::operator= (const CORBA::Object_ptr oref)
{
  MICOMT::AutoLock l(_ref_lock);
  // assume same POA
  iddirty = TRUE;
  CORBA::release (obj);
  obj = CORBA::Object::_duplicate (oref);
  return *this;
}

MICOPOA::POAObjectReference &
MICOPOA::POAObjectReference::operator= (const POAObjectReference &o)
{
  MICOMT::AutoLock l(_ref_lock);

  CORBA::release (obj);

  poa = o.poa;
  poaname = o.poaname;
  repoid = o.repoid;
  oid = o.oid;
  iddirty = o.iddirty;
  obj = CORBA::Object::_duplicate (o.obj);

  return *this;
}

const char *
MICOPOA::POAObjectReference::poa_name ()
{
  bool r = decompose_ref ();
  assert (r);
  return poaname.c_str();
}

bool
MICOPOA::POAObjectReference::in_poa (const char * pname)
{
  if (!decompose_ref()) {
    return false;
  }

  const char * pstr = poaname.c_str();
  return !strcmp (pstr, pname);
}

bool
MICOPOA::POAObjectReference::in_descendant_poa (const char * fqn,
						const char * implname)
{
  if (!decompose_ref()) {
    return false;
  }

  const char * pstr = poaname.c_str();

  if (*implname && strncmp (pstr, implname, strlen (implname)) == 0) {
    pstr += strlen (implname);
    if (!*pstr) {
      pstr = poaname.c_str(); // collapsed POA name
    }
    else if (*pstr++ != '/') {
      return false;
    }
  }
  else {
    return false;
  }
  
  return (!strncmp (pstr, fqn, strlen (fqn)) &&
	  (!*fqn || pstr[strlen(fqn)] == '/'));
}

char *
MICOPOA::POAObjectReference::next_descendant_poa (const char * fqn,
						  const char * implname)
{
  bool r = decompose_ref ();
  assert (r);
  assert (in_descendant_poa (fqn, implname));

  const char * pstr = poaname.c_str();

  if (*implname && strncmp (pstr, implname, strlen (implname)) == 0) {
    pstr += strlen (implname);
    if (!*pstr) {
      pstr = poaname.c_str(); // collapsed POA name
    }
    else {
      assert (*pstr == '/');
      pstr++;
    }
  }
  else {
    assert (0);
  }

  size_t i, j;
  char * res;

  if (*fqn) {
    pstr += strlen (fqn);
    assert (*pstr == '/');
    pstr++;
  }

  for (i=0, j=0; pstr[i] && pstr[i] != '/'; i++, j++) {
    if (pstr[i] == '\\') {
      i++;
    }
  }

  res = CORBA::string_alloc (i);
  assert (res);

  for (i=0, j=0; pstr[i] && pstr[i] != '/'; i++, j++) {
    if (pstr[i] == '\\') {
      i++;
    }
    res[j] = pstr[i];
  }

  res[j] = '\0';
  return res;
}

/*
 * Unique Id generator
 */

MICOPOA::UniqueIdGenerator::UniqueIdGenerator ()
{
  uid = NULL;
  prefix = NULL;
  pfxlen = 0;
}

MICOPOA::UniqueIdGenerator::UniqueIdGenerator (const char * pfx)
{
  uid = NULL;
  prefix = CORBA::string_dup (pfx);
  pfxlen = strlen (prefix);
}

MICOPOA::UniqueIdGenerator::~UniqueIdGenerator ()
{
  CORBA::string_free (uid);
  CORBA::string_free (prefix);
}

char *
MICOPOA::UniqueIdGenerator::new_id ()
{
  char * id;

  /*
   * Generate a new unique id
   */

  if (uid == NULL) {
    ulen = 1;
    uid  = CORBA::string_alloc (ulen);
    assert (uid);
    uid[0] = '0';
    uid[1] = 0;
  }
  else {
    int i;
    for (i=0; i<ulen; i++) {
      if (uid[i] != '9')
	break;
      uid[i] = '0';
    }
    if (i == ulen) {
      CORBA::string_free (uid);
      uid = CORBA::string_alloc (++ulen);
      assert (uid);
      for (i=0; i<ulen-1; i++) {
	uid[i] = '0';
      }
      uid[ulen-1] = '1';
      uid[ulen] = 0;
    }
    else {
      uid[i] = uid[i] + 1;
    }
  }
  id = CORBA::string_alloc (ulen + pfxlen);
  assert (id);
  if (prefix) strcpy (id, prefix);
  strcpy (id+pfxlen, uid);

  return id;
}

char *
MICOPOA::UniqueIdGenerator::state ()
{
  char * res;

  if (uid == NULL && prefix == NULL) {
    res = CORBA::string_alloc (0);
    res[0] = 0;
    return res;
  }

  if (uid == NULL) {
    res = CORBA::string_alloc (pfxlen + 1);
    strcpy (res+1, prefix);
    res[0] = ':';
    return res;
  }
    
  res = CORBA::string_alloc (ulen + pfxlen + 1);
  strcpy (res, uid);
  if (prefix) strcpy (res+ulen+1, prefix);
  res[ulen] = ':';
  return res;
}

void
MICOPOA::UniqueIdGenerator::state (const char * st)
{
  CORBA::string_free (uid);
  CORBA::string_free (prefix);

  if (!*st) {
    uid = NULL;
    prefix = NULL;
  }

  if (*st == ':') {
    uid = NULL;
    pfxlen = strlen (st+1);
    prefix = CORBA::string_dup (st+1);
  }

  if (*st && *st != ':') {
    for (ulen=0; st[ulen] && st[ulen] != ':'; ulen++);
    assert (st[ulen] == ':');
    uid = CORBA::string_alloc (ulen);
    for (int i=0; i<ulen; i++) {
      uid[i] = st[i];
    }
    uid[ulen] = 0;
    pfxlen = strlen (st+ulen+1);
    prefix = CORBA::string_dup (st+ulen+1);
  }
}

/*
 * Object Record and Object Map
 */

MICOPOA::ObjectMap::ObjectRecord::ObjectRecord (POAObjectReference * _por,
						PortableServer::Servant _serv)
  : por (_por), serv (_serv)
{
  por = _por;
  serv = _serv;
  active = TRUE;
  invoke_cnt = 0;
  delref = 0;
  serv->_add_ref ();
}

MICOPOA::ObjectMap::ObjectRecord::~ObjectRecord ()
{
  delete por;
  serv->_remove_ref ();
}

MICOPOA::ObjectMap::~ObjectMap ()
{
  clear ();
}

bool
MICOPOA::ObjectMap::empty () const
{
  return objs.empty ();
}

void
MICOPOA::ObjectMap::clear ()
{
  iterator it = objs.begin ();

  while (it != objs.end()) {
    delete (*it).second;
    it++;
  }

  objs.clear ();
  servants.clear ();
}

MICOPOA::ObjectMap::iterator
MICOPOA::ObjectMap::begin ()
{
  return objs.begin ();
}

MICOPOA::ObjectMap::iterator
MICOPOA::ObjectMap::end ()
{
  return objs.end ();
}

MICOPOA::ObjectMap::ObjectRecord *
MICOPOA::ObjectMap::pop ()
{
  assert (!empty());
  iterator oit = objs.begin ();
  ObjectRecord * orec = (*oit).second;
  objs.erase (oit);

  SvMap::iterator sit = servants.find (orec->serv);
  assert (sit != servants.end());

  for (vector<ObjectRecord *>::iterator orit = (*sit).second.begin();
       orit != (*sit).second.end(); orit++) {
    if ((*orit) == orec) {
      (*sit).second.erase (orit);
      break;
    }
  }

  if ((*sit).second.empty()) {
    servants.erase (sit);
  }

  return orec;
}

MICOPOA::ObjectMap::ObjectRecord *
MICOPOA::ObjectMap::add (POAObjectReference * por,
			 PortableServer::Servant serv)
{
  ObjectRecord * orec = new ObjectRecord (por, serv);
  ObjectId oid (por->get_id());
  assert (objs.find (oid) == objs.end());

  objs[oid] = orec;
  servants[serv].push_back (orec);
  return orec;
}

MICOPOA::ObjectMap::ObjectRecord *
MICOPOA::ObjectMap::del (const ObjectId & oid)
{
  iterator it = objs.find (oid);
  assert (it != objs.end());
  ObjectRecord * orec = (*it).second;
  SvMap::iterator sit = servants.find (orec->serv);
  assert (sit != servants.end());

  for (vector<ObjectRecord *>::iterator orit = (*sit).second.begin();
       orit != (*sit).second.end(); orit++) {
    if ((*orit) == orec) {
      (*sit).second.erase (orit);
      break;
    }
  }

  if ((*sit).second.empty()) {
    servants.erase (sit);
  }

  objs.erase (it);
  return orec;
}

MICOPOA::ObjectMap::ObjectRecord *
MICOPOA::ObjectMap::del (const PortableServer::ObjectId & id)
{
  return del (ObjectId (id));
}

bool
MICOPOA::ObjectMap::exists (const ObjectId & oid)
{
  return (objs.find (oid) != objs.end());
}

bool
MICOPOA::ObjectMap::exists (const PortableServer::ObjectId & id)
{
  return exists (ObjectId (id));
}

bool
MICOPOA::ObjectMap::exists (const POAObjectReference & por)
{
  // need to cast constness away
  POAObjectReference * nc = (POAObjectReference *) &por;
  return (objs.find (nc->get_oid()) != objs.end());
}

bool
MICOPOA::ObjectMap::exists (PortableServer::Servant serv)
{
  SvMap::iterator sit = servants.find (serv);
  assert (sit==servants.end() || !(*sit).second.empty());
  return (sit != servants.end());
}

MICOPOA::ObjectMap::ObjectRecord *
MICOPOA::ObjectMap::find (const ObjectId & oid)
{
  iterator it = objs.find (oid);
  if (it == objs.end()) {
    return NULL;
  }
  return (*it).second;
}

MICOPOA::ObjectMap::ObjectRecord *
MICOPOA::ObjectMap::find (const PortableServer::ObjectId & id)
{
  return find (ObjectId (id));
}

MICOPOA::ObjectMap::ObjectRecord *
MICOPOA::ObjectMap::find (const POAObjectReference & por)
{
  // need to cast constness away
  POAObjectReference * nc = (POAObjectReference *) &por;
  return find (nc->get_oid());
}

MICOPOA::ObjectMap::ObjectRecord *
MICOPOA::ObjectMap::find (POA_impl * poa, CORBA::Object_ptr obj)
{
  CORBA::IORProfile* prof = NULL;
  if (CORBA::is_nil (obj) || !obj->_ior() || !(prof = obj->_ior()->profile())) {
    return NULL;
  }

  CORBA::Long length, i;
  const CORBA::Octet * key = prof->objectkey (length);

  if (!key) {
    return NULL;
  }

  /*
   * Object Id is everything after the last unescaped slash. We do some
   * trivial checking if the last slash is unescaped (i.e. if the ObjectId
   * does not contain a slash). If yes, we can simply point to that data
   * to speed up the search; else we use a POAObjectReference and save us
   * the work of unescaping the string by ourselves.
   */

  for (i=length-1; i>0; i--) {
    if (key[i] == '/') {
      break;
    }
  }

  if (i==0 || key[i++-1] != '\\') {
    return find (ObjectId ((const char *) key+i,
			   (CORBA::ULong) length-i,
			   false));
  }

  return find (POAObjectReference (poa, obj));
}

MICOPOA::ObjectMap::ObjectRecord *
MICOPOA::ObjectMap::find (PortableServer::Servant serv)
{
  SvMap::iterator sit = servants.find (serv);
  if (sit == servants.end()) {
    return NULL;
  }
  assert ((*sit).second.size() == 1);
  return (*sit).second[0];
}

/*
 * ----------------------------------------------------------------------
 *
 * The POA
 *
 * ----------------------------------------------------------------------
 */

MICOPOA::POA_impl::InvocationRecord::InvocationRecord (CORBA::ORBMsgId _id,
						       POAObjectReference * _por,
						       CORBA::ORBRequest * _req,
						       CORBA::Principal_ptr _pr)
  : orbid (_id)
{
  por = new POAObjectReference (*_por);
  req = CORBA::ORBRequest::_duplicate (_req);
  pr  = CORBA::Principal::_duplicate (_pr);
  svr = CORBA::ServerRequest::_nil ();
}

MICOPOA::POA_impl::InvocationRecord::~InvocationRecord ()
{
  CORBA::release (req);
  CORBA::release (pr);
  CORBA::release (svr);
  delete por;
}

void
MICOPOA::POA_impl::InvocationRecord::exec (POA_impl * poa)
{
  poa->local_invoke (this);
}

CORBA::ORBMsgId
MICOPOA::POA_impl::InvocationRecord::id ()
{
  return orbid;
}

CORBA::ORBRequest *
MICOPOA::POA_impl::InvocationRecord::get_or ()
{
  return req;
}

MICOPOA::POAObjectReference *
MICOPOA::POA_impl::InvocationRecord::get_por ()
{
  return por;
}

CORBA::ServerRequestBase_ptr
MICOPOA::POA_impl::InvocationRecord::make_req (POA_impl * poa,
					       PortableServer::Servant serv)
{
  assert (serv);
  assert (CORBA::is_nil (svr));
  if (CORBA::is_nil (svr)) {
    svr = serv->make_request (req, por->get_ref(), orbid, poa, pr);
    assert (!CORBA::is_nil (svr));
  }
  return svr;
}

CORBA::ServerRequest_ptr
MICOPOA::POA_impl::InvocationRecord::make_dyn_req (POA_impl * poa)
{
  CORBA::ServerRequest_ptr dynsvr;

  assert (CORBA::is_nil (svr));
  dynsvr = new CORBA::ServerRequest (req, por->get_ref(), id(), poa, pr);
  svr = dynsvr;
  assert (!CORBA::is_nil (svr));
  return dynsvr;
}

/*
 * POA attributes
 */

char *
MICOPOA::POA_impl::the_name ()
{
  return CORBA::string_dup (name.c_str());
}

PortableServer::POA_ptr
MICOPOA::POA_impl::the_parent ()
{
  return PortableServer::POA::_duplicate (parent);
}

PortableServer::POAManager_ptr
MICOPOA::POA_impl::the_POAManager ()
{
  return PortableServer::POAManager::_duplicate (manager);
}

PortableServer::POAList *
MICOPOA::POA_impl::the_children ()
{
  PortableServer::POAList * res = new PortableServer::POAList;
  POAMap::iterator it = children.begin ();
  CORBA::ULong i = 0;

  res->length (children.size());
  while (it != children.end()) {
    (*res)[i++] = PortableServer::POA::_duplicate ((*it++).second);
  }

  return res;
}

PortableServer::AdapterActivator_ptr
MICOPOA::POA_impl::the_activator ()
{
  return PortableServer::AdapterActivator::_duplicate (adapter_activator);
}

void
MICOPOA::POA_impl::the_activator (PortableServer::AdapterActivator_ptr _aa)
{
  adapter_activator = PortableServer::AdapterActivator::_duplicate (_aa);
}

CORBA::OctetSeq*
MICOPOA::POA_impl::id()
{
  string t_str = this->get_oaid();
  CORBA::OctetSeq* retval = new CORBA::OctetSeq;
  retval->length(t_str.length());
  for (CORBA::ULong i=0; i<retval->length(); i++) {
    (*retval)[i] = t_str[i];
  }
  return retval;
}

/*
 * Factories for Policy objects
 */

PortableServer::ThreadPolicy_ptr
MICOPOA::POA_impl::create_thread_policy (PortableServer::ThreadPolicyValue _v)
{
  return new ThreadPolicy_impl (PortableServer::THREAD_POLICY_ID, _v);
}

PortableServer::LifespanPolicy_ptr
MICOPOA::POA_impl::create_lifespan_policy (PortableServer::LifespanPolicyValue _v)
{
  return new LifespanPolicy_impl (PortableServer::LIFESPAN_POLICY_ID, _v);
}

PortableServer::IdUniquenessPolicy_ptr
MICOPOA::POA_impl::create_id_uniqueness_policy (PortableServer::IdUniquenessPolicyValue _v)
{
  return new IdUniquenessPolicy_impl (PortableServer::ID_UNIQUENESS_POLICY_ID, _v);
}

PortableServer::IdAssignmentPolicy_ptr
MICOPOA::POA_impl::create_id_assignment_policy (PortableServer::IdAssignmentPolicyValue _v)
{
  return new IdAssignmentPolicy_impl (PortableServer::ID_ASSIGNMENT_POLICY_ID, _v);
}

PortableServer::ImplicitActivationPolicy_ptr
MICOPOA::POA_impl::create_implicit_activation_policy (PortableServer::ImplicitActivationPolicyValue _v)
{
  return new ImplicitActivationPolicy_impl (PortableServer::IMPLICIT_ACTIVATION_POLICY_ID, _v);
}

PortableServer::ServantRetentionPolicy_ptr
MICOPOA::POA_impl::create_servant_retention_policy (PortableServer::ServantRetentionPolicyValue _v)
{
  return new ServantRetentionPolicy_impl (PortableServer::SERVANT_RETENTION_POLICY_ID, _v);
}

PortableServer::RequestProcessingPolicy_ptr
MICOPOA::POA_impl::create_request_processing_policy (PortableServer::RequestProcessingPolicyValue _v)
{
  return new RequestProcessingPolicy_impl (PortableServer::REQUEST_PROCESSING_POLICY_ID, _v);
}

/*
 * ServantManager registration
 */

PortableServer::ServantManager_ptr
MICOPOA::POA_impl::get_servant_manager ()
{
  if (request_processing_policy->value () != PortableServer::USE_SERVANT_MANAGER) {
    mico_throw (PortableServer::POA::WrongPolicy());
  }

  return PortableServer::ServantManager::_duplicate (servant_manager);
}

void
MICOPOA::POA_impl::set_servant_manager (PortableServer::ServantManager_ptr _s)
{
  if (request_processing_policy->value () != PortableServer::USE_SERVANT_MANAGER) {
    mico_throw (PortableServer::POA::WrongPolicy());
  }

  if (CORBA::is_nil (_s)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  if ((servant_retention_policy->value() == PortableServer::RETAIN &&
       !_s->_is_a ("IDL:omg.org/PortableServer/ServantActivator:1.0")) ||
      (servant_retention_policy->value() == PortableServer::NON_RETAIN &&
       !_s->_is_a ("IDL:omg.org/PortableServer/ServantLocator:1.0"))) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  if (!CORBA::is_nil (servant_manager)) {
    mico_throw (CORBA::BAD_INV_ORDER (6, CORBA::COMPLETED_NO));
  }

  servant_manager = PortableServer::ServantManager::_duplicate (_s);
}

PortableServer::Servant
MICOPOA::POA_impl::get_servant ()
{
  if (request_processing_policy->value () != PortableServer::USE_DEFAULT_SERVANT) {
    mico_throw (PortableServer::POA::WrongPolicy());
  }

  if (default_servant == NULL) {
    mico_throw (PortableServer::POA::NoServant());
  }

  default_servant->_add_ref ();
  return default_servant;
}

void
MICOPOA::POA_impl::set_servant (PortableServer::Servant _s)
{
  if (request_processing_policy->value () != PortableServer::USE_DEFAULT_SERVANT) {
    mico_throw (PortableServer::POA::WrongPolicy());
  }

  if (default_servant) {
    default_servant->_remove_ref ();
  }

  _s->_add_ref ();
  default_servant = _s;
}

/*
 * POA creation and destruction
 */

void
MICOPOA::POA_impl::set_policies (const CORBA::PolicyList & policies)
{
  thread_policy = PortableServer::ThreadPolicy::_nil();
  lifespan_policy = PortableServer::LifespanPolicy::_nil();
  id_uniqueness_policy = PortableServer::IdUniquenessPolicy::_nil();
  id_assignment_policy = PortableServer::IdAssignmentPolicy::_nil();
  implicit_activation_policy =
    PortableServer::ImplicitActivationPolicy::_nil();
  servant_retention_policy = PortableServer::ServantRetentionPolicy::_nil();
  request_processing_policy = PortableServer::RequestProcessingPolicy::_nil();

  CORBA::PolicyList plist;
  for (CORBA::ULong i=0; i<policies.length(); i++) {
    CORBA::Policy_ptr pol = policies[i].in();

    if (pol->policy_type() == PortableServer::THREAD_POLICY_ID) {
      thread_policy = PortableServer::ThreadPolicy::_narrow (pol);
      assert (!CORBA::is_nil (thread_policy));
    }
    else if (pol->policy_type() == PortableServer::LIFESPAN_POLICY_ID) {
      lifespan_policy = PortableServer::LifespanPolicy::_narrow (pol);
      assert (!CORBA::is_nil (lifespan_policy));
    }
    else if (pol->policy_type() == PortableServer::ID_UNIQUENESS_POLICY_ID) {
      id_uniqueness_policy =
	PortableServer::IdUniquenessPolicy::_narrow (pol);
      assert (!CORBA::is_nil (id_uniqueness_policy));
    }
    else if (pol->policy_type() == PortableServer::ID_ASSIGNMENT_POLICY_ID) {
      id_assignment_policy =
	PortableServer::IdAssignmentPolicy::_narrow (pol);
      assert (!CORBA::is_nil (id_assignment_policy));
    }
    else if (pol->policy_type() == PortableServer::IMPLICIT_ACTIVATION_POLICY_ID) {
      implicit_activation_policy =
	PortableServer::ImplicitActivationPolicy::_narrow (pol);
      assert (!CORBA::is_nil (implicit_activation_policy));
    }
    else if (pol->policy_type() == PortableServer::SERVANT_RETENTION_POLICY_ID) {
      servant_retention_policy =
	PortableServer::ServantRetentionPolicy::_narrow (pol);
      assert (!CORBA::is_nil (servant_retention_policy));
    }
    else if (pol->policy_type() == PortableServer::REQUEST_PROCESSING_POLICY_ID) {
      request_processing_policy =
	PortableServer::RequestProcessingPolicy::_narrow (pol);
      assert (!CORBA::is_nil (request_processing_policy));
    }
    else { // see orbos/99-12-02 p. 14-93
	if (PInterceptor::PI::S_pfmap_.find(pol->policy_type()) != PInterceptor::PI::S_pfmap_.end()) {
	    CORBA::ULong l = plist.length();
	    plist.length(l + 1);
	    plist[l] = pol;  		
	}
	else {
	    PortableServer::POA::InvalidPolicy ex;
	    ex.index = i;
	    mico_throw (ex);
	}
    }
  }
  
  if (plist.length() > 0) {
	for (CORBA::ULong i = 0; i < plist.length(); ++i) {
	    CORBA::ULong j;
	    for (j = 0; j < _policies.length(); ++j) {
			if (_policies[j]->policy_type() == plist[i]->policy_type()) {
				_policies[j] = CORBA::Policy::_duplicate (plist[i].in());
				break;
			}
	    }
	    if (j == _policies.length()) {
			_policies.length (j+1);
			_policies[j] = CORBA::Policy::_duplicate (plist[i].in());
	    }
	}
  }

  /*
   * set default policies
   */

  if (CORBA::is_nil (thread_policy)) {
    thread_policy = create_thread_policy (PortableServer::ORB_CTRL_MODEL);
  }

  if (CORBA::is_nil (lifespan_policy)) {
    lifespan_policy = create_lifespan_policy (PortableServer::TRANSIENT);
  }

  if (CORBA::is_nil (id_uniqueness_policy)) {
    id_uniqueness_policy =
      create_id_uniqueness_policy (PortableServer::UNIQUE_ID);
  }

  if (CORBA::is_nil (id_assignment_policy)) {
    id_assignment_policy =
      create_id_assignment_policy (PortableServer::SYSTEM_ID);
  }

  if (CORBA::is_nil (implicit_activation_policy)) {
    implicit_activation_policy =
      create_implicit_activation_policy (PortableServer::NO_IMPLICIT_ACTIVATION);
  }

  if (CORBA::is_nil (servant_retention_policy)) {
    servant_retention_policy =
      create_servant_retention_policy (PortableServer::RETAIN);
  }

  if (CORBA::is_nil (request_processing_policy)) {
    request_processing_policy =
      create_request_processing_policy (PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY);
  }
}

void
MICOPOA::POA_impl::register_child (const char * cname, POA_impl * thechild)
{
  assert (children.find(cname) == children.end());
  (void) PortableServer::POA::_duplicate (thechild);
  children[cname] = thechild;
}

void
MICOPOA::POA_impl::unregister_child (const char * cname)
{
  POAMap::iterator it = children.find (cname);
  assert (it != children.end());
  POA_impl * thechild = (*it).second;
  children.erase (it);
  CORBA::release (thechild);
}

void
MICOPOA::POA_impl::register_poa (const char * pname, POA_impl * thechild)
{
  assert (AllPOAs.find(pname) == AllPOAs.end());
  AllPOAs[pname] = thechild;
}

void
MICOPOA::POA_impl::unregister_poa (const char * pname)
{
  POAMap::iterator it = AllPOAs.find (pname);
  assert (it != AllPOAs.end());
  AllPOAs.erase (it);
}

void
MICOPOA::POA_impl::etherealize ()
{
  /*
   * Clean up the Active Object Map. If RETAIN and USE_SERVANT_MANAGER,
   * call etherealize() on the Servant Activator.
   */

  PortableServer::ServantActivator_var sav;

  if (servant_retention_policy->value() == PortableServer::RETAIN &&
      request_processing_policy->value() == PortableServer::USE_SERVANT_MANAGER) {
    sav = PortableServer::ServantActivator::_narrow (servant_manager);
  }

  {
    MICOMT::AutoLock l(ObjectActivationLock);

    while (!ActiveObjectMap.empty()) {
      ObjectMap::ObjectRecord * orec = ActiveObjectMap.pop ();
      //FIXME: we have make sure, there is no invocation on this orec anymore !!
      //       better wait until it is finished
      //       maybe, we should have a invocation_counter on the orec ?

      if (!CORBA::is_nil (sav)) {
	PortableServer::Servant serv = orec->serv;
	POAObjectReference * por = orec->por;
	orec->por = NULL;
	delete orec;
	
	CORBA::Boolean other = ActiveObjectMap.exists (serv);
#ifdef HAVE_EXCEPTIONS
	try {
#endif
	  if (thread_policy->value() == PortableServer::SINGLE_THREAD_MODEL
	      || thread_policy->value() == PortableServer::MAIN_THREAD_MODEL) {
	    // not optimal but reliable solution
	    // for all POAs with single-thread policy
	    // we will lock global servant manager lock
	    // for main-thread policy this solution is optimal
	    // and right
	    MICOMT::AutoLock t_lock(S_servant_manager_lock);
	    sav->etherealize (por->get_id(), this, serv, TRUE, other);
	  }
	  else {
	    sav->etherealize (por->get_id(), this, serv, TRUE, other);
	  }
#ifdef HAVE_EXCEPTIONS
	} catch (CORBA::SystemException_catch &) {
	  // Exceptions are ignored
	}
#endif
	delete por;
      }
      else {
	delete orec;
      }
    }
  }
}

/*
 * Constructor for child POAs. They are not registered with the ORB
 */

MICOPOA::POA_impl::POA_impl (const char * _name,
			     PortableServer::POAManager_ptr _manager,
			     const CORBA::PolicyList & policies,
			     POA_impl * _parent,
			     CORBA::ORB_ptr _orb)
    : name (_name), parent(_parent), orb(_orb), ObjectActivationLock(FALSE, MICOMT::Mutex::Recursive)
{
  destructed = 0;
  unique_id = 0;
  set_policies (policies);
  ior_template_ = new CORBA::IOR(*_orb->ior_template());
  
  /*
   * We keep one reference to ourselves
   */

  (void) PortableServer::POA::_duplicate (this);

  /*
   * Use given POA Manager or create a new one (managers are a
   * dime a dozen, ask Dilbert)
   */

  if (!CORBA::is_nil (_manager)) {
    manager = PortableServer::POAManager::_duplicate (_manager);
  }
  else {
    manager = new POAManager_impl ();
  }

  assert (!CORBA::is_nil (orb));
  assert (parent);
  assert (!CORBA::is_nil (manager));

  default_servant   = NULL;
  servant_manager   = PortableServer::ServantManager::_nil ();
  adapter_activator = PortableServer::AdapterActivator::_nil ();

  if (parent->fqn.length() > 0) {
    fqn = parent->fqn + "/";
  }

  const char * nptr = name.c_str ();

  while (*nptr) {
    if (*nptr == '/' || *nptr == '\\') {
      fqn += '\\';
    }
    fqn += *nptr++;
  }

  state = manager->get_state ();

  /*
   * Construction of the POA's unique name depends on the Lifespan Policy.
   * For a transient POA, a unique name is generated based on IP Address,
   * the Pid, a timestamp and a unique ID, so that any generated objref
   * becomes invalid as soon as the server terminates or the POA is
   * deactivated.
   * A persistent POA should have received a unique name via the
   * -OAImplName command line option.
   *
   *  / <pid> / <time> / <guid>    (Transient)
   *  <impl-name> / <fqn>          (Persistent)
   *
   * For a persistent POA with <fqn> == <impl_name>, the unique name is
   * collapsed to <impl-name> as a special case
   */

  if (lifespan_policy->value() == PortableServer::TRANSIENT) {
    CORBA::String_var uid = poauid.new_id ();
    oaid  = oaprefix;
    oaid += "/";
    oaid += uid.in();
  }
  else {
    /*
     * We need an ImplName
     */

    if (impl_name.size() == 0) {
      PortableServer::POA::InvalidPolicy ex;
      for (CORBA::UShort j=0; j<policies.length(); j++) {
	CORBA::Policy_var p1 = policies[j];
	PortableServer::LifespanPolicy_var p2 =
	  PortableServer::LifespanPolicy::_narrow (p1);
	if (!CORBA::is_nil (p2) && p2->value() == PortableServer::PERSISTENT) {
	  ex.index = j;
	  break;
	}
      }
      mico_throw (ex);
    }

    assert (impl_name.size() > 0);

    /*
     * Collapse if impl_name == fqn and no escaping is necessary
     */
    
    if (impl_name == fqn && fqn.find ('/') == (size_t) -1) {
      oaid = fqn;
    }
    else {
      const char * inptr = impl_name.c_str ();

      while (*inptr) {
	if (*inptr == '/' || *inptr == '\\') {
	  oaid += '\\';
	}
	oaid += *inptr++;
      }

      oaid += "/";
      oaid += fqn;
    }
  }
  
  /*
   * Register globally, with our manager, and with our parent
   */
  register_poa (oaid.c_str(), this);
  manager->add_managed_poa (this);
  parent->register_child (name.c_str(), this);
// IOR interceptor
  CORBA::IOR * usedior = ior_template();
  PInterceptor::IORInfo_impl iorinfo(this, usedior);
  PInterceptor::PI::_exec_establish_components(&iorinfo);

#ifdef USE_CSL2
#ifdef HAVE_SSL
  odm_factory_ = NULL;
  odm_manager_ = NULL;
#endif // HAVE_SSL
#endif // USE_CSL2
}

/*
 * Constructor for Root POA
 */

MICOPOA::POA_impl::POA_impl (CORBA::ORB_ptr _orb)
  : orb (_orb)
{
  destructed = 0;
  unique_id = 0;
  ior_template_ = new CORBA::IOR(*_orb->ior_template());

  /*
   * We keep one reference to ourselves
   */

  (void) PortableServer::POA::_duplicate (this);

  /*
   * The only non-default policy for the RootPOA is the Implicit
   * Activation policy, which is IMPLICIT_ACTIVATION
   */

  CORBA::PolicyList policies (1);
  policies.length (1);
  policies[0] =
    create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION);
  set_policies (policies);

  parent = NULL;
  manager = new POAManager_impl ();
  current = new POACurrent_impl (orb);

  assert (!CORBA::is_nil (orb));
  assert (!CORBA::is_nil (manager));
  assert (current);

  default_servant   = NULL;
  servant_manager   = PortableServer::ServantManager::_nil ();
  adapter_activator = PortableServer::AdapterActivator::_nil ();

  name  = (const char *) "RootPOA";
  state = PortableServer::POAManager::HOLDING;

  /*
   * Generate a unique prefix for transient POAs based on the PID and the
   * current time.
   */

  OSMisc::TimeVal ct = OSMisc::gettime();

  oaprefix  = "/";
  oaprefix += xdec (OSMisc::getpid());
  oaprefix += "/";
  oaprefix += xdec (ct.tv_sec);

  /*
   * The Root POA has a TRANSIENT Lifespan Policy. Therefore, use
   *  / <pid> / <time>
   * as the unique name
   */

  oaid = oaprefix;

  /*
   * Assign ImplName.
   */

  if (poaopts["-POAImplName"] != NULL) {
    impl_name = poaopts["-POAImplName"];
  }
  else {
    impl_name = "Default";
  }

  /*
   * If we have an ImplName, connect to the Mediator
   */

  poamed = CORBA::POAMediator::_nil();
  ever_been_active = FALSE;

  if (poaopts["-POAImplName"] != NULL) {
    CORBA::Object_var obj = CORBA::Object::_nil();
    if (poaopts["-POARemoteIOR"] != NULL) {
	obj = orb->string_to_object (poaopts["-POARemoteIOR"]);
	assert (!CORBA::is_nil (obj));
    } else if (poaopts["-POARemoteAddr"] != NULL) {
      obj = orb->bind ("IDL:omg.org/CORBA/POAMediator:1.0",
		       poaopts["-POARemoteAddr"]);
      assert (!CORBA::is_nil (obj));
    }
    if (!CORBA::is_nil (obj)) {
      poamed = CORBA::POAMediator::_narrow (obj);
      assert (!CORBA::is_nil (poamed));
    }
    if (!CORBA::is_nil (poamed)) {
      /*
       * register with mediator
       */

      string myref = orb->ior_template()->stringify();
      CORBA::String_var poamed_ref =
	poamed->create_impl (impl_name.c_str(), myref.c_str());
      poamed_ior = CORBA::IOR (poamed_ref);
    }
  }

  /*
   * Set Root POA
   */

  PortableServer::_the_root_poa = this;

  /*
   * Register with Manager
   */

  register_poa (oaid.c_str(), this);
  manager->add_managed_poa (this);

  /*
   * Register with ORB
   */

  orb->set_initial_reference ("RootPOA", this);
  orb->register_oa (this);
// IOR interceptor
  CORBA::IOR * usedior = ior_template();
  PInterceptor::IORInfo_impl iorinfo(this, usedior);
  PInterceptor::PI::_exec_establish_components(&iorinfo);

#ifdef USE_CSL2
#ifdef HAVE_SSL
  odm_factory_ = NULL;
  odm_manager_ = NULL;
#endif // HAVE_SSL
#endif // USE_CSL2
}

MICOPOA::POA_impl::~POA_impl ()
{
  destroy (FALSE, FALSE);
  assert (children.empty());
  assert (InvocationQueue.empty());

  if (PortableServer::_the_root_poa == this) {
    PortableServer::_the_root_poa = PortableServer::POA::_nil ();
    CORBA::release (current);
  }

  CORBA::release (manager);
  delete ior_template_;
}

PortableServer::POA_ptr
MICOPOA::POA_impl::create_POA (const char * cname,
			       PortableServer::POAManager_ptr theManager,
			       const CORBA::PolicyList & policies)
{
  MICOMT::AutoRDLock t_lock(destroy_lock_);
  if (destructed)
    mico_throw(CORBA::BAD_INV_ORDER(17, CORBA::COMPLETED_NO));

  if (children.find (cname) != children.end()) {
    mico_throw (PortableServer::POA::AdapterAlreadyExists());
  }

  PortableServer::POA_ptr child = new POA_impl (cname, theManager,
						policies, this, orb);
  assert (!CORBA::is_nil (child));
  return child;
}

MICOPOA::POA_impl *
MICOPOA::POA_impl::_find_POA (const char * cname,
			      CORBA::Boolean activate_it)
{
  POAMap::iterator it = children.find (cname);

  if (it != children.end()) {
    return (*it).second;
  }

  if (!activate_it || CORBA::is_nil (adapter_activator)) {
    return NULL;
  }

  /*
   * May only invoke the adapter activator in the Active state
   */

  if (state != PortableServer::POAManager::ACTIVE) {
    return NULL;
  }
  //
  // We need to test if this POA is being destructed
  //
  {
    MICOMT::AutoRDLock t_lock(destroy_lock_);
    if (destructed)
      mico_throw(CORBA::TRANSIENT(4, CORBA::COMPLETED_NO));

    if (!adapter_activator->unknown_adapter (this, cname)) {
      return NULL;
    }
  }

  /*
   * See if the new adapter has registered itself
   */

  if ((it = children.find (cname)) != children.end()) {
    return (*it).second;
  }

  return NULL;
}


PortableServer::POA_ptr
MICOPOA::POA_impl::find_POA (const char * cname,
			     CORBA::Boolean activate_it)
{
  POA_impl * child = NULL;

#ifdef HAVE_EXCEPTIONS
  try {
#endif
    child = _find_POA (cname, activate_it);
#ifdef HAVE_EXCEPTIONS
  } catch (CORBA::SystemException_catch & sex) {
    mico_throw (CORBA::OBJ_ADAPTER (1, CORBA::COMPLETED_NO));
  }
#endif

  if (!child) {
    mico_throw (PortableServer::POA::AdapterNonExistent());
  }

  return PortableServer::POA::_duplicate (child);
}

void
MICOPOA::POA_impl::destroy (CORBA::Boolean etherealize_objects,
			    CORBA::Boolean wait_for_completion)
{
  //FIXME: add context check !!!
  MICOMT::AutoWRLock t_lock(destroy_lock_);
  if (destructed) {
    return;
  }

  //
  // POA is being destructed, this will ensure right
  // behavior of create_POA and poa_manager_callback
  // methods
  //
  destructed = 1;
  
  //
  // Destructed POA is assuming discarding state
  // except when the poa manager is in inactive state
  //
//    if (state != PortableServer::POAManager::INACTIVE)
//      state = PortableServer::POAManager::DISCARDING;
  //
  // kcg: code above is compliant with CORBA spec.
  // the problem is that etherealization which is below
  // depends on POA in active state
  //

  /*
   * Destroy child POAs
   */

  while (!children.empty()) {
    POAMap::iterator it = children.begin ();
    PortableServer::POA_ptr child = (*it).second;
    child->destroy (etherealize_objects, wait_for_completion);
  }

  //
  // POA continues with processing request(s)
  //

  while (!InvocationQueue.empty()) {
    InvocationRecord_ptr ir = InvocationQueue.front();
    InvocationQueue.erase (InvocationQueue.begin());
    ir->exec (this);
    CORBA::release (ir);
  }

  //
  // No request in the queue, the POA will become destructed
  // ie. unregister from parent POA/ORB and from manager
  //

  /*
   * unregister globally, from parent and from manager
   */

  unregister_poa (oaid.c_str());

  if (parent) {
    parent->unregister_child (name.c_str());
  }

  manager->del_managed_poa (this);

  /*
   * The RootPOA says goodbye to the POAMediator
   */

  if (!parent) {
    if (!CORBA::is_nil (poamed)) {
      poamed->deactivate_impl (impl_name.c_str());
    }
  }

  /*
   * Root POA must unregister from the ORB
   */

  if (!parent) {
    orb->unregister_oa (this);
  }

  /*
   * Etherealize active objects
   */

  if (etherealize_objects) {
    etherealize ();
  }

  if (default_servant) {
    default_servant->_remove_ref ();
  }

  /*
   * Clean up Active Object Map
   */

  {
    //FIXME: this is a quick fix only, has to be integrated in a better POA::destroy solution
    MICOMT::AutoLock l(ObjectActivationLock);
    ActiveObjectMap.clear ();
  }

  /*
   * Release the reference that we hold ourselves
   */

  CORBA::release (this);
}

/*
 * Callback from POA Manager
 */

void
MICOPOA::POA_impl::poa_manager_callback (PortableServer::POAManager::State s,
					 CORBA::Boolean etherealize_objects,
					 CORBA::Boolean wait_for_completion)
{
  MICOMT::AutoRDLock t_lock(destroy_lock_);
  if (destructed)
    return;

  state = s;

  /*
   * If any POA becomes active, we call back the mediator, which
   * may have started us up, and which may have invocations queued for
   * us.
   */

  if (state == PortableServer::POAManager::ACTIVE &&
      !ever_been_active && !CORBA::is_nil (poamed)) {
    ever_been_active = TRUE;
    poamed->activate_impl (impl_name.c_str());
  }

  /*
   * Perform state transition
   */

  switch (state) {
  case PortableServer::POAManager::HOLDING:
    /*
     * Nothing to do on transition to Holding
     */
    break;

  case PortableServer::POAManager::ACTIVE:
    /*
     * perform queued invocations
     */
    while (!InvocationQueue.empty()) {
      InvocationRecord_ptr ir = InvocationQueue.front();
      InvocationQueue.erase (InvocationQueue.begin());
      ir->exec (this);
      CORBA::release (ir);
    }
    break;

  case PortableServer::POAManager::INACTIVE:
    /*
     * Etherealize active objects
     */

    if (etherealize_objects) {
      etherealize ();
    }
    break;

  case PortableServer::POAManager::DISCARDING:
    /*
     * local_invoke() will discard queued invocations
     */
    while (!InvocationQueue.empty()) {
      InvocationRecord_ptr ir = InvocationQueue.front();
      InvocationQueue.erase (InvocationQueue.begin());
      ir->exec (this);
      CORBA::release (ir);
    }
    break;

  default:
    assert (0);
  }
}

/*
 * This is a callback from POAObjectReference. When a new reference
 * is to be generated, we are asked for the IOR template to use. This
 * is either our own in a transient POA, or the POA Mediator's in a
 * persistent one.
 */

CORBA::IOR *
MICOPOA::POA_impl::ior_template ()
{
  if (lifespan_policy->value() == PortableServer::PERSISTENT) {
    if (!CORBA::is_nil (poamed)) {
      return &poamed_ior;
    }
  }

  return ior_template_;
}

/*
 * Object activation and deactivation
 */

PortableServer::ObjectId *
MICOPOA::POA_impl::activate_object (PortableServer::Servant servant)
{
  MICOMT::AutoLock l(ObjectActivationLock);
  return __activate_object(servant);
}

/*
 * __activate_object has to be called with the
 * ObjectActivationLock held
 */
PortableServer::ObjectId *
MICOPOA::POA_impl::__activate_object (PortableServer::Servant servant)
{
  assert (servant);

  /*
   * Requires SYSTEM_ID and RETAIN
   */

  if (id_assignment_policy->value() != PortableServer::SYSTEM_ID ||
      servant_retention_policy->value() != PortableServer::RETAIN) {
    mico_throw (PortableServer::POA::WrongPolicy());
  }

  /*
   * See if Servant is already active
   */

  if (id_uniqueness_policy->value() != PortableServer::MULTIPLE_ID) {
    if (ActiveObjectMap.exists (servant)) {
      mico_throw (PortableServer::POA::ServantAlreadyActive());
    }
  }

  /*
   * Generate Object Reference
   *
   * In a persistent POA, we prefix the Object Id with oaprefix. This
   * takes care of Object Id Uniqueness in case of server restart.
   */

  CORBA::String_var uid = idfactory.new_id ();
  string fulluid;

  if (lifespan_policy->value() == PortableServer::PERSISTENT) {
    fulluid = oaprefix;
  }
  fulluid += uid.in();

  PortableServer::ObjectId * id =
    PortableServer::string_to_ObjectId (fulluid.c_str());
  CORBA::String_var iface = servant->_primary_interface (*id, this);
  POAObjectReference * por = new POAObjectReference (this, *id, iface,
						     servant);

  /*
   * Tell the servant that we're the one to ask for _this references
   */

  servant->_activated_in (this);

  /*
   * Insert into active object map
   */

  ActiveObjectMap.add (por, servant);
  return id;
}

void
MICOPOA::POA_impl::activate_object_with_id (const PortableServer::ObjectId & id,
					    PortableServer::Servant servant)
{
  assert (servant);

  /*
   * Requires RETAIN
   */

  if (servant_retention_policy->value() != PortableServer::RETAIN) {
    mico_throw (PortableServer::POA::WrongPolicy());
  }

  {
    MICOMT::AutoLock l(ObjectActivationLock);
    /*
     * See if Object or Servant is already active
     * or being deactivated
     */
    ObjectMap::ObjectRecord * orec;
    
    orec = ActiveObjectMap.find (id);
#ifdef HAVE_THREADS
    while ( orec && !orec->active ) {
      // object is being deactivated
      ObjectMap::DeletionRecord *delref = orec->delref;
      if (!delref) {
	delref = new ObjectMap::DeletionRecord(&ObjectActivationLock);
	orec->delref = delref;
      }
      delref->cnt++;
      do {
	// wait unlocks the ObjectActivationLock
	delref->condition.wait();
      } while (delref->signal != 0);
      delref->cnt--;
      if (delref->cnt == 0)
	delete delref;
      // orec is gone now and we hold the ObjectActivationLock again
      orec = ActiveObjectMap.find (id);
    }
#endif // HAVE_THREADS
    if ( orec ) {
      mico_throw (PortableServer::POA::ObjectAlreadyActive());
    }
    
    if (id_uniqueness_policy->value() != PortableServer::MULTIPLE_ID) {
      if (ActiveObjectMap.exists (servant)) {
	mico_throw (PortableServer::POA::ServantAlreadyActive());
      }
    }

    /*
     * Generate Object Reference
     */
    
    CORBA::String_var iface = servant->_primary_interface (id, this);
    POAObjectReference * por = new POAObjectReference (this, id, iface,
						       servant);
    
    /*
     * Tell the servant that we're the one to ask for _this references
     */
    
    servant->_activated_in (this);
    
    /*
     * Insert into active object map
     */
    
    ActiveObjectMap.add (por, servant);
  }
}

void
MICOPOA::POA_impl::deactivate_object (const PortableServer::ObjectId & id)
{
  /*
   * Requires RETAIN
   */

  if (servant_retention_policy->value() != PortableServer::RETAIN) {
    mico_throw (PortableServer::POA::WrongPolicy());
  }

  ObjectMap::ObjectRecord * orec;
  {
    MICOMT::AutoLock l(ObjectActivationLock);

    if (!ActiveObjectMap.exists (id)) {
      mico_throw (PortableServer::POA::ObjectNotActive());
    }
    
    orec = ActiveObjectMap.find (id);
    orec->active = FALSE;

    /*
     * there are no outstanding invocations, so remove it finaly
     */
    if (orec->invoke_cnt == 0)
      remove_object( id );

  }
}

/*
 * this methode has always to be called with the ObjectActivationLock held
 */
void
MICOPOA::POA_impl::remove_object (const PortableServer::ObjectId & id)
{
  ObjectMap::ObjectRecord * orec;
  CORBA::Boolean other;

  /*
   * we made a mistake somewhere is this should ever happen
   */
  assert( ActiveObjectMap.exists (id) );
  
  orec = ActiveObjectMap.del (id);
#ifdef HAVE_THREADS
  ObjectMap::DeletionRecord *delref;
  delref = orec->delref;
#endif // HAVE_THREADS
  other = ActiveObjectMap.exists (orec->serv);

  /*
   * Etherealize object
   */

  if (!CORBA::is_nil (servant_manager)) {
    PortableServer::ServantActivator_var sav =
      PortableServer::ServantActivator::_narrow (servant_manager);
    assert (!CORBA::is_nil (sav));

    PortableServer::Servant serv = orec->serv;
    POAObjectReference * por = orec->por;
    orec->por = NULL;
    delete orec;

    if (thread_policy->value() == PortableServer::SINGLE_THREAD_MODEL
	|| thread_policy->value() == PortableServer::MAIN_THREAD_MODEL) {
      // not optimal but reliable solution
      // for all POAs with single-thread policy
      // we will lock global servant manager lock
      // for main-thread policy this solution is optimal
      // and right
      MICOMT::AutoLock t_lock(S_servant_manager_lock);
      sav->etherealize (por->get_id(), this, serv, FALSE, other);
    }
    else {
      sav->etherealize (por->get_id(), this, serv, FALSE, other);
    }
    delete por;
  }
  else {
    delete orec;
  }
#ifdef HAVE_THREADS  
  if (delref) {
    delref->signal = 1;
    delref->condition.broadcast();
  }
#endif // HAVE_THREADS
}

CORBA::Object_ptr
MICOPOA::POA_impl::create_reference (const char * repoid)
{
  assert (repoid);

  /*
   * Requires SYSTEM_ID
   */

  if (id_assignment_policy->value() != PortableServer::SYSTEM_ID) {
    mico_throw (PortableServer::POA::WrongPolicy());
  }

  CORBA::String_var uid = idfactory.new_id ();
  string fulluid;

  if (lifespan_policy->value() == PortableServer::PERSISTENT) {
    fulluid = oaprefix;
  }
  fulluid += uid.in();

  PortableServer::ObjectId_var id =
    PortableServer::string_to_ObjectId (fulluid.c_str());
  POAObjectReference * por = new POAObjectReference (this, id.in(), repoid);
  CORBA::Object_ptr obj = por->ref();
  assert (!CORBA::is_nil (obj));
  delete por;
  return obj;
}

CORBA::Object_ptr
MICOPOA::POA_impl::create_reference_with_id (const PortableServer::ObjectId & id,
					     const char * repoid)
{
  assert (repoid);

  POAObjectReference * por = new POAObjectReference (this, id, repoid);
  CORBA::Object_ptr obj = por->ref();
  if (id_assignment_policy->value() == PortableServer::SYSTEM_ID
      && CORBA::is_nil(obj)) {
    // it seems that supplied object id
    // was not generated by this orb/poa
    mico_throw(CORBA::BAD_PARAM(14, CORBA::COMPLETED_NO));
  }
  assert (!CORBA::is_nil (obj));
  delete por;
  return obj;
}

/*
 * Perform activation upon _this()
 */

CORBA::Object_ptr
MICOPOA::POA_impl::activate_for_this (PortableServer::Servant servant)
{
  /*
   * Requires RETAIN and UNIQUE_ID
   */

  if (servant_retention_policy->value() != PortableServer::RETAIN ||
      id_uniqueness_policy->value() != PortableServer::UNIQUE_ID) {
    mico_throw (PortableServer::POA::WrongPolicy());
  }

  {
    MICOMT::AutoLock l(ObjectActivationLock);
    /*
     * See if the object is already active
     */
    
    ObjectMap::ObjectRecord * orec = ActiveObjectMap.find (servant);
    
    /*
     * If not active, require IMPLICIT_ACTIVATION and activate it
     */
    
    if (!orec || !orec->active) {
      if (implicit_activation_policy->value() != PortableServer::IMPLICIT_ACTIVATION) {
	mico_throw (PortableServer::POA::WrongPolicy());
      }

      PortableServer::ObjectId * id = __activate_object (servant);
      orec = ActiveObjectMap.find (*id);
      assert (orec);
      delete id;
    }

    return orec->por->ref();
  }
}

/*
 * Identity mapping operations
 */

PortableServer::ObjectId *
MICOPOA::POA_impl::servant_to_id (PortableServer::Servant servant)
{
  assert (servant);

  //
  // Check if we are inside context of POA invocation
  // or if all required policies are presented
  //
  CORBA::Boolean inside = FALSE;
  if ((!CORBA::is_nil(PortableServer::_the_poa_current))
      && PortableServer::_the_poa_current->iscurrent())
    inside = TRUE;

  if (!inside && (!(request_processing_policy->value() == PortableServer::USE_DEFAULT_SERVANT
		    || (servant_retention_policy->value() == PortableServer::RETAIN
			&& (implicit_activation_policy->value() == PortableServer::IMPLICIT_ACTIVATION
			    || id_uniqueness_policy->value() == PortableServer::UNIQUE_ID))))) {
    mico_throw(PortableServer::POA::WrongPolicy());
  }
  /*
   * If the POA has the USE_DEFAULT_SERVANT policy, the servant
   * specified is the default servant, and the operation is being
   * invoked in the context of executing a request on the default
   * servant, then the ObjectId associated with the current invocation
   * is returned.
   */
  
  if (request_processing_policy->value() == PortableServer::USE_DEFAULT_SERVANT &&
      servant == default_servant &&
      !CORBA::is_nil (PortableServer::_the_poa_current) &&
      PortableServer::_the_poa_current->iscurrent() &&
      PortableServer::_the_poa_current->get_servant() == servant) {
    return PortableServer::_the_poa_current->get_object_id ();
  }

  /*
   * Requires RETAIN
   */

  if (servant_retention_policy->value() == PortableServer::RETAIN) {
    MICOMT::AutoLock l(ObjectActivationLock);

    /*
     * Search in active map
     */
    
    ObjectMap::ObjectRecord * orec = ActiveObjectMap.find (servant);

    //
    // If UNIQUE_ID and servant is active
    //
    if (orec && orec->active
	&& id_uniqueness_policy->value() == PortableServer::UNIQUE_ID) {
      return orec->por->id();
    }

    /*
     * If IMPLICIT_ACTIVATION && MULTIPLE_ID || servant is not active,
     * the servant is activated
     */
    
    if (implicit_activation_policy->value() == PortableServer::IMPLICIT_ACTIVATION
	&& (!(orec && orec->active)
	    || id_uniqueness_policy->value() == PortableServer::MULTIPLE_ID)) {
      return __activate_object (servant);
    }
  } // end autoLock

  mico_throw (PortableServer::POA::ServantNotActive());

  /*
   * Never reached
   */

  assert (0);
  return NULL;
}

CORBA::Object_ptr
MICOPOA::POA_impl::servant_to_reference (PortableServer::Servant servant)
{
  assert (servant);

  /*
   * If invoked in the context of executing request on the specified
   * servant, the reference associated with the current invocation is
   * returned.
   */

  if (!CORBA::is_nil (PortableServer::_the_poa_current) &&
      PortableServer::_the_poa_current->iscurrent() &&
      PortableServer::_the_poa_current->get_servant() == servant) {
    return PortableServer::_the_poa_current->get_reference();
  }

  /*
   * Requires RETAIN and either UNIQUE_ID or IMPLICIT_ACTIVATION
   */

  if (servant_retention_policy->value() != PortableServer::RETAIN ||
      (id_uniqueness_policy->value() != PortableServer::UNIQUE_ID &&
       implicit_activation_policy->value() != PortableServer::IMPLICIT_ACTIVATION)) {
    mico_throw (PortableServer::POA::WrongPolicy());
  }
  
  {
    MICOMT::AutoLock l(ObjectActivationLock);
    ObjectMap::ObjectRecord * orec = ActiveObjectMap.find (servant);
    
    /*
     * If IMPLICIT_ACTIVATION, and if either MULTIPLE_ID or the servant
     * is not active yet, the Servant is activated
     */
    
    if (implicit_activation_policy->value() == PortableServer::IMPLICIT_ACTIVATION) {
      if (id_uniqueness_policy->value() == PortableServer::MULTIPLE_ID || !orec ) {
	PortableServer::ObjectId * id = __activate_object (servant);
	orec = ActiveObjectMap.find (*id);
	assert (orec);
	delete id;
      } else {
	if (orec && !orec->active)
	  //FIXME: nu sto
	  assert( 0 );
      }
    }
    if (orec && orec->active) {
      return orec->por->ref();
    }
  } // end autoLock
  
  mico_throw (PortableServer::POA::ServantNotActive());

  /*
   * never reached
   */

  assert (0);
  return CORBA::Object::_nil();
}

PortableServer::Servant
MICOPOA::POA_impl::reference_to_servant (CORBA::Object_ptr obj)
{
  assert (!CORBA::is_nil (obj));

  /*
   * Requires RETAIN or USE_DEFAULT_SERVANT
   */

  if (servant_retention_policy->value()  != PortableServer::RETAIN &&
      request_processing_policy->value() != PortableServer::USE_DEFAULT_SERVANT) {
    mico_throw (PortableServer::POA::WrongPolicy());
  }

  /*
   * Check if the object reference was created by this POA
   */

  POAObjectReference por (this, obj);

  if (!por.is_legal() || !por.in_poa(oaid.c_str())) {
    mico_throw (PortableServer::POA::WrongAdapter());
  }

  /*
   * If RETAIN, look in active object map
   */

  if (servant_retention_policy->value() == PortableServer::RETAIN) {
    MICOMT::AutoLock l(ObjectActivationLock);

    ObjectMap::ObjectRecord * orec = ActiveObjectMap.find (por);

    if (orec && orec->active) {
      orec->serv->_add_ref ();
      return orec->serv;
    }
  }

  /*
   * If USE_DEFAULT_SERVANT, return default servant
   */

  if (request_processing_policy->value() == PortableServer::USE_DEFAULT_SERVANT && default_servant) {
    default_servant->_add_ref ();
    return default_servant;
  }

  mico_throw (PortableServer::POA::ObjectNotActive());

  /*
   * never reached
   */

  assert (0);
  return NULL;
}

PortableServer::ObjectId *
MICOPOA::POA_impl::reference_to_id (CORBA::Object_ptr obj)
{
  /*
   * Check if the object reference was created by this POA
   */

  POAObjectReference por (this, obj);

  if (!por.is_legal() || !por.in_poa(oaid.c_str())) {
    mico_throw (PortableServer::POA::WrongAdapter());
  }

  /*
   * Extract Id from Reference
   */

  return por.id();
}

PortableServer::Servant
MICOPOA::POA_impl::id_to_servant (const PortableServer::ObjectId &id)
{
  /*
   * Requires RETAIN
   */

  if (servant_retention_policy->value() != PortableServer::RETAIN) {
    mico_throw (PortableServer::POA::WrongPolicy());
  }

  /*
   * Search in active object map
   */
  {
    MICOMT::AutoLock l(ObjectActivationLock);

    ObjectMap::ObjectRecord * orec = ActiveObjectMap.find (id);
    
    if (!orec || !orec->active) {
      mico_throw (PortableServer::POA::ObjectNotActive());
    }
    
    orec->serv->_add_ref ();
    return orec->serv;
  }
}

CORBA::Object_ptr
MICOPOA::POA_impl::id_to_reference (const PortableServer::ObjectId & id)
{
  /*
   * Requires RETAIN
   */

  if (servant_retention_policy->value() != PortableServer::RETAIN) {
    mico_throw (PortableServer::POA::WrongPolicy());
  }

  /*
   * Search in active object map
   */
  {
    MICOMT::AutoLock l(ObjectActivationLock);

    ObjectMap::ObjectRecord * orec = ActiveObjectMap.find (id);
    
    if (!orec || !orec->active ) {
      mico_throw (PortableServer::POA::ObjectNotActive());
    }
    
    return orec->por->ref();
  }
}

/*
 * Called by a stub before directly invoking an operation on the skeleton.
 * After setting POACurrent, we return the servant if the invocation is
 * allowed to proceed. This requires that (a) the POA is in the active
 * state and (b) the Object is still active.
 * If we return NULL, the stub will use the SII/DII instead.
 */

PortableServer::Servant
MICOPOA::POA_impl::preinvoke (CORBA::Object_ptr obj)
{
  MICOMT::AutoLock l(ObjectActivationLock);

  switch (state) {
  case PortableServer::POAManager::HOLDING:
  case PortableServer::POAManager::DISCARDING:
    mico_throw (CORBA::TRANSIENT());

  case PortableServer::POAManager::INACTIVE:
    return NULL;
  default:
    break;
  }

  ObjectMap::ObjectRecord * orec = ActiveObjectMap.find (this, obj);
  PortableServer::Servant serv;
  POAObjectReference * por;
  
  if (orec && orec->active) {
    serv = orec->serv;
    por = orec->por;
  }
  else if (request_processing_policy->value() == PortableServer::USE_DEFAULT_SERVANT 
	     && default_servant) {
    serv = default_servant;
    por = new POAObjectReference (this, obj);
  }
  else {
      return NULL;
  }
  
  //FIXME: we better make sure nobody can delete the servant now, until 
  //       our invocation is finished, see 11.3.8.17
  current->set (this, por, serv);
  return serv;
}

void
MICOPOA::POA_impl::postinvoke ()
{
  POAObjectReference * por = current->get_por ();
  current->unset ();
  if (request_processing_policy->value() == PortableServer::USE_DEFAULT_SERVANT && default_servant) {
    delete por;
  }
  //FIXME: invocation is finished - this would be the right place to relaese the servant.....
}

/*
 * ObjectAdapter Interface
 */

const char *
MICOPOA::POA_impl::get_oaid () const
{
  return oaid.c_str();
}

/*
 * has_object() is just to test if this is the right OA for the
 * object. Therefore, we can return TRUE if the reference refers
 * to any object in any POA in the local process.
 */

CORBA::Boolean
MICOPOA::POA_impl::has_object (CORBA::Object_ptr obj)
{
  CORBA::Long length;
  const CORBA::Octet * key;

  CORBA::IORProfile* prof = NULL;
  if (CORBA::is_nil (obj) || !obj->_ior() || !(prof = obj->_ior()->profile())) {
    return FALSE;
  }

  key = prof->objectkey (length);

  if (!key || length < 0) {
    return FALSE;
  }

  if (oaprefix.size() > 0 && (CORBA::ULong) length >= oaprefix.size()) {
    if (strncmp ((const char *) key, oaprefix.c_str(),
		 oaprefix.size()) == 0) {
      return TRUE;
    }
  }

  if (impl_name.size() > 0 && (CORBA::ULong) length >= impl_name.size()) {
    if (strncmp ((const char *) key, impl_name.c_str(), impl_name.size()) == 0
	&& ((CORBA::ULong)length==impl_name.size()
	    || key[impl_name.size()] == '/')) {
      return TRUE;
    }
  }

  if (!orb->plugged() && CORBA::ORB::is_mobile_key (key)) {
    // XXX - Should also check if it is our mobile key
    return TRUE;
  }

  return FALSE;
}

CORBA::Boolean
MICOPOA::POA_impl::is_local () const
{
  return TRUE;
}

#ifdef USE_CSL2
CORBA::Principal_ptr
MICOPOA::POA_impl::get_principal(CORBA::Object_ptr obj){
  assert(0);
  return (CORBA::Principal_ptr)0;
}
#endif /* USE_CSL2  */

/*
 * Global invoke. Only called for the RootPOA, which then delegates
 * the invocation to the appropriate descendant POA (including it-
 * self).
 */

CORBA::Boolean
MICOPOA::POA_impl::invoke (CORBA::ORBMsgId id,
			   CORBA::Object_ptr obj,
			   CORBA::ORBRequest * req,
			   CORBA::Principal_ptr pr,
			   CORBA::Boolean response_exp)
{
  assert (this == PortableServer::_the_root_poa);

  /*
   * Decompose reference
   */

  POAObjectReference por (this, obj);
  assert (por.is_legal());

  /*
   * If the Root POA is being destructed, queue the invocation. This
   * can only happen if the destruction is in progress and the Root
   * POA hasn't yet unregistered with the ORB.
   * After destruction is complete, the invocation queue is flushed;
   * local_invoke() will then either fail or bounce the request.
   */

  if (destructed) {
    InvocationRecord_ptr ir = new InvocationRecord (id, &por, req, pr);
    InvocationQueue.push_back (ir);
    return TRUE;
  }

  /*
   * See if obj is a reference to an object in this or a descendant POA.
   */

  POAMap::iterator it = AllPOAs.find (por.poa_name());
  POA_impl * poa;

  if (it != AllPOAs.end()) {
    poa = (*it).second;
  }
  else {
    /*
     * Doesn't exist. Search the most appropriate POA.
     * We need to do this only if we're looking for a persistent object;
     * there's no sense in reactivating transient POA's.
     */

    if (impl_name.size() == 0 || *por.poa_name() == '/') {
      InvocationRecord_var ir = new InvocationRecord (id, &por, req, pr);
      CORBA::ServerRequest_ptr svr = ir->make_dyn_req (this);
      CORBA::Exception * ex =
	new CORBA::OBJECT_NOT_EXIST (2, CORBA::COMPLETED_NO);
      svr->exception (ex);
      return TRUE;
    }

    CORBA::String_var cname;
    POA_impl *iter;
    poa = this;

    while (42) {
      if (por.in_poa (poa->oaid.c_str())) {
	break;
      }
      cname = por.next_descendant_poa (poa->fqn.c_str(), impl_name.c_str());
#ifdef HAVE_EXCEPTIONS
      try {
#endif
	iter = poa->_find_POA (cname, FALSE);
#ifdef HAVE_EXCEPTIONS
      } catch (CORBA::SystemException_catch & sex) {
	InvocationRecord_var ir = new InvocationRecord (id, &por, req, pr);
	CORBA::ServerRequest_ptr svr = ir->make_dyn_req (poa);
	CORBA::Exception * ex =
	  new CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO);
	svr->exception (ex);
	return TRUE;
      }
#endif

      if (iter == NULL) {
	break;
      }

      poa = iter;
    }
  }

  /*
   * Okay, found responsible POA
   */

  POAObjectReference por2(poa, obj);
  assert (por2.is_legal());
  InvocationRecord_var ir = new InvocationRecord (id, &por2, req, pr);

  poa->local_invoke (ir);

  return TRUE;
}

/*
 * Local invoke; only called for objects in this POA.
 *
 * Also called for requests on non-existent descendant POAs that we
 * must recreate via our adapter activator. But we may only do so in
 * the active state.
 */

void
MICOPOA::POA_impl::local_invoke (InvocationRecord_ptr ir)
{
  /*
   * If we are in a persistent POA that is being destructed, reply
   * with a LocationForward to myself, which will eventually cause
   * the poa to be reactivated.
   */

  if ((lifespan_policy->value() == PortableServer::PERSISTENT ||
       !ir->get_por()->in_poa (oaid.c_str())) && destructed) {
    if (CORBA::is_nil (poamed)) {
      CORBA::ServerRequest_ptr svreq = ir->make_dyn_req (this);
      CORBA::Exception * ex =
	new CORBA::OBJECT_NOT_EXIST (0, CORBA::COMPLETED_NO);
      svreq->exception (ex);
    }
    else {
      CORBA::Object_var obj = ir->get_por()->ref();
      const CORBA::Octet * key;
      CORBA::Long length;
      CORBA::IOR * theior = new CORBA::IOR (poamed_ior);
      key = obj->_ior()->profile()->objectkey (length);
      theior->objectkey ((CORBA::Octet *) key, length);
      // XXX obj->_repoid() is empty ...
      theior->objid (obj->_ior()->objid());
      CORBA::Object_var local_ref = new CORBA::Object (theior);
      orb->answer_invoke (ir->id(), CORBA::InvokeForward,
			  local_ref, ir->get_or(), 0);
    }
    return;
  }

  /*
   * What does our manager want us to do?
   */

  switch (state) {
  case PortableServer::POAManager::HOLDING:
    /*
     * Queue incoming requests
     */
    {
      InvocationQueue.push_back (InvocationRecord::_duplicate (ir));
      return;
    }

  case PortableServer::POAManager::DISCARDING:
    /*
     * Discard incoming requests
     */
    {
      CORBA::ServerRequest_ptr svreq = ir->make_dyn_req (this);
      CORBA::Exception * ex = new CORBA::TRANSIENT (1, CORBA::COMPLETED_NO);
      svreq->exception (ex);
      return;
    }

  case PortableServer::POAManager::INACTIVE:
    /*
     * The Inactive state is not a temporary state. In a transient POA,
     * we can safely return an OBJECT_NOT_EXIST exception. In a persistent
     * POA, we queue the request. After POA destruction, the special
     * rule above sends a LocationForward to ourselves, eventually
     * causing server reactivation.
     * However, we might be here because we want to call an Adapter
     * Activator, which might be supposed to activate a persistent
     * POA, so we also queue the request if it is not for us.
     */
    {
      if (lifespan_policy->value() == PortableServer::PERSISTENT ||
	  !ir->get_por()->in_poa (oaid.c_str())) {
	InvocationQueue.push_back (InvocationRecord::_duplicate (ir));
      }
      else {
	CORBA::ServerRequest_ptr svreq = ir->make_dyn_req (this);
	CORBA::Exception * ex =
	  new CORBA::OBJ_ADAPTER (1, CORBA::COMPLETED_NO);
	svreq->exception (ex);
      }
      return;
    }

  case PortableServer::POAManager::ACTIVE:
    // exec code below
    break;

  default:
    assert (0);
  }

  /*
   * We are in the active state. If the request is not for us but
   * for a descendant POA, try to call the adapter activator
   */

  if (!ir->get_por()->in_poa (oaid.c_str())) {
    CORBA::String_var cname =
      ir->get_por()->next_descendant_poa (fqn.c_str(), impl_name.c_str());
    POA_impl* poa = NULL;
#ifdef HAVE_EXCEPTIONS
    try {
#endif // HAVE_EXCEPTIONS
      poa = _find_POA (cname, TRUE);
#ifdef HAVE_EXCEPTIONS
    } catch (CORBA::SystemException_catch & sex) {
      CORBA::ServerRequest_ptr svr = ir->make_dyn_req (this);
      CORBA::Exception * ex =
	new CORBA::OBJ_ADAPTER (1, CORBA::COMPLETED_NO);
      svr->exception (ex);
      return;
    }
#endif // HAVE_EXCEPTIONS

    /*
     * Okay, next POA in chain successfully activated
     */

    // adam.stanbridge@roke.co.uk to fix crash on bad iioploc url
    if (!poa) {
      perform_invoke (ir);
      return;
    }

    poa->local_invoke (ir);
    return;
  }

  /*
   * Request for an object in this POA -- Fire away!
   */

  perform_invoke (ir);
}

/*
 * Builtin operations
 */

void
MICOPOA::POA_impl::builtin_is_a (InvocationRecord_ptr ir,
				 PortableServer::Servant serv)
{
  CORBA::ServerRequest_ptr svr = ir->make_dyn_req (this);
  CORBA::Boolean isa = FALSE, r;

  /*
   * Extract repoid
   */

  CORBA::NVList_ptr args = new CORBA::NVList (0);
  const char *repoid;

  args->add (CORBA::ARG_IN);
  args->item(0)->value()->set_type (CORBA::_tc_string);
  if (svr->params (args)) {
    r = (*args->item(0)->value() >>= repoid);
    assert (r);

#ifdef HAVE_EXCEPTIONS
    try {
#endif
      isa = serv->_is_a (repoid);
#ifdef HAVE_EXCEPTIONS
    } catch (CORBA::SystemException_catch & sex) {
      svr->exception (sex->_clone());
      return;
    }
#endif

    CORBA::Any * res = new CORBA::Any;
    *res <<= CORBA::Any::from_boolean (isa);
    svr->result (res);
  }
  else {
    assert (0);
  }
}

void
MICOPOA::POA_impl::builtin_component (InvocationRecord_ptr ir,
				      PortableServer::Servant serv)
{
  CORBA::ServerRequest_ptr svr = ir->make_dyn_req (this);
  CORBA::Object_var comp;

  CORBA::NVList_ptr args = new CORBA::NVList (0);
  if (svr->params (args)) {
#ifdef HAVE_EXCEPTIONS
    try {
#endif
      comp = serv->_get_component ();
#ifdef HAVE_EXCEPTIONS
    } catch (CORBA::SystemException_catch & sex) {
      svr->exception (sex->_clone());
      return;
    }
#endif

    CORBA::Any *res = new CORBA::Any;
    *res <<= comp.in();
    svr->result (res);
  }
  else {
    assert (0);
  }
}

void
MICOPOA::POA_impl::builtin_interface (InvocationRecord_ptr ir,
				      PortableServer::Servant serv)
{
  CORBA::ServerRequest_ptr svr = ir->make_dyn_req (this);
  CORBA::InterfaceDef_var ifd;

  CORBA::NVList_ptr args = new CORBA::NVList (0);
  if (svr->params (args)) {
#ifdef HAVE_EXCEPTIONS
    try {
#endif
      ifd = serv->_get_interface ();
#ifdef HAVE_EXCEPTIONS
    } catch (CORBA::SystemException_catch & sex) {
      svr->exception (sex->_clone());
      return;
    }
#endif

    if (CORBA::is_nil (ifd)) {
      CORBA::Exception * ex =
        new CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO);
      svr->exception (ex);
      return;
    }

    CORBA::Any *res = new CORBA::Any;
    *res <<= ifd.in();
    svr->result (res);
  }
  else {
    assert (0);
  }
}

void
MICOPOA::POA_impl::builtin_non_existent (InvocationRecord_ptr ir,
					 PortableServer::Servant serv)
{
  CORBA::ServerRequest_ptr svr = ir->make_dyn_req (this);
  CORBA::Boolean non_existent=TRUE;

  CORBA::NVList_ptr args = new CORBA::NVList (0);
  if (svr->params (args)) {
#ifdef HAVE_EXCEPTIONS
    try {
#endif
      non_existent = serv->_non_existent ();
#ifdef HAVE_EXCEPTIONS
    } catch (CORBA::SystemException_catch & sex) {
      svr->exception (sex->_clone());
      return;
    }
#endif

    CORBA::Any * res = new CORBA::Any;
    *res <<= CORBA::Any::from_boolean (non_existent);
    svr->result (res);
  }
  else {
    assert (0);
  }
}

bool
MICOPOA::POA_impl::builtin_invoke (InvocationRecord_ptr ir,
				   PortableServer::Servant serv)
{
  const char * opname = ir->get_or()->op_name();

#ifdef HAVE_EXCEPTIONS
  try {
#endif
    if (strcmp (opname, "_is_a") == 0) {
      builtin_is_a (ir, serv);
      return true;
    }
    else if (strcmp (opname, "_interface") == 0) {
      builtin_interface (ir, serv);
      return true;
    }
    else if (strcmp (opname, "_component") == 0) {
      builtin_component (ir, serv);
      return true;
    }
    else if (strcmp (opname, "_non_existent") == 0) {
      builtin_non_existent (ir, serv);
      return true;
    }
    
    return false;
#ifdef HAVE_EXCEPTIONS
  } catch (CORBA::SystemException_catch & sex) {
      // kcg: here exception should be already set
      // on svr, so no need to do so again
      // in addition we cannot create svr for ir again,
      // since it is already done and we do not have
      // any method how to reuse ir's svr yet -- is it
      // needed at all? I think we can just ignore every
      // system exception here
//      CORBA::ServerRequest_ptr svr = ir->make_dyn_req (this);
//      svr->exception (sex->_clone());
  } catch (...) {
      // kcg: we cannot throw anything from builtin_invoke
      // since we will break some assumption of code flow
      // i.e. current->unset and other code will not be executed
      // in perform_invoke method
      assert(0);
  }
  return true;
#endif
}

bool
MICOPOA::POA_impl::is_builtin (InvocationRecord_ptr ir)
{
  const char * opname = ir->get_or()->op_name();

  if (strcmp (opname, "_is_a")         == 0 ||
      strcmp (opname, "_interface")    == 0 ||
      strcmp (opname, "_component")    == 0 ||
      strcmp (opname, "_non_existent") == 0) {
    return true;
  }

  return false;
}

/*
 * Actually perform the invocation
 */

void
MICOPOA::POA_impl::perform_invoke (InvocationRecord_ptr ir)
{
  ObjectMap::ObjectRecord * orec = NULL;

  PortableServer::Servant servant = NULL;
  PortableServer::ServantLocator_var slo =
    PortableServer::ServantLocator::_nil ();
  PortableServer::ServantLocator::Cookie cookie;

  {
    /*
     * keep anybody from fidling with the servants
     * we can do what ever we what with them, nobody
     * is going do modify then now
     */

    MICOMT::AutoLock l(ObjectActivationLock);

    /*
     * If RETAIN, search the active map first
     */

    if (servant_retention_policy->value() == PortableServer::RETAIN) {
      orec = ActiveObjectMap.find (*ir->get_por());
      
      if (orec && orec->active) {
	assert (orec->serv);
	servant = orec->serv;
	// avoid generating real IOR in make_ref() ...
	ir->get_por()->set_ref (orec->por->get_ref());
      }
    }

    /*
     * If USE_DEFAULT_SERVANT, invoke it
     */
    
    if (!servant && request_processing_policy->value() == PortableServer::USE_DEFAULT_SERVANT) {
      if (!default_servant) {
	CORBA::ServerRequest_ptr svr = ir->make_dyn_req (this);
	CORBA::Exception * ex =
	  new CORBA::OBJ_ADAPTER (3, CORBA::COMPLETED_NO);
	svr->exception (ex);
      }
      
      servant = default_servant;
    }

    /*
     * If USE_SERVANT_MANAGER, ask it to incarnate a servant
     */
    
    if (!servant && request_processing_policy->value() == PortableServer::USE_SERVANT_MANAGER) {
      /*
       * The type of Servant Manager depends on the retention policy.
       * RETAIN:     ServantActivator
       * NON_RETAIN: ServantLocator
       */
      
      if (servant_retention_policy->value() == PortableServer::RETAIN) {
	PortableServer::ServantActivator_var sav =
	  PortableServer::ServantActivator::_narrow (servant_manager);
	
	if (CORBA::is_nil (sav)) {
	  CORBA::ServerRequest_ptr svr = ir->make_dyn_req (this);
	  CORBA::Exception * ex =
	    new CORBA::OBJ_ADAPTER (4, CORBA::COMPLETED_NO);
	  svr->exception (ex);
	  return;
	}
	
#ifdef HAVE_EXCEPTIONS
	try {
#endif
	  if (thread_policy->value() == PortableServer::SINGLE_THREAD_MODEL
	      ||thread_policy->value() == PortableServer::MAIN_THREAD_MODEL) {
	    // This solution is not optimal but reliable
	    // for all POAs with single-thread policy
	    // where we will lock global servant manager lock
	    // On the other hand this solution is optimal
	    // for main-thread policy
	    MICOMT::AutoLock t_lock(S_servant_manager_lock);
	    servant = sav->incarnate (ir->get_por()->get_id(), this);
	  }
	  else {
	    servant = sav->incarnate (ir->get_por()->get_id(), this);
	  }
#ifdef HAVE_EXCEPTIONS
	} catch (PortableServer::ForwardRequest_catch & fwr) {
	  orb->answer_invoke (ir->id(), CORBA::InvokeForward,
			      fwr->forward_reference,
			      ir->get_or(), 0);
	  return;
	} catch (CORBA::SystemException_catch & sex1) {
	  CORBA::ServerRequest_ptr svr = ir->make_dyn_req (this);
	  svr->exception (sex1->_clone());
	  return;
	}
#endif
	
	if (servant == NULL) {
	  CORBA::ServerRequest_ptr svr = ir->make_dyn_req (this);
	  CORBA::Exception * ex =
	    new CORBA::OBJ_ADAPTER (3, CORBA::COMPLETED_NO);
	  svr->exception (ex);
	  return;
	}

	if (id_uniqueness_policy->value() == PortableServer::UNIQUE_ID) {
	  //
	  // Is servant already active? => error
	  //
	  {
	    MICOMT::AutoLock t_lock(ObjectActivationLock);
	    if (ActiveObjectMap.exists(servant)) {
	      CORBA::ServerRequest_ptr svr = ir->make_dyn_req (this);
	      CORBA::Exception * ex =
		new CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO);
	      svr->exception (ex);
	      return;
	    }
	  }
	}
	/*
	 * Activate new servant and place in Active Object Map
	 */
	
#ifdef HAVE_EXCEPTIONS
	try {
#endif
	  activate_object_with_id (ir->get_por()->get_id(), servant);
	  orec = ActiveObjectMap.find (*ir->get_por());
#ifdef HAVE_EXCEPTIONS
	} catch (...) {
	  servant->_remove_ref ();
	  throw;
	}
#endif
      }
      else {
	slo = PortableServer::ServantLocator::_narrow (servant_manager);
	
	if (CORBA::is_nil (slo)) {
	  CORBA::ServerRequest_ptr svr = ir->make_dyn_req (this);
	  CORBA::Exception * ex =
	    new CORBA::OBJ_ADAPTER (4, CORBA::COMPLETED_NO);
	  svr->exception (ex);
	  return;
	}

#ifdef HAVE_EXCEPTIONS
	try {
#endif
	  if (thread_policy->value() == PortableServer::SINGLE_THREAD_MODEL
	      ||thread_policy->value() == PortableServer::MAIN_THREAD_MODEL) {
	    // not optimal but reliable solution
	    // for all POAs with single-thread policy
	    // we will lock global servant manager lock
	    // for main-thread policy this solution is optimal
	    // and right
	    MICOMT::AutoLock t_lock(S_servant_manager_lock);
	    servant = slo->preinvoke (ir->get_por()->get_id(), this,
				      ir->get_or()->op_name(), cookie);
	  }
	  else {
	    servant = slo->preinvoke (ir->get_por()->get_id(), this,
				      ir->get_or()->op_name(), cookie);
	  }
#ifdef HAVE_EXCEPTIONS
      } catch (PortableServer::ForwardRequest_catch & fwr) {
	orb->answer_invoke (ir->id(), CORBA::InvokeForward,
			    fwr->forward_reference,
			    ir->get_or(), 0);
	return;
      } catch (CORBA::SystemException_catch & sex2) {
	CORBA::ServerRequest_ptr svr = ir->make_dyn_req (this);
	svr->exception (sex2->_clone());
	return;
      }
#endif

	if (servant == NULL) {
	  CORBA::ServerRequest_ptr svr = ir->make_dyn_req (this);
	  CORBA::Exception * ex =
	    new CORBA::OBJ_ADAPTER (3, CORBA::COMPLETED_NO);
	  svr->exception (ex);
	  return;
	}
      }
    }

    if (!servant) {
      /*
       * Cannot serve the request
       */
      
      CORBA::ServerRequest_ptr svr = ir->make_dyn_req (this);
      CORBA::Exception * ex =
	new CORBA::OBJECT_NOT_EXIST (2, CORBA::COMPLETED_NO);
      svr->exception (ex);
      return;
    }

    //FIXME: make sure, nobody can remove the servant now, until
    //       we are finished

    //this make sure nobody is going to remove the object
    if (servant_retention_policy->value() == PortableServer::RETAIN) {
	orec->invoke_cnt++;
    }
  } // end autolock

  /*
   * we have a valid, activated servant at this point
   */

  current->set (this, ir->get_por(), servant);

  /*
   * Handle builtin operations or perform the invocation
   */

  if (!builtin_invoke (ir, servant)) {
    CORBA::ServerRequestBase_ptr svr = ir->make_req (this, servant);
    if (thread_policy->value() == PortableServer::MAIN_THREAD_MODEL) {
      MICOMT::AutoLock t_lock(S_global_invoke_lock);
      servant->doinvoke(svr);
    } else if (thread_policy->value() == PortableServer::SINGLE_THREAD_MODEL) {
      MICOMT::AutoLock l(serialize_invoke);
      servant->doinvoke (svr);
    } else {
      servant->doinvoke (svr);
    }
  }

  current->unset ();

  /*
   * Cleanup necessary with ServantLocator
   */

  if (!CORBA::is_nil (slo)) {
    if (thread_policy->value() == PortableServer::SINGLE_THREAD_MODEL
	||thread_policy->value() == PortableServer::MAIN_THREAD_MODEL) {
      // not optimal but reliable solution
      // for all POAs with single-thread policy
      // we will lock global servant manager lock
      // for main-thread policy this solution is optimal
      // and right
      MICOMT::AutoLock t_lock(S_servant_manager_lock);
      slo->postinvoke (ir->get_por()->get_id(), this,
		       ir->get_or()->op_name(), cookie,
		       servant);
    }
    else {
      slo->postinvoke (ir->get_por()->get_id(), this,
		       ir->get_or()->op_name(), cookie,
		       servant);
    }
  }
  
  if (servant_retention_policy->value() == PortableServer::RETAIN) {
    MICOMT::AutoLock l(ObjectActivationLock);
    orec->invoke_cnt--;
    if (orec->invoke_cnt == 0 && !orec->active)
      this->remove_object(ir->get_por()->get_id());
  }
  //FIXME: release the servant
}

CORBA::Boolean
MICOPOA::POA_impl::bind (CORBA::ORBMsgId id, const char *repoid,
			 const CORBA::ORB::ObjectTag &oid,
			 CORBA::Address *addr)
{
  if (addr && !addr->is_local()) {
    return FALSE;
  }

  /*
   * We only bind to active objects
   */

  if (servant_retention_policy->value() == PortableServer::RETAIN) {
    MICOMT::AutoLock l(ObjectActivationLock);
    for (ObjectMap::iterator it = ActiveObjectMap.begin ();
	 it != ActiveObjectMap.end ();
	 it++) {
      ObjectMap::ObjectRecord * orec = (*it).second;

      /*
       * Try _is_a; if that doesn't work (maybe because it's a DSI
       * interface), check the primary interface
       */

      if (!orec->serv->_is_a (repoid)) {
	CORBA::String_var pif =
	  orec->serv->_primary_interface (orec->por->get_id(), this);
	if (strcmp (repoid, pif) != 0) {
	  continue;
	}
      }

      /*
       * If the client sent an OID, compare them
       */

      if (oid.length() == 0 || oid == orec->por->get_id()) {
	orb->answer_bind (id, CORBA::LocateHere, orec->por->get_ref());
	return TRUE;
      }
    }
  }
  
  /*
   * Ask child POA's
   */

  POAMap::iterator it = children.begin ();

  while (it != children.end()) {
    if ((*it).second->bind (id, repoid, oid, addr)) {
      return TRUE;
    }
    it++;
  }

  return FALSE;
}

CORBA::Boolean
MICOPOA::POA_impl::locate (CORBA::ORBMsgId id, CORBA::Object_ptr obj)
{
  orb->answer_locate (id, CORBA::LocateHere, CORBA::Object::_nil(), 0);
  return TRUE;
}

/*
 * skeleton() calls the servant's generated _make_stub method which
 * returns a POA-specific skeleton that invokes the servant directly
 * after asking the POA whether the invocation may proceed
 */

CORBA::Object_ptr
MICOPOA::POA_impl::skeleton (CORBA::Object_ptr obj)
{
  assert (this == PortableServer::_the_root_poa);

  POAObjectReference por (this, obj);
  assert (por.is_legal());

  POAMap::iterator it = AllPOAs.find (por.poa_name());

  if (it == AllPOAs.end()) {
    return CORBA::Object::_nil ();
  }

  /*
   * Check the POA's Active Object Map
   */

  POA_impl * poa = ((*it).second);
  {
    MICOMT::AutoLock l(poa->ObjectActivationLock);

    ObjectMap::ObjectRecord * orec = poa->ActiveObjectMap.find (poa, obj);

    if (orec) {
      return orec->serv->_make_stub (poa, obj);
    }
  }

  if (poa->request_processing_policy->value() == PortableServer::USE_DEFAULT_SERVANT && poa->default_servant) {
    return poa->default_servant->_make_stub (poa, obj);
  }

  return CORBA::Object::_nil ();
}

void
MICOPOA::POA_impl::cancel (CORBA::ORBMsgId id)
{
  vector<InvocationRecord *>::iterator it = InvocationQueue.begin ();

  while (it != InvocationQueue.end()) {
    if (CORBA::ORB::get_msgid((*it)->id()) == CORBA::ORB::get_msgid(id) ) {
      delete *it;
      InvocationQueue.erase (it);
      return;
    }
    it++;
  }

  /*
   * If we got here, it's probably meant for one of our children.
   * Unfortunately, we don't know which one.
   */

  POAMap::iterator child = children.begin ();

  while (child != children.end()) {
    (*child).second->cancel (id);
    child++;
  }
}

void
MICOPOA::POA_impl::shutdown (CORBA::Boolean wait_for_completion)
{
  destroy (TRUE, wait_for_completion);
  orb->answer_shutdown (this);
}

void
MICOPOA::POA_impl::answer_invoke (CORBA::ORBMsgId id,
				  CORBA::Object_ptr obj,
				  CORBA::ORBRequest * oreq,
				  CORBA::InvokeStatus stat)
{
  orb->answer_invoke (id, stat, CORBA::Object::_nil(), oreq, 0);
  //  orb->answer_invoke (msgid, stat, CORBA::Object::_nil(), oreq, 0);
}

#ifdef USE_CSL2
// ODM
void
MICOPOA::POA_impl::register_ODM_factory(ObjectDomainMapping::Factory_ptr fry)
{
#ifdef HAVE_SSL
    odm_factory_ = dynamic_cast<MICOSODM::Factory_impl*>(fry);
    odm_manager_ = dynamic_cast<MICOSODM::Manager_impl*>
	(odm_factory_->create());
    string key;
    string key2, tstr;
    PortableServer::POA_ptr poa = this;
    if (lifespan_policy->value() == PortableServer::TRANSIENT) { // transient poa
	while (poa != NULL) {
	    // kcg: memleak: returned name is copied into tstr
	    // but not deleted
	    tstr = poa->the_name();
	    if (key2.length() > 0)
		tstr += '/';
	    tstr += key2;
	    key2 = tstr;
	    poa = poa->the_parent();
	}
	key = key2;

    }
    else { // persistent
	//		if (fqn.length() > 0)  // has normal name
	//			key = fqn;
	//		else
	//			key = oaid;
	while (poa != NULL) {
	    // kcg: memleak: returned name is copied into tstr
	    // but not deleted
	    tstr = poa->the_name();
	    if (tstr == "RootPOA")
		break;
	    if (key2.length() > 0)
		tstr += '/';
	    tstr += key2;
	    key2 = tstr;
	    poa = poa->the_parent();
	}
	key = key2;
    }
    MICOSODM::set_lspolicy(odm_manager_, lifespan_policy->value());
    MICOSODM::DomainMap::iterator it = (odm_factory_->domains_->get_domain_map()).find(key);
    if (it != (odm_factory_->domains_->get_domain_map()).end()) { // mapped to domain
	MICOSODM::ODMRecord* rec = (*it).second;
	rec->dm_ = odm_manager_;
    }
    string defkey = "/";
    it = (odm_factory_->domains_->get_domain_map()).find(defkey);
    if (it != (odm_factory_->domains_->get_domain_map()).end())  // found default
	odm_manager_->set_default_parent_odm(odm_manager_);
#endif // HAVE_SSL
}

ObjectDomainMapping::Manager_ptr
MICOPOA::POA_impl::get_ODM()
{
#ifdef HAVE_SSL
    return ObjectDomainMapping::Manager::_duplicate(odm_manager_);
#else
    return ObjectDomainMapping::Manager::_nil();
#endif		
}
#endif // USE_CSL2
