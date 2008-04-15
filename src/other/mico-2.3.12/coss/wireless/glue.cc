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
#include <mico/impl.h>
#include "glue.h"

using namespace std;

/*************************** IdGenerator ***************************/

const CORBA::ULong WATMGlue::IdGenerator::INVALID_ID = 0xFFFFFFFFUL;

WATMGlue::IdGenerator::IdGenerator (CORBA::ULong i, CORBA::ULong s)
{
    current = i;
    step = s;
}

CORBA::ULong
WATMGlue::IdGenerator::new_id ()
{
    CORBA::ULong ret;

    do {
	ret = current;
	current += step;
    } while (ret == INVALID_ID || used_ids.find(ret) != used_ids.end());
    used_ids.insert(ret);
    return ret;
}

void
WATMGlue::IdGenerator::release_id (CORBA::ULong id)
{
    used_ids.erase(id);
}

/************************** TargetAddress **************************/

WATMGlue::TargetAddress::TargetAddress ()
{
    kind = Nil;
}

WATMGlue::TargetAddress::~TargetAddress ()
{
    free();
}

void
WATMGlue::TargetAddress::free ()
{
    switch (kind) {
    case Key:
	delete u.key;
	break;
    case Profile:
	delete u.profile;
	break;
    case Reference:
	delete u.reference.ior;
	break;
    }
    kind = Nil;
}

void
WATMGlue::TargetAddress::encode (CORBA::DataEncoder &ec) const
{
    ec.union_begin();
    {
	ec.put_short(kind);
	switch (kind) {
	case Key:
	    ec.seq_begin(u.key->length());
	    {
		ec.put_octets(u.key->get_buffer(), u.key->length());
	    }
	    ec.seq_end();
	    break;
	case Profile:
	    u.profile->encode(ec);
	    break;
	case Reference:
	    ec.struct_begin();
	    {
		ec.put_ulong(u.reference.index);
		ec.put_ior(*u.reference.ior);
	    }
	    ec.struct_end();
	    break;
	default:
	    assert(0);
	}
    }
    ec.union_end();
}

#define check(exp) if (!(exp)) goto bad

CORBA::Boolean
WATMGlue::TargetAddress::decode (CORBA::DataDecoder &dc)
{
    CORBA::Short temp_kind;
    free();

    check(dc.union_begin());
    {
	check(dc.get_short(temp_kind));
	switch (temp_kind) {
	case Key: {
	    CORBA::ULong keylen;
	    check(dc.seq_begin(keylen));
	    {
		u.key = new CORBA::OctetSeq(keylen);
		u.key->length(keylen);
		kind = temp_kind;
		check(dc.get_octets(u.key->get_buffer(), keylen));
	    }
	    check(dc.seq_end());
	    break;
	}
	case Profile: {
	    check(u.profile = CORBA::IORProfile::decode(dc));
	    kind = temp_kind;
	    break;
	}
	case Reference: {
	    check(dc.struct_begin());
	    {
		check(dc.get_ulong(u.reference.index));
		u.reference.ior = new CORBA::IOR;
		kind = temp_kind;
		check(dc.get_ior(*u.reference.ior));
	    }
	    check(dc.struct_end());
	    break;
	}
	default:
	    goto bad;
	}
    }
    check(dc.union_end());

    return TRUE;

  bad:
    free();
    return FALSE;
}

#undef check

void
WATMGlue::TargetAddress::dump (ostream &s) const
{
    s << "Target: " << kind << endl;
}

/************************** BridgeRequest **************************/

WATMGlue::BridgeRequest::BridgeRequest ()
{
    conn_id = IdGenerator::INVALID_ID;
    target = NULL;
    giop_data = NULL;
    data_type = GIOP::MessageError;
    req_id = 0;
    code = 0;
}

WATMGlue::BridgeRequest::~BridgeRequest ()
{
    delete target;
    delete giop_data;
}

void
WATMGlue::BridgeRequest::dump (ostream &s) const
{
    s << "--------" << endl;
    s << "Connection: " << conn_id << endl;
    if (target) target->dump(s);
    if (giop_data) giop_data->dump("Data", s);
    s << "--------" << endl;
}
