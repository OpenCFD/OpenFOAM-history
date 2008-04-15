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

#ifndef __mico_util_h__
#define __mico_util_h__

#include <CORBA.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif
#include <ctype.h>
#include <mico/basic.h>
#include <mico/os-misc.h>

namespace MICO {

  class Logger {
  public:
    enum MessageType {
      Info,           /* trivial stuff */
      Warning,        /* warnings that might be of interest */
      Error,          /* errors that must be noticed */
      GIOP,           /* i.e. sending and receiving of messages */
      IIOP,           /* i.e. incoming and outcoming connections */
      Transport,      /* raw message contents */
      Thread,         /* thread related stuff */
      POA,            /* POA internals */
      ORB,            /* ORB internals */
      Support,        /* support functions */
      PI,             /* Portable Interceptor messages */
      Security,       /* security related debugging. e.g. MICOSec, CSIv2 etc */
      Exception,      /* exception debugging */
      All /* must be the maximum value */
    };

    static std::ostream ** _out;
    static Logger * _instance;

  public:
    Logger ();
    ~Logger ();

    static void Log (MessageType, CORBA::Boolean = 1, const char * file = 0);
    static void Log (const char *, CORBA::Boolean = 1, const char * file = 0);

    static CORBA::Boolean IsLogged (MessageType msg)
    {
      if (!_instance) {
	return 0;
      }
      return (_out[msg] != 0);
    };

    static std::ostream & Stream (MessageType msg)
    {
	assert (_instance);
	assert (_out[msg]);
	return *_out[msg];
    };

  };

}


class MICOGetOpt {
public:
    typedef std::map<std::string, std::string, std::less<std::string> > OptMap;
    typedef std::vector<std::pair<std::string, std::string> > OptVec;
private:
    OptMap _in_opts;
    OptVec _out_opts;

    CORBA::Boolean parse (const std::vector<std::string> &args, std::vector<int> &erase,
			  CORBA::Boolean ignore = FALSE);
public:
    MICOGetOpt (const OptMap &opts);
    ~MICOGetOpt ();

    CORBA::Boolean parse (int &argc, char *argv[],
			  CORBA::Boolean ignore = FALSE);
    CORBA::Boolean parse (const std::vector<std::string> &args,
			  CORBA::Boolean ignore = FALSE);
    CORBA::Boolean parse (const std::string &filename,
			  CORBA::Boolean ignore = FALSE);

    const OptVec &opts () const;
};

template<class V>
inline CORBA::Long
mico_vec_compare (V v1, V v2)
{
    int len = v1.size() < v2.size() ? v1.size() : v2.size();
    for (int i = 0; i < len; ++i) {
	if (v1[i] < v2[i])
	    return -1;
	if (v2[i] < v1[i])
	    return 1;
    }
    return v1.size() - v2.size();
}

static inline CORBA::Long
mico_key_compare (const CORBA::Octet *k1, const CORBA::Octet *k2,
		  CORBA::Long len)
{
    while (--len >= 0) {
	if (*k1 != *k2)
	    return (CORBA::Long)*k1 - (CORBA::Long)*k2;
	++k1;
	++k2;
    }
    return 0;
}


static inline CORBA::Octet
mico_from_xdigit (CORBA::Octet c)
{
    c = tolower (c);
    assert (isxdigit (c));
    return isdigit (c) ? c - '0' : c - 'a' + 10;
}

static inline CORBA::Octet
mico_to_xdigit (CORBA::Octet o)
{
    static const char *xdigits = "0123456789abcdef";
    assert (o < 16);
    return xdigits[o];
}

#ifdef USE_CSIV2

namespace ASN1
{
    class Codec
    {
	static std::vector<unsigned int>
	parse_oid(const char* __oid);
    public:
	static char*
	decode_oid(CORBA::OctetSeq& __oid);

	static CORBA::OctetSeq*
	encode_oid(const char* __oid);
    };
}

#endif // USE_CSIV2

extern void mico_ieee2float (CORBA::Octet ieee[4], CORBA::Float &f);
extern void mico_float2ieee (CORBA::Octet ieee[4], CORBA::Float  f);
extern void mico_ieee2double (CORBA::Octet ieee[8], CORBA::Double &f);
extern void mico_double2ieee (CORBA::Octet ieee[8], CORBA::Double  f);
extern void mico_ieee2ldouble (CORBA::Octet ieee[16], CORBA::LongDouble &f);
extern void mico_ldouble2ieee (CORBA::Octet ieee[16], CORBA::LongDouble  f);
extern CORBA::Boolean mico_fnmatch (const char *repoid, const char *patt);

extern std::string xstrerror (int);
extern std::string xdec (int);
extern std::string xdec (long);
extern std::string xdec (OSMisc::longlong);
extern std::string xdec (unsigned int);
extern std::string xdec (unsigned long);
extern std::string xdec (OSMisc::ulonglong);

extern size_t xwcslen (const wchar_t *);
extern int xwcscmp (const wchar_t *, const wchar_t *);
extern wchar_t *xwcscpy (wchar_t *, const wchar_t *);
extern wchar_t *xwcsncpy (wchar_t *, const wchar_t *, size_t);

extern char * mico_url_encode (const CORBA::Octet *, CORBA::ULong);
extern CORBA::Octet * mico_url_decode (const char *, CORBA::ULong &);

extern CORBA::ULong mico_string_hash (const char *s, CORBA::ULong max);

#ifndef HAVE_STRERROR_PROTO
extern "C" char *strerror (int);
#endif

#ifndef HAVE_GETHOSTNAME_PROTO
extern "C" int gethostname (char *, size_t);
#endif

#ifndef HAVE_FINITE_PROTO
extern "C" int finite (double);
#endif

#ifndef HAVE_FTIME_PROTO
extern "C" int ftime (struct timeb *);
#endif

#endif // __mico_util_h__
