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

#ifndef __mico_codec_h__
#define __mico_codec_h__

namespace CORBA {

/************************* ForwardDecls ******************************/


class Buffer;


/************************** Interfaces *******************************/


enum ByteOrder {
    BigEndian,
    LittleEndian,
    DefaultEndian
};


class DataEncoder {
public:
    struct EncapsState {
	ULong pos;
	ByteOrder bo;
	ULong align;
    };
    struct DelayedSeqState {
	ULong pos;
    };

    typedef std::map<ValueBase *, Long, std::less<ValueBase *> > MapValueId;

    class ValueState {
	friend class DataEncoder;
	Long chunk_level;
	Long nesting_level;
	Long tag_pos;
    public:
	MapValueId visited;

	void reset ()
	{
	    chunk_level = 0x7fffffff;
	    nesting_level = 0;
	    tag_pos = -1;
	    visited.erase (visited.begin(), visited.end());
	}
	ValueState ()
	{
	    chunk_level = 0x7fffffff;
	    nesting_level = 0;
	    tag_pos = -1;
	}
	ValueState (const ValueState &vs)
	    : chunk_level (vs.chunk_level), nesting_level (vs.nesting_level),
	      tag_pos (vs.tag_pos), visited (vs.visited)
	{
	}
	~ValueState ()
	{
	}
	ValueState &operator= (const ValueState &vs)
	{
	    chunk_level = vs.chunk_level;
	    nesting_level = vs.nesting_level;
	    tag_pos = vs.tag_pos;
	    visited = vs.visited;
	    return *this;
	}
    };

protected:
    Buffer *buf;
    Boolean dofree_buf;
    CodeSetCoder *conv;
    Boolean dofree_conv;
    ValueState *vstate;
    Boolean dofree_vstate;

public:
    DataEncoder ();
    DataEncoder (Buffer *b, Boolean dofree_b = TRUE,
                 CodeSetCoder *c = 0, Boolean dofree_c = TRUE,
		 ValueState *vs = 0, Boolean dofree_vs = TRUE);
    virtual ~DataEncoder ();

    virtual DataEncoder *clone () const = 0;
    virtual DataEncoder *clone (Buffer *b, Boolean dofree_b = TRUE,
                                CodeSetCoder *c = 0,
				Boolean dofree_c = TRUE,
				ValueState *vs = 0,
				Boolean dofree_vs = TRUE) const = 0;
    virtual DataDecoder *decoder () const = 0;
    virtual DataDecoder *decoder (Buffer *b, Boolean dofree_b = TRUE,
				  CodeSetCoder *c = 0,
				  Boolean dofree_c = TRUE) const = 0;
    virtual const char *type () const = 0;

    virtual void put_short (Short) = 0;
    virtual void put_ushort (UShort) = 0;
    virtual void put_long (Long) = 0;
    virtual void put_longlong (LongLong) = 0;
    virtual void put_ulong (ULong) = 0;
    virtual void put_ulonglong (ULongLong) = 0;
    virtual void put_float (Float) = 0;
    virtual void put_double (Double) = 0;
    virtual void put_longdouble (LongDouble) = 0;
    virtual void put_char (Char) = 0;
    virtual void put_char_raw (Char) = 0;
    virtual void put_wchar (WChar) = 0;
    virtual void put_octet (Octet) = 0;
    virtual void put_boolean (Boolean) = 0;
    virtual void put_string (const char *) = 0;
    virtual void put_string_raw (const char *) = 0;
    virtual void put_wstring (const wchar_t *) = 0;
    virtual void put_fixed (const FixedBase::FixedValue &value,
			    UShort digits, Short scale) = 0;

    virtual void put_shorts (const Short *, ULong) = 0;
    virtual void put_ushorts (const UShort *, ULong) = 0;
    virtual void put_longs (const Long *, ULong) = 0;
    virtual void put_longlongs (const LongLong *, ULong) = 0;
    virtual void put_ulongs (const ULong *, ULong) = 0;
    virtual void put_ulonglongs (const ULongLong *, ULong) = 0;
    virtual void put_floats (const Float *, ULong) = 0;
    virtual void put_doubles (const Double *, ULong) = 0;
    virtual void put_longdoubles (const LongDouble *, ULong) = 0;
    virtual void put_chars (const Char *, ULong) = 0;
    virtual void put_chars_raw (const Char *, ULong) = 0;
    virtual void put_wchars (const WChar *, ULong) = 0;
    virtual void put_booleans (const Boolean *, ULong) = 0;

    virtual void put_context (const Context &, ContextList_ptr = 0);
    virtual void put_principal (const Principal &);
    virtual void put_any (const Any &);
    virtual void put_typecode (const TypeCode &);
    virtual void put_ior (const IOR &);

    void put_string (const std::string &s);
    void put_string_raw (const std::string &s);
    virtual void put_buffer (const Buffer &);
    virtual void put_octets (const void *, ULong len);

    virtual void enumeration (ULong);

    virtual void struct_begin ()
    {}
    virtual void struct_end ()
    {}
    
    virtual void except_begin (const std::string &repoid);
    virtual void except_end ()
    {}

    virtual void seq_begin (ULong);
    virtual void seq_end ()
    {}

    virtual void encaps_begin (EncapsState &state);
    virtual void encaps_end (EncapsState &state);

    virtual void delayed_seq_begin (DelayedSeqState &state);
    virtual void delayed_seq_end (DelayedSeqState &state, ULong size);

    virtual void union_begin ()
    {}
    virtual void union_end ()
    {}

    virtual void arr_begin ()
    {}
    virtual void arr_end ()
    {}

    virtual void value_begin (const std::string &url,
			      const std::vector<std::string> &repoids,
			      Boolean chunked,
			      Long &value_id,
			      ValueState &state);
    void value_begin (const std::string &url,
		      const std::vector<std::string> &repoids,
		      Boolean chunked,
		      Long &value_id);
    virtual void value_end (Long value_id, ValueState &state);
    void value_end (Long value_id);
    virtual void value_ref (Long value_id);

    virtual ULong max_alignment () const = 0;

    Buffer *buffer ()
    { return buf; }

    void buffer (Buffer *, Boolean dofree = TRUE);

    virtual ByteOrder byteorder () const;
    virtual void byteorder (ByteOrder);

    CodeSetCoder *converter ()
    { return conv; }

    void converter (CodeSetCoder *, Boolean dofree = TRUE);

    ValueState *valuestate ()
    { return vstate; }

    void valuestate (ValueState *vs, Boolean dofree = TRUE);
};


class DataDecoder {
public:
    typedef DataEncoder::EncapsState EncapsState;

    struct ValueState1 {
      Boolean chunking;
      Long nesting_level;
      Long saved_pos;
      Long end_of_chunk;

      void reset ()
	{
	  chunking = FALSE;
	  nesting_level = 0;
	  saved_pos = -1;
	  end_of_chunk = -1;
	}
      ValueState1 ()
	{
	  reset();
	}
    };
    typedef std::map<Long, ValueState1, std::less<Long> > MapIdState;
    typedef std::map<Long, ValueBase *, std::less<Long> > MapIdValue;

    class ValueState {
	friend class DataDecoder;
	ValueState1 s;
	MapIdState skipped;
    public:
	MapIdValue visited;

	void reset ()
	{
	    s.reset();
	    skipped.erase (skipped.begin(), skipped.end());
	    visited.erase (visited.begin(), visited.end());
	}

	ValueState ()
	{
	}
	ValueState (const ValueState &vs)
	    : s (vs.s), skipped (vs.skipped), visited (vs.visited)
	{
	}
	~ValueState ()
	{
	}
	ValueState &operator= (const ValueState &vs)
	{
	    s = vs.s;
	    skipped = vs.skipped;
	    visited = vs.visited;
	    return *this;
	}
    };

protected:
    Buffer *buf;
    Boolean dofree_buf;
    CodeSetCoder *conv;
    Boolean dofree_conv;
    ValueState *vstate;
    Boolean dofree_vstate;

    Boolean get_indirect_string (std::string &s);
    Boolean get_indirect_string_seq (std::vector<std::string> &s);
public:

    DataDecoder (Buffer *b, Boolean dofree_b = TRUE,
                 CodeSetCoder *c = 0, Boolean dofree_c = TRUE,
		 ValueState *vs = 0, Boolean dofree_vs = TRUE);
    virtual ~DataDecoder ();

    virtual DataDecoder *clone () const = 0;
    virtual DataDecoder *clone (Buffer *b, Boolean dofree_b = TRUE,
				CodeSetCoder *c = 0,
				Boolean dofree_c = TRUE,
				ValueState *vs = 0,
				Boolean dofree_vs = TRUE) const = 0;
    virtual DataEncoder *encoder () const = 0;
    virtual DataEncoder *encoder (Buffer *b, Boolean dofree_b = TRUE,
				  CodeSetCoder *c = 0,
				  Boolean dofree_c = TRUE) const = 0;
    virtual const char *type () const = 0;

    virtual Boolean get_short (Short &) = 0;
    virtual Boolean get_ushort (UShort &) = 0;
    virtual Boolean get_long (Long &) = 0;
    virtual Boolean get_longlong (LongLong &) = 0;
    virtual Boolean get_ulong (ULong &) = 0;
    virtual Boolean get_ulonglong (ULongLong &) = 0;
    virtual Boolean get_float (Float &) = 0;
    virtual Boolean get_double (Double &) = 0;
    virtual Boolean get_longdouble (LongDouble &) = 0;
    virtual Boolean get_char (Char &) = 0;
    virtual Boolean get_char_raw (Char &) = 0;
    virtual Boolean get_wchar (WChar &) = 0;
    virtual Boolean get_octet (Octet &) = 0;
    virtual Boolean get_boolean (Boolean &) = 0;
    virtual Boolean get_string (String_out) = 0;
    virtual Boolean get_string_raw (String_out) = 0;
    virtual Boolean get_wstring (WString_out) = 0;
    virtual Boolean get_fixed (FixedBase::FixedValue &val,
			       UShort digits, Short scale) = 0;

    virtual Boolean get_shorts (Short *, ULong) = 0;
    virtual Boolean get_ushorts (UShort *, ULong) = 0;
    virtual Boolean get_longs (Long *, ULong) = 0;
    virtual Boolean get_longlongs (LongLong *, ULong) = 0;
    virtual Boolean get_ulongs (ULong *, ULong) = 0;
    virtual Boolean get_ulonglongs (ULongLong *, ULong) = 0;
    virtual Boolean get_floats (Float *, ULong) = 0;
    virtual Boolean get_doubles (Double *, ULong) = 0;
    virtual Boolean get_longdoubles (LongDouble *, ULong) = 0;
    virtual Boolean get_chars (Char *, ULong) = 0;
    virtual Boolean get_chars_raw (Char *, ULong) = 0;
    virtual Boolean get_wchars (WChar *, ULong) = 0;
    virtual Boolean get_booleans (Boolean *, ULong) = 0;

    virtual Boolean get_context (Context &);
    virtual Boolean get_principal (Principal &);
    virtual Boolean get_any (Any &);
    virtual Boolean get_typecode (TypeCode &);
    virtual Boolean get_ior (IOR &);

    Boolean get_string_stl (std::string &);
    Boolean get_string_raw_stl (std::string &);
    virtual Boolean get_octets (void *, ULong len);

    virtual Boolean enumeration (ULong &);

    virtual Boolean struct_begin ()
    { return TRUE; }
    virtual Boolean struct_end ()
    { return TRUE; }

    virtual Boolean except_begin (std::string &repoid);
    virtual Boolean except_end ()
    { return TRUE; }

    virtual Boolean seq_begin (ULong &);
    virtual Boolean seq_end ()
    { return TRUE; }

    virtual Boolean encaps_begin (EncapsState &state, ULong &len);
    virtual Boolean encaps_end (EncapsState &state);

    virtual Boolean union_begin ()
    { return TRUE; }
    virtual Boolean union_end ()
    { return TRUE; }

    virtual Boolean arr_begin ()
    { return TRUE; }
    virtual Boolean arr_end ()
    { return TRUE; }

    virtual Boolean value_begin (std::string &url,
				 std::vector<std::string> &repoids,
				 Long &value_id,
				 Boolean &is_ref,
				 ValueState &state);
    Boolean value_begin (std::string &url,
			 std::vector<std::string> &repoids,
			 Long &value_id,
			 Boolean &is_ref);
    virtual Boolean value_end (Long value_id, ValueState &state);
    Boolean value_end (Long value_id);

    Boolean begin_chunk ();
    Boolean end_chunk ();

    Boolean check_chunk ()
    {
      if (!vstate || !vstate->s.chunking || vstate->s.end_of_chunk == -1) {
	return TRUE;
      }
      if (vstate->s.end_of_chunk >= 0 && buf->rpos() >= (CORBA::ULong)vstate->s.end_of_chunk) {
	return end_chunk() && begin_chunk();
      }
      return TRUE;
    }

    virtual ULong max_alignment () const = 0;

    Buffer *buffer ()
    { return buf; }

    void buffer (Buffer *, Boolean dofree = TRUE);

    virtual ByteOrder byteorder () const;
    virtual void byteorder (ByteOrder);

    CodeSetCoder *converter ()
    { return conv; }

    void converter (CodeSetCoder *, Boolean dofree = TRUE);

    ValueState *valuestate ()
    { return vstate; }

    void valuestate (ValueState *vs, Boolean dofree = TRUE);
};

}

#endif // __mico_codec_h__
