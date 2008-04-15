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

#ifndef __mico_codeset_impl_h__
#define __mico_codeset_impl_h__

namespace MICO {

/*
 * Codeset Component
 */

class CodesetComponent : public CORBA::Component {
public:
    typedef CORBA::ULong CodeSetId;
private:
    CodeSetId _native_cs;
    CodeSetId _native_wcs;
    std::vector<CodeSetId> _conv_cs;
    std::vector<CodeSetId> _conv_wcs;
    CodeSetId _selected_cs;
    CodeSetId _selected_wcs;
public:
    CodesetComponent ();
    CodesetComponent (CodeSetId native_cs, CodeSetId native_wcs,
                      const std::vector<CodeSetId> &conv_cs,
                      const std::vector<CodeSetId> &conv_wcs);
    CodesetComponent (const CodesetComponent &csc);
    ~CodesetComponent ();
    
    void encode (CORBA::DataEncoder &) const;
    ComponentId id () const;
    void print (std::ostream &) const;
    
    CORBA::Component *clone () const;
    CORBA::Long compare (const CORBA::Component &) const;
    CORBA::Boolean operator== (const CORBA::Component &) const;
    CORBA::Boolean operator< (const CORBA::Component &) const;

    CodeSetId selected_cs ();
    CodeSetId selected_wcs ();
};


class CodesetComponentDecoder : public CORBA::ComponentDecoder {
public:
    CodesetComponentDecoder ();
    ~CodesetComponentDecoder ();
    
    CORBA::Component *decode (CORBA::DataDecoder &,
    			      ComponentId, CORBA::ULong) const;
    CORBA::Boolean has_id (ComponentId) const;
};

/*
 * Codeset Converter
 */

class CodesetConv {
protected:
  CORBA::Codeset *_from;
  CORBA::Codeset *_to;
  
  CodesetConv ();
  CodesetConv (const CodesetConv &);
  CodesetConv (CORBA::Codeset *, CORBA::Codeset *);
public:
  virtual ~CodesetConv ();

  static CodesetConv *create (CORBA::Codeset *from,
			      CORBA::Codeset *to);
  static CORBA::Boolean can_convert (CORBA::Codeset::CodesetId from,
				     CORBA::Codeset::CodesetId to);
  
  CORBA::Codeset *from ()
    { return _from; }

  CORBA::Codeset *to ()
    { return _to; }
  
  virtual CodesetConv *clone () const;
  /*
   * both from and to codesets can be byte oriented and non byte oriented.
   * there is one convert() method for each of those four combinations.
   */
  virtual CORBA::Long encode (const CORBA::Char *from, CORBA::ULong len,
			      CORBA::Buffer &to,
			      CORBA::Boolean terminate = TRUE);
  virtual CORBA::Long encode (const CORBA::WChar *from, CORBA::ULong len,
			      CORBA::Buffer &to,
			      CORBA::Boolean terminate = TRUE);
  virtual CORBA::Long decode (CORBA::Buffer &from, CORBA::ULong len,
			      CORBA::WChar *to,
			      CORBA::Boolean terminate = TRUE);
  virtual CORBA::Long decode (CORBA::Buffer &from, CORBA::ULong len,
			      CORBA::Char *to,
			      CORBA::Boolean terminate = TRUE);
};

class UniCodesetConv : public CodesetConv {
protected:
  UniCodesetConv ();
  UniCodesetConv (const UniCodesetConv &);
  UniCodesetConv (CORBA::Codeset *, CORBA::Codeset *);
  
  static CORBA::Boolean supported_csid (CORBA::Codeset::CodesetId);
  
  CORBA::Long convert (const char *from, CORBA::ULong len,
		       char *to);
public:
  ~UniCodesetConv ();
  
  static CodesetConv *create (CORBA::Codeset *from,
			      CORBA::Codeset *to);
  static CORBA::Boolean can_convert (CORBA::Codeset::CodesetId from,
				     CORBA::Codeset::CodesetId to);
  
  CodesetConv *clone () const;
  /*
   * both from and to codesets can be byte oriented and non byte oriented.
   * there is one convert() method for each of those four combinations.
   */
  CORBA::Long encode (const CORBA::Char *from, CORBA::ULong len,
		      CORBA::Buffer &to,
		      CORBA::Boolean terminate = TRUE);
  CORBA::Long encode (const CORBA::WChar *from, CORBA::ULong len,
		      CORBA::Buffer &to,
		      CORBA::Boolean terminate = TRUE);
  CORBA::Long decode (CORBA::Buffer &from, CORBA::ULong len,
		      CORBA::WChar *to,
		      CORBA::Boolean terminate = TRUE);
  CORBA::Long decode (CORBA::Buffer &from, CORBA::ULong len,
		      CORBA::Char *to,
		      CORBA::Boolean terminate = TRUE);
};

/*
 * CodeSetCoder implementation for GIOP 1.0
 *   - TCS-C is ISO 8859-1
 *   - no TCS-W
 */

class GIOP_1_0_CodeSetCoder : public CORBA::CodeSetCoder
{
private:
  MICO::CodesetConv * _conv;
  CORBA::Boolean _isok;

public:
  GIOP_1_0_CodeSetCoder ();
  ~GIOP_1_0_CodeSetCoder ();
  
  CORBA::CodeSetCoder * clone ();
  CORBA::Boolean isok ();
  
  CORBA::Boolean get_char (CORBA::DataDecoder &, CORBA::Char &);
  CORBA::Boolean get_chars (CORBA::DataDecoder &, CORBA::Char *, CORBA::ULong);
  CORBA::Boolean get_string (CORBA::DataDecoder &, CORBA::String_out,
			     CORBA::ULong);
  
  CORBA::Boolean get_wchar (CORBA::DataDecoder &, CORBA::WChar &);
  CORBA::Boolean get_wchars (CORBA::DataDecoder &, CORBA::WChar *, CORBA::ULong);
  CORBA::Boolean get_wstring (CORBA::DataDecoder &, CORBA::WString_out,
			      CORBA::ULong);
  
  CORBA::Boolean put_char (CORBA::DataEncoder &, CORBA::Char);
  CORBA::Boolean put_chars (CORBA::DataEncoder &, const CORBA::Char *, CORBA::ULong);
  CORBA::Boolean put_string (CORBA::DataEncoder &, const char *, CORBA::ULong);
  
  CORBA::Boolean put_wchar (CORBA::DataEncoder &, CORBA::WChar);
  CORBA::Boolean put_wchars (CORBA::DataEncoder &, const CORBA::WChar *, CORBA::ULong);
  CORBA::Boolean put_wstring (CORBA::DataEncoder &, const CORBA::WChar *, CORBA::ULong);
};

/*
 * CodeSetCoder implementation for GIOP 1.1
 *   - conversion is performed from and to TCS-C
 *   - handling of TCS-W is ignored, it isn't portable anyway
 *
 * If _conv is NULL; then it is assumed that TCS-C is the same as
 * the native charset, and that no conversion is necessary
 */

class GIOP_1_1_CodeSetCoder : public CORBA::CodeSetCoder
{
protected:
  CORBA::Codeset::CodesetId _tcsc;
private:
  MICO::CodesetConv * _conv_n2t;
  MICO::CodesetConv * _conv_t2n;
  CORBA::ULong _max_codepoints;
  CORBA::ULong _codepoint_size;
  CORBA::ULong _native_maxcp;
  CORBA::Boolean _isok;

public:
  GIOP_1_1_CodeSetCoder (CORBA::Codeset::CodesetId tcsc);
  ~GIOP_1_1_CodeSetCoder ();

  CORBA::CodeSetCoder * clone ();
  CORBA::Boolean isok ();
  
  CORBA::Boolean get_char (CORBA::DataDecoder &, CORBA::Char &);
  CORBA::Boolean get_chars (CORBA::DataDecoder &, CORBA::Char *, CORBA::ULong);
  CORBA::Boolean get_string (CORBA::DataDecoder &, CORBA::String_out,
			     CORBA::ULong);
  
  CORBA::Boolean get_wchar (CORBA::DataDecoder &, CORBA::WChar &);
  CORBA::Boolean get_wchars (CORBA::DataDecoder &, CORBA::WChar *, CORBA::ULong);
  CORBA::Boolean get_wstring (CORBA::DataDecoder &, CORBA::WString_out,
			      CORBA::ULong);
  
  CORBA::Boolean put_char (CORBA::DataEncoder &, CORBA::Char);
  CORBA::Boolean put_chars (CORBA::DataEncoder &, const CORBA::Char *, CORBA::ULong);
  CORBA::Boolean put_string (CORBA::DataEncoder &, const char *, CORBA::ULong);
  
  CORBA::Boolean put_wchar (CORBA::DataEncoder &, CORBA::WChar);
  CORBA::Boolean put_wchars (CORBA::DataEncoder &, const CORBA::WChar *, CORBA::ULong);
  CORBA::Boolean put_wstring (CORBA::DataEncoder &, const CORBA::WChar *, CORBA::ULong);
};

/*
 * CodeSetCoder implementation for GIOP 1.2
 *   - correct handling of TCS-C and TCS-W
 *
 * char and string encoding is the same as in GIOP 1.1, so we
 * inherit the GIOP 1.1 encoder and just overload the wchar and
 * wstring functions.
 *
 * CORBA 2.4 introduced special handling of UTF-16 (optional Byte
 * Order Markers, default Big Endian) without changing the GIOP
 * version number. This is getting complicated ...
 */

class GIOP_1_2_CodeSetCoder : public GIOP_1_1_CodeSetCoder
{
private:
  CORBA::Codeset::CodesetId _tcsw;
  MICO::CodesetConv * _w_conv_n2t;
  MICO::CodesetConv * _w_conv_t2n;
  CORBA::ULong _w_max_codepoints;
  CORBA::ULong _w_codepoint_size;
  CORBA::ULong _w_native_maxcp;
  CORBA::Boolean _w_isok;
      
public:
  GIOP_1_2_CodeSetCoder (CORBA::Codeset::CodesetId tcsc,
			 CORBA::Codeset::CodesetId tcsw);
  ~GIOP_1_2_CodeSetCoder ();
  
  CORBA::CodeSetCoder * clone ();
  CORBA::Boolean isok ();
  
  CORBA::Boolean get_wchar (CORBA::DataDecoder &, CORBA::WChar &);
  CORBA::Boolean get_wchars (CORBA::DataDecoder &, CORBA::WChar *, CORBA::ULong);
  CORBA::Boolean get_wstring (CORBA::DataDecoder &, CORBA::WString_out,
			      CORBA::ULong);
  
  CORBA::Boolean put_wchar (CORBA::DataEncoder &, CORBA::WChar);
  CORBA::Boolean put_wchars (CORBA::DataEncoder &, const CORBA::WChar *, CORBA::ULong);
  CORBA::Boolean put_wstring (CORBA::DataEncoder &, const CORBA::WChar *, CORBA::ULong);
};

}

#endif // __mico_codeset_impl_h__
