/*
 *  MIWCO --- Wireless CORBA support for MICO (OMG document formal/2004-04-02)
 *            Access Bridge implementation
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

#include "AccessBridge_impl.h"
#ifdef HAVE_BLUETOOTH
#include <mico/ltp.h>
#endif

using namespace std;

// Implementation for interface AccessBridge

AccessBridge_impl::AccessBridge_impl (CORBA::ORB_ptr orb,
				      WATMProxy::GTPNetwork *proxy,
				      CosNaming::NamingContext_ptr root,
				      vector<CORBA::Address *> addrs,
				      CosEventChannelAdmin::EventChannel_ptr channel)
{
    _orb = orb;
    _root = CosNaming::NamingContext::_duplicate(root);
    _proxy = proxy;
    _channel = CosEventChannelAdmin::EventChannel::_duplicate(channel);

    _addrs.length(addrs.size());
    for (mico_vec_size_type i = 0; i < addrs.size(); ++i) {
	MobileTerminal::AccessBridgeTransportAddress real_addr;
	MobileTerminal::GTPInfo gtp_info;
	gtp_info.protocol_level = 0x02;
	gtp_info.gtp_version.major = 0x01;
	gtp_info.gtp_version.minor = 0x00;
	real_addr.tunneling_protocol = gtp_info;
	const char *proto = addrs[i]->proto();
	if (strcmp(proto, "inet") == 0 || strcmp(proto, "inet-stream") == 0) {
	    real_addr.tunneling_protocol.protocol_id =
		MobileTerminal::TCP_TUNNELING;
	    MICO::InetAddress *iaddr =
		dynamic_cast<MICO::InetAddress *>(addrs[i]);
	    assert(iaddr != NULL);
	    string addr_str = iaddr->host();
	    addr_str += ":";
	    addr_str += xdec(iaddr->port());
	    real_addr.transport_address.length(addr_str.length());
	    memcpy(real_addr.transport_address.get_buffer(), addr_str.c_str(),
		   addr_str.length());
	} else if (strcmp(proto, "utp") == 0) {
	    real_addr.tunneling_protocol.protocol_id =
		MobileTerminal::UDP_TUNNELING;
	    // not implemented
	    assert(0);
	} else if (strcmp(proto, "waptp") == 0) {
	    real_addr.tunneling_protocol.protocol_id =
		MobileTerminal::WAP_TUNNELING;
	    // not implemented
	    assert(0);
#ifdef HAVE_BLUETOOTH
	} else if (strcmp(proto, "bt-l2cap") == 0) {
	    real_addr.tunneling_protocol.protocol_id =
		MobileTerminal::L2CAP_TUNNELING;
	    MICOLTP::LTPAddress *l2addr =
		dynamic_cast<MICOLTP::LTPAddress *>(addrs[i]);
	    assert(l2addr != NULL);
	    string addr_str = l2addr->bthost();
	    addr_str += "#";
	    addr_str += xdec(l2addr->psm());
	    real_addr.transport_address.length(addr_str.length());
	    memcpy(real_addr.transport_address.get_buffer(), addr_str.c_str(),
		   addr_str.length());
#endif
	} else { // add new transports here
	    assert(0);
	}
	_addrs[i] = real_addr;
    }
}

AccessBridge_impl::~AccessBridge_impl ()
{
    delete _proxy;
    CORBA::release(_root);
}

MobileTerminal::ObjectIdList*
AccessBridge_impl::list_initial_services ()
    throw(CORBA::SystemException)
{
    MobileTerminal::ObjectIdList* retval = new MobileTerminal::ObjectIdList;
    if (!CORBA::is_nil(_root)) {
	retval->length(1);
	(*retval)[0] = CORBA::string_dup("NameService");
    }
    return retval; 
}


CORBA::Object_ptr
AccessBridge_impl::resolve_initial_references (const char *identifier)
    throw(CORBA::SystemException, MobileTerminal::InvalidName)
{
    CORBA::Object_ptr retval;

    if (CORBA::is_nil(_root) || strcmp(identifier, "NameService") != 0) {
	throw MobileTerminal::InvalidName();
    } else {
	retval = CORBA::Object::_duplicate(_root);
    }
    return retval; 
}


CORBA::Boolean
AccessBridge_impl::terminal_attached
(const MobileTerminal::TerminalId& terminal_id)
    throw(CORBA::SystemException)
{
    return _proxy->is_here(terminal_id);
}


MobileTerminal::HomeLocationAgent_ptr
AccessBridge_impl::home_location_agent
(const MobileTerminal::TerminalId& terminal_id)
    throw(CORBA::SystemException, MobileTerminal::UnknownTerminalId)
{
    MobileTerminal::HomeLocationAgent_ptr retval =
	_proxy->terminal_home(terminal_id);
    if (CORBA::is_nil(retval))
	throw MobileTerminal::UnknownTerminalId();
    return MobileTerminal::HomeLocationAgent::_duplicate(retval);
}


void
AccessBridge_impl::get_address_info
(MobileTerminal::AccessBridgeTransportAddressList_out transport_address_list)
    throw(CORBA::SystemException)
{
    transport_address_list =
	new MobileTerminal::AccessBridgeTransportAddressList(_addrs);
}


/*
 * Called by external control to initiate handoff procedure of terminal_id
 * to new_access_bridge.  After handoff status is reported by calling
 * handoff_callback_target->report_handoff_status(snafu).
 */
void
AccessBridge_impl::start_handoff
(const MobileTerminal::TerminalId& terminal_id,
 MobileTerminal::AccessBridge_ptr new_access_bridge,
 MobileTerminal::HandoffCallback_ptr handoff_callback_target )
    throw(CORBA::SystemException)
{
    CORBA::Boolean ok;
    MobileTerminal::AccessBridgeTransportAddressList *new_addrs;

    if (_this()->_is_equivalent(new_access_bridge)) {
	if (!CORBA::is_nil(handoff_callback_target)) {
	    handoff_callback_target->report_handoff_status(MobileTerminal::HANDOFF_SUCCESS);
	}
	return;
    }

    new_access_bridge->transport_address_request(terminal_id, new_addrs, ok);
    if (!ok) {
	if (!CORBA::is_nil(handoff_callback_target)) {
	    handoff_callback_target->report_handoff_status(MobileTerminal::HANDOFF_FAILURE);
	}
	return;
    }
    if (!_proxy->do_handoff(terminal_id, new_addrs)) {
	if (!CORBA::is_nil(handoff_callback_target)) {
	    handoff_callback_target->report_handoff_status(MobileTerminal::HANDOFF_FAILURE);
	}
	return;
    }
    HandoffData data;
    data.new_ab = MobileTerminal::AccessBridge::_duplicate(new_access_bridge);
    data.cb =
	MobileTerminal::HandoffCallback::_duplicate(handoff_callback_target);
    _handoffs_in_progress[terminal_id] = data;
}


/*
 * Called at start of handoff by old access bridge to get all the addresses
 * terminal_id can connect to.
 */
void
AccessBridge_impl::transport_address_request
(const MobileTerminal::TerminalId& terminal_id,
 MobileTerminal::AccessBridgeTransportAddressList_out new_access_bridge_addresses,
 CORBA::Boolean_out terminal_accepted)
    throw(CORBA::SystemException)
{
    new_access_bridge_addresses =
	new MobileTerminal::AccessBridgeTransportAddressList(_addrs);
    terminal_accepted = TRUE;
}


/*
 * Called by new access bridge to inform that handoff of terminal_id is
 * done and old access bridge can release things related to terminal_id.
 */
void
AccessBridge_impl::handoff_completed
(const MobileTerminal::TerminalId& terminal_id,
 MobileTerminal::HandoffStatus status)
    throw(CORBA::SystemException)
{
    HandoffData data = _handoffs_in_progress[terminal_id];
    for (ABList::iterator i = _handoff_subscribers[terminal_id].begin();
	 i != _handoff_subscribers[terminal_id].end(); ++i) {
	(*i)->handoff_notice(terminal_id, data.new_ab);
	CORBA::release(*i);
    }
    _handoff_subscribers.erase(terminal_id);
    _proxy->terminal_moved(terminal_id, data.new_ab);
    _proxy->mobility_event(WATMProxy::NET_DEPART, terminal_id, data.new_ab);
    if (!CORBA::is_nil(data.cb)) {
	data.cb->report_handoff_status(status);
    }
    CORBA::release(data.new_ab);
    CORBA::release(data.cb);
    _handoffs_in_progress.erase(terminal_id);
}


/*
 * Called by new access bridge at old access bridge in response to
 * terminal_id wanting to handoff.
 */
void
AccessBridge_impl::handoff_in_progress
(const MobileTerminal::TerminalId& terminal_id,
 MobileTerminal::AccessBridge_ptr new_access_bridge)
    throw(CORBA::SystemException)
{
    _proxy->terminal_moved(terminal_id, new_access_bridge);
    _proxy->mobility_event(WATMProxy::NET_DEPART, terminal_id,
			   new_access_bridge);
    MapIdHoff::iterator it = _handoffs_in_progress.find(terminal_id);
    if (it != _handoffs_in_progress.end()) {
	CORBA::release(it->second.new_ab);
	CORBA::release(it->second.cb);
	_handoffs_in_progress.erase(it);
    }
}


/*
 * Called by new access bridge at old access bridge in response to
 * terminal_id wanting to recover.
 */
void
AccessBridge_impl::recovery_request
(const MobileTerminal::TerminalId& terminal_id,
 MobileTerminal::AccessBridge_ptr new_access_bridge,
 CORBA::UShort gtp_at_terminal,
 CORBA::UShort_out gtp_at_access_bridge)
    throw(CORBA::SystemException, MobileTerminal::UnknownTerminalId)
{
    if (!_proxy->is_known(terminal_id))
	throw MobileTerminal::UnknownTerminalId();
    _proxy->notify_handoff(terminal_id);
    gtp_at_access_bridge =
	_proxy->terminal_moved(terminal_id, new_access_bridge,
			       gtp_at_terminal);
    _proxy->mobility_event(WATMProxy::NET_DEPART, terminal_id,
			   new_access_bridge);
    MapIdHoff::iterator it = _handoffs_in_progress.find(terminal_id);
    if (it != _handoffs_in_progress.end()) {
	if (!CORBA::is_nil(it->second.cb)) {
	    it->second.cb->report_handoff_status(MobileTerminal::HANDOFF_SUCCESS);
	}
	CORBA::release(it->second.new_ab);
	CORBA::release(it->second.cb);
	_handoffs_in_progress.erase(it);
    }
    _proxy->prepare_forwarding(terminal_id);
}


/*
 * Called by old access bridge at current access bridge to forward GTP
 * message gtp_message to terminal_id.
 */
void
AccessBridge_impl::gtp_to_terminal
(const MobileTerminal::TerminalId& terminal_id,
 MobileTerminal::AccessBridge_ptr old_access_bridge,
 CORBA::ULong gtp_message_id,
 const MobileTerminal::GTPEncapsulation& gtp_message)
    throw(CORBA::SystemException, MobileTerminal::TerminalNotHere)
{
    if (!_proxy->is_here(terminal_id))
	throw MobileTerminal::TerminalNotHere();
    _proxy->terminal_forward(terminal_id, old_access_bridge, gtp_message_id,
			     gtp_message);
}


/*
 * Called by current access bridge at old access bridge to forward GTP
 * message gtp_message from terminal_id.
 */
void
AccessBridge_impl::gtp_from_terminal
(const MobileTerminal::TerminalId& terminal_id, CORBA::ULong gtp_message_id,
 const MobileTerminal::GTPEncapsulation& gtp_message)
    throw(CORBA::SystemException, MobileTerminal::UnknownTerminalId)
{
    if (!_proxy->is_known(terminal_id))
	throw MobileTerminal::UnknownTerminalId();
    _proxy->net_forward(terminal_id, gtp_message);
}

void
AccessBridge_impl::gtp_acknowledge (CORBA::ULong gtp_message_id,
				    CORBA::ULong forward_status)
{
    // This doesn't do anything
}

/*
 * Called at handoff to inform that terminal_id has moved to
 * new_access_bridge.
 */
void
AccessBridge_impl::handoff_notice
(const MobileTerminal::TerminalId& terminal_id,
 MobileTerminal::AccessBridge_ptr new_access_bridge )
    throw(CORBA::SystemException)
{
    if (_proxy->change_forward(terminal_id, new_access_bridge)) {
	MobileTerminal::AccessBridge_var ab = _this();
	new_access_bridge->subscribe_handoff_notice(terminal_id, ab);
    }
}


/*
 * Called by interested_access_bridge to request handoff_notice
 * invocation when terminal_id moves.
 */
void
AccessBridge_impl::subscribe_handoff_notice
(const MobileTerminal::TerminalId& terminal_id,
 MobileTerminal::AccessBridge_ptr interested_access_bridge)
    throw(CORBA::SystemException, MobileTerminal::TerminalNotHere)
{
    if (!_proxy->add_notice(terminal_id, interested_access_bridge))
	throw MobileTerminal::TerminalNotHere();
}


CORBA::Object_ptr
AccessBridge_impl::mobility_channel_reference ()
{
    return CORBA::Object::_duplicate(_channel);
}

