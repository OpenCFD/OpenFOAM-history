/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 2001 by The Vivian project
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

#define MICO_CONF_INTERCEPT
#define MICO_CONF_IMR
#define MICO_CONF_IR
#define MICO_CONF_POA

#include <CORBA-SMALL.h>
#include <string.h>
#include <ctype.h>
#include <iostream>
#include <mico/util.h>
#include <mico/impl.h>
#include <mico/intercept.h>
#include <mico/ssl.h>
#include <mico/template_impl.h>
#include <mico/os-misc.h>
#include <mico/ltp.h>

#endif // FAST_PCH

using namespace std;

// Let's just put this here
void
MICOLTP::_init ()
{
    CORBA::IORProfileDecoder *l2capiop_profile_decoder =
	new L2CAPIOPProfileDecoder(CORBA::IORProfile::TAG_LTP_IOP);
    CORBA::IORProfile::register_decoder(l2capiop_profile_decoder);
    CORBA::AddressParser *ltp_address_parser = new LTPAddressParser;
    CORBA::Address::register_parser(ltp_address_parser);
}

/****************************** L2CAPIOPProfile *****************************/


MICOLTP::L2CAPIOPProfile::L2CAPIOPProfile (CORBA::Octet *o, CORBA::ULong l,
				const LTPAddress &ia,
				const CORBA::MultiComponent &mc,
				CORBA::UShort ver,
				ProfileId id)
{
    version = ver;

    if (version < 0x0101 && mc.size() > 0)
	version = 0x0101;

    tagid = id;
    comps = mc;

    objkey = new CORBA::Octet[length = l];
    memcpy (objkey, o, length);
    
    myaddr = ia;
}

MICOLTP::L2CAPIOPProfile::L2CAPIOPProfile (const L2CAPIOPProfile &ip)
{
    version = ip.version;

    tagid = ip.tagid;
    myaddr = ip.myaddr;
    comps = ip.comps;
    objkey = new CORBA::Octet[length = ip.length];
    memcpy (objkey, ip.objkey, length);
}

MICOLTP::L2CAPIOPProfile::~L2CAPIOPProfile ()
{
    delete[] objkey;
}

MICOLTP::L2CAPIOPProfile &
MICOLTP::L2CAPIOPProfile::operator= (const L2CAPIOPProfile &ip)
{
    if (this != &ip) {
	version = ip.version;
	tagid = ip.tagid;
	myaddr = ip.myaddr;
	comps = ip.comps;
	objectkey (ip.objkey, ip.length);
    }
    return *this;
}

void
MICOLTP::L2CAPIOPProfile::encode (CORBA::DataEncoder &ec) const
{
    ec.struct_begin ();
    {
	ec.struct_begin ();
	{
	    ec.put_octet ((CORBA::Octet)(version >> 8));
	    ec.put_octet ((CORBA::Octet)version);
	}
	ec.struct_end ();

	ec.put_string_raw(myaddr.bthost());
	ec.put_ushort(myaddr.psm());

	ec.seq_begin (length);
	{
	    ec.put_octets (objkey, length);
	}
	ec.seq_end ();

	if (version >= 0x0101)
	    comps.encode (ec);
    }
    ec.struct_end ();
}

const CORBA::Address *
MICOLTP::L2CAPIOPProfile::addr () const
{
    return &myaddr;
}

void
MICOLTP::L2CAPIOPProfile::addr (const MICOLTP::LTPAddress &ia)
{
    myaddr = ia;
}

MICOLTP::L2CAPIOPProfile::ProfileId
MICOLTP::L2CAPIOPProfile::id () const
{
    return tagid;
}

MICOLTP::L2CAPIOPProfile::ProfileId
MICOLTP::L2CAPIOPProfile::encode_id () const
{
    return tagid;
}

void
MICOLTP::L2CAPIOPProfile::objectkey (CORBA::Octet *o, CORBA::Long l)
{
    delete[] objkey;
    objkey = new CORBA::Octet[length = l];
    memcpy (objkey, o, length);
}

const CORBA::Octet *
MICOLTP::L2CAPIOPProfile::objectkey (CORBA::Long &l) const
{
    l = length;
    return objkey;
}

CORBA::Boolean
MICOLTP::L2CAPIOPProfile::reachable ()
{
    return TRUE;
}

void
MICOLTP::L2CAPIOPProfile::print (ostream &o) const
{
  o << "L2CAPIOP Profile" << endl;
  o << "    Version:  " << (CORBA::Long) (version >> 8) << "."
    << (CORBA::Long) (version & 0xff) << endl;
  o << "    Address:  " << myaddr.stringify() << endl;

  o << "   Location:  corbaloc::";
  if (version != 0x0100) {
    o << (CORBA::Long) (version >> 8) << "."
      << (CORBA::Long) (version & 0xff) << "@";
  }
  o << myaddr.bthost() << "#" << myaddr.psm();
  if (length > 0) {
    CORBA::String_var url = mico_url_encode (objkey, length);
    o << "/" << url.in() << endl;
  }

  comps.print (o);
}

CORBA::IORProfile *
MICOLTP::L2CAPIOPProfile::clone () const
{
    return new L2CAPIOPProfile (*this);
}

CORBA::Long
MICOLTP::L2CAPIOPProfile::compare (const CORBA::IORProfile &p) const
{
    if (p.id() != id())
	return (CORBA::Long)id() - (CORBA::Long)p.id();

    const L2CAPIOPProfile &ip = (const L2CAPIOPProfile &)p;

    if (length != ip.length)
	return (CORBA::Long)length - (CORBA::Long)ip.length;

    CORBA::Long r = mico_key_compare (objkey, ip.objkey, length);
    if (r)
	return r;

    if (version != ip.version)
	return (CORBA::Long)version - (CORBA::Long)ip.version;

    r = myaddr.compare (ip.myaddr);
    if (r)
	return r;

    return comps.compare (ip.comps);
}

CORBA::Boolean
MICOLTP::L2CAPIOPProfile::operator== (const CORBA::IORProfile &p) const
{
    return compare (p) == 0;
}

CORBA::Boolean
MICOLTP::L2CAPIOPProfile::operator< (const CORBA::IORProfile &p) const
{
    return compare (p) < 0;
}


/*************************** L2CAPIOPProfileDecoder *************************/

MICOLTP::L2CAPIOPProfileDecoder::L2CAPIOPProfileDecoder (CORBA::IORProfile::ProfileId id)
{
    tagid = id;
}

MICOLTP::L2CAPIOPProfileDecoder::~L2CAPIOPProfileDecoder ()
{
}

#define check(exp) if (!(exp)) goto bad;

CORBA::IORProfile *
MICOLTP::L2CAPIOPProfileDecoder::decode (CORBA::DataDecoder &dc, ProfileId,
                                  CORBA::ULong) const
{
    CORBA::Octet major, minor, *objkey;
    CORBA::UShort version;
    CORBA::ULong len;
    CORBA::UShort port;
    CORBA::MultiComponent comps;
    string host;
    CORBA::IORProfile *ip = 0;

    check (dc.struct_begin ());
    {
	check (dc.struct_begin ());
	{
	    check (dc.get_octet (major));
	    check (dc.get_octet (minor));
	    version = ((major << 8) | minor);
	}
	check (dc.struct_end ());

	check (dc.get_string_raw_stl (host));
	check (dc.get_ushort (port));

	check (dc.seq_begin (len));
	{
	    // XXX make sure seeked over data is still valid later
            check (dc.buffer()->length() >= len);
	    objkey = dc.buffer()->data();
	    dc.buffer()->rseek_rel (len);
	}
	check (dc.seq_end ());

	if (major > 1 || minor > 0)
	    check (comps.decode (dc));

	ip = new L2CAPIOPProfile (objkey, len,
			      LTPAddress (host.c_str(), port),
			      comps,
			      version,
			      tagid);
#ifdef HAVE_SSL
	if (comps.component (CORBA::Component::TAG_SSL_SEC_TRANS)) {
	    MICOSSL::SSLAddress sa (ip->addr()->clone());
	    ip = new MICOSSL::SSLProfile (ip, sa);
	}
#endif
    }
    check (dc.struct_end ());
    return ip;

bad:
    if (ip)
	delete ip;
    return 0;
}

#undef check

CORBA::Boolean
MICOLTP::L2CAPIOPProfileDecoder::has_id (MICOLTP::L2CAPIOPProfile::ProfileId id) const
{
    return tagid == id;
}
