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

#define MICO_CONF_IMR

#include <CORBA-SMALL.h>
#include <string.h>
#include <errno.h>
#include <iostream>

#include <mico/os-net.h>
#include <mico/impl.h>
#include <mico/template_impl.h>
#include <mico/util.h>
#include <mico/ltp.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>

using namespace std;

/*******************************************************************************
* void baswap2(bdaddr_t *dst, bdaddr_t *src)
*
* Copies given source bluetooth address to destination address and swaps it.
*
* input parameters:
*   bdaddr_t *dst                 : pointer to destination address
*   bdaddr_t *src                 : pointer of a source address
*
* return value: none
*******************************************************************************/

void baswap2(bdaddr_t *dst, bdaddr_t *src){
  register unsigned char * d = (unsigned char *)dst;
  register unsigned char * s = (unsigned char *)src;
  register int i;
  for(i=0; i<6; i++) d[i] = s[5-i];
}


/*******************************************************************************
* char * batostr2(bdaddr_t *ba)
*
* Return given bluetooth address as a string.
*
* input parameters:
*   bdaddr_t *ba                  : Bluetooth address to be stringified
*
* return value: pointer to a string that contains Bluetooth address
*******************************************************************************/

char * batostr2(bdaddr_t *ba){
  static char str[2][18];
  static int i = 1;
  i ^= 1;
  sprintf(str[i], "%2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X",
    ba->b[0], ba->b[1], ba->b[2],
    ba->b[3], ba->b[4], ba->b[5]);
  return str[i];
}


/*******************************************************************************
* bdaddr_t * strtoba2(char *str)
*
* Returns given address string in bluetooth address form.
*
* input parameters:
*   char *str : Pointer to string that is to be converted to bluetooth address
*
* return value: pointer to bluetooth address
*******************************************************************************/

bdaddr_t * strtoba2(char *str){
  static unsigned char ba[2][sizeof(bdaddr_t)];
  static int i = 1;
  register char *ptr = str;
  register int x;
  i ^= 1;
  for(x=0; x<6; x++){
    ba[i][x] = (uint8_t) strtol(ptr, NULL, 16);
    if( x!=5 && !(ptr=strchr(ptr,':')) ) ptr = ":00:00:00:00:00";
    ptr++;
  }
  return (bdaddr_t *) ba[i];
}


/*******************************************************************************
*                              LTPAddress class                                *
*******************************************************************************/


/*******************************************************************************
* MICOLTP::LTPAddress::LTPAddress(const char *bthost, CORBA::UShort psm)
*
* Constructor of the LTPAddress class.
*
* input parameters:
*   const char *bthost            : host address that is to be used
*   CORBA::UShort psm             : PSM that is to be used
*
* return value: If you don't know THIS, you shouldn't be looking these codes...
*******************************************************************************/

MICOLTP::LTPAddress::LTPAddress(const char *bthost, CORBA::UShort psm)
    : _psm (psm), _bthost (bthost ? bthost : "") {
  #ifdef LTP_DEBUG
    cerr << "LTPAddress::LTPAddress(bthost,psm)" << endl;
    cerr << "    BD_ADDR : " << _bthost.c_str() << endl;
    cerr << "    PSM     : " << _psm << endl;
  #endif
}


/*******************************************************************************
* MICOLTP::LTPAddress::~LTPAddress()
*
* Destructor of LTPAddress.
*
* input parameters:
*   none
*
* return value: empty space where once was something known as LTPAddress object
*******************************************************************************/

MICOLTP::LTPAddress::~LTPAddress() {
  #ifdef LTP_DEBUG
    cerr << "LTPAddress::~LTPAddress" << endl;
  #endif
}


/*******************************************************************************
* string MICOLTP::LTPAddress::stringify() const
*
* Returns address in stringified form.
*
* input parameters:
*   none
*
* return value: string that contains address in form of proto:BD_ADDR#PSM
*******************************************************************************/

string MICOLTP::LTPAddress::stringify() const {
  #ifdef LTP_DEBUG
    cerr << "LTPAddress::stringify" << endl;
  #endif

  string s = proto();

  s += ":";
  s += _bthost;
  s += "#";
  s += xdec (_psm);

  #ifdef LTP_DEBUG
    cerr << "    stringified bt: " << s << endl;
  #endif

  return s;
}


/*******************************************************************************
* const char * MICOLTP::LTPAddress::proto() const
*
* Returns a pointer to string containing the proto of used protocol.
*
* input parameters:
*   none
*
* return value: the proto of LTP protocol as a string
*******************************************************************************/

const char * MICOLTP::LTPAddress::proto() const {
  #ifdef LTP_DEBUG
    cerr << "LTPAdress::proto" << endl;
  #endif

  return "bt-l2cap";
}


/*******************************************************************************
* CORBA::Transport * MICOLTP::LTPAddress::make_transport() const
*
* Creates a new Transport object suitable for the LTP addressing mode.
*
* input parameters:
*   none
*
* return value: LTPTransport object
*******************************************************************************/

CORBA::Transport * MICOLTP::LTPAddress::make_transport() const {
  #ifdef LTP_DEBUG
    cerr << "LTPAddress::make_transport" << endl;
  #endif

  return new LTPTransport;
}


/*******************************************************************************
* CORBA::TransportServer * MICOLTP::LTPAddress::make_transport_server() const
*
* Creates a new Transportserver object suitable for the LTP addressing  mode.
*
* input parameters:
*   none
*
* return value: LTPTransportServer object
*******************************************************************************/

CORBA::TransportServer * MICOLTP::LTPAddress::make_transport_server() const {
  #ifdef LTP_DEBUG
    cerr << "LTPAddress::make_transport_server" << endl;
  #endif

  return new LTPTransportServer;
}


/*******************************************************************************
* CORBA::IORProfile * MICOLTP::LTPAddress::make_ior_profile() const
*
* Creates a new L2CAPIOPProfile object.
*
* input parameters:
*   CORBA::Octet *key               : object key
*   CORBA::ULong len                : length
*   const CORBA::MultiComponent &mc : multicomponent
*   CORBA::UShort version           : GIOP version number
*
* return value: L2CAPIOPProfile object
*******************************************************************************/

CORBA::IORProfile * MICOLTP::LTPAddress::make_ior_profile(CORBA::Octet *key,
  CORBA::ULong len, const CORBA::MultiComponent &mc,
  CORBA::UShort version) const {
  #ifdef LTP_DEBUG
    cerr << "LTPAddress::make_ior_profile" << endl;
    cerr << "    BD_ADDR : " << _bthost.c_str() << endl;
    cerr << "    PSM     : " << _psm << endl;
  #endif

  return new L2CAPIOPProfile(key, len, *this, mc, version);
}


/*******************************************************************************
* CORBA::Boolean MICOLTP::LTPAddress::is_local() const
*
* Just returns FALSE.
*
* input parameters:
*   none
*
* return value: FALSE
*******************************************************************************/

CORBA::Boolean MICOLTP::LTPAddress::is_local() const {
  #ifdef LTP_DEBUG
    cerr << "LTPAddress::is_local" << endl;
  #endif

  return FALSE;
}


/*******************************************************************************
* CORBA::Boolean MICOLTP::LTPAddress::is_here() const
*
* Just returns false.
*
* input parameters:
*   none
*
* return value: FALSE
*******************************************************************************/

CORBA::Boolean MICOLTP::LTPAddress::is_here() const {
  #ifdef LTP_DEBUG
    cerr << "LTPAddress::is_here" << endl;
  #endif

  return FALSE;
}


/*******************************************************************************
* CORBA::Address * MICOLTP::LTPAddress::clone() const
*
* Clones current LTPAddress object.
*
* input parameters:
*   none
*
* return value: LTPAddress object
*******************************************************************************/

CORBA::Address * MICOLTP::LTPAddress::clone() const {
  #ifdef LTP_DEBUG
    cerr << "LTPAddress::clone" << endl;
  #endif

  return new LTPAddress(*this);
}


/*******************************************************************************
* CORBA::Long MICOLTP::LTPAddress::compare(const CORBA::Address &a) const
*
* Compares given address.
*
* input parameters:
*   const CORBA::Address &a : address that is to be compared
*
* return value: the difference between addresses
*******************************************************************************/

CORBA::Long MICOLTP::LTPAddress::compare(const CORBA::Address &a) const {
  #ifdef LTP_DEBUG
    cerr << "LTPAddress::compare" << endl;
  #endif

  CORBA::Long r = strcmp(proto(), a.proto());
  if (r) return r;

  const LTPAddress &he = (const LTPAddress &)a;
  if (_psm != he._psm) return (CORBA::Long)_psm - (CORBA::Long)he._psm;

  return 0;
}


/*******************************************************************************
* CORBA::Boolean MICOLTP::LTPAddress::operator==(const CORBA::Address &a) const
*
* Overloads the == operator.
*
* input parameters:
*   const CORBA::Address &a       : an address
*
* return value: TRUE if compare equal to 0
*******************************************************************************/

CORBA::Boolean MICOLTP::LTPAddress::operator==(const CORBA::Address &a) const {
  #ifdef LTP_DEBUG
    cerr << "LTPAddress::operator==" << endl;
  #endif

  return compare (a) == 0;
}


/*******************************************************************************
* CORBA::Boolean MICOLTP::LTPAddress::operator< (const CORBA::Address &a) const
*
* overloads <operator
*
* input parameters:
*   const CORBA::Address &a :
*
* return value: true if compare less than 0
*******************************************************************************/

CORBA::Boolean MICOLTP::LTPAddress::operator<(const CORBA::Address &a) const {
  #ifdef LTP_DEBUG
    cerr << "LTPAddress::operator<" << endl;
  #endif

  return compare (a) < 0;
}


/*******************************************************************************
* const char * MICOLTP::LTPAddress::bthost() const
*
* Returns the value of _bthost variable as string.
*
* input parameters:
*   none
*
* return value: current Bluetooth address as a string
*******************************************************************************/

const char * MICOLTP::LTPAddress::bthost() const {
  #ifdef LTP_DEBUG
    cerr << "LTPAddress::bthost" << endl;
  #endif

  return _bthost.c_str();
}


/*******************************************************************************
* void MICOLTP::LTPAddress::bthost(const char *h)
*
* Sets Bluetooth host address (BD_ADDR).
*
* input parameters:
*   const char *h                 : string containing a new Bluetooth address
*
* return value: none
*******************************************************************************/

void MICOLTP::LTPAddress::bthost(const char *h) {
  #ifdef LTP_DEBUG
    cerr << "LTPAddress::bthost" << endl;
  #endif

  _bthost = h;
}


/*******************************************************************************
* CORBA::UShort MICOLTP::LTPAddress::psm() const
*
* Returns the value of _psm variable.
*
* input parameters:
*   none
*
* return value: current PSM
*******************************************************************************/

CORBA::UShort MICOLTP::LTPAddress::psm() const {
  #ifdef LTP_DEBUG
    cerr << "LTPAddress::psm" << endl;
  #endif

  return _psm;
}


/*******************************************************************************
* void MICOLTP::LTPAddress::psm(CORBA::UShort p)
*
* Sets the value of PSM.
*
* input parameters:
*   CORBA::UShort p               : PSM
*
* return value: none
*******************************************************************************/

void MICOLTP::LTPAddress::psm(CORBA::UShort p) {
  #ifdef LTP_DEBUG
    cerr << "LTPAddress::psm" << endl;
  #endif

  _psm = p;
}


/*******************************************************************************
* void MICOLTP::LTPAddress::sockaddr(void *p)
*
* Updates object's psm and Bluetooth address from sockaddr_l2 struct given in *p
*
* input parameters:
*   void *p                       : pointer to sockaddr struct
*
* return value: none
*******************************************************************************/

void MICOLTP::LTPAddress::sockaddr(void *p) {
  #ifdef LTP_DEBUG
    cerr << "LTPAddress::sockaddr" << endl;
  #endif

  struct sockaddr_l2 *sin;
  bdaddr_t tmp_addr;

  sin = (struct sockaddr_l2 *)p;
  _psm = btohs(sin->l2_psm);

  // we need to swap BD_ADDR's reading
  baswap2(&tmp_addr, &sin->l2_bdaddr);
  _bthost = batostr2(&tmp_addr);

  #ifdef LTP_DEBUG
    cerr << "    BD_ADDR : " << batostr2(&sin->l2_bdaddr) << endl;
    cerr << "    PSM     : " << sin->l2_psm << endl;
  #endif
}


/*******************************************************************************
* void MICOLTP::LTPAddress::getsockaddr(void *p)
*
* Inserts the values of _bthost and _psm to sockaddr struct.
*
* input parameters:
*   void *p                       : pointer to sockaddr struct
*
* return value: none
*******************************************************************************/

void MICOLTP::LTPAddress::getsockaddr(void *p) {
  #ifdef LTP_DEBUG
    cerr << "LTPAddress::getsockaddr" << endl;
  #endif

  struct sockaddr_l2 *sin;

  sin = (struct sockaddr_l2 *)p;
  memset(sin, 0, sizeof(struct sockaddr_l2));
  sin->l2_family = AF_BLUETOOTH;
  sin->l2_psm = htobs(_psm);
  baswap2(&sin->l2_bdaddr, strtoba2((char *)_bthost.c_str()));

  #ifdef LTP_DEBUG
    cerr << "    BD_ADDR : " << batostr2(&sin->l2_bdaddr) << endl;
    cerr << "    PSM     : " << sin->l2_psm << endl;
  #endif
}

/*******************************************************************************
*                           LTPAddressParser class                             *
*******************************************************************************/


/*******************************************************************************
* MICOLTP::LTPAddressParser::LTPAddressParser()
*
* LTPAddressParser constructor.
*
* input parameters:
*   none
*
* return value: check the previous constructors
*******************************************************************************/

MICOLTP::LTPAddressParser::LTPAddressParser() {
  #ifdef LTP_DEBUG
    cerr << "LTPAddressParser::LTPAddressParser" << endl;
  #endif
}


/*******************************************************************************
* MICOLTP::LTPAddressParser::~LTPAddressParser()
*
* LTPAddressParser destructor.
*
* input parameters:
*   none
*
* return value: check the previous constructors
*******************************************************************************/

MICOLTP::LTPAddressParser::~LTPAddressParser() {
  #ifdef LTP_DEBUG
    cerr << "LTPAddressParser::~LTPAddressParser" << endl;
  #endif
}


/*******************************************************************************
* CORBA::Address * MICOLTP::LTPAddressParser::parse() const
*
* Parses BD_ADDR and PSM fields from an address string.
*
* input parameters:
*   const char *str                : address string (should be BD_ADDR#PSM)
*   const char *proto              : unused
*
* return value: new LTPAddress object initialized with address in *str
*******************************************************************************/

CORBA::Address * MICOLTP::LTPAddressParser::parse(const char *str, const char *proto) const {
  #ifdef LTP_DEBUG
    cerr << "LTPAddressParser::parse" << endl;
    cerr << "    address to be parsed: " << str << endl;
  #endif

  string tmp_str(str);
  LTPAddress *ia;
  CORBA::UShort psm;

  int pos = tmp_str.find("#");
  if (pos < 0) return 0;

  // pasre BD_ADDR and PSM out from the address string
  string bthost = tmp_str.substr(0, pos);
  string psm_str = tmp_str.substr(pos+1);
  if (bthost.length() != 17) {
    cerr << "wrong address format, use bt-l2cap:XX:XX:XX:XX:XX:XX#PSM" << endl;
    return 0;
  }

  psm = atoi(psm_str.c_str());
  ia = new LTPAddress(bthost.c_str(), psm);

  return ia;
}


/*******************************************************************************
* CORBA::Boolean MICOLTP::LTPAddressParser::has_proto(const char *p) const
*
* Checks if a given proto mathces the poroto of LTP address format.
*
* input parameters:
*   const char *p                 : proto to be checked
*
* return value: TRUE if not equal to "bt-l2cap"
*******************************************************************************/

CORBA::Boolean MICOLTP::LTPAddressParser::has_proto(const char *p) const {
  #ifdef LTP_DEBUG
    cerr << "LTPAddressParser::has_proto" << endl;
  #endif

  return (!strcmp("bt-l2cap", p));
}
