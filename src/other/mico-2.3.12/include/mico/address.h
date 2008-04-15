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

#ifndef __mico_address_h__
#define __mico_address_h__

namespace CORBA {

/************************* ForwardDecls ******************************/


class Address;
class AddressParser;
class Transport;
class TransportServer;
class Buffer;
class IORProfile;
class MultiComponent;


/************************** Interfaces *******************************/


class AddressParser {
public:
    virtual Address *parse (const char *rest, const char *proto) const = 0;
    virtual CORBA::Boolean has_proto (const char *) const = 0;

    virtual ~AddressParser ();
};

class Address {
    static std::vector<AddressParser *> *parsers;
protected:
    void copy (Address *);
public:
    static Address *parse (const char *);
    static void register_parser (AddressParser *);
    static void unregister_parser (AddressParser *);

    virtual std::string stringify () const = 0;
    virtual const char *proto () const = 0;
    virtual Transport *make_transport () const = 0;
    virtual TransportServer *make_transport_server () const = 0;
    virtual IORProfile *make_ior_profile (
	Octet *, ULong, const CORBA::MultiComponent &,
        CORBA::UShort version = 0x0100) const = 0;
    virtual Boolean is_local () const = 0;
    virtual Boolean is_here () const = 0;

    virtual Address *clone () const = 0;

    virtual Long compare (const Address &) const = 0;
    virtual Boolean operator== (const Address &) const = 0;
    virtual Boolean operator< (const Address &) const = 0;

    virtual ~Address ();
};

}

#endif // __mico_address_h__
