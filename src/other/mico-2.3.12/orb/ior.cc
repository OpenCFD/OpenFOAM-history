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

// Needed by CSIv2 support
#define MICO_CONF_INTERCEPT
#define MICO_CONF_IR
#define MICO_CONF_POA // needed for csi_base.h

#include <CORBA-SMALL.h>
//#include <CORBA.h>
#ifndef _WIN32
#include <string.h>
#endif
#include <ctype.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif
#include <mico/impl.h>
#include <mico/template_impl.h>
#include <mico/util.h>
#include <mico/ssl.h>
#include <mico/os-misc.h>
#ifdef USE_CSIV2
#include <mico/security/csiv2_impl.h>
#endif // USE_CSIV2

#endif // FAST_PCH


using namespace std;

/************************* misc dtors *******************************/


CORBA::IORProfile::~IORProfile ()
{
}

CORBA::MultiComponent *
CORBA::IORProfile::components ()
{
    return 0;
}


CORBA::IORProfileDecoder::~IORProfileDecoder ()
{
}


/***************************** IOR **********************************/


// consumes the IORProfiles in t
CORBA::IOR::IOR (const char *objid, const IORProfileVec &t)
    : oid (objid)
{
  _active_profile = 0;
  _active_profile_index = 0;
  _addressing_disposition = 0;
    for (mico_vec_size_type i = 0; i < t.size(); ++i)
	add_profile (t[i]);
}

CORBA::IOR::IOR (DataDecoder &dc)
{
  _active_profile = 0;
  _active_profile_index = 0;
  _addressing_disposition = 0;
    CORBA::Boolean r = dc.get_ior (*this);
    assert (r);
}

CORBA::IOR::IOR (const char *_ior)
{
  _active_profile = 0;
  _active_profile_index = 0;
  _addressing_disposition = 0;
    CORBA::Boolean r = from_string (_ior);
    assert (r);
}

CORBA::IOR::IOR (const IOR &ior)
{
    copy (ior);
}

CORBA::IOR::IOR ()
{
  _active_profile = 0;
  _active_profile_index = 0;
  _addressing_disposition = 0;
}

CORBA::IOR::~IOR ()
{
    free ();
}

CORBA::IOR &
CORBA::IOR::operator= (const IOR &ior)
{
    if (this != &ior) {
	free ();
	copy (ior);
    }
    return *this;
}

CORBA::Boolean
CORBA::IOR::from_string (const char *_ior)
{
  _active_profile = 0;
  _active_profile_index = 0;
  _addressing_disposition = 0;

    string ior (_ior);
    if (ior.find ("IOR:") != 0)
	return FALSE;

    ULong len = ior.length();
    if (((len-4) & 1) != 0)
	return FALSE;

    Buffer b;
    for (ULong i = 4; i < len; i += 2) {
	if (!isxdigit (ior[i]) || !isxdigit (ior[i+1]))
	    return FALSE;
	b.put ((mico_from_xdigit (ior[i]) << 4) | mico_from_xdigit (ior[i+1]));
    }
    MICO::CDRDecoder dc (&b, FALSE);

    /*
     * XXX [10-7] says we should encaps it, but *not* as sequence<octet>,
     * only include byteorder octet...
     */
    Octet bo;
    if (!dc.get_octet (bo))
	return FALSE;
    dc.byteorder (bo ? CORBA::LittleEndian : CORBA::BigEndian);
    if (!decode (dc)) {
	free ();
	return FALSE;
    }
    return TRUE;
}

CORBA::Long
CORBA::IOR::compare (const IOR &ior) const
{
    /*
     * we ignore the oid assuming that two IORs with the same
     * set of profiles cannot differ by oid.
     * beware: the IIOP module passes in IORs with empty oids...
     */
    int len = tags.size() < ior.tags.size() ? tags.size() : ior.tags.size();
    CORBA::Long r;
    for (int i = 0; i < len; ++i) {
	r = tags[i]->compare (*ior.tags[i]);
	if (r)
	    return r;
    }
    return tags.size() - ior.tags.size();
}

CORBA::Long
CORBA::IOR::compare_reachable (const IOR &ior) const
{
    /*
     * we ignore the oid assuming that two IORs with the same
     * set of profiles cannot differ by oid.
     * beware: the IIOP module passes in IORs with empty oids...
     */
    int ilen = tags.size();
    int jlen = ior.tags.size();
    CORBA::Long r;
    for (int i = 0, j = 0;; ++i, ++j) {
	while (i < ilen && !tags[i]->reachable())
	    ++i;
	while (j < jlen && !ior.tags[j]->reachable())
	    ++j;
	if (i == ilen)
	    return j-jlen;
	if (j == jlen)
	    return 1;
	r = tags[i]->compare (*ior.tags[j]);
	if (r)
	    return r;
    }
}

CORBA::Boolean
CORBA::IOR::operator== (const IOR &ior) const
{
    return compare (ior) == 0;
}

CORBA::Boolean
CORBA::IOR::operator< (const IOR &ior) const
{
    return compare (ior) < 0;
}

void
CORBA::IOR::free ()
{
    for (ULong i = 0; i < tags.size(); ++i)
	delete tags[i];
    tags.erase (tags.begin(), tags.end());
}

void
CORBA::IOR::copy (const IOR &ior)
{
    tags.erase (tags.begin(), tags.end());
    for (ULong i = 0; i < ior.tags.size(); ++i)
	add_profile (ior.tags[i]->clone());
    oid = ior.oid;
    _active_profile_index = ior._active_profile_index;
    if (ior._active_profile != NULL) {
        _active_profile = get_profile (_active_profile_index);
    }
    else {
        _active_profile = NULL;
    }
    _addressing_disposition = ior._addressing_disposition;
}

#define check(exp) if (!(exp)) goto bad

CORBA::Boolean
CORBA::IOR::decode (DataDecoder &dc)
{
    {
	free ();

	ULong len;

	check (dc.struct_begin ());
	check (dc.get_string_raw_stl (oid));
	check (dc.seq_begin (len));

	IORProfile *tag;
	for (ULong i = 0; i < len; ++i) {
	    tag = IORProfile::decode (dc);
	    check (tag);
	    add_profile (tag);
	}
	check (dc.seq_end ());
	check (dc.struct_end ());
	return TRUE;
    }

bad:
    free ();
    return FALSE;
}

#undef check

const CORBA::Address *
CORBA::IOR::addr (IORProfile::ProfileId id, Boolean find_unreach,
                  const Address *prev) const
{
    mico_vec_size_type i = 0;
    if (prev) {
        for ( ; i < tags.size(); ++i) {
            if (tags[i]->addr() == prev) {
                ++i;
                break;
            }
        }
    }
    for ( ; i < tags.size(); ++i) {
	if (id == IORProfile::TAG_ANY || tags[i]->id() == id) {
	    if (find_unreach || tags[i]->reachable())
		return tags[i]->addr();
	}
    }
    return 0;
}

CORBA::IORProfile *
CORBA::IOR::profile (IORProfile::ProfileId id, Boolean find_unreach,
                     IORProfile *prev)
{
    mico_vec_size_type i = 0;
    if (prev) {
        for ( ; i < tags.size(); ++i) {
            if (tags[i] == prev) {
                ++i;
                break;
            }
        }
    }
    for ( ; i < tags.size(); ++i) {
	if (id == IORProfile::TAG_ANY || tags[i]->id() == id) {
	    if (find_unreach || tags[i]->reachable())
		return tags[i];
	}
    }
    return 0;
}

CORBA::IORProfile *
CORBA::IOR::active_profile (CORBA::ULong * index)
{
  if (_active_profile && index) {
    *index = _active_profile_index;
  }
  return _active_profile;
}

void
CORBA::IOR::active_profile (CORBA::IORProfile * prof)
{
  CORBA::ULong i;

  if (!prof) {
    _active_profile = prof;
    return;
  }

  for (i=0; i<tags.size(); i++) {
    if (tags[i] == prof) {
      _active_profile_index = i;
      break;
    }
  }

  assert (i != tags.size());
  _active_profile = prof;
}

GIOP::AddressingDisposition
CORBA::IOR::addressing_disposition ()
{
  return _addressing_disposition;
}

void
CORBA::IOR::addressing_disposition (GIOP::AddressingDisposition disp)
{
  _addressing_disposition = disp;
}

void
CORBA::IOR::add_profile (IORProfile *prof)
{
    int i = tags.size();
    while (--i >= 0 && *prof < *tags[i])
	;
    tags.insert (tags.begin()+i+1, prof);
}

void
CORBA::IOR::del_profile (IORProfile *prof)
{
    for (mico_vec_size_type i = 0; i < tags.size(); ++i) {
	if (tags[i] == prof) {
	    tags.erase (tags.begin()+i);
	    break;
	}
    }
    if (prof == _active_profile) {
      _active_profile = 0;
    }
}

CORBA::IORProfile *
CORBA::IOR::get_profile (ULong i)
{
  if (i >= tags.size()) return 0;
  return tags[i];
}

const char *
CORBA::IOR::objid () const
{
    return oid.c_str();
}

void
CORBA::IOR::objid (const char *s)
{
    oid = s;
}

void
CORBA::IOR::objectkey (Octet *key, ULong keylen)
{
    for (mico_vec_size_type i = 0; i < tags.size(); ++i) {
	tags[i]->objectkey (key, keylen);
    }
}

void
CORBA::IOR::encode (DataEncoder &ec) const
{
    CORBA::DataEncoder::EncapsState state;

    ec.struct_begin ();
    ec.put_string_raw (oid.c_str());
    ec.seq_begin (tags.size());
    for (ULong i = 0; i < tags.size(); ++i) {
	ec.struct_begin ();
	ec.put_ulong (tags[i]->encode_id());
	// profiles are encapsulated
	ec.encaps_begin (state);
	tags[i]->encode (ec);
        ec.encaps_end (state);
	ec.struct_end ();
    }
    ec.seq_end ();
    ec.struct_end ();
}

string
CORBA::IOR::stringify () const
{
    string s ("IOR:");

    MICO::CDREncoder ec;

    /*
     * XXX [10-7] says we should encaps it, but *not* as sequence<octet>,
     * only include byteorder octet...
     */
    ec.put_octet (ec.byteorder() == CORBA::BigEndian ? FALSE : TRUE);
    encode (ec);

    Buffer *b = ec.buffer();
    Octet o;
    for (Long i = b->length(); --i >= 0; ) {
	b->get (o);
	s += mico_to_xdigit ((o >> 4) & 0xf);
	s += mico_to_xdigit (o & 0xf);
    }
    return s;
}

void
CORBA::IOR::print (ostream &o) const
{
    CORBA::Long len, j, k;
    char buf[10];

    o << "    Repo Id:  ";
    if (oid.length()) {
      o << oid << endl << endl;
    }
    else {
      o << "(unknown)" << endl << endl;
    }

    for (mico_vec_size_type i = 0; i < tags.size(); ++i) {
      tags[i]->print (o);

      const CORBA::Octet *key = tags[i]->objectkey (len);
      if (key) {
	o << "        Key:  ";
	for (j=0; j<len; j+=16) {
	  for (k=j; k<j+16 && k<len; k++) {
	    sprintf (buf, "%02x ", (int) key[k]);
	    o << buf;
	  }
	  for (; k<j+16; k++) {
	    sprintf (buf, "   ");
	    o << buf;
	  }
	  for (k=j; k<j+16 && k<len; k++) {
	    if (isprint (key[k])) {
	      o << (char) key[k];
	    }
	    else {
	      o << '.';
	    }
	  }
	  o << endl;
	  
	  if (k < len) {
	    o << "              ";
	  }
	}
	if (len == 0) {
	  o << "(empty)";
	}
	if (j == len) {
	  o << endl;
	}
      }
      o << endl;
    }
}


/****************************** IORProfile *****************************/


namespace CORBA {
  vector<CORBA::IORProfileDecoder *> *IORProfile::decoders = 0;
}

void
CORBA::IORProfile::copy (IORProfile *p)
{
}

#define check(exp) if (!(exp)) goto bad

CORBA::IORProfile *
CORBA::IORProfile::decode (DataDecoder &dc)
{
    ULong pid, len, next_pos;
    CORBA::DataDecoder::EncapsState state;
    IORProfile *p = 0;

    check (dc.struct_begin ());
    check (dc.get_ulong (pid));
    check (dc.encaps_begin (state, len));

    next_pos = dc.buffer()->rpos() + len;
    p = decode_body (dc, (ProfileId)pid, len);
    // seek over everthing not read by the profile decoder
    check (dc.buffer()->rseek_beg (next_pos));

    check (p);
    check (dc.encaps_end (state));
    check (dc.struct_end ());
    return p;

bad:
    if (p)
	delete p;
    return 0;
}

#undef check

CORBA::IORProfile *
CORBA::IORProfile::decode_body (DataDecoder &dc, ProfileId pid, ULong len)
{
    if (!decoders)
	decoders = new vector<CORBA::IORProfileDecoder *>;

    ULong i;
    IORProfile *p;

    for (i = 0; i < decoders->size(); ++i) {
	if ((*decoders)[i]->has_id (pid))
	    break;
    }
    if (i < decoders->size()) {
	// known profile
	p = (*decoders)[i]->decode (dc, pid, len);
    } else {
	// unknown profile
	p = MICO::UnknownProfile::decode (dc, pid, len);
    }
    return p;
}

void
CORBA::IORProfile::register_decoder (IORProfileDecoder *decoder)
{
    if (!decoders)
	decoders = new vector<CORBA::IORProfileDecoder *>;
    decoders->push_back (decoder);
}

void
CORBA::IORProfile::unregister_decoder (IORProfileDecoder *decoder)
{
    if (!decoders)
	return;
    for (ULong i = 0; i < decoders->size(); ) {
	if ((*decoders)[i] == decoder)
	    decoders->erase (decoders->begin() + i);
	else
	    ++i;
    }
}


/****************************** LocalProfile *****************************/


MICO::LocalProfile::LocalProfile (CORBA::Octet *o, CORBA::ULong l,
				  ProfileId id,
				  const char *_host,
				  CORBA::Long _pid)
{
    tagid = id;
    host = _host ? _host : InetAddress::hostname().c_str();
    pid = (_pid >= 0) ? _pid : OSMisc::getpid();
    objkey = new CORBA::Octet[length = l];
    memcpy (objkey, o, length);
}

MICO::LocalProfile::LocalProfile (const LocalProfile &lp)
{
    tagid = lp.tagid;
    host = lp.host;
    pid = lp.pid;
    objkey = new CORBA::Octet[length = lp.length];
    memcpy (objkey, lp.objkey, length);
}

MICO::LocalProfile::~LocalProfile ()
{
    delete[] objkey;
}

MICO::LocalProfile &
MICO::LocalProfile::operator= (const LocalProfile &lp)
{
    if (this != &lp) {
	tagid = lp.tagid;
	host = lp.host;
	pid = lp.pid;
	objectkey (lp.objkey, lp.length);
    }
    return *this;
}

void
MICO::LocalProfile::encode (CORBA::DataEncoder &ec) const
{
    ec.struct_begin ();
    {
	ec.put_string_raw (host.c_str());
	ec.put_long (pid);

	ec.seq_begin (length);
	{
	    ec.put_octets (objkey, length);
	}
	ec.seq_end ();
    }
    ec.struct_end ();
}

const CORBA::Address *
MICO::LocalProfile::addr () const
{
    return &myaddr;
}

CORBA::IORProfile::ProfileId
MICO::LocalProfile::id () const
{
    return tagid;
}

CORBA::IORProfile::ProfileId
MICO::LocalProfile::encode_id () const
{
    return tagid;
}

void
MICO::LocalProfile::objectkey (CORBA::Octet *o, CORBA::Long l)
{
    delete[] objkey;
    objkey = new CORBA::Octet[length = l];
    memcpy (objkey, o, length);
}

const CORBA::Octet *
MICO::LocalProfile::objectkey (CORBA::Long &l) const
{
    l = length;
    return objkey;
}

CORBA::Boolean
MICO::LocalProfile::reachable ()
{
#ifdef _WIN32
    assert( 0 );
    return true;
#else
    return InetAddress::samehosts (host, InetAddress::hostname()) &&
	pid == OSMisc::getpid();
#endif
}

void
MICO::LocalProfile::print (ostream &o) const
{
  o << "Local Profile" << endl;
}

CORBA::IORProfile *
MICO::LocalProfile::clone () const
{
    return new LocalProfile (*this);
}

CORBA::Long
MICO::LocalProfile::compare (const CORBA::IORProfile &p) const
{
    if (p.id() != id())
	return (CORBA::Long)id() - (CORBA::Long)p.id();
    const LocalProfile &lp = (const LocalProfile &)p;
    if (pid != lp.pid)
	return (CORBA::Long)pid - (CORBA::Long)lp.pid;
    if (length != lp.length)
	return (CORBA::Long)length - (CORBA::Long)lp.length;
    CORBA::Long r = mico_key_compare (objkey, lp.objkey, length);
    if (r)
	return r;
    r = host.compare (lp.host);
    if (r)
	return r;
    return myaddr.compare (lp.myaddr);
}

CORBA::Boolean
MICO::LocalProfile::operator== (const CORBA::IORProfile &p) const
{
    return compare (p) == 0;
}

CORBA::Boolean
MICO::LocalProfile::operator< (const CORBA::IORProfile &p) const
{
    return compare (p) < 0;
}


/*************************** LocalProfileDecoder *************************/


MICO::LocalProfileDecoder::LocalProfileDecoder (
    CORBA::IORProfile::ProfileId id)
{
    tagid = id;
    CORBA::IORProfile::register_decoder (this);
}

MICO::LocalProfileDecoder::~LocalProfileDecoder ()
{
    CORBA::IORProfile::unregister_decoder (this);
}

#define check(exp) if (!(exp)) goto bad

CORBA::IORProfile *
MICO::LocalProfileDecoder::decode (CORBA::DataDecoder &dc, ProfileId,
                                   CORBA::ULong) const
{
    CORBA::ULong len;
    string host;
    CORBA::Long pid;
    LocalProfile *p = 0;

    check (dc.struct_begin ());
    {
	check (dc.get_string_raw_stl (host));
	check (dc.get_long (pid));

	check (dc.seq_begin (len));
	{
            check (dc.buffer()->length() >= len);
	    p = new LocalProfile (dc.buffer()->data(), len,
				  tagid, host.c_str(), pid);
	    dc.buffer()->rseek_rel (len);
	}
	check (dc.seq_end ());
    }
    check (dc.struct_end ());
    return p;

bad:
    if (p)
	delete p;
    return 0;
}

#undef check

CORBA::Boolean
MICO::LocalProfileDecoder::has_id (CORBA::IORProfile::ProfileId id) const
{
    return tagid == id;
}

static MICO::LocalProfileDecoder local_ior_decoder;


/****************************** IIOPProfile *****************************/


MICO::IIOPProfile::IIOPProfile (CORBA::Octet *o, CORBA::ULong l,
				const InetAddress &ia,
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

    object_reg = new RegAddr;
    object_reg->reged = TRUE;
    object_reg->refcnt = 1;
}

MICO::IIOPProfile::IIOPProfile (const IIOPProfile &ip)
{
    version = ip.version;

    tagid = ip.tagid;
    myaddr = ip.myaddr;
    comps = ip.comps;
    objkey = new CORBA::Octet[length = ip.length];
    memcpy (objkey, ip.objkey, length);
    object_reg = ip.object_reg;
    object_reg->refcnt++;
}

MICO::IIOPProfile::~IIOPProfile ()
{
    delete[] objkey;
    if (--object_reg->refcnt == 0) {
	delete object_reg;
    }
}

MICO::IIOPProfile &
MICO::IIOPProfile::operator= (const IIOPProfile &ip)
{
    if (this != &ip) {
	version = ip.version;
	tagid = ip.tagid;
	myaddr = ip.myaddr;
	comps = ip.comps;
	objectkey (ip.objkey, ip.length);
	if (--object_reg->refcnt == 0) {
	    delete object_reg;
	}
	object_reg = ip.object_reg;
	object_reg->refcnt++;
    }
    return *this;
}

void
MICO::IIOPProfile::encode (CORBA::DataEncoder &ec) const
{
    ec.struct_begin ();
    {
	ec.struct_begin ();
	{
	    ec.put_octet ((CORBA::Octet)(version >> 8));
	    ec.put_octet ((CORBA::Octet)version);
	}
	ec.struct_end ();

	ec.put_string_raw (myaddr.host());
	ec.put_ushort (myaddr.port());

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
MICO::IIOPProfile::addr () const
{
    return &myaddr;
}

void
MICO::IIOPProfile::addr (const MICO::InetAddress &ia)
{
    myaddr = ia;
}

MICO::IIOPProfile::ProfileId
MICO::IIOPProfile::id () const
{
    return tagid;
}

MICO::IIOPProfile::ProfileId
MICO::IIOPProfile::encode_id () const
{
    return tagid;
}

void
MICO::IIOPProfile::objectkey (CORBA::Octet *o, CORBA::Long l)
{
    delete[] objkey;
    objkey = new CORBA::Octet[length = l];
    memcpy (objkey, o, length);
    if (!object_reg->reged) {
	CORBA::ORB_var orb = CORBA::ORB_instance("mico-local-orb");
	CORBA::Object_var obj =
	    orb->resolve_initial_references("MobileTerminalBridge");
	/*
	 * XXX - Maybe this needs a custom marshalling routine so we can
	 *       avoid DII
	 */
	CORBA::Request_var req = obj->_request("register_profile");
	req->add_in_arg() <<=
	    (CORBA::ULong) CORBA::IORProfile::TAG_INTERNET_IOP;
	MICO::CDREncoder ec;
	encode(ec);
	CORBA::OctetSeq prof(ec.buffer()->length(), ec.buffer()->length(),
			     ec.buffer()->data(), FALSE);
	req->add_in_arg() <<= prof;
	req->set_return_type(CORBA::_tc_string);
	req->invoke();
	CORBA::Exception *ex = req->env()->exception();
	if (ex == NULL) {
	    const char *addr;
	    req->return_value() >>= addr;
	    CORBA::Address *gaddr = CORBA::Address::parse(addr);
	    assert(gaddr != NULL);
	    MICO::InetAddress *iaddr =
		dynamic_cast<MICO::InetAddress *>(gaddr);
	    assert(iaddr != NULL);
	    myaddr = *iaddr;
	} else if (CORBA::UserException::_downcast(ex)) {
	    // The only user exception is AddressNotAvailable
	    // XXX - I don't have a clue what to do now
	} else {
	    // A system exception
	    // XXX - Maybe we should throw something here?
	}
	object_reg->reged = TRUE;
    }
}

const CORBA::Octet *
MICO::IIOPProfile::objectkey (CORBA::Long &l) const
{
    l = length;
    return objkey;
}

CORBA::Boolean
MICO::IIOPProfile::reachable ()
{
    return TRUE;
}

void
MICO::IIOPProfile::print (ostream &o) const
{
  o << "IIOP Profile" << endl;
  o << "    Version:  " << (CORBA::Long) (version >> 8) << "."
    << (CORBA::Long) (version & 0xff) << endl;
  o << "    Address:  " << myaddr.stringify() << endl;

  o << "   Location:  corbaloc::";
  if (version != 0x0100) {
    o << (CORBA::Long) (version >> 8) << "."
      << (CORBA::Long) (version & 0xff) << "@";
  }
  o << myaddr.host() << ":" << myaddr.port();
  if (length > 0) {
    CORBA::String_var url = mico_url_encode (objkey, length);
    o << "/" << url.in() << endl;
  }

  comps.print (o);
}

void
MICO::IIOPProfile::prepare_mobile ()
{
    object_reg->reged = FALSE;
}

CORBA::IORProfile *
MICO::IIOPProfile::clone () const
{
    return new IIOPProfile (*this);
}

CORBA::Long
MICO::IIOPProfile::compare (const CORBA::IORProfile &p) const
{
    if (p.id() != id())
	return (CORBA::Long)id() - (CORBA::Long)p.id();

    const IIOPProfile &ip = (const IIOPProfile &)p;

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
MICO::IIOPProfile::operator== (const CORBA::IORProfile &p) const
{
    return compare (p) == 0;
}

CORBA::Boolean
MICO::IIOPProfile::operator< (const CORBA::IORProfile &p) const
{
    return compare (p) < 0;
}


/*************************** IIOPProfileDecoder *************************/


MICO::IIOPProfileDecoder::IIOPProfileDecoder (CORBA::IORProfile::ProfileId id,
					      MICO::InetAddress::Family f)
{
    tagid = id;
    family = f;
    CORBA::IORProfile::register_decoder (this);
}

MICO::IIOPProfileDecoder::~IIOPProfileDecoder ()
{
    CORBA::IORProfile::unregister_decoder (this);
}

#define check(exp) if (!(exp)) goto bad;

CORBA::IORProfile *
MICO::IIOPProfileDecoder::decode (CORBA::DataDecoder &dc, ProfileId,
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

	ip = new IIOPProfile (objkey, len,
			      InetAddress (host.c_str(), port, family),
			      comps,
			      version,
			      tagid);
#ifdef HAVE_SSL
	if (comps.component (CORBA::Component::TAG_SSL_SEC_TRANS)) {
	    MICOSSL::SSLAddress sa (ip->addr()->clone());
	    ip = new MICOSSL::SSLProfile (ip, sa);
	}
#endif
#ifdef HAVE_SSL
#ifdef USE_CSIV2
	CORBA::Component* __t_comp = NULL;
	__t_comp = comps.component(CSIIOP::TAG_CSI_SEC_MECH_LIST);
	CSIv2::Component* __c = NULL;
	__c = dynamic_cast<CSIv2::Component*>(__t_comp);
	if (__c != NULL) {
  	    CSIIOP::CompoundSecMechList* __list = __c->mech_list();
	    if (__list->mechanism_list[0].transport_mech.tag == CSIIOP::TAG_TLS_SEC_TRANS) {
		// CSIv2 uses TLS support
		if (MICO::Logger::IsLogged(MICO::Logger::Security))
		    MICO::Logger::Stream(MICO::Logger::Security)
			<< "CSIv2(ior.cc): decoding IIOP profile with CSIv2 TLS component." << endl;
		MICOSSL::SSLAddress sa (ip->addr()->clone());
		ip = new MICOSSL::SSLProfile (ip, sa);
	    }
	}
#endif // USE_CSIV2
#endif // HAVE_SSL
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
MICO::IIOPProfileDecoder::has_id (MICO::IIOPProfile::ProfileId id) const
{
    return tagid == id;
}

static MICO::IIOPProfileDecoder iiop_ior_decoder (
    CORBA::IORProfile::TAG_INTERNET_IOP, MICO::InetAddress::STREAM);
static MICO::IIOPProfileDecoder udpiop_ior_decoder (
    CORBA::IORProfile::TAG_UDP_IOP, MICO::InetAddress::DGRAM);

/****************************** UIOPProfile *****************************/

#ifdef HAVE_SYS_UN_H
MICO::UIOPProfile::UIOPProfile (CORBA::Octet *o, CORBA::ULong l,
				const UnixAddress &ua,
				const CORBA::MultiComponent &mc,
				CORBA::UShort ver,
				ProfileId id,
				const char *_host)
{
    version = ver;

    if (version < 0x0101 && mc.size() > 0)
	version = 0x0101;
    
    tagid = id;
    if (_host)
        host = _host;
    else
        host = InetAddress::hostname();
    myaddr = ua;
    comps = mc;
    objkey = new CORBA::Octet[length = l];
    memcpy (objkey, o, length);
}

MICO::UIOPProfile::UIOPProfile (const UIOPProfile &ip)
{
    version = ip.version;
    comps = ip.comps;
    tagid = ip.tagid;
    host = ip.host;
    myaddr = ip.myaddr;
    objkey = new CORBA::Octet[length = ip.length];
    memcpy (objkey, ip.objkey, length);
}

MICO::UIOPProfile::~UIOPProfile ()
{
    delete[] objkey;
}

MICO::UIOPProfile &
MICO::UIOPProfile::operator= (const UIOPProfile &ip)
{
    if (this != &ip) {
	version = ip.version;
	comps = ip.comps;
	tagid = ip.tagid;
	host = ip.host;
	myaddr = ip.myaddr;
	objectkey (ip.objkey, ip.length);
    }
    return *this;
}

void
MICO::UIOPProfile::encode (CORBA::DataEncoder &ec) const
{
    ec.struct_begin ();
    {
	ec.struct_begin ();
	{
	    ec.put_octet ((CORBA::Octet)(version >> 8));
	    ec.put_octet ((CORBA::Octet)version);
	}
	ec.struct_end ();

	ec.put_string_raw (host.c_str());
	ec.put_string_raw (myaddr.filename());

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
MICO::UIOPProfile::addr () const
{
    return &myaddr;
}

MICO::UIOPProfile::ProfileId
MICO::UIOPProfile::id () const
{
    return tagid;
}

MICO::UIOPProfile::ProfileId
MICO::UIOPProfile::encode_id () const
{
    return tagid;
}

void
MICO::UIOPProfile::objectkey (CORBA::Octet *o, CORBA::Long l)
{
    delete[] objkey;
    objkey = new CORBA::Octet[length = l];
    memcpy (objkey, o, length);
}

const CORBA::Octet *
MICO::UIOPProfile::objectkey (CORBA::Long &l) const
{
    l = length;
    return objkey;
}

CORBA::Boolean
MICO::UIOPProfile::reachable ()
{
    return InetAddress::samehosts (host, InetAddress::hostname());
}

void
MICO::UIOPProfile::print (ostream &o) const
{
  o << "UIOP Profile" << endl;
  o << "    Address:  " << myaddr.stringify() << endl;
}

CORBA::IORProfile *
MICO::UIOPProfile::clone () const
{
    return new UIOPProfile (*this);
}

CORBA::Long
MICO::UIOPProfile::compare (const CORBA::IORProfile &p) const
{
    if (p.id() != id())
	return (CORBA::Long)id() - (CORBA::Long)p.id();

    const UIOPProfile &ip = (const UIOPProfile &)p;

    if (length != ip.length)
	return (CORBA::Long)length - (CORBA::Long)ip.length;

    CORBA::Long r = mico_key_compare (objkey, ip.objkey, length);
    if (r)
	return r;

    if (version != ip.version)
	return (CORBA::Long)version - (CORBA::Long)ip.version;

    r = host.compare (ip.host);
    if (r)
	return r;

    r = myaddr.compare (ip.myaddr);
    if (r)
	return r;

    return comps.compare (ip.comps);
}

CORBA::Boolean
MICO::UIOPProfile::operator== (const CORBA::IORProfile &p) const
{
    return compare (p) == 0;
}

CORBA::Boolean
MICO::UIOPProfile::operator< (const CORBA::IORProfile &p) const
{
    return compare (p) < 0;
}


/*************************** UIOPProfileDecoder *************************/


MICO::UIOPProfileDecoder::UIOPProfileDecoder (CORBA::IORProfile::ProfileId id)
{
    tagid = id;
    CORBA::IORProfile::register_decoder (this);
}

MICO::UIOPProfileDecoder::~UIOPProfileDecoder ()
{
    CORBA::IORProfile::unregister_decoder (this);
}

#define check(exp) if (!(exp)) goto bad

CORBA::IORProfile *
MICO::UIOPProfileDecoder::decode (CORBA::DataDecoder &dc, ProfileId,
                                  CORBA::ULong) const
{
    CORBA::Octet minor, major, *objkey;
    CORBA::UShort version;
    CORBA::ULong len;
    CORBA::MultiComponent comps;
    string host, filename;
    CORBA::IORProfile *ip = 0;

    check (dc.struct_begin ());
    {
	check (dc.struct_begin ());
	{
	    check (dc.get_octet (major));
	    check (dc.get_octet (minor));
	    version = ((major << 8) | minor);
	    check (version <= 0x0102);
	}
	check (dc.struct_end ());

	check (dc.get_string_raw_stl (host));
	check (dc.get_string_raw_stl (filename));

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

	ip = new UIOPProfile (objkey, len,
			      UnixAddress (filename.c_str()),
			      comps,
			      version,
			      tagid, host.c_str());
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
MICO::UIOPProfileDecoder::has_id (MICO::UIOPProfile::ProfileId id) const
{
    return CORBA::IORProfile::TAG_UNIX_IOP == id;
}

static MICO::UIOPProfileDecoder uiop_ior_decoder;
#endif // HAVE_SYS_UN_H


/*************************** UnknownProfile ****************************/


MICO::UnknownProfile::UnknownProfile (ProfileId id)
    : tagid (id)
{
    // byteorder octet
    tagdata.push_back (0);
}

MICO::UnknownProfile::UnknownProfile (const UnknownProfile &p)
    : tagid (p.tagid), tagdata (p.tagdata)
{
}

MICO::UnknownProfile::~UnknownProfile ()
{
}

MICO::UnknownProfile &
MICO::UnknownProfile::operator= (const UnknownProfile &p)
{
    tagid = p.tagid;
    tagdata = p.tagdata;
    return *this;
}

void
MICO::UnknownProfile::encode (CORBA::DataEncoder &ec) const
{
    // XXX vector elements need not be continuous ...
    // seek back one byte to overwrite byteorder octet
    ec.buffer()->wseek_rel (-1);
    ec.put_octets (&tagdata[0], tagdata.size());
}

const CORBA::Address *
MICO::UnknownProfile::addr () const
{
    return 0;
}

MICO::UnknownProfile::ProfileId
MICO::UnknownProfile::id () const
{
    return tagid;
}

MICO::UnknownProfile::ProfileId
MICO::UnknownProfile::encode_id () const
{
    return tagid;
}

void
MICO::UnknownProfile::objectkey (CORBA::Octet *, CORBA::Long)
{
}

const CORBA::Octet *
MICO::UnknownProfile::objectkey (CORBA::Long &) const
{
    return 0;
}

CORBA::Boolean
MICO::UnknownProfile::reachable ()
{
    return FALSE;
}

void
MICO::UnknownProfile::print (ostream &o) const
{
  o << "Unknown Profile" << endl;
  o << " Profile Id:  " << tagid << endl;

    if (tagid != CORBA::IORProfile::TAG_MULTIPLE_COMPONENTS) {
      return;
    }

    o << " Components:  ";

    MICO::CDRDecoder dc;
    dc.buffer()->put (&tagdata[0], tagdata.size());

    // get byteorder octet from encapsulated sequence
    CORBA::Octet bo;
    CORBA::Boolean r = dc.get_octet (bo);
    assert (r);
    dc.byteorder (bo ? CORBA::LittleEndian : CORBA::BigEndian);

    char buf[16];
    CORBA::ULong len, l, c=16;
    dc.seq_begin (len);

    for (CORBA::ULong i = 0; i < len; ++i) {
	dc.struct_begin ();
	{
	    dc.get_ulong (l);

	    sprintf (buf, "%lu", (unsigned long) l);
	    if (c + strlen (buf) + 1 >= 80) {
	      o << endl;
	      o << "              ";
	      c = 16;
	    }
	    else if (i > 0) {
	      o << " ";
	    }
	    o << buf;
	    c += strlen (buf) + 1;
		
	    dc.seq_begin (l);
	    if (!dc.buffer()->rseek_rel (l)) {
                o << endl << "bad component data" << endl;
                return;
            }
	    dc.seq_end ();
	}
	dc.struct_end ();
    }
    dc.seq_end ();
    o << endl;
}

CORBA::IORProfile *
MICO::UnknownProfile::clone () const
{
    return new UnknownProfile (*this);
}

CORBA::Long
MICO::UnknownProfile::compare (const CORBA::IORProfile &p) const
{
    if (id() != p.id())
	return (CORBA::Long)id() - (CORBA::Long)p.id();

    UnknownProfile &up = (UnknownProfile &)p;
    return mico_vec_compare (tagdata, up.tagdata);
}

CORBA::Boolean
MICO::UnknownProfile::operator== (const CORBA::IORProfile &p) const
{
    return compare (p) == 0;
}

CORBA::Boolean
MICO::UnknownProfile::operator< (const CORBA::IORProfile &p) const
{
    return compare (p) < 0;
}

MICO::UnknownProfile *
MICO::UnknownProfile::decode (CORBA::DataDecoder &dc, ProfileId id,
			      CORBA::ULong len)
{
    if (len > 10000)
	// this must be garbage
	return 0;

    if (dc.buffer()->length() < len)
        return 0;

    UnknownProfile *p = new UnknownProfile (id);

    p->tagdata.erase (p->tagdata.begin(), p->tagdata.end());
    // include byteorder octet from encapsulated sequence
    p->tagdata.insert (p->tagdata.begin(),
		       dc.buffer()->data() - 1, dc.buffer()->data() + len);
    dc.buffer()->rseek_rel (len);

    return p;
}


/*************************** MultiComponent *****************************/


CORBA::MultiComponent::MultiComponent ()
{
}

void
CORBA::MultiComponent::free ()
{
    for (mico_vec_size_type i = 0; i < _comps.size(); ++i)
        delete _comps[i];
        
    _comps.erase (_comps.begin(), _comps.end());
}

void
CORBA::MultiComponent::copy (const MultiComponent &mc)
{
    for (mico_vec_size_type i = 0; i < mc._comps.size(); ++i)
        _comps.push_back (mc._comps[i]->clone());
}

CORBA::MultiComponent::MultiComponent (const MultiComponent &mc)
{
    copy (mc);
}

CORBA::MultiComponent &
CORBA::MultiComponent::operator= (const MultiComponent &mc)
{
    if (this != &mc) {
        free ();
        copy (mc);
    }
    return *this;
}

CORBA::MultiComponent::~MultiComponent ()
{
    free ();
}

CORBA::Long
CORBA::MultiComponent::compare (const MultiComponent &mc) const
{
    int len = _comps.size() < mc._comps.size()
        ? _comps.size()
        : mc._comps.size();

    CORBA::Long r;
    for (int i = 0; i < len; ++i) {
	r = _comps[i]->compare (*mc._comps[i]);
	if (r)
	    return r;
    }
    return _comps.size() - mc._comps.size();
}

CORBA::Boolean
CORBA::MultiComponent::operator< (const MultiComponent &mc) const
{
    return compare (mc) < 0;
}

CORBA::Boolean
CORBA::MultiComponent::operator== (const MultiComponent &mc) const
{
    return compare (mc) == 0;
}

void
CORBA::MultiComponent::add_component (Component *c)
{
    int i = _comps.size();
    while (--i >= 0 && *c < *_comps[i])
	;
    _comps.insert (_comps.begin()+i+1, c);
}

void
CORBA::MultiComponent::del_component (Component *c)
{
    for (mico_vec_size_type i = 0; i < _comps.size(); ++i) {
	if (_comps[i] == c) {
	    _comps.erase (_comps.begin()+i);
	    break;
	}
    }
}

CORBA::Component *
CORBA::MultiComponent::component (ComponentId cid)
{
    for (mico_vec_size_type i = 0; i < _comps.size(); ++i) {
        if (_comps[i]->id() == cid)
            return _comps[i];
    }
    return 0;
}

void
CORBA::MultiComponent::encode (DataEncoder &ec) const
{
    CORBA::DataEncoder::EncapsState state;

    ec.seq_begin (_comps.size());
    for (mico_vec_size_type i = 0; i < _comps.size(); ++i) {
	ec.struct_begin ();
	ec.put_ulong (_comps[i]->id());
	ec.encaps_begin (state);
        _comps[i]->encode (ec);
	ec.encaps_end (state);
	ec.struct_end ();
    }
    ec.seq_end ();
}

#define check(exp) if (!(exp)) goto bad

CORBA::Boolean
CORBA::MultiComponent::decode (DataDecoder &dc)
{
    {
	free ();

	CORBA::ULong len;

	check (dc.seq_begin (len));
	for (ULong i = 0; i < len; ++i) {
            Component *comp = Component::decode (dc);
	    check (comp);
	    add_component (comp);
	}
	check (dc.seq_end ());
	return TRUE;
    }

bad:
    free ();
    return FALSE;
}

#undef check

void
CORBA::MultiComponent::print (ostream &o) const
{
  if (_comps.size() > 0) {
    o << " Components:  ";
    for (mico_vec_size_type i = 0; i < _comps.size(); ++i) {
      if (i > 0) {
	o << "              ";
      }
      _comps[i]->print (o);
    }
  }
}

CORBA::ULong
CORBA::MultiComponent::size () const
{
    return _comps.size();
}


/************************** ComponentDecoder **************************/


CORBA::ComponentDecoder::~ComponentDecoder ()
{
}


/****************************** Component *****************************/


namespace CORBA {
  vector<CORBA::ComponentDecoder *> *Component::decoders = 0;
}

CORBA::Component::~Component ()
{
}

void
CORBA::Component::copy (Component *p)
{
}

#define check(exp) if (!(exp)) goto bad

CORBA::Component *
CORBA::Component::decode (DataDecoder &dc)
{
    ULong pid, len, next_pos;
    CORBA::DataDecoder::EncapsState state;
    Component *p = 0;

    check (dc.struct_begin ());
    check (dc.get_ulong (pid));
    check (dc.encaps_begin (state, len));

    next_pos = dc.buffer()->rpos() + len;
    p = decode_body (dc, (ComponentId)pid, len);
    // seek over everthing not read by the profile decoder
    check (dc.buffer()->rseek_beg (next_pos));

    check (p);
    check (dc.encaps_end (state));
    check (dc.struct_end ());
    return p;

bad:
    if (p)
	delete p;
    return 0;
}

#undef check

CORBA::Component *
CORBA::Component::decode_body (DataDecoder &dc, ComponentId pid, ULong len)
{
    if (!decoders)
	decoders = new vector<CORBA::ComponentDecoder *>;

    ULong i;
    Component *p;

    for (i = 0; i < decoders->size(); ++i) {
	if ((*decoders)[i]->has_id ((CORBA::Component::ComponentId)pid))
	    break;
    }
    if (i < decoders->size()) {
	// known profile
	p = (*decoders)[i]->decode (dc, (CORBA::Component::ComponentId)pid,
				    len);
    } else {
	// unknown profile
	p = MICO::UnknownComponent::decode(
            dc, (CORBA::Component::ComponentId)pid, len);
    }
    return p;
}

void
CORBA::Component::register_decoder (ComponentDecoder *decoder)
{
    if (!decoders)
	decoders = new vector<CORBA::ComponentDecoder *>;
    decoders->push_back (decoder);
}

void
CORBA::Component::unregister_decoder (ComponentDecoder *decoder)
{
    if (!decoders)
	return;
    for (ULong i = 0; i < decoders->size(); ) {
	if ((*decoders)[i] == decoder)
	    decoders->erase (decoders->begin() + i);
	else
	    ++i;
    }
}


/*************************** UnknownComponent ****************************/


MICO::UnknownComponent::UnknownComponent (ComponentId id,
                                          CORBA::Octet *data,
                                          CORBA::ULong len)
    : tagid (id)
{
    if (data) {
        tagdata.insert (tagdata.end(), data, data+len);
    } else {
        // byteorder octet
        tagdata.push_back (0);
    }
}

MICO::UnknownComponent::UnknownComponent (const UnknownComponent &p)
    : tagid (p.tagid), tagdata (p.tagdata)
{
}

MICO::UnknownComponent::~UnknownComponent ()
{
}

MICO::UnknownComponent &
MICO::UnknownComponent::operator= (const UnknownComponent &p)
{
    tagid = p.tagid;
    tagdata = p.tagdata;
    return *this;
}

void
MICO::UnknownComponent::encode (CORBA::DataEncoder &ec) const
{
    // XXX vector elements need not be continuous ...
    // seek back one byte to overwrite byteorder octet
    ec.buffer()->wseek_rel (-1);
    ec.put_octets (&tagdata[0], tagdata.size());
}

MICO::UnknownComponent::ComponentId
MICO::UnknownComponent::id () const
{
    return tagid;
}

void
MICO::UnknownComponent::print (ostream &o) const
{
  char buf[10];
  CORBA::ULong j, k;

  o << "Unknown Component" << endl;
  o << "              Tag Id:  " << tagid << endl;
  o << "                 Tag:  ";

  for (j=0; j<tagdata.size(); j+=8) {
    for (k=j; k<j+8 && k<tagdata.size(); k++) {
      sprintf (buf, "%02x ", (int) tagdata[k]);
      o << buf;
    }
    for (; k<j+8; k++) {
      sprintf (buf, "   ");
      o << buf;
    }
    for (k=j; k<j+8 && k<tagdata.size(); k++) {
      if (isprint (tagdata[k])) {
	o << (char) tagdata[k];
      }
      else {
	o << '.';
      }
    }
    o << endl;
    
    if (k < tagdata.size()) {
      o << "                       ";
    }
  }
}

CORBA::Component *
MICO::UnknownComponent::clone () const
{
    return new UnknownComponent (*this);
}

CORBA::Long
MICO::UnknownComponent::compare (const CORBA::Component &p) const
{
    if (id() != p.id())
	return (CORBA::Long)id() - (CORBA::Long)p.id();

    UnknownComponent &up = (UnknownComponent &)p;
    return mico_vec_compare (tagdata, up.tagdata);
}

CORBA::Boolean
MICO::UnknownComponent::operator== (const CORBA::Component &p) const
{
    return compare (p) == 0;
}

CORBA::Boolean
MICO::UnknownComponent::operator< (const CORBA::Component &p) const
{
    return compare (p) < 0;
}

MICO::UnknownComponent *
MICO::UnknownComponent::decode (CORBA::DataDecoder &dc, ComponentId id,
                                CORBA::ULong len)
{
    if (len > 10000)
	// this must be garbage
	return 0;

    if (dc.buffer()->length() < len)
        return 0;

    UnknownComponent *p = new UnknownComponent (id);

    p->tagdata.erase (p->tagdata.begin(), p->tagdata.end());
    // include byteorder octet from encapsulated sequence
    p->tagdata.insert (p->tagdata.begin(),
		       dc.buffer()->data() - 1, dc.buffer()->data() + len);
    dc.buffer()->rseek_rel (len);

    return p;
}


/************************* MultiCompProfile **************************/


MICO::MultiCompProfile::MultiCompProfile (const CORBA::MultiComponent &mc,
					  ProfileId tagid)
    : _mc (mc), _tagid (tagid)
{
}

MICO::MultiCompProfile::MultiCompProfile (const MultiCompProfile &mcp)
    : _mc (mcp._mc), _tagid (mcp._tagid)
{
}

MICO::MultiCompProfile::~MultiCompProfile ()
{
}

MICO::MultiCompProfile &
MICO::MultiCompProfile::operator= (const MultiCompProfile &mcp)
{
    if (this != &mcp) {
	_mc = mcp._mc;
	_tagid = mcp._tagid;
    }
    return *this;
}

void
MICO::MultiCompProfile::encode (CORBA::DataEncoder &ec) const
{
    _mc.encode (ec);
}

const CORBA::Address *
MICO::MultiCompProfile::addr () const
{
    assert (0);
    return 0;
}

MICO::MultiCompProfile::ProfileId
MICO::MultiCompProfile::id () const
{
    return _tagid;
}

MICO::MultiCompProfile::ProfileId
MICO::MultiCompProfile::encode_id () const
{
    return _tagid;
}

void
MICO::MultiCompProfile::objectkey (CORBA::Octet *, CORBA::Long length)
{
}

const CORBA::Octet *
MICO::MultiCompProfile::objectkey (CORBA::Long &length) const
{
    length = 0;
    return (const CORBA::Octet *)"";
}

CORBA::Boolean
MICO::MultiCompProfile::reachable ()
{
    return FALSE;
}

void
MICO::MultiCompProfile::print (ostream &o) const
{
  o << "Multiple Components Profile" << endl;
  _mc.print (o);
}

CORBA::IORProfile *
MICO::MultiCompProfile::clone () const
{
    return new MultiCompProfile (*this);
}

CORBA::Long
MICO::MultiCompProfile::compare (const CORBA::IORProfile &p) const
{
    if (id() != p.id())
	return (CORBA::Long)id() - (CORBA::Long)p.id();

    MultiCompProfile &mcp = (MultiCompProfile &)p;

    return _mc.compare (mcp._mc);
}

CORBA::Boolean
MICO::MultiCompProfile::operator== (const CORBA::IORProfile &p) const
{
    return compare (p) == 0;
}

CORBA::Boolean
MICO::MultiCompProfile::operator< (const CORBA::IORProfile &p) const
{
    return compare (p) < 0;
}

CORBA::MultiComponent *
MICO::MultiCompProfile::components()
{
    return &_mc;
}


/********************** MultiCompProfileDecoder ***********************/


MICO::MultiCompProfileDecoder::MultiCompProfileDecoder (
    CORBA::IORProfile::ProfileId id)
{
    _tagid = id;
    CORBA::IORProfile::register_decoder (this);
}

MICO::MultiCompProfileDecoder::~MultiCompProfileDecoder ()
{
    CORBA::IORProfile::unregister_decoder (this);
}

CORBA::IORProfile *
MICO::MultiCompProfileDecoder::decode (CORBA::DataDecoder &dc,
				       ProfileId id, CORBA::ULong) const
{
    CORBA::MultiComponent mc;
    if (!mc.decode (dc))
	return 0;

    return new MultiCompProfile (mc, id);
}

CORBA::Boolean
MICO::MultiCompProfileDecoder::has_id (ProfileId id) const
{
    return id == _tagid;
}

static MICO::MultiCompProfileDecoder multicomp_profile_decoder;
