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

#ifndef __mico_codeset_h__
#define __mico_codeset_h__

namespace CORBA {

class Codeset {
public:
    typedef CORBA::ULong CodesetId;
    typedef CORBA::UShort CharsetId;

    enum { MAX_CHARSETS = 5 };

    struct Info {
	CodesetId id;
	CORBA::UShort codepoint_size;
	CORBA::UShort max_codepoints;
	CharsetId charsets[MAX_CHARSETS];
	const char *desc;
	const char *name;
    };

    enum SpecialCS {
	_SpecialMin,
	NativeCS = 0,
	NativeWCS,
	FallbackCS,
	FallbackWCS,
	DefaultCS,
	DefaultWCS,
	_SpecialMax
    };
private:
    static CORBA::Boolean _disabled;
    static CORBA::Boolean _enabled_in_iiopprofile;
    static Codeset *_special[_SpecialMax];
    Info *_info;

    Codeset ();
    Codeset (Info *);
public:
    static void disable (CORBA::Boolean d)
    { _disabled = d; }

    static CORBA::Boolean disabled ()
    { return _disabled; }

    static void enable_in_iiop_profile(CORBA::Boolean value)
    { _enabled_in_iiopprofile = value; }

    static CORBA::Boolean enabled_in_iiop_profile()
    { return _enabled_in_iiopprofile; }

    Codeset (const Codeset &);
    ~Codeset ();

    static void _init ();

    static Info *_find_info (CodesetId);
    static Info *_find_info (const char *);

    static Codeset *create (CodesetId);
    static Codeset *create (const char *);

    static Codeset *special_cs (SpecialCS);
    static void set_special_cs (SpecialCS, Codeset *);

    CORBA::Boolean is_compatible (const Codeset *) const;
    CORBA::Boolean is_compatible (CodesetId) const;

    CodesetId id () const
    { return _info->id; }

    const char *name () const
    { return _info->name; }

    const char *desc () const
    { return _info->desc; }

    CORBA::UShort codepoint_size () const
    { return _info->codepoint_size; }

    CORBA::UShort max_codepoints () const
    { return _info->max_codepoints; }

    CORBA::ULong guess_size (CORBA::ULong nchars) const
    { return (nchars+1) * _info->max_codepoints; }

    static void free();
};

/*
 * Unfortunately, the encoding of (wide) chars and strings does not
 * depend only of the charset, but also on other things such as GIOP
 * version. Therefore, we define an abstract class for encoding and
 * decoding them.
 */

class CodeSetCoder {
public:
  virtual ~CodeSetCoder ();

  virtual CodeSetCoder * clone () = 0;
  virtual CORBA::Boolean isok () = 0;

  /*
   * Decode
   */
  
  virtual CORBA::Boolean get_char (CORBA::DataDecoder &,
				   CORBA::Char &) = 0;
  virtual CORBA::Boolean get_chars (CORBA::DataDecoder &,
				    CORBA::Char *,
				    CORBA::ULong) = 0;
  virtual CORBA::Boolean get_string (CORBA::DataDecoder &,
				     CORBA::String_out,
				     CORBA::ULong) = 0;
  virtual CORBA::Boolean get_wchar (CORBA::DataDecoder &,
				    CORBA::WChar &) = 0;
  virtual CORBA::Boolean get_wchars (CORBA::DataDecoder &,
				     CORBA::WChar *,
				     CORBA::ULong) = 0;
  virtual CORBA::Boolean get_wstring (CORBA::DataDecoder &,
				      CORBA::WString_out,
				      CORBA::ULong) = 0;
  
  /*
   * Encode
   */
  
  virtual CORBA::Boolean put_char (CORBA::DataEncoder &,
				   CORBA::Char) = 0;
  virtual CORBA::Boolean put_chars (CORBA::DataEncoder &,
				    const CORBA::Char *,
				    CORBA::ULong) = 0;
  virtual CORBA::Boolean put_string (CORBA::DataEncoder &,
				     const char *,
				     CORBA::ULong) = 0;
  virtual CORBA::Boolean put_wchar (CORBA::DataEncoder &,
				    CORBA::WChar) = 0;
  virtual CORBA::Boolean put_wchars (CORBA::DataEncoder &,
				     const CORBA::WChar *,
				     CORBA::ULong) = 0;
  virtual CORBA::Boolean put_wstring (CORBA::DataEncoder &,
				      const CORBA::WChar *,
				      CORBA::ULong) = 0;
};

}

#endif // __mico_codeset_h__
