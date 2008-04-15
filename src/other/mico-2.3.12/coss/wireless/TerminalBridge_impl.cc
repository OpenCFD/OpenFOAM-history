/*
 *  MIWCO --- Wireless CORBA support for MICO (OMG document formal/2004-04-02)
 *            Terminal Bridge implementation
 *  Copyright (C) 2001 Jaakko Kangasharju
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
 */

#include "TerminalBridge_impl.h"

// Implementation for interface TerminalBridge

TerminalBridge_impl::TerminalBridge_impl (CORBA::ORB_ptr orb,
					  WATMProxy::GTPTerminal *proxy,
					  WATMBridge::TerminalBridge *bridge,
					  CosNaming::NamingContext_ptr root)
{
    _orb = orb;
    _root = CosNaming::NamingContext::_duplicate(root);
    _proxy = proxy;
    _bridge = bridge;
    try {
	CORBA::Object_var obj =
	    _orb->resolve_initial_references("HomeLocationAgent");
	_hla = MobileTerminal::HomeLocationAgent::_narrow(obj);
    } catch (const CORBA::ORB::InvalidName &) {
	_hla = MobileTerminal::HomeLocationAgent::_nil();
    }
}

TerminalBridge_impl::~TerminalBridge_impl ()
{
    CORBA::release(_root);
    CORBA::release(_hla);
    delete _proxy;
}

MobileTerminal::ObjectIdList *
TerminalBridge_impl::list_initial_services ()
    throw (CORBA::SystemException)
{
    MobileTerminal::ObjectIdList *retval = new MobileTerminal::ObjectIdList;
    if (!CORBA::is_nil(_root)) {
	retval->length(1);
	(*retval)[0] = CORBA::string_dup("NameService");
    }
    return retval;
}

CORBA::Object_ptr
TerminalBridge_impl::resolve_initial_references (const char *identifier)
    throw (CORBA::SystemException, MobileTerminal::InvalidName)
{
    CORBA::Object_ptr retval;
    if (CORBA::is_nil(_root) || strcmp(identifier, "NameService") != 0) {
	throw MobileTerminal::InvalidName();
    } else {
	retval = CORBA::Object::_duplicate(_root);
    }
    return retval;
}

char *
TerminalBridge_impl::register_profile (CORBA::ULong tag,
				       const MobileTerminal::TerminalBridge::IORProfile &profile)
    throw (CORBA::SystemException,
	   MobileTerminal::TerminalBridge::AddressNotAvailable)
{
    CORBA::Buffer b((void *)profile.get_buffer());
#ifdef _WINDOWS
    MICO::CDRDecoder dc(&b, FALSE, CORBA::DefaultEndian,
			0, TRUE, 0, TRUE);
#else
    MICO::CDRDecoder dc(&b, FALSE);
#endif
    CORBA::IORProfile *prof =
	CORBA::IORProfile::decode_body(dc, tag, profile.length());
    CORBA::Long len;
    const CORBA::Octet *okey = prof->objectkey(len);
    CORBA::OctetSeq *key = new CORBA::OctetSeq(len);
    key->length(len);
    memcpy(key->get_buffer(), okey, len);

    CORBA::IORProfile *rprof = NULL;
    if (!CORBA::is_nil(_hla)) {
	rprof = _hla->_ior()->profile(tag);
    } else {
	MobileTerminal::AccessBridge_ptr ab = _proxy->current_connect_point();
	if (!CORBA::is_nil(ab)) {
	    rprof = ab->_ior()->profile(tag);
	}
    }
    if (rprof != NULL) {
	_bridge->register_object(key, prof);
	return CORBA::string_dup(rprof->addr()->stringify().c_str());
    } else {
	delete prof;
	delete key;
	throw MobileTerminal::TerminalBridge::AddressNotAvailable();
    }
}

void
TerminalBridge_impl::connect (const char *address)
    throw (CORBA::SystemException)
{
    CORBA::Address *addr = CORBA::Address::parse(address);
    if (addr == NULL)
	mico_throw(CORBA::BAD_PARAM());
    _proxy->ensure_connectivity(addr);
}

void
TerminalBridge_impl::release (CORBA::Boolean notify)
    throw (CORBA::SystemException)
{
    _proxy->break_connectivity(notify);
}

void
TerminalBridge_impl::do_handoff (const char *address)
    throw (CORBA::SystemException)
{
    CORBA::Address *addr = CORBA::Address::parse(address);
    if (addr == NULL)
	mico_throw(CORBA::BAD_PARAM());
    _proxy->handoff(addr);
    delete addr;
}
