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

#include <coss/GTP.h>
#include <mico/impl.h>
#include "glue.h"
#include "bridge.h"

using namespace std;

namespace {

CORBA::Boolean
whole_giop_message (CORBA::Buffer *buf)
{
    if (buf->data()[4] == 1 && buf->data()[5] == 0)
	return TRUE;
    return (buf->data()[6] & 0x02) == 0;
}

}

/*************************** GIOPBridge ***************************/

WATMBridge::GIOPBridge::GIOPBridge (CORBA::ORB_ptr orb,
				    vector<CORBA::Address *> addrs,
				    WATMGlue::BridgeRequestCallback *proxy,
				    WATMGlue::IdGenerator id_gen)
    : _id_gen(id_gen)
{
    _orb = orb;
    _upcall = new MICO::IIOPServer(orb);
    _proxy = proxy;
    _giop_ver = 0x0102;

    for (mico_vec_size_type i = 0; i < addrs.size(); ++i) {
	CORBA::IORProfile *prof;
	CORBA::TransportServer *tserv = addrs[i]->make_transport_server();
	if (!tserv->bind(addrs[i])) {
	    if (MICO::Logger::IsLogged(MICO::Logger::IIOP)) {
		MICO::Logger::Stream(MICO::Logger::IIOP)
		    << "Cannot bind to " << addrs[i]->stringify()
		    << ": " << tserv->errormsg() << endl;
	    }
	    continue;
	}
	tserv->block(_orb->dispatcher()->isblocking());
	tserv->aselect(_orb->dispatcher(), this);

	prof = tserv->addr()->make_ior_profile((CORBA::Octet *) "", 1,
					       CORBA::MultiComponent(),
					       _giop_ver);

	if (MICO::Logger::IsLogged (MICO::Logger::IIOP)) {
	    MICO::Logger::Stream (MICO::Logger::IIOP)
		<< "IIOP: server listening on "
		<< tserv->addr()->stringify()
		<< " IIOP version "
		<< (_giop_ver >> 8) << "." << (_giop_ver & 255)
		<< endl;
	}

	_orb->ior_template()->add_profile(prof);

	_tservers.push_back(tserv);
    }

}

WATMBridge::GIOPBridge::~GIOPBridge ()
{
    for (MapCidConn::iterator i = _conns.begin(); i != _conns.end(); ++i) {
	delete i->second;
    }
    for (MapCidReqMap::iterator it = _conn_addrs.begin();
	 it != _conn_addrs.end(); ++it) {
	for (MapReqTarget::iterator j = it->second.begin();
	     j != it->second.end(); ++j) {
	    delete j->second;
	}
    }

    for (mico_vec_size_type iv = 0; iv < _tservers.size(); ++iv) {
	_tservers[iv]->aselect(_orb->dispatcher(), 0);
	delete _tservers[iv];
    }

    delete _upcall;
}

CORBA::Boolean
WATMBridge::GIOPBridge::local_target (WATMGlue::TargetAddress *target)
{
    CORBA::OctetSeq key;

    switch (target->kind) {
    case WATMGlue::TargetAddress::Key:
	key.length(target->u.key->length());
	memcpy(key.get_buffer(), target->u.key->get_buffer(),
	       target->u.key->length());
	break;
    case WATMGlue::TargetAddress::Profile: {
	CORBA::Long len;
	const CORBA::Octet *pkey = target->u.profile->objectkey(len);
	key.length(len);
	memcpy(key.get_buffer(), pkey, len);
	break;
    }
    case WATMGlue::TargetAddress::Reference: {
	CORBA::Long len;
	const CORBA::Octet *pkey = target->u.reference.ior->get_profile(target->u.reference.index)->objectkey(len);
	key.length(len);
	memcpy(key.get_buffer(), pkey, len);
	break;
    }
    case WATMGlue::TargetAddress::Nil:
	return FALSE;
    }
    return _local_keys.find(key) != _local_keys.end();
}

CORBA::Boolean
WATMBridge::GIOPBridge::local_input (MICO::GIOPConn *conn, CORBA::Buffer *buf)
{
    return _upcall->input_callback(conn, buf);
}

CORBA::ULong
WATMBridge::GIOPBridge::put_header (CORBA::DataEncoder *ec, GIOP::MsgType mt,
				    CORBA::UShort giop_ver)
{
    CORBA::ULong offset;

    ec->struct_begin();
    {
	ec->arr_begin();
	{
	    ec->put_chars_raw((CORBA::Char *)"GIOP", 4);
	}
	ec->arr_end();
	ec->struct_begin();
	{
	    ec->put_octet((CORBA::Octet)(giop_ver >> 8));
	    ec->put_octet((CORBA::Octet)giop_ver);
	}
	ec->struct_end();
	ec->put_octet(ec->byteorder() == CORBA::LittleEndian
		      ? GIOP_BYTEORDER_BIT : 0);
	ec->put_octet(mt);
	offset = ec->buffer()->wpos();
	ec->put_ulong(0);
    }
    ec->struct_end();

    return offset;
}

void
WATMBridge::GIOPBridge::put_size (CORBA::DataEncoder *ec, CORBA::ULong offset)
{
    CORBA::ULong end_pos = ec->buffer()->wpos();
    ec->buffer()->wseek_beg(offset);
    ec->put_ulong(end_pos - ec->buffer()->wpos() - 4);
    ec->buffer()->wseek_beg(end_pos);
}

void
WATMBridge::GIOPBridge::send_req_addr_disp (MICO::GIOPConn *conn,
					    CORBA::ULong req_id)
{
    MICO::GIOPOutContext out(conn->codec());
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong offset =
	put_header(ec, GIOP::Reply, conn->codec()->version());

    ec->struct_begin();
    {
	ec->put_ulong(req_id);
	ec->enumeration(GIOP::NEEDS_ADDRESSING_MODE);
	ec->seq_begin(0);
	ec->seq_end();
    }
    ec->struct_end();

    ec->put_short(GIOP::ReferenceAddr);
    put_size(ec, offset);

    conn->output(out._retn());
}

void
WATMBridge::GIOPBridge::send_loc_addr_disp (MICO::GIOPConn *conn,
					    CORBA::ULong req_id)
{
    MICO::GIOPOutContext out(conn->codec());
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong offset =
	put_header(ec, GIOP::LocateReply, conn->codec()->version());

    ec->struct_begin();
    {
	ec->put_ulong(req_id);
	ec->enumeration(GIOP::LOC_NEEDS_ADDRESSING_MODE);
    }
    ec->struct_end();

    ec->put_short(GIOP::ReferenceAddr);
    put_size(ec, offset);

    conn->output(out._retn());
}

void
WATMBridge::GIOPBridge::send_req_nonexist (MICO::GIOPConn *conn,
					   CORBA::ULong req_id)
{
    MICO::GIOPOutContext out(conn->codec());
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong offset = put_header(ec, GIOP::Reply,
				     conn->codec()->version());

    ec->struct_begin();
    {
	if (conn->codec()->version() < 0x0102) {
	    ec->seq_begin(0);
	    ec->seq_end();
	    ec->put_ulong(req_id);
	    ec->enumeration(GIOP::SYSTEM_EXCEPTION);
	} else {
	    ec->put_ulong(req_id);
	    ec->enumeration(GIOP::SYSTEM_EXCEPTION);
	    ec->seq_begin(0);
	    ec->seq_end();
	}
    }
    ec->struct_end();

    ec->put_string_raw("IDL:omg.org/CORBA/OBJECT_NOT_EXIST:1.0");
    ec->put_ulong(0);
    ec->put_ulong(CORBA::COMPLETED_NO);
    put_size(ec, offset);

    conn->output(out._retn());
}

void
WATMBridge::GIOPBridge::send_loc_nonexist (MICO::GIOPConn *conn,
					   CORBA::ULong req_id)
{
    MICO::GIOPOutContext out(conn->codec());
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong offset = put_header(ec, GIOP::LocateReply,
				     conn->codec()->version());

    ec->struct_begin();
    {
	ec->put_ulong(req_id);
	ec->enumeration(GIOP::UNKNOWN_OBJECT);
    }
    ec->struct_end();

    put_size(ec, offset);

    conn->output(out._retn());
}

void
WATMBridge::GIOPBridge::send_req_forw (MICO::GIOPConn *conn,
				       CORBA::ULong req_id,
				       CORBA::IOR *forward)
{
    MICO::GIOPOutContext out(conn->codec());
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong offset = put_header(ec, GIOP::Reply,
				     conn->codec()->version());

    ec->struct_begin();
    {
	if (conn->codec()->version() < 0x0102) {
	    ec->seq_begin(0);
	    ec->seq_end();
	    ec->put_ulong(req_id);
	    ec->enumeration(GIOP::LOCATION_FORWARD);
	} else {
	    ec->put_ulong(req_id);
	    ec->enumeration(GIOP::LOCATION_FORWARD);
	    ec->seq_begin(0);
	    ec->seq_end();
	}
    }
    ec->struct_end();

    ec->put_ior(*forward);
    put_size(ec, offset);

    conn->output(out._retn());
}

void
WATMBridge::GIOPBridge::send_loc_forw (MICO::GIOPConn *conn,
				       CORBA::ULong req_id,
				       CORBA::IOR *forward)
{
    MICO::GIOPOutContext out(conn->codec());
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong offset = put_header(ec, GIOP::LocateReply,
				     conn->codec()->version());

    ec->struct_begin();
    {
	ec->put_ulong(req_id);
	ec->enumeration(GIOP::OBJECT_FORWARD);
    }
    ec->struct_end();

    ec->put_ior(*forward);
    put_size(ec, offset);

    conn->output(out._retn());
}

#define check(exp) if (!(exp)) return FALSE

CORBA::Boolean
WATMBridge::GIOPBridge::skip_contextlist (MICO::GIOPInContext &in,
					  MICO::GIOPCodec *codec)
{
    CORBA::DataDecoder *dc = in.dc();
    CORBA::ULong len;
    CORBA::Octet buf[256];

    check(dc->seq_begin(len));
    {
	CORBA::ULong context_id;
	CORBA::ULong context_len;
	for (CORBA::ULong i = 0; i < len; ++i) {
	    check(dc->struct_begin());
	    {
		check(dc->get_ulong(context_id));
		check(dc->seq_begin(context_len));
		{
		    while (context_len > sizeof buf) {
			check(dc->get_octets(buf, sizeof buf));
			context_len -= sizeof buf;
		    }
		    check(dc->get_octets(buf, context_len));
		}
		check(dc->seq_end());
	    }
	    check(dc->struct_end());
	}
    }
    check(dc->seq_end());

    return TRUE;
}

#undef check

#define check(exp) if (!(exp)) do { delete ret; return NULL; } while (0)

WATMGlue::TargetAddress *
WATMBridge::GIOPBridge::get_request_target (MICO::GIOPInContext &in,
					    CORBA::ULong &req_id,
					    MICO::GIOPCodec *codec)
{
    CORBA::DataDecoder *dc = in.dc();
    WATMGlue::TargetAddress *ret = new WATMGlue::TargetAddress;
    CORBA::Octet flags;

    if (codec->version() < 0x0102) {
	CORBA::ULong keylen;

	check(skip_contextlist(in, codec));
	check(dc->get_ulong(req_id));
	check(dc->get_boolean(flags));
	if (codec->version() != 0x0100) {
	    CORBA::Octet o[3];
	    check(dc->get_octets(o, 3));
	}
	check(dc->seq_begin(keylen));
	{
	    ret->u.key = new CORBA::OctetSeq(keylen);
	    ret->kind = WATMGlue::TargetAddress::Key;
	    ret->u.key->length(keylen);
	    check(dc->get_octets(ret->u.key->get_buffer(), keylen));
	}
	check(dc->seq_end());
    } else {
	check(dc->get_ulong(req_id));
	check(dc->get_octet(flags));
	CORBA::Octet o[3];
	check(dc->get_octets(o, 3));
	check(ret->decode(*dc));
    }
    return ret;
}

WATMGlue::TargetAddress *
WATMBridge::GIOPBridge::get_locate_target (MICO::GIOPInContext &in,
					   CORBA::ULong &req_id,
					   MICO::GIOPCodec *codec)
{
    CORBA::DataDecoder *dc = in.dc();
    WATMGlue::TargetAddress *ret = new WATMGlue::TargetAddress;

    if (codec->version() < 0x0102) {
	CORBA::ULong keylen;
	check(dc->get_ulong(req_id));
	check(dc->seq_begin(keylen));
	{
	    ret->u.key = new CORBA::OctetSeq(keylen);
	    ret->kind = WATMGlue::TargetAddress::Key;
	    ret->u.key->length(keylen);
	    check(dc->get_octets(ret->u.key->get_buffer(), keylen));
	}
	check(dc->seq_end());
    } else {
	check(dc->get_ulong(req_id));
	check(ret->decode(*dc));
    }
    return ret;
}

#undef check

#define check(exp) if (!(exp)) return NULL

WATMGlue::BridgeRequest *
WATMBridge::GIOPBridge::get_target (MICO::GIOPInContext &in,
				    MICO::GIOPConn *conn)
{
    CORBA::DataDecoder *dc = in.dc();
    GIOP::MsgType mt;
    CORBA::ULong size;
    CORBA::Octet flags;
    if (!conn->codec()->get_header(in, mt, size, flags)) {
	conn_error(conn);
	return NULL;
    }

    WATMGlue::BridgeRequest *ret;
    WATMGlue::TargetAddress *target;
    CORBA::ULong req_id;
    check(dc->struct_begin());
    switch (mt) {
    case GIOP::Request:
	check(target = get_request_target(in, req_id, conn->codec()));
	if (conn->codec()->version() >= 0x0102
	    && target->kind != WATMGlue::TargetAddress::Reference
	    && (flags & 0x01)) {
	    send_req_addr_disp(conn, req_id);
	    delete target;
	    return NULL;
	}
	ret = new WATMGlue::BridgeRequest;
	ret->target = target;
	ret->data_type = GIOP::Request;
	ret->req_id = req_id;
	_conn_addrs[conn->id()][req_id] = ret;
	return ret;
    case GIOP::LocateRequest:
	check(target = get_locate_target(in, req_id, conn->codec()));
	if (conn->codec()->version() >= 0x0102
	    && target->kind != WATMGlue::TargetAddress::Reference
	    && (flags & 0x01)) {
	    send_loc_addr_disp(conn, req_id);
	    delete target;
	    return NULL;
	}
	ret = new WATMGlue::BridgeRequest;
	ret->target = target;
	ret->data_type = GIOP::LocateRequest;
	ret->req_id = req_id;
	_conn_addrs[conn->id()][req_id] = ret;
	return ret;
    case GIOP::Fragment: {
	check(conn->codec()->version() >= 0x0102);
	check(dc->get_ulong(req_id));
	MapReqTarget::iterator it = _conn_addrs[conn->id()].find(req_id);
	check(it != _conn_addrs[conn->id()].end());
	it->second->data_type = GIOP::Fragment;
	return it->second;
    }
    case GIOP::Reply:
	if (conn->codec()->version() < 0x0102)
	    check(skip_contextlist(in, conn->codec()));
	/* FALLTHROUGH */
    case GIOP::CancelRequest:
    case GIOP::LocateReply: {
	check(dc->get_ulong(req_id));
	MapReqTarget::iterator it = _conn_addrs[conn->id()].find(req_id);
	check(it != _conn_addrs[conn->id()].end());
	it->second->data_type = mt;
	return it->second;
    }
    case GIOP::CloseConnection:
    case GIOP::MessageError:
    default:
	return NULL;
    }
}

#undef check

#define check(exp) if (!(exp)) return FALSE

CORBA::Boolean
WATMBridge::GIOPBridge::transform_request_id (MICO::GIOPConn *conn,
					      WATMGlue::BridgeRequest *br)
{
    MICO::GIOPInContext in(conn->codec(), new CORBA::Buffer(*br->giop_data));
    GIOP::MsgType mt;
    CORBA::ULong size;
    CORBA::Octet flags;
    CORBA::ULong req_id;
    CORBA::ULong start = in.dc()->buffer()->rpos();
    if (!conn->codec()->get_header(in, mt, size, flags))
	return FALSE;

    if (mt == GIOP::Request) {
	CORBA::Octet resp;
	CORBA::ORBRequest *req;
	CORBA::Principal pr;
	CORBA::Object_ptr obj = new CORBA::Object(new CORBA::IOR);

	CORBA::Codeset::disable(TRUE);
	if (!conn->codec()->get_invoke_request(in, req_id, resp,
					       obj, req, &pr)) {
	    CORBA::release(obj);
	    return FALSE;
	}
	CORBA::ULong new_id = _id_gen.new_id();
	_request_proxy[conn->id()][req_id] = new_id;
	_proxy_request[new_id].first = req_id;
	_proxy_request[new_id].second = 0;
	if (!obj->_ior_fwd()->active_profile())
	    obj->_ior_fwd()->active_profile(obj->_ior_fwd()->profile());
	/*
	 * The latter should always be the case when version downgrade
	 * is needed.
	 */
	if (need_version_downgrade()
	    && (obj->_ior_fwd()->addressing_disposition()
		== GIOP::ReferenceAddr)) {
	    CORBA::IORProfile *prof = obj->_ior_fwd()->active_profile();
	    MICO::IIOPProfile *iprof = dynamic_cast<MICO::IIOPProfile *>(prof);
	    if (iprof != NULL
		&& iprof->iiop_version() < conn->codec()->version()) {
		_proxy_request[new_id].second = conn->codec()->version();
		conn->codec()->version(iprof->iiop_version());
	    }
	}
	MICO::GIOPOutContext out(conn->codec());
	out.ec()->byteorder((flags & 1)
			    ? CORBA::LittleEndian : CORBA::BigEndian);
	conn->codec()->put_invoke_request(out, new_id, resp, obj, req, &pr);
	delete br->giop_data;
	br->giop_data = out._retn();
    } else if (mt == GIOP::LocateRequest) {
	CORBA::Object_ptr obj = new CORBA::Object(new CORBA::IOR);
	if (!conn->codec()->get_locate_request(in, req_id, obj)) {
	    CORBA::release(obj);
	    return FALSE;
	}
	CORBA::ULong new_id = _id_gen.new_id();
	_request_proxy[conn->id()][req_id] = new_id;
	_proxy_request[new_id].first = req_id;
	_proxy_request[new_id].second = 0;
	if (!obj->_ior_fwd()->active_profile())
	    obj->_ior_fwd()->active_profile(obj->_ior_fwd()->profile());
	/*
	 * The latter should always be the case when version downgrade
	 * is needed.
	 */
	if (need_version_downgrade()
	    && (obj->_ior_fwd()->addressing_disposition()
		== GIOP::ReferenceAddr)) {
	    CORBA::IORProfile *prof = obj->_ior_fwd()->active_profile();
	    MICO::IIOPProfile *iprof = dynamic_cast<MICO::IIOPProfile *>(prof);
	    if (iprof != NULL
		&& iprof->iiop_version() < conn->codec()->version()) {
		_proxy_request[new_id].second = conn->codec()->version();
		conn->codec()->version(iprof->iiop_version());
	    }
	}
	MICO::GIOPOutContext out(conn->codec());
	out.ec()->byteorder((flags & 1)
			    ? CORBA::LittleEndian : CORBA::BigEndian);
	conn->codec()->put_locate_request(out, new_id, obj);
	delete br->giop_data;
	br->giop_data = out._retn();
    } else if (mt == GIOP::CancelRequest) {
	MapIdId req_id_map = _request_proxy[conn->id()];
	CORBA::ULong end = in.dc()->buffer()->rpos();
	check(in.dc()->get_ulong(req_id));
	check(req_id_map.find(req_id) != req_id_map.end());
	CORBA::ULong wend = br->giop_data->wpos();
	br->giop_data->wseek_beg(end - start);
	MICO::CDREncoder ec(br->giop_data, FALSE, in.dc()->byteorder(),
			    NULL, TRUE, NULL, TRUE);
	ec.put_ulong(req_id_map[req_id]);
	br->giop_data->wseek_beg(wend);
    } else if (mt == GIOP::Fragment) {
	MapIdId req_id_map = _request_proxy[conn->id()];
	CORBA::ULong end = in.dc()->buffer()->rpos();
	check(in.dc()->get_ulong(req_id));
	// This would mean that the fragment is a part of a reply
	if (req_id_map.find(req_id) == req_id_map.end())
	    return TRUE;
	CORBA::ULong wend = br->giop_data->wpos();
	br->giop_data->wseek_beg(end - start);
	MICO::CDREncoder ec(br->giop_data, FALSE, in.dc()->byteorder(),
			    NULL, TRUE, NULL, TRUE);
	ec.put_ulong(req_id_map[req_id]);
	br->giop_data->wseek_beg(wend);
    }
    return TRUE;
}

CORBA::Boolean
WATMBridge::GIOPBridge::revert_request_id (MICO::GIOPConn *conn,
					   WATMGlue::BridgeRequest *br)
{
    MICO::GIOPInContext in(conn->codec(), new CORBA::Buffer(*br->giop_data));
    GIOP::MsgType mt;
    CORBA::ULong size;
    CORBA::Octet flags;
    CORBA::ULong start = in.dc()->buffer()->rpos();
    CORBA::ULong req_id;
    if (!conn->codec()->get_header(in, mt, size, flags)) {
	conn_error(conn);
	return FALSE;
    }

    /*
     * XXX - Reply and LocateReply need to be transformed to version
     * 1.2 or higher if we are on the terminal.
     */
    if (mt == GIOP::Reply) {
	GIOP::ReplyStatusType stat;
	IOP::ServiceContextList ctx;
	CORBA::Codeset::disable(TRUE);
	if (!conn->codec()->get_invoke_reply1(in, req_id, stat, ctx)) {
	    return FALSE;
	}
	check(_proxy_request.find(req_id) != _proxy_request.end());
	CORBA::Object_ptr obj = CORBA::Object::_nil();
	GIOP::AddressingDisposition ad = GIOP::KeyAddr;
	MICO::GIOPRequest *req =
	    new MICO::GIOPRequest("foo", in._retn(), conn->codec());
	if (!conn->codec()->get_invoke_reply2(in, req_id, stat, obj,
					      req, ad, ctx)) {
	    delete req;
	    return FALSE;
	}
	if (_proxy_request[req_id].second != 0)
	    conn->codec()->version(_proxy_request[req_id].second);
	MICO::GIOPOutContext out(conn->codec());
	out.ec()->byteorder((flags & 1)
			    ? CORBA::LittleEndian : CORBA::BigEndian);
	conn->codec()->put_invoke_reply(out, _proxy_request[req_id].first,
					stat, obj, req, ad);
	delete req;
	delete br->giop_data;
	br->giop_data = out._retn();
    } else if (mt == GIOP::LocateReply) {
	GIOP::LocateStatusType stat;
	CORBA::Object_ptr obj = CORBA::Object::_nil();
	GIOP::AddressingDisposition ad = GIOP::KeyAddr;
	if (!conn->codec()->get_locate_reply(in, req_id, stat, obj, ad)) {
	    return FALSE;
	}
	check(_proxy_request.find(req_id) != _proxy_request.end());
	if (_proxy_request[req_id].second != 0)
	    conn->codec()->version(_proxy_request[req_id].second);
	MICO::GIOPOutContext out(conn->codec());
	out.ec()->byteorder((flags & 1)
			    ? CORBA::LittleEndian : CORBA::BigEndian);
	conn->codec()->put_locate_reply(out, _proxy_request[req_id].first,
					stat, obj, ad);
	delete br->giop_data;
	br->giop_data = out._retn();
    } else if (mt == GIOP::Fragment) {
	CORBA::ULong end = in.dc()->buffer()->rpos();
	check(in.dc()->get_ulong(req_id));
	if (_proxy_request.find(req_id) == _proxy_request.end())
	    return TRUE;
	CORBA::ULong wend = br->giop_data->wpos();
	br->giop_data->wseek_beg(end - start);
	MICO::CDREncoder ec(br->giop_data, FALSE, in.dc()->byteorder(),
			    NULL, TRUE, NULL, TRUE);
	ec.put_ulong(_proxy_request[req_id].first);
	br->giop_data->wseek_beg(wend);
    }
    return TRUE;
}

#undef check

CORBA::IORProfile *
WATMBridge::GIOPBridge::key_to_profile (CORBA::OctetSeq *key)
{
    return NULL;
}

MICO::GIOPConn *
WATMBridge::GIOPBridge::make_conn (const CORBA::Address *addr)
{
    if (MICO::Logger::IsLogged(MICO::Logger::IIOP)) {
	MICO::Logger::Stream(MICO::Logger::IIOP)
	    << "Connecting to " << addr->stringify() << endl;
    }

    MapAddrConn::iterator i = _conn_cache.find(addr);
    if (i != _conn_cache.end())
	return i->second;

    CORBA::Transport *t = addr->make_transport();
    if (!t->connect(addr)) {
	delete t;
	return NULL;
    }
    MICO::GIOPConn *conn =
	new MICO::GIOPConn(_orb->dispatcher(), t, this,
			   new MICO::GIOPCodec(new MICO::CDRDecoder,
					       new MICO::CDREncoder,
					       _giop_ver),
			   0L, 0UL,
#ifdef HAVE_THREADS
			   FALSE, FALSE,
#endif
			   _id_gen.new_id());
    _conn_cache[t->peer()] = conn;
    return conn;
}

MICO::GIOPConn *
WATMBridge::GIOPBridge::make_conn (const WATMGlue::TargetAddress *target)
{
    CORBA::IORProfile *prof = NULL;
    CORBA::IOR *ior = NULL;
    const CORBA::Address *addr;
    MICO::GIOPConn *conn;

    if (target->kind == WATMGlue::TargetAddress::Key) {
	prof = key_to_profile(target->u.key);
    } else if (target->kind == WATMGlue::TargetAddress::Profile) {
	prof = target->u.profile;
    } else if (target->kind == WATMGlue::TargetAddress::Reference) {
	ior = target->u.reference.ior;
	prof = ior->get_profile(target->u.reference.index);
    } else
	return NULL;

    if (prof) {
	addr = prof->addr();
	assert(addr);
	return make_conn(addr);
    }
    if (ior == NULL)
	return NULL;

    CORBA::ORB_var orb = CORBA::ORB_instance("mico-local-orb");
    CORBA::DomainManager *dm;
    orb->get_default_domain_manager(dm);
    CORBA::Policy_var policy =
	dm->get_domain_policy(MICOPolicy::TRANSPORTPREF_POLICY_TYPE);
    MICOPolicy::TransportPrefPolicy_var tpp =
	MICOPolicy::TransportPrefPolicy::_narrow(policy);
    assert(!CORBA::is_nil(tpp));

    MICOPolicy::TransportPrefPolicy::ProfileTagSeq *prefs =
	tpp->preferences_nocopy();

    for (CORBA::ULong i = 0; i < prefs->length(); ++i) {
	prof = ior->profile((*prefs)[i]);
	while (prof) {
	    addr = prof->addr();
	    assert(addr);
	    conn = make_conn(addr);
	    if (conn)
		return conn;
	    prof = ior->profile((*prefs)[i], FALSE, prof);
	}
    }
    return NULL;
}

void
WATMBridge::GIOPBridge::kill_conn (MICO::GIOPConn *conn)
{
    CORBA::Boolean again;

    do {
	again = FALSE;
	for (MapCidConn::iterator i = _conns.begin(); i != _conns.end(); ++i) {
	    if (i->second == conn) {
		_conns.erase(i);
		again = TRUE;
		break;
	    }
	}
    } while (again);

    do {
	again = FALSE;
	for (MapAddrConn::iterator i = _conn_cache.begin();
	     i != _conn_cache.end(); ++i) {
	    if (i->second == conn) {
		_conn_cache.erase(i);
		again = TRUE;
		break;
	    }
	}
    } while (again);

    if (_conn_addrs.find(conn->id()) != _conn_addrs.end()) {
	MapReqTarget addrmap = _conn_addrs[conn->id()];
	for (MapReqTarget::iterator i = addrmap.begin();
	     i != addrmap.end(); ++i) {
	    delete i->second;
	}
	addrmap.erase(addrmap.begin(), addrmap.end());
	_conn_addrs.erase(conn->id());
    }

    MapCidIdMap::iterator ici = _request_proxy.find(conn->id());
    if (ici != _request_proxy.end()) {
	for (MapIdId::iterator ii = ici->second.begin();
	     ii != ici->second.end(); ++ii) {
	    _proxy_request.erase(ii->second);
	    _id_gen.release_id(ii->second);
	}
	_request_proxy.erase(ici);
    }

    _id_gen.release_id(conn->id());
    conn->deref(TRUE);
    delete conn;
}

void
WATMBridge::GIOPBridge::conn_error (MICO::GIOPConn *conn,
				    CORBA::Boolean send_error)
{
    if (!send_error) {
	kill_conn(conn);
	return;
    }

    MICO::GIOPOutContext out(conn->codec());
    conn->codec()->put_error_msg(out);
    conn->output(out._retn());
    conn->deref(TRUE);
    conn->flush();
}

CORBA::Boolean
WATMBridge::GIOPBridge::send_data (MICO::GIOPConn *conn,
				   WATMGlue::BridgeRequest *br)
{
    conn->output(br->giop_data);
    br->giop_data = NULL;
    return TRUE;
}

CORBA::Boolean
WATMBridge::GIOPBridge::handle_input (MICO::GIOPConn *conn,
				      CORBA::Buffer *buf)
{
    CORBA::Boolean from_cache = FALSE;
    MICO::GIOPInContext in(conn->codec(), buf);
    CORBA::ULong pos = in.dc()->buffer()->rpos();

    WATMGlue::BridgeRequest *br = get_target(in, conn);
    if (br == NULL)
	return FALSE;
    in.dc()->buffer()->rseek_beg(pos);
    if (local_target(br->target)) {
	CORBA::Boolean ret = local_input(conn, new CORBA::Buffer(*buf));
	return ret;
    }
    if (br->data_type == GIOP::Request
	|| br->data_type == GIOP::LocateRequest) {
	conn->ref();
    } else if (br->data_type == GIOP::Reply
	       || br->data_type == GIOP::LocateReply) {
	conn->deref();
    }
    if (br->target->kind == WATMGlue::TargetAddress::Reference
	&& (br->data_type == GIOP::Request
	    || br->data_type == GIOP::LocateRequest)
	&& whole_giop_message(buf)
	&& (_cached_iors[conn->id()].find(br->target->u.reference.ior)
	    != _cached_iors[conn->id()].end())
	&& _proxy->active(_cached_iors[conn->id()][br->target->u.reference.ior])) {
	from_cache = TRUE;
	br->conn_id = _cached_iors[conn->id()][br->target->u.reference.ior];
    } else if (!_proxy->callback(br, WATMGlue::BridgeRequestCallback::Open)) {
	if (br->code == WATMGlue::BridgeRequestCallback::FORWARD
	    && br->target->kind == WATMGlue::TargetAddress::Reference) {
	    if (br->data_type == GIOP::Request) {
		send_req_forw(conn, br->req_id, br->target->u.reference.ior);
		conn->deref();
		return TRUE;
	    } else if (br->data_type == GIOP::LocateRequest) {
		send_loc_forw(conn, br->req_id, br->target->u.reference.ior);
		conn->deref();
		return TRUE;
	    }
	    // We shouldn't get here so we let the connection be killed
	}
	if (br->data_type == GIOP::Request) {
	    send_req_nonexist(conn, br->req_id);
	} else if (br->data_type == GIOP::LocateRequest) {
	    send_loc_nonexist(conn, br->req_id);
	}
	if (conn->deref()) {
	    kill_conn(conn);
	}
	return FALSE;
    } else if (br->target->kind == WATMGlue::TargetAddress::Reference
	       && (br->data_type == GIOP::Request
		   || br->data_type == GIOP::LocateRequest)
	       && whole_giop_message(buf)) {
	// non-cached Open succeeded, cache reference
	_cached_iors[conn->id()][br->target->u.reference.ior] = br->conn_id;
    }

    _conns[br->conn_id] = conn;
    CORBA::Buffer *cache_buf = NULL;
    if (from_cache)
	cache_buf = new CORBA::Buffer(*buf);
    br->giop_data = new CORBA::Buffer(*buf);
    transform_request_id(conn, br);
    if (!_proxy->callback(br, WATMGlue::BridgeRequestCallback::Invoke)) {
	if (from_cache) {
	    // delete from cache and try again
	    _cached_iors[conn->id()].erase(br->target->u.reference.ior);
	    CORBA::Boolean ret = handle_input(conn, cache_buf);
	    return ret;
	}
	if (conn->deref()) {
	    kill_conn(conn);
	}
	return FALSE;
    }
    delete cache_buf;
    return TRUE;
}

CORBA::Boolean
WATMBridge::GIOPBridge::handle_input (MICO::GIOPConn *conn)
{
    return handle_input(conn, conn->input());
}

void
WATMBridge::GIOPBridge::register_local_object (CORBA::Object_ptr o)
{
    CORBA::Long len;
    const CORBA::Octet *pkey = o->_ior()->active_profile()->objectkey(len);
    CORBA::OctetSeq key(len);
    key.length(len);
    memcpy(key.get_buffer(), pkey, len);
    _local_keys.insert(key);
}

void
WATMBridge::GIOPBridge::callback (CORBA::TransportServer *tserv,
				  CORBA::TransportServerCallback::Event ev)
{
    switch (ev) {
    case CORBA::TransportServerCallback::Accept: {
	CORBA::Transport *t = tserv->accept();
	if (t) {
	    if (MICO::Logger::IsLogged(MICO::Logger::IIOP)) {
		MICO::Logger::Stream(MICO::Logger::IIOP)
		    << "GIOP connection from " << t->peer()->stringify()
		    << endl;
	    }
	    if (t->bad()) {
		delete t;
		break;
	    }
	    const CORBA::Address *addr = t->peer();
	    assert(addr);
	    MICO::GIOPConn *conn =
		new MICO::GIOPConn(_orb->dispatcher(), t, this,
				   new MICO::GIOPCodec(new MICO::CDRDecoder,
						       new MICO::CDREncoder,
						       _giop_ver),
				   0L, 0UL,
#ifdef HAVE_THREADS
				   FALSE, FALSE,
#endif
				   _id_gen.new_id());
	    _conn_cache[addr] = conn;
	}
	break;
    }
    default:
	assert(0);
    }
}

CORBA::Boolean
WATMBridge::GIOPBridge::callback (WATMGlue::BridgeRequest *br,
				  WATMGlue::BridgeRequestCallback::Event ev)
{
    switch (ev) {
    case WATMGlue::BridgeRequestCallback::Open: {
	if (br->target == NULL) {
	    br->code = WATMGlue::BridgeRequestCallback::COMM_FAILURE;
	    return FALSE;
	}
	MICO::GIOPConn *conn = make_conn(br->target);
	if (conn == NULL) {
	    br->code = WATMGlue::BridgeRequestCallback::COMM_FAILURE;
	    return FALSE;
	}
	conn->ref();
	_conns[br->conn_id] = conn;
	return TRUE;
    }
    case WATMGlue::BridgeRequestCallback::Close: {
	MapCidConn::iterator i = _conns.find(br->conn_id);
	if (i == _conns.end()) {
	    br->code = WATMGlue::BridgeRequestCallback::INVALID_ID;
	    return FALSE;
	}
	if (i->second->deref()) {
	    kill_conn(i->second);
	}
	return TRUE;
    }
    case WATMGlue::BridgeRequestCallback::Invoke: {
	MapCidConn::iterator i = _conns.find(br->conn_id);
	if (i == _conns.end()) {
	    br->code = WATMGlue::BridgeRequestCallback::INVALID_ID;
	    return FALSE;
	}
	revert_request_id(i->second, br);
	MICO::GIOPInContext in(i->second->codec(),
			       new CORBA::Buffer(*br->giop_data));
	/*
	 * get_target() inserts its return value into
	 * request_id-target map, so tem_br is not deallocated.
	 */
	WATMGlue::BridgeRequest *tem_br = get_target(in, i->second);
	if (tem_br == NULL) {
	    br->code = WATMGlue::BridgeRequestCallback::UNKNOWN;
	    return FALSE;
	}
	tem_br->conn_id = br->conn_id;
	return send_data(i->second, br);
    }
    case WATMGlue::BridgeRequestCallback::Error: {
	MapCidConn::iterator i = _conns.find(br->conn_id);
	if (i != _conns.end()) {
	    if (i->second->deref()) {
		kill_conn(i->second);
	    }
	}
    }
    default:
	return FALSE;
    }
}

CORBA::Boolean
WATMBridge::GIOPBridge::callback (MICO::GIOPConn *conn,
				  MICO::GIOPConnCallback::Event ev)
{
    switch (ev) {
    case MICO::GIOPConnCallback::InputReady:
	return handle_input(conn);
    case MICO::GIOPConnCallback::Idle:
    case MICO::GIOPConnCallback::Closed: {
	WATMGlue::BridgeRequest br;
	CORBA::Boolean ret = TRUE;
	for (MapReqTarget::iterator it = _conn_addrs[conn->id()].begin();
	     it != _conn_addrs[conn->id()].end(); ++it) {
	    ret = ret &&
		_proxy->callback(it->second,
				 WATMGlue::BridgeRequestCallback::Close);
	}
	kill_conn(conn);
	return ret;
    }
    default:
	assert(0);
    }
    return TRUE;
}

CORBA::Boolean
WATMBridge::GIOPBridge::input_callback (MICO::GIOPConn *conn,
					CORBA::Buffer *inp)
{
    return handle_input(conn, inp);
}

/************************* TerminalBridge *************************/

WATMBridge::TerminalBridge::TerminalBridge (CORBA::ORB_ptr orb,
					    vector<CORBA::Address *> addrs,
					    WATMGlue::BridgeRequestCallback *p)
    : GIOPBridge(orb, addrs, p)
{
}

WATMBridge::TerminalBridge::~TerminalBridge ()
{
    for (MapKeyProf::iterator i = _terminal_objs.begin();
	 i != _terminal_objs.end(); ++i) {
	delete i->first;
	delete i->second;
    }
}

CORBA::IORProfile *
WATMBridge::TerminalBridge::key_to_profile (CORBA::OctetSeq *key)
{
    return _terminal_objs[key];
}

void
WATMBridge::TerminalBridge::register_object (CORBA::OctetSeq *key,
					     CORBA::IORProfile *prof)
{
    _terminal_objs[key] = prof;
}

void
WATMBridge::TerminalBridge::transform_target (WATMGlue::BridgeRequest *br)
{
    MICO::GIOPCodec_var codec = new MICO::GIOPCodec(new MICO::CDRDecoder,
						    new MICO::CDREncoder,
						    _giop_ver);
    MICO::GIOPInContext in(codec, new CORBA::Buffer(*br->giop_data));
    GIOP::MsgType mt;
    CORBA::ULong sz;
    CORBA::Octet flags;

    if (!codec->get_header(in, mt, sz, flags))
	return;
    if (codec->version() < 0x0102
	|| (mt != GIOP::Request && mt != GIOP::LocateRequest))
	return;

    switch (mt) {
    case GIOP::Request: {
	CORBA::ULong req_id;
	CORBA::Octet resp;
	CORBA::ORBRequest *req;
	CORBA::Principal pr;
	CORBA::Object_ptr obj = new CORBA::Object(new CORBA::IOR);

	CORBA::Codeset::disable(TRUE);
	if (!codec->get_invoke_request(in, req_id, resp, obj, req, &pr)) {
	    CORBA::release(obj);
	    return;
	}
	if (obj->_ior_fwd()->addressing_disposition() != GIOP::KeyAddr) {
	    if (!obj->_ior_fwd()->active_profile())
		obj->_ior_fwd()->active_profile(obj->_ior_fwd()->profile());
	    obj->_ior_fwd()->addressing_disposition(GIOP::KeyAddr);
	    MICO::GIOPOutContext out(codec);
	    out.ec()->byteorder((flags & 1)
				? CORBA::LittleEndian : CORBA::BigEndian);
	    codec->put_invoke_request(out, req_id, resp, obj, req, &pr);
	    delete br->giop_data;
	    br->giop_data = out._retn();
	}
	CORBA::release(req);
	CORBA::release(obj);
	break;
    }
    case GIOP::LocateRequest: {
	CORBA::ULong req_id;
	CORBA::Object_ptr obj = new CORBA::Object(new CORBA::IOR);

	if (!codec->get_locate_request(in, req_id, obj)) {
	    CORBA::release(obj);
	    return;
	}
	if (obj->_ior_fwd()->addressing_disposition() != GIOP::KeyAddr) {
	    if (!obj->_ior_fwd()->active_profile())
		obj->_ior_fwd()->active_profile(obj->_ior_fwd()->profile());
	    obj->_ior_fwd()->addressing_disposition(GIOP::KeyAddr);
	    MICO::GIOPOutContext out(codec);
	    out.ec()->byteorder((flags & 1)
				? CORBA::LittleEndian : CORBA::BigEndian);
	    codec->put_locate_request(out, req_id, obj);
	    delete br->giop_data;
	    br->giop_data = out._retn();
	}
	CORBA::release(obj);
	break;
    }
    default:
	assert(0);
    }
    return;
}

CORBA::Boolean
WATMBridge::TerminalBridge::send_data (MICO::GIOPConn *conn,
				       WATMGlue::BridgeRequest *br)
{
    transform_target(br);
    return GIOPBridge::send_data(conn, br);
}

MICO::GIOPConn *
WATMBridge::TerminalBridge::make_conn (const WATMGlue::TargetAddress *target)
{
    CORBA::IORProfile *prof = NULL;
    CORBA::OctetSeq *key = NULL;

    if (target->kind == WATMGlue::TargetAddress::Key) {
	key = new CORBA::OctetSeq(*target->u.key);
    } else if (target->kind == WATMGlue::TargetAddress::Profile) {
	prof = target->u.profile;
    } else if (target->kind == WATMGlue::TargetAddress::Reference) {
	prof = target->u.reference.ior->get_profile(target->u.reference.index);
    } else
	return NULL;

    if (prof) {
	CORBA::Long len;
	const CORBA::Octet *keyseq = prof->objectkey(len);
	key = new CORBA::OctetSeq(len);
	key->length(len);
	memcpy(key->get_buffer(), keyseq, len);
    }
    if (key) {
	CORBA::IORProfile *tprof = key_to_profile(key);
	delete key;
	if (tprof != NULL) {
	    const CORBA::Address *addr = tprof->addr();
	    assert(addr);
	    return GIOPBridge::make_conn(addr);
	}
    }
    return NULL;
}

/************************** AccessBridge **************************/

WATMBridge::AccessBridge::AccessBridge (CORBA::ORB_ptr orb,
					vector<CORBA::Address *> addrs,
					WATMGlue::BridgeRequestCallback *proxy)
    : GIOPBridge(orb, addrs, proxy)
{
}

WATMBridge::AccessBridge::~AccessBridge ()
{
}
