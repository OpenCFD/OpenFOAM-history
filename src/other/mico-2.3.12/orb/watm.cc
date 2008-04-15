/*
 *  MIWCO --- Wireless CORBA support for MICO (OMG document formal/2004-04-02)
 *  Copyright (C) 2001 Jaakko Kangasharju
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
 */

#define MICO_CONF_IMR
#define MICO_CONF_DYNANY

#include <string.h>
#include <CORBA.h>
#include <mico/util.h>
#include <mico/impl.h>
#include <mico/watm.h>
#include <mico/throw.h>


using namespace std;

void
WATM::_init ()
{
    CORBA::ComponentDecoder *home_location_decoder = new HomeLocationDecoder;
    CORBA::Component::register_decoder(home_location_decoder);
    CORBA::IORProfileDecoder *watm_profile_decoder = new WATMProfileDecoder;
    CORBA::IORProfile::register_decoder(watm_profile_decoder);
}

/************************* HomeLocationComponent ************************/

WATM::HomeLocationComponent::HomeLocationComponent (CORBA::Object_ptr obj)
{
    home = CORBA::Object::_duplicate(obj);
}

WATM::HomeLocationComponent::HomeLocationComponent
(const HomeLocationComponent &c)
{
    home = CORBA::Object::_duplicate(c.home);
}

WATM::HomeLocationComponent::~HomeLocationComponent ()
{
    CORBA::release(home);
}

WATM::HomeLocationComponent &
WATM::HomeLocationComponent::operator= (const HomeLocationComponent &c)
{
    if (this != &c) {
	CORBA::release(home);
	home = CORBA::Object::_duplicate(c.home);
    }
    return *this;
}

void
WATM::HomeLocationComponent::encode (CORBA::DataEncoder &ec) const
{
    ec.struct_begin();
    {
	ec.put_ior(*home->_ior());
    }
    ec.struct_end();
}

void
WATM::HomeLocationComponent::print (ostream &s) const
{
    CORBA::ORB_var orb = CORBA::ORB_instance("mico-local-orb");
    s << "Home Location: ";
    s << orb->object_to_string(home);
    s << endl;
}

CORBA::Long
WATM::HomeLocationComponent::compare (const CORBA::Component &c) const
{
    if (id() != c.id())
	return (CORBA::Long)id() - (CORBA::Long)c.id();

    const HomeLocationComponent &hlc = (const HomeLocationComponent &)c;

    return home->_ior()->compare(*hlc.home->_ior());
}

/************************** HomeLocationDecoder *************************/

#define check(exp) if (!(exp)) return 0

CORBA::Component *
WATM::HomeLocationDecoder::decode (CORBA::DataDecoder &dc,
				       ComponentId id, CORBA::ULong len) const
{
    CORBA::IOR *ior = new CORBA::IOR("", CORBA::IOR::IORProfileVec());

    check (dc.struct_begin());
    {
	check(dc.get_ior(*ior));
    }
    check (dc.struct_end());
    CORBA::ORB_var orb = CORBA::ORB_instance("mico-local-orb");
    CORBA::Object_var obj = orb->ior_to_object(ior);

    return new HomeLocationComponent(obj);
}

#undef check

/****************************** WATMProfile *****************************/

WATM::WATMProfile::WATMProfile (const CORBA::Octet *key, CORBA::ULong len,
				const CORBA::Octet *tid, CORBA::ULong tlen,
				const CORBA::MultiComponent &temp_mc,
				CORBA::UShort ver, ProfileId id)
    : tagid(id), version(ver)
{
    CORBA::MultiComponent mc = temp_mc;
    object_key = new CORBA::Octet[keylen = len];
    memcpy(object_key, key, keylen);
    terminal_id = new CORBA::Octet[tidlen = tlen];
    memcpy(terminal_id, tid, tidlen);

    if (mc.component(CORBA::Component::TAG_HOME_LOCATION_INFO) == NULL) {
	CORBA::ORB_var orb = CORBA::ORB_instance("mico-local-orb");
	try {
	    CORBA::Object_var obj =
		orb->resolve_initial_references("HomeLocationAgent");
	    mc.add_component(new HomeLocationComponent(obj));
	} catch (const CORBA::ORB::InvalidName &) {
	    // We are homeless
	}
    }
    comps = mc;
    my_addr = NULL;
}

WATM::WATMProfile::WATMProfile (const WATMProfile &wp)
{
    version = wp.version;
    tagid = wp.tagid;
    comps = wp.comps;
    terminal_id = new CORBA::Octet[tidlen = wp.tidlen];
    memcpy(terminal_id, wp.terminal_id, tidlen);
    object_key = new CORBA::Octet[keylen = wp.keylen];
    memcpy(object_key, wp.object_key, keylen);
}

WATM::WATMProfile::~WATMProfile ()
{
    delete[] object_key;
    delete[] terminal_id;
}

WATM::WATMProfile &
WATM::WATMProfile::operator= (const WATMProfile &wp)
{
    if (this != &wp) {
	version = wp.version;
	tagid = wp.tagid;
	comps = wp.comps;
	terminalid(wp.terminal_id, wp.tidlen);
	objectkey(wp.object_key, wp.keylen);
    }
    return *this;
}

void
WATM::WATMProfile::encode (CORBA::DataEncoder &ec) const
{
    ec.struct_begin();
    {
	ec.struct_begin();
	{
	    ec.put_octet((CORBA::Octet)(version >> 8));
	    ec.put_octet((CORBA::Octet)(version & 0xff));
	}
	ec.struct_end();

	ec.put_octet(0);

	ec.seq_begin(tidlen);
	{
	    ec.put_octets(terminal_id, tidlen);
	}
	ec.seq_end();

	ec.seq_begin(keylen);
	{
	    ec.put_octets(object_key, keylen);
	}
	ec.seq_end();

	comps.encode(ec);
    }
    ec.struct_end();
}

const CORBA::Address *
WATM::WATMProfile::addr () const
{
    return my_addr;
}

WATM::WATMProfile::ProfileId
WATM::WATMProfile::id () const
{
    return tagid;
}

WATM::WATMProfile::ProfileId
WATM::WATMProfile::encode_id () const
{
    return tagid;
}

void
WATM::WATMProfile::objectkey (CORBA::Octet *key, CORBA::Long len)
{
    delete[] object_key;
    object_key = new CORBA::Octet[keylen = len];
    memcpy(object_key, key, keylen);
}

const CORBA::Octet *
WATM::WATMProfile::objectkey (CORBA::Long &len) const
{
    len = keylen;
    return object_key;
}

void
WATM::WATMProfile::terminalid (CORBA::Octet *tid, CORBA::Long len)
{
    delete[] terminal_id;
    terminal_id = new CORBA::Octet[tidlen = len];
    memcpy(terminal_id, tid, tidlen);
}

const CORBA::Octet *
WATM::WATMProfile::terminalid (CORBA::Long &len) const
{
    len = tidlen;
    return terminal_id;
}

CORBA::Boolean
WATM::WATMProfile::reachable ()
{
    return FALSE;
}

void
WATM::WATMProfile::print (ostream &s) const
{
    s << "WATM Profile" << endl;
    s << "    Version:  " << (CORBA::Long)(version >> 8) << "."
      << (CORBA::Long)(version & 0xff) << endl;
    CORBA::String_var temp = mico_url_encode(terminal_id, tidlen);
    s << "    Terminal: " << temp.in() << endl;
    temp = mico_url_encode(object_key, keylen);
    s << "    Object:   " << temp << endl;

    comps.print(s);
}

CORBA::IORProfile *
WATM::WATMProfile::clone () const
{
    return new WATMProfile(*this);
}

CORBA::Long
WATM::WATMProfile::compare (const CORBA::IORProfile &p) const
{
    if (id() != p.id())
	return (CORBA::Long)id() - (CORBA::Long)p.id();

    const WATMProfile &wp = (const WATMProfile &) p;

    if (keylen != wp.keylen)
	return (CORBA::Long)keylen - (CORBA::Long)wp.keylen;

    CORBA::Long r = mico_key_compare(object_key, wp.object_key, keylen);
    if (r)
	return r;

    if (version != wp.version)
	return (CORBA::Long)version - (CORBA::Long)wp.version;

    return comps.compare(wp.comps);
}

CORBA::Boolean
WATM::WATMProfile::operator== (const CORBA::IORProfile &p) const
{
    return compare(p) == 0;
}

CORBA::Boolean
WATM::WATMProfile::operator< (const CORBA::IORProfile &p) const
{
    return compare(p) < 0;
}

/*************************** WATMProfileDecoder *************************/

WATM::WATMProfileDecoder::WATMProfileDecoder (CORBA::IORProfile::ProfileId id)
{
    tagid = id;
}

#define check(exp) if (!(exp)) goto bad

CORBA::IORProfile *
WATM::WATMProfileDecoder::decode (CORBA::DataDecoder &dc, ProfileId,
				  CORBA::ULong) const
{
    CORBA::Octet major, minor;
    CORBA::Octet *key, *tid;
    CORBA::ULong klen, tlen;
    CORBA::UShort version;
    CORBA::MultiComponent comps;
    CORBA::IORProfile *wp = 0;

    check(dc.struct_begin());
    {
	check(dc.struct_begin());
	{
	    check(dc.get_octet(major));
	    check(dc.get_octet(minor));
	    version = ((major << 8) | minor);
	    check(version == 0x0100);
	}
	check(dc.struct_end());

	check(dc.seq_begin(tlen));
	{
	    check(dc.buffer()->length() >= tlen);
	    tid = dc.buffer()->data();
	    dc.buffer()->rseek_rel(tlen);
	}
	check(dc.seq_end());

	check(dc.seq_begin(klen));
	{
	    check(dc.buffer()->length() >= klen);
	    key = dc.buffer()->data();
	    dc.buffer()->rseek_rel(klen);
	}
	check(dc.seq_end());

	check(comps.decode(dc));

	wp = new WATMProfile(key, klen, tid, tlen, comps, version, tagid);
    }
    check(dc.struct_end());
    return wp;

  bad:
    if (wp)
	delete(wp);
    return 0;
}

#undef check

CORBA::Boolean
WATM::WATMProfileDecoder::has_id (ProfileId id) const
{
    return tagid == id;
}
