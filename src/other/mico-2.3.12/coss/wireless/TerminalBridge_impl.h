// -*- c++ -*-
/*
 *  MIWCO --- Wireless CORBA support for MICO (OMG document formal/2004-04-02)
 *            Terminal Bridge interface
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

#ifndef __TERMINALBRIDGE_IMPL_H__
#define __TERMINALBRIDGE_IMPL_H__

#include <coss/MobileTerminal.h>
#include <coss/CosNaming.h>
#include <coss/CosEventComm.h>
#include <coss/GTP.h>
#include <mico/impl.h>
#include "glue.h"
#include "proxy.h"
#include "bridge.h"

// Implementation for interface TerminalBridge
class TerminalBridge_impl : virtual public POA_MobileTerminal::TerminalBridge
{
public:

    MobileTerminal::ObjectIdList *list_initial_services ()
	throw (CORBA::SystemException);

    CORBA::Object_ptr resolve_initial_references (const char *identifier)
	throw (CORBA::SystemException, MobileTerminal::InvalidName);

    char *register_profile (CORBA::ULong tag, const MobileTerminal::TerminalBridge::IORProfile& profile)
	throw (CORBA::SystemException, MobileTerminal::TerminalBridge::AddressNotAvailable);
    void connect (const char *address)
	throw (CORBA::SystemException);

    void release (CORBA::Boolean notify)
	throw (CORBA::SystemException);

    void do_handoff (const char *address)
	throw (CORBA::SystemException);

    TerminalBridge_impl (CORBA::ORB_ptr orb, WATMProxy::GTPTerminal *proxy,
			 WATMBridge::TerminalBridge *bridge,
			 CosNaming::NamingContext_ptr root);

    ~TerminalBridge_impl ();

private:

    CORBA::ORB_ptr _orb;
    CosNaming::NamingContext_ptr _root;
    WATMProxy::GTPTerminal *_proxy;
    WATMBridge::TerminalBridge *_bridge;
    MobileTerminal::HomeLocationAgent_ptr _hla;
};


#endif
