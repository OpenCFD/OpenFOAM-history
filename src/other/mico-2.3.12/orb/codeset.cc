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
#define MICO_CONF_IR
#define MICO_CONF_INTERCEPT
#define MICO_CONF_POA

#include <CORBA-SMALL.h>
#include <string.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif
#include <mico/util.h>
#include <mico/impl.h>
#include <mico/template_impl.h>

#include <mico/code_set_registry.h>

#endif // FAST_PCH


using namespace std;

namespace CORBA {
  CORBA::Codeset *Codeset::_special[CORBA::Codeset::_SpecialMax] = { 0 };
  CORBA::Boolean Codeset::_disabled = FALSE;
  CORBA::Boolean Codeset::_enabled_in_iiopprofile = FALSE;
}

CORBA::Codeset::Codeset()
{
    assert (0);
}

CORBA::Codeset::Codeset (Info *info)
{
    _info = info;
}

CORBA::Codeset::Codeset (const Codeset &cs)
{
    _info = cs._info;
}

CORBA::Codeset::~Codeset()
{
}

CORBA::Codeset::Info *
CORBA::Codeset::_find_info (CodesetId id)
{
    for (int i = 0; _osf_cs_reg[i].id; ++i) {
	if (id == _osf_cs_reg[i].id)
	    return &_osf_cs_reg[i];
    }
    return 0;
}

CORBA::Codeset::Info *
CORBA::Codeset::_find_info (const char *pat)
{
    for (int i = 0; _osf_cs_reg[i].id; ++i) {
	if (mico_fnmatch (_osf_cs_reg[i].desc, pat))
	    return &_osf_cs_reg[i];
    }
    return 0;
}

CORBA::Codeset *
CORBA::Codeset::create (CodesetId id)
{
    Info *i = _find_info (id);
    if (!i)
	return 0;
    return new Codeset (i);
}

CORBA::Codeset *
CORBA::Codeset::create (const char *pat)
{
    Info *i = _find_info (pat);
    if (!i)
	return 0;
    return new Codeset (i);
}

CORBA::Codeset *
CORBA::Codeset::special_cs (SpecialCS csid)
{
    assert (_SpecialMin <= csid && csid < _SpecialMax);
    return _special[csid];
}

void
CORBA::Codeset::set_special_cs (SpecialCS csid, Codeset *cs)
{
    assert (_SpecialMin <= csid && csid < _SpecialMax);
    _special[csid] = cs;
}

CORBA::Boolean
CORBA::Codeset::is_compatible (const Codeset *cs) const
{
    // we assume that charset ids are sorted in ascending order
    const CharsetId *cp1 = _info->charsets;
    const CharsetId *cp2 = cs->_info->charsets;

    // they are compatible if they have at least one charset in common
    while (*cp1 && *cp2) {
	if (*cp1 < *cp2)
	    ++cp1;
	else if (*cp1 > *cp2)
	    ++cp2;
	else
	    return TRUE;
    }
    return FALSE;
}

CORBA::Boolean
CORBA::Codeset::is_compatible (CodesetId id) const
{
    Info *i = _find_info (id);
    if (!i)
	return FALSE;

    Codeset cs (i);

    return is_compatible (&cs);
}

void
CORBA::Codeset::free()
{
	CORBA::Codeset* cs = NULL;
	cs = CORBA::Codeset::special_cs(CORBA::Codeset::NativeCS);
	if (cs)
	    delete cs;
	cs = CORBA::Codeset::special_cs(CORBA::Codeset::NativeWCS);
	if (cs)
	    delete cs;
	cs = CORBA::Codeset::special_cs(CORBA::Codeset::FallbackCS);
	if (cs)
	    delete cs;
	cs = CORBA::Codeset::special_cs(CORBA::Codeset::FallbackWCS);
	if (cs)
	    delete cs;
	cs = CORBA::Codeset::special_cs(CORBA::Codeset::DefaultCS);
	if (cs)
	    delete cs;
	cs = CORBA::Codeset::special_cs(CORBA::Codeset::DefaultWCS);
	if (cs)
	    delete cs;
	CORBA::Codeset::set_special_cs(CORBA::Codeset::NativeCS, NULL);
	CORBA::Codeset::set_special_cs(CORBA::Codeset::NativeWCS, NULL);
	CORBA::Codeset::set_special_cs(CORBA::Codeset::FallbackCS, NULL);
	CORBA::Codeset::set_special_cs(CORBA::Codeset::FallbackWCS, NULL);
	CORBA::Codeset::set_special_cs(CORBA::Codeset::DefaultCS, NULL);
	CORBA::Codeset::set_special_cs(CORBA::Codeset::DefaultWCS, NULL);
}

//-----------------


MICO::CodesetConv::CodesetConv ()
{
    assert (0);
}

MICO::CodesetConv::CodesetConv (CORBA::Codeset *f, CORBA::Codeset *t)
{
    _from = f;
    _to = t;
}

MICO::CodesetConv::CodesetConv (const CodesetConv &csc)
{
    _from = new CORBA::Codeset (*csc._from);
    _to = new CORBA::Codeset (*csc._to);
}

MICO::CodesetConv::~CodesetConv ()
{
    delete _from;
    delete _to;
}

MICO::CodesetConv *
MICO::CodesetConv::create (CORBA::Codeset *f, CORBA::Codeset *t)
{
    if (f->id() == t->id())
	return new CodesetConv (f, t);
    return UniCodesetConv::create (f, t);
}

CORBA::Boolean
MICO::CodesetConv::can_convert (CORBA::Codeset::CodesetId from,
				CORBA::Codeset::CodesetId to)
{
    if (from == to)
	return TRUE;
    return UniCodesetConv::can_convert (from, to);
}

MICO::CodesetConv *
MICO::CodesetConv::clone () const
{
    return new CodesetConv (*this);
}

CORBA::Long
MICO::CodesetConv::encode (const CORBA::Char *f, CORBA::ULong len,
			   CORBA::Buffer &t, CORBA::Boolean terminate)
{
    switch (_from->codepoint_size()) {
    case 1: {
	t.put (f, len);
	if (terminate)
	    t.put (0);
	break;
    }
    case 2: {
        CORBA::UShort b;
	for (CORBA::Long i = len; --i >= 0; ) {
	  b = (CORBA::UShort)*f++;
	  t.put (&b, 2);
	}
	if (terminate) {
	  b = 0;
	  t.put (&b, 2);
	}
	break;
    }
    case 3:
    case 4: {
        CORBA::ULong b;
	for (CORBA::Long i = len; --i >= 0; ) {
	  b = (CORBA::ULong) *f++;
	  t.put (&b, 4);
	}
	if (terminate) {
	  b = 0;
	  t.put (&b, 4);
	}
	break;
    }
    default:
	assert (0);
    }
    // number of codepoints written
    return len + !!terminate;
}

CORBA::Long
MICO::CodesetConv::encode (const CORBA::WChar *f, CORBA::ULong len,
			   CORBA::Buffer &t, CORBA::Boolean terminate)
{
    switch (_from->codepoint_size()) {
    case 1: {
	for (CORBA::Long i = len; --i >= 0; )
	    t.put ((CORBA::Char)*f++);
	if (terminate)
	    t.put (0);
	break;
    }
    case 2: {
        CORBA::UShort b;
	for (CORBA::Long i = len; --i >= 0; ) {
	  b = (CORBA::UShort)*f++;
	  t.put (&b, 2);
	}
	if (terminate) {
	  b = 0;
	  t.put (&b, 2);
	}
	break;
    }
    case 3:
    case 4: {
        CORBA::ULong b;
	for (CORBA::Long i = len; --i >= 0; ) {
	  b = (CORBA::ULong) *f++;
	  t.put (&b, 4);
	}
	if (terminate) {
	  b = 0;
	  t.put (&b, 4);
	}
	break;
    }
    default:
	assert (0);
    }
    // number of codepoints written
    return len + !!terminate;
}

CORBA::Long
MICO::CodesetConv::decode (CORBA::Buffer &f, CORBA::ULong len,
			   CORBA::Char *t, CORBA::Boolean terminate)
{
    switch (_from->codepoint_size()) {
    case 1: {
	if (!f.get (t, len))
	    return -1;
	if (terminate)
	    t[len] = 0;
	break;
    }
    case 2: {
	CORBA::UShort us;
	for (CORBA::Long i = len; --i >= 0; ) {
	    if (!f.get (&us, 2))
		return -1;
	    *t++ = (CORBA::Char)us;
	}
	if (terminate)
	    *t = 0;
	break;
    }
    case 3:
    case 4: {
	CORBA::ULong ul;
	for (CORBA::Long i = len; --i >= 0; ) {
	    if (!f.get (&ul, 4))
		return -1;
	    *t++ = (CORBA::Char)ul;
	}
	if (terminate)
	    *t = 0;
	break;
    }
    default:
	assert (0);
    }
    // number of codepoints read
    return len;
}

CORBA::Long
MICO::CodesetConv::decode (CORBA::Buffer &f, CORBA::ULong len,
			   CORBA::WChar *t, CORBA::Boolean terminate)
{
    switch (_from->codepoint_size()) {
    case 1: {
	CORBA::Octet ch;
	for (CORBA::Long i = len; --i >= 0; ) {
	    if (!f.get (ch))
		return -1;
	    *t++ = (CORBA::WChar)ch;
	}
	if (terminate)
	    *t = 0;
	break;
    }
    case 2: {
	CORBA::UShort us;
	for (CORBA::Long i = len; --i >= 0; ) {
	    if (!f.get (&us, 2))
		return -1;
	    *t++ = (CORBA::WChar)us;
	}
	if (terminate)
	    *t = 0;
	break;
    }
    case 3:
    case 4: {
	CORBA::ULong ul;
	for (CORBA::Long i = len; --i >= 0; ) {
	    if (!f.get (&ul, 4))
		return -1;
	    *t++ = (CORBA::WChar)ul;
	}
	if (terminate)
	    *t = 0;
	break;
    }
    default:
	assert (0);
    }
    // number of codepoints read
    return len;
}

//-----------------


#include "uni_conversion.h"
#include "uni_strtypes.h"
#include "uni_errors.h"

MICO::UniCodesetConv::UniCodesetConv ()
{
    assert (0);
}

MICO::UniCodesetConv::UniCodesetConv (CORBA::Codeset *f, CORBA::Codeset *t)
    : MICO::CodesetConv (f, t)
{
}

MICO::UniCodesetConv::UniCodesetConv (const UniCodesetConv &csc)
    : MICO::CodesetConv (csc)
{
}

MICO::UniCodesetConv::~UniCodesetConv ()
{
}

CORBA::Boolean
MICO::UniCodesetConv::supported_csid (CORBA::Codeset::CodesetId csid)
{
    switch (csid) {
    case C_ISO8859_1:
    case C_ISO8859_2:
    case C_ISO8859_3:
    case C_ISO8859_4:
    case C_ISO8859_5:
    case C_ISO8859_6:
    case C_ISO8859_7:
    case C_ISO8859_8:
    case C_ISO8859_9:
    case C_ISO8859_10:
    case C_ISO646:
    case C_UCS2:
    case C_IBM_437:
    case C_IBM_850:
    case C_IBM_852:
    case C_IBM_860:
    case C_IBM_863:
    case C_IBM_865:
    case C_UCS4:
    case C_UTF16:
    case C_UTF8:
    case (uni_ulong)C_WIN31_LATIN1:
    case (uni_ulong)C_ASCII7:
    case (uni_ulong)C_EBCDIC:
    case (uni_ulong)C_MACINTOSH:
    case (uni_ulong)C_KOI8_R:
    case (uni_ulong)C_UTF7:
    case (uni_ulong)C_HTML3:
	return TRUE;
    default:
	return FALSE;
    }
}

MICO::CodesetConv *
MICO::UniCodesetConv::create (CORBA::Codeset *f, CORBA::Codeset *t)
{
    if (can_convert (f->id(), t->id()))
	return new UniCodesetConv (f, t);
    return 0;
}

CORBA::Boolean
MICO::UniCodesetConv::can_convert (CORBA::Codeset::CodesetId from,
				   CORBA::Codeset::CodesetId to)
{
    return supported_csid (from) && supported_csid (to);
}

MICO::CodesetConv *
MICO::UniCodesetConv::clone () const
{
    return new UniCodesetConv (*this);
}

CORBA::Long
MICO::UniCodesetConv::convert (const char *f, CORBA::ULong len, char *t)
{
    if (_from->id() == C_UTF8) {
	CORBA::ULong chars = len, utf_read = 0, written = 0;

	CORBA::Long ret = uni_fromUTF8 (t, f, &chars, &utf_read, _to->id(),
					C_LINE_LF, &written);

	if (ret != C_OK) {
	  if (MICO::Logger::IsLogged (MICO::Logger::Error)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::Error)
	      << "Error: codeset conversion error in stage 1" << endl;
	  }
	  return -1;
	}
	return written;
    }

    if (_to->id() == C_UTF8) {
	CORBA::ULong written = 0;

	CORBA::Long ret = uni_toUTF8 (t, f, len, _from->id(),
				      C_LINE_LF, &written);

	if (ret != C_OK) {
	  if (MICO::Logger::IsLogged (MICO::Logger::Error)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::Error)
	      << "Error: codeset conversion error in stage 2" << endl;
	  }
	  return -1;
	}
	return written;
    }

    // UTF8 has max 6 code points per char
#ifdef __GNUG__
    char tmp[len*6+1];
#else
    CORBA::String_var tmp = CORBA::string_alloc (len*6);
#endif

    CORBA::ULong chars, utf_read, written;
    CORBA::Long ret = uni_toUTF8 (tmp, f,
				  len, _from->id(), C_LINE_LF, &chars);

    if (ret != C_OK) {
      if (MICO::Logger::IsLogged (MICO::Logger::Error)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Error)
	  << "Error: codeset conversion error in stage 3" << endl;
      }
      return -1;
    }

    ret = uni_fromUTF8 (t, tmp, &chars, &utf_read, _to->id(),
			C_LINE_LF, &written);

    if (ret != C_OK) {
      if (MICO::Logger::IsLogged (MICO::Logger::Error)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Error)
	  << "Error: codeset conversion error in stage 4" << endl;
      }
      return -1;
    }
    return written;
}

CORBA::Long
MICO::UniCodesetConv::encode (const CORBA::Char *f, CORBA::ULong len,
			      CORBA::Buffer &t, CORBA::Boolean terminate)
{
    char *from = NULL, *to = NULL, *cp = NULL;

    CORBA::ULong flen = _from->codepoint_size();
    assert (flen == 1 || flen == 2 || flen == 4);
    flen *= len;

#ifdef __GNUG__
    char _f[flen+1];
#else
    CORBA::String_var _f;
    if (_from->codepoint_size() != 1)
	_f = CORBA::string_alloc (flen);
#endif

    switch (_from->codepoint_size()) {
    case 1:
	from = (char *)f;
	break;
    case 2:
        from = cp = (char*) _f;
        {
	  for (CORBA::Long i = len; --i >= 0; cp += 2)
	    *(CORBA::UShort *)cp = (CORBA::UShort)*f++;
	}
	break;
    case 3:
    case 4:
        from = cp = (char*) _f;
        {
	  for (CORBA::Long i = len; --i >= 0; cp += 4)
	    *(CORBA::ULong *)cp = (CORBA::ULong)*f++;
	}
	break;
    default:
	assert (0);
    }

    CORBA::ULong tlen = _to->codepoint_size();
    if (tlen == 3)
	tlen = 4;
    tlen *= len * _to->max_codepoints();

#ifdef __GNUG__
    char _t[tlen+4];
#else
    CORBA::String_var _t;
    _t = CORBA::string_alloc (tlen);
#endif

    to = (char*) _t;

    CORBA::Long written = convert (from, flen, to);
    if (written < 0)
	return written;
    if (!terminate)
	--written;

    t.put (to, written * _to->codepoint_size());

    return written;
}

CORBA::Long
MICO::UniCodesetConv::encode (const CORBA::WChar *f, CORBA::ULong len,
			      CORBA::Buffer &t, CORBA::Boolean terminate)
{
    char *cp = NULL, *from = NULL;

    CORBA::ULong flen = _from->codepoint_size();
    assert (flen == 1 || flen == 2 || flen == 4);
    assert (sizeof (CORBA::WChar) == 4);
    flen *= len;

#ifdef __GNUG__
    char _f[flen+1];
#else
    CORBA::String_var _f;
    if (_from->codepoint_size() < 3)
	_f = CORBA::string_alloc (flen);
#endif

    switch (_from->codepoint_size()) {
    case 1:
        from = cp = (char*) _f;
        {
	  for (CORBA::Long i = len; --i >= 0; ++cp)
	    *cp = (CORBA::Char)*f++;
	}
	break;
    case 2:
        from = cp = (char*) _f;
        {
	  for (CORBA::Long i = len; --i >= 0; cp += 2)
	    *(CORBA::UShort *)cp = (CORBA::UShort)*f++;
	}
	break;
    case 3:
    case 4:
	from = (char *)f;
	break;
    default:
	assert (0);
    }

    CORBA::ULong tlen = _to->codepoint_size();
    if (tlen == 3)
	tlen = 4;
    tlen *= len * _to->max_codepoints();

#ifdef __GNUG__
    char _t[tlen+4];
#else
    CORBA::String_var _t = CORBA::string_alloc (tlen);
#endif

    CORBA::Long written = convert (from, len, _t);
    if (written < 0)
	return written;
    if (!terminate)
	--written;

    t.put ((char *) _t, written * _to->codepoint_size());

    return written;
}

CORBA::Long
MICO::UniCodesetConv::decode (CORBA::Buffer &f, CORBA::ULong len,
			      CORBA::Char *t, CORBA::Boolean terminate)
{
    char *cp, *to;

    CORBA::ULong flen = _from->codepoint_size();
    if (flen == 3)
	flen = 4;
    flen *= len;

    if (len * _from->codepoint_size() > f.length()) {
      return -1;
    }

#ifdef __GNUG__
    char _f[flen+1];
#else
    CORBA::String_var _f;
    if (_from->codepoint_size() != 1)
	_f = CORBA::string_alloc (flen);
#endif

    if (!f.get (_f, len * _from->codepoint_size())) {
      return -1;
    }
    _f[len] = 0;

    CORBA::ULong tlen = _to->codepoint_size();
    if (tlen == 3)
	tlen = 4;
    tlen *= len * _to->max_codepoints();

#ifdef __GNUG__
    char _t[tlen+1];
#else
    CORBA::String_var _t;
    if (_to->codepoint_size() != 1)
	_t = CORBA::string_alloc (tlen);
#endif

    if (_to->codepoint_size() == 1) {
      to = t;
    }
    else {
      to = (char*) _t;
    }

    CORBA::Long written = convert (_f, flen, to);

    if (written < 0)
	return written;
    if (!terminate)
	--written;

    switch (_to->codepoint_size()) {
    case 1:
	break;
    case 2:
        cp = (char*) _t;
        {
	  for (CORBA::Long i = written; --i >= 0; cp += 2)
	    *t++ = (CORBA::Char)*(CORBA::UShort *)cp;
	}
	break;
    case 3:
    case 4:
        cp = (char*) _t;
        {
	  for (CORBA::Long i = written; --i >= 0; cp += 4)
	    *t++ = (CORBA::Char)*(CORBA::ULong *)cp;
	}
	break;
    default:
	assert (0);
    }

    return written;
}

CORBA::Long
MICO::UniCodesetConv::decode (CORBA::Buffer &f, CORBA::ULong len,
			      CORBA::WChar *t, CORBA::Boolean terminate)
{
    char *cp, *to;

    CORBA::ULong flen = _from->codepoint_size();
    if (flen == 3)
	flen = 4;
    flen *= len;

    if (len * _from->codepoint_size() > f.length()) {
      return -1;
    }

#ifdef __GNUG__
    char _f[flen+1];
#else
    CORBA::String_var _f = CORBA::string_alloc (flen);
#endif

    if (!f.get (_f, len * _from->codepoint_size())) {
      return -1;
    }
    _f[len * _from->codepoint_size()] = 0;

    CORBA::ULong tlen = _to->codepoint_size();
    if (tlen == 3)
	tlen = 4;
    tlen *= len * _to->max_codepoints();

#ifdef __GNUG__
    char _t[tlen+1];
#else
    CORBA::String_var _t;
    if (_to->codepoint_size() < 3)
	_t = CORBA::string_alloc (tlen);
#endif

    if (_to->codepoint_size() >= 3)
	to = (char *)t;
    else
	to = (char*) _t;

    CORBA::Long written = convert (_f, len, to);

    if (written < 0)
	return written;
    if (!terminate)
	--written;

    switch (_to->codepoint_size()) {
    case 1:
        cp = (char*) _t;
        {
	  for (CORBA::Long i = written; --i >= 0; )
	    *t++ = *cp++;
	}
	break;
    case 2:
        cp = (char*) _t;
        {
	  for (CORBA::Long i = written; --i >= 0; cp += 2)
	    *t++ = (CORBA::WChar)*(CORBA::UShort *)cp;
	}
	break;
    case 3:
    case 4:
	break;
    default:
	assert (0);
    }

    return written;
}

/*
 * ----------------------------------------------------------------------
 * CDR::GIOP_1_0_CodeSetCoder
 * ----------------------------------------------------------------------
 */

CORBA::CodeSetCoder::~CodeSetCoder ()
{
}

/*
 * GIOP 1.0 does not have code set negotiation.
 *  - TCS-C is fixed to ISO 8859-1 (0x00010001)
 *  - there is no TCS-W
 */

MICO::GIOP_1_0_CodeSetCoder::GIOP_1_0_CodeSetCoder ()
{
  CORBA::Codeset::CodesetId nativecs =
    ::CORBA::Codeset::special_cs(::CORBA::Codeset::NativeCS)->id();

  if (nativecs == 0x00010001uL) {
    /*
     * ISO 8859-1 is the native charset, no conversion necessary
     */
    _conv = NULL;
    _isok = TRUE;
  }
  else if (!::MICO::CodesetConv::can_convert (nativecs, 0x00010001uL)) {
    _conv = NULL;
    _isok = FALSE;
  }
  else {
    ::CORBA::Codeset * csnative = ::CORBA::Codeset::create (nativecs);
    ::CORBA::Codeset * cstcs =    ::CORBA::Codeset::create (0x00010001uL);
    assert (csnative && cstcs);

    _conv = ::MICO::CodesetConv::create (csnative, cstcs);
    _isok = (_conv != NULL);
  }
}

MICO::GIOP_1_0_CodeSetCoder::~GIOP_1_0_CodeSetCoder ()
{
  delete _conv;
}

CORBA::CodeSetCoder *
MICO::GIOP_1_0_CodeSetCoder::clone ()
{
  return new GIOP_1_0_CodeSetCoder ();
}

CORBA::Boolean
MICO::GIOP_1_0_CodeSetCoder::isok ()
{
  return _isok;
}

CORBA::Boolean
MICO::GIOP_1_0_CodeSetCoder::get_char (CORBA::DataDecoder & decoder,
				       CORBA::Char & data)
{
  assert (_isok);
  if (!_conv) {
    return decoder.buffer()->get1 (&data);
  }

  if (_conv->decode (*decoder.buffer(), 1, &data, FALSE) != 1) {
    return FALSE;
  }

  return TRUE;
}

CORBA::Boolean
MICO::GIOP_1_0_CodeSetCoder::get_chars (CORBA::DataDecoder & decoder,
					CORBA::Char * data,
					CORBA::ULong count)
{
  assert (_isok);

  if (!_conv) {
    return decoder.buffer()->get (data, count);
  }
  CORBA::Long res;
  if ((res = _conv->decode (*decoder.buffer(), count, data, FALSE)) < 0
      || (CORBA::ULong)res != count) {
    return FALSE;
  }

  return TRUE;
}

CORBA::Boolean
MICO::GIOP_1_0_CodeSetCoder::get_string (CORBA::DataDecoder & decoder,
					 CORBA::String_out data,
					 CORBA::ULong bound)
{
  CORBA::ULong length;
  char * buf;

  assert (_isok);

  if (!decoder.get_ulong (length)) {
    return FALSE;
  }

  if (length == 0 || (bound > 0 && length-1 > bound) ||
      length > decoder.buffer()->length()) {
    return FALSE;
  }

  buf = CORBA::string_alloc (length-1);

  if (!_conv) {
    if (!decoder.buffer()->get (buf, length)) {
      CORBA::string_free (buf);
      return FALSE;
    }
    if (buf[length-1] != '\0') {
      CORBA::string_free (buf);
      return FALSE;
    }
  }
  else {
    CORBA::Long res;
    if ((res = _conv->decode (*decoder.buffer(), length-1, buf, TRUE)) < 0
	|| (CORBA::ULong) res != length) {
      CORBA::string_free (buf);
      return FALSE;
    }
    if (!decoder.buffer()->get1 (&length)) { /* eat up null octet */
      return FALSE;
    }
  }

  data = buf;
  return TRUE;
}

/*
 * There is no wchar in GIOP 1.0, so we refuse to handle them
 */

CORBA::Boolean
MICO::GIOP_1_0_CodeSetCoder::get_wchar (CORBA::DataDecoder &,
					CORBA::WChar &)
{
  return FALSE;
}

CORBA::Boolean
MICO::GIOP_1_0_CodeSetCoder::get_wchars (CORBA::DataDecoder &,
					 CORBA::WChar *,
					 CORBA::ULong)
{
  return FALSE;
}

CORBA::Boolean
MICO::GIOP_1_0_CodeSetCoder::get_wstring (CORBA::DataDecoder &,
					  CORBA::WString_out,
					  CORBA::ULong)
{
  return FALSE;
}

CORBA::Boolean
MICO::GIOP_1_0_CodeSetCoder::put_char (CORBA::DataEncoder & encoder,
				       CORBA::Char data)
{
  assert (_isok);

  if (!_conv) {
    encoder.buffer()->put1 (&data);
    return TRUE;
  }

  if (_conv->encode (&data, 1, *encoder.buffer(), FALSE) != 1) {
    return FALSE;
  }

  return TRUE;
}

CORBA::Boolean
MICO::GIOP_1_0_CodeSetCoder::put_chars (CORBA::DataEncoder & encoder,
					const CORBA::Char * data,
					CORBA::ULong count)
{
  assert (_isok);

  if (!_conv) {
    encoder.buffer()->put (data, count);
    return TRUE;
  }
  CORBA::Long res;
  if ((res = _conv->encode (data, count, *encoder.buffer(), FALSE)) < 0
      || (CORBA::ULong)res != count) {
    return FALSE;
  }

  return TRUE;
}

CORBA::Boolean
MICO::GIOP_1_0_CodeSetCoder::put_string (CORBA::DataEncoder & encoder,
					 const char * data,
					 CORBA::ULong bound)
{
  assert (_isok);

  CORBA::ULong length = strlen (data);

  if (bound > 0 && length > bound) {
    return FALSE;
  }

  encoder.put_ulong (length+1);

  if (!_conv) {
    encoder.buffer()->put (data, length+1);
  }
  else {
    CORBA::Long res;
    if ((res = _conv->encode (data, length, *encoder.buffer(), FALSE)) < 0
	|| (CORBA::ULong)res != length) {
      return FALSE;
    }
    encoder.put_octet (0);
  }

  return TRUE;
}

CORBA::Boolean
MICO::GIOP_1_0_CodeSetCoder::put_wchar (CORBA::DataEncoder &,
					CORBA::WChar)
{
  return FALSE;
}

CORBA::Boolean
MICO::GIOP_1_0_CodeSetCoder::put_wchars (CORBA::DataEncoder &,
					 const CORBA::WChar *,
					 CORBA::ULong)
{
  return FALSE;
}

CORBA::Boolean
MICO::GIOP_1_0_CodeSetCoder::put_wstring (CORBA::DataEncoder &,
					  const CORBA::WChar *,
					  CORBA::ULong)
{
  return FALSE;
}

/*
 * ----------------------------------------------------------------------
 * CDR::GIOP_1_1_CodeSetCoder
 * ----------------------------------------------------------------------
 */

/*
 * GIOP 1.1 supports codeset negotiation. However, it is a bit confused
 * about the encoding of wchars and wstrings. Therefore, we handle TCS-C
 * encoding, but don't support wchar and wstring.
 */

MICO::GIOP_1_1_CodeSetCoder::GIOP_1_1_CodeSetCoder (CORBA::Codeset::CodesetId tcsc)
{
  CORBA::Codeset::CodesetId nativecs =
    ::CORBA::Codeset::special_cs(::CORBA::Codeset::NativeCS)->id();

  _tcsc = tcsc;

  if (tcsc == nativecs && ::CORBA::Codeset::special_cs(::CORBA::Codeset::NativeCS)->codepoint_size() == sizeof (CORBA::Char)) {
    /*
     * No conversion is necessary
     */
    _conv_n2t = NULL;
    _conv_t2n = NULL;
    _isok     = TRUE;
  }
  else if (!::MICO::CodesetConv::can_convert (nativecs, tcsc)) {
    _conv_n2t = NULL;
    _conv_t2n = NULL;
    _isok     = FALSE;
  }
  else {
    ::CORBA::Codeset * csnative_1 =
      ::CORBA::Codeset::create (nativecs);
    ::CORBA::Codeset * cstcs_1 =
      ::CORBA::Codeset::create (tcsc);

    ::CORBA::Codeset * csnative_2 =
      ::CORBA::Codeset::create (nativecs);
    ::CORBA::Codeset * cstcs_2 =
      ::CORBA::Codeset::create (tcsc);

    assert (csnative_1 && cstcs_1);
    assert (csnative_2 && cstcs_2);

    _codepoint_size = cstcs_1->codepoint_size();
    _max_codepoints = cstcs_1->max_codepoints();
    _native_maxcp   = csnative_1->max_codepoints();

    _conv_n2t = ::MICO::CodesetConv::create (csnative_1, cstcs_1);
    _conv_t2n = ::MICO::CodesetConv::create (cstcs_2, csnative_2);
    _isok = (_conv_n2t != NULL && _conv_t2n != NULL);
  }    
}

MICO::GIOP_1_1_CodeSetCoder::~GIOP_1_1_CodeSetCoder ()
{
  delete _conv_n2t;
  delete _conv_t2n;
}

CORBA::CodeSetCoder *
MICO::GIOP_1_1_CodeSetCoder::clone ()
{
  return new GIOP_1_1_CodeSetCoder (_tcsc);
}

CORBA::Boolean
MICO::GIOP_1_1_CodeSetCoder::isok ()
{
  return _isok;
}

CORBA::Boolean
MICO::GIOP_1_1_CodeSetCoder::get_char (CORBA::DataDecoder & decoder,
				       CORBA::Char & data)
{
  assert (_isok);

  if (!_conv_t2n) {
    decoder.buffer()->get1 (&data);
    return TRUE;
  }

  /*
   * A character is encoded as a single octet. If TCS-C is a multi-byte
   * character set, only the first octet is sent. Doesn't make a lot of
   * sense, does it?
   */

  if (_max_codepoints == 1 && _codepoint_size == 1) {
    if (_conv_t2n->decode (*decoder.buffer(), 1, &data, FALSE) != 1) {
      return FALSE;
    }
  }
  else {
    CORBA::Octet o[8] = {0};
    if (!decoder.buffer()->get1 (o)) {
      return FALSE;
    }
    CORBA::Buffer buf (o);
    if (_conv_t2n->decode (buf, 1, &data, FALSE) != 1) {
      return FALSE;
    }
  }

  return TRUE;
}

CORBA::Boolean
MICO::GIOP_1_1_CodeSetCoder::get_chars (CORBA::DataDecoder & decoder,
					CORBA::Char * data,
					CORBA::ULong count)
{
  assert (_isok);

  if (!_conv_t2n) {
    decoder.buffer()->get (data, count);
    return TRUE;
  }

  if (_max_codepoints == 1 && _codepoint_size == 1) {
    CORBA::Long res;
    if ((res = _conv_t2n->decode (*decoder.buffer(), count, data, FALSE)) < 0
	|| (CORBA::ULong)res != count) {
      return FALSE;
    }
  }
  else {
    CORBA::Buffer buf (8);

    while (count--) {
      buf.rseek_beg (0);
      *((CORBA::LongLong *) buf.buffer()) = 0;
      if (!decoder.buffer()->get1 (buf.buffer())) {
	return FALSE;
      }
      if (_conv_t2n->decode (buf, 1, data, FALSE) != 1) {
	return FALSE;
      }
      data++;
    }
  }

  return TRUE;
}

CORBA::Boolean
MICO::GIOP_1_1_CodeSetCoder::get_string (CORBA::DataDecoder & decoder,
					 CORBA::String_out data,
					 CORBA::ULong bound)
{
  CORBA::ULong length;
  char * buf;

  assert (_isok);

  if (!decoder.get_ulong (length)) {
    return FALSE;
  }

  if (length == 0 || (bound > 0 && length-1 > bound)) {
    return FALSE;
  }

  if (!_conv_t2n) {
    if (length > decoder.buffer()->length()) {
      return FALSE;
    }

    buf = CORBA::string_alloc (length-1);

    if (!decoder.buffer()->get (buf, length)) {
      CORBA::string_free (buf);
      return FALSE;
    }
    if (buf[length-1] != '\0') {
      CORBA::string_free (buf);
      return FALSE;
    }
  }
  else if (_max_codepoints == 1 && _codepoint_size == 1 &&
	   _native_maxcp == 1) {
    if (length > decoder.buffer()->length()) {
      return FALSE;
    }

    buf = CORBA::string_alloc (length-1);
    CORBA::Long res;
    if ((res = _conv_t2n->decode (*decoder.buffer(), length-1, buf, TRUE)) < 0
	|| (CORBA::ULong)res != length) {
      CORBA::string_free (buf);
      return FALSE;
    }
    if (!decoder.buffer()->get1 (&length)) { /* eat up null octet */
      return FALSE;
    }
  }
  else if (_tcsc == 0x00010109) { /* special handling for UTF-16 */
    /*
     * Handle BOM if TCS-C is UTF-16
     */

    CORBA::ULong codepoints = length / 2 - 1;
    CORBA::Boolean swap = FALSE;
    CORBA::Octet bom[2];

    if (codepoints > 0) {
      if (!decoder.buffer()->peek (bom, 2)) {
	return FALSE;
      }

      if (bom[0] == 0xFE && bom[1] == 0xFF) {
	/*
	 * BOM says that codepoints are in Big Endian
	 */
#ifdef HAVE_BYTEORDER_BE
	swap = FALSE;
#else
	swap = TRUE;
#endif
	decoder.buffer()->get (bom, 2); /* eat up BOM */
	codepoints--;
      }
      else if (bom[0] == 0xFF && bom[1] == 0xFE) {
	/*
	 * BOM says that codepoints are in Little Endian
	 */
#ifdef HAVE_BYTEORDER_BE
	swap = TRUE;
#else
	swap = FALSE;
#endif
	decoder.buffer()->get (bom, 2); /* eat up BOM */
	codepoints--;
      }
      else {
	/*
	 * There is no BOM, so default to Big Endian
	 */
#ifdef HAVE_BYTEORDER_BE
	swap = FALSE;
#else
	swap = TRUE;
#endif
      }
    }
    else {
      /*
       * zero codepoints, so it doesn't matter anyway
       */
      swap = FALSE;
    }

    if (codepoints * _codepoint_size > decoder.buffer()->length()) {
      return FALSE;
    }

    buf = CORBA::string_alloc (codepoints);

    if (!swap) {
      if (_conv_t2n->decode (*decoder.buffer(), codepoints, buf, TRUE) < 0) {
	CORBA::string_free (buf);
	return FALSE;
      }
    }
    else {
      /*
       * codepoint swapping is necessary
       */

      CORBA::Buffer temp (codepoints*2);
      CORBA::Octet * tptr = temp.buffer ();
      CORBA::ULong count;

      for (count=0; count<codepoints; count++) {
	if (!decoder.buffer()->get (*(tptr+1)) ||
	    !decoder.buffer()->get (*tptr)) {
	  return FALSE;
	}
	tptr += 2;
      }
      
      if (_conv_t2n->decode (temp, codepoints, buf, TRUE) < 0) {
	CORBA::string_free (buf);
	return FALSE;
      }
    }

    if (!decoder.buffer()->get (bom, 2)) { /* eat up null octets */
      CORBA::string_free (buf);
      return FALSE;
    }
  }
  else {
    CORBA::ULong codepoints = length / _codepoint_size - 1;

    if (codepoints * _codepoint_size > decoder.buffer()->length()) {
      return FALSE;
    }

    buf = CORBA::string_alloc (codepoints * _native_maxcp);

    if (_conv_t2n->decode (*decoder.buffer(), codepoints, buf, TRUE) < 0) {
      CORBA::string_free (buf);
      return FALSE;
    }

    /*
     * eat up null octets
     */

    CORBA::ULong cnt = _codepoint_size;

    while (cnt--) {
      if (!decoder.buffer()->get1 (&length)) {
	CORBA::string_free (buf);
	return FALSE;
      }
    }
  }

  data = buf;
  return TRUE;
}

CORBA::Boolean
MICO::GIOP_1_1_CodeSetCoder::get_wchar (CORBA::DataDecoder &,
					CORBA::WChar &)
{
  return FALSE;
}

CORBA::Boolean
MICO::GIOP_1_1_CodeSetCoder::get_wchars (CORBA::DataDecoder &,
					 CORBA::WChar *,
					 CORBA::ULong)
{
  return FALSE;
}

CORBA::Boolean
MICO::GIOP_1_1_CodeSetCoder::get_wstring (CORBA::DataDecoder &,
					  CORBA::WString_out,
					  CORBA::ULong)
{
  return FALSE;
}

CORBA::Boolean
MICO::GIOP_1_1_CodeSetCoder::put_char (CORBA::DataEncoder & encoder,
				       CORBA::Char data)
{
  assert (_isok);

  if (!_conv_n2t) {
    encoder.buffer()->put1 (&data);
    return TRUE;
  }

  if (_max_codepoints == 1 && _codepoint_size == 1) {
    if (_conv_n2t->encode (&data, 1, *encoder.buffer(), FALSE) != 1) {
      return FALSE;
    }
  }
  else {
    CORBA::Buffer buf (8);
    if (_conv_n2t->encode (&data, 1, buf, FALSE) <= 0) {
      return FALSE;
    }
    encoder.buffer()->put1 (buf.buffer());
  }

  return TRUE;
}

CORBA::Boolean
MICO::GIOP_1_1_CodeSetCoder::put_chars (CORBA::DataEncoder & encoder,
					const CORBA::Char * data,
					CORBA::ULong count)
{
  assert (_isok);

  if (!_conv_n2t) {
    encoder.buffer()->put (data, count);
    return TRUE;
  }

  if (_max_codepoints == 1 && _codepoint_size == 1) {
    CORBA::Long res;
    if ((res = _conv_n2t->encode (data, count, *encoder.buffer(), FALSE)) < 0
	|| (CORBA::ULong)res != count) {
      return FALSE;
    }
  }
  else {
    CORBA::Buffer buf (8);

    while (count--) {
      buf.wseek_beg (0);
      if (_conv_n2t->encode (data, 1, buf, FALSE) <= 0) {
	return FALSE;
      }
      encoder.buffer()->put1 (buf.buffer());
      data++;
    }
  }

  return TRUE;
}

CORBA::Boolean
MICO::GIOP_1_1_CodeSetCoder::put_string (CORBA::DataEncoder & encoder,
					 const char * data,
					 CORBA::ULong bound)
{
  CORBA::ULong length = strlen (data);

  assert (_isok);

  if (bound > 0 && length > bound) {
    return FALSE;
  }

  if (!_conv_n2t) {
    encoder.put_ulong (length+1);
    encoder.buffer()->put (data, length+1);
  }
  else if (_max_codepoints == 1 && _codepoint_size == 1 &&
	   _native_maxcp == 1) {
    encoder.put_ulong (length+1);
    CORBA::Long res;
    if ((res = _conv_n2t->encode (data, length, *encoder.buffer(), FALSE)) < 0
	|| (CORBA::ULong)res != length) {
      return FALSE;
    }
    encoder.put_octet (0);
  }
  else {
    CORBA::ULong npos, pos, cnt;
    
    encoder.put_ulong (/* dummy value */ 0);
    pos = encoder.buffer()->wpos ();

    /*
     * place BOM if TCS-C is UTF-16 and HostByteOrder is not the
     * default BigEndian
     */

#ifndef HAVE_BYTEORDER_BE
    if (_tcsc == 0x00010109) {
      encoder.buffer()->put2 ("\xFF\xFE");
    }
#endif

    if (_conv_n2t->encode (data, length, *encoder.buffer(), FALSE) < 0) {
      return FALSE;
    }

    /*
     * put null octets
     */

    cnt = _codepoint_size;

    while (cnt--) {
      encoder.put_octet (0);
    }

    /*
     * fix length info
     */

    npos = encoder.buffer()->wpos ();
    encoder.buffer()->wseek_beg (pos-4);
    encoder.put_ulong (npos - pos);
    encoder.buffer()->wseek_beg (npos);
  }

  return TRUE;
}

CORBA::Boolean
MICO::GIOP_1_1_CodeSetCoder::put_wchar (CORBA::DataEncoder &,
					CORBA::WChar)
{
  return FALSE;
}

CORBA::Boolean
MICO::GIOP_1_1_CodeSetCoder::put_wchars (CORBA::DataEncoder &,
					 const CORBA::WChar *,
					 CORBA::ULong)
{
  return FALSE;
}

CORBA::Boolean
MICO::GIOP_1_1_CodeSetCoder::put_wstring (CORBA::DataEncoder &,
					  const CORBA::WChar *,
					  CORBA::ULong)
{
  return FALSE;
}

/*
 * ----------------------------------------------------------------------
 * CDR::GIOP_1_2_CodeSetCoder
 * ----------------------------------------------------------------------
 */

MICO::GIOP_1_2_CodeSetCoder::GIOP_1_2_CodeSetCoder (CORBA::Codeset::CodesetId tcsc,
						    CORBA::Codeset::CodesetId tcsw)
  : GIOP_1_1_CodeSetCoder (tcsc)
{
  CORBA::Codeset::CodesetId nativewcs =
    ::CORBA::Codeset::special_cs(::CORBA::Codeset::NativeWCS)->id();

  _tcsw = tcsw;

  if (tcsw == nativewcs && ::CORBA::Codeset::special_cs(::CORBA::Codeset::NativeWCS)->codepoint_size() == sizeof (CORBA::WChar)) {
    /*
     * No conversion is necessary
     */
    _w_conv_n2t = NULL;
    _w_conv_t2n = NULL;
    _w_codepoint_size = ::CORBA::Codeset::special_cs(::CORBA::Codeset::NativeWCS)->codepoint_size();
    _w_max_codepoints = ::CORBA::Codeset::special_cs(::CORBA::Codeset::NativeWCS)->max_codepoints();
    _w_native_maxcp = _w_max_codepoints;
    _w_isok = TRUE;
  }
  else if (!::MICO::CodesetConv::can_convert (nativewcs, tcsw)) {
    _w_conv_n2t = NULL;
    _w_conv_t2n = NULL;
    _w_isok = FALSE;
  }
  else {
    ::CORBA::Codeset * wcsnative_1 =
      ::CORBA::Codeset::create (nativewcs);
    ::CORBA::Codeset * wcstcs_1 =
      ::CORBA::Codeset::create (tcsw);

    ::CORBA::Codeset * wcsnative_2 =
      ::CORBA::Codeset::create (nativewcs);
    ::CORBA::Codeset * wcstcs_2 =
      ::CORBA::Codeset::create (tcsw);

    assert (wcsnative_1 && wcstcs_1);
    assert (wcsnative_2 && wcstcs_2);

    _w_codepoint_size = wcstcs_1->codepoint_size();
    _w_max_codepoints = wcstcs_1->max_codepoints();
    _w_native_maxcp   = wcsnative_1->max_codepoints();
    _w_conv_n2t = ::MICO::CodesetConv::create (wcsnative_1, wcstcs_1);
    _w_conv_t2n = ::MICO::CodesetConv::create (wcstcs_2, wcsnative_2);
    _w_isok = (_w_conv_n2t != NULL && _w_conv_t2n != NULL);
  }
}

MICO::GIOP_1_2_CodeSetCoder::~GIOP_1_2_CodeSetCoder ()
{
  delete _w_conv_n2t;
  delete _w_conv_t2n;
}

CORBA::CodeSetCoder *
MICO::GIOP_1_2_CodeSetCoder::clone ()
{
  return new GIOP_1_2_CodeSetCoder (_tcsc, _tcsw);
}

CORBA::Boolean
MICO::GIOP_1_2_CodeSetCoder::isok ()
{
  return _w_isok && GIOP_1_1_CodeSetCoder::isok ();
}

CORBA::Boolean
MICO::GIOP_1_2_CodeSetCoder::get_wchar (CORBA::DataDecoder & decoder,
					CORBA::WChar & data)
{
  CORBA::Octet len;

  assert (_w_isok);

  if (!decoder.get_octet (len)) {
    return FALSE;
  }

  if ((len % _w_codepoint_size) != 0) {
    return FALSE;
  }

  if (!_w_conv_t2n) {
    /*
     * Special handling if TCS-W is UTF-16. There can be a BOM
     */
    if (_tcsw == 0x00010109 && len == 4) {
      CORBA::Octet bom[2];
      if (!decoder.buffer()->get (bom, 2)) {
	return FALSE;
      }
      if (bom[0] == 0xFE && bom[1] == 0xFF) {
#ifdef HAVE_BYTEORDER_BE
	return decoder.buffer()->get (&data, 2);
#else
	return (decoder.buffer()->get1 (((CORBA::Octet *) &data) + 1) &&
		decoder.buffer()->get1 (((CORBA::Octet *) &data)));
#endif
      }
      else if (bom[0] == 0xFF && bom[1] == 0xFE) {
#ifdef HAVE_BYTEORDER_BE
	return (decoder.buffer()->get1 (((CORBA::Octet *) &data) + 1) &&
		decoder.buffer()->get1 (((CORBA::Octet *) &data)));
#else
	return decoder.buffer()->get (&data, 2);
#endif
      }
      else {
	/* oops, no BOM, but a length of 4? */
	return FALSE;
      }
    }
    else if (_tcsw == 0x00010109 && len == 2) {
#ifdef HAVE_BYTEORDER_BE
      return decoder.buffer()->get (&data, 2);
#else
      return (decoder.buffer()->get1 (((CORBA::Octet *) &data) + 1) &&
	      decoder.buffer()->get1 (((CORBA::Octet *) &data)));
#endif
    }
    if (len != _w_codepoint_size) {
      /* can this happen? */
      return FALSE;
    }
    return decoder.buffer()->get (&data, _w_codepoint_size);
  }
  else if (_tcsw == 0x00010109) {
    /*
     * Special handling for UTF-16
     */

    CORBA::Boolean swap = FALSE;

    if (len == 4) {
      CORBA::Octet bom[2];
      if (!decoder.buffer()->get (bom, 2)) {
	return FALSE;
      }
      if (bom[0] == 0xFE && bom[1] == 0xFF) {
#ifdef HAVE_BYTEORDER_BE
	swap = FALSE;
#else
	swap = TRUE;
#endif
      }
      else if (bom[0] == 0xFF && bom[1] == 0xFE) {
#ifdef HAVE_BYTEORDER_BE
	swap = TRUE;
#else
	swap = FALSE;
#endif
      }
      else {
	/* oops, no BOM, but a length of 4? */
	return FALSE;
      }
    }
    else if (len == 2) {
#ifdef HAVE_BYTEORDER_BE
      swap = FALSE;
#else
      swap = TRUE;
#endif
    }
    else {
      /* len != 4 && len != 2 */
      return FALSE;
    }

    if (swap == FALSE) {
      if (_w_conv_t2n->decode (*decoder.buffer(), 1, &data, FALSE) != 1) {
	return FALSE;
      }
    }
    else {
      CORBA::Buffer temp (2);
      if (!decoder.buffer()->get1 (temp.buffer()+1) ||
	  !decoder.buffer()->get1 (temp.buffer())) {
	return FALSE;
      }
      temp.wseek_beg (2);
      if (_w_conv_t2n->decode (temp, 1, &data, FALSE) != 1) {
	return FALSE;
      }
    }
  }
  else if (len == _w_codepoint_size) {
    if (_w_conv_t2n->decode (*decoder.buffer(), 1, &data, FALSE) != 1) {
      return FALSE;
    }
  }
  else {
#ifdef __GNUG__
    CORBA::WChar o[len];
#else
    CORBA::WString_var o = CORBA::wstring_alloc (len);
#endif
    if (_w_conv_t2n->decode (*decoder.buffer(), len / _w_codepoint_size,
                             (wchar_t*)o, FALSE) <= 0) {
      return FALSE;
    }
    data = o[(CORBA::ULong)0];
  }

  return TRUE;
}

CORBA::Boolean
MICO::GIOP_1_2_CodeSetCoder::get_wchars (CORBA::DataDecoder & decoder,
					 CORBA::WChar * data,
					 CORBA::ULong count)
{
  /*
   * Nothing to optimize here
   */

  while (count--) {
    if (!get_wchar (decoder, *data++)) {
      return FALSE;
    }
  }
  return TRUE;
}

CORBA::Boolean
MICO::GIOP_1_2_CodeSetCoder::get_wstring (CORBA::DataDecoder & decoder,
					  CORBA::WString_out data,
					  CORBA::ULong bound)
{
  CORBA::ULong length;
  CORBA::WChar * buf;

  assert (_w_isok);

  if (!decoder.get_ulong (length)) {
    return FALSE;
  }

  if ((length % _w_codepoint_size) != 0) {
    return FALSE;
  }

  if (length > decoder.buffer()->length()) {
    return FALSE;
  }

  if (!_w_conv_t2n) {
    /*
     * Special handling if TCS-W is UTF-16. There can be a BOM
     */

    if (_tcsw == 0x00010109) {
      CORBA::Boolean swap = FALSE;

      if (length >= 2) {
	CORBA::Octet bom[2];
	if (!decoder.buffer()->peek (bom, 2)) {
	  return FALSE;
	}
	if (bom[0] == 0xFE && bom[1] == 0xFF) {
#ifdef HAVE_BYTEORDER_BE
	  swap = FALSE;
#else
	  swap = TRUE;
#endif
	  decoder.buffer()->get (bom, 2);
	  length -= 2;
	}
	else if (bom[0] == 0xFF && bom[1] == 0xFE) {
#ifdef HAVE_BYTEORDER_BE
	  swap = TRUE;
#else
	  swap = FALSE;
#endif
	  decoder.buffer()->get (bom, 2);
	  length -= 2;
	}
	else {
#ifdef HAVE_BYTEORDER_BE
	  swap = FALSE;
#else
	  swap = TRUE;
#endif
	}
      }

      buf = CORBA::wstring_alloc (length / 2);
      // kcg: added here -- see comment few lines below
      buf[length / 2] = 0;
      if (!swap) {
	if (!decoder.buffer()->get (buf, length)) {
	  CORBA::wstring_free (buf);
	  return FALSE;
	}
	// kcg: why we end string with '0' only when !swap?
	// imho this is wrong, so I've commented it here
	// and added it directly after wstring_alloc above
	//buf[length / 2] = 0;
      }
      else {
	CORBA::WChar * ptr = buf;
	while (length) {
	  if (!decoder.buffer()->get1 (((CORBA::Octet *) ptr) + 1) ||
	      !decoder.buffer()->get1 (((CORBA::Octet *) ptr))) {
	    return FALSE;
	  }
	  length -= 2;
	  ptr++;
	}
      }
    }
    else {
      /*
       * Native WCS == TCS-W != UTF-8
       */

      buf = CORBA::wstring_alloc (length / _w_codepoint_size);
      if (!decoder.buffer()->get (buf, length)) {
	CORBA::wstring_free (buf);
	return FALSE;
      }
      buf[length / _w_codepoint_size] = 0;
    }
  }
  else if (_tcsw == 0x00010109) {
    /*
     * Handle BOM if TCS-C is UTF-16
     */

    CORBA::ULong codepoints = length / 2;
    CORBA::Boolean swap = FALSE;
    CORBA::Octet bom[2];

    if (codepoints > 0) {
      if (!decoder.buffer()->peek (bom, 2)) {
	return FALSE;
      }

      if (bom[0] == 0xFE && bom[1] == 0xFF) {
	/*
	 * BOM says that codepoints are in Big Endian
	 */
#ifdef HAVE_BYTEORDER_BE
	swap = FALSE;
#else
	swap = TRUE;
#endif
	decoder.buffer()->get (bom, 2); /* eat up BOM */
	codepoints--;
      }
      else if (bom[0] == 0xFF && bom[1] == 0xFE) {
	/*
	 * BOM says that codepoints are in Little Endian
	 */
#ifdef HAVE_BYTEORDER_BE
	swap = TRUE;
#else
	swap = FALSE;
#endif
	decoder.buffer()->get (bom, 2); /* eat up BOM */
	codepoints--;
      }
      else {
	/*
	 * There is no BOM, so default to Big Endian
	 */
#ifdef HAVE_BYTEORDER_BE
	swap = FALSE;
#else
	swap = TRUE;
#endif
      }
    }
    else {
      /*
       * zero codepoints, so it doesn't matter anyway
       */
      swap = FALSE;
    }

    buf = CORBA::wstring_alloc (codepoints * _w_native_maxcp);

    if (!swap) {
      if (_w_conv_t2n->decode (*decoder.buffer(), codepoints, buf, TRUE) < 0) {
	CORBA::wstring_free (buf);
	return FALSE;
      }
    }
    else {
      /*
       * codepoint swapping is necessary
       */

      CORBA::Buffer temp (codepoints*2);
      CORBA::Octet * tptr = temp.buffer ();
      CORBA::ULong count;

      for (count=0; count<codepoints; count++) {
	if (!decoder.buffer()->get1 (tptr + 1) ||
	    !decoder.buffer()->get1 (tptr)) {
	  return FALSE;
	}
	tptr += 2;
      }
      
      temp.wseek_beg (codepoints*2);

      if (_w_conv_t2n->decode (temp, codepoints, buf, TRUE) < 0) {
	CORBA::wstring_free (buf);
	return FALSE;
      }
    }
  }
  else {
    /*
     * TCS-W != UTF-16
     */

    buf = CORBA::wstring_alloc (length / _w_codepoint_size * _w_native_maxcp);
    CORBA::Long len = _w_conv_t2n->decode (*decoder.buffer(),
					   length / _w_codepoint_size,
					   buf, FALSE);
    if (len < 0) {
      return FALSE;
    }
    buf[len] = 0;
  }

  data = buf;
  return TRUE;
}

CORBA::Boolean
MICO::GIOP_1_2_CodeSetCoder::put_wchar (CORBA::DataEncoder & encoder,
					CORBA::WChar data)
{
  assert (_w_isok);

  if (!_w_conv_n2t) {
#ifndef HAVE_BYTEORDER_BE
    if (_tcsw == 0x00010109) {
      /*
       * place BOM if TCS-C is UTF-16 and HostByteOrder is not the
       * default BigEndian
       */
      encoder.put_octet (4);
      encoder.buffer()->put ("\xFF\xFE", 2);
      encoder.buffer()->put (&data, 2);
      return TRUE;
    }
#endif

    encoder.put_octet (_w_codepoint_size);
    encoder.buffer()->put (&data, _w_codepoint_size);
    return TRUE;
  }
  else {
    CORBA::ULong pos, npos;
    encoder.put_octet (_w_codepoint_size); /* dummy value */
    pos = encoder.buffer()->wpos ();

#ifndef HAVE_BYTEORDER_BE
    if (_tcsw == 0x00010109) {
      /*
       * place BOM if TCS-C is UTF-16 and HostByteOrder is not the
       * default BigEndian
       */
      encoder.buffer()->put ("\xFF\xFE", 2);
    }
#endif

    if (_w_conv_n2t->encode (&data, 1, *encoder.buffer(), FALSE) <= 0) {
      return FALSE;
    }
    npos = encoder.buffer()->wpos ();

    if (npos - pos != _w_codepoint_size) {
      /*
       * fix dummy length
       */
      encoder.buffer()->wseek_beg (pos - 1);
      encoder.put_octet (npos - pos);
      encoder.buffer()->wseek_beg (npos);
    }
  }
  return TRUE;
}

CORBA::Boolean
MICO::GIOP_1_2_CodeSetCoder::put_wchars (CORBA::DataEncoder & decoder,
					 const CORBA::WChar * data,
					 CORBA::ULong count)
{
  /*
   * Nothing to improve here
   */

  while (count) {
    if (!put_wchar (decoder, *data++)) {
      return FALSE;
    }
  }
  return TRUE;
}

CORBA::Boolean
MICO::GIOP_1_2_CodeSetCoder::put_wstring (CORBA::DataEncoder & encoder,
					  const CORBA::WChar * data,
					  CORBA::ULong bound)
{
  CORBA::ULong length = xwcslen (data);

  assert (_w_isok);

  if (bound > 0 && length > bound) {
    return FALSE;
  }

  if (length == 0) {
    encoder.put_ulong (0);
    return TRUE;
  }

  if (!_w_conv_n2t) {
#ifndef HAVE_BYTEORDER_BE
    if (_tcsw == 0x00010109) {
      encoder.put_ulong (length * _w_codepoint_size + 2);
      encoder.buffer()->put2 ("\xFF\xFE");
      encoder.buffer()->put (data, length * _w_codepoint_size);
      return TRUE;
    }
#endif
    encoder.put_ulong (length * _w_codepoint_size);
    encoder.buffer()->put (data, length * _w_codepoint_size);
  }
  else {
    CORBA::ULong pos, npos;

    encoder.put_ulong (/* dummy value */ 0);
    pos = encoder.buffer()->wpos ();

#ifndef HAVE_BYTEORDER_BE
    if (_tcsw == 0x00010109) {
      /*
       * place BOM if TCS-C is UTF-16 and HostByteOrder is not the
       * default BigEndian
       */
      encoder.buffer()->put2 ("\xFF\xFE");
    }
#endif

    if (_w_conv_n2t->encode (data, length, *encoder.buffer(), FALSE) < 0) {
      return FALSE;
    }

    /*
     * fix length info
     */

    npos = encoder.buffer()->wpos ();
    encoder.buffer()->wseek_beg (pos-4);
    encoder.put_ulong (npos - pos);
    encoder.buffer()->wseek_beg (npos);
  }

  return TRUE;
}

/*
 * ----------------------------------------------------------------------
 * Codeset Component
 * ----------------------------------------------------------------------
 */

MICO::CodesetComponent::CodesetComponent ()
    : _native_cs (0), _native_wcs (0),
      _selected_cs (0), _selected_wcs (0)
{
}

MICO::CodesetComponent::CodesetComponent (CodeSetId native_cs,
					  CodeSetId native_wcs,
					  const vector<CodeSetId> &conv_cs,
					  const vector<CodeSetId> &conv_wcs)
    : _native_cs (native_cs), _native_wcs (native_wcs),
      _conv_cs (conv_cs), _conv_wcs (conv_wcs),
      _selected_cs (0), _selected_wcs (0)
{
}

MICO::CodesetComponent::CodesetComponent (const CodesetComponent &csc)
    : _native_cs (csc._native_cs), _native_wcs (csc._native_wcs),
      _conv_cs (csc._conv_cs), _conv_wcs (csc._conv_wcs),
      _selected_cs (csc._selected_cs), _selected_wcs (csc._selected_wcs)
{
}

MICO::CodesetComponent::~CodesetComponent ()
{
}

void
MICO::CodesetComponent::encode (CORBA::DataEncoder &ec) const
{
    // CodeSetComponentInfo
    ec.struct_begin();
    {
	// ForCharData
	ec.struct_begin();
	{
	    // native_code_set
	    ec.put_ulong (_native_cs);
	    // conversion_code_sets
	    ec.seq_begin (_conv_cs.size());
	    for (mico_vec_size_type i = 0; i < _conv_cs.size(); ++i)
		ec.put_ulong (_conv_cs[i]);
	    ec.seq_end ();
	}
	ec.struct_end();

	// ForWcharData
	ec.struct_begin();
	{
	    // native_code_set
	    ec.put_ulong (_native_wcs);
	    // conversion_code_sets
	    ec.seq_begin (_conv_wcs.size());
	    for (mico_vec_size_type i = 0; i < _conv_wcs.size(); ++i)
		ec.put_ulong (_conv_wcs[i]);
	    ec.seq_end ();
	}
	ec.struct_end();
    }
    ec.struct_end();
}

CORBA::Component::ComponentId
MICO::CodesetComponent::id () const
{
    return TAG_CODE_SETS;
}

void
MICO::CodesetComponent::print (ostream &o) const
{
  CORBA::Codeset::Info * nci = CORBA::Codeset::_find_info (_native_cs);
  CORBA::Codeset::Info * nwi = CORBA::Codeset::_find_info (_native_wcs);

  o << "Native Codesets:" << endl;
  o << "              normal: ";

  if (nci) {
    o << nci->desc << endl;
  }
  else {
    o << "0x" << hex << _native_cs << endl;
  }

  o << "                wide: ";

  if (nwi) {
    o << nwi->desc << endl;
  }
  else {
    o << "0x" << hex << _native_wcs << endl;
  }

  if (_conv_cs.size() > 0) {
    o << "              Other Codesets: " << endl;
    for (mico_vec_size_type i = 0; i < _conv_cs.size(); i++) {
      nci = CORBA::Codeset::_find_info (_conv_cs[i]);
      o << "                      ";
      if (nci) {
	o << nci->desc << endl;
      }
      else {
	o << "0x" << hex << _conv_cs[i] << endl;
      }
    }
  }

  if (_conv_wcs.size() > 0) {
    o << "              Other Wide Codesets: " << endl;
    for (mico_vec_size_type i = 0; i < _conv_wcs.size(); i++) {
      nwi = CORBA::Codeset::_find_info (_conv_wcs[i]);
      o << "                      ";
      if (nwi) {
	o << nwi->desc << endl;
      }
      else {
	o << "0x" << hex << _conv_wcs[i] << endl;
      }
    }
  }
}

CORBA::Component *
MICO::CodesetComponent::clone () const
{
    return new CodesetComponent (*this);
}

CORBA::Long
MICO::CodesetComponent::compare (const CORBA::Component &c) const
{
    if (id() != c.id())
	return (CORBA::Long)id() - (CORBA::Long)c.id();

    CodesetComponent &csc = (CodesetComponent &)c;

    if (_native_cs != csc._native_cs)
	return (CORBA::Long)_native_cs - (CORBA::Long)csc._native_cs;

    if (_native_wcs != csc._native_wcs)
	return (CORBA::Long)_native_wcs - (CORBA::Long)csc._native_wcs;

    CORBA::Long r = mico_vec_compare (_conv_cs, csc._conv_cs);
    if (r)
	return r;

    return mico_vec_compare (_conv_wcs, csc._conv_wcs);
}

CORBA::Boolean
MICO::CodesetComponent::operator== (const CORBA::Component &c) const
{
    return compare (c) == 0;
}

CORBA::Boolean
MICO::CodesetComponent::operator< (const CORBA::Component &c) const
{
    return compare (c) < 0;
}

MICO::CodesetComponent::CodeSetId
MICO::CodesetComponent::selected_cs ()
{
    if (_selected_cs)
	return _selected_cs;

    CodeSetId cl_nat =
	CORBA::Codeset::special_cs (CORBA::Codeset::NativeCS)->id();

    CodeSetId sv_nat = _native_cs;
    /*
     * this is a workaround for a bug in VB, which specifies
     * a native code set 0 and no conversion code sets
     */
    if (!_native_cs && !_conv_cs.size())
	sv_nat = CORBA::Codeset::special_cs(CORBA::Codeset::DefaultCS)->id();

    // same native code sets?
    if (sv_nat == cl_nat) {
	_selected_cs = cl_nat;
	return _selected_cs;
    }
    // can client convert its native code set to servers native code set?
    if (MICO::CodesetConv::can_convert (cl_nat, sv_nat)) {
	_selected_cs = sv_nat;
	return _selected_cs;
    }
    // can server convert clients native code set to its native code set?
    for (mico_vec_size_type i = 0; i < _conv_cs.size(); ++i) {
	if (cl_nat == _conv_cs[i]) {
	    _selected_cs = cl_nat;
	    return _selected_cs;
	}
    }
    /*
     * is there an intermediate code set client and server can convert
     * their native code sets to? favor server preferences.
     */
    {
      for (mico_vec_size_type i = 0; i < _conv_cs.size(); ++i) {
	if (MICO::CodesetConv::can_convert (cl_nat, _conv_cs[i])) {
	  _selected_cs = _conv_cs[i];
	  return _selected_cs;
	}
      }
    }
    // use fallback code set
    _selected_cs =
	CORBA::Codeset::special_cs(CORBA::Codeset::FallbackCS)->id();
    return _selected_cs;
}

MICO::CodesetComponent::CodeSetId
MICO::CodesetComponent::selected_wcs ()
{
    if (_selected_wcs)
	return _selected_wcs;

    CodeSetId cl_nat =
	CORBA::Codeset::special_cs (CORBA::Codeset::NativeWCS)->id();

    CodeSetId sv_nat = _native_wcs;
    /*
     * this is a workaround for a bug in VB, which specifies
     * a native code set 0 and no conversion code sets
     */
    if (!_native_wcs && !_conv_wcs.size())
	sv_nat = CORBA::Codeset::special_cs(CORBA::Codeset::DefaultWCS)->id();

    // same native code sets?
    if (sv_nat == cl_nat) {
	_selected_wcs = cl_nat;
	return _selected_wcs;
    }
    // can client convert its native code set to servers native code set?
    if (MICO::CodesetConv::can_convert (cl_nat, sv_nat)) {
	_selected_wcs = sv_nat;
	return _selected_wcs;
    }
    // can server convert clients native code set to its native code set?
    for (mico_vec_size_type i = 0; i < _conv_wcs.size(); ++i) {
	if (cl_nat == _conv_wcs[i]) {
	    _selected_wcs = cl_nat;
	    return _selected_wcs;
	}
    }
    /*
     * is there an intermediate code set client and server can convert
     * their native code sets to? favor server preferences.
     */
    {
      for (mico_vec_size_type i = 0; i < _conv_wcs.size(); ++i) {
	if (MICO::CodesetConv::can_convert (cl_nat, _conv_wcs[i])) {
	  _selected_wcs = _conv_wcs[i];
	  return _selected_wcs;
	}
      }
    }
    // use fallback code set
    _selected_wcs =
	CORBA::Codeset::special_cs(CORBA::Codeset::FallbackWCS)->id();
    return _selected_wcs;
}


//-----------------


MICO::CodesetComponentDecoder::CodesetComponentDecoder ()
{
    CORBA::Component::register_decoder (this);
}

MICO::CodesetComponentDecoder::~CodesetComponentDecoder ()
{
    CORBA::Component::unregister_decoder (this);
}

#define check(exp) if (!(exp)) return 0;

CORBA::Component *
MICO::CodesetComponentDecoder::decode (CORBA::DataDecoder &dc,
				       ComponentId, CORBA::ULong) const
{
    CodesetComponent::CodeSetId native_cs, native_wcs;
    vector<CodesetComponent::CodeSetId> conv_cs, conv_wcs;
    CORBA::ULong len;

    // CodeSetComponentInfo
    check (dc.struct_begin());
    {
	// ForCharData
	check (dc.struct_begin());
	{
	    // native_code_set
	    check (dc.get_ulong (native_cs));
	    // conversion_code_sets
	    check (dc.seq_begin (len));
	    CORBA::ULong cs;
	    for (CORBA::ULong i = 0; i < len; ++i) {
		check (dc.get_ulong (cs));
		conv_cs.push_back (cs);
	    }
	    check (dc.seq_end ());
	}
	check (dc.struct_end());

	// ForWcharData
	check (dc.struct_begin());
	{
	    // native_code_set
	    check (dc.get_ulong (native_wcs));
	    // conversion_code_sets
	    check (dc.seq_begin (len));
	    CORBA::ULong cs;
	    for (CORBA::ULong i = 0; i < len; ++i) {
		check (dc.get_ulong (cs));
		conv_wcs.push_back (cs);
	    }
	    check (dc.seq_end ());
	}
	check (dc.struct_end());
    }
    check (dc.struct_end());

    return new CodesetComponent (native_cs, native_wcs, conv_cs, conv_wcs);
}

#undef check

CORBA::Boolean
MICO::CodesetComponentDecoder::has_id (ComponentId id) const
{
    return id == CORBA::Component::TAG_CODE_SETS;
}

static MICO::CodesetComponentDecoder codeset_component_decoder;


//-----------------


static class CodesetInit : public Interceptor::InitInterceptor {
public:
    CodesetInit ()
        : Interceptor::InitInterceptor(0)
    {
    }
    Interceptor::Status initialize (CORBA::ORB_ptr orb,
				    const char *orbid,
				    int &argc, char *argv[])
    {
	MICOGetOpt::OptMap opts;
	opts["-ORBNativeCS"]   = "arg-expected";
	opts["-ORBNativeWCS"]  = "arg-expected";
	opts["-ORBNoCodeSets"] = "";
	opts["-ORBCodeSetsInIIOPProfile"] = "";
	opts["-ORBNoCodeSetsInMultiCompProfile"] = "";

	MICOGetOpt opt_parser (opts);
	CORBA::Boolean r = opt_parser.parse (orb->rcfile(), TRUE);
	assert (r);
	r = opt_parser.parse (argc, argv, TRUE);
	assert (r);

	const char *cs_name = 0;
	const char *wcs_name = 0;
	CORBA::Boolean disable = FALSE;
	CORBA::Boolean iiop_profile_enable = FALSE;
	CORBA::Boolean multi_comp_profile_disable = FALSE;

	const MICOGetOpt::OptVec &o = opt_parser.opts();
	MICOGetOpt::OptVec::const_iterator i;
	for (i = o.begin(); i != o.end(); ++i) {
	    const string &arg = (*i).first;
	    const string &val = (*i).second;
	    if (arg == "-ORBNativeCS") {
		cs_name = val.c_str();
	    } else if (arg == "-ORBNativeWCS") {
		wcs_name = val.c_str();
	    } else if (arg == "-ORBNoCodeSets") {
		disable = TRUE;
	    } else if (arg == "-ORBCodeSetsInIIOPProfile") {
		iiop_profile_enable = TRUE;
	    } else if (arg == "-ORBNoCodeSetsInMultiCompProfile") {
		multi_comp_profile_disable = TRUE;
	    }
	}

	CORBA::Codeset::disable (disable);
	CORBA::Codeset::enable_in_iiop_profile(iiop_profile_enable);

	// native char code set
	if (!cs_name)
	    cs_name = "*8859-1*";

	CORBA::Codeset *cs = CORBA::Codeset::create (cs_name);
	if (!cs) {
	  if (MICO::Logger::IsLogged (MICO::Logger::Error)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::Error)
	      << "Error: unknown native char code set: " << cs_name << endl;
	  }
	  assert (0);
	}
	CORBA::Codeset::set_special_cs (CORBA::Codeset::NativeCS, cs);

	// native wide char code set
	if (!wcs_name)
	    wcs_name = "*UTF-16*";

	cs = CORBA::Codeset::create (wcs_name);
	if (!cs) {
	  if (MICO::Logger::IsLogged (MICO::Logger::Error)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::Error)
	      << "Error: unknown native wide char code set: "
	      << wcs_name << endl;
	  }
	  assert (0);
	}
	CORBA::Codeset::set_special_cs (CORBA::Codeset::NativeWCS, cs);

	// default char code set
	cs_name = "*8859-1*";
	cs = CORBA::Codeset::create (cs_name);
	if (!cs) {
	  if (MICO::Logger::IsLogged (MICO::Logger::Error)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::Error)
	      << "Error: unknown default char code set: " << cs_name << endl;
	  }
	  assert (0);
	}
	CORBA::Codeset::set_special_cs (CORBA::Codeset::DefaultCS, cs);

	// default wide char code set (spec says there is no default ...)
	cs_name = "*UTF-16*";
	cs = CORBA::Codeset::create (cs_name);
	if (!cs) {
	  if (MICO::Logger::IsLogged (MICO::Logger::Error)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::Error)
	      << "Error: unknown default wide char code set: "
	      << cs_name << endl;
	  }
	  assert (0);
	}
	CORBA::Codeset::set_special_cs (CORBA::Codeset::DefaultWCS, cs);

	// fallback char code set
	cs_name = "*UTF-8*";
	cs = CORBA::Codeset::create (cs_name);
	if (!cs) {
	  if (MICO::Logger::IsLogged (MICO::Logger::Error)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::Error)
	      << "Error: unknown fallback char code set: "
	      << cs_name << endl;
	  }
	  assert (0);
	}
	CORBA::Codeset::set_special_cs (CORBA::Codeset::FallbackCS, cs);

	// fallback wide char code set
	cs_name = "*UTF-16*";
	cs = CORBA::Codeset::create (cs_name);
	if (!cs) {
	  if (MICO::Logger::IsLogged (MICO::Logger::Error)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::Error)
	      << "Error: unknown fallback wide char code set: "
	      << cs_name << endl;
	  }
	  assert (0);
	}
	CORBA::Codeset::set_special_cs (CORBA::Codeset::FallbackWCS, cs);


	// install code set info in ior template
	if (!disable && !multi_comp_profile_disable) {
	    MICO::MultiCompProfile *mcp =
		new MICO::MultiCompProfile (CORBA::MultiComponent());

	    mcp->components()->add_component (new MICO::CodesetComponent (
		CORBA::Codeset::special_cs(CORBA::Codeset::NativeCS)->id(),
		CORBA::Codeset::special_cs(CORBA::Codeset::NativeWCS)->id(),
		vector<MICO::CodesetComponent::CodeSetId>(),
		vector<MICO::CodesetComponent::CodeSetId>()));
	    orb->ior_template()->add_profile (mcp);
	}

	return Interceptor::INVOKE_CONTINUE;
    }
} __cs_init;

namespace MICO
{
    class CodesetIORInterceptor_impl
	: virtual public PortableInterceptor::IORInterceptor,
	  virtual public CORBA::LocalObject
    {
    public:
	CodesetIORInterceptor_impl()
	    : name_("CodesetIORInterceptor")
	{}

	virtual char*
	name()
	{ return CORBA::string_dup(name_.c_str()); }

	virtual void
	name(char* name)
	{ name_ = name; }

	virtual void
	destroy()
	{}

	virtual void
	establish_components(PortableInterceptor::IORInfo_ptr info);
    private:
	std::string name_;
    };

    class CodesetORBInitializer
	: virtual public PortableInterceptor::ORBInitializer,
	  virtual public CORBA::LocalObject
    {
    public:
	CodesetORBInitializer();

	virtual
	~CodesetORBInitializer();

	virtual void
	pre_init(PortableInterceptor::ORBInitInfo_ptr info);

	virtual void
	post_init(PortableInterceptor::ORBInitInfo_ptr info);
    };
} // MICO

void
MICO::CodesetIORInterceptor_impl::establish_components(PortableInterceptor::IORInfo_ptr info)
{
    if (CORBA::Codeset::enabled_in_iiop_profile()
	&& !CORBA::Codeset::disabled()) {
	//cerr << "enabled for IIOP profiles!" << endl;
	CORBA::Buffer* buf = new CORBA::Buffer;
	CDREncoder encoder(buf);
	CodesetComponent comp
	    (CORBA::Codeset::special_cs(CORBA::Codeset::NativeCS)->id(),
	     CORBA::Codeset::special_cs(CORBA::Codeset::NativeWCS)->id(),
	     vector<MICO::CodesetComponent::CodeSetId>(),
	     vector<MICO::CodesetComponent::CodeSetId>());
	comp.encode(encoder);
	buf->rseek_beg(0);
	IOP::TaggedComponent component;
	component.tag = comp.id();
	component.component_data.length(buf->length());
	for (CORBA::ULong i=0; i<buf->length(); i++) {
	    component.component_data[i] = (buf->data())[i];
	}
	try {
	    info->add_ior_component_to_profile
		(component, CORBA::IORProfile::TAG_INTERNET_IOP);
	} catch (CORBA::BAD_PARAM& ex) {
	    if (MICO::Logger::IsLogged(MICO::Logger::ORB))
		MICO::Logger::Stream(MICO::Logger::ORB)
		    << "IOR: can't find IIOP profile" << endl;
	}
	try {
	    info->add_ior_component_to_profile
		(component, CORBA::IORProfile::TAG_SSL_INTERNET_IOP);
	} catch (CORBA::BAD_PARAM& ex) {
	    if (MICO::Logger::IsLogged(MICO::Logger::ORB))
		MICO::Logger::Stream(MICO::Logger::ORB)
		    << "IOR: can't find SSL IIOP profile" << endl;
	}
    }
}

MICO::CodesetORBInitializer::CodesetORBInitializer()
{
}

MICO::CodesetORBInitializer::~CodesetORBInitializer()
{
}

void
MICO::CodesetORBInitializer::pre_init
(PortableInterceptor::ORBInitInfo_ptr info)
{
    PortableInterceptor::IORInterceptor_var icept = new CodesetIORInterceptor_impl;
    info->add_ior_interceptor(icept);
}

void
MICO::CodesetORBInitializer::post_init
(PortableInterceptor::ORBInitInfo_ptr info)
{
}

void
CORBA::Codeset::_init ()
{
  // Is it really needed? E.g. for some obsolete compilers?
  // In that case, please uncomment following line
  //__cs_init;
  PortableInterceptor::register_orb_initializer(new MICO::CodesetORBInitializer());
}

struct __init_CODESET
{
    __init_CODESET()
    {
    }

    ~__init_CODESET()
    {
	CORBA::Codeset::free();
    }
};

static __init_CODESET __module_init_CODESET;

