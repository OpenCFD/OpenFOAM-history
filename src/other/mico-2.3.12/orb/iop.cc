/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2005 by The Mico Team
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
#define MICO_CONF_INTERCEPT
#define MICO_CONF_POA

#include <CORBA-SMALL.h>
#ifndef _WIN32
#include <string.h>
#endif
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif
#include <mico/impl.h>
#include <mico/template_impl.h>
#include <mico/util.h>

#endif // FAST_PCH

//#define USE_OLD_INTERCEPTORS

using namespace std;

//
// static member initialization
//
#ifdef HAVE_THREADS
bool
MICO::GIOPConn::S_reader_key_initialized_ = false;
// S_reader_key_ is initialized by a call to
// `GIOPConn::initialize_reader_key' method
MICOMT::Thread::ThreadKey
MICO::GIOPConn::S_reader_key_;
#endif // HAVE_THREADS

/****************************** misc dtors *******************************/


MICO::GIOPConnCallback::~GIOPConnCallback ()
{
}


/***************************** GIOPInContext *****************************/


MICO::GIOPInContext::GIOPInContext (GIOPCodec *codec, CORBA::Buffer *buf,
				    CORBA::CodeSetCoder *csc)
{
    _buf = buf;
    _delete_buf = TRUE;
    _dc = codec->dc_proto()->clone (buf, 0, csc ? csc : codec->converter(), 0);
    _delete_dc = TRUE;
}

MICO::GIOPInContext::~GIOPInContext ()
{
    if (_delete_buf)
	delete _buf;
    if (_delete_dc)
	delete _dc;
}

void
MICO::GIOPInContext::buffer (CORBA::Buffer *b)
{
    if (_delete_buf)
	delete _buf;

    _dc->buffer (b, FALSE);
    _buf = b;
    _delete_buf = FALSE;
}

void
MICO::GIOPInContext::converters (CORBA::CodeSetCoder *csc)
{
    _dc->converter (csc, FALSE);
}

CORBA::DataDecoder *
MICO::GIOPInContext::_retn ()
{
    assert (_delete_buf);
    assert (_delete_dc);
    _delete_buf = FALSE;
    //_delete_dc = FALSE;
    _delete_dc = TRUE;
    //return _dc;

    //FIXME: has to be rechecked fot MT
    return _dc->clone (_buf, TRUE, _dc->converter(), FALSE);
}


/***************************** GIOPOutContext *****************************/


MICO::GIOPOutContext::GIOPOutContext (GIOPCodec *codec,
				      CORBA::CodeSetCoder *csc)
{
    _buf = new CORBA::Buffer;
    _delete_buf = TRUE;
    _ec = codec->ec_proto()->clone (_buf, 0, csc ? csc : codec->converter(), 0);
    _delete_ec = TRUE;
}

MICO::GIOPOutContext::GIOPOutContext (CORBA::DataEncoder *ec)
{
    _buf = ec->buffer();
    _delete_buf = FALSE;
    _ec = ec;
    _delete_ec = FALSE;
}

MICO::GIOPOutContext::~GIOPOutContext ()
{
    if (_delete_buf)
	delete _buf;
    if (_delete_ec)
      delete _ec;
}

void
MICO::GIOPOutContext::converters (CORBA::CodeSetCoder *csc)
{
    _ec->converter (csc, FALSE);
}

void
MICO::GIOPOutContext::reset()
{
    _buf->reset();
}

CORBA::Buffer *
MICO::GIOPOutContext::_retn()
{
    assert (_delete_buf);
    _delete_buf = FALSE;
    return _buf;
}


/******************************* GIOPCodec *******************************/


MICO::GIOPCodec::GIOPCodec (CORBA::DataDecoder *_dc,
                            CORBA::DataEncoder *_ec,
                            CORBA::UShort giop_ver)
    : _dc_proto(_dc), _ec_proto(_ec)
{
    _giop_ver = giop_ver;
    _conv = 0;
    _codesets_sent = FALSE;

    if (_giop_ver > 0x0102) {
      _giop_ver = 0x0102;
    }

    GIOPOutContext ctx (this);
    _size_offset = put_header (ctx, GIOP::Request);
    _headerlen = ctx.ec()->buffer()->length();

     if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	 MICOMT::AutoDebugLock __lock;
	 MICO::Logger::Stream (MICO::Logger::GIOP)
	     << "GIOPCodec::GIOPCodec(): " << this << endl;
     }
}

MICO::GIOPCodec::~GIOPCodec ()
{
    delete _ec_proto;
    delete _dc_proto;
    delete _conv;

    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::GIOP)
	    << "GIOPCodec::~GIOPCodec: " << this << endl;
    }
}

CORBA::Boolean
MICO::GIOPCodec::setup_codeset_ids (CORBA::Object_ptr obj)
{
  CORBA::IORProfile *prof;
  CORBA::MultiComponent *comps;
  CORBA::Component *comp;
  MICO::CodesetComponent *csc = 0;

  assert (!_conv);

  /*
   * code set support disabled?
   */

  if (CORBA::Codeset::disabled()) {
    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
      MICO::Logger::Stream (MICO::Logger::GIOP)
	<< "GIOP: not setting up context because of -ORBNoCodesets"
	<< endl;
    }
    return TRUE;
  }

  /*
   * No code set support in GIOP 1.0
   */

  if (_giop_ver == 0x0100) {
    _csid = 0;
    _wcsid = 0;
    _conv = new MICO::GIOP_1_0_CodeSetCoder ();
    return TRUE;
  }

  /*
   * Try to find Code Sets component in active profile first
   */

  prof = obj->_ior_fwd()->active_profile();

  if (prof && (comps = prof->components())) {
    if ((comp = comps->component (CORBA::Component::TAG_CODE_SETS))) {
      csc = (MICO::CodesetComponent *) comp;
    }
  }

  /*
   * Next, try to find it in a global MultipleComponents profile
   */

  prof = obj->_ior_fwd()->profile (CORBA::IORProfile::TAG_MULTIPLE_COMPONENTS,
				   TRUE);

  if (prof && (comps = prof->components())) {
    if ((comp = comps->component (CORBA::Component::TAG_CODE_SETS))) {
      csc = (MICO::CodesetComponent *) comp;
    }
  }

  if (csc) {
    _csid = csc->selected_cs();
    _wcsid = csc->selected_wcs();
  } else {
    // no code set info, use defaults ...
    _csid = CORBA::Codeset::special_cs(CORBA::Codeset::DefaultCS)->id();
    _wcsid = CORBA::Codeset::special_cs(CORBA::Codeset::DefaultWCS)->id();
  }

  if (_giop_ver == 0x0100) {
    _conv = new MICO::GIOP_1_0_CodeSetCoder ();
  }
  else if (_giop_ver == 0x0101) {
    _conv = new MICO::GIOP_1_1_CodeSetCoder (_csid);
  }
  else if (_giop_ver >= 0x0102) {
    _conv = new MICO::GIOP_1_2_CodeSetCoder (_csid, _wcsid);
  }

  _dc_proto->converter (_conv, FALSE);
  _ec_proto->converter (_conv, FALSE);

  if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
    MICOMT::AutoDebugLock __lock;
    if (csc) {
      MICO::Logger::Stream (MICO::Logger::GIOP)
	<< "GIOP: advertised code sets: " << endl;
      csc->print (MICO::Logger::Stream (MICO::Logger::GIOP));
    }
    else {
      MICO::Logger::Stream (MICO::Logger::GIOP)
	<< "GIOP: no codesets tag found, using defaults." << endl;
    }

    if (_giop_ver == 0x0101) {
      MICO::Logger::Stream (MICO::Logger::GIOP)
	<< "GIOP:    TCS-C is "
	<< CORBA::Codeset::_find_info(_csid)->desc
	<< endl;
    }
    else if (_giop_ver == 0x0102) {
      MICO::Logger::Stream (MICO::Logger::GIOP)
	<< "GIOP:    TCS-C is "
	<< CORBA::Codeset::_find_info(_csid)->desc
	<< endl
	<< "GIOP:    TCS-W is "
	<< CORBA::Codeset::_find_info(_wcsid)->desc
	<< endl;
    }
  }

  return _conv->isok();
}

CORBA::ULong
MICO::GIOPCodec::put_header (GIOPOutContext &out, GIOP::MsgType mt)
{
    CORBA::ULong key;

    CORBA::DataEncoder *ec = out.ec();

    ec->struct_begin();
    {
	ec->arr_begin();
	{
	    // these are really chars, but must be encoded in latin1 ...
	    ec->put_chars_raw ((CORBA::Char *)"GIOP", 4);
	}
	ec->arr_end();

	ec->struct_begin();
	{
	    ec->put_octet ((CORBA::Octet)(_giop_ver >> 8));
	    ec->put_octet ((CORBA::Octet)_giop_ver);
	}
	ec->struct_end();

        if (_giop_ver == 0x0100) {
            ec->put_boolean (ec->byteorder() == CORBA::LittleEndian);
        } else {
            ec->put_octet ((ec->byteorder() == CORBA::LittleEndian)
                ? GIOP_BYTEORDER_BIT : 0);
        }
	ec->put_octet (mt);
	key = ec->buffer()->wpos();
	ec->put_ulong (0);
    }
    ec->struct_end();

    return key;
}

void
MICO::GIOPCodec::put_contextlist (GIOPOutContext &out,
				  const IOP::ServiceContextList &ctx,
				  CORBA::Boolean codesets)
{
    CORBA::DataEncoder *ec = out.ec();

    if (CORBA::Codeset::disabled())
	codesets = FALSE;

    ec->seq_begin (ctx.length() + !!codesets);
    {
	CORBA::DataEncoder::EncapsState state;
	for (CORBA::ULong i = 0; i < ctx.length(); ++i) {
	    ec->struct_begin();
	    {
		ec->put_ulong (ctx[i].context_id);
                // no encapsulation done here, the user has to supply
                // an encapsulated sequence in the interceptor ...
                ec->seq_begin (ctx[i].context_data.length());
		if (ctx[i].context_data.length() > 0) {
		    ec->put_octets (
			(const CORBA::Octet *)&ctx[i].context_data[0],
			ctx[i].context_data.length());
		}
                ec->seq_end();
	    }
	    ec->struct_end();
	}

	if (codesets) {
	    ec->struct_begin();
	    {
		ec->put_ulong (IOP::CodeSets);
		ec->encaps_begin (state);
		ec->struct_begin ();
		{
		    ec->put_ulong (_csid);
		    ec->put_ulong (_wcsid);
		}
		ec->struct_end ();
		ec->encaps_end (state);
	    }
	    ec->struct_end();
	}
    }
    ec->seq_end ();
}

void
MICO::GIOPCodec::put_target (GIOPOutContext &out, CORBA::Object_ptr obj)
{
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong active_index;
    CORBA::IORProfile *prof = obj->_ior_fwd()->active_profile (&active_index);
    assert (prof);

    CORBA::Long objkeylen;
    const CORBA::Octet *objkey = prof->objectkey (objkeylen);

    if (_giop_ver < 0x0102) {
	ec->seq_begin (objkeylen);
	ec->put_octets (objkey, objkeylen);
	ec->seq_end ();
    } else {
	ec->union_begin();
	{
	  GIOP::AddressingDisposition kind =
	    obj->_ior_fwd()->addressing_disposition();

	  if (kind == GIOP::KeyAddr) {
	    ec->put_short (kind);
	    ec->seq_begin (objkeylen);
	    ec->put_octets (objkey, objkeylen);
	    ec->seq_end ();
	  }
	  else if (kind == GIOP::ProfileAddr) {
	    ec->put_short (kind);
	    prof->encode (*ec);
	  }
	  else if (kind == GIOP::ReferenceAddr) {
	    ec->put_short (kind);
	    ec->put_ulong (active_index);
	    obj->_ior_fwd()->encode (*ec);
	  }
	  else {
	    assert (0);
	  }
	}
	ec->union_end();
    }
}

CORBA::Boolean
MICO::GIOPCodec::put_args (GIOPOutContext &out, CORBA::ORBRequest *req,
			   CORBA::Boolean inp)
{
    CORBA::DataEncoder *ec = out.ec();

    ec->struct_begin ();
    {
	if (inp) {
	    if (!req->get_in_args (ec))
		return FALSE;
	} else {
	    CORBA::Boolean is_except;
	    if (!req->get_out_args (ec, is_except))
		return FALSE;
	}
    }
    ec->struct_end ();
    return TRUE;
}

void
MICO::GIOPCodec::put_size (GIOPOutContext &out, CORBA::ULong key)
{
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong end_pos = ec->buffer()->wpos();
    ec->buffer()->wseek_beg (key);
    ec->put_ulong (end_pos - ec->buffer()->rpos() - _headerlen);
    ec->buffer()->wseek_beg (end_pos);
}

CORBA::Boolean
MICO::GIOPCodec::put_invoke_request (GIOPOutContext &out,
				     CORBA::ULong req_id,
				     CORBA::Octet response_flags,
				     CORBA::Object_ptr obj,
				     CORBA::ORBRequest *req,
				     CORBA::Principal_ptr pr)
{
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ByteOrder bo = ec->byteorder();
    if (!strcmp (req->type(), "giop"))
        ec->byteorder (((GIOPRequest *)req)->input_byteorder());

    CORBA::ULong key = put_header (out, GIOP::Request);

    CORBA::Boolean send_codesets;
    if (!_codesets_sent && _conv && _giop_ver >= 0x0101) {
      send_codesets = TRUE;
      _codesets_sent = TRUE;
    }
    else {
      send_codesets = FALSE;
    }

    ec->struct_begin ();
    {
	if (_giop_ver < 0x0102) {
	    put_contextlist (out, *req->context(), send_codesets);
	    ec->put_ulong (req_id);
	    ec->put_boolean (response_flags & 0x01);
	    if (_giop_ver != 0x0100)
		ec->put_octets ((CORBA::Octet*)"\0\0\0", 3);
	    put_target (out, obj);
	    ec->put_string_raw (req->op_name());
            if (pr) {
                ec->put_principal (*pr);
            } else {
                ec->put_principal (CORBA::Principal());
            }
	} else {
	    ec->put_ulong (req_id);
	    ec->put_octet ((response_flags & 0x1) ? 0x03 : 0);
	    ec->put_octets ((CORBA::Octet*)"\0\0\0", 3);
	    put_target (out, obj);
	    ec->put_string_raw (req->op_name());
	    put_contextlist (out, *req->context(), send_codesets);
	}
    }
    ec->struct_end ();

    if (_giop_ver >= 0x0102)
	ec->buffer()->walign (ec->max_alignment());

    if (!put_args (out, req, TRUE)) {
	ec->byteorder (bo);
	return FALSE;
    }

#ifdef USE_OLD_INTERCEPTORS
#warning "using old interceptors!"
    if (!Interceptor::ClientInterceptor::_idle()) {
	CORBA::Environment env;
	ec->buffer()->rseek_rel (_headerlen);
	if (!Interceptor::ClientInterceptor::_exec_output_message (
	    ec->buffer(), &env)) {
	    ec->byteorder (bo);
	    return FALSE;
	}
	ec->buffer()->rseek_rel (-_headerlen);
    }
#endif // USE_OLD_INTERCEPTORS

    put_size (out, key);

    ec->byteorder (bo);
    return TRUE;
}

CORBA::Boolean
MICO::GIOPCodec::put_invoke_reply_offset (GIOPOutContext &out,
					  CORBA::ORBRequest *req)
{
    CORBA::DataEncoder *ec = out.ec();

    ec->buffer()->wseek_rel (_headerlen);

    ec->struct_begin ();
    {
	put_contextlist (out, *req->context());
	ec->put_ulong (0);
	ec->enumeration (0);
    }
    ec->struct_end ();

    return TRUE;
}

CORBA::Boolean
MICO::GIOPCodec::put_invoke_reply (GIOPOutContext &out,
				   CORBA::ULong req_id,
				   GIOP::ReplyStatusType stat,
				   CORBA::Object_ptr obj,
				   CORBA::ORBRequest *req,
				   GIOP::AddressingDisposition ad)
{
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ByteOrder bo = ec->byteorder();
    if (!strcmp (req->type(), "giop"))
        ec->byteorder (((GIOPRequest *)req)->output_byteorder());

    CORBA::ULong key = put_header (out, GIOP::Reply);

    ec->struct_begin ();
    {
	if (_giop_ver < 0x0102) {
	    put_contextlist (out, *req->context());
	    ec->put_ulong (req_id);
	    ec->enumeration ((CORBA::ULong)stat);
	} else {
	    ec->put_ulong (req_id);
	    ec->enumeration ((CORBA::ULong)stat);
	    put_contextlist (out, *req->context());
	}
    }
    ec->struct_end ();

    switch (stat) {
    case GIOP::NO_EXCEPTION:
    case GIOP::USER_EXCEPTION:
    case GIOP::SYSTEM_EXCEPTION:
	if (_giop_ver >= 0x0102) {
	    ec->buffer()->walign (ec->max_alignment());
	}
	if (!put_args (out, req, FALSE)) {
	    ec->byteorder (bo);
	    return FALSE;
	}
	break;

    case GIOP::LOCATION_FORWARD:
    case GIOP::LOCATION_FORWARD_PERM:
	ec->put_ior (*obj->_ior());
	break;

    case GIOP::NEEDS_ADDRESSING_MODE:
      ec->put_short (ad);
      break;

    default:
	assert (0);
	break;
    }

#ifdef USE_OLD_INTERCEPTORS
    if (!Interceptor::ServerInterceptor::_idle()) {
	CORBA::Environment env;
	ec->buffer()->rseek_rel (_headerlen);
	if (!Interceptor::ServerInterceptor::_exec_output_message (
	    ec->buffer(), &env)) {
	    ec->byteorder (bo);
	    return FALSE;
	}
	ec->buffer()->rseek_rel (-_headerlen);
    }
#endif // USE_OLD_INTERCEPTORS

    put_size (out, key);

    ec->byteorder (bo);
    return TRUE;
}

CORBA::Boolean
MICO::GIOPCodec::put_cancel_request (GIOPOutContext &out, CORBA::ULong req_id)
{
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong key = put_header (out, GIOP::CancelRequest);

    ec->struct_begin();
    {
	ec->put_ulong (req_id);
    }
    ec->struct_end();

    put_size (out, key);
    return TRUE;
}

CORBA::Boolean
MICO::GIOPCodec::put_locate_request (GIOPOutContext &out,
				     CORBA::ULong req_id,
				     CORBA::Object_ptr obj)
{
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong key = put_header (out, GIOP::LocateRequest);

    ec->struct_begin();
    {
	ec->put_ulong (req_id);
	put_target (out, obj);
    }
    ec->struct_end();

    put_size (out, key);
    return TRUE;
}

CORBA::Boolean
MICO::GIOPCodec::put_locate_reply (GIOPOutContext &out,
				   CORBA::ULong req_id,
				   GIOP::LocateStatusType stat,
				   CORBA::Object_ptr obj,
				   GIOP::AddressingDisposition ad)
{
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong key = put_header (out, GIOP::LocateReply);

    ec->struct_begin();
    {
	ec->put_ulong (req_id);
	ec->enumeration ((CORBA::ULong)stat);
    }
    ec->struct_end();

    switch (stat) {
    case GIOP::OBJECT_FORWARD:
    case GIOP::OBJECT_FORWARD_PERM:
	ec->put_ior (*obj->_ior());
	break;
    case GIOP::LOC_NEEDS_ADDRESSING_MODE:
        ec->put_short (ad);
        break;
    default:
	break;
    }
    put_size (out, key);
    return TRUE;
}

CORBA::Boolean
MICO::GIOPCodec::put_bind_request (GIOPOutContext &out, CORBA::ULong req_id,
				   const char *repoid,
				   const CORBA::ORB::ObjectTag &oid)
{
    /*
     * mapped to an invocation:
     *   GIOP::LocateStatusType _bind (in string repoid,
     *				       in sequence<octet> oid,
     *				       out object obj);
     */
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong key = put_header (out, GIOP::Request);

    ec->struct_begin ();
    {
	if (_giop_ver < 0x0102) {
	    put_contextlist (out, IOP::ServiceContextList());
	    ec->put_ulong (req_id);
	    ec->put_boolean (TRUE);
	    if (_giop_ver != 0x0100) {
		ec->put_octets ((CORBA::Octet*)"\0\0\0", 3);
	    }
	    ec->put_ulong (0); // object key
	    ec->put_string ("_bind");
	    ec->put_principal (CORBA::Principal());
	} else {
	    ec->put_ulong (req_id);
	    ec->put_octet (0x03);
	    ec->put_octets ((CORBA::Octet*)"\0\0\0", 3);
	    ec->put_short (GIOP::KeyAddr);
	    ec->put_ulong (0); // object key
	    ec->put_string ("_bind");
	    put_contextlist (out, IOP::ServiceContextList());
	}
    }
    ec->struct_end ();

    if (_giop_ver >= 0x0102)
	ec->buffer()->walign (ec->max_alignment());

    ec->struct_begin ();
    {
	ec->put_string (repoid);
	ec->seq_begin (oid.length());
	if (oid.length() > 0)
	    ec->put_octets ((const CORBA::Octet *)&oid[0], oid.length());
	ec->seq_end ();
    }
    ec->struct_end ();

#ifdef USE_OLD_INTERCEPTORS
    if (!Interceptor::ClientInterceptor::_idle()) {
	CORBA::Environment env;
	ec->buffer()->rseek_rel (_headerlen);
	if (!Interceptor::ClientInterceptor::_exec_output_message (
	    ec->buffer(), &env)) {
	    return FALSE;
	}
	ec->buffer()->rseek_rel (-_headerlen);
    }
#endif // USE_OLD_INTERCEPTORS

    put_size (out, key);
    return TRUE;
}

CORBA::Boolean
MICO::GIOPCodec::put_bind_reply (GIOPOutContext &out,
				 CORBA::ULong req_id,
				 GIOP::LocateStatusType stat,
				 CORBA::Object_ptr obj)
{
    /*
     * mapped to an invocation:
     *   GIOP::LocateStatusType _bind (in string repoid,
     *				       in sequence<octet> oid,
     *				       out object obj);
     */
    CORBA::DataEncoder *ec = out.ec();

    CORBA::ULong key = put_header (out, GIOP::Reply);

    ec->struct_begin ();
    {
	if (_giop_ver < 0x0102) {
	    put_contextlist (out, IOP::ServiceContextList());
	    ec->put_ulong (req_id);
	    ec->enumeration ((CORBA::ULong)GIOP::NO_EXCEPTION);
	} else {
	    ec->put_ulong (req_id);
	    ec->enumeration ((CORBA::ULong)GIOP::NO_EXCEPTION);
	    put_contextlist (out, IOP::ServiceContextList());
	}
    }
    ec->struct_end ();

    if (_giop_ver >= 0x0102)
	ec->buffer()->walign (ec->max_alignment());

    ec->struct_begin ();
    {
	ec->enumeration ((CORBA::ULong)stat);
        if (stat == GIOP::OBJECT_HERE) {
            ec->put_ior (*obj->_ior());
        } else {
            ec->put_ior (CORBA::IOR());
        }
    }
    ec->struct_end ();

#ifdef USE_OLD_INTERCEPTORS
    if (!Interceptor::ServerInterceptor::_idle()) {
	CORBA::Environment env;
	ec->buffer()->rseek_rel (_headerlen);
	if (!Interceptor::ServerInterceptor::_exec_output_message (
	    ec->buffer(), &env)) {
	    return FALSE;
	}
	ec->buffer()->rseek_rel (-_headerlen);
    }
#endif // USE_OLD_INTERCEPTORS

    put_size (out, key);
    return TRUE;
}

CORBA::Boolean
MICO::GIOPCodec::put_close_msg (GIOPOutContext &out)
{
    CORBA::ULong key = put_header (out, GIOP::CloseConnection);
    put_size (out, key);
    return TRUE;
}

CORBA::Boolean
MICO::GIOPCodec::put_error_msg (GIOPOutContext &out)
{
    CORBA::ULong key = put_header (out, GIOP::MessageError);
    put_size (out, key);
    return TRUE;
}

#define check(exp) if (!(exp)) return FALSE

CORBA::Boolean
MICO::GIOPCodec::get_header (GIOPInContext &in, GIOP::MsgType &mt,
			     CORBA::ULong &sz, CORBA::Octet &flags)
{
    CORBA::DataDecoder *dc = in.dc();

    CORBA::Boolean b;
    CORBA::Octet o;

    check (dc->struct_begin());
    {
	check (dc->arr_begin());
	{
	    CORBA::Char magic[5];
	    magic[4] = 0;

	    // these are really chars, but must be encoded in latin1 ...
	    check (dc->get_chars_raw (magic, 4) &&
		   !strcmp ((char*)magic, "GIOP"));
	}
	check (dc->arr_end());

	check (dc->struct_begin());
	{
	    CORBA::Octet min, maj;
	    CORBA::UShort ver;

	    check (dc->get_octet (maj));
	    check (dc->get_octet (min));

	    ver = ((maj << 8) | min);

	    /*
	     * We don't speak higher GIOP versions than _giop_ver. So we
	     * support downgrading, but not upgrading.
	     */

	    if (ver < _giop_ver && maj == 1) {
	      if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
		MICOMT::AutoDebugLock __lock;
		MICO::Logger::Stream (MICO::Logger::GIOP)
		  << "GIOP: peer requests GIOP version "
		  << (int) maj << "." << (int) min
		  << " instead of "
		  << (_giop_ver>>8) << "." << (_giop_ver&255)
		  << ", downgrading." << endl;
	      }
	      _giop_ver = ver;
	    }
	    else if (ver > _giop_ver) {
	      if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
		MICOMT::AutoDebugLock __lock;
		MICO::Logger::Stream (MICO::Logger::GIOP)
		  << "GIOP: peer sends message using unsupported GIOP version "
		  << (int) maj << "." << (int) min
		  << endl;
	      }
	      return 0;
	    }
	}
	check (dc->struct_end());

        if (_giop_ver == 0x0100) {
            check (dc->get_boolean (b));
            dc->byteorder (b ? CORBA::LittleEndian
                           : CORBA::BigEndian);
            flags = b ? 1 : 0;
        } else {
            check (dc->get_octet (flags));
            dc->byteorder ((flags & GIOP_BYTEORDER_BIT) ? CORBA::LittleEndian
                           : CORBA::BigEndian);
        }
	check (dc->get_octet (o));
	mt = (GIOP::MsgType)o;
	check (dc->get_ulong (sz));
    }
    check (dc->struct_end());
    return TRUE;
}

CORBA::Boolean
MICO::GIOPCodec::get_contextlist (GIOPInContext &in,
				  IOP::ServiceContextList &ctx,
				  CORBA::Boolean codesets)
{
    CORBA::DataDecoder *dc = in.dc();

    CORBA::Codeset::CodesetId csid = 0, wcsid = 0;
    CORBA::Boolean got_codeset_context = 0;

    CORBA::ULong len;
    CORBA::DataDecoder::EncapsState state;
    check (dc->seq_begin (len));
    ctx.length (len);
    {
	CORBA::ULong context_id;
	for (CORBA::ULong i = 0; i < ctx.length(); ) {
	    check (dc->struct_begin());
	    {
		check (dc->get_ulong (context_id));
		/*
		 * Modified for MIWCO to respect codeset disabling
		 * even here (see bridge.cc:transform_target() for
		 * use).
		 */
		if (context_id == IOP::CodeSets && !CORBA::Codeset::disabled()) {
                    check (dc->encaps_begin (state, len));
		    // code set service ...
		    ctx.length (ctx.length()-1);
		    check (dc->struct_begin());
		    {
			check (dc->get_ulong (csid));
			check (dc->get_ulong (wcsid));
		    }
		    check (dc->struct_end());
                    check (dc->encaps_end (state));

		    got_codeset_context = 1;

		    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
		      MICOMT::AutoDebugLock __lock;
		      MICO::Logger::Stream (MICO::Logger::GIOP)
			<< "GIOP: incoming CodeSets context" << endl;

		      if (CORBA::Codeset::_find_info(csid)) {
			MICO::Logger::Stream (MICO::Logger::GIOP)
			  << "GIOP:   requested TCS-C is "
			  << CORBA::Codeset::_find_info(csid)->desc << endl;
		      }
		      else {
			MICO::Logger::Stream (MICO::Logger::GIOP)
			  << "GIOP:   unknown requested TCS-C: "
			  << csid << endl;
		      }

		      if (CORBA::Codeset::_find_info(wcsid)) {
			MICO::Logger::Stream (MICO::Logger::GIOP)
			  << "GIOP:   requested TCS-W is "
			  << CORBA::Codeset::_find_info(wcsid)->desc << endl;
		      }
		      else {
			MICO::Logger::Stream (MICO::Logger::GIOP)
			  << "GIOP:   unknown requested TCS-W: "
			  << wcsid << endl;
		      }
		    }
		} else {
		    // other service ...
                    // no de-encapsulation done here, the user has to
                    // do this in the interceptor ...
                    check (dc->seq_begin (len));
		    ctx[i].context_id = context_id;
		    ctx[i].context_data.length (len);
		    if (len > 0) {
			check (dc->get_octets (
			    &ctx[i].context_data[0],
			    ctx[i].context_data.length()));
		    }
                    check (dc->seq_end ());
		    ++i;
		}
	    }
	    check (dc->struct_end());
	}
    }
    check (dc->seq_end ());

    // set up code sets ...
    if (codesets) {
      if (got_codeset_context) {
	/*
	 * We got a code set context
	 */
	if (_conv != 0) {
	  if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::GIOP)
	      << "GIOP: codesets already set up, ignoring codeset context"
	      << endl;
	  }
	}
	else if (CORBA::Codeset::disabled()) {
	  if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::GIOP)
	      << "GIOP: ignoring codeset context because of -ORBNoCodesets"
	      << endl;
	  }
	}
	else if (_giop_ver == 0x0100) {
	  if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::GIOP)
	      << "GIOP: ignoring codeset context on GIOP 1.0 connection,"
	      << " assuming ISO 8859-1" << endl;
	    _conv = new MICO::GIOP_1_0_CodeSetCoder ();
	    in.converters (_conv);
	    _dc_proto->converter (_conv, FALSE);
	    _ec_proto->converter (_conv, FALSE);
	  }
	}
	else {
	  // __giop_ver == 0x0100 already handled above
	  if (_giop_ver == 0x0101) {
	    _conv = new MICO::GIOP_1_1_CodeSetCoder (csid);
	  }
	  else if (_giop_ver >= 0x0102) {
	    _conv = new MICO::GIOP_1_2_CodeSetCoder (csid, wcsid);
	  }

	  if (!_conv->isok()) {
	    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	      MICOMT::AutoDebugLock __lock;
	      MICO::Logger::Stream (MICO::Logger::GIOP)
		<< "GIOP: cannot setup codeset converter" << endl;
	    }

	    return FALSE;
	  }

	  _csid = csid;
	  _wcsid = wcsid;

	  in.converters (_conv);
	  _dc_proto->converter (_conv, FALSE);
	  _ec_proto->converter (_conv, FALSE);
	}
      }
      else {
	/*
	 * We should setup codesets, but didn't receive a code set context
	 * This is OK if codesets are already setup or if this is a GIOP
	 * 1.0 conn
	 */
	if (!_conv) {
	  if (_giop_ver == 0x0100) {
	    if (CORBA::Codeset::disabled()) {
	      if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
		MICOMT::AutoDebugLock __lock; 
		MICO::Logger::Stream (MICO::Logger::GIOP)
		  << "GIOP: not setting up codeset converter because of -ORBNoCodesets"
		  << endl;
	      }
	    }
	    else {
	      _conv = new MICO::GIOP_1_0_CodeSetCoder ();
	      in.converters (_conv);
	      _dc_proto->converter (_conv, FALSE);
	      _ec_proto->converter (_conv, FALSE);
	    }
	  }
	  else {
	    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	      MICOMT::AutoDebugLock __lock;
	      MICO::Logger::Stream (MICO::Logger::GIOP)
		<< "GIOP: expected codeset context, but didn't get any"
		<< endl;
	    }
	    if (CORBA::Codeset::disabled()) {
	      if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
		MICOMT::AutoDebugLock __lock;
		MICO::Logger::Stream (MICO::Logger::GIOP)
		  << "GIOP: ignoring this fact because of -ORBNoCodesets"
		  << endl;
	      }
	    }
	    else {
	      _conv = new MICO::GIOP_1_0_CodeSetCoder ();
	      in.converters (_conv);
	      _dc_proto->converter (_conv, FALSE);
	      _ec_proto->converter (_conv, FALSE);
	    }
	  }
	}
      }
    }
    return TRUE;
}

CORBA::Boolean
MICO::GIOPCodec::get_target (GIOPInContext &in, CORBA::Object_ptr obj)
{
    CORBA::DataDecoder *dc = in.dc();

    if (_giop_ver < 0x0102) {
	CORBA::ULong objkeylen;
	CORBA::Octet *objkey;

	obj->_ior()->add_profile (new GIOPSimpleProf);

	check (dc->seq_begin (objkeylen));
        check (dc->buffer()->length() >= objkeylen);
	objkey = dc->buffer()->data();
	dc->buffer()->rseek_rel (objkeylen);
	check (dc->seq_end ());

	obj->_ior()->objectkey (objkey, objkeylen);
	obj->_ior()->addressing_disposition (GIOP::KeyAddr);
    } else {
	check (dc->union_begin());
	{
	    GIOP::AddressingDisposition kind;
	    check (dc->get_short (kind));
	    obj->_ior()->addressing_disposition (kind);
	    if (kind == GIOP::KeyAddr) {
		obj->_ior()->add_profile (new GIOPSimpleProf);

		CORBA::ULong objkeylen;
		CORBA::Octet *objkey;

		check (dc->seq_begin (objkeylen));
                check (dc->buffer()->length() >= objkeylen);
		objkey = dc->buffer()->data();
		dc->buffer()->rseek_rel (objkeylen);
		check (dc->seq_end ());

		obj->_ior()->objectkey (objkey, objkeylen);
	    } else if (kind == GIOP::ProfileAddr) {
		CORBA::IORProfile *prof = CORBA::IORProfile::decode (*dc);
		check (prof);
		obj->_ior()->add_profile (prof);
	    } else if (kind == GIOP::ReferenceAddr) {
		check (dc->struct_begin());
		{
		    CORBA::ULong index;
		    check (dc->get_ulong (index));
		    check (dc->get_ior (*obj->_ior()));
		}
		check (dc->struct_end());
	    } else {
		return FALSE;
	    }
	}
	check (dc->union_end());
    }
    return TRUE;
}

CORBA::Boolean
MICO::GIOPCodec::check_header (GIOPInContext &in, GIOP::MsgType &mt,
			       CORBA::ULong &sz, CORBA::Octet &flags)
{
    CORBA::ULong pos = in.dc()->buffer()->rpos ();
    CORBA::Boolean ret = get_header (in, mt, sz, flags);
    in.dc()->buffer()->rseek_beg (pos);
    return ret;
}

CORBA::Boolean
MICO::GIOPCodec::get_invoke_request (GIOPInContext &in,
				     CORBA::ULong &req_id,
				     CORBA::Octet &response_flags,
				     CORBA::Object_ptr obj,
				     CORBA::ORBRequest * &req,
				     CORBA::Principal_ptr pr)
{
    CORBA::DataDecoder *dc = in.dc();

#ifdef USE_OLD_INTERCEPTORS
    if (!Interceptor::ServerInterceptor::_idle()) {
	CORBA::Environment env;
	if (!Interceptor::ServerInterceptor::_exec_input_message (
	    dc->buffer(), &env)) {
	    return FALSE;
	}
    }
#endif // USE_OLD_INTERCEPTORS

    CORBA::String_var opname;
    IOP::ServiceContextList ctx;

    check (dc->struct_begin ());
    {
	if (_giop_ver < 0x0102) {
	    check (get_contextlist (in, ctx, TRUE));
	    check (dc->get_ulong (req_id));
	    check (dc->get_boolean (response_flags));
	    if (_giop_ver != 0x0100) {
		CORBA::Octet o[3];
		check (dc->get_octets (o, 3));
	    }
	    check (get_target (in, obj));
	    check (dc->get_string_raw (opname.out()));
	    check (dc->get_principal (*pr));
	} else {
	    check (dc->get_ulong (req_id));
	    check (dc->get_octet (response_flags));
            // XXX handle 0x01 properly ...
            response_flags = (response_flags & 0x02) ? 1 : 0;
	    CORBA::Octet o[3];
	    check (dc->get_octets (o, 3));
	    check (get_target (in, obj));
	    check (dc->get_string_raw (opname.out()));
	    check (get_contextlist (in, ctx, TRUE));
	}
    }
    check (dc->struct_end ());

    if (_giop_ver >= 0x0102)
	// if there are no arguments aligning might fail ...
	dc->buffer()->ralign (dc->max_alignment());

    req = new GIOPRequest (opname, in._retn(), this);
    req->context (&ctx);

    return TRUE;
}

CORBA::Boolean
MICO::GIOPCodec::get_invoke_reply1 (GIOPInContext &in,
				    CORBA::ULong &req_id,
				    GIOP::ReplyStatusType &stat,
				    IOP::ServiceContextList &ctx)
{
    CORBA::DataDecoder *dc = in.dc();

#ifdef USE_OLD_INTERCEPTORS
    if (!Interceptor::ClientInterceptor::_idle()) {
	CORBA::Environment env;
	if (!Interceptor::ClientInterceptor::_exec_input_message (
	    dc->buffer(), &env)) {
	    return FALSE;
	}
    }
#endif // USE_OLD_INTERCEPTORS

    CORBA::ULong k;

    check (dc->struct_begin ());
    {
	if (_giop_ver < 0x0102) {
	    check (get_contextlist (in, ctx));
	    check (dc->get_ulong (req_id));
	    check (dc->enumeration (k));
	    stat = (GIOP::ReplyStatusType)k;
	} else {
	    check (dc->get_ulong (req_id));
	    check (dc->enumeration (k));
	    stat = (GIOP::ReplyStatusType)k;
	    check (get_contextlist (in, ctx));
	}
    }
    check (dc->struct_end ());

    switch (stat) {
    case GIOP::NO_EXCEPTION:
    case GIOP::USER_EXCEPTION:
    case GIOP::SYSTEM_EXCEPTION:
    case GIOP::LOCATION_FORWARD:
      break;
    case GIOP::LOCATION_FORWARD_PERM:
      if (_giop_ver < 0x0102) {
	if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	  MICOMT::AutoDebugLock __lock;
	  MICO::Logger::Stream (MICO::Logger::GIOP)
	    << "GIOP: got LOCATION_FORWARD_PERM on GIOP "
	    << (_giop_ver>>8) << "." << (_giop_ver&255)
	    << " connection" << endl;
	}
	return FALSE;
      }
      break;
    case GIOP::NEEDS_ADDRESSING_MODE:
      if (_giop_ver < 0x0102) {
	if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	  MICOMT::AutoDebugLock __lock;
	  MICO::Logger::Stream (MICO::Logger::GIOP)
	    << "GIOP: got LOCATION_FORWARD_PERM on GIOP "
	    << (_giop_ver>>8) << "." << (_giop_ver&255)
	    << " connection" << endl;
	}
	return FALSE;
      }
      break;
    default:
      if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
        MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::GIOP)
	  << "GIOP: got illegal reply status" << endl;
      }
      return FALSE;
    }

    if (_giop_ver >= 0x0102)
	// if there are no arguments aligning might fail ...
	dc->buffer()->ralign (dc->max_alignment());

    return TRUE;
}

CORBA::Boolean
MICO::GIOPCodec::get_invoke_reply2 (GIOPInContext &in,
				    CORBA::ULong req_id,
				    GIOP::ReplyStatusType stat,
				    CORBA::Object_ptr &obj,
				    CORBA::ORBRequest *req,
				    GIOP::AddressingDisposition &ad,
				    IOP::ServiceContextList &ctx)
{
    CORBA::DataDecoder *dc = in.dc();

    obj = CORBA::Object::_nil();

    switch (stat) {
    case GIOP::NO_EXCEPTION:
	if (req) {
	    // may be NULL for a bind()
	    req->context (&ctx);
	    return req->set_out_args (dc, FALSE);
	}
	break;

    case GIOP::USER_EXCEPTION:
    case GIOP::SYSTEM_EXCEPTION:
	if (req) {
	    // may be NULL for a bind()
	    req->context (&ctx);
	    return req->set_out_args (dc, TRUE);
	}
	break;

    case GIOP::LOCATION_FORWARD:
    case GIOP::LOCATION_FORWARD_PERM: {
        CORBA::IOR *ior = new CORBA::IOR ("", CORBA::IOR::IORProfileVec());
        check (dc->get_ior (*ior));
	CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
        obj = orb->ior_to_object (ior);
	break;
    }
    case GIOP::NEEDS_ADDRESSING_MODE: {
	check (dc->get_short (ad));
	break;
    }
    default: {
      return FALSE;
    }
    }
    return TRUE;
}

CORBA::Boolean
MICO::GIOPCodec::get_cancel_request (GIOPInContext &in, CORBA::ULong &req_id)
{
    CORBA::DataDecoder *dc = in.dc();

    check (dc->struct_begin());
    {
	check (dc->get_ulong (req_id));
    }
    check (dc->struct_end());
    return TRUE;
}

CORBA::Boolean
MICO::GIOPCodec::get_locate_request (GIOPInContext &in,
				     CORBA::ULong &req_id,
				     CORBA::Object_ptr obj)
{
    CORBA::DataDecoder *dc = in.dc();

    check (dc->struct_begin());
    {
	check (dc->get_ulong (req_id));
	check (get_target (in, obj));
    }
    check (dc->struct_end());
    return TRUE;
}

CORBA::Boolean
MICO::GIOPCodec::get_locate_reply (GIOPInContext &in,
				   CORBA::ULong &req_id,
				   GIOP::LocateStatusType &stat,
				   CORBA::Object_ptr &obj,
				   GIOP::AddressingDisposition &ad)
{
    CORBA::DataDecoder *dc = in.dc();

    CORBA::ULong k;

    check (dc->struct_begin());
    {
	check (dc->get_ulong (req_id));
	check (dc->enumeration (k));
	stat = (GIOP::LocateStatusType)k;
    }
    check (dc->struct_end());

    switch (stat) {
    case GIOP::UNKNOWN_OBJECT:
    case GIOP::OBJECT_HERE: {
      obj = CORBA::Object::_nil();
      break;
    }

    case GIOP::OBJECT_FORWARD:
    case GIOP::OBJECT_FORWARD_PERM: {
	CORBA::IOR *ior = new CORBA::IOR;
	check (dc->get_ior (*ior));
	CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
	obj = orb->ior_to_object (ior);
	break;
    }

    case GIOP::LOC_NEEDS_ADDRESSING_MODE: {
	check (dc->get_short (ad));
	obj = CORBA::Object::_nil();
	break;
    }

    case GIOP::LOC_SYSTEM_EXCEPTION: // cannot handle yet
    default:
	return FALSE;
    }

    return TRUE;
}

CORBA::Boolean
MICO::GIOPCodec::get_bind_request (GIOPInContext &in, CORBA::String_out repoid,
				   CORBA::ORB::ObjectTag &oid)
{
    /*
     * mapped to an invocation:
     *   GIOP::LocateStatusType _bind (in string repoid,
     *				       in sequence<octet> oid,
     *				       out object obj);
     * at this point only the invoke request in args are left in the
     * buffer...
     */
    CORBA::DataDecoder *dc = in.dc();

    check (dc->struct_begin ());
    {
	check (dc->get_string (repoid));
	CORBA::ULong l;
	check (dc->seq_begin (l));
	oid.length (l);
	check (dc->get_octets (&oid[0], oid.length()));
	check (dc->seq_end ());
    }
    check (dc->struct_end ());

    return TRUE;
}

CORBA::Boolean
MICO::GIOPCodec::get_bind_reply (GIOPInContext &in,
				 GIOP::LocateStatusType &stat,
				 CORBA::Object_ptr &obj)
{
    /*
     * mapped to an invocation:
     *   GIOP::LocateStatusType _bind (in string repoid,
     *				       in sequence<octet> oid,
     *				       out object obj);
     * at this point only the invoke reply out args/result are left in the
     * buffer...
     */
    CORBA::DataDecoder *dc = in.dc();

    CORBA::ULong k;
    CORBA::IOR ior;

    obj = CORBA::Object::_nil();

    check (dc->struct_begin ());
    {
	check (dc->enumeration (k));
	stat = (GIOP::LocateStatusType)k;
	check (dc->get_ior (ior));
    }
    check (dc->struct_end ());

    switch (stat) {
    case GIOP::UNKNOWN_OBJECT:
    case GIOP::OBJECT_HERE:
    case GIOP::OBJECT_FORWARD:
    case GIOP::OBJECT_FORWARD_PERM:
    case GIOP::LOC_NEEDS_ADDRESSING_MODE:
      break;
    case GIOP::LOC_SYSTEM_EXCEPTION:
    default:
      return FALSE;
    }


    CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
    obj = orb->ior_to_object (new CORBA::IOR (ior));
    return TRUE;
}

CORBA::Boolean
MICO::GIOPCodec::get_close_msg (GIOPInContext &in)
{
    return TRUE;
}

CORBA::Boolean
MICO::GIOPCodec::get_error_msg (GIOPInContext &in)
{
    return TRUE;
}

CORBA::Boolean
MICO::GIOPCodec::get_fragment_header (GIOPInContext &in, CORBA::ULong &req_id,
				      CORBA::Boolean &has_id)
{
    CORBA::DataDecoder *dc = in.dc();

    if (_giop_ver < 0x0102) {
	has_id = FALSE;
	return TRUE;
    }

    check (dc->get_ulong (req_id));
    has_id = TRUE;
    return TRUE;
}

#undef check


/****************************** GIOPRequest *****************************/


MICO::GIOPRequest::GIOPRequest (const char *op, CORBA::DataDecoder *indata,
				GIOPCodec *c)
{
    _codec = GIOPCodec::_duplicate (c);

    CORBA::CodeSetCoder *conv = indata->converter();
    CORBA::CodeSetCoder *rconv = conv;

    _opname = op;
    _idc = indata;
    _istart = _idc->buffer()->rpos();

    _oec = _idc->encoder (&_obuf, FALSE, rconv, FALSE);
    _ostart = 0;
    _is_except = FALSE;
}

MICO::GIOPRequest::~GIOPRequest ()
{
    CORBA::release (_codec);
    delete _idc;
    delete _oec;
}

const char *
MICO::GIOPRequest::op_name()
{
    return _opname.c_str();
}

CORBA::Boolean
MICO::GIOPRequest::get_in_args (CORBA::NVList_ptr iparams,
				CORBA::Context_ptr &ctx)
{
    _idc->buffer()->rseek_beg (_istart);

    if (iparams->count() == 0 && _idc->buffer()->length() == 0)
	return TRUE;

    CORBA::Any *a;
    CORBA::TypeCode_var tc;
    CORBA::Boolean ret;

    // share one state for all arguments
    CORBA::DataDecoder::ValueState vstate;
    _idc->valuestate (&vstate, FALSE);

    for (CORBA::ULong i = 0; i < iparams->count(); ++i) {
	if (iparams->item(i)->flags() & (CORBA::ARG_IN|CORBA::ARG_INOUT)) {
	    a = iparams->item(i)->value();
	    tc = a->type();
	    ret = a->demarshal (tc, *_idc);
	    if (!ret) {
		_idc->valuestate (0);
                return FALSE;
	    }
	}
    }
    if (_idc->buffer()->length() > 0) {
	ctx = new CORBA::Context ("");
	if (!_idc->get_context (*ctx)) {
	    CORBA::release (ctx);
	    ctx = CORBA::Context::_nil();
	}
    }
    _idc->valuestate (0);
    return TRUE;
}

CORBA::Boolean
MICO::GIOPRequest::get_in_args (StaticAnyList *iparams,
				CORBA::Context_ptr &ctx)
{
    _idc->buffer()->rseek_beg (_istart);

    if (iparams->size() == 0 && _idc->buffer()->length() == 0)
	return TRUE;

    CORBA::Boolean ret;

    // share one state for all arguments
    CORBA::DataDecoder::ValueState vstate;
    _idc->valuestate (&vstate, FALSE);

    for (mico_vec_size_type i = 0; i < iparams->size(); ++i) {
	if ((*iparams)[i]->flags() & (CORBA::ARG_IN|CORBA::ARG_INOUT)) {
	    ret = (*iparams)[i]->demarshal (*_idc);
            if (!ret) {
		_idc->valuestate (0);
                return FALSE;
	    }
	}
    }
    if (_idc->buffer()->length() > 0) {
	ctx = new CORBA::Context ("");
	if (!_idc->get_context (*ctx)) {
	    CORBA::release (ctx);
	    ctx = CORBA::Context::_nil();
	}
    }
    _idc->valuestate (0);
    return TRUE;
}

CORBA::Boolean
MICO::GIOPRequest::get_in_args (CORBA::DataEncoder *ec)
{
    // XXX alignment, byteorder, differing codecs ???
    assert (!strcmp (ec->type(), _idc->type()));
    assert (ec->byteorder() == _idc->byteorder());

    _idc->buffer()->rseek_beg (_istart);
    ec->put_octets (_idc->buffer()->data(), _idc->buffer()->length());
    return TRUE;
}

CORBA::Boolean
MICO::GIOPRequest::get_out_args (CORBA::Any *res, CORBA::NVList_ptr oparams,
				 CORBA::Exception *&except)
{
#if 0
    except = 0;
    _odc->buffer()->rseek_beg (_ostart);

    if (!_is_except && !res && oparams->count() == 0 &&
	_odc->buffer()->length() == 0)
	return TRUE;

    // share one state for all arguments
    CORBA::DataDecoder::ValueState vstate;
    _odc->valuestate (&vstate, FALSE);

    if (_is_except) {
	except = CORBA::Exception::_decode (*_odc);
	assert (except);
    } else {
	CORBA::TypeCode_var tc;
	CORBA::Boolean ret;
	CORBA::Any *a;

	if (res) {
	    tc = res->type();
	    if (!tc->equal (CORBA::_tc_void)) {
		ret = res->demarshal (tc, *_odc);
                if (!ret) {
		    _odc->valuestate (0);
                    return FALSE;
		}
	    }
	}
	for (CORBA::ULong i = 0; i < oparams->count(); ++i) {
	    if (oparams->item(i)->flags() & (CORBA::ARG_OUT|CORBA::ARG_INOUT)){
		a = oparams->item(i)->value();
		tc = a->type();
		ret = a->demarshal (tc, *_odc);
                if (!ret) {
		    _odc->valuestate (0);
                    return FALSE;
		}
	    }
	}
    }
    _odc->valuestate (0);
#else
    assert (0);
#endif
    return TRUE;
}

CORBA::Boolean
MICO::GIOPRequest::get_out_args (CORBA::StaticAny *res,
				 StaticAnyList *oparams,
				 CORBA::Exception *&except)
{
#if 0
    except = 0;
    _odc->buffer()->rseek_beg (_ostart);

    if (!_is_except && !res && oparams->size() == 0 &&
	_odc->buffer()->length() == 0)
	return TRUE;

    // share one state for all arguments
    CORBA::DataDecoder::ValueState vstate;
    _odc->valuestate (&vstate, FALSE);

    if (_is_except) {
	except = CORBA::Exception::_decode (*_odc);
	assert (except);
    } else {
	CORBA::Boolean ret;
	if (res) {
	    ret = res->demarshal (*_odc);
            if (!ret) {
		_odc->valuestate (0);
                return FALSE;
	    }
	}
	for (mico_vec_size_type i = 0; i < oparams->size(); ++i) {
	    if ((*oparams)[i]->flags() & (CORBA::ARG_OUT|CORBA::ARG_INOUT)){
		ret = (*oparams)[i]->demarshal (*_odc);
		if (!ret) {
		    _odc->valuestate (0);
                    return FALSE;
		}
	    }
	}
    }
    _odc->valuestate (0);
#else
    assert (0);
#endif
    return TRUE;
}

CORBA::Boolean
MICO::GIOPRequest::get_out_args (CORBA::DataEncoder *ec, CORBA::Boolean &is_ex)
{
    // XXX alignment, byteorder, differing codecs ???
    assert (!strcmp (ec->type(), _oec->type()));
    assert (ec->byteorder() == _oec->byteorder());

    _oec->buffer()->rseek_beg (_ostart);
    ec->put_octets (_oec->buffer()->data(), _oec->buffer()->length());
    is_ex = _is_except;

    return TRUE;
}

CORBA::Boolean
MICO::GIOPRequest::set_out_args (CORBA::Any *res, CORBA::NVList_ptr oparams)
{
    _is_except = FALSE;
    _oec->buffer()->reset();

    if (!res && oparams->count() == 0)
	return TRUE;

    if (_codec->version() < 0x0102) {
	// this is to get alignment right
	GIOPOutContext out (_oec);
	_codec->put_invoke_reply_offset (out, this);
	_ostart = _oec->buffer()->wpos();
    }
    _oec->buffer()->rseek_beg (_ostart);

    // share one state for all arguments
    CORBA::DataEncoder::ValueState vstate;
    _oec->valuestate (&vstate, FALSE);

    if (res) {
	if (!res->marshal (*_oec))
	    return FALSE;
    }
    for (CORBA::ULong i = 0; i < oparams->count(); ++i) {
	if (oparams->item(i)->flags() & (CORBA::ARG_OUT|CORBA::ARG_INOUT)) {
	    if (!oparams->item(i)->value()->marshal (*_oec))
		return FALSE;
	}
    }
    _oec->valuestate (0);
    return TRUE;
}

CORBA::Boolean
MICO::GIOPRequest::set_out_args (CORBA::StaticAny *res,
				 StaticAnyList *oparams)
{
    _is_except = FALSE;
    _oec->buffer()->reset();

    if (!res && oparams->size() == 0)
	return TRUE;

    if (_codec->version() < 0x0102) {
	// this is to get alignment right
	GIOPOutContext out (_oec);
	_codec->put_invoke_reply_offset (out, this);
	_ostart = _oec->buffer()->wpos();
    }
    _oec->buffer()->rseek_beg (_ostart);

    // share one state for all arguments
    CORBA::DataEncoder::ValueState vstate;
    _oec->valuestate (&vstate, FALSE);

    if (res) {
	if (!res->marshal (*_oec))
	    return FALSE;
    }
    for (mico_vec_size_type i = 0; i < oparams->size(); ++i) {
	if ((*oparams)[i]->flags() & (CORBA::ARG_OUT|CORBA::ARG_INOUT)) {
	    if (!(*oparams)[i]->marshal (*_oec))
		return FALSE;
	}
    }
    _oec->valuestate (0);
    return TRUE;
}

void
MICO::GIOPRequest::set_out_args (CORBA::Exception *except)
{
    _oec->buffer()->reset();

    if (_codec->version() < 0x0102) {
	// this is to get alignment right
	GIOPOutContext out (_oec);
	_codec->put_invoke_reply_offset (out, this);
	_ostart = _oec->buffer()->wpos();
    }
    _oec->buffer()->rseek_beg (_ostart);

    // share one state for all arguments
    CORBA::DataEncoder::ValueState vstate;
    _oec->valuestate (&vstate, FALSE);

    _is_except = TRUE;
    except->_encode (*_oec);

    _oec->valuestate (0);
}

CORBA::Boolean
MICO::GIOPRequest::set_out_args (CORBA::DataDecoder *dc, CORBA::Boolean is_ex)
{
    _oec->buffer()->reset();

    if (_codec->version() < 0x0102) {
	// this is to get alignment right
	GIOPOutContext out (_oec);
	_codec->put_invoke_reply_offset (out, this);
	_ostart = _oec->buffer()->wpos();
    }
    _oec->buffer()->rseek_beg (_ostart);

    // XXX alignment, byteorder, differing codecs ???
    assert (!strcmp (dc->type(), _oec->type()));
    assert (dc->byteorder() == _oec->byteorder());

    _oec->put_octets (dc->buffer()->data(), dc->buffer()->length());
    dc->buffer()->rseek_rel (dc->buffer()->length());
    _is_except = is_ex;
    return TRUE;
}

CORBA::Boolean
MICO::GIOPRequest::copy_out_args (CORBA::ORBRequest *req)
{
    if (this == req)
	return TRUE;

    // copy service context list
    copy_svc (req);

    assert (_oec);

    _oec->buffer()->reset();

    if (_codec->version() < 0x0102) {
	// this is to get alignment right
	GIOPOutContext out (_oec);
	_codec->put_invoke_reply_offset (out, this);
	_ostart = _oec->buffer()->wpos();
    }
    _oec->buffer()->rseek_beg (_ostart);

    req->get_out_args (_oec, _is_except);
    return TRUE;
}

CORBA::Boolean
MICO::GIOPRequest::copy_in_args (CORBA::ORBRequest *req)
{
    assert (0);
    return FALSE;
}

const char *
MICO::GIOPRequest::type ()
{
    return "giop";
}

CORBA::ByteOrder
MICO::GIOPRequest::input_byteorder ()
{
    return _idc->byteorder();
}

CORBA::ByteOrder
MICO::GIOPRequest::output_byteorder ()
{
    return _oec->byteorder();
}

CORBA::CodeSetCoder *
MICO::GIOPRequest::converter ()
{
    return _idc->converter();
}
    
/***************************** GIOPSimpleProf ****************************/


MICO::GIOPSimpleProf::GIOPSimpleProf ()
    : _objkey (0), _length (0)
{
}

MICO::GIOPSimpleProf::GIOPSimpleProf (const GIOPSimpleProf &p)
    : _objkey (0), _length (0)
{
    if (p._objkey) {
	_objkey = new CORBA::Octet[_length = p._length];
	memcpy (_objkey, p._objkey, p._length);
    }
}

MICO::GIOPSimpleProf::~GIOPSimpleProf ()
{
    if (_objkey)
	delete[] _objkey;
}

MICO::GIOPSimpleProf &
MICO::GIOPSimpleProf::operator= (const GIOPSimpleProf &p)
{
    if (this != &p)
	objectkey (p._objkey, p._length);
    return *this;
}

void
MICO::GIOPSimpleProf::encode (CORBA::DataEncoder &) const
{
    assert (0);
}

const CORBA::Address *
MICO::GIOPSimpleProf::addr () const
{
    assert (0);
    return 0;
}

CORBA::IORProfile::ProfileId
MICO::GIOPSimpleProf::id () const
{
    return 10000;
}

CORBA::IORProfile::ProfileId
MICO::GIOPSimpleProf::encode_id () const
{
    return 10000;
}

void
MICO::GIOPSimpleProf::objectkey (CORBA::Octet *o, CORBA::Long l)
{
    if (_objkey)
	delete[] _objkey;
    _objkey = new CORBA::Octet[_length = l];
    memcpy (_objkey, o, l);
}

const CORBA::Octet *
MICO::GIOPSimpleProf::objectkey (CORBA::Long &l) const
{
    l = _length;
    if (!_objkey)
	return (const CORBA::Octet *)"";
    return _objkey;
}

CORBA::Boolean
MICO::GIOPSimpleProf::reachable ()
{
    return TRUE;
}

void
MICO::GIOPSimpleProf::print (ostream &o) const
{
  o << "GIOP Simple Profile" << endl;
}

CORBA::IORProfile *
MICO::GIOPSimpleProf::clone () const
{
    return new GIOPSimpleProf (*this);
}

CORBA::Long
MICO::GIOPSimpleProf::compare (const CORBA::IORProfile &p) const
{
    if (p.id() != id())
	return (CORBA::Long)id() - (CORBA::Long)p.id();
    const GIOPSimpleProf &gp = (const GIOPSimpleProf &)p;
    if (_length != gp._length)
	return (CORBA::Long)_length - (CORBA::Long)gp._length;
    return mico_key_compare (_objkey, gp._objkey, _length);
}

CORBA::Boolean
MICO::GIOPSimpleProf::operator== (const CORBA::IORProfile &p) const
{
    return compare (p) == 0;
}

CORBA::Boolean
MICO::GIOPSimpleProf::operator< (const CORBA::IORProfile &p) const
{
    return compare (p) < 0;
}

#ifdef HAVE_THREADS
/******************************* GIOPConnReader *****************************/

MICO::GIOPConnReader::GIOPConnReader( MICO::GIOPConn *_conn ) {

    if (MICO::Logger::IsLogged(MICO::Logger::GIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream(MICO::Logger::GIOP)
	    << "MICO::GIOPConnReader::GIOPConnReader(" << _conn << ")" << endl;
    }
    conn = _conn;
}

void
MICO::GIOPConnReader::_run(void* arg) {

    if (MICO::Logger::IsLogged(MICO::Logger::GIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream(MICO::Logger::GIOP)
	    << "MICO::GIOPConnReader::_run()" << endl;
    }
    GIOPConn::set_as_reader_thread();
    if (MICO::MTManager::threaded_client() && conn->side() == CLIENT_SIDE) {
        // client side using threaded client
        while (conn->state() == MICOMT::StateRefCnt::Active) {
            conn->dispatcher()->run(FALSE);
        }
    }
    else {
        // client or server (TPC) side using reader
        conn->do_read(FALSE);
    }
}

/******************************* GIOPConnWriter *****************************/

MICO::GIOPConnWriter::GIOPConnWriter( MICO::GIOPConn *_conn ) {

    WorkerThread *kt;
    PassiveMsgQueue     *mq;

    __NAME( name( "GIOPConnWriter" ) );
    conn = _conn;
    mq = new PassiveMsgQueue();
    __NAME( mq->name ("GIOPConnWriter") );
    input_mc = mq;
    kt = MICO::MTManager::thread_pool_manager()->get_idle_thread( Operation::Writer );
    assert (kt);
    kt->register_operation( this );
    kt->mark_busy();
}

MICO::GIOPConnWriter::~GIOPConnWriter() {

    delete input_mc;
}

void
MICO::GIOPConnWriter::process( msg_type * msg ) {

    CORBA::Buffer *b = (CORBA::Buffer *)msg->data();

     if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	 MICOMT::AutoDebugLock __lock;
	 MICO::Logger::Stream (MICO::Logger::GIOP)
	     << "void GIOPConnWriter::GIOPConnWriter process( msg_type& msg )" << endl;
     }
    conn->output_handler( b );
    delete msg;

}

void
MICO::GIOPConnWriter::send_msg( msg_type * msg) {

    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::GIOP)
	    << "void GIOPConnWriter::send_msg( msg_type* msg )" << endl;
    }
    this->get_input_mc()->put_msg( MICO::Operation::Writer, msg );
}

/******************************* InputHandler ***************************/

MICO::InputHandler::InputHandler(): PassiveOperation()
{
    __NAME( name( "InputHandler" ) );
    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::GIOP)
	    << "MICO::InputHandler::InputHandler()" << endl;
    }
    this->info().set_op_id( MICO::Operation::DeCode );
}


MICO::InputHandler::InputHandler( ThreadPool& _tp ): PassiveOperation( _tp) {

    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::GIOP)
	    << "MICO::InputHandler::InputHandler( ThreadPoolManager& _tp )" << endl;
    }
    this->info().set_op_id( MICO::Operation::DeCode );
}

void
MICO::InputHandler::process( msg_type * msg ) {

    MICO::GIOPConnMsg *m = (MICO::GIOPConnMsg *)msg->data();

    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::GIOP)
	    << "void  InputHandler::process( msg_type& msg )" << endl
	    << "  conn: " << m->conn << endl
	    << "    ev: " << m->ev << endl
	    << "     b: " << m->b << endl;
    }

    m->conn->input_ready_callback( m->b );

    delete m;
    delete msg;
}

MICO::Operation *
MICO::InputHandler::copy() const {

    return new InputHandler( *this );
}

#endif // HAVE_THREADS

/******************************* GIOPConn *******************************/

#ifdef HAVE_THREADS
MICO::GIOPConn::GIOPConn (CORBA::Dispatcher *disp, CORBA::Transport *transp,
			  GIOPConnCallback *cb, GIOPCodec *codec,
			  CORBA::Long tmout, CORBA::ULong max_size,
                          MICO::GIOPSide side,
			  CORBA::Boolean __reader_thread,
			  CORBA::Boolean __writer_thread,
			  CORBA::ULong id)
  : write_lock_(FALSE, MICOMT::Mutex::Recursive), _inctx (codec, new CORBA::Buffer()),
    side_(side), _M_use_reader_thread(__reader_thread), _M_use_writer_thread(__writer_thread)
#else
MICO::GIOPConn::GIOPConn (CORBA::Dispatcher *disp, CORBA::Transport *transp,
			  GIOPConnCallback *cb, GIOPCodec *codec,
			  CORBA::Long tmout, CORBA::ULong max_size,
			  CORBA::ULong id)
  : _inctx (codec, new CORBA::Buffer())
#endif
{
    _disp = disp;
    _transp = transp;
    _cb = cb;
    // we take over ownership of codec ...
    _codec = codec;
    _max_message_size = max_size;
    _id = id;

    _inbuf = new CORBA::Buffer;
    _inlen = _codec->header_length ();
    _inflags = 0;
    _infrag = 0;
    _inbufs = 0;
    _total_fragsize = 0;

    _refcnt = 0;
    _idle_tmout = tmout;
    _have_tmout = FALSE;
    _have_wselect = FALSE;
#ifdef HAVE_THREADS
    if (side_ == CLIENT_SIDE && MICO::MTManager::blocking_threaded_client()) {
        _disp->block(TRUE);
    }
    if (side_ == SERVER_SIDE && MICO::MTManager::thread_per_connection()) {
        _disp->block(TRUE);
    }
#endif // HAVE_THREADS
    _transp->block ( _disp->isblocking() );
    _transp->rselect (_disp, this);

#ifdef HAVE_THREADS
    _reader = NULL;
    _writer = NULL;
    if (_M_use_reader_thread)
      _reader = new GIOPConnReader(this);
    if (_M_use_writer_thread)
      _writer = new GIOPConnWriter(this);
#endif

    check_idle ();
}

#ifdef HAVE_THREADS
// added for debuging reason
// because I'm not able to add breakpoint on line in .h file :-((
CORBA::Boolean
MICO::GIOPConn::active_ref()
{
    //cerr << "active_ref(): " << _activerefs << " -> " << (_activerefs + 1) << endl;
    return MICOMT::StateRefCnt::active_ref();
}

void
MICO::GIOPConn::active_deref()
{
    //cerr << "active_deref(): " << _activerefs << " -> " << (_activerefs - 1) << endl;
    return MICOMT::StateRefCnt::active_deref();
}

CORBA::Boolean
MICO::GIOPConn::is_this_reader_thread()
{
    assert(S_reader_key_initialized_);
    if (MICOMT::Thread::get_specific(S_reader_key_) != NULL) {
        return TRUE;
    }
    return FALSE;
}

void
MICO::GIOPConn::set_as_reader_thread()
{
    assert(S_reader_key_initialized_);
    // kcg: this is a trick: reuse bool value
    // for key setup with known storage and omit a need
    // for key value cleanup function
    MICOMT::Thread::set_specific(S_reader_key_, &S_reader_key_initialized_);
}

void
MICO::GIOPConn::initialize_reader_key()
{
    // kcg: we are called from ORB_init, so we might be called several
    // times!
    if (!S_reader_key_initialized_) {
        MICOMT::Thread::create_key(S_reader_key_, NULL);
        S_reader_key_initialized_ = true;
    }
}
#endif // HAVE_THREADS

/*
 * connection shutdown is very complicated for mt
 * 
 * we cannot delete the GIOPConn object before 
 * all running invocations have been terminated
 *
 */
#ifdef HAVE_THREADS
void
MICO::GIOPConn::terminate ()
{
    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::GIOP)
	    << "MICO::GIOPConn::terminate"<< endl;
    }
    assert (_refcnt >= 0);
    if (!this->state_change( MICOMT::StateRefCnt::Shutdown )) {
	return;
    }
    list<CORBA::Buffer *>::iterator i;
    for (i = _outbufs.begin(); i != _outbufs.end(); ++i)
	delete *i;

    _disp->remove (this, CORBA::Dispatcher::Timer);

    //REMEMBER: terminate tells the thread to exit
    //          it does NOT cancel the thread
    if (_M_use_writer_thread)
      _writer->init_shutdown();

    _transp->close();

    _transp->rselect (_disp, 0);
    _transp->wselect (_disp, 0);

    if (_M_use_writer_thread) {
      _writer->finalize_shutdown();
      delete _writer;
    }
    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::GIOP)
	    << "  GIOPConn::terminated"<< endl;
    }
    // do not delete: disp
    this->state_change( MICOMT::StateRefCnt::Terminated );
}
#endif

MICO::GIOPConn::~GIOPConn ()
{
    if (_refcnt != 0 ) {
	if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::GIOP)
		<< "_refcnt == 0: " << _refcnt 
		<< " peer: " << _transp->peer()->stringify() << endl;
	}
    }

    assert (_refcnt == 0);

    delete _transp;
    delete _inbuf;
    CORBA::release (_codec);
#ifdef HAVE_THREADS
    if (_M_use_reader_thread) {
        delete _reader;
    }
    if (this->side() == CLIENT_SIDE && !MICO::MTManager::reactive_client()) {
        // we're using our own dispatcher
        delete _disp;
    }
#endif // HAVE_THREADS
}

#ifdef HAVE_THREADS
void 
MICO::GIOPConn::start() 
{
    this->ref();
    this->state_change( MICOMT::StateRefCnt::Active );
    if (_M_use_writer_thread)
      _writer->start();
    if (_M_use_reader_thread) {
        if (_reader->start() != MICOMT::Thread::NoError) {
            // kcg: does not work for TPC CM!
            mico_throw(CORBA::NO_RESOURCES());
        }
    }
}

CORBA::Boolean
MICO::GIOPConn::input_ready()
{
    if (!this->active_ref())
	//connection is being terminated
	return FALSE;

    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::GIOP)
	    << "MICO::GIOPConn::input_ready ()" << endl
	    << "  conn: " << this << endl
	    << "    ev: GIOPConnCallback::InputReady" << endl
	    << " t_mod: " << MICO::MTManager::server_concurrency_model() << endl
	    << "  pool: " << MICO::MTManager::thread_pool() << endl
	    << "  conn: " << MICO::MTManager::thread_per_connection() << endl
	    << "   req: " << MICO::MTManager::thread_per_request() << endl
	    << "_activerefs: " << _activerefs << endl;
    }
	
    if (this->side() == CLIENT_SIDE) {
        // kcg: we're on client side, hence passing input
        // directly down inside this thread
        return this->input_ready_callback(this->input());
    }
    if (MICO::MTManager::thread_pool()) {
        MICO::msg_type    *msg;
        msg = new msg_type(new GIOPConnMsg(this, this->input(),
					   GIOPConnCallback::InputReady));
        MICO::MTManager::thread_pool_manager()->get_thread_pool(MICO::Operation::DeCode).put_msg(0, msg);
        return TRUE;
    }
    if (MICO::MTManager::thread_per_request()) {
        MICO::msg_type    *msg;
	msg = new msg_type(new GIOPConnMsg(this, this->input(),
					   GIOPConnCallback::InputReady));
	//_reader->send_msg( MICO::Operation::DeCode, msg );
        assert(0); // how to rewrite line above for new design?
	return TRUE;
    }
    if (MICO::MTManager::thread_per_connection()) {
	return this->input_ready_callback(this->input());
    }
    // never reached
    assert(0);
    return FALSE;
}

void
MICO::GIOPConn::close_connection() 
{
    if ( !this->init_shutdown() )
	return;

    MICO::msg_type *msg;

    msg = new msg_type( new ORBMsg(this, _cb, ORBMsg::CloseConn) );

    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::GIOP)
	    << "MICO::GIOPConn::close_connection()" << endl
	    << "  conn: " << this << endl;
    }

    MICO::MTManager::thread_pool_manager()->put_msg( MICO::Operation::ORB, msg );
}
#else
void
MICO::GIOPConn::close_connection() 
{
    _cb->callback (this, GIOPConnCallback::Closed);
}
#endif


void
MICO::GIOPConn::do_read ( const CORBA::Boolean break_after_read )
{
    while (42) {
	assert (_inlen > 0);
	CORBA::Long r = _transp->read (*_inbuf, _inlen);
#if 1
	if (r < 0) {
            /*
             * workaround for a problem on Linux and Win32
             * (and probably other OSes as well):
             *   if you send something on a TCP connection that has
             *   been closed by the peer you will get an error on
             *   the next read() instead of beeing able to read
             *   the remaining data until you reach EOF.
             */
            r = _transp->read (*_inbuf, _inlen);
        }
#endif
	if (r < 0 || (r == 0 && _transp->eof())) {
	    // connection broken or EOF
	    _transp->rselect (_disp, 0);
	    _transp->wselect (_disp, 0);
	    this->close_connection();
	    // _cb->callback (this, GIOPConnCallback::Closed);
	    break;
	} else if (r > 0) {
	    _inlen -= r;
	    if (_inbuf->length() == _codec->header_length()) {
		// header completely received
		assert (_inlen == 0);
		GIOP::MsgType mt;
		_inctx.buffer (_inbuf);
		if (!_codec->check_header (_inctx, mt, _inlen, _inflags)) {
		    // garbled message, send it to input handler ...
		    assert (!_inbufs);
		    _inbufs = _inbuf;
		    _inbuf = new CORBA::Buffer;
		    _inlen = _codec->header_length ();
		    _transp->rselect (_disp, 0);
		    _transp->wselect (_disp, 0);
		    this->input_ready();
		    break;
		}
                if (_max_message_size > 0 && _inlen > _max_message_size) {
		  if (MICO::Logger::IsLogged (MICO::Logger::Transport)) {
		    MICOMT::AutoDebugLock __lock;
		    _inbuf->dump ("In Data", MICO::Logger::Stream (MICO::Logger::Transport));
		  }
                  if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
		    MICOMT::AutoDebugLock __lock;
                    MICO::Logger::Stream (MICO::Logger::GIOP)
                      << "GIOP: message size (" << _inlen << " bytes) "
                      << "larger than threshold (" << _max_message_size
                      << ")" << endl;
                  }
                  _transp->rselect (_disp, 0);
                  _transp->wselect (_disp, 0);
		  this->close_connection();
                  //_cb->callback (this, GIOPConnCallback::Closed);
                  break;
                }
		if (mt == GIOP::Fragment || _inflags & GIOP_FRAGMENT_BIT) {
		  if (_max_message_size > 0 &&
		      _inlen + _total_fragsize > _max_message_size) {
		    if (MICO::Logger::IsLogged (MICO::Logger::Transport)) {
		      MICOMT::AutoDebugLock __lock;
		      _inbuf->dump ("In Data", MICO::Logger::Stream (MICO::Logger::Transport));
		    }
		    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
		      MICOMT::AutoDebugLock __lock;
		      MICO::Logger::Stream (MICO::Logger::GIOP)
			<< "GIOP: fragments size (" << _total_fragsize
			<< "+" << _inlen << " bytes) "
			<< "larger than threshold (" << _max_message_size
			<< ")" << endl;
		    }
		    _transp->rselect (_disp, 0);
		    _transp->wselect (_disp, 0);
		    //_cb->callback (this, GIOPConnCallback::Closed);
		    this->close_connection();
		    break;
		  }
		}
		if (mt == GIOP::Fragment) {
		  if (_codec->version() == 0x0101) {
		    // a fragment, append contents to infrag ...
		    if (!_infrag) {
		      if (MICO::Logger::IsLogged (MICO::Logger::Transport)) {
			MICOMT::AutoDebugLock __lock;
			_inbuf->dump ("In Data", MICO::Logger::Stream (MICO::Logger::Transport));
		      }
		      if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
			MICOMT::AutoDebugLock __lock;
			MICO::Logger::Stream (MICO::Logger::GIOP)
			  << "GIOP: got unexpected fragment" << endl;
		      }
		      _transp->rselect (_disp, 0);
		      _transp->wselect (_disp, 0);
		      //_cb->callback (this, GIOPConnCallback::Closed);
		      this->close_connection();
		      break;
		    }
		    delete _inbuf;
		    _inbuf = _infrag;
		    _infrag = 0;
		  }
		}
	    }
	    if (_inlen == 0) {
	      // message completely received
	      // check if it is a fragment message

	      CORBA::Octet mt;
	      _inbuf->rseek_beg (7);
	      _inbuf->get1 (&mt);
	      _inbuf->rseek_beg (0);

	      if ((GIOP::MsgType) mt == GIOP::Fragment &&
		  _codec->version() == 0x0102) {
		if (MICO::Logger::IsLogged (MICO::Logger::Transport)) {
		  MICOMT::AutoDebugLock __lock;
		  _inbuf->dump ("Frag Data", MICO::Logger::Stream (MICO::Logger::Transport));
		}
		if (_inbuf->length() < 16) {
		  if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
		    MICOMT::AutoDebugLock __lock;
		    MICO::Logger::Stream (MICO::Logger::GIOP)
		      << "GIOP: short fragment error"
		      << endl;
		  }
		  _transp->rselect (_disp, 0);
		  _transp->wselect (_disp, 0);
		  //_cb->callback (this, GIOPConnCallback::Closed);
		  this->close_connection();
		  break;
		}
		// we don't really need the correct byte order here
		CORBA::ULong reqid;
		_inbuf->rseek_beg (12); // request id
		_inbuf->get4 (&reqid);
		if (_fragments.find (reqid) == _fragments.end()) {
		  if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
		    MICOMT::AutoDebugLock __lock;
		    MICO::Logger::Stream (MICO::Logger::GIOP)
		      << "GIOP: got fragment with unknown request id "
		      << reqid << endl;
		  }
		  _transp->rselect (_disp, 0);
		  _transp->wselect (_disp, 0);
		  //_cb->callback (this, GIOPConnCallback::Closed);
		  this->close_connection();
		  break;
		}
		if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
		  MICOMT::AutoDebugLock __lock;
		  MICO::Logger::Stream (MICO::Logger::GIOP)
		    << "GIOP: got fragment for request id " << reqid
		    << ((_inflags & GIOP_FRAGMENT_BIT) ? " (more follow)" :
			" (complete)") << endl;
		}
		// add contents of _inbuf (after the request id) to the
		// stored buffer
		CORBA::Buffer * fragbuf = _fragments[reqid];
		fragbuf->put (_inbuf->data(), _inbuf->length());
		_total_fragsize += _inbuf->length();
		// reset input buffer
		_inbuf->reset ();
		_inlen = _codec->header_length();
		// do more fragments follow? then continue
		if (_inflags & GIOP_FRAGMENT_BIT) {
		  continue;
		}
		// pass data to input handler
		assert (!_inbufs);
		_fragments.erase (reqid);
		_inbufs = fragbuf;
		assert (_inbuf->length() < _total_fragsize);
		_total_fragsize -= _inbuf->length();
		//_cb->callback (this, GIOPConnCallback::InputReady);
		if (!this->input_ready() || break_after_read)
		    break;
	      }

	      if (_inflags & GIOP_FRAGMENT_BIT) {
		// more fragments follow ...
		if (MICO::Logger::IsLogged (MICO::Logger::Transport)) {
		  MICOMT::AutoDebugLock __lock;
		  _inbuf->dump ("Frag Data", MICO::Logger::Stream (MICO::Logger::Transport));
		}
		if (_codec->version() >= 0x0102) {
		  // determine request id
		  if (_inbuf->length() < 16) {
		    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
		      MICOMT::AutoDebugLock __lock;
		      MICO::Logger::Stream (MICO::Logger::GIOP)
			<< "GIOP: message fragmented before request id"
			<< endl;
		    }
		    _transp->rselect (_disp, 0);
		    _transp->wselect (_disp, 0);
		    //_cb->callback (this, GIOPConnCallback::Closed);
		    this->close_connection();
		    break;
		  }
		  if (_max_message_size &&
		      (_inbuf->length() + _total_fragsize > 
		       _max_message_size)) {
		    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
		      MICOMT::AutoDebugLock __lock;
		      MICO::Logger::Stream (MICO::Logger::GIOP)
			<< "GIOP: fragments size (" << _total_fragsize
			<< "+" << _inbuf->length() << " bytes) "
			<< "larger than threshold (" << _max_message_size
			<< ")" << endl;
		    }
		    _transp->rselect (_disp, 0);
		    _transp->wselect (_disp, 0);
		    //_cb->callback (this, GIOPConnCallback::Closed);
		    this->close_connection();
		    break;
		  }
		  // we don't really need the correct byte order here
		  CORBA::ULong reqid;
		  _inbuf->rseek_beg (12); // request id
		  _inbuf->get4 (&reqid);
		  if (_fragments.find (reqid) != _fragments.end()) {
		    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
		      MICOMT::AutoDebugLock __lock;
		      MICO::Logger::Stream (MICO::Logger::GIOP)
			<< "GIOP: got first fragment for request id "
			<< reqid << " which is already there" << endl;
		    }
		    _transp->rselect (_disp, 0);
		    _transp->wselect (_disp, 0);
		    //_cb->callback (this, GIOPConnCallback::Closed);
		    this->close_connection();
		    break;
		  }
		  if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
		    MICOMT::AutoDebugLock __lock;
		    MICO::Logger::Stream (MICO::Logger::GIOP)
		      << "GIOP: got first fragment for id "
		      << reqid << endl;
		  }
		  _inbuf->rseek_beg (0);
		  _fragments[reqid] = _inbuf;
		  _total_fragsize += _inbuf->length();
		  _inbuf = new CORBA::Buffer;
		  _inlen = _codec->header_length();
		}
		else {
		  // handle GIOP 1.1 fragments
		  if (_infrag) {
		    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
		      MICOMT::AutoDebugLock __lock;
		      MICO::Logger::Stream (MICO::Logger::GIOP)
			<< "GIOP: got fragment while still in fragment"
			<< endl;
		    }
		    _transp->rselect (_disp, 0);
		    _transp->wselect (_disp, 0);
		    //_cb->callback (this, GIOPConnCallback::Closed);
		    this->close_connection();
		    break;
		  }
		  _infrag = _inbuf;
		  _inbuf = new CORBA::Buffer;
		  _inlen = _codec->header_length();
		}
	      } else {
		assert (!_inbufs);
		_inbufs = _inbuf;
		_inbuf = new CORBA::Buffer;
		_inlen = _codec->header_length ();
		//_cb->callback (this, GIOPConnCallback::InputReady);
		if (!this->input_ready() || break_after_read)
		    break;
	      }
	    }
	} else if (r == 0) {
	    break;
	} else {
	    assert (0);
	}
    }
}

void
MICO::GIOPConn::do_write ()
{
    MICOMT::AutoLock lock(write_lock_);

    while (42) {
	assert (_outbufs.size() > 0);
	CORBA::Buffer *b = _outbufs.front();
	CORBA::Long r = _transp->write (*b, b->length());
	if (r > 0) {
	    if (b->length() == 0) {
		// message completely sent
		delete b;
		_outbufs.pop_front();
		if (_outbufs.size() == 0) {
		    check_idle ();
		    break;
		}
	    }
	} else if (r < 0) {
	    // connection broken
	    _transp->rselect (_disp, 0);
	    _transp->wselect (_disp, 0);
	    this->close_connection();
	    // _cb->callback (this, GIOPConnCallback::Closed);
	    break;
	} else if (r == 0) {
	    break;
	} else {
	    assert (0);
	}
    }
}

void
MICO::GIOPConn::callback (CORBA::Transport *,
			  CORBA::TransportCallback::Event ev)
{
    switch (ev) {
    case Read:
	do_read ( _disp->isblocking() );
	break;

    case Write:
	do_write ();
	break;

    default:
	assert (0);
    }
}

void
MICO::GIOPConn::callback (CORBA::Dispatcher *d,
			  CORBA::DispatcherCallback::Event ev)
{
    switch (ev) {
    case CORBA::Dispatcher::Timer:
	_cb->callback (this, GIOPConnCallback::Idle);
	break;
    case CORBA::Dispatcher::Moved:
	_disp = d;
        break;
    default:
	assert (0);
    }
}

#ifndef HAVE_THREADS

void
MICO::GIOPConn::check_idle ()
{
    if (_idle_tmout > 0 && _refcnt == 0 && _outbufs.size() == 0) {
        if (_have_tmout)
            _disp->remove (this, CORBA::Dispatcher::Timer);
	_disp->tm_event (this, _idle_tmout);
	_have_tmout = TRUE;
    }
    if (_have_wselect && _outbufs.size() == 0) {
        _transp->wselect (_disp, 0);
        _have_wselect = FALSE;
    }
}

void
MICO::GIOPConn::check_busy ()
{
    if (_have_tmout && (_refcnt > 0 || _outbufs.size() > 0)) {
	_disp->remove (this, CORBA::Dispatcher::Timer);
	_have_tmout = FALSE;
    }
    if (!_have_wselect && _outbufs.size() > 0) {
        _transp->wselect (_disp, this);
        _have_wselect = TRUE;
    }
}

CORBA::Boolean
MICO::GIOPConn::check_events ()
{
    if (_transp->isreadable()) {
        do_read( _disp->isblocking() );
        return TRUE;
    }
    return FALSE;
}

void
MICO::GIOPConn::ref ()
{
    ++_refcnt;
    check_busy ();
}

CORBA::Boolean
MICO::GIOPConn::deref (CORBA::Boolean all)
{
    if (all)
	_refcnt = 0;
    else
	--_refcnt;
    check_idle ();
    return (_refcnt <= 0);
}

#endif // HAVE_THREADS

#ifdef HAVE_THREADS

void
MICO::GIOPConn::ref()
{
    MICOMT::AutoLock lock(refcnt_lock_);
    _refcnt++;
}

CORBA::Boolean
MICO::GIOPConn::deref (CORBA::Boolean all)
{
    MICOMT::AutoLock lock(refcnt_lock_);
    --_refcnt;
    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::GIOP) 
	    << "GIOPConn::deref: " << this 
	    << ", refcnt: " << _refcnt 
	    << ", activerefs: " << _activerefs 
	    << endl;
    }
    return (_refcnt == 0);
}

#endif // HAVE_THREADS

void
MICO::GIOPConn::output_handler (CORBA::Buffer *b)
{
    MICOMT::AutoLock lock(write_lock_);

    if (MICO::Logger::IsLogged (MICO::Logger::Transport)) {
	MICOMT::AutoDebugLock __lock;
	b->dump ("Out Data", MICO::Logger::Stream (MICO::Logger::Transport));
    }

    // try to write as much as possible immediatly
    if (_outbufs.size() == 0) {
	_transp->write (*b, b->length());
	if (b->length() == 0) {
	    delete b;
	    return;
	}
#ifndef HAVE_THREADS
	_outbufs.push_back (b);
#else // HAVE_THREADS
	if (!MICO::MTManager::thread_pool()) {
	    _outbufs.push_back (b);
	}
	else {
	    // thread pool concurrency model
	    // kcg: it is better here to write in busy-loop
	    // instead of scheduling connection for later
	    // writting with 
	    //	    _transp->wselect (_disp, this);
	    // 	    _have_wselect = TRUE;
	    // the problem is that with select, the connection (write)
	    // needs to wait timeout of select to get into the select set
	    // => busy wait is better for multi-threading I/O
	    // We need this only for thread-pool since it is the only one
	    // multi-threaded concurrency model using non-blocking I/O
	    // Anyway, if we find any solution how to interrupt select
	    // to get into select set sooner, then it should be rewritten
	    // to use select instead of busy-wait.
	    while(b->length() != 0) {
		CORBA::Long r = _transp->write (*b, b->length());
		if (r < 0) {
		    // connection broken
		    _transp->rselect (_disp, 0);
		    _transp->wselect (_disp, 0);
		    this->close_connection();
		    break;
		}
	    }
	    delete b;
	}
#endif // HAVE_THREADS
    } else {
	_outbufs.push_back (b);
	do_write ();
    }
    check_busy ();
}

#ifdef HAVE_THREADS

void
MICO::GIOPConn::output (CORBA::Buffer *b)
{
    // post the Buffer to the write thread
    if ( this->state() == MICOMT::StateRefCnt::Active ) {
	
	if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::GIOP)
		<< "MICO::GIOPConn::output (CORBA::Buffer *b)" << endl
		<< "     b: " << b << endl;
	}
	
	if (MICO::MTManager::thread_pool()) {
	  //_writer->send_msg( msg );
	  this->output_handler(b);
	}
	else {
//  	  msg = new msg_type( b );
//  	  _writer->process(msg);
	  this->output_handler(b);
	}
    } else {
        delete b;
    }
}

#endif // HAVE_THREADS

CORBA::Buffer *
MICO::GIOPConn::input ()
{
    CORBA::Buffer *b = _inbufs;
    _inbufs = 0;

    if (MICO::Logger::IsLogged (MICO::Logger::Transport)) {
      MICOMT::AutoDebugLock __lock;
      b->dump ("In Data", MICO::Logger::Stream (MICO::Logger::Transport));
    }

    return b;
}

void
MICO::GIOPConn::flush ()
{
    CORBA::Boolean isblock = _transp->isblocking();
    _transp->block (TRUE);
    while (_outbufs.size() > 0) {
	CORBA::Buffer *b = _outbufs.front();
	_outbufs.pop_front();
	_transp->write (*b, b->length());
	delete b;
    }
    _transp->block (isblock);
}

void
MICO::GIOPConn::buffering (CORBA::Boolean dobuffering)
{
    _transp->buffering (dobuffering);
}

void
MICO::GIOPConn::cancel (CORBA::ULong reqid)
{
    // XXX kill pending fragment if reqid matches

    // for now disallow CancelRequests inbetween fragments
    //assert (!_infrag);
}

#ifdef HAVE_THREADS

/******************************* GIOPConnCallback ***********************/

void
MICO::GIOPConnCallback::send_orb_msg (GIOPConn *conn, ORBMsg::Event ev) {
    MICO::msg_type *msg;

    msg = new msg_type( new ORBMsg(conn, 0, ev) );

    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::GIOP)
	    << "MICO::GIOPConnCallback::send_orb_msg (GIOPConn *conn)" << endl
	    << "  conn: " << conn << endl
	    << "    ev: " << ev << endl;
    }
    
    MICO::MTManager::thread_pool_manager()->put_msg( MICO::Operation::ORB, msg );
}

#endif


/******************************* IIOPProxy ******************************/


MICO::IIOPProxy::IIOPProxy (CORBA::ORB_ptr orb,
                            CORBA::UShort giop_ver,
			    CORBA::ULong max_size)
{
    /*
     * these are the IOR profile types we can handle.
     */
    _valid_profiles.push_back (CORBA::IORProfile::TAG_INTERNET_IOP);
    _valid_profiles.push_back (CORBA::IORProfile::TAG_UNIX_IOP);
    _valid_profiles.push_back (CORBA::IORProfile::TAG_UDP_IOP);
    _valid_profiles.push_back (CORBA::IORProfile::TAG_SSL_INTERNET_IOP);
    _valid_profiles.push_back (CORBA::IORProfile::TAG_SSL_UNIX_IOP);
    _valid_profiles.push_back (CORBA::IORProfile::TAG_SSL_UDP_IOP);

    _orb = orb;

#ifdef USE_IOP_CACHE
    _cache_used = FALSE;
    _cache_rec = new IIOPProxyInvokeRec;
#endif

    _max_message_size = max_size;
    _giop_ver = giop_ver;
    _orb->register_oa (this);
    _reroute = NULL;
}

MICO::IIOPProxy::~IIOPProxy ()
{
    _orb->unregister_oa (this);
    /*
     * the address keys point to GIOPConn::Transport->addr(), so do not
     * delete them.
     * the GIOPConn entries in the 'ids' map just pointers to the entries
     * in the 'conns' map, so do not delete them
     */
    _conns.lock();

    for (MapVerAddrConn::iterator i = _conns.begin(); i != _conns.end(); ++i) {
	for (MapAddrConn::iterator j = (*i).second.begin(); j != (*i).second.end(); ++j) {
#ifdef HAVE_THREADS
	    (*j).second->active_deref();
	    (*j).second->terminate();
	    (*j).second->deref();
	    _orb->resource_manager ().release_connection ();
	    delete (*j).second;
#else // HAVE_THREADS
	    this->deref_conn((*j).second, TRUE);
#endif // HAVE_THREADS
	}
    }
    _conns.unlock();
    _ids.lock();

    for (MapIdConn::iterator i1 = _ids.begin(); i1 != _ids.end(); ++i1)
	delete (*i1).second;

    _ids.unlock();

#ifdef USE_IOP_CACHE
    delete _cache_rec;
#endif
}


void
MICO::IIOPProxy::register_profile_id (CORBA::ULong id)
{
    for (ProfIdVec::iterator i = _valid_profiles.begin();
         i != _valid_profiles.end(); ++i) {
        if (*i == id)
            return;
    }
    _valid_profiles.push_back (id);
}

void
MICO::IIOPProxy::unregister_profile_id (CORBA::ULong id)
{
    for (ProfIdVec::iterator i = _valid_profiles.begin();
         i != _valid_profiles.end(); ++i) {
        if (*i == id) {
            _valid_profiles.erase (i);
            break;
        }
    }
}

MICO::IIOPProxyInvokeRec *
MICO::IIOPProxy::create_invoke ()
{
#ifdef USE_IOP_CACHE
    if (!_cache_used) {
	_cache_used = TRUE;
	return _cache_rec;
    }
#endif

    return new IIOPProxyInvokeRec;
}

MICO::IIOPProxyInvokeRec *
MICO::IIOPProxy::pull_invoke (CORBA::ORBMsgId id)
{
    MICOMT::AutoLock l(_ids);

    if (id == NULL)
	return NULL;

#ifdef USE_IOP_CACHE
    if (_cache_used && _cache_rec->id() == id)
	return _cache_rec;
#endif
    IIOPProxyInvokeRec *rec = (MICO::IIOPProxyInvokeRec *)_orb->get_invoke_hint(id);

    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::GIOP)
	    << "IIOPProxy::pull_invoke: id="<< id << ", "
	    << "rec = " << rec << endl;
    }
    if (rec && rec->active() ) {
	rec->deactivate();
	return rec;
    }
    return 0;
}

void
MICO::IIOPProxy::add_invoke (IIOPProxyInvokeRec *rec)
{
    MICOMT::AutoLock l(_ids);

#ifdef USE_IOP_CACHE
    if (_cache_rec == rec)
	return;
#endif

    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::GIOP)
	    << "IIOPProxy::add_invoke: rec=" << rec << ", id="<< rec->id() << ", msgid=" 
	    << CORBA::ORB::get_msgid(rec->id()) << ")" << endl;
    }

    assert( rec->active() );
    _ids[CORBA::ORB::get_msgid(rec->id())] = rec;
    _orb->set_invoke_hint(rec->id(), rec);
}

void
MICO::IIOPProxy::del_invoke (IIOPProxyInvokeRec *rec)
{

    MICOMT::AutoLock l(_ids);

    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::GIOP)
	    << "IIOPProxy::del_invoke: " 
	    << "rec = " << rec << endl;
    }
    assert(rec != NULL);
    assert( !rec->active() );

#ifdef USE_IOP_CACHE
    if (_cache_used && _cache_rec->id() == id) {
	_cache_rec->free();
	_cache_used = FALSE;
	return;
    }
#endif
    
    MapIdConn::iterator i = _ids.find (rec->msgid());
    if (i != _ids.end()) {
	delete (*i).second;
	_ids.erase (i);
    }
}

void
MICO::IIOPProxy::abort_invoke (CORBA::ORBMsgId id)
{
    // make invocation fail; notify orb ...
    if (MICO::Logger::IsLogged (MICO::Logger::IIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::IIOP)
	    << "GIOP: invocation(" << id << ") aborted" << endl;
    }

    IIOPProxyInvokeRec* inv_rec = pull_invoke(id);

    switch (_orb->request_type (id)) {
    case CORBA::RequestInvoke: {
	CORBA::Object_var obj = new CORBA::Object (new CORBA::IOR);
	CORBA::Request_var req =
	    new CORBA::Request (obj, "someop");
	LocalRequest orbreq (req);
	orbreq.set_out_args (
	    new CORBA::TRANSIENT (0, CORBA::COMPLETED_MAYBE));
	_orb->answer_invoke (id, CORBA::InvokeSysEx,
			     CORBA::Object::_nil(), &orbreq, 0);
	break;
    }
    case CORBA::RequestLocate:
	_orb->answer_locate (id, CORBA::LocateUnknown,
			     CORBA::Object::_nil(), 0);
	break;
	
    case CORBA::RequestBind:
	_orb->answer_bind (id, CORBA::LocateUnknown,
			   CORBA::Object::_nil());
	break;

    default:
	assert (0);
    }

    this->del_invoke(inv_rec);
}

void
MICO::IIOPProxy::redo_invoke (CORBA::ORBMsgId id)
{
    // redo invocation ...
    if (MICO::Logger::IsLogged (MICO::Logger::IIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::IIOP)
	    << "GIOP: invocation(" << id << ") redone" << endl;
    }

    del_invoke ( pull_invoke (id) );
    _orb->redo_request (id);
}

MICO::GIOPConn *
MICO::IIOPProxy::make_conn (const CORBA::Address *addr,
			    CORBA::Boolean docreate,
			    CORBA::UShort version)
{
    MICO::GIOPConn *conn;

    MICOMT::AutoLock l(_conns);
    // kcg: we can receive version == 0 for other than IIOP and SSL transports
    if (version == 0 || version > _giop_ver) {
      version = _giop_ver;
    }

    MapAddrConn::iterator i = _conns[version].find (addr);
    if (i != _conns[version].end()) {
	assert(version == (*i).second->codec()->version());
#if 1
        /*
         * the check_events() is a workaround for a problem
         * on Linux and Win32 (and probably other OSes as well):
         *   if you send something on a TCP connection that has
         *   been closed by the peer you will get an error on
         *   the next read() instead of beeing able to read
         *   the remaining data until you reach EOF.
         */
        if ((*i).second->check_events()) {
            while ((i = _conns[version].find (addr)) != _conns[version].end()) {
                if (!(*i).second->check_events())
                {
                    conn = (*i).second;
#ifdef HAVE_THREADS
		    // The connection was created and I have it in _conns map
		    // so I don't need to increment active ref count
		    // I'll only test if it's active
                    //if( conn->active_ref ())
		    if (conn->state() == MICOMT::StateRefCnt::Active)
                        return conn;
                    else
                        return 0;
#else // HAVE_THREADS
		    return conn;
#endif // HAVE_THREADS
                }
            }
        } else
#endif // 1
	{
            conn = (*i).second;
#ifdef HAVE_THREADS
	    // The connection was created and I have it in _conns map
	    // so I don't need to increment active ref count
	    // I'll only test if it's active
	    //if( conn->active_ref ())
	    if (conn->state() == MICOMT::StateRefCnt::Active)
                return conn;
            else
                return 0;
#else // HAVE_THREADS
	    return conn;
#endif // HAVE_THREADS
        }
    }

    if (!docreate)
	return 0;
#ifdef HAVE_THREADS
    if ( !_orb->resource_manager ().acquire_connection () ) {
	// raise Exception - do what ever nessesary
	if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::GIOP)
		<< "connect: out of connections" << endl;
	}
	return 0;
    }
#endif

    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
      MICOMT::AutoDebugLock __lock;
      MICO::Logger::Stream (MICO::Logger::GIOP)
	<< "IIOP: making new GIOP "
	<< (version>>8) << "." << (version&255)
	<< " connection to " << addr->stringify()
	<< endl;
    }

    CORBA::Transport *t = addr->make_transport();
    if (!t->connect (addr)) {
      if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::GIOP)
	  << "IIOP: connect to " << addr->stringify()
	  << " failed: " << t->errormsg() << endl;
      }
      delete t;
#ifdef HAVE_THREADS
      _orb->resource_manager ().release_connection ();
#endif
      return 0;
    }
#ifdef HAVE_THREADS
    CORBA::Boolean __use_reader_thread = TRUE;
    CORBA::Dispatcher* disp = NULL;
    if (MICO::MTManager::reactive_client()) {
        __use_reader_thread = FALSE;
        disp = _orb->dispatcher();
    }
    else {
        disp = _orb->create_dispatcher();
    }
    CORBA::Boolean __use_writer_thread = FALSE;
    conn =
        new GIOPConn (disp, t, this,
		      new GIOPCodec (new CDRDecoder, new CDREncoder,
                                     version),
		      0L /* no tmout */, _max_message_size,
                      CLIENT_SIDE,
		      __use_reader_thread, __use_writer_thread);
#else
    conn =
	new GIOPConn (Dispatcher(), t, this,
		      new GIOPCodec (new CDRDecoder, new CDREncoder,
                                     version),
		      0L /* no tmout */, _max_message_size);
#endif    
    _conns[version][t->peer()] = conn;
#ifdef HAVE_THREADS
    conn->start();
    if (conn->active_ref ())
        return conn;
    else
        return 0;
#else // HAVE_THREADS
    return conn;
#endif // HAVE_THREADS
}

MICO::GIOPConn *
MICO::IIOPProxy::make_conn (CORBA::Object_ptr obj)
{
    CORBA::IORProfile *prof;
    const CORBA::Address *addr;

    /*
     * See if we have already opened a conn for this profile
     */

    prof = obj->_ior_fwd()->active_profile();

    if (prof) {
      MapProfConn::iterator i;
      GIOPConn* conn=NULL;
      do { // same hack as above
#ifdef HAVE_THREADS
        MICOMT::AutoLock l(_prof_conns);
#endif
	i = _prof_conns.find (prof);
        conn = ( (i!=_prof_conns.end()) ? (*i).second : NULL );
        // TODO: add timeout here
      } while (conn != NULL && conn->check_events());

      /*
       * If yes, then use it
       */

      if (conn != NULL) {
	return conn;
      }

      /*
       * There was an active (connected) profile, but the conn is gone.
       * Invalidate active profile; we might have to reconnect using an
       * alternate address
       */

      obj->_ior_fwd()->active_profile((CORBA::IORProfile *) 0);
    }

    /*
     * _prof_conns could grow indefinitely if the same object is created
     * over and over again to perform operations on it.
     *
     * Because _prof_conns is nothing more than a cache really, we can
     * simply clear it before it grows beyond an arbitrary limit.
     */
    {
#ifdef HAVE_THREADS
     MICOMT::AutoLock l(_prof_conns);
#endif
     if (_prof_conns.size() > 1000) {
       for(MapProfConn::iterator it=_prof_conns.begin(); it!=_prof_conns.end();
           ++it) {
	   // we can safely delete profile here, since we have cloned it
	   // into the map (code below)
          delete (*it).first;
       }
       _prof_conns.clear ();
     }
    }

    /*
     * Open a new connection, or select a profile that matches an open
     * connection.
     */

    CORBA::Policy_var policy =
	obj->_get_policy (MICOPolicy::TRANSPORTPREF_POLICY_TYPE);
    MICOPolicy::TransportPrefPolicy_var tpp =
	MICOPolicy::TransportPrefPolicy::_narrow (policy);
    assert (!CORBA::is_nil (tpp));

    MICOPolicy::TransportPrefPolicy::ProfileTagSeq *prefs =
	tpp->preferences_nocopy();

    for (CORBA::ULong i = 0; i < prefs->length(); ++i) {
        CORBA::UShort version;
        prof = obj->_ior_fwd()->profile ((*prefs)[i]);
        while (prof) {
	    addr = prof->addr ();
	    assert (addr);
	    /*
	     * If the address is not local to this machine and
	     * redirection is set up, change the address
	     */
	    if (_reroute != NULL && !addr->is_here()) {
		addr = _reroute;
	    }

	    version = 0;
            if (prof->id() == CORBA::IORProfile::TAG_INTERNET_IOP
                || prof->id() == CORBA::IORProfile::TAG_SSL_INTERNET_IOP) {
                MICO::ProfileIIOPVersionProvider* version_provider
                    = dynamic_cast<MICO::ProfileIIOPVersionProvider*>(prof);
                //assert(version_provider != NULL);
                if (version_provider != NULL) {
                    version = version_provider->iiop_version();
                }
                // Terminal Bridge needs IOR which means IIOP 1.2+
                if (!_orb->plugged() && version < 0x0102)
                    version = 0x0102;
            }

	    GIOPConn *conn = make_conn (addr, 1, version);
	    if (conn) {
	      obj->_ior_fwd()->active_profile (prof);
#ifdef HAVE_THREADS
              MICOMT::AutoLock l(_prof_conns);
#endif
              if (_prof_conns.count(prof) == 0) {
                  // we need to add prof/conn pair to the map
                  _prof_conns[prof->clone()] = conn;
              }
	      return conn;
	    }
            prof = obj->_ior_fwd()->profile ((*prefs)[i], FALSE, prof);
	}
    }
    return 0;
}

#ifdef HAVE_THREADS
void
MICO::IIOPProxy::deref_conn (GIOPConn *conn, CORBA::Boolean all )
{
    if ( conn->deref() ) {
	this->send_orb_msg( conn, MICO::ORBMsg::KillConn );
	_orb->resource_manager ().release_connection ();
    }
}

#else

void
MICO::IIOPProxy::deref_conn (GIOPConn *conn, CORBA::Boolean all )
{
    conn->deref(all);
    if (all)
	delete conn;
}

#endif

void
MICO::IIOPProxy::kill_conn (GIOPConn *conn, CORBA::Boolean redo)
{
    CORBA::Boolean again;
    CORBA::Boolean found = FALSE;

    //    _orb->unregister_oa (this);

    // remove the conn from the _conns Map

    _conns.lock();
    CORBA::UShort conn_version = conn->codec()->version();
    do {
	again = FALSE;
	for (MapAddrConn::iterator i = _conns[conn_version].begin(); i != _conns[conn_version].end(); ++i) {
	    if ((*i).second == conn) {
		_conns[conn_version].erase (i);
		again = TRUE;
		found = TRUE;
		break;
	    }
	}
    } while (again);

    _conns.unlock();

    if (!found)
	return;

    do {
	again = FALSE;
#ifdef HAVE_THREADS
        MICOMT::AutoLock l(_prof_conns);
#endif
	for (MapProfConn::iterator i = _prof_conns.begin();
	     i != _prof_conns.end(); ++i) {
	    if ((*i).second == conn) {
		// we can safely delete profile here, since we have cloned it
		// into the map
                delete (*i).first;
		_prof_conns.erase (i);
		again = TRUE;
		break;
	    }
	}
    } while (again);
#ifdef HAVE_THREADS
    conn->active_deref();
    conn->terminate();
#endif
    // abort/redo pending requests for this connection
#ifdef USE_IOP_CACHE
    if (_cache_used && _cache_rec->conn() == conn) {
	_cache_rec->active() = FALSE;
	if (redo) {
	    redo_invoke (_cache_rec->id());
	} else {
	    abort_invoke (_cache_rec->id());
	    // del_invoke (_cache_rec);
	}
    }
#endif

    do {
	again = FALSE;
	IIOPProxyInvokeRec *rec;

	_ids.lock();

	for (MapIdConn::iterator i = _ids.begin(); i != _ids.end(); ++i) {
	    rec = (*i).second;
	    if (rec->conn() != conn)
		continue;
	    if (!rec->active() )
		continue;
	    // cannot deactivate rec because it'll crash on
	    // abort_invoke(rec->id()) later because
	    // abort_invoke calls del_invoke(pull_invoke())
	    //rec->deactivate();

	    _ids.unlock();

	    if (redo) {
		//FIXME: if we issue a redo, the write methode
		// will hit a delete conn object !!!
		// kcg: I've commented out assert below for experiments
		// ie. it seems that this bug has been fixed already
		//assert( 0 );
		redo_invoke (rec->id());
	    } else {
		abort_invoke (rec->id());
		// del_invoke (rec);
	    }
	    again = TRUE;
	    break;
	}

	if (!again) _ids.unlock();

    } while (again);

    deref_conn( conn, TRUE );
}

void
MICO::IIOPProxy::conn_error (GIOPConn *conn, CORBA::Boolean send_error)
{
    if (!send_error) {
	kill_conn (conn);
	return;
    }

    GIOPOutContext out (conn->codec());
    conn->codec()->put_error_msg (out);
    conn->output (out._retn());
    // prepare shutdown (i.e. wait until MessageError has been sent)
    deref_conn ( conn, TRUE );
}

const char *
MICO::IIOPProxy::get_oaid () const
{
    return "mico-iiop-proxy";
}

CORBA::Boolean
MICO::IIOPProxy::has_object (CORBA::Object_ptr obj)
{
    CORBA::IOR *tmpl = _orb->ior_template();
    /*
     * we have every object whose IOR has a profile tag that is listed
     * in valid_profiles and the profile doesnt point to this process
     * (to prevent infinite loops when trying to talk to nonexisting
     * objects in existent servers).
     */
    for (mico_vec_size_type i = 0; i < _valid_profiles.size(); ++i) {
        CORBA::IORProfile *p1, *p2;
        p1 = obj->_ior_fwd()->profile (_valid_profiles[i]);
        if (p1) {
            p2 = tmpl->profile (_valid_profiles[i]);
            return !p2 || !(*p1 == *p2);
        }
    }
    return FALSE;
}

CORBA::Boolean
MICO::IIOPProxy::is_local () const
{
    return FALSE;
}

#ifdef USE_CSL2
CORBA::Principal_ptr
MICO::IIOPProxy::get_principal (CORBA::Object_ptr obj){
    GIOPConn *conn = make_conn (obj);
    // WARNING: This assertion is raised if a cipher is not supported!!!
    assert(conn);
    CORBA::Principal_ptr server_pr = conn->transport()->get_principal();
    assert(server_pr);
    return server_pr;
}
#endif /* USE_CSL2  */


CORBA::Boolean
MICO::IIOPProxy::invoke (CORBA::ORBMsgId id, CORBA::Object_ptr obj,
			 CORBA::ORBRequest *req,
			 CORBA::Principal_ptr pr, CORBA::Boolean response_exp)
{
    GIOPConn *conn = make_conn (obj);
    if (!conn) {
        CORBA::COMM_FAILURE ex;
	req->set_out_args (&ex);
	_orb->answer_invoke (id, CORBA::InvokeSysEx, CORBA::Object::_nil(),
			     req, 0);
	return FALSE;
    }
#ifdef HAVE_THREADS
    assert(conn->active_ref());
#endif
    if (!conn->codec()->converter()) {
#ifdef HAVE_THREADS
      conn->get_exclusive();
      if (!conn->codec()->converter()) {
#endif
	if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	  MICOMT::AutoDebugLock __lock;
	  MICO::Logger::Stream (MICO::Logger::GIOP)
	    << "GIOP: Codeset negotiation with "
	    << conn->transport()->peer()->stringify()
	    << " using GIOP version "
	    << (conn->codec()->version() >> 8) << "."
	    << (conn->codec()->version() & 255) << endl;
	}

	if (!conn->codec()->setup_codeset_ids (obj)) {
	    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
		MICOMT::AutoDebugLock __lock;
		MICO::Logger::Stream (MICO::Logger::GIOP)
		    << "GIOP: Codeset negotiation with "
		    << conn->transport()->peer()->stringify()
		    << " has failed" << endl;
	    }

	    CORBA::DATA_CONVERSION ex;
	    req->set_out_args (&ex);
	    _orb->answer_invoke (id, CORBA::InvokeSysEx,
				 CORBA::Object::_nil(),
				 req, 0);
	    return FALSE;
	}
#ifdef HAVE_THREADS
      }
      conn->release_exclusive();
#endif
    }

    // I think that we can allocate for oneway call yet another msgid
    // so in case of using oneway calls the msgids are scaled after two not after
    // one because we've already allocated it in ORB::invoke_async and now here
    CORBA::ORB::MsgId __id = CORBA::ORB::get_msgid(id);
    if (__id == 0)
	__id = obj->_orbnc()->new_msgid ();

    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
      MICOMT::AutoDebugLock __lock;
      MICO::Logger::Stream (MICO::Logger::GIOP)
	<< "GIOP: sending Request to "
	<< conn->transport()->peer()->stringify() 
	<< " msgid is " << __id << endl;
    }

    GIOPOutContext out (conn->codec(), conn->codec()->converter());
    if (!conn->codec()->put_invoke_request (out, __id, response_exp,
					    obj, req, pr)) {
        CORBA::MARSHAL ex;
	req->set_out_args (&ex);
	_orb->answer_invoke (id, CORBA::InvokeSysEx, CORBA::Object::_nil(),
			     req, 0);
#ifdef HAVE_THREADS
        conn->active_deref();
#endif // HAVE_THREADS
	return FALSE;
    }

    if (response_exp) {
	conn->ref();
	IIOPProxyInvokeRec *rec = create_invoke();
	rec->init (id, conn, req);
	add_invoke (rec);
    }
    conn->buffering (!response_exp);
    conn->output (out._retn());
#ifndef HAVE_THREADS
    if (response_exp && Dispatcher()->isblocking() )
	conn->do_read ( Dispatcher()->isblocking() );
#endif // HAVE_THREADS
#ifdef HAVE_THREADS
    conn->active_deref();
#endif // HAVE_THREADS
    return TRUE;
}

CORBA::Boolean
MICO::IIOPProxy::bind (CORBA::ORBMsgId id, const char *repoid,
		       const CORBA::ORB::ObjectTag &oid,
		       CORBA::Address *addr)
{
    if (!addr || addr->is_local())
	return FALSE;

    GIOPConn *conn = make_conn (addr);
    if (!conn) {
        _orb->answer_bind (id, CORBA::LocateUnknown, CORBA::Object::_nil());
        return TRUE;
    }
#ifdef HAVE_THREADS
    assert(conn->active_ref());
#endif
    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
      MICOMT::AutoDebugLock __lock;
      MICO::Logger::Stream (MICO::Logger::GIOP)
	<< "GIOP: sending _bind Request to "
	<< conn->transport()->peer()->stringify() 
	<< " msgid is " << CORBA::ORB::get_msgid(id) << endl;
    }

    GIOPOutContext out (conn->codec());
    conn->codec()->put_bind_request (out, CORBA::ORB::get_msgid(id), repoid, oid);

    conn->ref();
    //assert (_ids.count (msgid) == 0);
    IIOPProxyInvokeRec *rec = create_invoke();
    rec->init (id, conn);
    add_invoke (rec);

    conn->output (out._retn());
#ifdef HAVE_THREADS
    conn->active_deref ();
#endif // HAVE_THREADS
    return TRUE;
}

CORBA::Boolean
MICO::IIOPProxy::locate (CORBA::ORBMsgId id, CORBA::Object_ptr obj)
{
    GIOPConn *conn = make_conn (obj);
    if (!conn) {
	_orb->answer_locate (id, CORBA::LocateUnknown,
			     CORBA::Object::_nil(), 0);
	return FALSE;
    }

    if (!conn->codec()->converter()) {
#ifdef HAVE_THREADS
      conn->get_exclusive();
      if (!conn->codec()->converter()) {
#endif
	if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	  MICOMT::AutoDebugLock __lock;
	  MICO::Logger::Stream (MICO::Logger::GIOP)
	    << "GIOP: Codeset negotiation with "
	    << conn->transport()->peer()->stringify()
	    << " using GIOP version "
	    << (conn->codec()->version() >> 8) << "."
	    << (conn->codec()->version() & 255) << endl;
	}

	if (!conn->codec()->setup_codeset_ids (obj)) {
	    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
		MICOMT::AutoDebugLock __lock;
		MICO::Logger::Stream (MICO::Logger::GIOP)
		    << "GIOP: Codeset negotiation with "
		    << conn->transport()->peer()->stringify()
		    << " has failed" << endl;
	    }

	    CORBA::DATA_CONVERSION ex;
	    _orb->answer_locate (id, CORBA::LocateUnknown,
				 CORBA::Object::_nil(), 0);
	    return FALSE;
	}
#ifdef HAVE_THREADS
      }
      conn->release_exclusive();
#endif
    }

    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
      MICOMT::AutoDebugLock __lock;
      MICO::Logger::Stream (MICO::Logger::GIOP)
	<< "GIOP: sending LocateRequest to "
	<< conn->transport()->peer()->stringify() 
	<< " msgid is " << CORBA::ORB::get_msgid(id) << endl;
    }

    GIOPOutContext out (conn->codec());
    conn->codec()->put_locate_request (out, CORBA::ORB::get_msgid(id), obj);

    conn->ref();
    //assert (_ids.count (msgid) == 0);
    IIOPProxyInvokeRec *rec = create_invoke();
    rec->init (id, conn);
    add_invoke (rec);

    conn->output (out._retn());
#ifdef HAVE_THREADS
    conn->active_deref ();
#endif // HAVE_THREADS
    return TRUE;
}

CORBA::Object_ptr
MICO::IIOPProxy::skeleton (CORBA::Object_ptr)
{
    assert (0);
    return CORBA::Object::_nil ();
}

void
MICO::IIOPProxy::cancel (CORBA::ORBMsgId id)
{
    IIOPProxyInvokeRec *rec = pull_invoke (id);
    if (!rec)
	return;

    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
      MICOMT::AutoDebugLock __lock;
      MICO::Logger::Stream (MICO::Logger::GIOP)
	<< "GIOP: sending Cancel to "
	<< rec->conn()->transport()->peer()->stringify() 
	<< " msgid is " << CORBA::ORB::get_msgid(id) << endl;
    }

    GIOPOutContext out (rec->conn()->codec());
    rec->conn()->codec()->put_cancel_request (out, CORBA::ORB::get_msgid(id));

    rec->conn()->output (out._retn());
    deref_conn( rec->conn() );
    del_invoke (rec);
}

void
MICO::IIOPProxy::shutdown (CORBA::Boolean wait_for_completion)
{
    // XXX make sure all invocations have completed
    _orb->answer_shutdown (this);
}

void
MICO::IIOPProxy::answer_invoke (CORBA::ORBMsgId, CORBA::Object_ptr,
				CORBA::ORBRequest *, CORBA::InvokeStatus)
{
    assert (0);
}

void
MICO::IIOPProxy::exec_invoke_reply (GIOPInContext &in, CORBA::ORBMsgId id,
				    GIOP::ReplyStatusType stat,
				    CORBA::Object_ptr obj,
				    CORBA::ORBRequest *req,
				    GIOP::AddressingDisposition ad,
				    GIOPConn *conn)
{
    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::GIOP)
	    << "MICO::IIOPProxy::exec_invoke_reply ("
	    << "obj=" << &(*obj)
	    << ", *req=" << req
	    << ", *conn=" << conn
	    << ")" << endl;
    }
    switch (_orb->request_type (id)) {
    case CORBA::RequestBind: {
	// it was a bind
	GIOP::LocateStatusType bind_stat;
	CORBA::Object_ptr bind_obj;

	if (stat != GIOP::NO_EXCEPTION ||
	    !conn->codec()->get_bind_reply (in, bind_stat, bind_obj) ||
	    bind_stat != GIOP::OBJECT_HERE) {
	    _orb->answer_bind (id, CORBA::LocateUnknown,
			       CORBA::Object::_nil());
	} else {
	    _orb->answer_bind (id, CORBA::LocateHere, bind_obj);
	    CORBA::release (bind_obj);
	}
	break;
    }
    case CORBA::RequestInvoke: {
	// it was a normal invocation
	CORBA::InvokeStatus orb_stat = CORBA::InvokeOk;

	switch (stat) {
	case GIOP::NO_EXCEPTION:
	    orb_stat = CORBA::InvokeOk;
	    break;

	case GIOP::USER_EXCEPTION:
	    orb_stat = CORBA::InvokeUsrEx;
	    break;

	case GIOP::SYSTEM_EXCEPTION:
	    orb_stat = CORBA::InvokeSysEx;
	    break;

	case GIOP::LOCATION_FORWARD:
	case GIOP::LOCATION_FORWARD_PERM:
	    orb_stat = CORBA::InvokeForward;
	    break;

	case GIOP::NEEDS_ADDRESSING_MODE:
	    orb_stat = CORBA::InvokeAddrDisp;
	    break;

	default:
	    assert (0);
	    break;
	}
	_orb->answer_invoke (id, orb_stat, obj, req, ad);
	break;
    }
    case CORBA::RequestUnknown:
	// request was canceled or reply contains invalid id
	break;
    default:
	assert (0);
    }
}

CORBA::Boolean
MICO::IIOPProxy::handle_invoke_reply (GIOPConn *conn, GIOPInContext &in)
{
    CORBA::ULong req_id;
    GIOP::ReplyStatusType stat;
    CORBA::Object_ptr obj = CORBA::Object::_nil();
    GIOP::AddressingDisposition ad = GIOP::KeyAddr;
    IOP::ServiceContextList ctx;

    // XXX take care, get_invoke_reply() does a in._retn()
    if (!conn->codec()->get_invoke_reply1 (in, req_id, stat, ctx)) {
	if (MICO::Logger::IsLogged (MICO::Logger::IIOP)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::IIOP)
		<< "GIOP: cannot decode incoming Reply from "
		<< conn->transport()->peer()->stringify() << endl;
	}
	conn_error (conn);
	return FALSE;
    }

    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
      MICOMT::AutoDebugLock __lock;
      MICO::Logger::Stream (MICO::Logger::GIOP)
	<< "GIOP: incoming Reply from "
	<< conn->transport()->peer()->stringify()
	<< " for msgid " << req_id
	<< " status is " << (CORBA::ULong) stat
	<< endl;
    }

    // turn the MsgId into somthing usefull
    CORBA::ORBMsgId id = _orb->get_orbid(req_id);
    IIOPProxyInvokeRec *rec = pull_invoke (id);
    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::GIOP)
	    << "IIOPProxy::handle_invoke_reply: rec="<< rec << ")" << endl;
    }
    if (!rec) {
	// has been canceled; cancel() did the deref() already
#ifdef HAVE_THREADS
	conn->active_deref();
#endif // HAVE_THREADS
	return TRUE;
    }

    in.converters (conn->codec()->converter());

    if (!conn->codec()->get_invoke_reply2 (in, req_id, stat, obj,
					   rec->request(), ad, ctx)) {
	if (MICO::Logger::IsLogged (MICO::Logger::IIOP)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::IIOP)
		<< "GIOP: reply marshalling error for msgid " << req_id << endl;
	}
	if (rec->request()) {
	    CORBA::MARSHAL ex (0, CORBA::COMPLETED_MAYBE);
	    rec->request()->set_out_args (&ex);
	    stat = GIOP::SYSTEM_EXCEPTION;
	}
    }

    // we have to delete IIOPProxyInvokeRec before notifying waiting
    // thread with reply, because client code might do another
    // invocation where old ORBInvokeRec is deleted and new one
    // created on the same address (might be). Then after switching
    // back to this code del_invoke would delete invocation hint on
    // this new ORBInvokeRec which belong to another invocation and so
    // after receiving reply for this invocation - it'll be w/o hint
    // and so will look like canceled invocation. This wrong behaviour
    // is fixed with moving del_invoke before exec_invoke_reply
    // NOTE: Don't delete req!
    CORBA::ORBRequest_ptr req = rec->request();
    del_invoke(rec);
    exec_invoke_reply (in, id, stat, obj, req, ad, conn);
    CORBA::release (obj);
    // calling active_deref after deref_conn is quite
    // ugly, but needed because of ~IIOPProxy where
    // we call terminate and a bit after it we delete
    // connection, which would otherwise assert since
    // _ref might not be 0 (in case we call active_deref
    // and deref_conn after it)
    deref_conn( conn );
#ifdef HAVE_THREADS
    conn->active_deref();
#endif // HAVE_THREADS

    return TRUE;
}

CORBA::Boolean
MICO::IIOPProxy::handle_locate_reply (GIOPConn *conn, GIOPInContext &in)
{
    CORBA::ULong req_id;
    GIOP::LocateStatusType stat = GIOP::OBJECT_HERE;
    CORBA::LocateStatus orb_stat = CORBA::LocateHere;
    CORBA::Object_ptr obj = CORBA::Object::_nil();
    GIOP::AddressingDisposition ad = GIOP::KeyAddr;

    if (!conn->codec()->get_locate_reply (in, req_id, stat, obj, ad)) {
      if (MICO::Logger::IsLogged (MICO::Logger::IIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::IIOP)
	  << "GIOP: cannot decode LocateReply" << endl;
      }
	conn_error (conn);
	return FALSE;
    }

    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
      MICOMT::AutoDebugLock __lock;
      MICO::Logger::Stream (MICO::Logger::GIOP)
	<< "GIOP: incoming LocateReply from "
	<< conn->transport()->peer()->stringify()
	<< " for msgid " << req_id
	<< " status is " << (CORBA::ULong) stat
	<< endl;
    }

    CORBA::ORBMsgId id = _orb->get_orbid(req_id);
    IIOPProxyInvokeRec *rec = pull_invoke (id);
#ifdef HAVE_THREADS
    conn->active_deref();
#endif // HAVE_THREADS
    if (!rec) {
	// has been canceled; cancel() did the deref() already
	return TRUE;
    }

    switch (stat) {
    case GIOP::UNKNOWN_OBJECT:
	orb_stat = CORBA::LocateUnknown;
	break;

    case GIOP::OBJECT_HERE:
	orb_stat = CORBA::LocateHere;
	break;

    case GIOP::OBJECT_FORWARD:
    case GIOP::OBJECT_FORWARD_PERM:
	orb_stat = CORBA::LocateForward;
	break;

    case GIOP::LOC_NEEDS_ADDRESSING_MODE:
        orb_stat = CORBA::LocateAddrDisp;
       break;

    default:
	assert (0);
	break;
    }

    del_invoke (rec);

    _orb->answer_locate (id, orb_stat, obj, ad);
    CORBA::release (obj);

    deref_conn( conn );

    return TRUE;
}

CORBA::Boolean
MICO::IIOPProxy::handle_input (GIOPConn *conn, CORBA::Buffer *inp)
{
    if (MICO::Logger::IsLogged (MICO::Logger::IIOP)) {
      MICOMT::AutoDebugLock __lock;
      MICO::Logger::Stream (MICO::Logger::IIOP)
	<< "IIOP: incoming data from "
	<< conn->transport()->peer()->stringify() << endl;
    }

    GIOPInContext in (conn->codec(), inp);

    GIOP::MsgType mt;
    CORBA::ULong size;
    CORBA::Octet flags;

    // XXX size is wrong for fragmented messages ...
    if (!conn->codec()->get_header (in, mt, size, flags)) {
	if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::GIOP)
		<< "GIOP: cannot decode incoming header from "
		<< conn->transport()->peer()->stringify() << endl;
	}
#ifdef HAVE_THREADS
	conn->active_deref();
#endif // HAVE_THREADS
	conn_error (conn);
	return FALSE;
    }

    switch (mt) {
    case GIOP::Reply:
      return handle_invoke_reply (conn, in);

    case GIOP::LocateReply:
      return handle_locate_reply (conn, in);

    case GIOP::CloseConnection:
	if (!conn->codec()->get_close_msg (in)) {
	    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
		MICOMT::AutoDebugLock __lock;
		MICO::Logger::Stream (MICO::Logger::GIOP)
		    << "GIOP: cannot decode CloseConnection from "
		    << conn->transport()->peer()->stringify() << endl;
	    }
#ifdef HAVE_THREADS
	    conn->active_deref();
#endif // HAVE_THREADS
	    conn_error (conn);
	} else {
	    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
		MICOMT::AutoDebugLock __lock;
		MICO::Logger::Stream (MICO::Logger::GIOP)
		    << "GIOP: incoming CloseConnection from "
		    << conn->transport()->peer()->stringify() << endl;
	    }
#ifdef HAVE_THREADS
	    conn->active_deref();
#endif
	    kill_conn (conn, TRUE);
	}
	return FALSE;

    case GIOP::MessageError:
	if (!conn->codec()->get_error_msg (in)) {
	    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
		MICOMT::AutoDebugLock __lock;
		MICO::Logger::Stream (MICO::Logger::GIOP)
		    << "GIOP: cannot decode MessageError from "
		    << conn->transport()->peer()->stringify() << endl;
	    }
#ifdef HAVE_THREADS
	    conn->active_deref();
#endif // HAVE_THREADS
	    conn_error (conn, FALSE);
	    break;
	}
	if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	  MICOMT::AutoDebugLock __lock;
	  MICO::Logger::Stream (MICO::Logger::GIOP)
	    << "GIOP: incoming MessageError from "
	    << conn->transport()->peer()->stringify() << endl;
	}
#ifdef HAVE_THREADS
	conn->active_deref();
#endif // HAVE_THREADS
	kill_conn (conn);
	return FALSE;

    default:
	if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	  MICOMT::AutoDebugLock __lock;
	  MICO::Logger::Stream (MICO::Logger::GIOP)
	    << "GIOP: bad incoming message type (" << mt << ") from "
	    << conn->transport()->peer()->stringify() << endl;
	}
#ifdef HAVE_THREADS
	conn->active_deref();
#endif // HAVE_THREADS
	conn_error (conn, FALSE);
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
MICO::IIOPProxy::input_callback (GIOPConn *conn, CORBA::Buffer *inp)
{
    return handle_input( conn, inp );
}

CORBA::Boolean
MICO::IIOPProxy::callback (GIOPConn *conn, GIOPConnCallback::Event ev)
{
    switch (ev) {
    case GIOPConnCallback::InputReady:
	return input_callback (conn, conn->input());

    case GIOPConnCallback::Idle:
	if (MICO::Logger::IsLogged (MICO::Logger::IIOP)) {
	  MICOMT::AutoDebugLock __lock;
	  MICO::Logger::Stream (MICO::Logger::IIOP)
	    << "IIOP: shutting down idle conn to "
	    << conn->transport()->peer()->stringify() << endl;
	}
	kill_conn (conn);
	return FALSE;

    case GIOPConnCallback::Closed:
	if (MICO::Logger::IsLogged (MICO::Logger::IIOP)) {
	  MICOMT::AutoDebugLock __lock;
	  MICO::Logger::Stream (MICO::Logger::IIOP)
	    << "IIOP: connection to "
	    << conn->transport()->peer()->stringify() 
	    << " closed or broken" << endl;
	}
	kill_conn (conn);
	return FALSE;

    default:
	assert (0);
    }
    return TRUE;
}


CORBA::Dispatcher*
MICO::IIOPProxy::Dispatcher() {
#ifndef HAVE_THREADS
    return _orb->dispatcher();
#else
    return MICO::GIOPConnMgr::Dispatcher();
#endif
}

/************************* IIOPServerInvokeRec ************************/


MICO::IIOPServerInvokeRec::IIOPServerInvokeRec ()
{
    _conn = 0;
    _req = 0;
    _obj = 0;
    _pr = 0;
    _orbid = 0;
}


MICO::IIOPServerInvokeRec::~IIOPServerInvokeRec ()
{
    CORBA::release (_req);
    CORBA::release (_obj);
    CORBA::release (_pr);

    // XXX do not free connection
}

void
MICO::IIOPServerInvokeRec::free ()
{
    CORBA::release (_req);
    CORBA::release (_obj);
    CORBA::release (_pr);

    _conn = 0;
    _req = 0;
    _obj = 0;
    _pr = 0;
    _orbid = 0;
    _active = TRUE;
}

void
MICO::IIOPServerInvokeRec::init_invoke (
    GIOPConn *conn, MsgId reqid,
    CORBA::ORBMsgId orbid, CORBA::ORBRequest *req,
    CORBA::Object_ptr obj,
    CORBA::Principal_ptr pr)
{
    _conn = conn;
    _orbid = orbid;
    _orbmsgid = CORBA::ORB::get_msgid(orbid);
    _reqid = reqid;
    _req = req;
    _obj = obj;
    _pr = pr;
    _active = TRUE;
}

void
MICO::IIOPServerInvokeRec::init_locate (
    GIOPConn *conn, MsgId reqid,
    CORBA::ORBMsgId orbid, CORBA::Object_ptr obj)
{
    _conn = conn;
    _orbid = orbid;
    _orbmsgid = CORBA::ORB::get_msgid(orbid);
    _reqid = reqid;
    _req = CORBA::ORBRequest::_nil();
    _obj = obj;
    _pr = CORBA::Principal::_nil();
    _active = TRUE;
}


/******************************* IIOPServer *****************************/

MICO::IIOPServer::IIOPServer (CORBA::ORB_ptr orb, CORBA::UShort iiop_ver,
			      CORBA::ULong max_size)
    : _orbids_mutex(FALSE, MICOMT::Mutex::Recursive)
{
    _orb = orb;

#ifdef USE_IOP_CACHE
    _cache_used = FALSE;
    _cache_rec = new IIOPServerInvokeRec;
#endif

    _iiop_ver = iiop_ver;
    _max_message_size = max_size;

    // we only register as an OA to be notified of shutdown...
    _orb->register_oa (this);
}

MICO::IIOPServer::~IIOPServer ()
{
    unsigned int cnt;

    _orb->unregister_oa (this);
    /*
     * the GIOPConn entries in the 'orbids' and 'reqids' maps are just
     * pointers to the entries in the 'conns' list, so do not delete them
     */

    _conns.lock();

    for (ListConn::iterator i0 = _conns.begin(); i0 != _conns.end(); ++i0)
	delete *i0;

    _conns.unlock();


#ifdef USE_IOP_CACHE
    if (_cache_used)
	_orb->cancel (_cache_rec->orbid());
#endif
    {
	MICOMT::AutoLock l(_orbids_mutex);

	for (MapIdConn::iterator i1 = _orbids.begin();
	     i1 != _orbids.end(); ++i1) {
	    IIOPServerInvokeRec *rec = (*i1).second;
	    _orb->cancel ( rec->orbid() );
	    delete rec;
	}
    }

    for (cnt = _tservers.count(); cnt > 0; cnt--) {
	_tservers[_tservers.first()]->aselect (Dispatcher(), 0);
	delete _tservers[_tservers.first()];
	_tservers.remove(_tservers.first());
    }
}

CORBA::Boolean
MICO::IIOPServer::listen (CORBA::Address *addr, CORBA::Address *fwproxyaddr)
{
    CORBA::IORProfile *prof;
    CORBA::TransportServer *tserv = addr->make_transport_server ();
#ifdef HAVE_THREADS
    if (!MICO::MTManager::thread_pool())
	tserv->create_thread();
#endif
    if (!tserv->bind (addr)) {
      if (MICO::Logger::IsLogged (MICO::Logger::IIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::IIOP)
	  << "IIOP: cannot bind to " << addr->stringify() << ": "
	  << tserv->errormsg() << endl;
      }
      return FALSE;
    }
    tserv->block ( Dispatcher()->isblocking() );
    tserv->aselect ( Dispatcher(), this);

    if (!fwproxyaddr) {
        prof = tserv->addr()->make_ior_profile ((CORBA::Octet *)"", 1,
						CORBA::MultiComponent(),
                                                _iiop_ver);
    } else {
        prof = fwproxyaddr->make_ior_profile ((CORBA::Octet *)"", 1,
                                              CORBA::MultiComponent(),
                                              _iiop_ver);
    }


    if (MICO::Logger::IsLogged (MICO::Logger::IIOP)) {
      MICOMT::AutoDebugLock __lock;
      MICO::Logger::Stream (MICO::Logger::IIOP)
	<< "IIOP: server listening on "
	<< tserv->addr()->stringify()
	<< " IIOP version "
	<< (_iiop_ver >> 8) << "." << (_iiop_ver & 255)
	<< endl;
    }

    /*
     * install an IIOP profile tag in the ORB's object template.
     * object adapters will use this template to generate new object
     * references...
     */

    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::GIOP)
	    << "binding to " << prof->addr()->stringify() << endl;
    }
    _orb->ior_template()->add_profile (prof);

    _tservers.insert (tserv);
#ifdef HAVE_THREADS
    if (!MICO::MTManager::thread_pool())
	tserv->start();
#endif // HAVE_THREADS
    return TRUE;
}

CORBA::Boolean
MICO::IIOPServer::listen ()
{
    InetAddress addr ("0.0.0.0", 0);
    return listen (&addr, 0);
}

MICO::IIOPServerInvokeRec *
MICO::IIOPServer::create_invoke ()
{
#ifdef USE_IOP_CACHE
    if (!_cache_used) {
	_cache_used = TRUE;
	return _cache_rec;
    }
#endif

    return new IIOPServerInvokeRec;
}

MICO::IIOPServerInvokeRec *
MICO::IIOPServer::pull_invoke_reqid (MsgId msgid, GIOPConn *conn)
{
    MICOMT::AutoLock l(_orbids_mutex);

#ifdef USE_IOP_CACHE
    if (_cache_used && _cache_rec->reqid() == msgid &&
	_cache_rec->conn() == conn) {
	_cache_rec->deactivate();
	return _cache_rec;
    }
#endif
    // XXX slow, but only needed for cancel

    IIOPServerInvokeRec *rec;
    for (MapIdConn::iterator i = _orbids.begin(); i != _orbids.end(); ++i) {
	rec = (*i).second;
	if (rec->reqid() == msgid && rec->conn() == conn && rec->active() )
	    rec->deactivate();
	    return rec;
    }
    return 0;
}

MICO::IIOPServerInvokeRec *
MICO::IIOPServer::pull_invoke_orbid (CORBA::ORBMsgId id)
{
#ifdef USE_IOP_CACHE
    if (_cache_used && _cache_rec->orbid() == msgid) {
	_cache_rec->deactivate();
	return _cache_rec;
    }
#endif

    MICOMT::AutoLock l(_orbids_mutex);

    MICO::IIOPServerInvokeRec *rec;

    rec = (MICO::IIOPServerInvokeRec *)_orb->get_request_hint( id );
    if (rec && rec->active() ) {
	rec->deactivate();
	return rec;
    }
    del_invoke_orbid(rec);
    return 0;
}

void
MICO::IIOPServer::add_invoke (IIOPServerInvokeRec *rec)
{
    MICOMT::AutoLock l(_orbids_mutex);

#ifdef USE_IOP_CACHE
    if (_cache_rec == rec)
	return;
#endif

    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::GIOP)
	    << "IIOPServer::add_invoke (id="<< rec->orbmsgid() << ")" << endl;
    }
    //assert (_orbids.count (rec->orbid()) == 0);
    _orbids[ rec->orbmsgid() ] = rec;
    _orb->set_request_hint( rec->orbid(), rec );
}

void
MICO::IIOPServer::del_invoke_orbid (IIOPServerInvokeRec *rec)
{
    MICOMT::AutoLock l(_orbids_mutex);

    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::GIOP)
	    << "IIOPServer::del_invoke (id="<< rec->orbmsgid() << ")" << endl;
    }
    assert( !rec->active() );

#ifdef USE_IOP_CACHE
    if (_cache_used && _cache_rec->orbmsgid() == msgid) {
	assert( !_cache_rec->active() );
	_cache_rec->free();
	_cache_used = FALSE;
	return;
    }
#endif

    deref_conn( rec->conn() );
    MapIdConn::iterator i = _orbids.find (rec->orbmsgid());
    if (i != _orbids.end()) {
	delete (*i).second;
	_orbids.erase (i);
    }
}

void
MICO::IIOPServer::del_invoke_reqid (MsgId msgid, GIOPConn *conn)
{
    MICOMT::AutoLock l(_orbids_mutex);

#ifdef USE_IOP_CACHE
    if (_cache_used && _cache_rec->reqid() == msgid &&
	_cache_rec->conn() == conn) {
	assert( !_cache_rec->active() );
	_cache_rec->free();
	_cache_used = FALSE;
	return;
    }
#endif

    // XXX slow, but rarely used
    IIOPServerInvokeRec *rec;

    deref_conn( conn );
    for (MapIdConn::iterator i = _orbids.begin(); i != _orbids.end(); ++i) {
	rec = (*i).second;
	if (rec->reqid() == msgid && rec->conn() == conn) {
	    assert( !rec->active() );
	    delete rec;
	    _orbids.erase (i);
	    break;
	}
    }
}

void
MICO::IIOPServer::abort_invoke_orbid (IIOPServerInvokeRec *rec)
{
    _orb->cancel ( rec->orbmsgid() );
    // del_invoke_orbid ( rec );
}

#ifdef HAVE_THREADS

void
MICO::IIOPServer::deref_conn (GIOPConn *conn, CORBA::Boolean all )
{
    if ( conn->deref() ) {
	this->send_orb_msg( conn, MICO::ORBMsg::KillConn );
	_orb->resource_manager ().release_connection ();
    }
}

#else

void
MICO::IIOPServer::deref_conn (GIOPConn *conn, CORBA::Boolean all )
{
    conn->deref(all);
    if (all)
	delete conn;
}

#endif

void
MICO::IIOPServer::kill_conn (GIOPConn *conn, CORBA::Boolean redo)
{
#ifdef HAVE_THREADS
    if (conn->state() != MICOMT::StateRefCnt::Active
        && conn->state() != MICOMT::StateRefCnt::InitShutdown) {
        // kcg: calling kill_conn on terminated connection => return
        return;
    }
#endif // HAVE_THREADS
    CORBA::Boolean again;
    // this and connection establischment in callback() are the only reasons to lock _conns

    _conns.lock();

    do {
	again = FALSE;
	for (ListConn::iterator i = _conns.begin(); i != _conns.end(); ++i) {
	    if (*i == conn) {
		_conns.erase (i);
		again = TRUE;
		break;
	    }
	}
    } while (again);

    _conns.unlock();

#ifdef HAVE_THREADS
    conn->terminate();
#endif // HAVE_THREADS
    // abort pending invocations for this connection

#ifdef USE_IOP_CACHE
    if (_cache_used && _cache_rec->conn() == conn) {
	_orb->cancel (_cache_rec->orbid());
	_cache_used = FALSE;
    }
#endif

    do {
	MICOMT::AutoLock l(_orbids_mutex);

	again = FALSE;
	IIOPServerInvokeRec *rec;
	for (MapIdConn::iterator i = _orbids.begin(); 
	     i != _orbids.end(); ++i) {
	    rec = (*i).second;
	    if (rec->active() && (rec->conn() == conn)) {
		rec->deactivate();

		if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
		    MICOMT::AutoDebugLock __lock;
		    MICO::Logger::Stream (MICO::Logger::GIOP)
			<< "**aborting id="<< rec->orbmsgid()
			<< endl;
		}
		abort_invoke_orbid (rec);
		again = TRUE;
		break;
	    }
	}
    } while (again);
    deref_conn( conn, TRUE );
}

void
MICO::IIOPServer::conn_error (GIOPConn *conn, CORBA::Boolean send_error)
{
    if (!send_error) {
	kill_conn (conn);
	return;
    }

    GIOPOutContext out (conn->codec());
    conn->codec()->put_error_msg (out);
    conn->output (out._retn());

    conn->flush();
    //deref_conn ( conn, TRUE);
    this->kill_conn(conn);
}

void
MICO::IIOPServer::conn_closed (GIOPConn *conn)
{
    GIOPOutContext out (conn->codec());
    conn->codec()->put_close_msg (out);
    conn->output (out._retn());

    conn->flush();
#ifndef HAVE_THREADS
    deref_conn( conn );
#else // HAVE_THREADS
    conn->terminate();
#endif // HAVE_THREADS
}

CORBA::Boolean
MICO::IIOPServer::handle_input (GIOPConn *conn, CORBA::Buffer *inp)
{
    if (MICO::Logger::IsLogged (MICO::Logger::IIOP)) {
      MICOMT::AutoDebugLock __lock;
      MICO::Logger::Stream (MICO::Logger::IIOP)
	<< "IIOP: incoming data from "
	<< conn->transport()->peer()->stringify() << endl;
    }

    GIOPInContext in (conn->codec(), inp);

    GIOP::MsgType mt;
    CORBA::ULong size;
    CORBA::Octet flags;

    // XXX size is wrong for fragmented messages ...
    if (!conn->codec()->get_header (in, mt, size, flags)) {
      if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::GIOP)
	  << "GIOP: cannot decode incoming header from "
	  << conn->transport()->peer()->stringify() << endl;
      }
#ifdef HAVE_THREADS
      conn->active_deref();
#endif
      conn_error (conn);
      return FALSE;
    }

    switch (mt) {
    case GIOP::Request:
      return handle_invoke_request (conn, in);

    case GIOP::LocateRequest:
      return handle_locate_request (conn, in);

    case GIOP::MessageError:
#ifdef HAVE_THREADS
	conn->active_deref();
#endif // HAVE_THREADS
	if (!conn->codec()->get_error_msg (in)) {
	  if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::GIOP)
	      << "GIOP: cannot decode MessageError from "
	      << conn->transport()->peer()->stringify() << endl;
	  }
	  conn_error (conn, FALSE);
	} else {
	  if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::GIOP)
	      << "GIOP: incoming MessageError from "
	      << conn->transport()->peer()->stringify() << endl;
	  }
	  kill_conn (conn);
	}
	return FALSE;

    case GIOP::CancelRequest:
      return handle_cancel_request (conn, in);

    case GIOP::CloseConnection:
      if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::GIOP)
	  << "GIOP: incoming CloseConnection from "
	  << conn->transport()->peer()->stringify() << endl;
      }
      // for some strange reason Orbacus 4 clients send CloseConnection
      // upon exit ...
#ifdef HAVE_THREADS
      conn->active_deref();
#endif // HAVE_THREADS
      break;

    default:
#ifdef HAVE_THREADS
	conn->active_deref();
#endif // HAVE_THREADS
	if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::GIOP)
		<< "GIOP: bad incoming message type (" << mt << ") from "
		<< conn->transport()->peer()->stringify() << endl;
	}
	break;
    }
    return TRUE;
}

CORBA::ORBMsgId
MICO::IIOPServer::exec_invoke_request (GIOPInContext &in,
				       CORBA::Object_ptr obj, 
				       CORBA::ORBRequest *req,
				       CORBA::Principal_ptr pr,
				       CORBA::Boolean resp_exp,
				       GIOPConn *conn,
				       CORBA::ORBMsgId orbid)
{
    if (!strcmp (req->op_name(), "_bind")) {
	// its a bind
	CORBA::String_var repoid;
	CORBA::ORB::ObjectTag oid;
	CORBA::Boolean r = conn->codec()->get_bind_request (in, repoid.out(),
							    oid);
	assert (r);
	/*
	 * orb makes copies of repoid and oid so we can delete them
	 * after the call to bind_async()
	 */
	return _orb->bind_async (repoid, oid, 0, this, orbid);
    } else {
	// its a normal invocation
	return _orb->invoke_async (obj, req, pr, resp_exp, this, orbid);
    }
}

CORBA::Boolean
MICO::IIOPServer::handle_invoke_request (GIOPConn *conn, GIOPInContext &in)
{
    CORBA::ULong req_id;
    CORBA::Boolean resp;
    CORBA::ORBRequest *req;
    CORBA::Principal_ptr pr = conn->transport()->get_principal();
    CORBA::Object_ptr obj = new CORBA::Object (new CORBA::IOR);

    // XXX take care, get_invoke_request() does a in._retn()
    if (!conn->codec()->get_invoke_request (in, req_id, resp, obj, req, pr)) {
	CORBA::release (obj);
	CORBA::release (pr);

	if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	  MICOMT::AutoDebugLock __lock;
	  MICO::Logger::Stream (MICO::Logger::GIOP)
	    << "GIOP: cannot decode Request from "
	    << conn->transport()->peer()->stringify() << endl;
	}
#ifdef HAVE_THREADS
	conn->active_deref();
#endif // HAVE_THREADS
	conn->deref();
	conn_error (conn);
	return FALSE;
    }
    // XXX obj is incomplete, see IOR::operator== ...

    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
      MICOMT::AutoDebugLock __lock;
      MICO::Logger::Stream (MICO::Logger::GIOP)
	<< "GIOP: incoming Request from "
	<< conn->transport()->peer()->stringify()
	<< " with msgid " << req_id << endl;
    }

    /*
     * code sets are set up in get_contextlist(). GIOPRequest will
     * set up converters for out args, so we dont need to do anything
     * special.
     */

    /*
     * must install the invocation record before we call the ORB, because
     * may invoke callback before returning from invoke_async ...
     */
    MsgId msgid = _orb->new_msgid();
    CORBA::ORBMsgId orbid = 0;
    orbid = _orb->new_orbid(msgid);
    conn->ref ();
    IIOPServerInvokeRec *rec = create_invoke();
    rec->init_invoke (conn, req_id, orbid, req, obj, pr);
    add_invoke (rec);
#ifdef HAVE_THREADS
    // rssh: XXX:  are we must move active_deref into exec_invoke_request
    //  to prevent potential destruction of conn, when it used in 
    //  exec_invoke_request ?
    conn->active_deref();
#endif // HAVE_THREADS
    CORBA::ORBMsgId orbid2 = exec_invoke_request (in, obj, req, pr, resp, conn, orbid);
    assert (orbid == orbid2 || (orbid2 == 0 && !resp));

    // maybe the connection was closed inbetween: make do_read() break
    //return FALSE;
    // kcg: we need to return TRUE here, because of thread-per-connection
    // concurrency model, where we cannot break do_read when we are not
    // 100% sure that the connection is either closed or broken
    return TRUE;
}

CORBA::Boolean
MICO::IIOPServer::handle_locate_request (GIOPConn *conn, GIOPInContext &in)
{
    CORBA::ULong req_id;
    CORBA::Object_ptr obj = new CORBA::Object (new CORBA::IOR);

    if (!conn->codec()->get_locate_request (in, req_id, obj)) {
	CORBA::release (obj);
	if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	  MICOMT::AutoDebugLock __lock;
	  MICO::Logger::Stream (MICO::Logger::GIOP)
	    << "GIOP: cannot decode LocateRequest from "
	    << conn->transport()->peer()->stringify() << endl;
	}
#ifdef HAVE_THREADS
	conn->active_deref();
#endif // HAVE_THREADS
	conn_error (conn);
	return FALSE;
    }
    // XXX obj is incomplete, see IOR::operator== ...

    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
      MICOMT::AutoDebugLock __lock;
      MICO::Logger::Stream (MICO::Logger::GIOP)
	<< "GIOP: incoming LocateRequest from "
	<< conn->transport()->peer()->stringify()
	<< " with msgid " << req_id << endl;
    }

    /*
     * must install the invocation record before we call the ORB, because
     * may invoke callback before returning from invoke_async ...
     */
    CORBA::ORBMsgId orbid = _orb->new_orbid();
    conn->ref ();
    IIOPServerInvokeRec *rec = create_invoke();
    rec->init_locate (conn, req_id, orbid, obj);
    add_invoke (rec);
#ifdef HAVE_THREADS
    conn->active_deref();
#endif // HAVE_THREADS
    CORBA::ORBMsgId orbid2 = _orb->locate_async (obj, this, orbid);
    assert (orbid == orbid2);

    // maybe the connection was closed inbetween: make do_read() break
    //return FALSE;
    // kcg: we need to return TRUE here, because of thread-per-connection
    // concurrency model, where we cannot break do_read when we are not
    // 100% sure that the connection is either closed or broken
    return TRUE;
}

CORBA::Boolean
MICO::IIOPServer::handle_cancel_request (GIOPConn *conn, GIOPInContext &in)
{
    CORBA::ULong req_id;

    if (!conn->codec()->get_cancel_request (in, req_id)) {
      if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::GIOP)
	  << "GIOP: cannot decode CancelRequest from "
	  << conn->transport()->peer()->stringify() << endl;
      }
#ifdef HAVE_THREADS
      conn->active_deref();
#endif // HAVE_THREADS
      conn_error (conn);
      return FALSE;
    }

    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
      MICOMT::AutoDebugLock __lock;
      MICO::Logger::Stream (MICO::Logger::GIOP)
	<< "GIOP: incoming CancelRequest from "
	<< conn->transport()->peer()->stringify()
	<< " for msgid " << req_id << endl;
    }

    conn->cancel (req_id);

    IIOPServerInvokeRec *rec = pull_invoke_reqid (req_id, conn);
#ifdef HAVE_THREADS
    conn->active_deref();
#endif // HAVE_THREADS
    if (!rec) {
	// request already finished or no such id
	return TRUE;
    }
    CORBA::ORB::MsgId orbid = rec->orbmsgid();

    // maybe rec->conn() != conn ...
    // connection might be deleted during call to orb->cancel() ...

    //FIXME: should we realy delete it here ????????????
    del_invoke_orbid (rec);
    
    _orb->cancel (orbid);

    // maybe the connection was closed inbetween: make do_read() break
    //return FALSE;
    // kcg: we need to return TRUE here, because of thread-per-connection
    // concurrency model, where we cannot break do_read when we are not
    // 100% sure that the connection is either closed or broken
    return TRUE;
}

void
MICO::IIOPServer::handle_invoke_reply (CORBA::ORBMsgId id)
{
    CORBA::ORBRequest *req;
    CORBA::Object_ptr obj = CORBA::Object::_nil();

    GIOP::AddressingDisposition ad;

    IIOPServerInvokeRec *rec = pull_invoke_orbid ( id );
    if (rec == NULL)
	return;
    if (rec->orbid() != NULL) {
	if (!rec->orbid()->response_expected()) {
	    del_invoke_orbid (rec);
	    return;
	}
    }
    CORBA::InvokeStatus stat = _orb->get_invoke_reply (id, obj, req, ad);
    if (!rec) {
        // invocation canceled (perhaps connection to client broken)
	CORBA::release (obj);
        return;
    }
    // orb->get_invoke_reply calls orb->del_invoke, so we have to set
    // rec->orbid() to NULL
    rec->orbid(NULL);

    GIOP::ReplyStatusType giop_stat = GIOP::NO_EXCEPTION;
    switch (stat) {
    case CORBA::InvokeSysEx:
	giop_stat = GIOP::SYSTEM_EXCEPTION;
	break;

    case CORBA::InvokeUsrEx:
	giop_stat = GIOP::USER_EXCEPTION;
	break;

    case CORBA::InvokeOk:
	giop_stat = GIOP::NO_EXCEPTION;
	break;

    case CORBA::InvokeForward:
	giop_stat = GIOP::LOCATION_FORWARD;
	break;

    case CORBA::InvokeAddrDisp:
        giop_stat = GIOP::NEEDS_ADDRESSING_MODE;
        break;
    }

    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
      MICOMT::AutoDebugLock __lock;
      MICO::Logger::Stream (MICO::Logger::GIOP)
	<< "GIOP: sending Reply to "
	<< rec->conn()->transport()->peer()->stringify()
	<< " for msgid " << rec->reqid()
	<< " status is " << (CORBA::ULong) giop_stat
	<< endl;
    }

    GIOPOutContext out (rec->conn()->codec());
    if (!rec->conn()->codec()->put_invoke_reply (out, rec->reqid(), giop_stat,
						 obj, req, ad)) {
	out.reset ();
	CORBA::MARSHAL ex;
	req->set_out_args (&ex);
	rec->conn()->codec()->put_invoke_reply (out, rec->reqid(),
						GIOP::SYSTEM_EXCEPTION,
						obj, req, ad);
    }
    CORBA::release (obj);

    rec->conn()->output (out._retn());
    del_invoke_orbid (rec);
}

void
MICO::IIOPServer::handle_locate_reply (CORBA::ORBMsgId id)
{
    CORBA::Object_ptr obj = CORBA::Object::_nil();

    GIOP::AddressingDisposition ad;

    IIOPServerInvokeRec *rec = pull_invoke_orbid ( id );
    CORBA::LocateStatus stat = _orb->get_locate_reply (id, obj, ad);

    if (!rec) {
        // invocation canceled (perhaps connection to client broken)
	CORBA::release (obj);
        return;
    }

    GIOP::LocateStatusType giop_stat = GIOP::OBJECT_HERE;
    switch (stat) {
    case CORBA::LocateHere:
	giop_stat = GIOP::OBJECT_HERE;
	break;

    case CORBA::LocateUnknown:
	giop_stat = GIOP::UNKNOWN_OBJECT;
	break;

    case CORBA::LocateForward:
	giop_stat = GIOP::OBJECT_FORWARD;
	break;

    case CORBA::LocateAddrDisp:
        giop_stat = GIOP::LOC_NEEDS_ADDRESSING_MODE;
	break;
    }

    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
      MICOMT::AutoDebugLock __lock;
      MICO::Logger::Stream (MICO::Logger::GIOP)
	<< "GIOP: sending LocateReply to "
	<< rec->conn()->transport()->peer()->stringify()
	<< " for msgid " << rec->reqid()
	<< " status is " << (CORBA::ULong) giop_stat
	<< endl;
    }

    GIOPOutContext out (rec->conn()->codec());
    rec->conn()->codec()->put_locate_reply (out, rec->reqid(),
					    giop_stat, obj, ad);
    CORBA::release (obj);

    rec->conn()->output (out._retn());
    del_invoke_orbid (rec);
}

void
MICO::IIOPServer::handle_bind_reply (CORBA::ORBMsgId id)
{
    CORBA::Object_ptr obj = CORBA::Object::_nil();

    IIOPServerInvokeRec *rec = pull_invoke_orbid ( id );
    CORBA::LocateStatus stat = _orb->get_bind_reply (id, obj);

    if (!rec) {
        // invocation canceled (perhaps connection to client broken)
	CORBA::release (obj);
        return;
    }

    GIOP::LocateStatusType giop_stat = GIOP::OBJECT_HERE;
    switch (stat) {
    case CORBA::LocateHere:
	giop_stat = GIOP::OBJECT_HERE;
	break;

    case CORBA::LocateUnknown:
	giop_stat = GIOP::UNKNOWN_OBJECT;
	break;

    case CORBA::LocateForward:
	giop_stat = GIOP::OBJECT_FORWARD;
	break;
    default:
	break;
    }

    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
      MICOMT::AutoDebugLock __lock;
      MICO::Logger::Stream (MICO::Logger::GIOP)
	<< "GIOP: sending _bind Reply to "
	<< rec->conn()->transport()->peer()->stringify()
	<< " for msgid " << rec->reqid()
	<< " status is " << (CORBA::ULong) giop_stat
	<< endl;
    }

    GIOPOutContext out (rec->conn()->codec());
    rec->conn()->codec()->put_bind_reply (out, rec->reqid(), giop_stat, obj);
    CORBA::release (obj);

    rec->conn()->output (out._retn());
    del_invoke_orbid (rec);
}

const char *
MICO::IIOPServer::get_oaid () const
{
    return "mico-iiop-server";
}

CORBA::Boolean
MICO::IIOPServer::has_object (CORBA::Object_ptr)
{
    return FALSE;
}

#ifdef USE_CSL2
CORBA::Principal_ptr
MICO::IIOPServer::get_principal(CORBA::Object_ptr obj){
  assert(0);
  return (CORBA::Principal_ptr)0;
}
#endif /* USE_CSL2  */

CORBA::Boolean
MICO::IIOPServer::is_local () const
{
    // so we shutdown() after all local adapters which might
    // need IIOPServer for shutdown ...
    return FALSE;
}

CORBA::Boolean
MICO::IIOPServer::invoke (CORBA::ORBMsgId, CORBA::Object_ptr,
			  CORBA::ORBRequest *,
			  CORBA::Principal_ptr, CORBA::Boolean)
{
    assert (0);
    return TRUE;
}

CORBA::Boolean
MICO::IIOPServer::bind (CORBA::ORBMsgId, const char *,
			const CORBA::ORB::ObjectTag &,
			CORBA::Address *)
{
    return FALSE;
}

CORBA::Boolean
MICO::IIOPServer::locate (CORBA::ORBMsgId, CORBA::Object_ptr)
{
    assert (0);
    return FALSE;
}

CORBA::Object_ptr
MICO::IIOPServer::skeleton (CORBA::Object_ptr)
{
    assert (0);
    return CORBA::Object::_nil ();
}

void
MICO::IIOPServer::cancel (CORBA::ORBMsgId)
{
    assert (0);
}

void
MICO::IIOPServer::shutdown (CORBA::Boolean wait_for_completion)
{
    unsigned int cnt;

    for (cnt = _tservers.count(); cnt > 0; cnt--) {
	_tservers[_tservers.first()]->close();
	delete _tservers[_tservers.first()];
	_tservers.remove(_tservers.first());
    }

    /*
     * the GIOPConn entries in the 'orbids' and 'reqids' maps are just
     * pointers to the entries in the 'conns' list, so do not delete them
     */

    _conns.lock();

    for (ListConn::iterator i0 = _conns.begin(); i0 != _conns.end(); ++i0) {
	conn_closed (*i0);
	deref_conn(*i0, TRUE);
    }
    _conns.erase (_conns.begin(), _conns.end());

    _conns.unlock();


#ifdef USE_IOP_CACHE
    if (_cache_used)
	_orb->cancel (_cache_rec->orbid());
#endif

    {
	MICOMT::AutoLock l(_orbids_mutex);

	for (MapIdConn::iterator i1 = _orbids.begin();
	     i1 != _orbids.end(); ++i1) {
	    IIOPServerInvokeRec *rec = (*i1).second;
	    _orb->cancel ( rec->orbid() );
	    delete rec;
	}
	_orbids.erase (_orbids.begin(), _orbids.end());
    }
#ifdef USE_IOP_CACHE
    _cache_used = FALSE;
#endif
    _orb->answer_shutdown (this);
}

void
MICO::IIOPServer::answer_invoke (CORBA::ORBMsgId, CORBA::Object_ptr,
				 CORBA::ORBRequest *, CORBA::InvokeStatus)
{
    assert (0);
}

CORBA::Boolean
MICO::IIOPServer::waitfor (CORBA::ORB_ptr, CORBA::ORBMsgId id,
			   CORBA::ORBCallback::Event ev, CORBA::Long tmout)
{
    assert(0);
    return TRUE;
}

void
MICO::IIOPServer::notify (CORBA::ORB_ptr, CORBA::ORBMsgId id,
			  CORBA::ORBCallback::Event ev)
{
    switch (ev) {
    case CORBA::ORBCallback::Invoke:
	handle_invoke_reply (id);
	break;

    case CORBA::ORBCallback::Locate:
	handle_locate_reply (id);
	break;

    case CORBA::ORBCallback::Bind:
	handle_bind_reply (id);
	break;

    default:
	assert (0);
    }
}

CORBA::Boolean
MICO::IIOPServer::input_callback (GIOPConn *conn, CORBA::Buffer *inp)
{
    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::GIOP)
	    << "MICO::Server::input_callback (GIOPConn *conn, CORBA::Buffer *inp)" << endl
	    << "   conn: " << conn << endl
	    << "    inp: " << inp << endl;
    }

    return handle_input( conn, inp );
}

CORBA::Boolean
MICO::IIOPServer::callback (GIOPConn *conn, GIOPConnCallback::Event ev)
{
    switch (ev) {
    case GIOPConnCallback::InputReady:
	return input_callback (conn, conn->input());

    case GIOPConnCallback::Idle:
	if (MICO::Logger::IsLogged (MICO::Logger::IIOP)) {
	  MICOMT::AutoDebugLock __lock;
	  MICO::Logger::Stream (MICO::Logger::IIOP)
	    << "IIOP: shutting down idle conn to "
	    << conn->transport()->peer()->stringify() << endl;
	}
	conn_closed (conn);
	kill_conn (conn);
	return FALSE;

    case GIOPConnCallback::Closed: {
	if (MICO::Logger::IsLogged (MICO::Logger::IIOP)) {
	  MICOMT::AutoDebugLock __lock;
	  MICO::Logger::Stream (MICO::Logger::IIOP)
	    << "IIOP: connection to "
	    << conn->transport()->peer()->stringify() 
	    << " closed or broken" << endl;
	}
	const CORBA::Address *addr = conn->transport()->peer();
	assert (addr);
#ifdef USE_OLD_INTERCEPTORS
	Interceptor::ConnInterceptor::
	    _exec_client_disconnect (addr->stringify().c_str());
#endif // USE_OLD_INTERCEPTORS
	kill_conn (conn);
	return FALSE;
    }
    default:
	assert (0);
    }
    return TRUE;
}

void
MICO::IIOPServer::callback (CORBA::TransportServer *tserv,
			    CORBA::TransportServerCallback::Event ev)
{
    if (MICO::Logger::IsLogged (MICO::Logger::GIOP)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::GIOP)
	    << "MICO::IIOPServer::callback: tserv = " << tserv << ", ev =" << ev << endl;
    }
    switch (ev) {
    case CORBA::TransportServerCallback::Accept: {
	CORBA::Transport *t = tserv->accept();
	if (t) {
	    if (MICO::Logger::IsLogged (MICO::Logger::IIOP)) {
	      MICOMT::AutoDebugLock __lock;
	      MICO::Logger::Stream (MICO::Logger::IIOP)
		<< "IIOP: new connection opened from "
		<< t->peer()->stringify() << endl;
	    }
#ifdef HAVE_THREADS
	    if ( !_orb->resource_manager ().acquire_connection() ) {
		//XXX raise Exception - do what ever nessesary
		delete t;
		break;
	    }
#endif // HAVE_THREADS
            if (t->bad()) {
	        if (MICO::Logger::IsLogged (MICO::Logger::IIOP)) {
		  MICOMT::AutoDebugLock __lock;
		  MICO::Logger::Stream (MICO::Logger::IIOP)
		    << "IIOP: connection from "
		    << t->peer()->stringify() 
		    << " is bad: " << t->errormsg() << endl;
		}
                delete t;
#ifdef HAVE_THREADS
		_orb->resource_manager ().release_connection ();
#endif // HAVE_THREADS
                break;
            }
	    const CORBA::Address *addr = t->peer();
	    assert (addr);
#ifdef USE_OLD_INTERCEPTORS
	    CORBA::Boolean r = Interceptor::ConnInterceptor::
		_exec_client_connect (addr->stringify().c_str());
	    if (!r) {
		delete t;
#ifdef HAVE_THREADS
		_orb->resource_manager ().release_connection ();
#endif // HAVE_THREADS
		break;
	    }
#endif // USE_OLD_INTERCEPTORS
#ifdef HAVE_THREADS
	    CORBA::Boolean __use_reader_thread = TRUE;
	    CORBA::Boolean __use_writer_thread = FALSE;
	    if (MICO::MTManager::thread_pool())
	      __use_reader_thread = FALSE;

	    GIOPConn *conn =
		new GIOPConn (Dispatcher(), t, this,
			      new GIOPCodec (new CDRDecoder,
					     new CDREncoder,
					     _iiop_ver),
			      0L /* no tmout */, _max_message_size,
                              SERVER_SIDE,
			      __use_reader_thread, __use_writer_thread);
#else
	    GIOPConn *conn =
	        new GIOPConn (Dispatcher(), t, this,
			      new GIOPCodec (new CDRDecoder,
					     new CDREncoder,
					     _iiop_ver),
			      0L /* no tmout */, _max_message_size);
#endif
	    // this and kill_conn are the only reasons to lock _conns

	    _conns.lock();
	    _conns.push_back (conn);
	    _conns.unlock();
#ifdef HAVE_THREADS
	    conn->start();
#endif // HAVE_THREADS
	}
	break;
    }
    default:
	//FIXME: temp solution - a OA should have states like init, init_shutdown, shutdown, etc.
	//       geting Event::Remove during shutdown is perfectly OK (actualy it is also OK during normal 
	//       operation, although its a sign that something went horrible wrong) 
	//       - we should do the _tservs.remove here !
	//         - this would acctualy work since _tservers is a FastArray and not an STL type !!
	// assert (0);
	break;
    }
}


CORBA::Dispatcher*
MICO::IIOPServer::Dispatcher() {
#ifndef HAVE_THREADS
    return _orb->dispatcher();
#else
    if (MICO::MTManager::thread_pool()) {
      return _orb->dispatcher();
    }
    else {
      return MICO::GIOPConnMgr::Dispatcher();
    }
#endif
}

