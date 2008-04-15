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

#include <CORBA-SMALL.h>
#ifndef _WIN32
#include <string.h>
#endif
#include <mico/os-net.h>
#include <mico/impl.h>
#include <mico/template_impl.h>
#include <mico/util.h>

#endif // FAST_PCH


using namespace std;


//
// static data initialization
//
MICOMT::Mutex MICO::InetAddress::S_netdb_lock_;

/************************* misc dtors *******************************/


CORBA::AddressParser::~AddressParser ()
{
}

CORBA::Address::~Address ()
{
}


/*************************** Address ********************************/


namespace CORBA {
  vector<CORBA::AddressParser *> *Address::parsers = 0;
}

void
CORBA::Address::copy (Address *a)
{
}

CORBA::Address *
CORBA::Address::parse (const char *_a)
{
    if (!parsers)
	return 0;

    string rest, proto, a = _a;

    Long pos = a.find (":");
    if (pos < 0) {
	proto = a;
	rest = "";
    } else {
	proto = a.substr (0, pos);
	rest = a.substr (pos+1);
    }

    ULong i;
    for (i = 0; i < parsers->size(); ++i) {
	if ((*parsers)[i]->has_proto(proto.c_str()))
	    break;
    }
    if (i >= parsers->size())
	return 0;
    Address *addr = (*parsers)[i]->parse (rest.c_str(), proto.c_str());
    return addr;
}

void
CORBA::Address::register_parser (AddressParser *ap)
{
    if (!parsers)
	parsers = new vector<CORBA::AddressParser *>;
    parsers->push_back (ap);
}

void
CORBA::Address::unregister_parser (AddressParser *ap)
{
    if (!parsers)
	return;
    
    for (ULong i = 0; i < parsers->size(); ) {
	if ((*parsers)[i] == ap)
	    parsers->erase (parsers->begin() + i);
	else
	    ++i;
    }
}


/****************************** LocalAddress ****************************/


MICO::LocalAddress::LocalAddress ()
{
}

MICO::LocalAddress::~LocalAddress ()
{
}

string
MICO::LocalAddress::stringify () const
{
    return "local:";
}

const char *
MICO::LocalAddress::proto () const
{
    return "local";
}

CORBA::Transport *
MICO::LocalAddress::make_transport () const
{
    assert (0);
    return NULL;
}

CORBA::TransportServer *
MICO::LocalAddress::make_transport_server () const
{
    assert (0);
    return NULL;
}

CORBA::IORProfile *
MICO::LocalAddress::make_ior_profile (CORBA::Octet *key,
				      CORBA::ULong keylen,
				      const CORBA::MultiComponent &,
                                      CORBA::UShort version) const
{
    return new LocalProfile (key, keylen);
}

CORBA::Boolean
MICO::LocalAddress::is_local () const
{
    return TRUE;
}

CORBA::Boolean
MICO::LocalAddress::is_here () const
{
    return TRUE;
}

CORBA::Address *
MICO::LocalAddress::clone () const
{
    return new LocalAddress;
}

CORBA::Long
MICO::LocalAddress::compare (const CORBA::Address &a) const
{
    return strcmp (proto(), a.proto());
}

CORBA::Boolean
MICO::LocalAddress::operator== (const CORBA::Address &a) const
{
    return compare (a) == 0;
}

CORBA::Boolean
MICO::LocalAddress::operator< (const CORBA::Address &a) const
{
    return compare (a) < 0;
}


/*************************** LocalAddressParser *************************/


MICO::LocalAddressParser::LocalAddressParser ()
{
    CORBA::Address::register_parser (this);
}

MICO::LocalAddressParser::~LocalAddressParser ()
{
    CORBA::Address::unregister_parser (this);
}

CORBA::Address *
MICO::LocalAddressParser::parse (const char *a, const char *) const
{
    return new LocalAddress;
}

CORBA::Boolean
MICO::LocalAddressParser::has_proto (const char *p) const
{
    return !strcmp ("local", p);
}

static MICO::LocalAddressParser local_address_parser;


/****************************** InetAddress *****************************/


CORBA::Boolean MICO::InetAddress::_resolve = TRUE;

MICO::InetAddress::InetAddress (const char *host, CORBA::UShort port,
				Family fam)
    : _port (port), _host (host ? host : ""), _family (fam)
{
}

MICO::InetAddress::InetAddress (const vector<CORBA::Octet> &ip,
				CORBA::UShort port, Family fam)
    : _port (port), _ipaddr (ip), _family (fam)
{
}

MICO::InetAddress::InetAddress (struct sockaddr_in &sin, Family fam)
    : _family (fam)
{
    sockaddr (sin);
}

MICO::InetAddress::~InetAddress ()
{
}

CORBA::Boolean
MICO::InetAddress::resolve_ip () const
{
    if (_ipaddr.size() > 0)
	return TRUE;
    if (_host.length() == 0)
	return FALSE;

    InetAddress &me = (InetAddress &)*this;

    CORBA::ULong addr = ::inet_addr ((char *)_host.c_str());
    if (addr != (CORBA::ULong)-1L || _host == string("255.255.255.255")) {
	me._ipaddr.insert (me._ipaddr.begin(),
			   (CORBA::Octet *)&addr,
			   (CORBA::Octet *)&addr + sizeof (CORBA::ULong));
	return TRUE;
    }
#ifndef HAVE_THREADS
    struct hostent *hent = ::gethostbyname ((char *)_host.c_str());
#else // HAVE_THREADS
    struct hostent* hent = NULL;
    {
        MICOMT::AutoLock lock(S_netdb_lock_);
        hent = ::gethostbyname ((char *)_host.c_str());
#endif // HAVE_THREADS
    if (hent) {
        me._ipaddr.insert (me._ipaddr.begin(),
                           (CORBA::Octet *)hent->h_addr,
                           (CORBA::Octet *)hent->h_addr + hent->h_length);
        return TRUE;
    }
#ifdef HAVE_THREADS
    }
#endif // HAVE_THREADS

    if (MICO::Logger::IsLogged (MICO::Logger::Warning)) {
      MICOMT::AutoDebugLock __lock;
      MICO::Logger::Stream (MICO::Logger::Warning)
	<< "Warning: cannot resolve hostname '" << _host
	<< "' into an IP address." << endl;
    }
    return FALSE;
}

CORBA::Boolean
MICO::InetAddress::resolve_host () const
{
    if (_host.length() > 0)
	return TRUE;
    if (_ipaddr.size() == 0)
	return FALSE;

    InetAddress &me = (InetAddress &)*this;

    if (_resolve) {
	// XXX vector elements need not be continuous ?!
	assert (_ipaddr.size() < 2 || &_ipaddr[0] + 1 == &_ipaddr[1]);
#ifdef HAVE_THREADS
        struct hostent* hent = NULL;
        {
            MICOMT::AutoLock lock(S_netdb_lock_);
            hent = ::gethostbyaddr ((char *)&_ipaddr.front(),
                                    _ipaddr.size(), AF_INET);
#else // HAVE_THREADS
	struct hostent *hent = ::gethostbyaddr ((char *)&_ipaddr.front(),
						_ipaddr.size(), AF_INET);
#endif // HAVE_THREADS
	if (hent) {
	    string s = hent->h_name;
	    if ((int)s.find (".") < 0) {
		// official name is not the FQDN. search the alias list ...
		for (int i = 0; hent->h_aliases[i]; ++i) {
		    s = hent->h_aliases[i];
		    if ((int)s.find (".") >= 0) {
			me._host = s;
			break;
		    }
		}
	    } else {
		me._host = s;
	    }
	}
#ifdef HAVE_THREADS
        }
#endif // HAVE_THREADS
    }
    if (me._host.length() == 0) {
	// no FQDN found or no database entry. use dotted decimal ...
	me._host = "";
	for (mico_vec_size_type i = 0; i < _ipaddr.size(); ++i) {
	    if (i > 0)
		me._host += ".";
	    me._host += xdec (_ipaddr[i]);
	}
    }
    return TRUE;
}

string
MICO::InetAddress::stringify () const
{
    string s = proto();
    s += ":";
    CORBA::Boolean r = resolve_host ();

    if (r) {
      s += _host;
    }
    else {
      s += "(oops)";
    }

    s += ":";
    s += xdec (_port);
    return s;
}

const char *
MICO::InetAddress::proto () const
{
    switch (_family) {
    case STREAM:
	return "inet";
    case DGRAM:
	return "inet-dgram";
    default:
	assert (0);
	return 0;
    }
}

CORBA::Transport *
MICO::InetAddress::make_transport () const
{
    CORBA::Transport *ret; 
    switch (_family) {
    case STREAM:
	ret = new TCPTransport;
	break;
    case DGRAM:
	ret = new UDPTransport;
	break;
    default:
	assert (0);
	return 0;
    }
    ret->open();
    return ret;
}

CORBA::TransportServer *
MICO::InetAddress::make_transport_server () const
{
    switch (_family) {
    case STREAM:
	return new TCPTransportServer;
    case DGRAM:
	return new UDPTransportServer;
    default:
	assert (0);
	return 0;
    }
}

CORBA::IORProfile *
MICO::InetAddress::make_ior_profile (CORBA::Octet *key,
				     CORBA::ULong len,
				     const CORBA::MultiComponent &mc,
                                     CORBA::UShort version) const
{
    struct sockaddr_in sin = sockaddr();
    switch (_family) {
    case STREAM:
	if (sin.sin_addr.s_addr == htonl (INADDR_ANY)) {
	    InetAddress ia (InetAddress::hostname().c_str(), port());
	    return new IIOPProfile (key, len, ia, mc, version);
	}
	return new IIOPProfile (key, len, *this, mc, version);
    case DGRAM:
	if (sin.sin_addr.s_addr == htonl (INADDR_ANY)) {
	    InetAddress ia (InetAddress::hostname().c_str(), port(),
			    InetAddress::DGRAM);
	    return new IIOPProfile (key, len, ia, mc, version,
				    CORBA::IORProfile::TAG_UDP_IOP);
	}
	return new IIOPProfile (key, len, *this, mc, version,
				CORBA::IORProfile::TAG_UDP_IOP);
    default:
	assert (0);
	return 0;
    }
}

CORBA::Boolean
MICO::InetAddress::is_local () const
{
    return FALSE;
}

CORBA::Boolean
MICO::InetAddress::is_here () const
{
    /*
     * This test is not necessarily correct, since hostid() could give
     * a different interface from what ipaddr() gives, fooling the
     * determination to incorrectly assume an address to be remote.
     */
    return hostid() == ipaddr();
}

CORBA::Address *
MICO::InetAddress::clone () const
{
    return new InetAddress (*this);
}

CORBA::Long
MICO::InetAddress::compare (const CORBA::Address &a) const
{
    CORBA::Long r = strcmp (proto(), a.proto());
    if (r)
	return r;

    const InetAddress &he = (const InetAddress &)a;
    if (_port != he._port)
        return (CORBA::Long)_port - (CORBA::Long)he._port;

    // need not compare _family, because each family has a different
    // protocol identifier.

    CORBA::Boolean r1 = resolve_ip();
    CORBA::Boolean r2 = he.resolve_ip();

    /*
     * avoid assertion here and declare invalid addresses "less" than
     * valid ones
     */

    if (!r1 && !r2) {
      return 0;
    }
    else if (!r1 && r2) {
      return -1;
    }
    else if (r1 && !r2) {
      return 1;
    }

    return mico_vec_compare (_ipaddr, he._ipaddr);
}

CORBA::Boolean
MICO::InetAddress::operator== (const CORBA::Address &a) const
{
    return compare (a) == 0;
}

CORBA::Boolean
MICO::InetAddress::operator< (const CORBA::Address &a) const
{
    return compare (a) < 0;
}

CORBA::Boolean
MICO::InetAddress::valid () const
{
    return resolve_host() && resolve_ip();
}

const char *
MICO::InetAddress::host () const
{
    CORBA::Boolean r = resolve_host();
    assert (r);
    return _host.c_str();
}

void
MICO::InetAddress::host (const char *h)
{
    _ipaddr.erase (_ipaddr.begin(), _ipaddr.end());
    _host = h;
}

const vector<CORBA::Octet> &
MICO::InetAddress::ipaddr () const
{
    CORBA::Boolean r = resolve_ip ();
    assert (r);
    return _ipaddr;
}

void
MICO::InetAddress::ipaddr (const vector<CORBA::Octet> &ip)
{
    _host = "";
    _ipaddr = ip;
}

CORBA::UShort
MICO::InetAddress::port () const
{
    return _port;
}

void
MICO::InetAddress::port (CORBA::UShort p)
{
    _port = p;
}

MICO::InetAddress::Family
MICO::InetAddress::family () const
{
    return _family;
}

void
MICO::InetAddress::family (Family fam)
{
    _family = fam;
}

struct sockaddr_in
MICO::InetAddress::sockaddr () const
{
    CORBA::Boolean r = resolve_ip();
    assert (r);

    struct sockaddr_in sin;
    memset (&sin, 0, sizeof (sin));

    sin.sin_family = AF_INET;
    sin.sin_port = htons (_port);

    // XXX vector elements need not be continuous ?!
    assert (_ipaddr.size() < 2 || &_ipaddr[0] + 1 == &_ipaddr[1]);
    assert (_ipaddr.size() == sizeof (sin.sin_addr.s_addr));
    memcpy (&sin.sin_addr.s_addr, &_ipaddr[0], _ipaddr.size());
    return sin;
}

void
MICO::InetAddress::sockaddr (struct sockaddr_in &sin)
{
    _ipaddr.erase (_ipaddr.begin(), _ipaddr.end());
    _ipaddr.insert (_ipaddr.begin(),
		    (CORBA::Octet *)&sin.sin_addr.s_addr,
		    (CORBA::Octet *)(&sin.sin_addr.s_addr+1));
    _port = ntohs (sin.sin_port);
    _host = "";
}

string MICO::InetAddress::hname;

string
MICO::InetAddress::hostname ()
{
    if (hname.length() == 0) {
        char buf[200];
        int r = gethostname (buf, 200);
	assert (r == 0);
        /*
         * some OSes do not return an FQDN. So we get the ip address for the
         * hostname and resolve that address into the FQDN...
         */
        InetAddress a1 (buf, 0);
#ifndef __CYGWIN32__
        InetAddress a2 (a1.ipaddr());
        hname = a2.host();
#else
	/*
	 * with CDK beta 19 resolving IP addresses into host names
	 * hangs forever (even for this machines' IP address) if there
	 * is no name server. therefore we use dotted decimal notation.
	 */
	const vector<CORBA::Octet> &ipaddr = a1.ipaddr();
	for (mico_vec_size_type i = 0; i < ipaddr.size(); ++i) {
	    if (i > 0)
	        hname += ".";
	    hname += xdec (ipaddr[i]);
	}
#endif
    }
    return hname;
}

vector<CORBA::Octet> MICO::InetAddress::hid;

vector<CORBA::Octet>
MICO::InetAddress::hostid ()
{
    if (hid.size() == 0) {
        char buf[200];
        int r = gethostname (buf, 200);
	assert (r == 0);

        InetAddress a (buf, 0);
	hid = a.ipaddr();
    }
    return hid;
}

CORBA::Boolean
MICO::InetAddress::samehosts (const string &h1, const string &h2)
{
    // XXX what if h1 is an alias for h2 ???
    return h1 == h2;
}

void
MICO::InetAddress::resolve (CORBA::Boolean r)
{
    _resolve = r;
}

CORBA::Boolean
MICO::InetAddress::resolve ()
{
    return _resolve;
}


/*************************** InetAddressParser *************************/


MICO::InetAddressParser::InetAddressParser ()
{
    CORBA::Address::register_parser (this);
}

MICO::InetAddressParser::~InetAddressParser ()
{
    CORBA::Address::unregister_parser (this);
}

CORBA::Address *
MICO::InetAddressParser::parse (const char *str, const char *proto) const
{
    string s (str);
    CORBA::Long pos = s.find (":");
    if (pos < 0)
	return 0;

    MICO::InetAddress::Family family;
    if (!strcmp (proto, "inet") || !strcmp (proto, "inet-stream"))
	family = MICO::InetAddress::STREAM;
    else if (!strcmp (proto, "inet-dgram"))
	family = MICO::InetAddress::DGRAM;
    else
	return 0;

    string host = s.substr (0, pos);
    string port = s.substr (pos+1);
    CORBA::UShort portnum = atoi (port.c_str());

    InetAddress *ia;

    if (host.length() > 0) {
      ia = new InetAddress (host.c_str(), portnum, family);
    }
    else {
      ia = new InetAddress ("0.0.0.0", portnum, family);
    }
    if (!ia->valid()) {
	delete ia;
	return 0;
    }
    return ia;
}

CORBA::Boolean
MICO::InetAddressParser::has_proto (const char *p) const
{
    return
	!strcmp ("inet", p) ||
	!strcmp ("inet-stream", p) ||
	!strcmp ("inet-dgram", p);
}

static MICO::InetAddressParser inet_address_parser;


/****************************** UnixAddress *****************************/

#ifdef HAVE_SYS_UN_H
MICO::UnixAddress::UnixAddress (struct sockaddr_un &una)
{
    _filename = una.sun_path;
}

MICO::UnixAddress::UnixAddress (const char *filename)
{
    if (filename)
	_filename = filename;
}

MICO::UnixAddress::~UnixAddress ()
{
}

string
MICO::UnixAddress::stringify () const
{
    string s = proto();
    s += ":";
    s += _filename;
    return s;
}

const char *
MICO::UnixAddress::proto () const
{
    return "unix";
}

CORBA::Transport *
MICO::UnixAddress::make_transport () const
{
    CORBA::Transport *ret = new UnixTransport;
    ret->open();
    return ret;
}

CORBA::TransportServer *
MICO::UnixAddress::make_transport_server () const
{
    return new UnixTransportServer;
}

CORBA::IORProfile *
MICO::UnixAddress::make_ior_profile (CORBA::Octet *key,
				     CORBA::ULong len,
				     const CORBA::MultiComponent &mc,
                                     CORBA::UShort version) const
{
    return new UIOPProfile (key, len, *this, mc, version);
}

CORBA::Boolean
MICO::UnixAddress::is_local () const
{
    return FALSE;
}

CORBA::Boolean
MICO::UnixAddress::is_here () const
{
    return TRUE;
}

CORBA::Address *
MICO::UnixAddress::clone () const
{
    return new UnixAddress (*this);
}

CORBA::Long
MICO::UnixAddress::compare (const CORBA::Address &a) const
{
    CORBA::Long r = strcmp (proto(), a.proto());
    if (r)
	return r;

    const UnixAddress &he = (const UnixAddress &)a;

    return _filename.compare (he._filename);
}

CORBA::Boolean
MICO::UnixAddress::operator== (const CORBA::Address &a) const
{
    return compare (a) == 0;
}

CORBA::Boolean
MICO::UnixAddress::operator< (const CORBA::Address &a) const
{
    return compare (a) < 0;
}

const char *
MICO::UnixAddress::filename () const
{
    return _filename.c_str();
}

void
MICO::UnixAddress::filename (const char *filename)
{
    _filename = filename;
}

void
MICO::UnixAddress::sockaddr (struct sockaddr_un &una)
{
    _filename = una.sun_path;
}

struct sockaddr_un
MICO::UnixAddress::sockaddr () const
{
    struct sockaddr_un una;
    memset (&una, 0, sizeof (una));

    una.sun_family = AF_UNIX;
    assert (_filename.length()+1 <= sizeof (una.sun_path));
    strcpy (una.sun_path, _filename.c_str());

    return una;
}


/*************************** UnixAddressParser **************************/


MICO::UnixAddressParser::UnixAddressParser ()
{
    CORBA::Address::register_parser (this);
}

MICO::UnixAddressParser::~UnixAddressParser ()
{
    CORBA::Address::unregister_parser (this);
}

CORBA::Address *
MICO::UnixAddressParser::parse (const char *str, const char *) const
{
    return new UnixAddress (str);
}

CORBA::Boolean
MICO::UnixAddressParser::has_proto (const char *p) const
{
    return !strcmp ("unix", p);
}

static MICO::UnixAddressParser unix_address_parser;

#endif // HAVE_SYS_UN_H
