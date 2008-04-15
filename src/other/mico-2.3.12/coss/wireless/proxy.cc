/*
 *  MIWCO --- Wireless CORBA support for MICO (OMG document formal/2004-04-02)
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

#include <CORBA.h>
#include <coss/GTP.h>
#include <coss/MobileTerminalNotification.h>
#include <mico/impl.h>
#include <mico/watm.h>
#include <coss/CosEventComm.h>
#ifdef USE_MEMTRACE
#include <mico/memtrace.h>
#endif
#include "glue.h"
#include "proxy.h"

using namespace std;

/************************** GTPInContext **************************/

WATMProxy::GTPInContext::GTPInContext (GTPCodec *codec, CORBA::Buffer *buf)
{
    _buf = buf;
    _delete_buf = TRUE;

    _dc = codec->dc_proto();
    _dc->buffer(buf, FALSE);
    _own_seq = _peer_seq = 0;
}

WATMProxy::GTPInContext::~GTPInContext ()
{
    if (_delete_buf)
	delete _buf;
}

void
WATMProxy::GTPInContext::buffer (CORBA::Buffer *buf)
{
    if (_delete_buf)
	delete _buf;

    _dc->buffer(buf, FALSE);
    _buf = buf;
    _delete_buf = FALSE;
}

CORBA::DataDecoder *
WATMProxy::GTPInContext::_retn ()
{
    assert(_delete_buf);
    _delete_buf = FALSE;
    return _dc->clone(_buf, TRUE);
}

void
WATMProxy::GTPInContext::get_seq_nos (CORBA::UShort &own, CORBA::UShort &peer)
{
    own = _own_seq;
    peer = _peer_seq;
}

void
WATMProxy::GTPInContext::set_seq_nos (CORBA::UShort own, CORBA::UShort peer)
{
    _own_seq = own;
    _peer_seq = peer;
}

/************************** GTPOutContext **************************/

WATMProxy::GTPOutContext::GTPOutContext (GTPCodec *codec, CORBA::Buffer *buf)
{
    _buf = buf;
    if (_buf == NULL)
	_buf = new CORBA::Buffer;
    _delete_buf = TRUE;

    _ec = codec->ec_proto();
    _ec->buffer(_buf, FALSE);

    _own_seq = _peer_seq = 0;
}

WATMProxy::GTPOutContext::GTPOutContext (CORBA::DataEncoder *ec)
{
    _buf = ec->buffer();
    _delete_buf = FALSE;

    _ec = ec;
}

WATMProxy::GTPOutContext::~GTPOutContext ()
{
    if (_delete_buf)
	delete _buf;
}

CORBA::Buffer *
WATMProxy::GTPOutContext::_retn ()
{
    assert(_delete_buf);
    _delete_buf = FALSE;
    return _buf;
}

void
WATMProxy::GTPOutContext::get_seq_nos (CORBA::UShort &own, CORBA::UShort &peer)
{
    own = _own_seq;
    peer = _peer_seq;
}

void
WATMProxy::GTPOutContext::set_seq_nos (CORBA::UShort own, CORBA::UShort peer)
{
    _own_seq = own;
    _peer_seq = peer;
}

/**************************** GTPCodec ****************************/

WATMProxy::GTPCodec::GTPCodec (CORBA::DataDecoder *dc, CORBA::DataEncoder *ec,
			       CORBA::UShort gtp_ver)
    : _dc_proto(dc), _ec_proto(ec)
{
    _gtp_ver = gtp_ver;

    GTPOutContext ctx(this);
    put_header(ctx, GTP::IdleSync);
    _headerlen = ctx.ec()->buffer()->length();
}

WATMProxy::GTPCodec::~GTPCodec ()
{
    delete _ec_proto;
    delete _dc_proto;
}

CORBA::ULong
WATMProxy::GTPCodec::put_header (GTPOutContext &out, GTP::MsgType type)
{
    CORBA::DataEncoder *ec = out.ec();
    CORBA::UShort own, peer;
    CORBA::ULong offset;

    out.get_seq_nos(own, peer);
    ec->struct_begin();
    {
	ec->put_octet(type != GTP::Error ? type : 0xff);
	ec->put_octet(ec->byteorder() == CORBA::LittleEndian
		      ? GTP_BYTEORDER_BIT : 0);

	ec->put_ushort(own);
	ec->put_ushort(peer);

	offset = ec->buffer()->wpos();
	ec->put_ushort(0);
    }
    ec->struct_end();

    return offset;
}

void
WATMProxy::GTPCodec::put_size (GTPOutContext &out, CORBA::ULong offset)
{
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong end_pos = ec->buffer()->wpos();
    ec->buffer()->wseek_beg(offset);
    ec->put_ushort(end_pos - ec->buffer()->rpos() - _headerlen);
    ec->buffer()->wseek_beg(end_pos);
}

void
WATMProxy::GTPCodec::put_target (GTPOutContext &out,
				 const WATMGlue::TargetAddress *target)
{
    CORBA::DataEncoder *ec = out.ec();

    ec->put_short(target->kind);
    switch (target->kind) {
    case WATMGlue::TargetAddress::Key:
	ec->seq_begin(target->u.key->length());
	ec->put_octets(target->u.key->get_buffer(), target->u.key->length());
	ec->seq_end();
	break;
    case WATMGlue::TargetAddress::Profile:
	target->u.profile->encode(*ec);
	break;
    case WATMGlue::TargetAddress::Reference:
	ec->struct_begin();
	ec->put_ulong(target->u.reference.index);
	ec->put_ior(*target->u.reference.ior);
	ec->struct_end();
	break;
    }
}

void
WATMProxy::GTPCodec::put_transport_address (GTPOutContext &out,
					    MobileTerminal::AccessBridgeTransportAddress addr)
{
    CORBA::DataEncoder *ec = out.ec();

    ec->struct_begin();
    {
	ec->struct_begin();
	{
	    ec->struct_begin();
	    {
		ec->put_octet(addr.tunneling_protocol.gtp_version.major);
		ec->put_octet(addr.tunneling_protocol.gtp_version.minor);
	    }
	    ec->struct_end();
	    ec->put_octet(addr.tunneling_protocol.protocol_level);
	    ec->put_octet(addr.tunneling_protocol.protocol_id);
	}
	ec->struct_end();
	ec->seq_begin(addr.transport_address.length());
	{
	    ec->put_octets(addr.transport_address.get_buffer(),
			   addr.transport_address.length());
	}
	ec->seq_end();
    }
    ec->struct_end();
}

#define check(exp) if (!(exp)) return FALSE

CORBA::Boolean
WATMProxy::GTPCodec::get_target (GTPInContext &in,
				 WATMGlue::TargetAddress *target)
{
    CORBA::DataDecoder *dc = in.dc();

    check(dc->get_short(target->kind));
    switch (target->kind) {
    case WATMGlue::TargetAddress::Key: {
	CORBA::ULong len;
	check(dc->seq_begin(len));
	target->u.key = new CORBA::OctetSeq(len);
	target->u.key->length(len);
	check(dc->get_octets(target->u.key->get_buffer(), len));
	check(dc->seq_end());
	break;
    }
    case WATMGlue::TargetAddress::Profile:
	check(target->u.profile = CORBA::IORProfile::decode(*dc));
	break;
    case WATMGlue::TargetAddress::Reference: {
	CORBA::IOR ior;
	check(dc->struct_begin());
	check(dc->get_ulong(target->u.reference.index));
	check(ior.decode(*dc));
	target->u.reference.ior = new CORBA::IOR(ior);
	check(dc->struct_end());
	break;
    }
    default:
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPCodec::get_transport_address (GTPInContext &in,
					    MobileTerminal::AccessBridgeTransportAddress &addr)
{
    CORBA::DataDecoder *dc = in.dc();

    check(dc->struct_begin());
    {
	check(dc->struct_begin());
	{
	    check(dc->struct_begin());
	    {
		check(dc->get_octet(addr.tunneling_protocol.gtp_version.major));
		check(dc->get_octet(addr.tunneling_protocol.gtp_version.minor));
	    }
	    check(dc->struct_end());
	    check(dc->get_octet(addr.tunneling_protocol.protocol_level));
	    check(dc->get_octet(addr.tunneling_protocol.protocol_id));
	}
	check(dc->struct_end());
	CORBA::ULong len;
	check(dc->seq_begin(len));
	{
	    addr.transport_address.length(len);
	    check(dc->get_octets(addr.transport_address.get_buffer(), len));
	}
	check(dc->seq_end());
    }
    check(dc->struct_end());

    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPCodec::get_header (GTPInContext &in, GTP::MsgType &type,
				 CORBA::UShort &size, CORBA::Octet &flags)
{
    CORBA::DataDecoder *dc = in.dc();
    CORBA::UShort own, peer;

    check(dc->struct_begin());
    {
	CORBA::Octet temp;
	check(dc->get_octet(temp));
	type = temp == 0xff ? GTP::Error : static_cast<GTP::MsgType>(temp);
	check(dc->get_octet(flags));
	dc->byteorder(flags & GTP_BYTEORDER_BIT
		      ? CORBA::LittleEndian : CORBA::BigEndian);
	check(dc->get_ushort(peer));
	check(dc->get_ushort(own));
	check(dc->get_ushort(size));
    }
    check(dc->struct_end());

    in.set_seq_nos(own, peer);
    return TRUE;
}

#undef check

CORBA::Boolean
WATMProxy::GTPCodec::check_header (GTPInContext &in, GTP::MsgType &type,
				   CORBA::UShort &size, CORBA::Octet &flags)
{
    CORBA::ULong pos = in.dc()->buffer()->rpos();
    CORBA::Boolean ret = get_header(in, type, size, flags);
    in.dc()->buffer()->rseek_beg(pos);
    return ret;
}

CORBA::Boolean
WATMProxy::GTPCodec::put_idle_sync (GTPOutContext &out)
{
    CORBA::ULong offset = put_header(out, GTP::IdleSync);
    put_size(out, offset);
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPCodec::put_initial_request (GTPOutContext &out,
					  CORBA::OctetSeq tid,
					  MobileTerminal::HomeLocationAgent_ptr hla,
					  CORBA::ULong ttl)
{
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong offset = put_header(out, GTP::EstablishRequest);

    ec->union_begin();
    {
	ec->put_short(GTP::INITIAL_REQUEST);
	ec->struct_begin();
	{
	    ec->seq_begin(tid.length());
	    {
		ec->put_octets(tid.get_buffer(), tid.length());
	    }
	    ec->seq_end();
	    if (!CORBA::is_nil(hla)) {
		ec->put_ior(*hla->_ior());
	    } else {
		ec->put_ior(CORBA::IOR());
	    }
	    ec->put_ulong(ttl);
	}
	ec->struct_end();
    }
    ec->union_end();

    put_size(out, offset);
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPCodec::put_initial_reply (GTPOutContext &out,
					GTP::AccessStatus status,
					MobileTerminal::AccessBridge_ptr ab,
					CORBA::ULong ttl)
{
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong offset = put_header(out, GTP::EstablishReply);

    ec->union_begin();
    {
	ec->put_short(GTP::INITIAL_REPLY);
	ec->struct_begin();
	{
	    ec->enumeration(status);
	    ec->put_ior(*ab->_ior());
	    ec->put_ulong(ttl);
	}
	ec->struct_end();
    }
    ec->union_end();

    put_size(out, offset);
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPCodec::put_recovery_request (GTPOutContext &out,
					   GTP::RequestType type,
					   CORBA::OctetSeq tid,
					   MobileTerminal::HomeLocationAgent_ptr hla,
					   MobileTerminal::AccessBridge_ptr old_ab,
					   CORBA::ULong old_ttl,
					   CORBA::UShort old_seq,
					   CORBA::ULong ttl)
{
    if (type == GTP::INITIAL_REQUEST)
	return FALSE;

    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong offset = put_header(out, GTP::EstablishRequest);

    ec->union_begin();
    {
	ec->put_short(type);
	ec->struct_begin();
	{
	    ec->seq_begin(tid.length());
	    {
		ec->put_octets(tid.get_buffer(), tid.length());
	    }
	    ec->seq_end();
	    if (!CORBA::is_nil(hla)) {
		ec->put_ior(*hla->_ior());
	    } else {
		ec->put_ior(CORBA::IOR());
	    }
	    ec->struct_begin();
	    {
		ec->put_ior(*old_ab->_ior());
		ec->put_ulong(old_ttl);
		ec->put_ushort(old_seq);
	    }
	    ec->struct_end();
	    ec->put_ulong(ttl);
	}
	ec->struct_end();
    }
    ec->union_end();

    put_size(out, offset);
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPCodec::put_recovery_reply (GTPOutContext &out,
					 GTP::ReplyType type,
					 GTP::AccessStatus status,
					 MobileTerminal::AccessBridge_ptr ab,
					 CORBA::ULong old_ttl,
					 CORBA::UShort old_seq,
					 CORBA::ULong ttl)
{
    if (type == GTP::INITIAL_REPLY)
	return FALSE;

    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong offset = put_header(out, GTP::EstablishReply);

    ec->union_begin();
    {
	ec->put_short(type);
	ec->struct_begin();
	{
	    ec->enumeration(status);
	    ec->put_ior(*ab->_ior());
	    ec->struct_begin();
	    {
		ec->put_ulong(old_ttl);
		ec->put_ushort(old_seq);
	    }
	    ec->struct_end();
	    ec->put_ulong(ttl);
	}
	ec->struct_end();
    }
    ec->union_end();

    put_size(out, offset);
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPCodec::put_release_request (GTPOutContext &out, CORBA::ULong ttl)
{
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong offset = put_header(out, GTP::ReleaseRequest);

    ec->struct_begin();
    {
	ec->put_ulong(ttl);
    }
    ec->struct_end();

    put_size(out, offset);
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPCodec::put_release_reply (GTPOutContext &out, CORBA::ULong ttl)
{
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong offset = put_header(out, GTP::ReleaseReply);

    ec->struct_begin();
    {
	ec->put_ulong(ttl);
    }
    ec->struct_end();

    put_size(out, offset);
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPCodec::put_handoff_request (GTPOutContext &out,
					  MobileTerminal::AccessBridgeTransportAddressList addr_list)
{
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong offset = put_header(out, GTP::HandoffRequest);

    ec->struct_begin();
    {
	ec->seq_begin(addr_list.length());
	{
	    for (CORBA::ULong i = 0; i < addr_list.length(); ++i) {
		put_transport_address(out, addr_list[i]);
	    }
	}
	ec->seq_end();
    }
    ec->struct_end();

    put_size(out, offset);
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPCodec::put_handoff_completed (GTPOutContext &out,
					    MobileTerminal::HandoffStatus st)
{
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong offset = put_header(out, GTP::HandoffReplyCompleted);

    ec->struct_begin();
    {
	ec->enumeration(st);
    }
    ec->struct_end();

    put_size(out, offset);
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPCodec::put_open_request (GTPOutContext &out, CORBA::ULong req_id,
				       CORBA::ULong tout,
				       const WATMGlue::TargetAddress *target)
{
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong offset = put_header(out, GTP::OpenRequest);

    ec->struct_begin();
    {
	put_target(out, target);
	ec->put_ulong(req_id);
	ec->put_ulong(tout);
    }
    ec->struct_end();

    put_size(out, offset);
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPCodec::put_open_reply (GTPOutContext &out, CORBA::ULong req_id,
				     CORBA::ULong conn_id,
				     GTP::OpenConnectionStatus status)
{
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong offset = put_header(out, GTP::OpenReply);

    ec->struct_begin();
    {
	ec->put_ulong(req_id);
	ec->enumeration(status);
	ec->put_ulong(conn_id);
    }
    ec->struct_end();

    put_size(out, offset);
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPCodec::put_close_request (GTPOutContext &out,
					CORBA::ULong conn_id)
{
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong offset = put_header(out, GTP::CloseRequest);

    ec->struct_begin();
    {
	ec->put_ulong(conn_id);
    }
    ec->struct_end();

    put_size(out, offset);
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPCodec::put_close_reply (GTPOutContext &out, CORBA::ULong conn_id,
				      GTP::CloseConnectionStatus status)
{
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong offset = put_header(out, GTP::CloseReply);

    ec->struct_begin();
    {
	ec->put_ulong(conn_id);
	ec->enumeration(status);
    }
    ec->struct_end();

    put_size(out, offset);
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPCodec::put_close_indication (GTPOutContext &out,
					   CORBA::ULong conn_id,
					   GTP::ConnectionCloseReason reason)
{
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong offset = put_header(out, GTP::CloseIndication);

    ec->struct_begin();
    {
	ec->put_ulong(conn_id);
	ec->enumeration(reason);
    }
    ec->struct_end();

    put_size(out, offset);
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPCodec::put_giop_data (GTPOutContext &out, CORBA::ULong conn_id,
				    CORBA::ULong mesg_id,
				    const CORBA::Buffer *buf)
{
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong offset = put_header(out, GTP::GIOPData);

    ec->struct_begin();
    {
	ec->put_ulong(conn_id);
	ec->put_ulong(mesg_id);
	ec->seq_begin(buf->length());
	{
	    ec->put_octets((CORBA::Octet*)buf->data(), buf->length());
	}
	ec->seq_end();
    }
    ec->struct_end();

    put_size(out, offset);
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPCodec::put_giop_data_error (GTPOutContext &out,
					  CORBA::ULong mesg_id,
					  GTP::DeliveryStatus status)
{
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong offset = put_header(out, GTP::GIOPDataError);

    ec->struct_begin();
    {
	ec->put_ulong(mesg_id);
	ec->enumeration(status);
    }
    ec->struct_end();

    put_size(out, offset);
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPCodec::put_gtp_forward (GTPOutContext &out,
				      MobileTerminal::AccessBridge_ptr ab,
				      CORBA::ULong mesg_id,
				      const CORBA::Buffer *buf)
{
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong offset = put_header(out, GTP::GTPForward);

    ec->struct_begin();
    {
	ec->put_ior(*ab->_ior());
	ec->put_ulong(mesg_id);
	ec->seq_begin(buf->length());
	{
	    ec->put_octets((CORBA::Octet *)buf->data(), buf->length());
	}
	ec->seq_end();
    }
    ec->struct_end();

    put_size(out, offset);
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPCodec::put_gtp_forward_reply (GTPOutContext &out,
					    CORBA::ULong mesg_id,
					    GTP::ForwardStatus status)
{
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong offset = put_header(out, GTP::GTPForwardReply);

    ec->struct_begin();
    {
	ec->put_ulong(mesg_id);
	ec->enumeration(status);
    }
    ec->struct_end();

    put_size(out, offset);
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPCodec::put_error (GTPOutContext &out, GTP::ErrorCode code,
				CORBA::UShort seq_no)
{
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong offset = put_header(out, GTP::Error);

    ec->struct_begin();
    {
	ec->put_ushort(seq_no);
	ec->enumeration(code);
    }
    ec->struct_end();

    put_size(out, offset);
    return TRUE;
}

#define check(exp) if (!(exp)) goto bad

CORBA::Boolean
WATMProxy::GTPCodec::get_establish_request (GTPInContext &in,
					    GTP::RequestType &type,
					    CORBA::OctetSeq *tid,
					    MobileTerminal::HomeLocationAgent_out hla,
					    MobileTerminal::AccessBridge_out old_ab,
					    CORBA::ULong &old_ttl,
					    CORBA::UShort &old_seq,
					    CORBA::ULong &ttl)
{
    CORBA::DataDecoder *dc = in.dc();
    CORBA::IOR *hior = new CORBA::IOR;
    CORBA::IOR *aior = new CORBA::IOR;
    MobileTerminal::HomeLocationAgent_var temp_hla;
    MobileTerminal::AccessBridge_var temp_ab;

    check(dc->union_begin());
    {
	check(dc->get_short(type));
	check(dc->struct_begin());
	{
	    CORBA::ULong len;
	    check(dc->seq_begin(len));
	    {
		tid->length(len);
		check(dc->get_octets(tid->get_buffer(), len));
	    }
	    check(dc->seq_end());
	    check(dc->get_ior(*hior));
	    CORBA::ORB_var orb = CORBA::ORB_instance("mico-local-orb");
	    CORBA::Object_var obj = orb->ior_to_object(hior);
	    temp_hla = MobileTerminal::HomeLocationAgent::_narrow(obj);
	    if (type == GTP::INITIAL_REQUEST) {
		temp_ab = MobileTerminal::AccessBridge::_nil();
		old_ttl = 0;
		old_seq = 0;
	    } else if (type == GTP::RECOVERY_REQUEST
		       || type == GTP::NETWORK_REQUEST
		       || type == GTP::TERMINAL_REQUEST) {
		check(dc->struct_begin());
		{
		    check(dc->get_ior(*aior));
		    obj = orb->ior_to_object(aior);
		    temp_ab = MobileTerminal::AccessBridge::_narrow(obj);
		    check(dc->get_ulong(old_ttl));
		    check(dc->get_ushort(old_seq));
		}
		check(dc->struct_end());
	    } else {
		goto bad;
	    }
	    check(dc->get_ulong(ttl));
	}
	check(dc->struct_end());
    }
    check(dc->union_end());

    hla = temp_hla._retn();
    old_ab = temp_ab._retn();
    return TRUE;

  bad:
    delete hior;
    delete aior;
    return FALSE;
}

CORBA::Boolean
WATMProxy::GTPCodec::get_establish_reply (GTPInContext &in,
					  GTP::ReplyType &type,
					  GTP::AccessStatus &status,
					  MobileTerminal::AccessBridge_out ab,
					  CORBA::ULong &old_ttl,
					  CORBA::UShort &old_seq,
					  CORBA::ULong &ttl)
{
    CORBA::DataDecoder *dc = in.dc();
    CORBA::IOR *ior = new CORBA::IOR;

    check(dc->union_begin());
    {
	check(dc->get_short(type));
	check(dc->struct_begin());
	{
	    CORBA::ULong temp;
	    check(dc->enumeration(temp));
	    status = static_cast<GTP::AccessStatus>(temp);
	    CORBA::ORB_var orb = CORBA::ORB_instance("mico-local-orb");
	    check(dc->get_ior(*ior));
	    CORBA::Object_var obj = orb->ior_to_object(ior);
	    ab = MobileTerminal::AccessBridge::_narrow(obj);
	    if (type == GTP::INITIAL_REPLY) {
		old_ttl = 0;
		old_seq = 0;
	    } else if (type == GTP::RECOVERY_REPLY
		       || type == GTP::NETWORK_REPLY
		       || type == GTP::TERMINAL_REPLY) {
		check(dc->struct_begin());
		{
		    check(dc->get_ulong(old_ttl));
		    check(dc->get_ushort(old_seq));
		}
		check(dc->struct_end());
	    } else {
		goto bad;
	    }
	    check(dc->get_ulong(ttl));
	}
	check(dc->struct_end());
    }
    check(dc->union_end());

    return TRUE;

  bad:
    delete ior;
    return FALSE;
}

CORBA::Boolean
WATMProxy::GTPCodec::get_release_request (GTPInContext &in, CORBA::ULong &ttl)
{
    CORBA::DataDecoder *dc = in.dc();

    check(dc->struct_begin());
    {
	check(dc->get_ulong(ttl));
    }
    check(dc->struct_end());

    return TRUE;

  bad:
    return FALSE;
}

CORBA::Boolean
WATMProxy::GTPCodec::get_release_reply (GTPInContext &in, CORBA::ULong &ttl)
{
    CORBA::DataDecoder *dc = in.dc();

    check(dc->struct_begin());
    {
	check(dc->get_ulong(ttl));
    }
    check(dc->struct_end());

    return TRUE;

  bad:
    return FALSE;
}

CORBA::Boolean
WATMProxy::GTPCodec::get_handoff_request (GTPInContext &in,
					  MobileTerminal::AccessBridgeTransportAddressList_out addr_list)
{
    CORBA::DataDecoder *dc = in.dc();
    addr_list = new MobileTerminal::AccessBridgeTransportAddressList;

    check(dc->struct_begin());
    {
	CORBA::ULong len;
	check(dc->seq_begin(len));
	{
	    addr_list->length(len);
	    for (CORBA::ULong i = 0; i < len; ++i) {
		check(get_transport_address(in, addr_list[i]));
	    }
	}
	check(dc->seq_end());
    }
    check(dc->struct_end());

    return TRUE;

  bad:
    return FALSE;
}

CORBA::Boolean
WATMProxy::GTPCodec::get_handoff_completed (GTPInContext &in,
					    MobileTerminal::HandoffStatus &st)
{
    CORBA::DataDecoder *dc = in.dc();

    check(dc->struct_begin());
    {
	CORBA::ULong temp;
	check(dc->enumeration(temp));
	st = static_cast<MobileTerminal::HandoffStatus>(temp);
    }
    check(dc->struct_end());

    return TRUE;

  bad:
    return FALSE;
}

CORBA::Boolean
WATMProxy::GTPCodec::get_open_request (GTPInContext &in, CORBA::ULong &req_id,
				       CORBA::ULong &tout,
				       WATMGlue::TargetAddress *ta)
{
    CORBA::DataDecoder *dc = in.dc();

    check(dc->struct_begin());
    {
	check(get_target(in, ta));
	check(dc->get_ulong(req_id));
	check(dc->get_ulong(tout));
    }
    check(dc->struct_end());

    return TRUE;

  bad:
    return FALSE;
}

CORBA::Boolean
WATMProxy::GTPCodec::get_open_reply (GTPInContext &in, CORBA::ULong &req_id,
				     CORBA::ULong &conn_id,
				     GTP::OpenConnectionStatus &status)
{
    CORBA::DataDecoder *dc = in.dc();

    check(dc->struct_begin());
    {
	CORBA::ULong temp;
	check(dc->get_ulong(req_id));
	check(dc->enumeration(temp));
	status = static_cast<GTP::OpenConnectionStatus>(temp);
	check(dc->get_ulong(conn_id));
    }
    check(dc->struct_end());

    return TRUE;

  bad:
    return FALSE;
}

CORBA::Boolean
WATMProxy::GTPCodec::get_close_request (GTPInContext &in,
					CORBA::ULong &conn_id)
{
    CORBA::DataDecoder *dc = in.dc();

    check(dc->struct_begin());
    {
	check(dc->get_ulong(conn_id));
    }
    check(dc->struct_end());

    return TRUE;

  bad:
    return FALSE;
}

CORBA::Boolean
WATMProxy::GTPCodec::get_close_reply (GTPInContext &in, CORBA::ULong &conn_id,
				      GTP::CloseConnectionStatus &status)
{
    CORBA::DataDecoder *dc = in.dc();

    check(dc->struct_begin());
    {
	CORBA::ULong temp;
	check(dc->get_ulong(conn_id));
	check(dc->enumeration(temp));
	status = static_cast<GTP::CloseConnectionStatus>(temp);
    }
    check(dc->struct_end());

    return TRUE;

  bad:
    return FALSE;
}

CORBA::Boolean
WATMProxy::GTPCodec::get_close_indication (GTPInContext &in,
					   CORBA::ULong &conn_id,
					   GTP::ConnectionCloseReason &reason)
{
    CORBA::DataDecoder *dc = in.dc();

    check(dc->struct_begin());
    {
	CORBA::ULong temp;
	check(dc->get_ulong(conn_id));
	check(dc->enumeration(temp));
	reason = static_cast<GTP::ConnectionCloseReason>(temp);
    }
    check(dc->struct_end());

    return TRUE;

  bad:
    return FALSE;
}

CORBA::Boolean
WATMProxy::GTPCodec::get_giop_data (GTPInContext &in, CORBA::ULong &conn_id,
				    CORBA::ULong &mesg_id, CORBA::Buffer *buf)
{
    CORBA::DataDecoder *dc = in.dc();

    check(dc->struct_begin());
    {
	check(dc->get_ulong(conn_id));
	check(dc->get_ulong(mesg_id));
	CORBA::ULong len;
	check(dc->seq_begin(len));
	{
	    buf->put(dc->buffer()->data(), len);
	    dc->buffer()->rseek_rel(len);
	}
	check(dc->seq_end());
    }
    check(dc->struct_end());

    return TRUE;

  bad:
    return FALSE;
}

CORBA::Boolean
WATMProxy::GTPCodec::get_giop_data_error (GTPInContext &in,
					  CORBA::ULong &mesg_id,
					  GTP::DeliveryStatus &status)
{
    CORBA::DataDecoder *dc = in.dc();

    check(dc->struct_begin());
    {
	CORBA::ULong temp;
	check(dc->get_ulong(mesg_id));
	check(dc->enumeration(temp));
	status = static_cast<GTP::DeliveryStatus>(temp);
    }
    check(dc->struct_end());

    return TRUE;

  bad:
    return FALSE;
}

CORBA::Boolean
WATMProxy::GTPCodec::get_gtp_forward (GTPInContext &in,
				      MobileTerminal::AccessBridge_out ab,
				      CORBA::ULong &mesg_id,
				      CORBA::Buffer *buf)
{
    CORBA::DataDecoder *dc = in.dc();
    CORBA::IOR *ior = new CORBA::IOR;

    check(dc->struct_begin());
    {
	check(dc->get_ior(*ior));
	CORBA::ORB_var orb = CORBA::ORB_instance("mico-local-orb");
	CORBA::Object_var obj = orb->ior_to_object(ior);
	ab = MobileTerminal::AccessBridge::_narrow(obj);
	check(dc->get_ulong(mesg_id));
	CORBA::ULong len;
	check(dc->seq_begin(len));
	{
	    buf->put(dc->buffer()->data(), len);
	    dc->buffer()->rseek_rel(len);
	}
	check(dc->seq_end());
    }
    check(dc->struct_end());

    return TRUE;

  bad:
    delete ior;
    return FALSE;
}

CORBA::Boolean
WATMProxy::GTPCodec::get_gtp_forward_reply (GTPInContext &in,
					    CORBA::ULong &mesg_id,
					    GTP::ForwardStatus &status)
{
    CORBA::DataDecoder *dc = in.dc();

    check(dc->struct_begin());
    {
	check(dc->get_ulong(mesg_id));
	CORBA::ULong temp;
	check(dc->enumeration(temp));
	status = static_cast<GTP::ForwardStatus>(temp);
    }
    check(dc->struct_end());

    return TRUE;

  bad:
    return FALSE;
}

CORBA::Boolean
WATMProxy::GTPCodec::get_error (GTPInContext &in, GTP::ErrorCode &code,
				CORBA::UShort &seq_no)
{
    CORBA::DataDecoder *dc = in.dc();

    check(dc->struct_begin());
    {
	CORBA::ULong temp;
	check(dc->get_ushort(seq_no));
	check(dc->enumeration(temp));
	code = static_cast<GTP::ErrorCode>(temp);
    }
    check(dc->struct_end());

    return TRUE;

  bad:
    return FALSE;
}

#undef check

/***************************** GTPConn *****************************/

WATMProxy::GTPConn::GTPConn (CORBA::ORB_ptr orb, CORBA::Transport *transp,
			     GTPConnCallback *cb, GTPCodec *codec,
			     CORBA::Long tout, CORBA::ULong id)
    : _inctx(codec, new CORBA::Buffer)
{
    _disp = orb->dispatcher();
    _orb = orb;
    _transp = transp;
    _cb = cb;
    _codec = codec;
    _id = id;
    _idle = FALSE;

    _inbuf = new CORBA::Buffer;
    _inlen = _codec->header_length();
    _inbufs = NULL;
    _inflags = 0;

    _transp->block(_disp->isblocking());
    _transp->rselect(_disp, this);
}

WATMProxy::GTPConn::~GTPConn ()
{
    if (MICO::Logger::IsLogged(MICO::Logger::Info)) {
	MICO::Logger::Stream(MICO::Logger::Info)
	    << "Destroying GTP connection " << _id << endl;
    }

    for (list<CORBA::Buffer *>::iterator i = _outbufs.begin();
	 i != _outbufs.end(); ++i) {
	delete *i;
    }

    _disp->remove(this, CORBA::Dispatcher::Timer);
    _transp->rselect(_disp, 0);
    _transp->wselect(_disp, 0);
    delete _transp;
    delete _inbuf;
    CORBA::release(_codec);
}

void
WATMProxy::GTPConn::do_read ()
{
    while (42) {
	assert(_inlen > 0);
	CORBA::Long r = _transp->read(*_inbuf, _inlen);
	if (r < 0) {
	    // Same workaround as in GIOPConn::do_read
	    r = _transp->read(*_inbuf, _inlen);
	}

	if (r < 0 || (r == 0 && _transp->eof())) {
	    _transp->rselect(_disp, 0);
	    _transp->wselect(_disp, 0);
	    _cb->callback(this, GTPConnCallback::Closed);
	    break;
	} else if (r > 0) {
	    _inlen -= r;
	    if (_inbuf->length() == _codec->header_length()) {
		assert(_inlen == 0);
		GTP::MsgType mt;
		_inctx.buffer(_inbuf);
		if (!_codec->check_header(_inctx, mt, _inlen, _inflags)) {
		    assert(!_inbufs);
		    _inbufs = _inbuf;
		    _inbuf = new CORBA::Buffer;
		    _inlen = _codec->header_length();
		    if (!_cb->callback(this, GTPConnCallback::InputReady)
			|| _disp->isblocking())
			break;
		    continue;
		}
	    }
	    if (_inlen == 0) {
		assert(!_inbufs);
		_inbufs = _inbuf;
		_inbuf = new CORBA::Buffer;
		_inlen = _codec->header_length();
		if (!_cb->callback(this, GTPConnCallback::InputReady)
		    || _disp->isblocking())
		    break;
	    }
	} else if (r == 0) {
	    break;
	} else {
	    assert(0);
	}
    }
}

void
WATMProxy::GTPConn::do_write ()
{
    while (42) {
	assert(_outbufs.size() > 0);
	CORBA::Buffer *buf = _outbufs.front();
	CORBA::Long r = _transp->write(*buf, buf->length());
	if (r > 0) {
	    if (buf->length() == 0) {
		delete buf;
		_outbufs.pop_front();
		if (_outbufs.size() == 0) {
		    break;
		}
	    }
	} else if (r < 0) {
	    _transp->rselect(_disp, 0);
	    _transp->wselect(_disp, 0);
	    _cb->callback(this, GTPConnCallback::Closed);
	    break;
	} else {
	    break;
	}
    }
}

void
WATMProxy::GTPConn::callback (CORBA::Dispatcher *disp,
			      CORBA::Dispatcher::Event ev)
{
    switch (ev) {
    case CORBA::Dispatcher::Timer:
	_cb->callback(this, GTPConnCallback::Idle);
	break;
    case CORBA::Dispatcher::Moved:
	_disp = disp;
	break;
    default:
	assert(0);
    }
}

void
WATMProxy::GTPConn::callback (CORBA::Transport *trans,
			      CORBA::TransportCallback::Event ev)
{
    switch (ev) {
    case Read:
	do_read();
	break;
    case Write:
	do_write();
	break;
    default:
	assert(0);
    }
}

void
WATMProxy::GTPConn::output (CORBA::Buffer *b)
{
    if (MICO::Logger::IsLogged(MICO::Logger::Transport)) {
	b->dump("GTP Out", MICO::Logger::Stream(MICO::Logger::Transport));
    }
    if (_outbufs.size() == 0) {
	_transp->write(*b, b->length());
	if (b->length() == 0) {
	    delete b;
	    return;
	}
	_outbufs.push_back(b);
    } else {
	_outbufs.push_back(b);
	do_write();
    }
}

CORBA::Buffer *
WATMProxy::GTPConn::input ()
{
    CORBA::Buffer *b = _inbufs;
    _inbufs = 0;

    if (MICO::Logger::IsLogged(MICO::Logger::Transport)) {
	b->dump("GTP In", MICO::Logger::Stream(MICO::Logger::Transport));
    }
    return b;
}

/*************************** GTPAddress ***************************/

WATMProxy::GTPAddress::GTPAddress ()
{
    protocol = 0xFF;
    version = 0;
    level = 0;
    address.length(0);
}

WATMProxy::GTPAddress::GTPAddress (MobileTerminal::AccessBridgeTransportAddress addr)
{
    protocol = addr.tunneling_protocol.protocol_id;
    level = addr.tunneling_protocol.protocol_level;
    version = 256 * addr.tunneling_protocol.gtp_version.major
	+ addr.tunneling_protocol.gtp_version.minor;
    address = addr.transport_address;
}

CORBA::Address *
WATMProxy::GTPAddress::make_address ()
{
    string addr = proto_str(protocol);

    if (addr == "")
	return NULL;
    else
	addr += ':';

    addr += stringify(protocol, address);

    return CORBA::Address::parse(addr.c_str());
}

MobileTerminal::AccessBridgeTransportAddress
WATMProxy::GTPAddress::make_transport_address ()
{
    MobileTerminal::AccessBridgeTransportAddress addr;

    addr.tunneling_protocol.protocol_id = protocol;
    addr.tunneling_protocol.protocol_level = level;
    addr.tunneling_protocol.gtp_version.major = version / 256;
    addr.tunneling_protocol.gtp_version.minor = version % 256;
    addr.transport_address = address;

    return addr;
}

string
WATMProxy::GTPAddress::proto_str (CORBA::Octet proto_id)
{
    if (proto_id == MobileTerminal::TCP_TUNNELING)
	return "inet";
    else if (proto_id == MobileTerminal::UDP_TUNNELING)
	return "utp";
    else if (proto_id == MobileTerminal::WAP_TUNNELING)
	return "waptp";
    else if (proto_id == MobileTerminal::L2CAP_TUNNELING)
	return "bt-l2cap";
    // add new address identifiers here
    else
	return "";
}

string
WATMProxy::GTPAddress::stringify (CORBA::Octet proto_id, CORBA::OctetSeq addr)
{
    string ret;

    if (proto_id == MobileTerminal::TCP_TUNNELING
	|| proto_id == MobileTerminal::UDP_TUNNELING) {
	for (CORBA::ULong i = 0; i < addr.length(); ++i) {
	    ret += (unsigned char) addr[i];
	}
    } else if (proto_id == MobileTerminal::WAP_TUNNELING) {
	// demarshal WDPAddressFormat from addr
    } else if (proto_id == MobileTerminal::L2CAP_TUNNELING) {
	for (CORBA::ULong i = 0; i < addr.length(); ++i) {
	    ret += (unsigned char) addr[i];
	}
    } // add new address formats here

    return ret;
}

/**************************** GTPProxy ****************************/

const CORBA::Long WATMProxy::GTPProxy::IdleTimer::REC_TIMEOUT = 5L;
const CORBA::Long WATMProxy::GTPProxy::ReleaseTimer::REC_TIMEOUT = 30L;

WATMProxy::GTPProxy::GTPProxy (CORBA::ORB_ptr orb, CORBA::UShort gtp_ver,
			       WATMGlue::IdGenerator id_gen,
			       CosEventComm::PushConsumer_ptr chan)
    : _id_gen(id_gen)
{
    _orb = orb;
    _gtp_ver = gtp_ver;
    _mob_chan = CosEventComm::PushConsumer::_duplicate(chan);

    _cb = NULL;
}

WATMProxy::GTPProxy::~GTPProxy ()
{
    for (MapIdReq::iterator i = _pending_requests.begin();
	 i != _pending_requests.end(); ++i) {
	for (vector<WATMGlue::BridgeRequest *>::iterator j = i->second.begin();
	     j != i->second.end(); ++j) {
	    delete *j;
	}
    }
    CORBA::release(_mob_chan);
    delete _cb;
}

void
WATMProxy::GTPProxy::set_seq_nos (GTPOutContext &out, GTPConn *conn)
{
    SequenceInfo *seq_info = _conn_statuses[conn->id()];
    out.set_seq_nos(seq_info->last_sent, seq_info->last_rec);
    ++seq_info->last_sent;
    if (seq_info->last_sent == 0) ++seq_info->last_sent;
}

void
WATMProxy::GTPProxy::replace_seq_nos (GTPOutContext &out)
{
    CORBA::DataEncoder *ec = out.ec();
    CORBA::ULong wpos = ec->buffer()->wpos();
    CORBA::UShort own, peer;
    out.get_seq_nos(own, peer);
    ec->buffer()->wseek_beg(ec->buffer()->rpos() + 2);
    ec->put_ushort(own);
    ec->put_ushort(peer);
    ec->buffer()->wseek_beg(wpos);
}

void
WATMProxy::GTPProxy::output (GTPOutContext &out, GTPConn *conn)
{
    if (MICO::Logger::IsLogged(MICO::Logger::GIOP)) {
	MICO::Logger::Stream(MICO::Logger::GIOP)
	    << "Outputting on connection " << conn->id() << endl;
    }
    CORBA::Buffer *buf = out._retn();
    SequenceInfo *seq_info = _conn_statuses[conn->id()];
    CORBA::UShort own, peer;
    out.get_seq_nos(own, peer);
    seq_info->unacked.push_back(make_pair(own, new CORBA::Buffer(*buf)));
    conn->output(buf);
    conn->idle(FALSE);
}

void
WATMProxy::GTPProxy::clear_unacked (SequenceInfo *seq_info, CORBA::UShort no)
{
    MessQueue::iterator i;
    for (i = seq_info->unacked.begin(); i != seq_info->unacked.end(); ++i)
	if (i->first == no)
	    break;
    if (i != seq_info->unacked.end()) {
	++i;
	for (MessQueue::iterator it = seq_info->unacked.begin();
	     it != i; ++it) {
	    MapSnoId::iterator is = seq_info->giops.find(it->first);
	    if (is != seq_info->giops.end()) {
		_id_gen.release_id(is->second);
		seq_info->giops.erase(is);
	    }
	    delete it->second;
	}
	seq_info->unacked.erase(seq_info->unacked.begin(), i);
    }
}

void
WATMProxy::GTPProxy::insert_correspondence (CORBA::ULong bridge_id,
					    CORBA::ULong proxy_id,
					    CORBA::ULong conn_id)
{
    _bridge_to_proxy[bridge_id] = make_pair(conn_id, proxy_id);
    _proxy_to_bridge[make_pair(conn_id, proxy_id)] = bridge_id;
}

CORBA::Boolean
WATMProxy::GTPProxy::fetch_proxy (CORBA::ULong bridge_id,
				  CORBA::ULong &proxy_id,
				  CORBA::ULong &conn_id)
{
    MapIdPair::iterator it = _bridge_to_proxy.find(bridge_id);
    if (it == _bridge_to_proxy.end())
	return FALSE;
    proxy_id = it->second.second;
    conn_id = it->second.first;
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPProxy::fetch_bridge (CORBA::ULong &bridge_id,
				   CORBA::ULong proxy_id,
				   CORBA::ULong conn_id)
{
    MapPairId::iterator it =
	_proxy_to_bridge.find(make_pair(conn_id, proxy_id));
    if (it == _proxy_to_bridge.end())
	return FALSE;
    bridge_id = it->second;
    return TRUE;
}

void
WATMProxy::GTPProxy::delete_from_proxy (CORBA::ULong proxy_id,
					CORBA::ULong conn_id)
{
    _id_gen.release_id(proxy_id);
    _id_gen.release_id(_proxy_to_bridge[make_pair(conn_id, proxy_id)]);
    _bridge_to_proxy.erase(_proxy_to_bridge[make_pair(conn_id, proxy_id)]);
    _proxy_to_bridge.erase(make_pair(conn_id, proxy_id));
}

void
WATMProxy::GTPProxy::delete_from_bridge (CORBA::ULong bridge_id)
{
    _id_gen.release_id(bridge_id);
    _id_gen.release_id(_bridge_to_proxy[bridge_id].second);
    _proxy_to_bridge.erase(_bridge_to_proxy[bridge_id]);
    _bridge_to_proxy.erase(bridge_id);
}

void
WATMProxy::GTPProxy::brselect (WATMGlue::BridgeRequestCallback *cb)
{
    delete _cb;
    _cb = cb;
}

void
WATMProxy::GTPProxy::mobility_event (MobilityEvent ev,
				     MobileTerminal::TerminalId terminal_id,
				     MobileTerminal::AccessBridge_ptr ab)
{
    if (!CORBA::is_nil(_mob_chan)) {
	CORBA::Any event_any;
	switch (ev) {
	case NET_DEPART: {
	    MobileTerminalNotification::HandoffDepartureEvent event;
	    event.terminal_id = terminal_id;
	    event.new_access_bridge =
		MobileTerminal::AccessBridge::_duplicate(ab);
	    event_any <<= event;
	    break;
	}
	case NET_ARRIVE: {
	    MobileTerminalNotification::HandoffArrivalEvent event;
	    event.terminal_id = terminal_id;
	    event.old_access_bridge =
		MobileTerminal::AccessBridge::_duplicate(ab);
	    event_any <<= event;
	    break;
	}
	case NET_DROPOUT: {
	    MobileTerminalNotification::AccessDropoutEvent event;
	    event.terminal_id = terminal_id;
	    event_any <<= event;
	    break;
	}
	case NET_RECOVERY: {
	    MobileTerminalNotification::AccessRecoveryEvent event;
	    event.terminal_id = terminal_id;
	    event_any <<= event;
	    break;
	}
	case TERM_HANDOFF: {
	    MobileTerminalNotification::TerminalHandoffEvent event;
	    event.new_access_bridge =
		MobileTerminal::AccessBridge::_duplicate(ab);
	    event_any <<= event;
	    break;
	}
	case TERM_DROPOUT: {
	    MobileTerminalNotification::TerminalDropoutEvent event;
	    event.terminal_id = terminal_id;
	    event_any <<= event;
	    break;
	}
	case TERM_RECOVERY: {
	    MobileTerminalNotification::TerminalRecoveryEvent event;
	    event.terminal_id = terminal_id;
	    event_any <<= event;
	    break;
	}
	default:
	    return;
	}
	try {
	    _mob_chan->push(event_any);
	} catch (const CORBA::Exception &) {
	    CORBA::release(_mob_chan);
	    _mob_chan = CosEventComm::PushConsumer::_nil();
	}
    }
}

void
WATMProxy::GTPProxy::conn_error (GTPConn *conn, GTPInContext &in,
				 GTP::ErrorCode code)
{
    if (MICO::Logger::IsLogged(MICO::Logger::Warning)) {
	MICO::Logger::Stream(MICO::Logger::Warning)
	    << "Error " << code << " on connection " << conn->id() << endl;
    }
#ifdef USE_MEMTRACE
    MemTrace_SelfTrace(stderr);
#endif
    CORBA::UShort own, peer;
    in.get_seq_nos(own, peer);
    GTPOutContext out(conn->codec());
    set_seq_nos(out, conn);
    conn->codec()->put_error(out, code, peer);
    output(out, conn);
    kill_conn(conn);
    ReleaseTimer *timer = _rel_timers[conn->id()];
    timer->force();
}

void
WATMProxy::GTPProxy::close_conn (GTPConn *conn)
{
    if (MICO::Logger::IsLogged(MICO::Logger::Info)) {
	MICO::Logger::Stream(MICO::Logger::Info)
	    << "Closing connection " << conn->id() << endl;
    }
    GTPOutContext out(conn->codec());
    set_seq_nos(out, conn);
    conn->codec()->put_release_request(out, 0);
    GTPProxy::output(out, conn);
    kill_conn(conn);
}

CORBA::Boolean
WATMProxy::GTPProxy::handle_input (GTPConn *conn)
{
    return handle_input(conn, conn->input());
}

CORBA::Boolean
WATMProxy::GTPProxy::handle_input (GTPConn *conn, CORBA::Buffer *buf)
{
    GTPInContext in(conn->codec(), buf);

    GTP::MsgType mt;
    CORBA::UShort size;
    CORBA::Octet flags;

    if (MICO::Logger::IsLogged(MICO::Logger::GIOP)) {
	MICO::Logger::Stream(MICO::Logger::GIOP)
	    << "Incoming input on connection " << conn->id() << endl;
    }
    if (!conn->codec()->get_header(in, mt, size, flags)) {
	conn_error(conn, in, GTP::ERROR_PROTOCOL_ERROR);
	return FALSE;
    }

    CORBA::UShort own, peer;
    in.get_seq_nos(own, peer);
    SequenceInfo *seq_info = _conn_statuses[conn->id()];
    // This is a bit hacky to handle wraparound
    if (peer != 0 && peer <= seq_info->last_rec
	&& peer + 10000 > seq_info->last_rec)
	return TRUE;
    if (peer == 0 || peer == seq_info->last_rec + 1
	|| (seq_info->last_rec == 65535 && peer == 1))
	seq_info->last_rec = peer;
    clear_unacked(seq_info, own);

    MapIdITimer::iterator i = _idle_timers.find(conn->id());
    if (i != _idle_timers.end())
	i->second->enable();

    switch (mt) {
    case GTP::IdleSync:
	return TRUE;
    case GTP::EstablishRequest:
	return handle_establish_request(conn, in);
    case GTP::EstablishReply:
	return handle_establish_reply(conn, in);
    case GTP::ReleaseRequest:
	return handle_release_request(conn, in);
    case GTP::ReleaseReply:
	return handle_release_reply(conn, in);
    case GTP::HandoffRequest:
	return handle_handoff_request(conn, in);
    case GTP::HandoffReplyCompleted:
	return handle_handoff_reply(conn, in);
    case GTP::OpenRequest:
	return handle_open_request(conn, in);
    case GTP::OpenReply:
	return handle_open_reply(conn, in);
    case GTP::CloseRequest:
	return handle_close_request(conn, in);
    case GTP::CloseReply:
	return handle_close_reply(conn, in);
    case GTP::CloseIndication:
	return handle_close_indication(conn, in);
    case GTP::GIOPData:
	return handle_giop_data(conn, in);
    case GTP::GIOPDataError:
	return handle_giop_data_error(conn, in);
    case GTP::GTPForward:
	return handle_gtp_forward(conn, in);
    case GTP::GTPForwardReply:
	return handle_gtp_forward_reply(conn, in);
    case GTP::Error: {
	GTP::ErrorCode code;
	CORBA::UShort seq_no;
	conn->codec()->get_error(in, code, seq_no);
	kill_conn(conn);
	ReleaseTimer *timer = _rel_timers[conn->id()];
	timer->force();
	break;
    }

    }

    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPProxy::handle_establish_request (GTPConn *conn, GTPInContext &in)
{
    return FALSE;
}

CORBA::Boolean
WATMProxy::GTPProxy::handle_establish_reply (GTPConn *conn, GTPInContext &in)
{
    return FALSE;
}

CORBA::Boolean
WATMProxy::GTPProxy::handle_release_request (GTPConn *conn, GTPInContext &in)
{
    CORBA::ULong ttl;

    if (!conn->codec()->get_release_request(in, ttl)) {
	conn_error(conn, in, GTP::ERROR_PROTOCOL_ERROR);
	return FALSE;
    }

    GTPOutContext out(conn->codec());
    set_seq_nos(out, conn);
    conn->codec()->put_release_reply(out, ttl);
    GTPProxy::output(out, conn);
    kill_conn(conn);
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPProxy::handle_release_reply (GTPConn *conn, GTPInContext &in)
{
    CORBA::ULong ttl;

    if (!conn->codec()->get_release_reply(in, ttl)) {
	conn_error(conn, in, GTP::ERROR_PROTOCOL_ERROR);
	return FALSE;
    }
    kill_conn(conn);
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPProxy::handle_handoff_request (GTPConn *conn, GTPInContext &in)
{
    return FALSE;
}

CORBA::Boolean
WATMProxy::GTPProxy::handle_handoff_reply (GTPConn *conn, GTPInContext &in)
{
    return FALSE;
}

CORBA::Boolean
WATMProxy::GTPProxy::handle_open_request (GTPConn *conn, GTPInContext &in)
{
    static const GTP::OpenConnectionStatus code_map[] = {
	GTP::OPEN_SUCCESS, GTP::OPEN_FAILED_UNKNOWN_REASON,
	GTP::OPEN_FAILED_TIMEOUT, GTP::OPEN_FAILED_UNREACHABLE_TARGET,
	GTP::OPEN_FAILED_OUT_OF_RESOURCES, GTP::OPEN_FAILED_UNKNOWN_REASON
    };
    CORBA::ULong req_id;
    CORBA::ULong tout;
    WATMGlue::TargetAddress *ta = new WATMGlue::TargetAddress;

    if (!conn->codec()->get_open_request(in, req_id, tout, ta)) {
	delete ta;
	conn_error(conn, in, GTP::ERROR_PROTOCOL_ERROR);
	return FALSE;
    }

    WATMGlue::BridgeRequest *br = new WATMGlue::BridgeRequest;
    br->target = ta;
    br->conn_id = _id_gen.new_id();
    CORBA::Boolean ret =
	_cb->callback(br, WATMGlue::BridgeRequestCallback::Open);

    if (ret) {
	insert_correspondence(br->conn_id, br->conn_id, conn->id());
    } else {
	_id_gen.release_id(br->conn_id);
	br->conn_id = WATMGlue::IdGenerator::INVALID_ID;
    }
    br->code = code_map[br->code];
    GTPOutContext out(conn->codec());
    set_seq_nos(out, conn);
    conn->codec()->put_open_reply(out, req_id, br->conn_id,
				  (GTP::OpenConnectionStatus)br->code);
    output(out, conn);
    delete br;
    return ret;
}

CORBA::Boolean
WATMProxy::GTPProxy::handle_open_reply (GTPConn *conn, GTPInContext &in)
{
    static const WATMGlue::BridgeRequestCallback::ErrorCode code_map[] = {
	BridgeRequestCallback::NO_ERR, BridgeRequestCallback::COMM_FAILURE,
	BridgeRequestCallback::RESOURCE, BridgeRequestCallback::TIMEOUT,
	BridgeRequestCallback::UNKNOWN
    };
    CORBA::ULong req_id, conn_id;
    GTP::OpenConnectionStatus status;

    if (!conn->codec()->get_open_reply(in, req_id, conn_id, status)) {
	conn_error(conn, in, GTP::ERROR_PROTOCOL_ERROR);
	return FALSE;
    }
    if (status != GTP::OPEN_SUCCESS) {
	WATMGlue::BridgeRequest *br = new WATMGlue::BridgeRequest;
	br->conn_id = req_id;
	br->code = code_map[status];
	_cb->callback(br, WATMGlue::BridgeRequestCallback::Error);
	_pending_requests.erase(req_id);
	delete br;
	return FALSE;
    }
    insert_correspondence(req_id, conn_id, conn->id());
    for (vector<WATMGlue::BridgeRequest *>::iterator i
	     = _pending_requests[req_id].begin();
	 i != _pending_requests[req_id].end(); ++i) {
	invoke(*i);
    }
    _pending_requests.erase(req_id);
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPProxy::handle_close_request (GTPConn *conn, GTPInContext &in)
{
    static const GTP::CloseConnectionStatus code_map[] = {
	GTP::CLOSE_SUCCESS, GTP::CLOSE_FAILED_INVALID_CONNECTION_ID,
	GTP::CLOSE_FAILED_UNKNOWN_REASON, GTP::CLOSE_FAILED_UNKNOWN_REASON,
	GTP::CLOSE_FAILED_UNKNOWN_REASON, GTP::CLOSE_FAILED_UNKNOWN_REASON
    };
    WATMGlue::BridgeRequest *br = new WATMGlue::BridgeRequest;
    CORBA::ULong conn_id;

    if (!conn->codec()->get_close_request(in, conn_id)) {
	conn_error(conn, in, GTP::ERROR_PROTOCOL_ERROR);
	delete br;
	return FALSE;
    }

    CORBA::Boolean ret = TRUE;

    if (conn_id == WATMGlue::IdGenerator::INVALID_ID) {
	list<CORBA::ULong> closed_ids;
	for (MapIdPair::iterator i = _bridge_to_proxy.begin();
	     i != _bridge_to_proxy.end(); ++i) {
	    if (i->second.first == conn->id()) {
		br->conn_id = i->first;
		ret = ret &&
		    _cb->callback(br, WATMGlue::BridgeRequestCallback::Close);
		closed_ids.push_back(i->first);
	    }
	}
	for (list<CORBA::ULong>::iterator it = closed_ids.begin();
	     it != closed_ids.end(); ++it) {
	    delete_from_bridge(*it);
	}
	if (ret) {
	    br->code = GTP::CLOSE_SUCCESS;
	} else {
	    br->code = GTP::CLOSE_FAILED_UNKNOWN_REASON;
	}
    } else {
	ret = fetch_bridge(br->conn_id, conn_id, conn->id());
	if (ret) {
	    _cb->callback(br, WATMGlue::BridgeRequestCallback::Close);
	    br->code = code_map[br->code];
	    delete_from_proxy(conn_id, conn->id());
	} else {
	    br->code = GTP::CLOSE_FAILED_INVALID_CONNECTION_ID;
	}
    }

    GTPOutContext out(conn->codec());
    set_seq_nos(out, conn);
    conn->codec()->put_close_reply(out, conn_id,
				   (GTP::CloseConnectionStatus)br->code);
    output(out, conn);
    delete br;
    return ret;
}

CORBA::Boolean
WATMProxy::GTPProxy::handle_close_reply (GTPConn *conn, GTPInContext &in)
{
    CORBA::ULong conn_id;
    GTP::CloseConnectionStatus status;

    if (!conn->codec()->get_close_reply(in, conn_id, status)) {
	conn_error(conn, in, GTP::ERROR_PROTOCOL_ERROR);
	return FALSE;
    }
    _pending_closes.erase(conn_id);
    if (status != GTP::CLOSE_SUCCESS) {
	// XXX - Should we do something to notify Bridge?
	return FALSE;
    }
    delete_from_proxy(conn_id, conn->id());
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPProxy::handle_close_indication (GTPConn *conn, GTPInContext &in)
{
    WATMGlue::BridgeRequest *br = new WATMGlue::BridgeRequest;
    CORBA::ULong conn_id;
    GTP::ConnectionCloseReason reason;

    if (!conn->codec()->get_close_indication(in, conn_id, reason)) {
	conn_error(conn, in, GTP::ERROR_PROTOCOL_ERROR);
	delete br;
	return FALSE;
    }

    CORBA::Boolean ret = TRUE;
    if (conn_id == WATMGlue::IdGenerator::INVALID_ID) {
	list<CORBA::ULong> closed_ids;
	for (MapIdPair::iterator i = _bridge_to_proxy.begin();
	     i != _bridge_to_proxy.end(); ++i) {
	    if (i->second.first == conn->id()) {
		br->conn_id = i->first;
		ret = ret &&
		    _cb->callback(br, WATMGlue::BridgeRequestCallback::Close);
		closed_ids.push_back(i->first);
	    }
	}
	for (list<CORBA::ULong>::iterator it = closed_ids.begin();
	     it != closed_ids.end(); ++it) {
	    delete_from_bridge(*it);
	}
    } else {
	ret = fetch_bridge(br->conn_id, conn_id, conn->id());
	if (ret) {
	    _cb->callback(br, WATMGlue::BridgeRequestCallback::Close);
	    delete_from_proxy(conn_id, conn->id());
	}
    }
    delete br;
    return ret;
}

CORBA::Boolean
WATMProxy::GTPProxy::handle_giop_data (GTPConn *conn, GTPInContext &in)
{
    static const GTP::DeliveryStatus code_map[] = {
	GTP::DELIVERY_FAILED_UNKNOWN_REASON, // should never happen
	GTP::DELIVERY_FAILED_INVALID_CONNECTION_ID,
	GTP::DELIVERY_FAILED_UNKNOWN_REASON,
	GTP::DELIVERY_FAILED_UNKNOWN_REASON,
	GTP::DELIVERY_FAILED_UNKNOWN_REASON,
	GTP::DELIVERY_FAILED_UNKNOWN_REASON
    };
    WATMGlue::BridgeRequest *br = new WATMGlue::BridgeRequest;

    CORBA::ULong mesg_id;
    br->giop_data = new CORBA::Buffer;

    if (!conn->codec()->get_giop_data(in, br->conn_id, mesg_id,
				      br->giop_data)) {
	conn_error(conn, in, GTP::ERROR_PROTOCOL_ERROR);
	delete br;
	return FALSE;
    }
    CORBA::Boolean ret = FALSE;
    if (!fetch_bridge(br->conn_id, br->conn_id, conn->id())) {
	br->code = GTP::DELIVERY_FAILED_INVALID_CONNECTION_ID;
    } else {
	ret = _cb->callback(br, WATMGlue::BridgeRequestCallback::Invoke);
	br->code = code_map[br->code];
    }

    GTPOutContext out(conn->codec());
    set_seq_nos(out, conn);
    /*
     * We don't typically have a thin pipe so we can waste some of it
     * by sending IdleSyncs.
     */
    if (!ret) {
	conn->codec()->put_giop_data_error(out, mesg_id,
					   (GTP::DeliveryStatus)br->code);
    } else {
	conn->codec()->put_idle_sync(out);
    }
    output(out, conn);
    delete br;
    return ret;
}

CORBA::Boolean
WATMProxy::GTPProxy::handle_giop_data_error (GTPConn *conn, GTPInContext &in)
{
    CORBA::ULong mesg_id;
    GTP::DeliveryStatus status;

    if (!conn->codec()->get_giop_data_error(in, mesg_id, status)) {
	conn_error(conn, in, GTP::ERROR_PROTOCOL_ERROR);
	return FALSE;
    }
#if 0
    /*
     * While this code is #if 0'd, it isn't completely useless.  There
     * should be a way to map mesg_id to conn_id and communicate error
     * status to our BridgeRequest callback object.  The if() is
     * unneeded, though.
     */
    if (status != GTP::DELIVERY_SUCCESS) {
	/*
	 * XXX - Need to map mesg_id to conn_id before this works
	WATMGlue::BridgeRequest *br = new WATMGlue::BridgeRequest;
	br->conn_id = mesg_to_conn(mesg_id);
	br->code = code_map[status];
	_cb->callback(br, WATMGlue::BridgeRequestCallback::Error);
	delete br;
	*/
	_id_gen.release_id(mesg_id);
	return FALSE;
    }
#endif
    _id_gen.release_id(mesg_id);
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPProxy::handle_gtp_forward (GTPConn *conn, GTPInContext &in)
{
    return FALSE;
}

CORBA::Boolean
WATMProxy::GTPProxy::handle_gtp_forward_reply (GTPConn *conn, GTPInContext &in)
{
    return FALSE;
}

CORBA::Boolean
WATMProxy::GTPProxy::callback (WATMGlue::BridgeRequest *br,
			       WATMGlue::BridgeRequestCallback::Event ev)
{
    switch (ev) {
    case WATMGlue::BridgeRequestCallback::Open:
	return open_connection(br);
    case WATMGlue::BridgeRequestCallback::Invoke:
	return invoke(br);
    case WATMGlue::BridgeRequestCallback::Close:
	return close_connection(br);
    case WATMGlue::BridgeRequestCallback::Error:
	// XXX - do something
    default:
	assert(0);
    }
    return FALSE;
}

CORBA::Boolean
WATMProxy::GTPProxy::callback (GTPConn *conn, GTPConnCallback::Event ev)
{
    switch (ev) {
    case GTPConnCallback::InputReady: {
	return handle_input(conn);
    }
    case GTPConnCallback::Idle: {
	if (MICO::Logger::IsLogged(MICO::Logger::IIOP)) {
	    MICO::Logger::Stream(MICO::Logger::IIOP)
		<< "Closing idle GTP connection" << endl;
	}
	kill_conn(conn);
	return FALSE;
    }
    case GTPConnCallback::Closed: {
	if (MICO::Logger::IsLogged(MICO::Logger::IIOP)) {
	    MICO::Logger::Stream(MICO::Logger::IIOP)
		<< "GTP conn closed or broken" << endl;
	}
	conn_closed(conn);
	return FALSE;
    }
    default:
	assert(0);
    }
    return TRUE;
}

/*************************** GTPTerminal ***************************/

namespace {

CORBA::Boolean
gtp_accepting (GTP::AccessStatus status)
{
    return status == GTP::ACCESS_ACCEPT
	|| status == GTP::ACCESS_ACCEPT_RECOVERY
	|| status == GTP::ACCESS_ACCEPT_HANDOFF
	|| status == GTP::ACCESS_ACCEPT_LOCAL;
}

CORBA::Boolean
req_type_valid (GTP::RequestType type)
{
    return type == GTP::INITIAL_REQUEST
	|| type == GTP::RECOVERY_REQUEST
	|| type == GTP::NETWORK_REQUEST
	|| type == GTP::TERMINAL_REQUEST;
}

CORBA::Boolean
is_forwardable (CORBA::Buffer *buf)
{
    CORBA::Octet type = *buf->data();
    return type == GTP::IdleSync || type == GTP::OpenReply
	|| type == GTP::CloseRequest || type == GTP::CloseReply
	|| type == GTP::CloseIndication || type == GTP::GIOPData
	|| type == GTP::GIOPDataError || type == GTP::GTPForwardReply
	|| type == GTP::Error;
}

CORBA::OctetSeq *
target_to_terminal_id (WATMGlue::TargetAddress *target)
{
    CORBA::OctetSeq_var idseq;
    const CORBA::Octet *key;
    CORBA::Long keylen;

    switch (target->kind) {
    case WATMGlue::TargetAddress::Reference: {
	CORBA::IORProfile *prof = target->u.reference.ior->profile
	    (CORBA::IORProfile::TAG_MOBILE_TERMINAL_IOP);
	if (prof != NULL) {
	    WATM::WATMProfile *wprof = dynamic_cast<WATM::WATMProfile *>(prof);
	    assert(wprof != NULL);
	    key = wprof->terminalid(keylen); // inapproriate use of key
	    idseq = new CORBA::OctetSeq(keylen);
	    idseq->length(keylen);
	    memcpy(idseq->get_buffer(), key, keylen);
	    return idseq._retn();
	} else {
	    prof = target->u.reference.ior->get_profile(target->u.reference.index);
	    key = prof->objectkey(keylen);
	}
	break;
    }
    case WATMGlue::TargetAddress::Profile:
	key = target->u.profile->objectkey(keylen);
	break;
    case WATMGlue::TargetAddress::Key:
	key = target->u.key->get_buffer();
	keylen = target->u.key->length();
	break;
    default:
	return NULL;
    }
#define check(exp) if (!(exp)) return NULL
    CORBA::Buffer buf(const_cast<CORBA::Octet *>(key));
#ifdef _WINDOWS
    MICO::CDRDecoder dc(&buf, FALSE, CORBA::DefaultEndian,
			0, TRUE, 0, TRUE);
#else
    MICO::CDRDecoder dc(&buf, FALSE);
#endif
    CORBA::Octet bo;
    check(dc.get_octet(bo));
    dc.byteorder(bo == 0 ? CORBA::BigEndian : CORBA::LittleEndian);

    char mior[4];
    check(dc.get_chars_raw(mior, 4));
    check(memcmp(mior, "MIOR", 4) == 0);
    check(dc.struct_begin());
    {
	CORBA::Octet maj, min;
	check(dc.get_octet(maj));
	check(dc.get_octet(min));
    }
    check(dc.struct_end());
    CORBA::Octet res;
    check(dc.get_octet(res));
    CORBA::ULong idlen;
    check(dc.seq_begin(idlen));
    {
	idseq = new CORBA::OctetSeq(idlen);
	idseq->length(idlen);
	check(dc.get_octets(idseq->get_buffer(), idlen));
    }
    check(dc.seq_end());
#undef check
    return idseq._retn();
}

}

WATMProxy::GTPTerminal::GTPTerminal (CORBA::ORB_ptr orb, CORBA::Address *addr,
				     CORBA::UShort gtp_ver,
				     CosEventComm::PushConsumer_ptr chan)
    : GTPProxy(orb, gtp_ver, WATMGlue::IdGenerator(1, 2), chan)
{
    CORBA::ULong tidlen;
    const CORBA::Octet *tid = _orb->terminal_id(tidlen);
    _terminal_id.length(tidlen);
    memcpy(_terminal_id.get_buffer(), tid, tidlen);

    try {
	CORBA::Object_var obj =
	    _orb->resolve_initial_references("HomeLocationAgent");
	_hla = MobileTerminal::HomeLocationAgent::_narrow(obj);
    } catch (const CORBA::ORB::InvalidName &) {
	_hla = MobileTerminal::HomeLocationAgent::_nil();
    }

    _active_id = WATMGlue::IdGenerator::INVALID_ID;
    _connected = FALSE;
    _establish_in_progress = FALSE;
    if (addr != NULL)
	ensure_connectivity(addr);
}

WATMProxy::GTPTerminal::~GTPTerminal ()
{
    if (_connected && _active_id != WATMGlue::IdGenerator::INVALID_ID) {
	ConnInfo *conn_info = _all_conns[_active_id];
	kill_conn(conn_info->conn);
    }
    list<ConnInfo *> conn_infos;
    for (MapIdInfo::iterator i = _all_conns.begin();
	 i != _all_conns.end(); ++i) {
	conn_infos.push_back(i->second);
    }
    for (list<ConnInfo *>::iterator it = conn_infos.begin();
	 it != conn_infos.end(); ++it) {
	ReleaseTimer *timer = _rel_timers[(*it)->conn->id()];
	timer->force();
    }
    CORBA::release(_hla);
}

CORBA::Boolean
WATMProxy::GTPTerminal::active (CORBA::ULong bridge_id)
{
    if (bridge_id == WATMGlue::IdGenerator::INVALID_ID)
	return FALSE;
    MapIdPair::iterator i = _bridge_to_proxy.find(bridge_id);
    if (i == _bridge_to_proxy.end())
	return FALSE;
    MapIdInfo::iterator it = _all_conns.find(i->second.first);
    if (it == _all_conns.end())
	return FALSE;
    return _active_id == it->second->conn->id();
}

WATMProxy::GTPConn *
WATMProxy::GTPTerminal::make_conn (CORBA::Address *addr, CORBA::ULong id)
{
    CORBA::Transport *t = addr->make_transport();
    if (MICO::Logger::IsLogged(MICO::Logger::IIOP)) {
	MICO::Logger::Stream(MICO::Logger::IIOP)
	    << "Connecting to " << addr->stringify() << endl;
    }
    if (!t->connect(addr)) {
	delete t;
	return NULL;
    }
    WATMProxy::GTPConn *conn =
	new WATMProxy::GTPConn(_orb, t, this,
			       new WATMProxy::GTPCodec(new MICO::CDRDecoder,
						       new MICO::CDREncoder,
						       _gtp_ver),
			       0L);
    ConnInfo *conn_info;
    if (id == WATMGlue::IdGenerator::INVALID_ID) {
	id = _id_gen.new_id();
	_conn_statuses[id] = new SequenceInfo;
	conn_info = new ConnInfo;
	conn_info->conn = conn;
	conn_info->ab_addr = addr;
	_all_conns[id] = conn_info;
    }
    conn->id(id);
    return conn;
}

void
WATMProxy::GTPTerminal::kill_conn (GTPConn *conn)
{
    if (MICO::Logger::IsLogged(MICO::Logger::IIOP)) {
	MICO::Logger::Stream(MICO::Logger::IIOP)
	    << "Killing GTP connection " << conn->id() << endl;
    }
    /*
     * We do a redundant check of _connected here since in various
     * cases the Terminal Bridge ends up calling kill_conn twice.
     * Similar check could be made in GTPNetwork::kill_conn, but so
     * far that has not caused problems.
     */
    if (conn->id() == _active_id && _connected) {
	mobility_event(TERM_DROPOUT, _terminal_id,
		       MobileTerminal::AccessBridge::_nil());
	_connected = FALSE;
    }
    MapIdRTimer::iterator i = _rel_timers.find(conn->id());
    if (i != _rel_timers.end())
	i->second->activate();
    MapIdITimer::iterator it = _idle_timers.find(conn->id());
    if (it != _idle_timers.end())
	it->second->disable();
    conn->transport()->close();
}

void
WATMProxy::GTPTerminal::release_conn (GTPConn *conn)
{
    CORBA::ULong id = conn->id();
    MapIdSeq::iterator it = _conn_statuses.find(id);
    if (it != _conn_statuses.end()) {
	delete it->second;
	_conn_statuses.erase(it);
    }
    MapIdRTimer::iterator i = _rel_timers.find(id);
    if (i != _rel_timers.end()) {
	delete i->second;
	_rel_timers.erase(i);
    }
    MapIdITimer::iterator itt = _idle_timers.find(id);
    if (itt != _idle_timers.end()) {
	delete itt->second;
	_idle_timers.erase(itt);
    }
    MapIdInfo::iterator iit = _all_conns.find(id);
    if (iit != _all_conns.end()) {
	_past_conns.erase(iit->second->ab);
	delete iit->second;
	_all_conns.erase(iit);
    }
    if (id == _active_id)
	_active_id = WATMGlue::IdGenerator::INVALID_ID;
    _id_gen.release_id(id);
}

void
WATMProxy::GTPTerminal::conn_closed (GTPConn *conn)
{
    kill_conn(conn);
}

CORBA::Boolean
WATMProxy::GTPTerminal::ensure_connectivity (CORBA::Address *addr)
{
    if (_active_id == WATMGlue::IdGenerator::INVALID_ID) {
	// Our first ever connection
	if (addr == NULL)
	    return FALSE;
	GTPConn *conn = make_conn(addr);
	if (conn == NULL)
	    return FALSE;
	_connected = TRUE;
	GTPOutContext out(conn->codec());
	set_seq_nos(out, conn);
	conn->codec()->put_initial_request(out, _terminal_id, _hla, 60UL);
	GTPProxy::output(out, conn);
	_establish_in_progress = TRUE;
    } else if (!_connected) {
	/*
	 * This is pretty hairy since it handles recovery to both old
	 * and new Access Bridge.  Maybe it could be cleaned up
	 * somewhat.
	 */
	CORBA::ULong id = _active_id;
	ConnInfo *conn_info = _all_conns[_active_id];
	ConnInfo *old_conn_info = conn_info;
	if (addr != NULL && !(*addr == *conn_info->ab_addr)) {
	    id = WATMGlue::IdGenerator::INVALID_ID;
	} else {
	    delete addr;
	    addr = conn_info->ab_addr;
	}
	GTPConn *conn = make_conn(addr, id);
	if (conn == NULL)
	    return FALSE;
	_connected = TRUE;
	if (id == WATMGlue::IdGenerator::INVALID_ID)
	    conn_info = _all_conns[conn->id()];
	conn_info->conn = conn;
	SequenceInfo *old_seq_info = _conn_statuses[_active_id];
	GTPOutContext out(conn->codec());
	if (conn->id() == _active_id)
	    _conn_statuses[_active_id] = new SequenceInfo;
	set_seq_nos(out, conn);
	if (conn->id() == _active_id) {
	    delete _conn_statuses[_active_id];
	    _conn_statuses[_active_id] = old_seq_info;
	}
	conn->codec()->put_recovery_request(out, GTP::RECOVERY_REQUEST,
					    _terminal_id, _hla,
					    old_conn_info->ab,
					    old_conn_info->ttl,
					    old_seq_info->last_rec, 60UL);
	GTPProxy::output(out, conn);
	_establish_in_progress = TRUE;
    }
    return TRUE;
}

void
WATMProxy::GTPTerminal::output (GTPOutContext &out, GTPConn *conn)
{ 
    if (MICO::Logger::IsLogged(MICO::Logger::GIOP)) {
	MICO::Logger::Stream(MICO::Logger::GIOP)
	    << "Outputting on connection " << conn->id() << endl;
    }
    MapIdITimer::iterator i = _idle_timers.find(conn->id());
    if (i != _idle_timers.end())
	i->second->disable();
    CORBA::Buffer *buf = out._retn();
    if (_active_id != WATMGlue::IdGenerator::INVALID_ID
	&& conn->id() != _active_id) {
	ConnInfo *conn_info = _all_conns[conn->id()];
	ConnInfo *new_conn_info = _all_conns[_active_id];
	GTPConn *new_conn = new_conn_info->conn;
	GTPOutContext new_out(new_conn->codec());
	set_seq_nos(new_out, new_conn);
	new_conn->codec()->put_gtp_forward(new_out, conn_info->ab,
					   _id_gen.new_id(), buf);
	output(new_out, new_conn);
	return;
    }
    SequenceInfo *seq_info = _conn_statuses[conn->id()];
    CORBA::UShort own, peer;
    out.get_seq_nos(own, peer);
    seq_info->unacked.push_back(make_pair(own, new CORBA::Buffer(*buf)));
    if (_connected && !_establish_in_progress) {
	conn->output(buf);
	conn->idle(FALSE);
    } else {
	delete buf;
    }
}

CORBA::Boolean
WATMProxy::GTPTerminal::open_connection (WATMGlue::BridgeRequest *br)
{
    MapIdPair::iterator i = _bridge_to_proxy.find(br->conn_id);
    if (i != _bridge_to_proxy.end())
	return TRUE;
    if (!br->target) {
	br->code = WATMGlue::BridgeRequestCallback::UNKNOWN;
	return FALSE;
    }
    if (!ensure_connectivity()) {
	br->code = WATMGlue::BridgeRequestCallback::COMM_FAILURE;
	return FALSE;
    }

    CORBA::ULong req_id = _id_gen.new_id();
    ConnInfo *conn_info = _all_conns[_active_id];
    GTPConn *conn = conn_info->conn;
    GTPOutContext out(conn->codec());
    set_seq_nos(out, conn);
    conn->codec()->put_open_request(out, req_id, 0, br->target);
    output(out, conn);
    br->conn_id = req_id;
    _pending_requests.insert(make_pair(req_id,
				       vector<WATMGlue::BridgeRequest *>()));
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPTerminal::invoke (WATMGlue::BridgeRequest *br)
{
    if (br->conn_id == WATMGlue::IdGenerator::INVALID_ID) {
	br->code = WATMGlue::BridgeRequestCallback::INVALID_ID;
	return FALSE;
    }

    MapIdPair::iterator it = _bridge_to_proxy.find(br->conn_id);
    if (it == _bridge_to_proxy.end()) {
	MapIdReq::iterator i = _pending_requests.find(br->conn_id);
	if (i == _pending_requests.end()) {
	    br->code = WATMGlue::BridgeRequestCallback::INVALID_ID;
	    return FALSE;
	}
	i->second.push_back(br);
	return TRUE;
    }

    MapIdInfo::iterator i = _all_conns.find(it->second.first);
    if (i == _all_conns.end()) {
	br->code = WATMGlue::BridgeRequestCallback::COMM_FAILURE;
	return FALSE;
    }

    GTPConn *conn = i->second->conn;
    GTPOutContext out(conn->codec());
    set_seq_nos(out, conn);
    CORBA::ULong giop_id = _id_gen.new_id();
    CORBA::UShort own, peer;
    out.get_seq_nos(own, peer);
    _conn_statuses[conn->id()]->giops[own] = giop_id;
    conn->codec()->put_giop_data(out, it->second.second, giop_id,
				 br->giop_data);
    output(out, conn);
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPTerminal::close_connection (WATMGlue::BridgeRequest *br)
{
    MapIdPair::iterator it = _bridge_to_proxy.find(br->conn_id);
    if (it == _bridge_to_proxy.end()) {
	br->code = WATMGlue::BridgeRequestCallback::INVALID_ID;
	return FALSE;
    }
    if (_pending_closes.find(it->second.second) == _pending_closes.end()) {
	MapIdInfo::iterator i = _all_conns.find(it->second.first);
	if (i == _all_conns.end()) {
	    delete_from_bridge(br->conn_id);
	    br->code = WATMGlue::BridgeRequestCallback::COMM_FAILURE;
	    return FALSE;
	}

	GTPConn *conn = i->second->conn;
	GTPOutContext out(conn->codec());
	set_seq_nos(out, conn);
	conn->codec()->put_close_request(out, it->second.second);
	output(out, conn);
	_pending_closes.insert(it->second.second);
    }
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPTerminal::handle_establish_reply (GTPConn *conn,
						GTPInContext &in)
{
    GTP::AccessStatus status;
    CORBA::ULong old_ttl, ttl;
    CORBA::UShort old_seq;
    CORBA::Short type;
    MobileTerminal::AccessBridge_ptr ab;

    _establish_in_progress = FALSE;
    if (!conn->codec()->get_establish_reply(in, type, status, ab,
					    old_ttl, old_seq, ttl)) {
	conn_error(conn, in, GTP::ERROR_PROTOCOL_ERROR);
	return FALSE;
    }

    if (!gtp_accepting(status)) {
	if (type == GTP::INITIAL_REPLY || type == GTP::RECOVERY_REPLY) {
	    conn->transport()->close();
	    _connected = FALSE;
	} else if (type == GTP::NETWORK_REPLY) {
	    conn->transport()->close();
	    GTPConn *curr_conn = _all_conns[_active_id]->conn;
	    GTPOutContext out(curr_conn->codec());
	    set_seq_nos(out, curr_conn);
	    curr_conn->codec()->put_handoff_completed(out, MobileTerminal::HANDOFF_FAILURE);
	    output(out, curr_conn);
	}
	delete _conn_statuses[conn->id()];
	_conn_statuses.erase(conn->id());
	delete _all_conns[conn->id()];
	_all_conns.erase(conn->id());
	CORBA::release(ab);
	return FALSE;
    }

    ConnInfo *conn_info = _all_conns[conn->id()];
    conn_info->ab = ab;
    conn_info->ttl = ttl;

    ConnInfo *old_conn_info = _all_conns[_active_id];
    SequenceInfo *old_seq_info = _conn_statuses[_active_id];

    if (status == GTP::ACCESS_ACCEPT_RECOVERY) {
	mobility_event(TERM_RECOVERY, _terminal_id, ab);
    } else {
	if (status == GTP::ACCESS_ACCEPT_HANDOFF) {
	    clear_unacked(old_seq_info, old_seq);
	    _active_id = conn->id();
	    close_conn(old_conn_info->conn);
	    MapRefId::iterator ir = _past_conns.find(ab);
	    if (ir != _past_conns.end()) {
		release_conn(_all_conns[ir->second]->conn);
		_conn_statuses[ir->second] = _conn_statuses[conn->id()];
		_conn_statuses.erase(conn->id());
		_all_conns.erase(conn->id());
		_id_gen.release_id(conn->id());
		conn->id(ir->second);
		_all_conns[conn->id()] = conn_info;
		_active_id = conn->id();
	    }
	}
	mobility_event(TERM_HANDOFF, _terminal_id, ab);
    }

    if (status == GTP::ACCESS_ACCEPT_RECOVERY) {
	delete _rel_timers[_active_id];
	_rel_timers.erase(_active_id);
	delete _idle_timers[_active_id];
	_idle_timers.erase(_active_id);
	clear_unacked(old_seq_info, old_seq);
	ConnInfo *conn_info = _all_conns[conn->id()];
	_id_gen.release_id(conn->id());
	_all_conns.erase(conn->id());
	conn->id(_active_id);
	_all_conns[conn->id()] = conn_info;
	_conn_statuses[conn->id()] = old_seq_info;
    }

    if (old_seq_info != NULL && old_conn_info != NULL) {
	GTPConn *old_conn = old_conn_info->conn;
	MessQueue unsent = old_seq_info->unacked;
	old_seq_info->unacked.clear();
	for (MessQueue::iterator i = unsent.begin(); i != unsent.end(); ++i) {
	    // XXX - A terrible hack for demo
	    if (is_forwardable(i->second)) {
		GTPOutContext out(old_conn->codec(),
				  new CORBA::Buffer(*i->second));
		set_seq_nos(out, old_conn);
		replace_seq_nos(out);
		output(out, old_conn);
	    }
	}
    }

    if (MICO::Logger::IsLogged(MICO::Logger::IIOP)) {
	MICO::Logger::Stream(MICO::Logger::IIOP)
	    << "Connection " << conn->id() << " established" << endl;
    }
    _rel_timers[conn->id()] = new ReleaseTimer(_orb, this, conn, ttl);
    _idle_timers[conn->id()] = new IdleTimer(_orb, this, conn);
    _active_id = conn->id();
    _past_conns[ab] = conn->id();
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPTerminal::handle_handoff_request (GTPConn *conn,
						GTPInContext &in)
{
    MobileTerminal::AccessBridgeTransportAddressList_var list;

    if (!conn->codec()->get_handoff_request(in, list)) {
	conn_error(conn, in, GTP::ERROR_PROTOCOL_ERROR);
	return FALSE;
    }
    CORBA::ULong i;
    for (i = 0; i < list->length(); ++i) {
	GTPAddress gtp_addr((*list)[i]);
	CORBA::Address *addr = gtp_addr.make_address();
	if (addr == NULL)
	    continue;
	GTPConn *conn = make_conn(addr);
	if (conn == NULL) {
	    delete addr;
	    continue;
	}
	ConnInfo *conn_info = _all_conns[_active_id];
	SequenceInfo *seq_info = _conn_statuses[_active_id];
	GTPOutContext out(conn->codec());
	set_seq_nos(out, conn);
	conn->codec()->put_recovery_request(out, GTP::NETWORK_REQUEST,
					    _terminal_id, _hla,
					    conn_info->ab, conn_info->ttl,
					    seq_info->last_rec, 60UL);
	/*
	 * We have to use the non-handoff-aware output function since
	 * we explicitly do not want to send data through the active
	 * connection.
	 */
	GTPProxy::output(out, conn);
	_establish_in_progress = TRUE;
	break;
    }
    if (i == list->length()) {
	GTPOutContext out(conn->codec());
	set_seq_nos(out, conn);
	conn->codec()->put_handoff_completed(out,
					     MobileTerminal::HANDOFF_FAILURE);
	output(out, conn);
    }
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPTerminal::handle_gtp_forward (GTPConn *conn, GTPInContext &in)
{
    MobileTerminal::AccessBridge_var ab;
    CORBA::ULong mesg_id;
    CORBA::Buffer *buf = new CORBA::Buffer;

    if (!conn->codec()->get_gtp_forward(in, ab, mesg_id, buf)) {
	conn_error(conn, in, GTP::ERROR_PROTOCOL_ERROR);
	return FALSE;
    }
    GTP::ForwardStatus status = GTP::FORWARD_SUCCESS;
    MapIdInfo::iterator it;
    for (it = _all_conns.begin(); it != _all_conns.end(); ++it) {
	try {
	    if (ab->_is_equivalent(it->second->ab))
		// This test can fail but it is the only one available
		break;
	} catch (const CORBA::Exception &) {
	}
    }
    if (it == _all_conns.end())
	status = GTP::FORWARD_ERROR_UNKNOWN_SENDER;
    else 
	handle_input(it->second->conn, buf);
    GTPOutContext out(conn->codec());
    set_seq_nos(out, conn);
    conn->codec()->put_gtp_forward_reply(out, mesg_id, status);
    output(out, conn);
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPTerminal::handle_gtp_forward_reply (GTPConn *conn,
						  GTPInContext &in)
{
    CORBA::ULong mesg_id;
    GTP::ForwardStatus status;

    if (!conn->codec()->get_gtp_forward_reply(in, mesg_id, status)) {
	conn_error(conn, in, GTP::ERROR_PROTOCOL_ERROR);
	return FALSE;
    }
    _id_gen.release_id(mesg_id);
    return TRUE;
}

MobileTerminal::AccessBridge_ptr
WATMProxy::GTPTerminal::current_connect_point ()
{
    if (_active_id != WATMGlue::IdGenerator::INVALID_ID) {
	ConnInfo *conn_info = _all_conns[_active_id];
	return conn_info->ab;
    } else {
	return MobileTerminal::AccessBridge::_nil();
    }
}

void
WATMProxy::GTPTerminal::break_connectivity (CORBA::Boolean notify)
{
    if (_active_id != WATMGlue::IdGenerator::INVALID_ID && _connected
	&& !_establish_in_progress) {
	ConnInfo *conn_info = _all_conns[_active_id];
	if (notify) {
	    mobility_event(TERM_HANDOFF, _terminal_id,
			   MobileTerminal::AccessBridge::_nil());
	    _connected = FALSE;
	    close_conn(conn_info->conn);
	} else {
	    kill_conn(conn_info->conn);
	}
    }
}

CORBA::Boolean
WATMProxy::GTPTerminal::handoff (CORBA::Address *addr)
{
    if (addr == NULL)
	return FALSE;
    if (_active_id == WATMGlue::IdGenerator::INVALID_ID || !_connected
	|| _establish_in_progress)
	// ensure_connectivity() is for recovery
	return FALSE;
    ConnInfo *conn_info = _all_conns[_active_id];
    if (*conn_info->ab_addr == *addr)
	// Already there, dude
	return TRUE;
    GTPConn *conn = make_conn(addr->clone());
    if (conn == NULL)
	return FALSE;
    SequenceInfo *seq_info = _conn_statuses[_active_id];
    GTPOutContext out(conn->codec());
    set_seq_nos(out, conn);
    conn->codec()->put_recovery_request(out, GTP::TERMINAL_REQUEST,
					_terminal_id, _hla, conn_info->ab,
					conn_info->ttl, seq_info->last_rec,
					60UL);
    GTPProxy::output(out, conn);
    _establish_in_progress = TRUE;
    return TRUE;
}

/*************************** GTPNetwork ***************************/

const CORBA::Long WATMProxy::GTPNetwork::ForwardTimer::REC_TIMEOUT = 3L;

WATMProxy::GTPNetwork::GTPNetwork (CORBA::ORB_ptr orb,
				   vector<CORBA::Address *> addrs,
				   CORBA::UShort gtp_ver,
				   CosEventComm::PushConsumer_ptr chan)
    : GTPProxy(orb, gtp_ver, WATMGlue::IdGenerator(0, 2), chan)
{
    for (mico_vec_size_type i = 0; i < addrs.size(); ++i) {
	CORBA::TransportServer *tserv = addrs[i]->make_transport_server();
	if (!tserv->bind(addrs[i])) {
	    if (MICO::Logger::IsLogged(MICO::Logger::IIOP)) {
		MICO::Logger::Stream(MICO::Logger::IIOP)
		    << "Cannot bind to GTP address " << addrs[i]->stringify()
		    << ": " << tserv->errormsg() << endl;
	    }
	    continue;
	}
	tserv->block(_orb->dispatcher()->isblocking());
	tserv->aselect(_orb->dispatcher(), this);
	_tservers.push_back(tserv);
    }

    _ab_ref = MobileTerminal::AccessBridge::_nil();
    
}

WATMProxy::GTPNetwork::~GTPNetwork ()
{
    for (MapIdInfo::iterator i = _all_conns.begin();
	 i != _all_conns.end(); ++i) {
	if (i->second->is_up)
	    kill_conn(i->second->conn);
    }
    list<ConnInfo *> conn_infos;
    for (MapIdInfo::iterator ir = _all_conns.begin();
	 ir != _all_conns.end(); ++ir) {
	conn_infos.push_back(ir->second);
    }
    for (list<ConnInfo *>::iterator it = conn_infos.begin();
	 it != conn_infos.end(); ++it) {
	ReleaseTimer *timer = _rel_timers[(*it)->conn->id()];
	timer->force();
    }

    for (mico_vec_size_type iv = 0; iv < _tservers.size(); ++iv) {
	_tservers[iv]->aselect(_orb->dispatcher(), 0);
	delete _tservers[iv];
    }

    CORBA::release(_ab_ref);
}

CORBA::Boolean
WATMProxy::GTPNetwork::active (CORBA::ULong bridge_id)
{
    if (bridge_id == WATMGlue::IdGenerator::INVALID_ID)
	return FALSE;
    MapIdPair::iterator i = _bridge_to_proxy.find(bridge_id);
    if (i == _bridge_to_proxy.end())
	return FALSE;
    MapIdInfo::iterator it = _all_conns.find(i->second.first);
    if (it == _all_conns.end()) {
	delete_from_bridge(bridge_id);
	return FALSE;
    }
    return CORBA::is_nil(it->second->forward);
}

void
WATMProxy::GTPNetwork::kill_conn (GTPConn *conn)
{
    if (MICO::Logger::IsLogged(MICO::Logger::IIOP)) {
	MICO::Logger::Stream(MICO::Logger::IIOP)
	    << "Killing GTP connection " << conn->id() << endl;
    }
    for (MapIdInfo::iterator ii = _all_conns.begin();
	 ii != _all_conns.end(); ++ii) {
	if (ii->second->conn == conn) {
	    ii->second->is_up = FALSE;
	}
    }
    MapIdRTimer::iterator i = _rel_timers.find(conn->id());
    if (i != _rel_timers.end())
	i->second->activate();
    MapIdITimer::iterator it = _idle_timers.find(conn->id());
    if (it != _idle_timers.end())
	it->second->disable();
    conn->transport()->close();
}

void
WATMProxy::GTPNetwork::release_conn (GTPConn *conn)
{
    CORBA::ULong id = conn->id();
    MapIdSeq::iterator it = _conn_statuses.find(id);
    if (it != _conn_statuses.end()) {
	delete it->second;
	_conn_statuses.erase(it);
    }
    /*
     * This function is called by the release timer we destroy here.
     * Since nothing after the delete here or in the calling function
     * accesses the timer, we should be okay.  C++ gurus are welcome
     * to correct me.
     */
    MapIdRTimer::iterator i = _rel_timers.find(id);
    if (i != _rel_timers.end()) {
	delete i->second;
	_rel_timers.erase(i);
    }
    MapIdITimer::iterator itt = _idle_timers.find(id);
    if (itt != _idle_timers.end()) {
	delete itt->second;
	_idle_timers.erase(itt);
    }
    MapIdInfo::iterator iit = _all_conns.find(id);
    if (iit != _all_conns.end()) {
	MapTidId::iterator itit = _conn_ids.find(iit->second->terminal_id);
	if (itit != _conn_ids.end() && itit->second == conn->id()) {
	    _conn_ids.erase(itit);
	}
	delete iit->second;
	_all_conns.erase(iit);
    }
    _id_gen.release_id(id);
}

void
WATMProxy::GTPNetwork::conn_closed (GTPConn *conn)
{
    MapIdInfo::iterator i = _all_conns.find(conn->id());
    if (i != _all_conns.end()) {
	ConnInfo *conn_info = i->second;
	/*
	 * XXX - This isn't actually a foolproof test since
	 * conn_info->forward may still be NIL even though handoff is
	 * actually in progress.  With MIWCO Access Bridge this works.
	 */
	if (CORBA::is_nil(conn_info->forward) && conn_info->is_up) {
	    mobility_event(NET_DROPOUT, conn_info->terminal_id, _ab_ref);
	    kill_conn(conn);
	}
    }
}

CORBA::Boolean
WATMProxy::GTPNetwork::timed_release (GTPConn *conn)
{
    ConnInfo *conn_info = _all_conns[conn->id()];
    MapTidId::iterator it = _conn_ids.find(conn_info->terminal_id);
    if (!CORBA::is_nil(conn_info->hla) && CORBA::is_nil(conn_info->forward)
	&& it != _conn_ids.end() && conn->id() == it->second) {
	try {
	    return conn_info->hla->deregister_terminal(conn_info->terminal_id,
						       _ab_ref);
	} catch (const CORBA::Exception &) {
	    // Did not work, forget the terminal
	}
    }
    MapIdFTimer::iterator ifw = _pending_forwards.find(conn->id());
    if (ifw != _pending_forwards.end()) {
	ifw->second->force();
	delete ifw->second;
	_pending_forwards.erase(ifw);
    }
    return TRUE;
}

GTP::AccessStatus
WATMProxy::GTPNetwork::update_home (MobileTerminal::HomeLocationAgent_ptr hla,
				    CORBA::OctetSeq *tid)
{
    if (CORBA::is_nil(hla)) {
	return GTP::ACCESS_ACCEPT_LOCAL;
    } else {
	try {
	    hla->update_location(*tid, _ab_ref);
	    return GTP::ACCESS_ACCEPT;
	} catch (const CORBA::Exception &) {
	    return GTP::ACCESS_REJECT_LOCATION_UPDATE_FAILURE;
	}
    }
}

void
WATMProxy::GTPNetwork::output (GTPOutContext &out, GTPConn *conn)
{
    if (MICO::Logger::IsLogged(MICO::Logger::GIOP)) {
	MICO::Logger::Stream(MICO::Logger::GIOP)
	    << "Outputting on connection " << conn->id() << endl;
    }
    MapIdITimer::iterator i = _idle_timers.find(conn->id());
    if (i != _idle_timers.end())
	i->second->disable();
    CORBA::Buffer *buf = out._retn();
    MapIdInfo::iterator it = _all_conns.find(conn->id());
    if (it == _all_conns.end())
	return;
    ConnInfo *conn_info = it->second;
    if (!CORBA::is_nil(conn_info->forward)) {
	if (!is_forwardable(buf))
	    // XXX - we really should have some error handling
	    return;
	if (MICO::Logger::IsLogged(MICO::Logger::GIOP)) {
	    MICO::Logger::Stream(MICO::Logger::GIOP)
		<< "Forwarding output to new Access Bridge" << endl;
	}
	CORBA::OctetSeq gtp_message;
	gtp_message.length(buf->length());
	memcpy(gtp_message.get_buffer(), buf->data(), buf->length());
	try {
	    conn_info->forward->gtp_to_terminal(conn_info->terminal_id,
						_ab_ref, 0UL, gtp_message);
	} catch (const CORBA::Exception &) {
	    /*
	     * Our information on the terminal's current location is
	     * apparently wrong.  We should probably catch
	     * TerminalNotHere separately and do some recovery.
	     */
	}
	delete buf;
	return;
    }
    SequenceInfo *seq_info = _conn_statuses[conn->id()];
    CORBA::UShort own, peer;
    out.get_seq_nos(own, peer);
    seq_info->unacked.push_back(make_pair(own, new CORBA::Buffer(*buf)));
    if (conn_info->is_up && conn_info->terminal_id.length() > 0) {
	conn->output(buf);
	conn->idle(FALSE);
    } else {
	delete buf;
    }
}

WATMGlue::TargetAddress *
WATMProxy::GTPNetwork::map_to_forward (WATMGlue::TargetAddress *target,
				       CORBA::IOR *ior)
{
    CORBA::OctetSeq *tid = target_to_terminal_id(target);
    if (tid == NULL)
	return NULL;
    if (ior == NULL) {
	CORBA::IORProfile *prof;
	if (target->kind == WATMGlue::TargetAddress::Reference
	    && (prof = target->u.reference.ior->profile(CORBA::IORProfile::TAG_MOBILE_TERMINAL_IOP)) != NULL) {
	    CORBA::MultiComponent *mc = prof->components();
	    if (mc == NULL)
		return NULL;
	    CORBA::Component *comp =
		mc->component(CORBA::Component::TAG_HOME_LOCATION_INFO);
	    if (comp == NULL)
		return NULL;
	    WATM::HomeLocationComponent *hlc =
		dynamic_cast<WATM::HomeLocationComponent *>(comp);
	    if (hlc == NULL)
		return NULL;
	    CORBA::Object_ptr obj = hlc->hla();
	    if (CORBA::is_nil(obj))
		return NULL;
	    ior = obj->_ior();
	    if (ior == NULL)
		return NULL;
	} else {
	    return NULL;
	}
    }

    // Get all Inet addresses from ior;
    list<const MICO::InetAddress *> addrs;
    const CORBA::Address *addr = NULL;
    while ((addr = ior->addr(CORBA::IORProfile::TAG_INTERNET_IOP,
			     FALSE, addr)) != NULL) {
	addrs.push_back(dynamic_cast<const MICO::InetAddress *>(addr));
    }
    if (addrs.empty())
	return NULL;

    CORBA::IOR *retior;
    if (target->kind == WATMGlue::TargetAddress::Reference) {
	retior = new CORBA::IOR(*target->u.reference.ior);
	retior->active_profile(retior->get_profile(target->u.reference.index));
	CORBA::IORProfile *prof = NULL;
	const MICO::InetAddress *iaddr = NULL;
	while ((prof = retior->profile(CORBA::IORProfile::TAG_INTERNET_IOP,
				       FALSE, prof)) != NULL) {
	    MICO::IIOPProfile *iprof = dynamic_cast<MICO::IIOPProfile *>(prof);
	    if (!addrs.empty()) {
		iaddr = addrs.front();
		addrs.pop_front();
	    }
	    iprof->addr(*iaddr);
	}
    } else if (target->kind == WATMGlue::TargetAddress::Profile) {
	if (target->u.profile->id() != CORBA::IORProfile::TAG_INTERNET_IOP)
	    return NULL;
	retior = new CORBA::IOR;
	CORBA::IORProfile *prof = target->u.profile->clone();
	MICO::IIOPProfile *iprof = dynamic_cast<MICO::IIOPProfile *>(prof);
	iprof->addr(*addrs.front());
	retior->add_profile(iprof);
    } else if (target->kind == WATMGlue::TargetAddress::Key) {
	retior = new CORBA::IOR;
	CORBA::IORProfile *prof =
	    addrs.front()->make_ior_profile(target->u.key->get_buffer(),
					    target->u.key->length(),
					    CORBA::MultiComponent(), 0x0102);
	retior->add_profile(prof);
    } else {
	return NULL;
    }

    WATMGlue::TargetAddress *ret = new WATMGlue::TargetAddress;
    ret->kind = WATMGlue::TargetAddress::Reference;
    ret->u.reference.ior = retior;
    ret->u.reference.index = 0;
    retior->active_profile(&ret->u.reference.index);
    return ret;
}

void
WATMProxy::GTPNetwork::forward_old_messages (CORBA::ULong conn_id)
{
    MapIdInfo::iterator it = _all_conns.find(conn_id);
    if (it == _all_conns.end() || CORBA::is_nil(it->second->forward))
	return;
    MapIdSeq::iterator is = _conn_statuses.find(conn_id);
    if (is == _conn_statuses.end())
	return;
    GTPConn *conn = it->second->conn;
    SequenceInfo *seq_info = is->second;
    MessQueue unsent = seq_info->unacked;
    seq_info->unacked.clear();
    for (MessQueue::iterator iq = unsent.begin();
	 iq != unsent.end(); ++iq) {
	if (is_forwardable(iq->second)) {
	    GTPOutContext out(conn->codec(),
			       new CORBA::Buffer(*iq->second));
	    set_seq_nos(out, conn);
	    replace_seq_nos(out);
	    output(out, conn);
	}
    }
}

CORBA::Boolean
WATMProxy::GTPNetwork::open_connection (WATMGlue::BridgeRequest *br)
{
    MapIdPair::iterator i = _bridge_to_proxy.find(br->conn_id);
    if (i != _bridge_to_proxy.end())
	return TRUE;
    if (!br->target) {
	br->code = WATMGlue::BridgeRequestCallback::UNKNOWN;
	return FALSE;
    }
    CORBA::OctetSeq *tid = target_to_terminal_id(br->target);
    if (tid == NULL) {
	br->code = WATMGlue::BridgeRequestCallback::UNKNOWN;
	return FALSE;
    }
    MapTidId::iterator it = _conn_ids.find(*tid);
    delete tid;
    if (it == _conn_ids.end()) {
	WATMGlue::TargetAddress *target = br->target;
	br->target = map_to_forward(target);
	if (br->target != NULL) {
	    br->code = WATMGlue::BridgeRequestCallback::FORWARD;
	} else {
	    br->code = WATMGlue::BridgeRequestCallback::INVALID_ID;
	}
	delete target;
	return FALSE;
    }
    ConnInfo *conn_info = _all_conns[it->second];
    if (!CORBA::is_nil(conn_info->forward)) {
	WATMGlue::TargetAddress *target = br->target;
	br->target = map_to_forward(target, conn_info->forward->_ior());
	if (br->target != NULL) {
	    br->code = WATMGlue::BridgeRequestCallback::FORWARD;
	} else {
	    br->code = WATMGlue::BridgeRequestCallback::INVALID_ID;
	}
	delete target;
	return FALSE;
    }
    GTPConn *conn = conn_info->conn;
    CORBA::ULong req_id = _id_gen.new_id();

    GTPOutContext out(conn->codec());
    set_seq_nos(out, conn);
    conn->codec()->put_open_request(out, req_id, 0, br->target);
    output(out, conn);
    br->conn_id = req_id;
    _pending_requests.insert(make_pair(req_id,
				       vector<WATMGlue::BridgeRequest *>()));
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPNetwork::invoke (WATMGlue::BridgeRequest *br)
{
    GTPConn *conn;
    CORBA::ULong conn_id;

    if (br->conn_id == WATMGlue::IdGenerator::INVALID_ID) {
	br->code = WATMGlue::BridgeRequestCallback::INVALID_ID;
	return FALSE;
    }

    MapIdPair::iterator it = _bridge_to_proxy.find(br->conn_id);
    if (it == _bridge_to_proxy.end()) {
	MapIdReq::iterator ir = _pending_requests.find(br->conn_id);
	if (ir == _pending_requests.end()) {
	    br->code = WATMGlue::BridgeRequestCallback::INVALID_ID;
	    return FALSE;
	}
	ir->second.push_back(br);
	return TRUE;
    }
    MapIdInfo::iterator i = _all_conns.find(it->second.first);
    if (i == _all_conns.end()) {
	br->code = WATMGlue::BridgeRequestCallback::COMM_FAILURE;
	return FALSE;
    }
    conn = i->second->conn;
    conn_id = it->second.second;

    GTPOutContext out(conn->codec());
    set_seq_nos(out, conn);
    CORBA::ULong giop_id = _id_gen.new_id();
    CORBA::UShort own, peer;
    out.get_seq_nos(own, peer);
    _conn_statuses[conn->id()]->giops[own] = giop_id;
    conn->codec()->put_giop_data(out, conn_id, giop_id, br->giop_data);
    output(out, conn);
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPNetwork::close_connection (WATMGlue::BridgeRequest *br)
{
    MapIdPair::iterator it = _bridge_to_proxy.find(br->conn_id);
    if (it == _bridge_to_proxy.end()) {
	br->code = GTP::CLOSE_FAILED_INVALID_CONNECTION_ID;
	return FALSE;
    }
    if (_pending_closes.find(it->second.second) == _pending_closes.end()) {
	MapIdInfo::iterator i = _all_conns.find(it->second.first);
	if (i == _all_conns.end()) {
	    delete_from_bridge(br->conn_id);
	    br->code = WATMGlue::BridgeRequestCallback::COMM_FAILURE;
	    return FALSE;
	}

	GTPConn *conn = i->second->conn;
	CORBA::ULong conn_id = it->second.second;

	GTPOutContext out(conn->codec());
	set_seq_nos(out, conn);
	conn->codec()->put_close_request(out, conn_id);
	output(out, conn);
	_pending_closes.insert(conn_id);
    }
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPNetwork::handle_establish_request (GTPConn *conn,
						 GTPInContext &in)
{
    CORBA::OctetSeq_var tid = new CORBA::OctetSeq;
    MobileTerminal::HomeLocationAgent_var hla;
    MobileTerminal::AccessBridge_var old_ab;
    CORBA::ULong old_ttl, ttl;
    CORBA::UShort old_seq;
    GTP::RequestType req_type;
    GTP::ReplyType rep_type = GTP::INITIAL_REPLY;
    GTP::AccessStatus status = GTP::ACCESS_ACCEPT;
    CORBA::Boolean is_initial = TRUE;
    CORBA::Boolean inform_home = TRUE;
    CORBA::Boolean is_recovery = FALSE;
    SequenceInfo *seq_info = NULL;
    ConnInfo *conn_info;

    if (!conn->codec()->get_establish_request(in, req_type, tid, hla, old_ab,
					      old_ttl, old_seq, ttl)) {
	conn_error(conn, in, GTP::ERROR_PROTOCOL_ERROR);
	return FALSE;
    }
    if (!req_type_valid(req_type)) {
	conn_error(conn, in, GTP::ERROR_PROTOCOL_ERROR);
	return FALSE;
    }
    // We know that REC_TIMEOUT is positive
    if (ttl > (CORBA::ULong) ReleaseTimer::REC_TIMEOUT)
	ttl = ReleaseTimer::REC_TIMEOUT;

    MapTidId::iterator i = _conn_ids.find(*tid);
    if (i != _conn_ids.end() && req_type == GTP::RECOVERY_REQUEST) {
	// Recovery if we know the terminal and it isn't forwarded
	ConnInfo *old_conn_info = _all_conns[i->second];
	if (CORBA::is_nil(old_conn_info->forward)) {
	    is_recovery = TRUE;
	    inform_home = FALSE;
	}
    }
    if (inform_home)
	status = update_home(hla, tid);
    if (gtp_accepting(status) && req_type != GTP::INITIAL_REQUEST) {
	is_initial = FALSE;
	if (is_recovery) {
	    MapIdSeq::iterator it = _conn_statuses.find(i->second);
	    if (it == _conn_statuses.end()) {
		conn_error(conn, in, GTP::ERROR_UNKNOWN_FATAL_ERROR);
		return FALSE;
	    }
	    clear_unacked(it->second, old_seq);
	    seq_info = new SequenceInfo(*it->second);
	    old_seq = seq_info->last_rec;
	    /*
	     * Just call release_conn since we are only freeing
	     * resources and not actually releasing this connection.
	     */
	    release_conn(_all_conns[i->second]->conn);
	    delete _conn_statuses[conn->id()];
	    _conn_statuses.erase(conn->id());
	    _id_gen.release_id(conn->id());
	    conn->id(i->second);
	    _conn_statuses[conn->id()] = seq_info;
	    rep_type = GTP::RECOVERY_REPLY;
	    status = GTP::ACCESS_ACCEPT_RECOVERY;
	    mobility_event(NET_RECOVERY, *tid, old_ab);
	} else {
	    if (req_type == GTP::NETWORK_REQUEST) {
		try {
		    old_ab->handoff_completed(*tid,
					      MobileTerminal::HANDOFF_SUCCESS);
		    status = GTP::ACCESS_ACCEPT_HANDOFF;
		} catch (const CORBA::Exception &) {
		    status = GTP::ACCESS_REJECT_ACCESS_DENIED;
		}
		rep_type = GTP::NETWORK_REPLY;
	    } else if (req_type == GTP::TERMINAL_REQUEST) {
		try {
		    old_ab->handoff_in_progress(*tid, _ab_ref);
		    status = GTP::ACCESS_ACCEPT_HANDOFF;
		} catch (const CORBA::Exception &) {
		    status = GTP::ACCESS_REJECT_ACCESS_DENIED;
		}
		rep_type = GTP::TERMINAL_REPLY;
	    } else if (req_type == GTP::RECOVERY_REQUEST) {
		CORBA::UShort ab_seq;
		try {
		    old_ab->recovery_request(*tid, _ab_ref, old_seq, ab_seq);
		    old_seq = ab_seq;
		    status = GTP::ACCESS_ACCEPT_HANDOFF;
		} catch (const CORBA::Exception &) {
		    status = GTP::ACCESS_REJECT_RECOVERY_FAILURE;
		}
		rep_type = GTP::RECOVERY_REPLY;
	    }
	    if (gtp_accepting(status)) {
		mobility_event(NET_ARRIVE, *tid, old_ab);
		/*
		 * It actually follows from the specification that we
		 * have to reuse a previous connection identifier if
		 * we had one.
		 */
		if (i != _conn_ids.end()) {
		    release_conn(_all_conns[i->second]->conn);
		    _conn_statuses[i->second] = _conn_statuses[conn->id()];
		    _conn_statuses.erase(conn->id());
		    _id_gen.release_id(conn->id());
		    conn->id(i->second);
		}
	    }
	}
    }
    if (gtp_accepting(status)) {
	conn_info = new ConnInfo;
	conn_info->conn = conn;
	conn_info->is_up = TRUE;
	conn_info->terminal_id = *tid;
	conn_info->hla = hla._retn();
	conn_info->ttl = ttl;
	if (!CORBA::is_nil(old_ab) && !_ab_ref->_is_equivalent(old_ab)) {
	    conn_info->past_abs.push_back(MobileTerminal::AccessBridge::_duplicate(old_ab));
	}
	_all_conns[conn->id()] = conn_info;
	delete _rel_timers[conn->id()];
	_rel_timers[conn->id()] = new ReleaseTimer(_orb, this, conn, ttl);
	delete _idle_timers[conn->id()];
	_idle_timers[conn->id()] = new IdleTimer(_orb, this, conn);

	_conn_ids[*tid] = conn->id();
	if (is_initial) {
	    mobility_event(NET_ARRIVE, *tid,
			   MobileTerminal::AccessBridge::_nil());
	}
    }
    GTPOutContext out(conn->codec());
    set_seq_nos(out, conn);
    if (is_initial) {
	conn->codec()->put_initial_reply(out, status, _ab_ref, ttl);
    } else {
	conn->codec()->put_recovery_reply(out, rep_type, status, _ab_ref,
					  old_ttl, old_seq, ttl);
    }
    GTPProxy::output(out, conn);
    if (is_recovery && gtp_accepting(status)) {
	// We have initialised seq_info above under the same conditions
	MessQueue unsent = seq_info->unacked;
	seq_info->unacked.clear();
	for (MessQueue::iterator iq = unsent.begin();
	     iq != unsent.end(); ++iq) {
	    if (is_forwardable(iq->second)) {
		GTPOutContext fout(conn->codec(),
				   new CORBA::Buffer(*iq->second));
		set_seq_nos(fout, conn);
		replace_seq_nos(fout);
		GTPProxy::output(fout, conn);
	    }
	}
    }

    if (MICO::Logger::IsLogged(MICO::Logger::IIOP)) {
	MICO::Logger::Stream(MICO::Logger::IIOP)
	    << "Connection " << conn->id() << " established" << endl;
    }
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPNetwork::handle_release_request (GTPConn *conn, GTPInContext &in)
{
    CORBA::Boolean ret = GTPProxy::handle_release_request(conn, in);

    if (ret) {
	MapIdInfo::iterator it = _all_conns.find(conn->id());
	if (it == _all_conns.end())
	    return FALSE;
	ConnInfo *conn_info = it->second;
	if (CORBA::is_nil(conn_info->forward))
	    mobility_event(NET_DEPART, conn_info->terminal_id,
			   MobileTerminal::AccessBridge::_nil());
	if (!CORBA::is_nil(conn_info->forward)) {
	    for (ConnInfo::ABList::iterator i = conn_info->past_abs.begin();
		 i != conn_info->past_abs.end(); ++i) {
		(*i)->handoff_notice(conn_info->terminal_id,
				     conn_info->forward);
		CORBA::release(*i);
	    }
	    conn_info->past_abs.clear();
	}
    }
    return ret;
}

CORBA::Boolean
WATMProxy::GTPNetwork::handle_handoff_reply (GTPConn *conn, GTPInContext &in)
{
    MobileTerminal::HandoffStatus status;

    if (!conn->codec()->get_handoff_completed(in, status)) {
	conn_error(conn, in, GTP::ERROR_PROTOCOL_ERROR);
	return FALSE;
    }
    /*
     * XXX - report HANDOFF_FAILURE if needed
     */
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPNetwork::handle_gtp_forward (GTPConn *conn, GTPInContext &in)
{
    MobileTerminal::AccessBridge_var ab;
    CORBA::ULong mesg_id;
    CORBA::Buffer *buf = new CORBA::Buffer;

    if (!conn->codec()->get_gtp_forward(in, ab, mesg_id, buf)) {
	conn_error(conn, in, GTP::ERROR_PROTOCOL_ERROR);
	return FALSE;
    }
    CORBA::OctetSeq_var gtp_encap = new CORBA::OctetSeq(buf->length());
    gtp_encap->length(buf->length());
    memcpy(gtp_encap->get_buffer(), buf->data(), buf->length());
    delete buf;
    ConnInfo *conn_info = _all_conns[conn->id()];
    GTP::ForwardStatus status = GTP::FORWARD_SUCCESS;
    try {
	ab->gtp_from_terminal(conn_info->terminal_id, mesg_id, gtp_encap);
    } catch (const MobileTerminal::UnknownTerminalId &) {
	status = GTP::FORWARD_ERROR_UNKNOWN_SENDER;
    } catch (const CORBA::Exception &) {
	status = GTP::FORWARD_ERROR_ACCESS_BRIDGE_UNREACHABLE;
    }
    GTPOutContext out(conn->codec());
    set_seq_nos(out, conn);
    conn->codec()->put_gtp_forward_reply(out, mesg_id, status);
    output(out, conn);
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPNetwork::handle_gtp_forward_reply (GTPConn *conn,
						 GTPInContext &in)
{
    CORBA::ULong mesg_id;
    GTP::ForwardStatus status;

    if (!conn->codec()->get_gtp_forward_reply(in, mesg_id, status)) {
	conn_error(conn, in, GTP::ERROR_PROTOCOL_ERROR);
	return FALSE;
    }
    MapIdForw::iterator i = _forwarded_gtps.find(mesg_id);
    if (i != _forwarded_gtps.end()) {
	try {
	    i->second.second->gtp_acknowledge(i->second.first, status);
	} catch (const CORBA::Exception &) {
	}
	CORBA::release(i->second.second);
	_id_gen.release_id(mesg_id);
    }
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPNetwork::is_here (CORBA::OctetSeq terminal_id)
{
    MapTidId::iterator it = _conn_ids.find(terminal_id);
    if (it == _conn_ids.end())
	return FALSE;
    ConnInfo *conn_info = _all_conns[it->second];
    return CORBA::is_nil(conn_info->forward);
}

CORBA::Boolean
WATMProxy::GTPNetwork::is_known (CORBA::OctetSeq terminal_id)
{
    return _conn_ids.find(terminal_id) != _conn_ids.end();
}

MobileTerminal::HomeLocationAgent_ptr
WATMProxy::GTPNetwork::terminal_home (CORBA::OctetSeq terminal_id)
{
    MapTidId::iterator it = _conn_ids.find(terminal_id);
    if (it == _conn_ids.end())
	return MobileTerminal::HomeLocationAgent::_nil();
    MapIdInfo::iterator i = _all_conns.find(it->second);
    if (i == _all_conns.end())
	return MobileTerminal::HomeLocationAgent::_nil();
    return i->second->hla;
}

void
WATMProxy::GTPNetwork::abselect (MobileTerminal::AccessBridge_ptr ab)
{
    CORBA::release(_ab_ref);
    _ab_ref = MobileTerminal::AccessBridge::_duplicate(ab);
}

CORBA::Boolean
WATMProxy::GTPNetwork::change_forward (MobileTerminal::TerminalId terminal_id,
				       MobileTerminal::AccessBridge_ptr new_ab)
{
    CORBA::Boolean ret = FALSE;
    MapTidId::iterator i = _conn_ids.find(terminal_id);
    if (i != _conn_ids.end()) {
	ConnInfo *conn_info = _all_conns[i->second];
	if (!CORBA::is_nil(conn_info->forward)) {
	    CORBA::release(conn_info->forward);
	    conn_info->forward =
		MobileTerminal::AccessBridge::_duplicate(new_ab);
	    ret = TRUE;
	}
    }
    return ret;
}

CORBA::Boolean
WATMProxy::GTPNetwork::do_handoff (MobileTerminal::TerminalId terminal_id,
				   MobileTerminal::AccessBridgeTransportAddressList *ab_addrs)
{
    MapTidId::iterator i = _conn_ids.find(terminal_id);
    if (i == _conn_ids.end())
	return FALSE;
    GTPConn *conn = _all_conns[i->second]->conn;
    GTPOutContext out(conn->codec());
    set_seq_nos(out, conn);
    conn->codec()->put_handoff_request(out, *ab_addrs);
    output(out, conn);
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPNetwork::add_notice (MobileTerminal::TerminalId terminal_id,
				   MobileTerminal::AccessBridge_ptr ab)
{
    MapTidId::iterator i = _conn_ids.find(terminal_id);
    if (i == _conn_ids.end())
	return FALSE;
    MapIdInfo::iterator it = _all_conns.find(i->second);
    if (it == _all_conns.end() || !CORBA::is_nil(it->second->forward))
	return FALSE;
    if (!CORBA::is_nil(ab) && !_ab_ref->_is_equivalent(ab)) {
	it->second->past_abs.push_back(MobileTerminal::AccessBridge::_duplicate(ab));
    }
    return TRUE;
}

void
WATMProxy::GTPNetwork::notify_handoff (MobileTerminal::TerminalId terminal_id)
{
    MapTidId::iterator i = _conn_ids.find(terminal_id);
    if (i == _conn_ids.end())
	return;
    MapIdInfo::iterator it = _all_conns.find(i->second);
    if (it == _all_conns.end() || CORBA::is_nil(it->second->forward))
	return;
    for (ConnInfo::ABList::iterator ai = it->second->past_abs.begin();
	 ai != it->second->past_abs.end(); ++ai) {
	(*ai)->handoff_notice(terminal_id, it->second->forward);
	CORBA::release(*ai);
    }
    it->second->past_abs.clear();
}

void
WATMProxy::GTPNetwork::prepare_forwarding (MobileTerminal::TerminalId tid)
{
    MapTidId::iterator i = _conn_ids.find(tid);
    if (i == _conn_ids.end())
	return;
    _pending_forwards[i->second] = new ForwardTimer(_orb, this, i->second);
}

CORBA::ULong
WATMProxy::GTPNetwork::terminal_moved (MobileTerminal::TerminalId terminal_id,
				       MobileTerminal::AccessBridge_ptr new_ab,
				       CORBA::ULong seqno)
{
    MapTidId::iterator it = _conn_ids.find(terminal_id);
    if (it == _conn_ids.end())
	return 0xFFFFFFFFUL;
    MapIdInfo::iterator iit = _all_conns.find(it->second);
    if (iit == _all_conns.end())
	return 0xFFFFFFFFUL;
    ConnInfo *conn_info = iit->second;
    CORBA::release(conn_info->forward);
    conn_info->forward = MobileTerminal::AccessBridge::_duplicate(new_ab);
    SequenceInfo *seq_info = _conn_statuses[it->second];
    if (seqno <= 0xFFFF) {
	clear_unacked(seq_info, seqno);
    } else {
	for (MessQueue::iterator i = seq_info->unacked.begin();
	     i != seq_info->unacked.end(); ++i)
	    delete i->second;
	seq_info->unacked.erase(seq_info->unacked.begin(),
				seq_info->unacked.end());
    }
    return seq_info->last_rec;
}

CORBA::Boolean
WATMProxy::GTPNetwork::terminal_forward (MobileTerminal::TerminalId term_id,
					 MobileTerminal::AccessBridge_ptr ab,
					 CORBA::ULong mesg_id,
					 MobileTerminal::GTPEncapsulation mesg)
{
    MapTidId::iterator it = _conn_ids.find(term_id);
    if (it == _conn_ids.end())
	return FALSE;
    ConnInfo *conn_info = _all_conns[it->second];
    GTPConn *conn = conn_info->conn;
    CORBA::Buffer *buf = new CORBA::Buffer;
    buf->resize(mesg.length());
    buf->put(mesg.get_buffer(), mesg.length());
    CORBA::ULong gen_id = _id_gen.new_id();
    _forwarded_gtps[gen_id] =
	make_pair(mesg_id, MobileTerminal::AccessBridge::_duplicate(ab));
    GTPOutContext out(conn->codec());
    set_seq_nos(out, conn);
    conn->codec()->put_gtp_forward(out, ab, gen_id, buf);
    output(out, conn);
    delete buf;
    return TRUE;
}

CORBA::Boolean
WATMProxy::GTPNetwork::net_forward (MobileTerminal::TerminalId terminal_id,
				    MobileTerminal::GTPEncapsulation message)
{
    MapTidId::iterator it = _conn_ids.find(terminal_id);
    if (it == _conn_ids.end())
	return FALSE;
    ConnInfo *conn_info = _all_conns[it->second];
    CORBA::Buffer *buf = new CORBA::Buffer;
    buf->resize(message.length());
    buf->put(message.get_buffer(), message.length());
    return handle_input(conn_info->conn, buf);
}

void
WATMProxy::GTPNetwork::callback (CORBA::TransportServer *tserv,
				 CORBA::TransportServerCallback::Event ev)
{
    switch (ev) {
    case CORBA::TransportServerCallback::Accept: {
	CORBA::Transport *trans = tserv->accept();
	if (trans) {
	    if (MICO::Logger::IsLogged(MICO::Logger::IIOP)) {
		MICO::Logger::Stream(MICO::Logger::IIOP)
		    << "Received GTP connection request from "
		    << trans->peer()->stringify() << endl;
	    }
	    if (trans->bad()) {
		delete trans;
		break;
	    }
	    GTPConn *conn = new GTPConn(_orb, trans, this,
					new GTPCodec(new MICO::CDRDecoder,
						     new MICO::CDREncoder,
						     _gtp_ver),
					0L, _id_gen.new_id());
	    _conn_statuses[conn->id()] = new SequenceInfo;
	}
	break;
    }
    default:
	assert(0);
    }
}
