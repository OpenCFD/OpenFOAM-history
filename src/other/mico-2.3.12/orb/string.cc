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
#ifndef _WIN32
#include <string.h>
#endif
#include <mico/util.h>
#include <mico/template_impl.h>

#endif // FAST_PCH

/************************* String support **************************/

/*
 * initializing the string here saves a lot of code elsewhere:
 * structs, unions, sequences, arrays and valuetypes all require
 * that their string members are initialized to the empty string.
 *
 * By using a special value for the empty string here, we don't need
 * to do a string_dup("") whenever a String_var is allocated. This
 * saves a lot of time if a lot of strings are used, i.e. a
 * sequence<string>.
 */

static const char * the_empty_string = "";

CORBA::String_var::String_var ()
  : _str ((char *) the_empty_string)
{
}

CORBA::String_var::String_var (char *s)
{
  _str = s;
}

CORBA::String_var::String_var (const char *s)
{
  _str = CORBA::string_dup (s);
}

CORBA::String_var::String_var (const String_var &s)
{
  if (s._str == the_empty_string) {
    _str = s._str;
  }
  else {
    _str = CORBA::string_dup (s._str);
  }
}

CORBA::String_var::~String_var ()
{
  if (_str != the_empty_string) {
    CORBA::string_free (_str);
  }
}

CORBA::String_var &
CORBA::String_var::operator= (char *s)
{
  if (_str != the_empty_string) {
    CORBA::string_free (_str);
  }
  _str = s;
  return *this;
}

CORBA::String_var &
CORBA::String_var::operator= (const char *s)
{
  if (_str != the_empty_string) {
    CORBA::string_free (_str);
  }
  _str = CORBA::string_dup (s);
  return *this;
}

CORBA::String_var &
CORBA::String_var::operator= (const String_var &s)
{
  if (this != &s) {
    if (_str != the_empty_string) {
      CORBA::string_free (_str);
    }
    
    if (s._str == the_empty_string) {
      _str = s._str;
    }
    else {
      _str = CORBA::string_dup (s._str);
    }
  }
  return *this;
}

CORBA::Boolean
CORBA::String_var::operator== (const String_var &s) const
{
  if (!s._str || !_str) {
    return !s._str && !_str;
  }
  return !strcmp (s._str, _str);
}

char *
CORBA::String_var::_retn ()
{
  char *s = _str;
  _str = 0;
  
  if (s == the_empty_string) {
    s = CORBA::string_dup ("");
  }
  
  return s;
}

const char *
CORBA::String_var::in () const
{
  return _str;
}

char *&
CORBA::String_var::out ()
{
  if (_str != the_empty_string) {
    CORBA::string_free (_str);
  }
  _str = 0;
  return _str;
}

char *&
CORBA::String_var::inout ()
{
  if (_str == the_empty_string) {
    _str = CORBA::string_dup ("");
  }

  return _str;
}


CORBA::String_out::String_out (char*& s)
    : _str (s)
{
    _str = 0;
}

CORBA::String_out::String_out (String_var& s)
    : _str (s._str)
{
  if (_str != the_empty_string) {
    CORBA::string_free (_str);
  }
  _str = 0;
}

CORBA::String_out::String_out (const String_out& s)
    : _str (s._str)
{
}

CORBA::String_out&
CORBA::String_out::operator= (const String_out& s)
{
    _str = s._str;
    return *this;
}

CORBA::String_out&
CORBA::String_out::operator= (char* s)
{
    _str = s;
    return *this;
}

CORBA::String_out&
CORBA::String_out::operator= (const char* s)
{
    _str = CORBA::string_dup (s);
    return *this;
}


char *
CORBA::string_alloc (ULong len)
{
  char *s = new char[len+1];
  s[0] = 0;
  return s;
}

char *
CORBA::string_dup (const char *s2)
{
  if (!s2)
    return 0;

  char *s = new char[strlen (s2) + 1];
  strcpy (s, s2);
  return s;
}

char *
CORBA::string_ndup (const char *s2, ULong len)
{
  if (!s2)
    return 0;
  
  char *s = new char[len+1];
  strncpy (s, s2, len);
  s[len] = 0;
  return s;
}

void
CORBA::string_free (char *s)
{
  if (s)
    delete[] s;
}


/************************* WString support **************************/


CORBA::WString_var::WString_var ()
{
    // initializing the string here saves a lot of code elsewhere:
    // structs, unions, sequences, arrays and valuetypes all require
    // that their string members are initialized to the empty string
    _str = CORBA::wstring_dup (L"");
}

CORBA::WString_var::WString_var (wchar_t *s)
{
    _str = s;
}

CORBA::WString_var::WString_var (const wchar_t *s)
{
    _str = CORBA::wstring_dup (s);
}

CORBA::WString_var::WString_var (const WString_var &s)
{
    _str = CORBA::wstring_dup (s._str);
}

CORBA::WString_var::~WString_var ()
{
    CORBA::wstring_free (_str);
}

CORBA::WString_var &
CORBA::WString_var::operator= (wchar_t *s)
{
    CORBA::wstring_free (_str);
    _str = s;
    return *this;
}

CORBA::WString_var &
CORBA::WString_var::operator= (const wchar_t *s)
{
    CORBA::wstring_free (_str);
    _str = CORBA::wstring_dup (s);
    return *this;
}

CORBA::WString_var &
CORBA::WString_var::operator= (const WString_var &s)
{
    if (this != &s) {
	CORBA::wstring_free (_str);
	_str = CORBA::wstring_dup (s._str);
    }
    return *this;
}

CORBA::Boolean
CORBA::WString_var::operator== (const WString_var &s) const
{
    if (!s._str || !_str)
        return !s._str && !_str;
    return !xwcscmp (s._str, _str);
}

wchar_t *
CORBA::WString_var::_retn ()
{
    wchar_t *s = _str;
    _str = 0;
    return s;
}

const wchar_t *
CORBA::WString_var::in () const
{
    return _str;
}

wchar_t *&
CORBA::WString_var::out ()
{
    CORBA::wstring_free (_str);
    _str = 0;
    return _str;
}

wchar_t *&
CORBA::WString_var::inout ()
{
    return _str;
}


CORBA::WString_out::WString_out (wchar_t*& s)
    : _str (s)
{
    _str = 0;
}

CORBA::WString_out::WString_out (WString_var& s)
    : _str (s._str)
{
    CORBA::wstring_free (_str);
    _str = 0;
}

CORBA::WString_out::WString_out (const WString_out& s)
    : _str (s._str)
{
}

CORBA::WString_out&
CORBA::WString_out::operator= (const WString_out& s)
{
    _str = s._str;
    return *this;
}

CORBA::WString_out&
CORBA::WString_out::operator= (wchar_t* s)
{
    _str = s;
    return *this;
}

CORBA::WString_out&
CORBA::WString_out::operator= (const wchar_t* s)
{
    _str = CORBA::wstring_dup (s);
    return *this;
}



wchar_t *
CORBA::wstring_alloc (ULong len)
{
    wchar_t *s = new wchar_t[len+1];
    s[0] = 0;
    return s;
}

wchar_t *
CORBA::wstring_dup (const wchar_t *s2)
{
    if (!s2)
        return 0;
    wchar_t *s = new wchar_t[xwcslen (s2) + 1];
    xwcscpy (s, s2);
    return s;
}

wchar_t *
CORBA::wstring_ndup (const wchar_t *s2, ULong len)
{
    if (!s2)
        return 0;
    wchar_t *s = new wchar_t[len+1];
    xwcsncpy (s, s2, len);
    s[len] = 0;
    return s;
}

void
CORBA::wstring_free (wchar_t *s)
{
    if (s)
        delete[] s;
}
