// -*- c++ -*-
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

#ifndef __mico_watm_h__
#define __mico_watm_h__

namespace WATM {

extern void _init ();

class HomeLocationComponent : public CORBA::Component {
    CORBA::Object_ptr home;
public:
    HomeLocationComponent (CORBA::Object_ptr);
    HomeLocationComponent (const HomeLocationComponent &);
    ~HomeLocationComponent ();
    HomeLocationComponent &operator= (const HomeLocationComponent &);

    void encode (CORBA::DataEncoder &) const;
    ComponentId id () const { return TAG_HOME_LOCATION_INFO; }
    void print (std::ostream &) const;

    CORBA::Component *clone () const
	{ return new HomeLocationComponent (*this); }
    CORBA::Long compare (const CORBA::Component &) const;
    CORBA::Boolean operator== (const Component &c) const
	{ return compare (c) == 0; }
    CORBA::Boolean operator< (const Component &c) const
	{ return compare (c) < 0; }

    CORBA::Object_ptr hla () const
	{ return home; }
};

class HomeLocationDecoder : public CORBA::ComponentDecoder {
public:
    HomeLocationDecoder () {}

    ~HomeLocationDecoder () {}

    CORBA::Component *decode (CORBA::DataDecoder &, ComponentId,
			      CORBA::ULong) const;
    CORBA::Boolean has_id (ComponentId id) const
	{ return id == CORBA::Component::TAG_HOME_LOCATION_INFO; }
};

class WATMProfile : public CORBA::IORProfile {
    CORBA::Octet *object_key;
    CORBA::ULong keylen;
    CORBA::Octet *terminal_id;
    CORBA::ULong tidlen;
    ProfileId tagid;
    CORBA::UShort version;
    CORBA::MultiComponent comps;
    CORBA::Address *my_addr;
public:
    WATMProfile (const CORBA::Octet *key, CORBA::ULong klen,
		 const CORBA::Octet *tid, CORBA::ULong tlen,
		 const CORBA::MultiComponent &,
		 CORBA::UShort ver = 0x0100,
		 ProfileId = TAG_MOBILE_TERMINAL_IOP);
    WATMProfile (const WATMProfile &);
    ~WATMProfile ();
    WATMProfile &operator= (const WATMProfile &);

    void encode (CORBA::DataEncoder &) const;
    const CORBA::Address *addr () const;
    ProfileId id () const;
    ProfileId encode_id () const;
    void objectkey (CORBA::Octet *, CORBA::Long);
    const CORBA::Octet *objectkey (CORBA::Long &) const;
    void terminalid (CORBA::Octet *, CORBA::Long);
    const CORBA::Octet *terminalid (CORBA::Long &) const;
    CORBA::Boolean reachable ();
    void print (std::ostream &) const;

    CORBA::IORProfile *clone () const;
    CORBA::Long compare (const CORBA::IORProfile &) const;
    CORBA::Boolean operator== (const CORBA::IORProfile &) const;
    CORBA::Boolean operator< (const CORBA::IORProfile &) const;

    CORBA::MultiComponent *components ()
	{ return &comps; }
};

class WATMProfileDecoder : public CORBA::IORProfileDecoder {
    CORBA::IORProfile::ProfileId tagid;
public:
    WATMProfileDecoder (CORBA::IORProfile::ProfileId
			= CORBA::IORProfile::TAG_MOBILE_TERMINAL_IOP);
    ~WATMProfileDecoder () {};

    CORBA::IORProfile *decode (CORBA::DataDecoder &, ProfileId,
			       CORBA::ULong) const;
    CORBA::Boolean has_id (ProfileId) const;
};

}

#endif // __mico_watm_h__
