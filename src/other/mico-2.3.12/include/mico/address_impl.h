// -*- c++ -*-
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

#ifndef __mico_address_impl_h__
#define __mico_address_impl_h__

namespace MICO {

class LocalAddress : public CORBA::Address {
public:
    LocalAddress ();
    ~LocalAddress ();

    std::string stringify () const;
    const char *proto () const;
    CORBA::Transport *make_transport () const;
    CORBA::TransportServer *make_transport_server () const;
    CORBA::IORProfile *make_ior_profile (
	CORBA::Octet *, CORBA::ULong, const CORBA::MultiComponent &,
        CORBA::UShort version = 0x0100) const;
    CORBA::Boolean is_local () const;
    CORBA::Boolean is_here () const;

    CORBA::Address *clone () const;
    
    CORBA::Long compare (const CORBA::Address &) const;
    CORBA::Boolean operator== (const CORBA::Address &) const;
    CORBA::Boolean operator< (const CORBA::Address &) const;
};

class LocalAddressParser : public CORBA::AddressParser {
public:
    LocalAddressParser ();
    ~LocalAddressParser ();
    CORBA::Address *parse (const char *, const char *) const;
    CORBA::Boolean has_proto (const char *) const;
};


class InetAddress : public CORBA::Address {
public:
    enum Family {
	STREAM,
	DGRAM
    };
private:
    CORBA::UShort _port;
    std::string _host;
    std::vector<CORBA::Octet> _ipaddr;
    Family _family;
    static CORBA::Boolean _resolve;

    static std::string hname;
    static std::vector<CORBA::Octet> hid;

    CORBA::Boolean resolve_ip () const;
    CORBA::Boolean resolve_host () const;
    // kcg: temporary lock for gethostbyname/gethostbyaddr
    // calls serialization. InetAddress and Transports
    // should be rewritten to use getaddrinfo/getnameinfo
    // functions
    static MICOMT::Mutex S_netdb_lock_;
public:
    InetAddress (struct sockaddr_in &sin, Family = STREAM);
    InetAddress (const char *host = NULL, CORBA::UShort port = 0,
		 Family = STREAM);
    InetAddress (const std::vector<CORBA::Octet> &, CORBA::UShort port = 0,
		 Family = STREAM);
    ~InetAddress ();

    std::string stringify () const;
    const char *proto () const;
    CORBA::Transport *make_transport () const;
    CORBA::TransportServer *make_transport_server () const;
    CORBA::IORProfile *make_ior_profile (
	CORBA::Octet *, CORBA::ULong, const CORBA::MultiComponent &,
        CORBA::UShort version = 0x0100) const;
    CORBA::Boolean is_local () const;
    CORBA::Boolean is_here () const;
    
    CORBA::Address *clone () const;
    
    CORBA::Long compare (const CORBA::Address &) const;
    CORBA::Boolean operator== (const CORBA::Address &) const;
    CORBA::Boolean operator< (const CORBA::Address &) const;

    CORBA::Boolean valid () const;

    const char *host () const;
    void host (const char *);

    const std::vector<CORBA::Octet> &ipaddr () const;
    void ipaddr (const std::vector<CORBA::Octet> &);

    CORBA::UShort port () const;
    void port (CORBA::UShort port);

    void sockaddr (struct sockaddr_in &);
    struct sockaddr_in sockaddr () const;

    void family (Family f);
    Family family () const;

    static std::string hostname ();
    static std::vector<CORBA::Octet> hostid();
    static CORBA::Boolean samehosts (const std::string &h1,
				     const std::string &h2);

    static CORBA::Boolean resolve ();
    static void resolve (CORBA::Boolean);
};

class InetAddressParser : public CORBA::AddressParser {
public:
    InetAddressParser ();
    ~InetAddressParser ();
    CORBA::Address *parse (const char *, const char *) const;
    CORBA::Boolean has_proto (const char *) const;
};

#ifdef HAVE_SYS_UN_H
class UnixAddress : public CORBA::Address {
    std::string _filename;
public:
    UnixAddress (struct sockaddr_un &una);
    UnixAddress (const char *filename = 0);
    ~UnixAddress ();

    std::string stringify () const;
    const char *proto () const;
    CORBA::Transport *make_transport () const;
    CORBA::TransportServer *make_transport_server () const;
    CORBA::IORProfile *make_ior_profile (
	CORBA::Octet *, CORBA::ULong, const CORBA::MultiComponent &,
        CORBA::UShort version = 0x0100) const;
    CORBA::Boolean is_local () const;
    CORBA::Boolean is_here () const;
    
    CORBA::Address *clone () const;
    
    CORBA::Long compare (const CORBA::Address &) const;
    CORBA::Boolean operator== (const CORBA::Address &) const;
    CORBA::Boolean operator< (const CORBA::Address &) const;

    const char *filename () const;
    void filename (const char *);

    void sockaddr (struct sockaddr_un &);
    struct sockaddr_un sockaddr () const;
};

class UnixAddressParser : public CORBA::AddressParser {
public:
    UnixAddressParser ();
    ~UnixAddressParser ();
    CORBA::Address *parse (const char *, const char *) const;
    CORBA::Boolean has_proto (const char *) const;
};
#endif // HAVE_SYS_UN_H

}

#endif // __mico_address_impl_h__
