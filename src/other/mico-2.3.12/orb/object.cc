/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
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

#ifdef FAST_PCH
#include "orb_pch.h"
#endif // FAST_PCH
#ifdef __COMO__
#pragma hdrstop
#endif // __COMO__

#ifndef FAST_PCH

#define MICO_CONF_IMR

#include <CORBA-SMALL.h>
#ifndef _WIN32
#include <string.h>
#endif
#include <mico/template_impl.h>
#include <mico/util.h>
#include <mico/throw.h>
#ifdef USE_MEMTRACE
#include <mico/memtrace.h>
#endif
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <sstream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <strstream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS

#endif // FAST_PCH


using namespace std;

/*************************** MagicChecker ****************************/


void
CORBA::MagicChecker::_check () const
{
    if (!this || magic != MICO_OBJ_MAGIC) {
#ifdef USE_MEMTRACE
	MemTrace_SelfTrace (stderr);
#endif
	mico_throw (BAD_PARAM());
    }
}

void
CORBA::MagicChecker::_check (const CORBA::Exception &ex) const
{
    if (!this || magic != MICO_OBJ_MAGIC) {
#ifdef USE_MEMTRACE
	MemTrace_SelfTrace (stderr);
#endif
	((CORBA::Exception&)ex)._raise();
    }
}

CORBA::Boolean
CORBA::MagicChecker::_check_nothrow () const
{
    if (!this || magic != MICO_OBJ_MAGIC) {
      if (MICO::Logger::IsLogged (MICO::Logger::Warning)) {
	MICO::Logger::Stream (MICO::Logger::Warning)
	  << "invalid object reference" << endl;
      }
#ifdef USE_MEMTRACE
      MemTrace_SelfTrace (stderr);
#endif
      return FALSE;
    }
    return TRUE;
}


/************************* ServerlessObject **************************/


CORBA::ServerlessObject::~ServerlessObject ()
{
}

void
CORBA::ServerlessObject::_ref ()
{
    MICOMT::AutoLock lock(refslock);
    _check ();
    ++refs;
}

CORBA::Boolean
CORBA::ServerlessObject::_deref ()
{
    MICOMT::AutoLock lock(refslock);
    return _check_nothrow() && --refs <= 0;
}

CORBA::Long
CORBA::ServerlessObject::_refcnt () const
{
    _check ();
    return refs;
}


/****************************** Object *******************************/


// consumes IOR* argument
CORBA::Object::Object (IOR *i)
{
    ior = i;
    fwd_ior = 0;
    orb = CORBA::ORB_instance ("mico-local-orb", FALSE);
    if (!CORBA::is_nil(orb) && !orb->plugged() && ior)
	ior->addressing_disposition (GIOP::ReferenceAddr);
}

CORBA::Object::Object (const Object &o)
{
    ior = o.ior ? new IOR (*o.ior) : 0;
    fwd_ior = o.fwd_ior ? new IOR (*o.fwd_ior) : 0;
    orb = CORBA::ORB::_duplicate (o.orb);
    _managers = o._managers;
    _policies = o._policies;
}

CORBA::Object &
CORBA::Object::operator= (const Object &o)
{
    MICO_OBJ_CHECK (this);

    if (this != &o) {
	if (ior)
	    delete ior;
	ior = o.ior ? new IOR (*o.ior) : 0;
	if (fwd_ior)
	    delete fwd_ior;
	fwd_ior = o.fwd_ior ? new IOR (*o.fwd_ior) : 0;
	CORBA::release (orb);
	orb = CORBA::ORB::_duplicate (o.orb);
	_managers = o._managers;
	_policies = o._policies;
    }
    return *this;
}

CORBA::Object::~Object ()
{
    if (ior)
        delete ior;
    if (fwd_ior)
        delete fwd_ior;
    CORBA::release (orb);
}

void
CORBA::Object::_forward (CORBA::Object_ptr o)
{
    assert (!CORBA::is_nil (o) && o->ior);
    if (fwd_ior)
	delete fwd_ior;
    fwd_ior = new IOR (*o->ior);
}

void
CORBA::Object::_unforward ()
{
    if (fwd_ior) {
	delete fwd_ior;
	fwd_ior = 0;
    }
}

void
CORBA::Object::_setup_domains (CORBA::Object_ptr parent)
{
    if (CORBA::is_nil (parent)) {
	CORBA::DomainManager_var def_manager;
	this->_orbnc()->get_default_domain_manager (def_manager);

	_managers.length (1);

	CORBA::Policy_var p =
	    def_manager->_get_policy (CORBA::SecConstruction);
	assert (!CORBA::is_nil (p));

	CORBA::ConstructionPolicy_var cp =
	    CORBA::ConstructionPolicy::_narrow (p);

	if (cp->constr_policy (this)) {
	    _managers[0] = def_manager->copy();
	} else {
	    _managers[0] = def_manager._retn();
	}
    } else {
	CORBA::DomainManagerList_var dml = parent->_get_domain_managers();
	_managers.length (dml->length());

	for (CORBA::ULong i = 0; i < dml->length(); ++i) {
	    CORBA::Policy_var p =
		dml[i]->_get_policy (CORBA::SecConstruction);
	    assert (!CORBA::is_nil (p));

	    CORBA::ConstructionPolicy_var cp =
		CORBA::ConstructionPolicy::_narrow (p);

	    if (cp->constr_policy (this)) {
		_managers[i] = CORBA::DomainManager::_duplicate (dml[i]);
	    } else {
		_managers[i] = dml[i]->copy();
	    }
	}
    }
}

const char *
CORBA::Object::_ident ()
{
    IORProfile *prof = _ior()->profile (IORProfile::TAG_ANY);
    assert (prof);
    Long len;
    const Octet *key = prof->objectkey (len);
    ident = "";
    for (int i = 0; i < len; ++i) {
	ident += mico_to_xdigit ((key[i] >> 4) & 0xf);
	ident += mico_to_xdigit (key[i] & 0xf);
    }
    return ident.c_str();
}

void *
CORBA::Object::_narrow_helper (const char *)
{
    return NULL;
}

CORBA::ImplementationDef_ptr
CORBA::Object::_get_implementation ()
{
    return _orbnc()->get_impl (this);
}

CORBA::InterfaceDef_ptr
CORBA::Object::_get_interface ()
{
    MICO_OBJ_CHECK2 (this, CORBA::OBJECT_NOT_EXIST());

    return _orbnc()->get_iface (this);
}

CORBA::Object_ptr
CORBA::Object::_get_component ()
{
    MICO_OBJ_CHECK2 (this, CORBA::OBJECT_NOT_EXIST());

    return _orbnc()->get_component (this);
}

void
CORBA::Object::_create_request (Context_ptr ctx,
				const char *op,
				NVList_ptr args,
				NamedValue_ptr result,
				Request_out request,
				Flags flags)
{
    request = new Request (this, ctx, op, args, result, flags);
}

void
CORBA::Object::_create_request (Context_ptr ctx,
				const char *op,
				NVList_ptr args,
				NamedValue_ptr result,
				ExceptionList_ptr elist,
				ContextList_ptr clist,
				Request_out request,
				Flags flags)
{
    request = new Request (this, ctx, op, args, result, elist, clist, flags);
}

CORBA::Request_ptr
CORBA::Object::_request (const char *op)
{
    return new Request (this, op);
}

CORBA::Policy_ptr
CORBA::Object::_get_policy (PolicyType policy_type)
{
    // look at this objects's policies
    for (CORBA::ULong i0 = 0; i0 < _policies.length(); ++i0) {
	if (_policies[i0]->policy_type() == policy_type)
	    return CORBA::Policy::_duplicate (_policies[i0]);
    }
    // look at domain manager policies
    for (CORBA::ULong i1 = 0; i1 < _managers.length(); ++i1) {
	CORBA::Policy_ptr p = _managers[i1]->_get_policy (policy_type);
	if (!CORBA::is_nil (p))
	    return p;
    }
    mico_throw (CORBA::BAD_PARAM());
    return CORBA::Policy::_nil();
}

CORBA::DomainManagerList *
CORBA::Object::_get_domain_managers ()
{
    return new DomainManagerList (_managers);
}

CORBA::Object_ptr
CORBA::Object::_set_policy_overrides (const PolicyList &policies,
				      SetOverrideType set_add)
{
    CORBA::Object_ptr nobj = new CORBA::Object (*this);
    switch (set_add) {
    case CORBA::SET_OVERRIDE:
	nobj->_policies = policies;
	break;

    case CORBA::ADD_OVERRIDE: {
	for (CORBA::ULong i = 0; i < policies.length(); ++i) {
	    CORBA::ULong j;
	    for (j = 0; j < nobj->_policies.length(); ++j) {
		if (nobj->_policies[j]->policy_type() ==
		      policies[i]->policy_type()) {
		    nobj->_policies[j] =
			CORBA::Policy::_duplicate (policies[i].in());
		    break;
		}
	    }
	    if (j == nobj->_policies.length()) {
		nobj->_policies.length (j+1);
		nobj->_policies[j] = CORBA::Policy::_duplicate (policies[i].in());
	    }
	}
	break;
    }
    default:
	assert (0);
    }
    return nobj;
}

CORBA::Boolean
CORBA::Object::_is_a (const char *repoid)
{
    MICO_OBJ_CHECK2 (this, CORBA::OBJECT_NOT_EXIST());

    // check some trivial cases first
    if (!strcmp (repoid, "IDL:omg.org/CORBA/Object:1.0"))
	return TRUE;
    if (ior && !strcmp (repoid, _repoid()))
	return TRUE;

    // try IDL compiler generated narrowing support
    if (_narrow_helper (repoid))
	return TRUE;

    return _is_a_remote (repoid);
}

CORBA::Boolean
CORBA::Object::_is_a_remote (const char *repoid)
{
    // only ask remote if we are a stub
    if (_orbnc()->is_impl (this))
	return FALSE;
    if (!ior)
        return FALSE;
    return _orbnc()->is_a (this, repoid);
}

CORBA::Boolean
CORBA::Object::_non_existent ()
{
    if (CORBA::is_nil (this))
	return FALSE;

    if (!_check_nothrow())
	return TRUE;

    return _orbnc()->non_existent (this);
}

CORBA::Boolean
CORBA::Object::_is_equivalent (Object_ptr o)
{
    if (CORBA::is_nil (this) && CORBA::is_nil (o))
	return TRUE;

    if (!CORBA::is_nil (this))
	MICO_OBJ_CHECK2 (this, CORBA::OBJECT_NOT_EXIST());

    if (!CORBA::is_nil (o))
	MICO_OBJ_CHECK2 (o, CORBA::OBJECT_NOT_EXIST());

    if (CORBA::is_nil (this) || CORBA::is_nil (o))
	return FALSE;

    if (this == o)
	return TRUE;
    return *_ior() == *o->_ior();
}

CORBA::ULong
CORBA::Object::_hash (ULong max)
{
    MICO_OBJ_CHECK2 (this, CORBA::OBJECT_NOT_EXIST());

    string s = _ior()->stringify();
    return mico_string_hash (s.c_str(), max);
}

CORBA::ORB_ptr
CORBA::Object::_orbnc()
{
    if (CORBA::is_nil(orb))
	orb = CORBA::ORB_instance ("mico-local-orb");
    return orb;
}

// ref-counting added in ptc/03-03-09

void
CORBA::LocalObject::_add_ref()
{
    this->_ref();
}

void
CORBA::LocalObject::_remove_ref()
{
    if (this->_deref())
	delete this;
}

CORBA::ULong
CORBA::LocalObject::_refcount_value() const
{
    return this->_refcnt();
}

// non-implemented methods of CORBA::Object

CORBA::InterfaceDef*
CORBA::LocalObject::_get_interface()
{
    mico_throw(CORBA::NO_IMPLEMENT());
    // never reached, just to avoid warning
    return NULL;
}

CORBA::DomainManagerList*
CORBA::LocalObject::_get_domain_managers()
{
    mico_throw(CORBA::NO_IMPLEMENT());
    // never reached, just to avoid warning
    return NULL;
}

CORBA::Policy_ptr
CORBA::LocalObject::_get_policy(PolicyType __policy_type)
{
    mico_throw(CORBA::NO_IMPLEMENT());
    // never reached, just to avoid warning
    return CORBA::Policy::_nil();
}

CORBA::Object_ptr
CORBA::LocalObject::_set_policy_overrides
(const PolicyList& __policies, SetOverrideType __set_add)
{
    mico_throw(CORBA::NO_IMPLEMENT());
    // never reached, just to avoid warning
    return CORBA::Object::_nil();
}

CORBA::ULong
CORBA::LocalObject::_hash(CORBA::ULong __max)
{
    // we simply hash string which represent
    // address of this object e.g '0xbffff5e8'
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
    stringstream __ss;
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
    ostrstream __ss;
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS
    __ss << hex << (void*)this;
    string __str = __ss.str();
    return mico_string_hash(__str.c_str(), __max);
}

CORBA::Boolean
CORBA::LocalObject::_is_equivalent(CORBA::Object_ptr __obj)
{
    return this == __obj;
}

