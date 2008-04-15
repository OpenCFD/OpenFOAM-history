// -*- c++ -*-
/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
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

#ifndef __mico_ior_h__
#define __mico_ior_h__

namespace CORBA {

/************************* ForwardDecls ******************************/


class IOR;
class IORProfile;
class IORProfileDecoder;
class Component;
class ComponentDecoder;
class DataDecoder;
class DataEncoder;
class Address;


/************************** Interfaces *******************************/


class Component {
    static std::vector<ComponentDecoder *> *decoders;
protected:
    void copy (Component *);
public:
    typedef CORBA::ULong ComponentId;
    enum {
	TAG_CODE_SETS = 1,
	TAG_SSL_SEC_TRANS = 20, // IIOP over SSL
	TAG_CSI_SEC_MECH_LIST = 33, // CSIv2 mechanism list
	TAG_HOME_LOCATION_INFO = 44 // Wireless CORBA HLA IOR
    };

    static Component *decode (DataDecoder &);
    static Component *decode_body (DataDecoder &, ComponentId, ULong len);
    static void register_decoder (ComponentDecoder *);
    static void unregister_decoder (ComponentDecoder *);

    virtual void encode (DataEncoder &) const = 0;
    virtual ComponentId id () const = 0;
    virtual void print (std::ostream &) const = 0;

    virtual Component *clone () const = 0;
    virtual Long compare (const Component &) const = 0;
    virtual Boolean operator== (const Component &) const = 0;
    virtual Boolean operator< (const Component &) const = 0;

    virtual ~Component ();
};

class ComponentDecoder {
public:
    typedef Component::ComponentId ComponentId;

    virtual Component *decode (DataDecoder &, ComponentId, ULong) const = 0;
    virtual CORBA::Boolean has_id (ComponentId) const = 0;

    virtual ~ComponentDecoder ();
};

class MultiComponent {
    std::vector<Component *> _comps;
    void free ();
    void copy (const MultiComponent &);
public:
    typedef Component::ComponentId ComponentId;

    MultiComponent ();
    MultiComponent (const MultiComponent &);
    MultiComponent &operator= (const MultiComponent &);
    ~MultiComponent ();

    CORBA::Long compare (const MultiComponent &) const;
    CORBA::Boolean operator< (const MultiComponent &) const;
    CORBA::Boolean operator== (const MultiComponent &) const;

    void add_component (Component *);
    void del_component (Component *);
    Component *component (ComponentId);

    void encode (DataEncoder &) const;
    CORBA::Boolean decode (DataDecoder &);
    void print (std::ostream &) const;
    CORBA::ULong size () const;
};



class IORProfile {
    static std::vector<IORProfileDecoder *> *decoders;
protected:
    void copy (IORProfile *);
public:
    typedef CORBA::ULong ProfileId;
    enum {
	TAG_INTERNET_IOP = 0,
	TAG_MULTIPLE_COMPONENTS = 1,
	TAG_MOBILE_TERMINAL_IOP = 4, // Profile from Wireless CORBA
	TAG_LTP_IOP = 9001,   // Bluetooth profile
	TAG_LOCAL = 20000,    // mico-extension
	TAG_ANY = 20001,      // mico-extension
	TAG_UNIX_IOP = 20002, // mico-extension
	TAG_SSL_INTERNET_IOP = 20002, // mico-extension
        TAG_SSL_UNIX_IOP = 20003, // mico-extension
	TAG_UDP_IOP = 20004, // mico-extension
	TAG_SSL_UDP_IOP = 20005 // mico-extension
    };

    static IORProfile *decode (DataDecoder &);
    static IORProfile *decode_body (DataDecoder &, ProfileId, ULong len);
    static void register_decoder (IORProfileDecoder *);
    static void unregister_decoder (IORProfileDecoder *);

    virtual void encode (DataEncoder &) const = 0;
    virtual const Address *addr () const = 0;
    virtual ProfileId id () const = 0;
    virtual ProfileId encode_id () const = 0;
    virtual void objectkey (Octet *, Long length) = 0;
    virtual const Octet *objectkey (Long &length) const = 0;
    virtual Boolean reachable () = 0;
    virtual void print (std::ostream &) const = 0;
    virtual CORBA::MultiComponent *components ();
    virtual void prepare_mobile () {}

    virtual IORProfile *clone () const = 0;
    virtual Long compare (const IORProfile &) const = 0;
    virtual Boolean operator== (const IORProfile &) const = 0;
    virtual Boolean operator< (const IORProfile &) const = 0;

    virtual ~IORProfile ();
};

class IORProfileDecoder {
public:
    typedef IORProfile::ProfileId ProfileId;

    virtual IORProfile *decode (DataDecoder &, ProfileId, ULong) const = 0;
    virtual CORBA::Boolean has_id (ProfileId) const = 0;

    virtual ~IORProfileDecoder ();
};

class IOR {
public:
    typedef std::vector<IORProfile *> IORProfileVec;

private:
    std::string oid;
    IORProfileVec tags;

    void free ();
    void copy (const IOR &);

    IORProfile * _active_profile;
    CORBA::ULong _active_profile_index;
    GIOP::AddressingDisposition _addressing_disposition;

public:
    IOR ();
    IOR (const char *objid, const IORProfileVec &tags);
    IOR (DataDecoder &);
    IOR (const char *); // uses CDRDecoder
    IOR (const IOR &);
    ~IOR ();
    IOR &operator= (const IOR &);
    Boolean from_string (const char *);
    Long compare (const IOR &) const;
    Long compare_reachable (const IOR &) const;
    Boolean operator== (const IOR &) const;
    Boolean operator< (const IOR &) const;

    const Address *addr (IORProfile::ProfileId = IORProfile::TAG_ANY,
			 Boolean find_unreachable = FALSE,
                         const Address *prev = 0) const;
    IORProfile *profile (IORProfile::ProfileId = IORProfile::TAG_ANY,
			 Boolean find_unreachable = FALSE,
                         IORProfile *prev = 0);
    void add_profile (IORProfile *);
    void del_profile (IORProfile *);
    IORProfile *get_profile (ULong);

    IORProfile * active_profile (CORBA::ULong * index = 0);
    void active_profile (IORProfile *);

    GIOP::AddressingDisposition addressing_disposition ();
    void addressing_disposition (GIOP::AddressingDisposition);

    const char *objid () const;
    void objid (const char *);
    void objectkey (Octet *, ULong len);
    void encode (DataEncoder &) const;
    Boolean decode (DataDecoder &);
    std::string stringify () const; // uses CDREncoder
    void print (std::ostream &) const;
};

}

#endif // __mico_ior_h__

