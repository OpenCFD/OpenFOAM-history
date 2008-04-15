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

#ifdef FAST_PCH
#include "orb_pch.h"
#endif // FAST_PCH
#ifdef __COMO__
#pragma hdrstop
#endif // __COMO__

#ifndef FAST_PCH

#include <CORBA-SMALL.h>
#include <stdio.h>
#include <string.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <iomanip>
#else
#include <iostream.h>
#include <iomanip.h>
#endif
#include <mico/template_impl.h>

#endif // FAST_PCH


using namespace std;

CORBA::Buffer::Buffer (void *b)
{
    // readonly buffer with given contents
    _len = _wptr = 0x7fffffff;
    _rptr = 0;
    _ralignbase = _walignbase = 0;
    _buf = (Octet *)b;
    _readonly = TRUE;
}

CORBA::Buffer::Buffer (ULong sz)
{
    // read/write buffer with given initial size
    if (sz < MINSIZE)
        sz = MINSIZE;
    _buf = alloc (sz);
    _len = sz;
    _rptr = _wptr = 0;
    _ralignbase = _walignbase = 0;
    _readonly = FALSE;
}

CORBA::Buffer::Buffer (const Buffer &b)
{
    _buf = alloc (b._len);
    memcpy (_buf, b._buf, b._len);
    _len = b._len;
    _rptr = b._rptr;
    _wptr = b._wptr;
    _ralignbase = b._ralignbase;
    _walignbase = b._walignbase;
    _readonly = FALSE;
}

CORBA::Buffer::~Buffer ()
{
    if (!_readonly)
        free (_buf);
}

CORBA::Buffer &
CORBA::Buffer::operator= (const Buffer &b)
{
    if (this != &b) {
	assert (!_readonly && !b._readonly);
	free (_buf);
	_buf = alloc (b._len);
	memcpy (_buf, b._buf, b._len);
	_len = b._len;
	_rptr = b._rptr;
	_wptr = b._wptr;
	_ralignbase = b._ralignbase;
	_walignbase = b._walignbase;
    }
    return *this;
}

CORBA::Boolean
CORBA::Buffer::operator== (const Buffer &b)
{
    assert (!_readonly && !b._readonly);
    return length() == b.length() && !memcmp (data(), b.data(), length());
}

CORBA::Octet *
CORBA::Buffer::alloc (ULong sz)
{
    Octet *b = (Octet *)::malloc (sz);
    assert (b);
    return b;
}

CORBA::Octet *
CORBA::Buffer::realloc (Octet *b, ULong osz, ULong nsz)
{
    Octet *nb = (Octet *)::realloc ((void *)b, nsz);
    assert (nb);
    return nb;
}

void
CORBA::Buffer::free (Octet *b)
{
    ::free ((void *)b);
}

void
CORBA::Buffer::reset (ULong sz)
{
    _rptr = 0;
    _ralignbase = _walignbase = 0;
    if (!_readonly) {
        _wptr = 0;
        if (sz < MINSIZE)
            sz = MINSIZE;
        if (_len < sz) {
	    free (_buf);
            _buf = alloc (sz);
            _len = sz;
        }
    }
}

void
CORBA::Buffer::doresize (ULong needed)
{
    assert (!_readonly);
    if (_wptr + needed > _len) {
        ULong nlen = (_len < RESIZE_THRESH)
            ? (2*_len)
            : (_len + RESIZE_INCREMENT);
        if (_wptr + needed > nlen)
            nlen = _wptr + needed;
	_buf = realloc (_buf, _len, nlen);
        _len = nlen;
    }
}

CORBA::Boolean
CORBA::Buffer::peek (void *b, ULong blen)
{
    if (_wptr - _rptr < blen)
        return FALSE;
    memcpy (b, &_buf[_rptr], blen);
    return TRUE;
}

CORBA::Boolean
CORBA::Buffer::peek (Octet &o)
{
    if (_wptr == _rptr)
        return FALSE;
    o = _buf[_rptr];
    return TRUE;
}

CORBA::Boolean
CORBA::Buffer::get (Octet &o)
{
    if (_wptr == _rptr)
        return FALSE;
    o = _buf[_rptr++];
    return TRUE;
}

CORBA::Boolean
CORBA::Buffer::get (void *b, ULong l)
{
    if (_wptr - _rptr < l)
	return FALSE;
    memcpy (b, &_buf[_rptr], l);
    _rptr += l;
    return TRUE;
}

CORBA::Boolean
CORBA::Buffer::get1 (void *p)
{
    if (_wptr == _rptr)
        return FALSE;
    *(Octet *)p = _buf[_rptr++];
    return TRUE;
}

CORBA::Boolean
CORBA::Buffer::get2 (void *p)
{
    // assert (((_rptr - _ralignbase) % 2) == 0);
    if (_rptr+2 > _wptr)
	return FALSE;
    // assume that pointers can be cast to long
    if (!((_rptr | (long)p)&1)) {
	*(CORBA::Short *)p = (CORBA::Short &)_buf[_rptr];
	_rptr += 2;
    } else {
	*((Octet * &)p)++ = _buf[_rptr++];
	*(Octet *)p = _buf[_rptr++];
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Buffer::get4 (void *p)
{
    // assert (((_rptr - _ralignbase) % 4) == 0);
    if (_rptr+4 > _wptr)
	return FALSE;
    if (!((_rptr | (long)p)&3)) {
	*(CORBA::Long *)p = (CORBA::Long &)_buf[_rptr];
	_rptr += 4;
    } else {
	*((Octet * &)p)++ = _buf[_rptr++];
	*((Octet * &)p)++ = _buf[_rptr++];
	*((Octet * &)p)++ = _buf[_rptr++];
	*(Octet *)p = _buf[_rptr++];
    }
    return TRUE;
}

CORBA::Boolean
CORBA::Buffer::get8 (void *p)
{
    // assert (((_rptr - _ralignbase) % 8) == 0);
    if (_rptr+8 > _wptr)
	return FALSE;
    if (!((_rptr | (long)p)&7)) {
	*(CORBA::LongLong *)p = (CORBA::LongLong &)_buf[_rptr];
	_rptr += 8;
    } else {
	*((Octet * &)p)++ = _buf[_rptr++];
	*((Octet * &)p)++ = _buf[_rptr++];
	*((Octet * &)p)++ = _buf[_rptr++];
	*((Octet * &)p)++ = _buf[_rptr++];
	*((Octet * &)p)++ = _buf[_rptr++];
	*((Octet * &)p)++ = _buf[_rptr++];
	*((Octet * &)p)++ = _buf[_rptr++];
	*(Octet *)p = _buf[_rptr++];
    }
    return TRUE;
}

// get 16 bytes with 8 byte alignment
CORBA::Boolean
CORBA::Buffer::get16 (void *p)
{
    // assert (((_rptr - _ralignbase) % 8) == 0);
    if (_rptr+16 > _wptr)
	return FALSE;
    if (!((_rptr | (long)p)&7)) {
	*((CORBA::LongLong * &)p)++ = (CORBA::LongLong &)_buf[_rptr];
	_rptr += 8;
	*(CORBA::LongLong *)p = (CORBA::LongLong &)_buf[_rptr];
	_rptr += 8;
    } else {
	*((Octet * &)p)++ = _buf[_rptr++];
	*((Octet * &)p)++ = _buf[_rptr++];
	*((Octet * &)p)++ = _buf[_rptr++];
	*((Octet * &)p)++ = _buf[_rptr++];
	*((Octet * &)p)++ = _buf[_rptr++];
	*((Octet * &)p)++ = _buf[_rptr++];
	*((Octet * &)p)++ = _buf[_rptr++];
	*((Octet * &)p)++ = _buf[_rptr++];
	*((Octet * &)p)++ = _buf[_rptr++];
	*((Octet * &)p)++ = _buf[_rptr++];
	*((Octet * &)p)++ = _buf[_rptr++];
	*((Octet * &)p)++ = _buf[_rptr++];
	*((Octet * &)p)++ = _buf[_rptr++];
	*((Octet * &)p)++ = _buf[_rptr++];
	*((Octet * &)p)++ = _buf[_rptr++];
	*(Octet *)p = _buf[_rptr++];
    }
    return TRUE;
}

void
CORBA::Buffer::replace (const void *o, ULong blen)
{
    assert (!_readonly);
    reset (blen);
    memcpy (&_buf[_wptr], o, blen);
    _wptr += blen;
}

void
CORBA::Buffer::replace (Octet o)
{
    assert (!_readonly);
    reset (1);
    _buf[_wptr++] = o;
}

void
CORBA::Buffer::put (const void *o, ULong l)
{
    assert (!_readonly);
    resize (l);
    memcpy (&_buf[_wptr], o, l);
    _wptr += l;
}

void
CORBA::Buffer::put (Octet o)
{
    assert (!_readonly);
    resize (1);
    _buf[_wptr++] = o;
}

void
CORBA::Buffer::put1 (const void *p)
{
    assert (!_readonly);
    resize (1);
    _buf[_wptr++] = *(const Octet *)p;
}

void
CORBA::Buffer::put2 (const void *p)
{
    // assert (((_wptr - _walignbase) % 2) == 0);
    assert (!_readonly && _wptr >= _walignbase);
    resize (2);
    CORBA::Octet *b = _buf + _wptr;
    if (!(((long)b | (long)p)&1)) {
	*((CORBA::Short *&)b)++ = *(const CORBA::Short *)p;
    } else {
	*b++ = *((const Octet * &)p)++;
	*b++ = *(const Octet *)p;
    }
    _wptr = b - _buf;
}

void
CORBA::Buffer::put4 (const void *p)
{
   // assert (((_wptr - _walignbase) % 4) == 0);
    assert (!_readonly && _wptr >= _walignbase);
    resize (4);
    CORBA::Octet *b = _buf + _wptr;
    if (!(((long)b | (long)p)&3)) {
	*((CORBA::Long * &)b)++ = *(const CORBA::Long *)p;
    } else {
	*b++ = *((const Octet * &)p)++;
	*b++ = *((const Octet * &)p)++;
	*b++ = *((const Octet * &)p)++;
	*b++ = *(const Octet *)p;
    }
    _wptr = b - _buf;
}

void
CORBA::Buffer::put8 (const void *p)
{
    // assert (((_wptr - _walignbase) % 8) == 0);
    assert (!_readonly && _wptr >= _walignbase);
    resize (8);
    CORBA::Octet *b = _buf + _wptr;
    if (!(((long)b | (long)p)&7)) {
	*((CORBA::LongLong *&)b)++ = *(const CORBA::LongLong *)p;
    } else {
	*b++ = *((const Octet * &)p)++;
	*b++ = *((const Octet * &)p)++;
	*b++ = *((const Octet * &)p)++;
	*b++ = *((const Octet * &)p)++;
	*b++ = *((const Octet * &)p)++;
	*b++ = *((const Octet * &)p)++;
	*b++ = *((const Octet * &)p)++;
	*b++ = *(const Octet *)p;
    }
    _wptr = b - _buf;
}

// put 16 bytes with 8 byte alignment
void
CORBA::Buffer::put16 (const void *p)
{
    //  assert (((_wptr - _walignbase) % 8) == 0);
    assert (!_readonly && _wptr >= _walignbase);
    resize (16);
    CORBA::Octet *b = _buf + _wptr;
    if (!(((long)b | (long)p)&7)) {
	*((CORBA::LongLong *&)b)++ = *((const CORBA::LongLong * &)p)++;
	*((CORBA::LongLong *&)b)++ = *(const CORBA::LongLong *)p;
    } else {
	*b++ = *((const Octet * &)p)++;
	*b++ = *((const Octet * &)p)++;
	*b++ = *((const Octet * &)p)++;
	*b++ = *((const Octet * &)p)++;
	*b++ = *((const Octet * &)p)++;
	*b++ = *((const Octet * &)p)++;
	*b++ = *((const Octet * &)p)++;
	*b++ = *((const Octet * &)p)++;
	*b++ = *((const Octet * &)p)++;
	*b++ = *((const Octet * &)p)++;
	*b++ = *((const Octet * &)p)++;
	*b++ = *((const Octet * &)p)++;
	*b++ = *((const Octet * &)p)++;
	*b++ = *((const Octet * &)p)++;
	*b++ = *((const Octet * &)p)++;
	*b++ = *(const Octet *)p;
    }
    _wptr = b - _buf;
}

void
CORBA::Buffer::dump (const char * desc, ostream &o) const
{
  CORBA::ULong i=_rptr, j;
  char temp[256];
  int l=0;

  while (i < _wptr) {
    sprintf (temp, "%10s  ", (i==_rptr) ? desc : "");
    o << temp;

    for (j=0; j<16 && i+j<_wptr; j++) {
      sprintf (temp, "%02x ", _buf[i+j]);
      o << temp;
    }

    for (; j<16; j++) {
      o << "   ";
    }

    o << " ";

    for (j=0; j<16 && i+j<_wptr; j++) {
      /*
       * Printable ISOLatin1 characters according to the Red Book
       */
      if ((_buf[i+j] >= 0040 && _buf[i+j] <= 0176) ||
	  (_buf[i+j] >= 0220 && _buf[i+j] != 0231 && _buf[i+j] != 0234)) {
	temp[j] = _buf[i+j];
      }
      else {
	temp[j] = '.';
      }
    }

    temp[j] = '\0';
    o << temp << endl;

    i += j;

    if (++l == 16 && i < _wptr) {
      o << endl;
      l = 0;
    }
  }
}
