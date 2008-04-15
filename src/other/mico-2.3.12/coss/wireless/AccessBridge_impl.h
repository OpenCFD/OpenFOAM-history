// -*- c++ -*-
/*
 *  MIWCO --- Wireless CORBA support for MICO (OMG document formal/2004-04-02)
 *            Access Bridge interface
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

#ifndef __ACCESSBRIDGE_IMPL_H__
#define __ACCESSBRIDGE_IMPL_H__

#include <coss/MobileTerminal.h>
#include <coss/CosNaming.h>
#include <coss/CosEventComm.h>
#include <coss/CosEventChannelAdmin.h>
#include <coss/GTP.h>
#include <mico/impl.h>
#include "glue.h"
#include "proxy.h"

// Implementation for interface AccessBridge
class AccessBridge_impl : virtual public POA_MobileTerminal::AccessBridge
{
public:

    MobileTerminal::ObjectIdList *list_initial_services ()
	throw(CORBA::SystemException);

    CORBA::Object_ptr resolve_initial_references (const char *identifier)
	throw(CORBA::SystemException, MobileTerminal::InvalidName);

    CORBA::Boolean
    terminal_attached (const MobileTerminal::TerminalId& terminal_id)
	throw(CORBA::SystemException);

    MobileTerminal::HomeLocationAgent_ptr
    home_location_agent (const MobileTerminal::TerminalId& terminal_id)
	throw(CORBA::SystemException, MobileTerminal::UnknownTerminalId);

    void get_address_info (MobileTerminal::AccessBridgeTransportAddressList_out transport_address_list)
	throw(CORBA::SystemException);

    void start_handoff (const MobileTerminal::TerminalId& terminal_id,
			MobileTerminal::AccessBridge_ptr new_access_bridge,
			MobileTerminal::HandoffCallback_ptr handoff_callback_target)
	throw(CORBA::SystemException);

    void
    transport_address_request (const MobileTerminal::TerminalId& terminal_id,
			       MobileTerminal::AccessBridgeTransportAddressList_out new_access_bridge_addresses,
			       CORBA::Boolean_out terminal_accepted)
	throw(CORBA::SystemException);

    void handoff_completed (const MobileTerminal::TerminalId& terminal_id,
			    MobileTerminal::HandoffStatus status)
	throw(CORBA::SystemException);

    void
    handoff_in_progress (const MobileTerminal::TerminalId& terminal_id,
			 MobileTerminal::AccessBridge_ptr new_access_bridge)
	throw(CORBA::SystemException);

    void
    recovery_request (const MobileTerminal::TerminalId& terminal_id,
		      MobileTerminal::AccessBridge_ptr new_access_bridge,
		      CORBA::UShort highest_gtp_seqno_received_at_terminal,
		      CORBA::UShort_out highest_gtp_seqno_received_at_access_bridge)
	throw(CORBA::SystemException, MobileTerminal::UnknownTerminalId);

    void gtp_to_terminal (const MobileTerminal::TerminalId& terminal_id,
			  MobileTerminal::AccessBridge_ptr old_access_bridge,
			  CORBA::ULong gtp_message_id,
			  const MobileTerminal::GTPEncapsulation& gtp_message)
	throw(CORBA::SystemException, MobileTerminal::TerminalNotHere);

    void
    gtp_from_terminal (const MobileTerminal::TerminalId& terminal_id,
		       CORBA::ULong gtp_message_id,
		       const MobileTerminal::GTPEncapsulation& gtp_message)
	throw(CORBA::SystemException, MobileTerminal::UnknownTerminalId);

    void
    gtp_acknowledge (CORBA::ULong gtp_message_id, CORBA::ULong status);

    void handoff_notice (const MobileTerminal::TerminalId& terminal_id,
			 MobileTerminal::AccessBridge_ptr new_access_bridge)
	throw(CORBA::SystemException);

    void
    subscribe_handoff_notice (const MobileTerminal::TerminalId& terminal_id,
			      MobileTerminal::AccessBridge_ptr interested_access_bridge)
	throw(CORBA::SystemException, MobileTerminal::TerminalNotHere);

    CORBA::Object_ptr mobility_channel_reference ();

    AccessBridge_impl (CORBA::ORB_ptr orb, WATMProxy::GTPNetwork *proxy,
		       CosNaming::NamingContext_ptr root,
		       std::vector<CORBA::Address *> addrs,
		       CosEventChannelAdmin::EventChannel_ptr channel);
    ~AccessBridge_impl ();

private:

    CORBA::ORB_ptr _orb;
    CosNaming::NamingContext_ptr _root;
    WATMProxy::GTPNetwork *_proxy;
    MobileTerminal::AccessBridgeTransportAddressList _addrs;
    CosEventChannelAdmin::EventChannel_ptr _channel;

    struct ic : std::binary_function<MobileTerminal::TerminalId,
		                     MobileTerminal::TerminalId,
		                     bool>
    {
	bool operator() (MobileTerminal::TerminalId id1,
			 MobileTerminal::TerminalId id2) const
	    {
		if (id1.length() != id2.length())
		    return id1.length() < id2.length();
		return mico_key_compare(id1.get_buffer(), id2.get_buffer(),
					id1.length()) < 0;
	    }
    };

    struct HandoffData {
	MobileTerminal::AccessBridge_ptr new_ab;
	MobileTerminal::HandoffCallback_ptr cb;
    };

    typedef std::map<MobileTerminal::TerminalId, HandoffData, ic> MapIdHoff;
    typedef std::list<MobileTerminal::AccessBridge_ptr> ABList;
    typedef std::map<MobileTerminal::TerminalId, ABList, ic> MapIdABs;

    MapIdHoff _handoffs_in_progress;
    MapIdABs _handoff_subscribers;
};


#endif
