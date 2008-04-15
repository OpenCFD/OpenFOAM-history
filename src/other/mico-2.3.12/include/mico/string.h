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

#ifndef __mico_string_h__
#define __mico_string_h__

namespace CORBA {

class String_out;

class String_var {

    friend class String_out;
    
private:
    char* _str;
    
public:
    String_var ();
    String_var (char *);
    String_var (const char *);
    String_var (const String_var &);
    ~String_var ();

    String_var &operator= (char *);
    String_var &operator= (const char *);
    String_var &operator= (const String_var &);
    // begin-mico-extension
    Boolean operator== (const String_var &s) const;
    // end-mico-extension

    // g++ const overload problem
#ifdef HAVE_CONST_OVERLOAD
    operator const char* () const
    {
        return _str;
    }

    operator char* ()
    {
        return _str;
    }

#else // HAVE_CONST_OVERLOAD
    operator char* () const
    {
        return _str;
    }
#endif // HAVE_CONST_OVERLOAD

    char &operator[] (ULong idx)
    { return _str[idx]; }

    char operator[] (ULong idx) const
    { return _str[idx]; }

    const char *in () const;
    char *&out ();
    char *&inout ();
    char *&_for_demarshal ()
    { return out(); }

    char * _retn ();
};


class String_out {
private:
    char*& _str;

public:
    String_out (char *&);
    String_out (String_var &);
    String_out (const String_out &);

    String_out &operator= (const String_out &);
    String_out &operator= (char *);
    String_out &operator= (const char *);

    operator char*& ()
    {
        return _str;
    }

    char*& ptr ()
    {
        return _str;
    }

private:
    // assignment from String_var is not allowed
    void operator= (const String_var&);
};


extern char *string_alloc (ULong len);
extern char *string_dup (const char *);
extern char *string_ndup (const char *, ULong len);
extern void string_free (char *);


class WString_out;

class WString_var {

    friend class WString_out;
    
    wchar_t* _str;
    
public:
    WString_var ();
    WString_var (wchar_t *);
    WString_var (const wchar_t *);
    WString_var (const WString_var &);
    ~WString_var ();

    WString_var &operator= (wchar_t *);
    WString_var &operator= (const wchar_t *);
    WString_var &operator= (const WString_var &);
    // begin-mico-extension
    Boolean operator== (const WString_var &s) const;
    // end-mico-extension

    // g++ const overload problem
#ifdef HAVE_CONST_OVERLOAD
    operator const wchar_t * () const
    {
        return _str;
    }

    operator wchar_t * ()
    {
        return _str;
    }
#else // HAVE_CONST_OVERLOAD
    operator wchar_t* () const
    {
        return _str;
    }
#endif

    wchar_t &operator[] (ULong idx)
    { return _str[idx]; }

    wchar_t operator[] (ULong idx) const
    { return _str[idx]; }

    const wchar_t *in () const;
    wchar_t *&out ();
    wchar_t *&inout ();
    wchar_t *&_for_demarshal ()
    { return out(); }

    wchar_t * _retn ();
};


class WString_out {
private:
    wchar_t*& _str;

public:
    WString_out (wchar_t *&);
    WString_out (WString_var &);
    WString_out (const WString_out &);

    WString_out &operator= (const WString_out &);
    WString_out &operator= (wchar_t *);
    WString_out &operator= (const wchar_t *);

    operator wchar_t*& ()
    {
        return _str;
    }

    wchar_t*& ptr ()
    {
        return _str;
    }

private:
    // assignment from WString_var is not allowed
    void operator= (const WString_var&);
};


extern wchar_t *wstring_alloc (ULong len);
extern wchar_t *wstring_dup (const wchar_t *);
extern wchar_t *wstring_ndup (const wchar_t *, ULong len);
extern void wstring_free (wchar_t *);

}

#endif // __mico_string_h__
