/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <algorithm>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <fstream>
#include <sstream>
#else
#include <iostream.h>
#include <fstream.h>
#include <strstream.h>
#endif
#include <mico/impl.h>
#include <mico/template_impl.h>
#include <mico/util.h>
#include <mico/os-misc.h>
#include <mico/os-math.h>
#ifdef USE_MEMTRACE
#include <mico/memtrace.h>
#endif

#endif // FAST_PCH


using namespace std;

#ifdef _POCKET_PC
extern char* getenv (const char*);
#endif

/******************************* assert *******************************/


#ifndef _POCKET_PC
void
mico_assert (const char *file, int line)
{
#ifdef HAVE_THREADS
    cout << "[" << ((MICO_ULong) OSMisc::timestamp()) << " - "
#ifdef NEED_THREADID_OPERATORS
		<< MICOMT::Thread::self().p
#else // NEED_THREADID_OPERATORS
		<< MICOMT::Thread::self()
#endif // NEED_THREADID_OPERATORS
		<< "] ";
#endif
    cout << file << ":" << line << ": assertion failed" << endl;
#ifdef USE_MEMTRACE
    MemTrace_SelfTrace (stderr);
#endif
    abort ();
}
#endif


/****************************** strerror ******************************/


#ifndef HAVE_STRERROR
extern "C" {

    extern char *sys_errlist[];
    extern int sys_nerr;

    char *
    strerror (int err)
    {
	assert (err >= 0 && err < sys_nerr);
	return sys_errlist[err];
    }
}
#endif

string
xstrerror (int err)
{
    char *cp = strerror (err);

    if (cp)
        return string(cp);

    // Cygnus CDK returnes (char *)0 for some errnos
    string lasterr;
    lasterr = "error ";
    lasterr += xdec (errno);
    return lasterr;
}

string
xdec (int i)
{
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
    ostringstream ostr;
    ostr << i;
    return ostr.str();
#else
    ostrstream ostr;
    ostr << i << ends;
    string s = ostr.str();
    ostr.rdbuf()->freeze (0);
    return s;
#endif
}

string
xdec (long i)
{
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
    ostringstream ostr;
    ostr << i;
    return ostr.str();
#else
    ostrstream ostr;
    ostr << i << ends;
    string s = ostr.str();
    ostr.rdbuf()->freeze (0);
    return s;
#endif
}

string
xdec (OSMisc::longlong i)
{
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
    ostringstream ostr;
    ostr << i;
    return ostr.str();
#else
    ostrstream ostr;
    ostr << i << ends;
    string s = ostr.str();
    ostr.rdbuf()->freeze (0);
    return s;
#endif
}

string
xdec (unsigned int i)
{
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
    ostringstream ostr;
    ostr << i;
    return ostr.str();
#else
    ostrstream ostr;
    ostr << i << ends;
    string s = ostr.str();
    ostr.rdbuf()->freeze (0);
    return s;
#endif
}

string
xdec (unsigned long i)
{
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
    ostringstream ostr;
    ostr << i;
    return ostr.str();
#else
    ostrstream ostr;
    ostr << i << ends;
    string s = ostr.str();
    ostr.rdbuf()->freeze (0);
    return s;
#endif
}

string
xdec (OSMisc::ulonglong i)
{
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
    ostringstream ostr;
    ostr << i;
    return ostr.str();
#else
    ostrstream ostr;
    ostr << i << ends;
    string s = ostr.str();
    ostr.rdbuf()->freeze (0);
    return s;
#endif
}

size_t
xwcslen (const wchar_t *s)
{
    size_t len = 0;
    while (*s++)
	++len;
    return len;
}

int
xwcscmp (const wchar_t *s1, const wchar_t *s2)
{
    while (*s1 && *s1 == *s2) {
	++s1; ++s2;
    }
    if (*s1 == *s2)
	return 0;
    if (*s1 < *s2)
	return -1;
    return 1;
}

wchar_t *
xwcscpy (wchar_t *_d, const wchar_t *s)
{
    wchar_t *d = _d;

    while ((*d++ = *s++))
	;
    return _d;
}

wchar_t *
xwcsncpy (wchar_t *_d, const wchar_t *s, size_t n)
{
    wchar_t *d = _d;

    int i = (int)n;
    while (--i >= 0 && (*d++ = *s++))
	;
    return _d;
}

char *
mico_url_encode (const CORBA::Octet * ptr, CORBA::ULong len)
{
  string res;

  for (CORBA::ULong i=0; i<len; i++) {
    // US-ASCII alphanumeric characters are not escaped
    if ((*ptr >= 'a' && *ptr <= 'z') || (*ptr >= 'A' && *ptr <= 'Z') ||
	(*ptr >= '0' && *ptr <= '9')) {
      res += *ptr++;
      continue;
    }
    // Also not escaped
    switch (*ptr) {
    case ';':
    case '/':
    case ':':
    case '?':
    case '@':
    case '&':
    case '=':
    case '+':
    case '$':
    case ',':
    case '-':
    case '.':
    case '!':
    case '~':
    case '*':
    case '\'':
    case '(':
    case ')':
      res += *ptr++;
      break;

    default:
      {
	CORBA::Octet x = *ptr++;
	res += '%';
	res += (char) mico_to_xdigit (x>>4);
	res += (char) mico_to_xdigit (x&15);
      }
    }
  }

  return CORBA::string_dup (res.c_str());
}

CORBA::Octet *
mico_url_decode (const char * ptr, CORBA::ULong & len)
{
  CORBA::Octet * str = (CORBA::Octet *) CORBA::string_alloc (strlen (ptr));
  CORBA::Octet * res = str;
  
  len = 0;

  while (*ptr) {
    if (*ptr == '%') {
      if (!isxdigit((unsigned char)ptr[1]) ||
	  !isxdigit((unsigned char)ptr[2])) {
	CORBA::string_free ((char *) str);
	return NULL;
      }
      *res = (char) (mico_from_xdigit(ptr[1])<<4 | mico_from_xdigit(ptr[2]));
      ptr += 3;
    }
    else {
      *res = *ptr++;
    }
    res++;
    len++;
  }

  /*
   * Null-terminate the result so that it can be used as a string. The
   * null is deliberately not added to the length, because it isn't part
   * of the string.
   */

  *res = 0;

  return str;
}

CORBA::ULong
mico_string_hash (const char *s, CORBA::ULong max)
{
    if (max == 0)
	return 0;

    CORBA::ULong g, v = 0;
    while (*s) {
	v = (v << 4) + *s++;
	if ((g = v & 0xf0000000)) {
	    v = v ^ (g >> 24);
	    v = v ^ g;
	}
    }
    return v % max;
}


/***************************** MICODebug ******************************/


MICO::Logger * MICO::Logger::_instance = 0;
ostream ** MICO::Logger::_out = 0;

MICO::Logger::Logger ()
{
  assert (!_instance);

  _out = new ostream * [All];

  for (int i=0; i<All; i++) {
    _out[i] = 0;
  }
  _instance = this;
  Log (Error);
}

MICO::Logger::~Logger ()
{
  _instance = 0;

  for (int i=0; i<All; i++) {
    if (_out[i] != &cout && _out[i] != &cerr) {
      delete _out[i];
    }
  }
  
  delete [] _out;
}

void
MICO::Logger::Log (MessageType msg, CORBA::Boolean onoff,
		   const char * file)
{
  if (!_instance) {
    new Logger;
    assert (_instance);
  }

  if (msg == All) {
    for (int i=0; i<All; i++) {
      Log ((MessageType) i, onoff, file);
    }
    return;
  }

  if (_out[msg] != &cout && _out[msg] != &cerr) {
    delete _out[msg];
  }

  if (onoff) {
    if (file) {
      _out[msg] = new ofstream (file, ios::app);
    }
    else {
      _out[msg] = &cerr;
    }
  }
  else {
    _out[msg] = 0;
  }
}

void
MICO::Logger::Log (const char * msg, CORBA::Boolean onoff,
		   const char * file)
{
  if (strcmp (msg, "Info") == 0) {
    Log (Info, onoff, file);
  }
  else if (strcmp (msg, "Warning") == 0) {
    Log (Warning, onoff, file);
  }
  else if (strcmp (msg, "Error") == 0) {
    Log (Error, onoff, file);
  }
  else if (strcmp (msg, "GIOP") == 0) {
    Log (GIOP, onoff, file);
  }
  else if (strcmp (msg, "IIOP") == 0) {
    Log (IIOP, onoff, file);
  }
  else if (strcmp (msg, "Transport") == 0) {
    Log (Transport, onoff, file);
  }
  else if (strcmp (msg, "Thread") == 0) {
    Log (Thread, onoff, file);
  }
  else if (strcmp (msg, "POA") == 0) {
    Log (POA, onoff, file);
  }
  else if (strcmp (msg, "ORB") == 0) {
    Log (ORB, onoff, file);
  }
  else if (strcmp (msg, "Support") == 0) {
    Log (Support, onoff, file);
  }
  else if (strcmp (msg, "PI") == 0) {
    Log (PI, onoff, file);
  }
  else if (strcmp (msg, "Security") == 0) {
    Log (Security, onoff, file);
  }
  else if (strcmp (msg, "Exception") == 0) {
    Log (Exception, onoff, file);
  }
  else if (strcmp (msg, "All") == 0) {
    Log (All, onoff, file);
  }
  else {
    if (IsLogged (Warning)) {
      Stream (Warning)
	<< "Warning: No such debug level" << msg << " is available." << endl;
    }
  }
}

/***************************** MICOGetOpt ******************************/


MICOGetOpt::MICOGetOpt (const OptMap &opts)
    : _in_opts (opts)
{
}

MICOGetOpt::~MICOGetOpt ()
{
}

CORBA::Boolean
MICOGetOpt::parse (int &argc, char *argv[], CORBA::Boolean ignore)
{
    vector<int> erase;
    vector<string> args;

    int i;
    for (i = 1; i < argc; ++i)
	args.push_back (argv[i]);
    if (!parse (args, erase, ignore))
	return FALSE;

    int nargc = 0;
    for (i = 0; i < argc; ++i) {
	if (erase.size() > 0 && i == erase[0]+1) {
	    erase.erase (erase.begin());
	} else {
	    argv[nargc++] = argv[i];
	}
    }
    if (nargc < argc)
	argv[nargc] = 0;
    argc = nargc;

    return TRUE;
}

CORBA::Boolean
MICOGetOpt::parse (const vector<string> &argv, vector<int> &erase,
		   CORBA::Boolean ignore)
{
    for (mico_vec_size_type i = 0; i < argv.size(); ++i) {
	string arg = argv[i];
	if (arg == "--") {
	    erase.push_back (i);
	    break;
	}
	if (!ignore && (arg.size() == 0 || arg[0] != '-')) {
	    break;
	}
	OptMap::const_iterator it = _in_opts.find (arg);
	if (it == _in_opts.end()) {
	    int pos = 0;
	    if (arg.length() > 2) {
		// -Oval ??
		arg = arg.substr (0, 2);
		pos = 2;
		it = _in_opts.find (arg);
	    }
	    if (it == _in_opts.end() && (pos = argv[i].find ("=")) > 0) {
		// --long-opt=something ??
		arg = argv[i].substr (0, pos);
		++pos;
		it = _in_opts.find (arg);
	    }
	    if (it != _in_opts.end()) {
		if ((*it).second.length() == 0) {
		    cerr << "unexpected argument for option " << arg << endl;
		    return FALSE;
		}
		_out_opts.push_back (make_pair (arg, argv[i].substr (pos)));
		erase.push_back (i);
	    } else if (!ignore) {
		cerr << "unknown option: " << argv[i] << endl;
		return FALSE;
	    }
	} else {
	    erase.push_back (i);
	    if ((*it).second.length() > 0) {
		// -O val
		// --long-opt val
		if (++i == argv.size()) {
		    cerr << "missing argument for option " << arg << endl;
		    return FALSE;
		}
		_out_opts.push_back (make_pair (arg, argv[i]));
		erase.push_back (i);
	    } else {
		// -O
		// --long-opt
		_out_opts.push_back (make_pair (arg, string("")));
	    }
	}
    }
    return TRUE;
}

CORBA::Boolean
MICOGetOpt::parse (const vector<string> &argv, CORBA::Boolean ignore)
{
    vector<int> erase;
    return parse (argv, erase, ignore);
}

CORBA::Boolean
MICOGetOpt::parse (const string &_filename, CORBA::Boolean ignore)
{
    string fn = _filename;

    if (fn[0] == '~') {
        char *s = getenv ("HOME");
        if (!s) {
            return TRUE;
        }
        string home = s;
        fn.replace (0, 1, home);
    }
    ifstream in (fn.c_str());
    if (!in)
	return TRUE;

    char line[10000], *lptr, *tok;
    vector<string> argv;

    while (42) {
	if (in.getline(line, sizeof(line)).eof())
	    break;
	if (!line[0])
	    continue;
	lptr = line;
	while ((tok = ::strtok (lptr, " \t"))) {
	    if (lptr && *tok == '#')
		break;
	    argv.push_back (tok);
	    lptr = 0;
	}
    }
    return parse (argv, ignore);
}

const MICOGetOpt::OptVec &
MICOGetOpt::opts () const
{
    return _out_opts;
}


#ifdef USE_CSIV2

vector<unsigned int>
ASN1::Codec::parse_oid(const char* __oid)
{
    string __s_tmp = __oid;
    string __str;
    if (__s_tmp.find(':') > 0)
	__str = __s_tmp.substr(__s_tmp.find(':') + 1);
    else
	__str = __s_tmp;
    vector<unsigned int> __buf;
    string __s1;
    int __ix = 0;
    for (;;) {
	__ix = __str.find('.');
	if (__ix > 0) {
	    __s1 = __str.substr(0, __ix);
	    __str = __str.substr(__ix + 1, __str.length());
	    __buf.push_back(atoi(__s1.c_str()));
	}
	else {
	    __buf.push_back(atoi(__str.c_str()));
	    break;
	}
    }
    return __buf;
}

char*
ASN1::Codec::decode_oid(CORBA::OctetSeq& __oid)
{
    assert(__oid[0] == 6); // OID Tag = 6
    int __len = __oid[1] + 2;
    vector<unsigned int> __buf;
    vector<unsigned int> __big_num;
    string __res;
    int __first = __oid[2] / 40; // first number can be 0/1/2
    int __second = __oid[2] - __first * 40;
    int __i;
    for (__i=3; __i<__len; __i++) {
	if (__oid[__i] < 127) {
	    if (__big_num.size () == 0)
		__buf.push_back(__oid[__i]);
	    else {
		unsigned int __sum = 0;
		for (CORBA::ULong __j=0; __j<__big_num.size(); __j++)
		    __sum += __big_num[__j];
		__sum += __oid[__i];
		__big_num.erase(__big_num.begin(), __big_num.end());
		__buf.push_back(__sum);
	    }
	}
	else {
	    __big_num.push_back(__oid[__i] % 128);
	    for (CORBA::ULong __j=0; __j<__big_num.size(); __j++) {
		unsigned int __tmp = __big_num[__j];
		__big_num[__j] = __tmp * 128;
	    }
	}
    }
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
    stringstream __sstr;
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
    ostrstream __sstr;
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS
    __sstr << "oid:" << __first << "." << __second << ".";
    int __x = __buf.size();
    for (__i = 0; __i < __x-1; __i++) {
	__sstr << __buf[__i] << ".";
    }
    __sstr << __buf[__x-1];
    __res = __sstr.str();
    return CORBA::string_dup(__res.c_str());
}

CORBA::OctetSeq*
ASN1::Codec::encode_oid(const char* __o)
{
    vector<unsigned int> __oid = ASN1::Codec::parse_oid(__o);
    assert(__oid.size() < 127);
    vector<unsigned char> __buf;
    __buf.push_back(6); // OID Tag
    __buf.push_back(0); // temporary length
    int __oid_len = __oid.size();
    int __len = 1;
    __buf.push_back(40 * __oid[0] + __oid[1]);
    for (int __i=2; __i<__oid_len; __i++) {
	if (__oid[__i] < 127) {
	    __buf.push_back(__oid[__i]);
	    __len++;
	}
	else {
	    int __div = __oid[__i];
	    int __tmp = 0;
	    vector<char> __tmp_buf;
	    while(__div) {
		__tmp = __div;
		__div /= 128;
		if (__tmp_buf.size() == 0)
		    __tmp_buf.push_back(__tmp % 128);
		else
		    __tmp_buf.push_back((__tmp % 128) | 0x80);
	    }
	    reverse(__tmp_buf.begin(), __tmp_buf.end());
	    for (vector<char>::iterator __i=__tmp_buf.begin(); __i!=__tmp_buf.end(); __i++) {
		__buf.push_back(*__i);
		__len++;
	    }
	}
    }
    __buf[1] = __len;
    CORBA::OctetSeq* __ret = new CORBA::OctetSeq;
    __ret->length(__buf.size());
    for (CORBA::ULong __j=0; __j<__buf.size(); __j++)
	(*__ret)[__j] = __buf[__j];
    return __ret;
}

#endif // USE_CSIV2

/************************** repo id matcher ***************************/


CORBA::Boolean
mico_fnmatch (const char *s, const char *p)
{
    register int scc;
    int ok, lc;
    int c, cc;

    for (;;) {
	scc = *s++ & 0177;
	switch ((c = *p++)) {
	case '[':
	    ok = 0;
	    lc = 077777;
	    while ((cc = *p++)) {
		if (cc == ']') {
		    if (ok)
			break;
		    return FALSE;
		}
		if (cc == '-') {
		    if (lc <= scc && scc <= *p++)
			ok++;
		} else
		    if (scc == (lc = cc))
			ok++;
	    }
	    if (cc == 0)
		if (ok)
		    p--;
		else
		    return FALSE;
	    continue;
	    
	case '*':
	    if (!*p)
		return TRUE;
	    s--;
	    do {
		if (mico_fnmatch(s, p))
		    return TRUE;
	    } while (*s++);
	    return FALSE;
			
	case 0:
	    return (scc == 0);

	default:
	    if (c != scc)
		return FALSE;
	    continue;

	case '?':
	    if (scc == 0)
		return FALSE;
	    continue;
	}
    }
}


/****************************** Process ******************************/


MICO::Process::~Process ()
{
}

MICO::ProcessCallback::~ProcessCallback ()
{
}


/***************************** SharedLib ******************************/


CORBA::Boolean
MICO::SharedLib::init ()
{
#ifdef HAVE_EXPLICIT_CTORS
    void (*gblctor) () = (void (*) ())symbol ("_GLOBAL__DI");
    assert (gblctor);
    (*gblctor) ();
#endif
    CORBA::Boolean (*initfn) (const char *) =
	(CORBA::Boolean (*) (const char *))symbol ("mico_module_init");
    if (!initfn)
	return FALSE;
    return (*initfn) (MICO_VERSION);
}

void
MICO::SharedLib::exit ()
{
    void (*exitfn) () = (void (*) ())symbol ("mico_module_exit");
    if (exitfn)
	(*exitfn) ();
#ifdef HAVE_EXPLICIT_CTORS
    void (*gbldtor) () = (void (*) ())symbol ("_GLOBAL__DD");
    assert (gbldtor);
    (*gbldtor) ();
#endif
}

MICO::SharedLib::~SharedLib ()
{
}


/********************** Floating point converters *********************/


struct IeeeLDouble {
#ifdef HAVE_BYTEORDER_BE
    unsigned int s : 1;
    unsigned int e : 15;
    unsigned int f1 : 16;
    unsigned int f2 : 32;
    unsigned int f3 : 32;
    unsigned int f4 : 32;
#else
    unsigned int f4 : 32;
    unsigned int f3 : 32;
    unsigned int f2 : 32;
    unsigned int f1 : 16;
    unsigned int e : 15;
    unsigned int s : 1;
#endif
};

#define LDBL_EXP_BITS  15
#define LDBL_EXP_BIAS  16384
#define LDBL_EXP_MAX   ((1L << LDBL_EXP_BITS) - 1 - LDBL_EXP_BIAS)
#define LDBL_EXP_MIN   (1 - LDBL_EXP_BIAS)
#define LDBL_FRC1_BITS 16
#define LDBL_FRC2_BITS (LDBL_FRC1_BITS+32)
#define LDBL_FRC3_BITS (LDBL_FRC2_BITS+32)
#define LDBL_FRC_BITS  (LDBL_FRC3_BITS+32)

struct IeeeDouble {
#ifdef HAVE_BYTEORDER_BE
    unsigned int s : 1;
    unsigned int e : 11;
    unsigned int f1 : 20;
    unsigned int f2 : 32;
#else
    unsigned int f2 : 32;
    unsigned int f1 : 20;
    unsigned int e : 11;
    unsigned int s : 1;
#endif
};

#define DBL_EXP_BITS  11
#define DBL_EXP_BIAS  1023
#define DBL_EXP_MAX   ((1L << DBL_EXP_BITS) - 1 - DBL_EXP_BIAS)
#define DBL_EXP_MIN   (1 - DBL_EXP_BIAS)
#define DBL_FRC1_BITS 20
#define DBL_FRC2_BITS 32
#define DBL_FRC_BITS  (DBL_FRC1_BITS + DBL_FRC2_BITS)

struct IeeeFloat {
#ifdef HAVE_BYTEORDER_BE
    unsigned int s : 1;
    unsigned int e : 8;
    unsigned int f : 23;
#else
    unsigned int f : 23;
    unsigned int e : 8;
    unsigned int s : 1;
#endif
};

#define FLT_EXP_BITS 8
#define FLT_EXP_BIAS 127
#define FLT_EXP_MAX  ((1L << FLT_EXP_BITS) - 1 - FLT_EXP_BIAS)
#define FLT_EXP_MIN  (1 - FLT_EXP_BIAS)
#define FLT_FRC_BITS 23


void
mico_ieee2ldouble (CORBA::Octet ieee[16], CORBA::LongDouble &d)
{
    IeeeLDouble &ie = (IeeeLDouble &)*ieee;

    if (ie.e == 0) {
	if (ie.f1 == 0 && ie.f2 == 0 && ie.f3 == 0 && ie.f4 == 0) {
	    // zero
	    d = 0.0;
	} else {
	    // denormalized number
	    d  = ldexpl ((long double)ie.f1, -LDBL_FRC1_BITS + LDBL_EXP_MIN);
	    d += ldexpl ((long double)ie.f2, -LDBL_FRC2_BITS + LDBL_EXP_MIN);
	    d += ldexpl ((long double)ie.f3, -LDBL_FRC3_BITS + LDBL_EXP_MIN);
	    d += ldexpl ((long double)ie.f4, -LDBL_FRC_BITS  + LDBL_EXP_MIN);
	    if (ie.s)
		d = -d;
	}
    } else if (ie.e == LDBL_EXP_MAX + LDBL_EXP_BIAS) {
	if (ie.f1 == 0 && ie.f2 == 0 && ie.f3 == 0 && ie.f4 == 0) {
	    // +/- infinity
	    d = OSMath::infinityl (ie.s);
	} else {
	    // not a number
	    d = OSMath::nanl ();
	}
    } else {
	// normalized number
	d = ldexpl (ldexpl ((long double)ie.f1, -LDBL_FRC1_BITS) +
		    ldexpl ((long double)ie.f2, -LDBL_FRC2_BITS) +
		    ldexpl ((long double)ie.f3, -LDBL_FRC3_BITS) +
		    ldexpl ((long double)ie.f4, -LDBL_FRC_BITS) +
                    1.0,
		    ie.e - LDBL_EXP_BIAS);
	if (ie.s)
	    d = -d;
    }
}

void
mico_ldouble2ieee (CORBA::Octet ieee[16], CORBA::LongDouble d)
{
    IeeeLDouble &ie = (IeeeLDouble &)*ieee;

    if (OSMath::is_nanl (d)) {
	// XXX not a number (what is signaling NAN ???)
	ie.s = 0;
	ie.e = LDBL_EXP_MAX + LDBL_EXP_BIAS;
	ie.f1 = 1;
	ie.f2 = 1;
	ie.f3 = 1;
	ie.f4 = 1;
    } else if (OSMath::is_infinityl (d)) {
	// +/- infinity
	ie.s = (d < 0);
	ie.e = LDBL_EXP_MAX + LDBL_EXP_BIAS;
	ie.f1 = 0;
	ie.f2 = 0;
	ie.f3 = 0;
	ie.f4 = 0;
    } else if (d == 0.0) {
	// zero
	ie.s = 0;
	ie.e = 0;
	ie.f1 = 0;
	ie.f2 = 0;
	ie.f3 = 0;
	ie.f4 = 0;
    } else {
	// finite number
	int exp;
	long double frac = frexpl (fabsl (d), &exp);

	while (frac < 1.0 && exp >= LDBL_EXP_MIN) {
	    frac = ldexpl (frac, 1);
	    --exp;
	}
	if (exp < LDBL_EXP_MIN) {
	    // denormalized number (or zero)
	    frac = ldexpl (frac, exp - LDBL_EXP_MIN);
	    exp = 0;
	} else {
	    // normalized number
	    assert (1.0 <= frac && frac < 2.0);
	    assert (LDBL_EXP_MIN <= exp && exp <= LDBL_EXP_MAX);

	    exp += LDBL_EXP_BIAS;
            frac -= 1.0;
	}
	ie.s = (d < 0);
	ie.e = exp;
	ie.f1 = (unsigned long)ldexpl (frac, LDBL_FRC1_BITS);
	ie.f2 = (unsigned long)ldexpl (frac, LDBL_FRC2_BITS);
	ie.f3 = (unsigned long)ldexpl (frac, LDBL_FRC3_BITS);
	ie.f4 = (unsigned long)ldexpl (frac, LDBL_FRC_BITS);
    }
}

void
mico_ieee2double (CORBA::Octet ieee[8], CORBA::Double &d)
{
    IeeeDouble &ie = (IeeeDouble &)*ieee;

    if (ie.e == 0) {
	if (ie.f1 == 0 && ie.f2 == 0) {
	    // zero
	    d = 0.0;
	} else {
	    // denormalized number
	    d  = ldexp ((double)ie.f1, -DBL_FRC1_BITS + DBL_EXP_MIN);
	    d += ldexp ((double)ie.f2, -DBL_FRC_BITS  + DBL_EXP_MIN);
	    if (ie.s)
		d = -d;
	}
    } else if (ie.e == DBL_EXP_MAX + DBL_EXP_BIAS) {
	if (ie.f1 == 0 && ie.f2 == 0) {
	    // +/- infinity
	    d = OSMath::infinity (ie.s);
	} else {
	    // not a number
	    d = OSMath::nan ();
	}
    } else {
	// normalized number
	d = ldexp (ldexp ((double)ie.f1, -DBL_FRC1_BITS) +
		   ldexp ((double)ie.f2, -DBL_FRC_BITS) +
		   1.0,
		   ie.e - DBL_EXP_BIAS);
	if (ie.s)
	    d = -d;
    }
}

void
mico_double2ieee (CORBA::Octet ieee[8], CORBA::Double d)
{
    IeeeDouble &ie = (IeeeDouble &)*ieee;

    if (OSMath::is_nan (d)) {
	// XXX not a number (what is signaling NAN ???)
	ie.s = 0;
	ie.e = DBL_EXP_MAX + DBL_EXP_BIAS;
	ie.f1 = 1;
	ie.f2 = 1;
    } else if (OSMath::is_infinity (d)) {
	// +/- infinity
	ie.s = (d < 0);
	ie.e = DBL_EXP_MAX + DBL_EXP_BIAS;
	ie.f1 = 0;
	ie.f2 = 0;
    } else if (d == 0.0) {
	// zero
	ie.s = 0;
	ie.e = 0;
	ie.f1 = 0;
	ie.f2 = 0;
    } else {
	// finite number
	int exp;
	double frac = frexp (fabs (d), &exp);

	while (frac < 1.0 && exp >= DBL_EXP_MIN) {
	    frac = ldexp (frac, 1);
	    --exp;
	}
	if (exp < DBL_EXP_MIN) {
	    // denormalized number (or zero)
	    frac = ldexp (frac, exp - DBL_EXP_MIN);
	    exp = 0;
	} else {
	    // normalized number
	    assert (1.0 <= frac && frac < 2.0);
	    assert (DBL_EXP_MIN <= exp && exp <= DBL_EXP_MAX);

	    exp += DBL_EXP_BIAS;
	    frac -= 1.0;
	}
	ie.s = (d < 0);
	ie.e = exp;
	ie.f1 = (unsigned long)ldexp (frac, DBL_FRC1_BITS);
        // kcg: what's right, this: (following two lines fixes double
        // encoding issue on big-endian hosts (sparc/ppc64), test case
        // is test/idl/37
        double mantissa = ldexp( frac , DBL_FRC_BITS );
        ie.f2 = (unsigned long)fmod( mantissa , 4294967296.0 ); // 2^32 = 4294967296
        // kcg: or this:
//	ie.f2 = (unsigned long)ldexp (frac, DBL_FRC_BITS);
    }
}

void
mico_ieee2float (CORBA::Octet ieee[4], CORBA::Float &f)
{
    IeeeFloat &ie = (IeeeFloat &)*ieee;

    if (ie.e == 0) {
	if (ie.f == 0) {
	    // zero
	    f = 0.0;
	} else {
	    // denormalized number
	    f = ldexp ((double)ie.f, -FLT_FRC_BITS + FLT_EXP_MIN);
	    if (ie.s)
		f = -f;
	}
    } else if (ie.e == FLT_EXP_MAX + FLT_EXP_BIAS) {
	if (ie.f == 0) {
	    // +/- infinity
	    f = OSMath::infinity (ie.s);
	} else {
	    // not a number
	    f = OSMath::nan ();
	}
    } else {
	// normalized number
	f = ldexp (ldexp ((double)ie.f, -FLT_FRC_BITS) + 1.0,
		   ie.e - FLT_EXP_BIAS);
	if (ie.s)
	    f = -f;
    }
}

void
mico_float2ieee (CORBA::Octet ieee[4], CORBA::Float f)
{
    IeeeFloat &ie = (IeeeFloat &)*ieee;

    if (OSMath::is_nan (f)) {
	// XXX not a number (what is signaling NAN ???)
	ie.s = 0;
	ie.e = FLT_EXP_MAX + FLT_EXP_BIAS;
	ie.f = 1;
    } else if (OSMath::is_infinity (f)) {
	// +/- infinity
	ie.s = (f < 0);
	ie.e = FLT_EXP_MAX + FLT_EXP_BIAS;
	ie.f = 0;
    } else if (f == 0.0) {
	// zero
	ie.s = 0;
	ie.e = 0;
	ie.f = 0;
    } else {
	// finite number
	int exp;
	double frac = frexp (fabs (f), &exp);

	while (frac < 1.0 && exp >= FLT_EXP_MIN) {
	    frac = ldexp (frac, 1);
	    --exp;
	}
	if (exp < FLT_EXP_MIN) {
	    // denormalized number (or zero)
	    frac = ldexp (frac, exp - FLT_EXP_MIN);
	    exp = 0;
	} else {
	    // normalized number
	    assert (1.0 <= frac && frac < 2.0);
	    assert (FLT_EXP_MIN <= exp && exp <= FLT_EXP_MAX);

	    exp += FLT_EXP_BIAS;
	    frac -= 1.0;
	}
	ie.s = (f < 0);
	ie.e = exp;
	ie.f = (unsigned long)ldexp (frac, FLT_FRC_BITS);
    }
}
