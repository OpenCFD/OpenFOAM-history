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

#ifndef __mico_codec_impl_h__
#define __mico_codec_impl_h__

namespace MICO {

class CDREncoder : public CORBA::DataEncoder {
    CORBA::ByteOrder data_bo, mach_bo;
public:
    CDREncoder ();
    CDREncoder (CORBA::Buffer *, CORBA::Boolean do_free = TRUE,
		CORBA::ByteOrder = CORBA::DefaultEndian,
                CORBA::CodeSetCoder *c = 0, CORBA::Boolean dofree_c = TRUE,
		ValueState *vs = 0, CORBA::Boolean dofree_vs = TRUE);
    ~CDREncoder ();
    
    CORBA::DataEncoder *clone () const;
    CORBA::DataEncoder *clone (CORBA::Buffer *,
    			       CORBA::Boolean dofree_b = TRUE,
                               CORBA::CodeSetCoder *c = 0,
                               CORBA::Boolean dofree_c = TRUE,
			       ValueState *vs = 0,
			       CORBA::Boolean dofree_vs = TRUE) const;
    CORBA::DataDecoder *decoder () const;
    CORBA::DataDecoder *decoder (CORBA::Buffer *,
    				 CORBA::Boolean dofree_b = TRUE,
    				 CORBA::CodeSetCoder *c = 0,
    				 CORBA::Boolean dofree_c = TRUE) const;
    const char *type () const;
    
    void put_short (CORBA::Short);
    void put_ushort (CORBA::UShort);
    void put_long (CORBA::Long);
    void put_longlong (CORBA::LongLong);
    void put_ulong (CORBA::ULong);
    void put_ulonglong (CORBA::ULongLong);
    void put_float (CORBA::Float);
    void put_double (CORBA::Double);
    void put_longdouble (CORBA::LongDouble);
    void put_char (CORBA::Char);
    void put_char_raw (CORBA::Char);
    void put_wchar (CORBA::WChar);
    void put_octet (CORBA::Octet);
    void put_boolean (CORBA::Boolean);
    void put_string (const char *);
    void put_string_raw (const char *);
    void put_wstring (const wchar_t *);
    void put_fixed (const FixedBase::FixedValue &value,
    		    CORBA::UShort digits,
    		    CORBA::Short scale);
    
    
    void put_shorts (const CORBA::Short *, CORBA::ULong);
    void put_ushorts (const CORBA::UShort *, CORBA::ULong);
    void put_longs (const CORBA::Long *, CORBA::ULong);
    void put_longlongs (const CORBA::LongLong *, CORBA::ULong);
    void put_ulongs (const CORBA::ULong *, CORBA::ULong);
    void put_ulonglongs (const CORBA::ULongLong *, CORBA::ULong);
    void put_floats (const CORBA::Float *, CORBA::ULong);
    void put_doubles (const CORBA::Double *, CORBA::ULong);
    void put_longdoubles (const CORBA::LongDouble *, CORBA::ULong);
    void put_chars (const CORBA::Char *, CORBA::ULong);
    void put_chars_raw (const CORBA::Char *, CORBA::ULong);
    void put_wchars (const CORBA::WChar *, CORBA::ULong);
    void put_booleans (const CORBA::Boolean *, CORBA::ULong);

    CORBA::ULong max_alignment () const;
    
    CORBA::ByteOrder byteorder () const;
    void byteorder (CORBA::ByteOrder);
};


class CDRDecoder : public CORBA::DataDecoder {
    CORBA::ByteOrder data_bo, mach_bo;
public:
    CDRDecoder ();
    CDRDecoder (CORBA::Buffer *b, CORBA::Boolean do_free = TRUE,
		CORBA::ByteOrder = CORBA::DefaultEndian,
                CORBA::CodeSetCoder *c = 0, CORBA::Boolean dofree_c = TRUE,
		ValueState *vs = 0, CORBA::Boolean dofree_vs = TRUE);
    ~CDRDecoder ();
    
    CORBA::DataDecoder *clone () const;
    CORBA::DataDecoder *clone (CORBA::Buffer *b,
    			       CORBA::Boolean dofree_b = TRUE,
                               CORBA::CodeSetCoder *c = 0,
                               CORBA::Boolean dofree_c = TRUE,
			       ValueState *vs = 0,
			       CORBA::Boolean dofree_vs = TRUE) const;
    CORBA::DataEncoder *encoder () const;
    CORBA::DataEncoder *encoder (CORBA::Buffer *b,
    				 CORBA::Boolean dofree_b = TRUE,
    				 CORBA::CodeSetCoder *c = 0,
    				 CORBA::Boolean dofree_c = TRUE) const;
    const char *type () const;
    
    CORBA::Boolean get_short (CORBA::Short &);
    CORBA::Boolean get_ushort (CORBA::UShort &);
    CORBA::Boolean get_long (CORBA::Long &);
    CORBA::Boolean get_longlong (CORBA::LongLong &);
    CORBA::Boolean get_ulong (CORBA::ULong &);
    CORBA::Boolean get_ulonglong (CORBA::ULongLong &);
    CORBA::Boolean get_float (CORBA::Float &);
    CORBA::Boolean get_double (CORBA::Double &);
    CORBA::Boolean get_longdouble (CORBA::LongDouble &);
    CORBA::Boolean get_char (CORBA::Char &);
    CORBA::Boolean get_char_raw (CORBA::Char &);
    CORBA::Boolean get_wchar (CORBA::WChar &);
    CORBA::Boolean get_octet (CORBA::Octet &);
    CORBA::Boolean get_boolean (CORBA::Boolean &);
    CORBA::Boolean get_string (CORBA::String_out);
    CORBA::Boolean get_string_raw (CORBA::String_out);
    CORBA::Boolean get_wstring (CORBA::WString_out);
    CORBA::Boolean get_fixed (FixedBase::FixedValue &val,
    			      CORBA::UShort digits,
    			      CORBA::Short scale);
    
    CORBA::Boolean get_shorts (CORBA::Short *, CORBA::ULong);
    CORBA::Boolean get_ushorts (CORBA::UShort *, CORBA::ULong);
    CORBA::Boolean get_longs (CORBA::Long *, CORBA::ULong);
    CORBA::Boolean get_longlongs (CORBA::LongLong *, CORBA::ULong);
    CORBA::Boolean get_ulongs (CORBA::ULong *, CORBA::ULong);
    CORBA::Boolean get_ulonglongs (CORBA::ULongLong *, CORBA::ULong);
    CORBA::Boolean get_floats (CORBA::Float *, CORBA::ULong);
    CORBA::Boolean get_doubles (CORBA::Double *, CORBA::ULong);
    CORBA::Boolean get_longdoubles (CORBA::LongDouble *, CORBA::ULong);
    CORBA::Boolean get_chars (CORBA::Char *, CORBA::ULong);
    CORBA::Boolean get_chars_raw (CORBA::Char *, CORBA::ULong);
    CORBA::Boolean get_wchars (CORBA::WChar *, CORBA::ULong);
    CORBA::Boolean get_booleans (CORBA::Boolean *, CORBA::ULong);

    CORBA::ULong max_alignment () const;

    CORBA::ByteOrder byteorder () const;
    void byteorder (CORBA::ByteOrder);
};

}

#endif // __mico_codec_impl_h__
