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
#define MICO_CONF_IR
#define MICO_CONF_POA
#define MICO_CONF_INTERCEPT

#include <CORBA-SMALL.h>
#include <mico/impl.h>
#include <mico/throw.h>
#include <mico/poa_impl.h>
#include <mico/template_impl.h>
#include <mico/pi_impl.h>

#endif // FAST_PCH

MICO::Policy_impl::Policy_impl (CORBA::PolicyType _pt)
{
    pt = _pt;
}

MICO::Policy_impl::~Policy_impl ()
{
    // nothing to do
}

CORBA::PolicyType
MICO::Policy_impl::policy_type ()
{
    return pt;
}

void
MICO::Policy_impl::destroy ()
{
    // nothing to do
}


//-------------------


MICO::DomainManager_impl::DomainManager_impl ()
{
    _policies.length (1);
    _policies[0] = new MICO::ConstructionPolicy_impl;
}

MICO::DomainManager_impl::~DomainManager_impl ()
{
}

CORBA::Policy_ptr
MICO::DomainManager_impl::get_domain_policy (CORBA::PolicyType policy_type)
{
    // look only at this domain's policies
    for (CORBA::ULong i0 = 0; i0 < _policies.length(); ++i0) {
	if (_policies[i0]->policy_type() == policy_type)
	    return CORBA::Policy::_duplicate (_policies[i0]);
    }
    mico_throw (CORBA::INV_POLICY());
    return CORBA::Policy::_nil();
}

void
MICO::DomainManager_impl::set_domain_policy (CORBA::Policy_ptr policy)
{
    CORBA::PolicyType policy_type = policy->policy_type();
    for (CORBA::ULong i0 = 0; i0 < _policies.length(); ++i0) {
	if (_policies[i0]->policy_type() == policy_type) {
	    _policies[i0] = CORBA::Policy::_duplicate (policy);
	    return;
	}
    }
    _policies.length (_policies.length()+1);
    _policies[_policies.length()-1] = CORBA::Policy::_duplicate (policy);
}

CORBA::DomainManager_ptr
MICO::DomainManager_impl::copy ()
{
    DomainManager_impl *d = new DomainManager_impl;
    d->_managers = _managers;

    d->_policies.length (_policies.length());
    for (CORBA::ULong i = 0; i < _policies.length(); ++i)
	d->_policies[i] = _policies[i]->copy();

    return d;
}


//-------------------


MICO::ConstructionPolicy_impl::ConstructionPolicy_impl ()
    : MICO::Policy_impl(CORBA::SecConstruction)
{
}

MICO::ConstructionPolicy_impl::~ConstructionPolicy_impl ()
{
}

void
MICO::ConstructionPolicy_impl::make_domain_manager (
    CORBA::InterfaceDef_ptr in,
    CORBA::Boolean constr_policy)
{
    CORBA::InterfaceDef::FullInterfaceDescription_var idef =
	in->describe_interface();

    _constr[idef->id.in()] = constr_policy;
}

CORBA::Boolean
MICO::ConstructionPolicy_impl::constr_policy (CORBA::Object_ptr object)
{
    IdConstrMap::iterator i = _constr.find (object->_repoid());
    if (i != _constr.end())
	return (*i).second;
    return FALSE;
}

CORBA::Policy_ptr
MICO::ConstructionPolicy_impl::copy ()
{
    MICO::ConstructionPolicy_impl *p = new MICO::ConstructionPolicy_impl;
    p->_constr = _constr;
    return p;
}


//-------------------


MICO::TransportPrefPolicy_impl::TransportPrefPolicy_impl (
    const ProfileTagSeq &prefs)
    : MICO::Policy_impl(MICOPolicy::TRANSPORTPREF_POLICY_TYPE)
{
    _prefs = prefs;
}

MICO::TransportPrefPolicy_impl::TransportPrefPolicy_impl ()
    : MICO::Policy_impl(MICOPolicy::TRANSPORTPREF_POLICY_TYPE)
{
}

MICO::TransportPrefPolicy_impl::~TransportPrefPolicy_impl ()
{
}

MICOPolicy::TransportPrefPolicy::ProfileTagSeq *
MICO::TransportPrefPolicy_impl::preferences()
{
    return new ProfileTagSeq (_prefs);
}

void
MICO::TransportPrefPolicy_impl::preferences (const ProfileTagSeq &prefs)
{
    _prefs = prefs;
}

MICOPolicy::TransportPrefPolicy::ProfileTagSeq *
MICO::TransportPrefPolicy_impl::preferences_nocopy()
{
    // no copy ...
    return &_prefs;
}

CORBA::Policy_ptr
MICO::TransportPrefPolicy_impl::copy ()
{
    MICO::TransportPrefPolicy_impl *p = new MICO::TransportPrefPolicy_impl;
    p->_prefs = _prefs;
    return p;
}


//-------------------


MICO::BidirectionalPolicy_impl::BidirectionalPolicy_impl (
    BiDirPolicy::BidirectionalPolicyValue value)
    : MICO::Policy_impl(BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE)
{
    _value = value;
}

MICO::BidirectionalPolicy_impl::BidirectionalPolicy_impl ()
    : MICO::Policy_impl(BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE)
{
    _value = BiDirPolicy::NORMAL;
}

MICO::BidirectionalPolicy_impl::~BidirectionalPolicy_impl ()
{
}

BiDirPolicy::BidirectionalPolicyValue
MICO::BidirectionalPolicy_impl::value ()
{
    return _value;
}

CORBA::Policy_ptr
MICO::BidirectionalPolicy_impl::copy ()
{
    MICO::BidirectionalPolicy_impl *p = new MICO::BidirectionalPolicy_impl ();
    p->_value = _value;
    return p;
}


//-------------------


CORBA::Policy_ptr
CORBA::ORB::create_policy (CORBA::PolicyType type, const CORBA::Any &any)
{
  if (type == PortableServer::THREAD_POLICY_ID) {
    PortableServer::ThreadPolicyValue val;
    if (!(any >>= val))
      mico_throw (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE));
    return new MICOPOA::ThreadPolicy_impl (type, val);
  }
  else if (type == PortableServer::LIFESPAN_POLICY_ID) {
    PortableServer::LifespanPolicyValue val;
    if (!(any >>= val))
      mico_throw (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE));
    return new MICOPOA::LifespanPolicy_impl (type, val);
  }
  else if (type == PortableServer::ID_UNIQUENESS_POLICY_ID) {
    PortableServer::IdUniquenessPolicyValue val;
    if (!(any >>= val))
      mico_throw (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE));
    return new MICOPOA::IdUniquenessPolicy_impl (type, val);
  }
  else if (type == PortableServer::ID_ASSIGNMENT_POLICY_ID) {
    PortableServer::IdAssignmentPolicyValue val;
    if (!(any >>= val))
      mico_throw (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE));
    return new MICOPOA::IdAssignmentPolicy_impl (type, val);
  }
  else if (type == PortableServer::IMPLICIT_ACTIVATION_POLICY_ID) {
    PortableServer::ImplicitActivationPolicyValue val;
    if (!(any >>= val))
      mico_throw (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE));
    return new MICOPOA::ImplicitActivationPolicy_impl (type, val);
  }
  else if (type == PortableServer::SERVANT_RETENTION_POLICY_ID) {
    PortableServer::ServantRetentionPolicyValue val;
    if (!(any >>= val))
      mico_throw (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE));
    return new MICOPOA::ServantRetentionPolicy_impl (type, val);
  }
  else if (type == PortableServer::REQUEST_PROCESSING_POLICY_ID) {
    PortableServer::RequestProcessingPolicyValue val;
    if (!(any >>= val))
      mico_throw (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE));
    return new MICOPOA::RequestProcessingPolicy_impl (type, val);
  }
  else if (type == MICOPolicy::TRANSPORTPREF_POLICY_TYPE) {
    MICOPolicy::TransportPrefPolicy::ProfileTagSeq val;
    if (!(any >>= val))
      mico_throw (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE));
    return new MICO::TransportPrefPolicy_impl (val);
  }
  else if (type == BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE) {
    BiDirPolicy::BidirectionalPolicyValue val;
    if (!(any >>= val))
      mico_throw (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE));
    return new MICO::BidirectionalPolicy_impl (val);
  }
  else if (PInterceptor::PI::S_pfmap_.find(type)
	   != PInterceptor::PI::S_pfmap_.end()) {
    return PInterceptor::PI::S_pfmap_[type]->create_policy
      (type, any); // see orbos/99-12-02 p. 8-67
  }
  mico_throw (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE));
  // Make Sun CC happy
  return CORBA::Policy::_nil ();
}
