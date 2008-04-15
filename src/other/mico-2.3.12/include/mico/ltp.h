// -*- c++ -*-
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

#ifndef __MICO_LTP_H__
#define __MICO_LTP_H__

namespace MICOLTP {

void _init ();

/******************************************************************************
*                            LTPAddress class                                 *
******************************************************************************/

class LTPAddress:public CORBA::Address {
  private:
    CORBA::UShort _psm;                 // PSM to use
    std::string _bthost;                // BD_ADDR to use

  public:
    LTPAddress(const char *host = NULL, CORBA::UShort port = 0);
    ~LTPAddress();

    std::string stringify() const;
    const char *proto() const;
    CORBA::Transport *make_transport() const;
    CORBA::TransportServer *make_transport_server() const;
    CORBA::IORProfile *make_ior_profile(
      CORBA::Octet *, CORBA::ULong, const CORBA::MultiComponent &,
      CORBA::UShort version = 0x0100) const;
    CORBA::Boolean is_local() const;
    CORBA::Boolean is_here() const;

    CORBA::Address *clone() const;

    CORBA::Long compare(const CORBA::Address &) const;
    CORBA::Boolean operator==(const CORBA::Address &) const;
    CORBA::Boolean operator<(const CORBA::Address &) const;

    const char *bthost() const;
    void bthost(const char *);

    CORBA::UShort psm() const;
    void psm(CORBA::UShort psm);

    void sockaddr(void *p);
    void getsockaddr(void *p);
};

/******************************************************************************
*                         LTPAddressParser class                              *
******************************************************************************/

class LTPAddressParser:public CORBA::AddressParser {
  public:
    LTPAddressParser();
    ~LTPAddressParser();

    CORBA::Address *parse(const char *, const char *) const;
    CORBA::Boolean has_proto(const char *) const;
};

class L2CAPIOPProfile : public CORBA::IORProfile {
    CORBA::Octet *objkey;
    CORBA::ULong length;
    MICOLTP::LTPAddress myaddr;
    CORBA::MultiComponent comps;
    ProfileId tagid;
    CORBA::UShort version;
public:
    L2CAPIOPProfile (CORBA::Octet *objkey, CORBA::ULong length,
		 const MICOLTP::LTPAddress &,
		 const CORBA::MultiComponent & = CORBA::MultiComponent(),
		 CORBA::UShort version = 0x0100,
		 ProfileId = TAG_LTP_IOP);
    L2CAPIOPProfile (const L2CAPIOPProfile &);
    ~L2CAPIOPProfile ();
    L2CAPIOPProfile &operator= (const L2CAPIOPProfile &);

    void encode (CORBA::DataEncoder &) const;
    const CORBA::Address *addr () const;
    void addr (const MICOLTP::LTPAddress &); // miwco-extension
    ProfileId id () const;
    ProfileId encode_id () const;
    void objectkey (CORBA::Octet *, CORBA::Long length);
    const CORBA::Octet *objectkey (CORBA::Long &length) const;
    CORBA::Boolean reachable ();
    void print (std::ostream &) const;

    CORBA::IORProfile *clone () const;
    CORBA::Long compare (const CORBA::IORProfile &) const;
    CORBA::Boolean operator== (const CORBA::IORProfile &) const;
    CORBA::Boolean operator< (const CORBA::IORProfile &) const;

    CORBA::UShort iiop_version () const
    { return version; }
    
    CORBA::MultiComponent *components()
    { return &comps; }
};

class L2CAPIOPProfileDecoder : public CORBA::IORProfileDecoder {
    CORBA::IORProfile::ProfileId tagid;

public:
    L2CAPIOPProfileDecoder (CORBA::IORProfile::ProfileId
#ifndef _WINDOWS
			= CORBA::IORProfile::TAG_LTP_IOP
#endif
		);
    ~L2CAPIOPProfileDecoder ();
    CORBA::IORProfile *decode (CORBA::DataDecoder &, ProfileId,
			       CORBA::ULong) const;
    CORBA::Boolean has_id (ProfileId) const;
};

#define BUF_SIZE 	8192	// internal buffer
#define LTP_BUF 	672	// buffer for one LTP message
#define LTP_HDR_SIZE 	3	// LTP header size

#define LTP_SEG_MIDDLE 	0x00	// LTP segment flags
#define LTP_SEG_FIRST 	0x01
#define LTP_SEG_LAST 	0x02
#define LTP_SEG_NONE 	0x03

/******************************************************************************
*                            LTPTransport class                               *
******************************************************************************/

class LTPTransport:public CORBA::Transport, public CORBA::DispatcherCallback {
  private:
    CORBA::Dispatcher *rdisp, *wdisp;   // read and write dispatchers
    CORBA::TransportCallback *rcb, *wcb;// callback
    CORBA::Long fd;                     // file descriptor to a BT socket
    CORBA::Boolean ateof;               // EOF reached when reading a socket
    std::string err;                    // error message
    MICOLTP::LTPAddress local_addr, peer_addr;   // local and peer BT addresses
    CORBA::Boolean is_blocking;         // is the server blocking or not
    CORBA::Boolean is_buffering;        // is the server buffering or not

    char *ltp_buffer;			// buffer for LTP layer messages
    char *buf_the_dragon;               // internal buffer used for reading
    int scales;                         // how much data do we have in buffer
    int tail;                           // buffer index
    bool buffer_empty;                  // is the whole buffer used or not

  public:
    LTPTransport(CORBA::Long fd = -1);
    ~LTPTransport();

    void rselect(CORBA::Dispatcher *, CORBA::TransportCallback *);
    void wselect(CORBA::Dispatcher *, CORBA::TransportCallback *);
    void callback(CORBA::Dispatcher *, CORBA::Dispatcher::Event);

    CORBA::Boolean bind(const CORBA::Address *);
    CORBA::Boolean connect(const CORBA::Address *);
    void open(CORBA::Long fd = -1);
    void close();
    void block(CORBA::Boolean doblock = TRUE);
    CORBA::Boolean isblocking();
    void buffering(CORBA::Boolean dobuffering = TRUE);
    CORBA::Boolean isbuffering();
    CORBA::Boolean isreadable();

    CORBA::Long read(void *, CORBA::Long len);
    CORBA::Long write(const void *, CORBA::Long len);

    const CORBA::Address *addr();
    const CORBA::Address *peer();

    CORBA::Boolean eof() const;
    CORBA::Boolean bad() const;
    std::string errormsg() const;
};

/******************************************************************************
*                         LTPTransportServer class                            *
******************************************************************************/

class LTPTransportServer:public CORBA::TransportServer,
    public CORBA::DispatcherCallback {

  private:
    CORBA::Dispatcher *adisp;           // event dispatcher
    CORBA::TransportServerCallback *acb;// callback
    CORBA::Long fd;                     // file descriptor for the L2CAP socket
    std::string err;                    // error message
    MICOLTP::LTPAddress local_addr;     // local address
    CORBA::Boolean listening;           // is the server is listening or not
    CORBA::Boolean is_blocking;         // is the server is blocking or not
    void listen();                      // orders the server to listen a socket

  public:
    LTPTransportServer();
    ~LTPTransportServer();

    void aselect(CORBA::Dispatcher *, CORBA::TransportServerCallback *);
    void callback(CORBA::Dispatcher *, CORBA::Dispatcher::Event);

    CORBA::Boolean bind(const CORBA::Address *);
    void close();
    void block(CORBA::Boolean doblock = TRUE);
    CORBA::Boolean isblocking() const;

    CORBA::Transport *accept();
    const CORBA::Address *addr();

    CORBA::Boolean bad() const;
    std::string errormsg() const;
};

}

#endif
