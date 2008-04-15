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

#define MICO_CONF_IMR

#include <CORBA-SMALL.h>
#ifndef _WIN32
#include <string.h>
#endif
#include <mico/impl.h>
#include <mico/template_impl.h>
#include <mico/util.h>

#endif // FAST_PCH


using namespace std;

/**************************** DataEncoder ******************************/


CORBA::DataEncoder::DataEncoder ()
{
    buf = new Buffer;
    dofree_buf = TRUE;
    conv = 0;
    dofree_conv = FALSE;
    vstate = 0;
    dofree_vstate = FALSE;
}

CORBA::DataEncoder::DataEncoder (Buffer *b, Boolean dofree_b,
                                 CodeSetCoder *c, Boolean dofree_c,
				 ValueState *vs, Boolean dofree_vs)
{
    buf = b;
    dofree_buf = dofree_b;
    conv = c;
    dofree_conv = dofree_c;
    vstate = vs;
    dofree_vstate = dofree_vs;
}

CORBA::DataEncoder::~DataEncoder ()
{
    if (dofree_buf)
	delete buf;
    if (dofree_conv && conv)
        delete conv;
    if (dofree_vstate && vstate)
	delete vstate;
}

void
CORBA::DataEncoder::put_buffer (const Buffer &b)
{
    buf->put (b.data(), b.length());
}

void
CORBA::DataEncoder::put_octets (const void *data, ULong len)
{
    buf->put (data, len);
}

void
CORBA::DataEncoder::put_string (const string &s)
{
    put_string (s.c_str());
}

void
CORBA::DataEncoder::put_string_raw (const string &s)
{
    put_string_raw (s.c_str());
}

void
CORBA::DataEncoder::put_context (const Context &ctx, ContextList_ptr clist)
{
    ctx.encode (*this, clist);
}

void
CORBA::DataEncoder::put_principal (const Principal &pr)
{
    pr.encode (*this);
}

void
CORBA::DataEncoder::put_any (const Any &a)
{
    a.encode (*this);
}

void
CORBA::DataEncoder::put_typecode (const TypeCode &t)
{
    t.encode (*this);
}

void
CORBA::DataEncoder::put_ior (const IOR &ior)
{
    ior.encode (*this);
}

void
CORBA::DataEncoder::enumeration (ULong val)
{
    put_ulong (val);
}

void
CORBA::DataEncoder::except_begin (const string &repoid)
{
    put_string (repoid);
}

void
CORBA::DataEncoder::seq_begin (ULong l)
{
    put_ulong (l);
}

void
CORBA::DataEncoder::encaps_begin (EncapsState &state)
{
    /*
     * XXX [12-9] says the aligment base for the encapsulated data
     * is the first octet of the encapsulated data.
     */
    state.align = buffer()->walign_base ();
    state.bo = byteorder();
    state.pos = buffer()->wpos();

    put_ulong (0);

    buffer()->walign_base (buffer()->wpos());

    put_octet (byteorder() == CORBA::LittleEndian ? TRUE : FALSE);
}

void
CORBA::DataEncoder::encaps_end (EncapsState &state)
{
    byteorder (state.bo);
    buffer()->walign_base (state.align);

    ULong end = buffer()->wpos();

    buffer()->wseek_beg (state.pos);
    put_ulong (0);

    ULong data_start = buffer()->wpos();
    buffer()->wseek_beg (state.pos);
    put_ulong (end-data_start);

    buffer()->wseek_beg (end);
}

void
CORBA::DataEncoder::delayed_seq_begin (DelayedSeqState &state)
{
    state.pos = buffer()->wpos();
    put_ulong (0);
}

void
CORBA::DataEncoder::delayed_seq_end (DelayedSeqState &state, ULong l)
{
    ULong pos = buffer()->wpos();
    buffer()->wseek_beg (state.pos);
    put_ulong (l);
    buffer()->wseek_beg (pos);
}

void
CORBA::DataEncoder::value_begin (const string &url,
				 const vector<string> &repoids,
				 Boolean chunked,
				 Long &value_id)
{
    assert (vstate);
    value_begin (url, repoids, chunked, value_id, *vstate);
}

void
CORBA::DataEncoder::value_begin (const string &url,
				 const vector<string> &repoids,
				 Boolean chunked,
				 Long &value_id,
				 ValueState &state)
{
    if (state.chunk_level <= state.nesting_level && state.tag_pos >= 0) {
	// close the previous chunk
	if (state.tag_pos + sizeof(Long) == buf->wpos()) {
	    // chunk is empty
	    buf->wseek_beg (state.tag_pos);
	} else {
	    // chunk contains data, write out chunk length
	    Long wpos = buf->wpos();
	    buf->wseek_beg (state.tag_pos);
	    put_long (wpos - state.tag_pos - sizeof (Long));
	    buf->wseek_beg (wpos);
	}
    }

    state.nesting_level += 1;
    if (chunked && state.chunk_level > state.nesting_level)
	state.chunk_level = state.nesting_level;

    Long value_tag = 0x7fffff00;

    if (url.length() > 0)
	// url
	value_tag |= 0x01;

    if (repoids.size() == 1) {
	// single repoid
	value_tag |= 0x02;
    } else if (repoids.size() > 1) {
	// repoid sequence
	value_tag |= 0x06;
    }

    if (state.chunk_level <= state.nesting_level)
	// chunked
	value_tag |= 0x08;

    put_long (value_tag);

    // +1 to avoid value_id 0
    value_id = buf->wpos() - sizeof(Long) + 1;

    if (url.length() > 0)
	put_string (url);

    if (repoids.size() == 1) {
	put_string (repoids[0]);
    } else if (repoids.size() > 1) {
	seq_begin (repoids.size());
	for (mico_vec_size_type i = 0; i < repoids.size(); ++i)
	    put_string (repoids[i]);
	seq_end();
    }

    if (state.chunk_level <= state.nesting_level) {
	// chunked, write out tag for next chunk
	put_long (0);
	state.tag_pos = buf->wpos() - sizeof (Long);
    }
}

void
CORBA::DataEncoder::value_end (Long value_id)
{
    assert (vstate);
    value_end (value_id, *vstate);
}

void
CORBA::DataEncoder::value_end (Long value_id, ValueState &state)
{
    assert (state.nesting_level > 0);

    if (!(state.chunk_level <= state.nesting_level)) {
	// not chunked
	state.nesting_level -= 1;
	return;
    }

    if (state.tag_pos >= 0) {
	// close the previous chunk
	if (state.tag_pos + sizeof(Long) == buf->wpos()) {
	    // chunk is empty
	    buf->wseek_beg (state.tag_pos);
	} else {
	    // chunk contains data, write out chunk length
	    Long wpos = buf->wpos();
	    buf->wseek_beg (state.tag_pos);
	    put_long (wpos - state.tag_pos - sizeof (Long));
	    buf->wseek_beg (wpos);
	}
    }

    // write end tag
    put_long (-state.nesting_level);

    state.nesting_level -= 1;
    if (state.nesting_level > 0 &&
	state.chunk_level <= state.nesting_level) {
	// still chunked, write out tag for next chunk
	put_long (0);
	state.tag_pos = buf->wpos() - sizeof (Long);
    } else {
	// now follows unchunked data (or no data at all)
	state.chunk_level = 0x7fffffff;
	state.tag_pos = -1;
    }
}

void
CORBA::DataEncoder::value_ref (Long value_id)
{
    if (value_id == 0) {
	// nil
	put_long (0);
    } else {
	// indirection
	put_long (0xffffffff);
	// -1 because value_id is buffer pos of referred value +1
	put_long (value_id - 1 - buf->wpos());
    }
}

void
CORBA::DataEncoder::buffer (Buffer *b, Boolean release)
{
    if (dofree_buf)
	delete buf;
    buf = b;
    dofree_buf = release;
}

void
CORBA::DataEncoder::converter (CodeSetCoder *c, Boolean release)
{
    if (dofree_conv && conv)
	delete conv;
    conv = c;
    dofree_conv = release;
}

void
CORBA::DataEncoder::valuestate (ValueState *vs, Boolean release)
{
    if (dofree_vstate && vstate)
	delete vstate;
    vstate = vs;
    dofree_vstate = release;
}

CORBA::ByteOrder
CORBA::DataEncoder::byteorder () const
{
#if defined(HAVE_BYTEORDER_BE)
    return CORBA::BigEndian;
#else
    return CORBA::LittleEndian;
#endif
}

void
CORBA::DataEncoder::byteorder (CORBA::ByteOrder bo)
{
    assert (bo == byteorder());
}


/**************************** DataDecoder ******************************/


CORBA::DataDecoder::DataDecoder (Buffer *b, Boolean dofree_b,
                                 CodeSetCoder *c, Boolean dofree_c,
				 ValueState *vs, Boolean dofree_vs)
{
    buf = b;
    dofree_buf = dofree_b;
    conv = c;
    dofree_conv = dofree_c;
    vstate = vs;
    dofree_vstate = dofree_vs;
}

CORBA::DataDecoder::~DataDecoder ()
{
    if (dofree_buf)
	delete buf;
    if (dofree_conv && conv)
        delete conv;
    if (dofree_vstate && vstate)
	delete vstate;
}

CORBA::Boolean
CORBA::DataDecoder::get_octets (void *data, ULong len)
{
    return buf->get (data, len);
}

CORBA::Boolean
CORBA::DataDecoder::get_string_stl (string &str)
{
    char *s;
    if (!get_string (s))
        return FALSE;
    str = s;
    CORBA::string_free (s);
    return TRUE;
}

CORBA::Boolean
CORBA::DataDecoder::get_string_raw_stl (string &str)
{
    char *s;
    if (!get_string_raw (s))
        return FALSE;
    str = s;
    CORBA::string_free (s);
    return TRUE;
}

CORBA::Boolean
CORBA::DataDecoder::get_context (Context &ctx)
{
    return ctx.decode (*this);
}

CORBA::Boolean
CORBA::DataDecoder::get_principal (Principal &pr)
{
    return pr.decode (*this);
}

CORBA::Boolean
CORBA::DataDecoder::get_any (Any &a)
{
    return a.decode (*this);
}

CORBA::Boolean
CORBA::DataDecoder::get_typecode (TypeCode &t)
{
    return t.decode (*this);
}

CORBA::Boolean
CORBA::DataDecoder::get_ior (IOR &ior)
{
    return ior.decode (*this);
}

CORBA::Boolean
CORBA::DataDecoder::enumeration (ULong &val)
{
    return get_ulong (val);
}

CORBA::Boolean
CORBA::DataDecoder::except_begin (string &repoid)
{
    return get_string_stl (repoid);
}

CORBA::Boolean
CORBA::DataDecoder::seq_begin (ULong &l)
{
  /*
   * We want to be protected from ill-formed messages that advertise
   * an invalid sequence length (e.g. with 1G members). Ultimately,
   * this would fail at the end of the message while still reading
   * members, but at that point, the program has already failed from
   * a memory allocation error.
   *
   * We do a crude check here whether the number of elements is larger
   * than the number of bytes left in the message, which is impossible.
   */

  if (!get_ulong (l)) {
    return FALSE;
  }

  if (l > buf->length()) {
    return FALSE;
  }

  return TRUE;
}

CORBA::Boolean
CORBA::DataDecoder::encaps_begin (EncapsState &state, ULong &len)
{
    /*
     * XXX [12-9] says the aligment base for the encapsulated data
     * is the first octet of the encapsulated data.
     */
    state.align = buffer()->ralign_base();
    state.bo = byteorder();

    if (!get_ulong (len))
	return FALSE;

    if (len > buf->length()) {
      return FALSE;
    }

    buffer()->ralign_base (buffer()->rpos());

    if (len == 0) {
        // if encapsulation contains no data then we dont need to
        // know the byteorder ...
        return TRUE;
    }
    --len;

    Octet bo;
    if (!get_octet (bo))
	return FALSE;
    byteorder (bo == 0 ? CORBA::BigEndian : CORBA::LittleEndian);
    return TRUE;
}

CORBA::Boolean
CORBA::DataDecoder::encaps_end (EncapsState &state)
{
    byteorder (state.bo);
    buffer()->ralign_base (state.align);
    return TRUE;
}

CORBA::Boolean
CORBA::DataDecoder::get_indirect_string (string &s)
{
    Long rpos = buf->rpos();
    ULong tag = 0;

    if (!get_ulong (tag))
	return FALSE;
    if (tag == 0xffffffff) {
	// indirection
	Long indir;
	rpos = buf->rpos();
	if (!get_long (indir))
	    return FALSE;
	indir += rpos;
	rpos = buf->rpos();
	if (!buf->rseek_beg (indir))
	    return FALSE;
	if (!get_string_stl (s))
	    return FALSE;
	buf->rseek_beg (rpos);
    } else {
	// string
	buf->rseek_beg (rpos);
	if (!get_string_stl (s))
	    return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::DataDecoder::get_indirect_string_seq (vector<string> &v)
{
    Long rpos = buf->rpos();
    ULong tag = 0;

    if (!get_ulong (tag))
	return FALSE;
    if (tag == 0xffffffff) {
	// indirection
	Long indir;
	rpos = buf->rpos();
	if (!get_long (indir))
	    return FALSE;
	indir += rpos;
	rpos = buf->rpos();
	if (!buf->rseek_beg (indir))
	    return FALSE;

	ULong len;
	string s;
	if (!seq_begin (len))
	    return FALSE;
	for (ULong i = 0; i < len; ++i) {
	    if (!get_string_stl (s))
		return FALSE;
	    v.push_back (s);
	}
	if (!seq_end ())
	    return FALSE;
	buf->rseek_beg (rpos);
    } else {
	// sequence<string>
	buf->rseek_beg (rpos);

	ULong len;
	string s;
	if (!seq_begin (len))
	    return FALSE;
	for (ULong i = 0; i < len; ++i) {
	    if (!get_string_stl (s))
		return FALSE;
	    v.push_back (s);
	}
	if (!seq_end ())
	    return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
CORBA::DataDecoder::begin_chunk ()
{
  assert (vstate);
  assert (vstate->s.chunking);

  CORBA::Long length_of_chunk;

  if (!get_long (length_of_chunk)) {
    return FALSE;
  }

  if (length_of_chunk < 0 || (CORBA::ULong)length_of_chunk > buf->length()) {
    return FALSE;
  }

  vstate->s.end_of_chunk = buf->rpos () + length_of_chunk;

  return TRUE;
}

CORBA::Boolean
CORBA::DataDecoder::end_chunk ()
{
  assert (vstate);
  assert (vstate->s.chunking);

  if (vstate->s.end_of_chunk == -1) {
    return TRUE;
  }

  if (!buf->rseek_beg (vstate->s.end_of_chunk)) {
    return FALSE;
  }

  vstate->s.end_of_chunk = -1;
  return TRUE;
}

CORBA::Boolean
CORBA::DataDecoder::value_begin (string &url,
				 vector<string> &repoids,
				 Long &value_id,
				 Boolean &is_ref)
{
    assert (vstate);
    return value_begin (url, repoids, value_id, is_ref, *vstate);
}

CORBA::Boolean
CORBA::DataDecoder::value_begin (string &url,
				 vector<string> &repoids,
				 Long &value_id,
				 Boolean &is_ref,
				 ValueState &state)
{
    ULong value_tag;

    if (!get_ulong (value_tag))
	return FALSE;

    if (value_tag == 0xffffffff) {
	// indirection
	Long indir;
	if (!get_long (indir))
	    return FALSE;

	// +1 to avoid value_id 0
	value_id = buf->rpos() - sizeof(Long) + indir + 1;

	MapIdState::iterator i = state.skipped.find (value_id);
	if (i != state.skipped.end() && (*i).second.saved_pos < 0) {
	    // the actual encoding of the value has been
	    // skipped, so we have to decode it now instead
	    // of returning an indirection ...
	    ValueState1 tmp_state = state.s;
	    tmp_state.saved_pos = buf->rpos();

	    state.s = (*i).second;
	    (*i).second = tmp_state;

	    buf->rseek_beg (value_id-1);
	    return value_begin (url, repoids, value_id, is_ref, state);
	} else {
	    // already decoded earlier, return an indirection
	    is_ref = TRUE;
	    return TRUE;
	}
    }
    if (value_tag == 0) {
	// nil
	value_id = 0;
	is_ref = TRUE;
	return TRUE;
    }
    if (!(0x7fffff00 <= value_tag && value_tag <= 0x7fffffff)) {
      if (MICO::Logger::IsLogged (MICO::Logger::Info)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Info)
	  << "Info: DataDecoder::value_begin(): bad tag: "
	  << "0x" << hex << value_tag << endl;
      }
      return FALSE;
    }
    
    // value
    // +1 to avoid value_id 0
    value_id = buf->rpos() - sizeof (Long) + 1;
    is_ref = FALSE;

    if (value_tag & 0x01) {
	// url
	if (!get_indirect_string (url))
	    return FALSE;
    }

    repoids.erase (repoids.begin(), repoids.end());
    if ((value_tag & 0x06) == 0x02) {
	// one repoid
	string s;
	if (!get_indirect_string (s))
	    return FALSE;
	repoids.push_back (s);
    } else if ((value_tag & 0x06) == 0x06) {
	// sequence of repoids
	if (!get_indirect_string_seq (repoids))
	    return FALSE;
    }

    if (value_tag & 0x08) {
      state.s.chunking = 1;
    }

    if (state.s.chunking) {
      state.s.nesting_level++;
      if (!begin_chunk()) {
	return FALSE;
      }
    }

    return TRUE;
}

CORBA::Boolean
CORBA::DataDecoder::value_end (Long finished_value_id)
{
    assert (vstate);
    return value_end (finished_value_id, *vstate);
}

CORBA::Boolean
CORBA::DataDecoder::value_end (Long finished_value_id, ValueState &state)
{
    // must not be called if value_begin() returned an indirection or nil,
    // i.e. if is_ref == TRUE!

    MapIdState::iterator i = state.skipped.find (finished_value_id);
    if (i != state.skipped.end() && (*i).second.saved_pos >= 0) {
	// we just finished decoding a value that had been
	// skipped earlier ...
	buf->rseek_beg ((*i).second.saved_pos);

	state.s = (*i).second;
	state.s.saved_pos = -1;

	state.skipped.erase (i);
	return TRUE;
    }

    if (state.s.chunking) {
      if (!end_chunk()) {
	return FALSE;
      }

      Long level = state.s.nesting_level + 1;
      Long tag, pos = 0;

      while (level > state.s.nesting_level) {
	pos = buf->rpos();

	if (!get_long (tag)) {
	  return FALSE;
	}
	if (tag > 0 && tag < 0x7fffff00) {
	  /*
	   * Another chunk follows with truncated data. Ignore.
	   */
	  buf->rseek_beg (pos);
	  if (!begin_chunk() || !end_chunk()) {
	    return FALSE;
	  }
	}
	else if (tag < 0) {
	  /*
	   * end tag
	   */
	  level = -tag;
	}
	else {
	  /*
	   * nested value follows
	   */

	  buf->rseek_beg (pos);
	  
	  string url;
	  vector<string> repoids;
	  Long value_id;
	  Boolean is_ref;
	  ValueState1 saved_state = state.s;
	  
	  if (!value_begin (url, repoids, value_id, is_ref, state))
	    return FALSE;
	  if (!is_ref) {
	    if (!value_end (value_id, state))
	      return FALSE;
	    // later we may come across indirections that refer
	    // to the value we just skipped. when we come across
	    // the first such reference we have to treat the
	    // indirection as if is was the value itself. therefore
	    // we have to save the state ...
	    saved_state.saved_pos = -1;
	    state.skipped[value_id] = saved_state;
	  }
	}
      }

      if (level < state.s.nesting_level) {
	/*
	 * seek back before end tag, so that outer end_value()
	 * can read it
	 */
	buf->rseek_beg (pos);
      }

      if (--state.s.nesting_level == 0) {
	state.s.chunking = FALSE;
      }
    }

    return TRUE;
}

void
CORBA::DataDecoder::buffer (Buffer *b, Boolean release)
{
    if (dofree_buf)
	delete buf;
    buf = b;
    dofree_buf = release;
}

void
CORBA::DataDecoder::converter (CodeSetCoder *c, Boolean release)
{
    if (dofree_conv && conv)
	delete conv;
    conv = c;
    dofree_conv = release;
}

void
CORBA::DataDecoder::valuestate (ValueState *vs, Boolean release)
{
    if (dofree_vstate && vstate)
	delete vstate;
    vstate = vs;
    dofree_vstate = release;
}

CORBA::ByteOrder
CORBA::DataDecoder::byteorder () const
{
#if defined(HAVE_BYTEORDER_BE)
    return CORBA::BigEndian;
#else
    return CORBA::LittleEndian;
#endif
}

void
CORBA::DataDecoder::byteorder (CORBA::ByteOrder bo)
{
    assert (bo == byteorder());
}


/**************************** CDREncoder ******************************/

static inline void swap2 (void *d, const void *s)
{
    ((CORBA::Octet *)d)[0] = ((CORBA::Octet *)s)[1];
    ((CORBA::Octet *)d)[1] = ((CORBA::Octet *)s)[0];
}

static inline void swap4 (void *d, const void *s)
{
    ((CORBA::Octet *)d)[0] = ((CORBA::Octet *)s)[3];
    ((CORBA::Octet *)d)[1] = ((CORBA::Octet *)s)[2];
    ((CORBA::Octet *)d)[2] = ((CORBA::Octet *)s)[1];
    ((CORBA::Octet *)d)[3] = ((CORBA::Octet *)s)[0];
}

static inline void swap8 (void *d, const void *s)
{
    ((CORBA::Octet *)d)[0] = ((CORBA::Octet *)s)[7];
    ((CORBA::Octet *)d)[1] = ((CORBA::Octet *)s)[6];
    ((CORBA::Octet *)d)[2] = ((CORBA::Octet *)s)[5];
    ((CORBA::Octet *)d)[3] = ((CORBA::Octet *)s)[4];
    ((CORBA::Octet *)d)[4] = ((CORBA::Octet *)s)[3];
    ((CORBA::Octet *)d)[5] = ((CORBA::Octet *)s)[2];
    ((CORBA::Octet *)d)[6] = ((CORBA::Octet *)s)[1];
    ((CORBA::Octet *)d)[7] = ((CORBA::Octet *)s)[0];
}


// OpenBSD defines macro swap16
static inline void swap_16 (void *d, const void *s)
{
    ((CORBA::Octet *)d)[0]  = ((CORBA::Octet *)s)[15];
    ((CORBA::Octet *)d)[1]  = ((CORBA::Octet *)s)[14];
    ((CORBA::Octet *)d)[2]  = ((CORBA::Octet *)s)[13];
    ((CORBA::Octet *)d)[3]  = ((CORBA::Octet *)s)[12];
    ((CORBA::Octet *)d)[4]  = ((CORBA::Octet *)s)[11];
    ((CORBA::Octet *)d)[5]  = ((CORBA::Octet *)s)[10];
    ((CORBA::Octet *)d)[6]  = ((CORBA::Octet *)s)[9];
    ((CORBA::Octet *)d)[7]  = ((CORBA::Octet *)s)[8];
    ((CORBA::Octet *)d)[8]  = ((CORBA::Octet *)s)[7];
    ((CORBA::Octet *)d)[9]  = ((CORBA::Octet *)s)[6];
    ((CORBA::Octet *)d)[10] = ((CORBA::Octet *)s)[5];
    ((CORBA::Octet *)d)[11] = ((CORBA::Octet *)s)[4];
    ((CORBA::Octet *)d)[12] = ((CORBA::Octet *)s)[3];
    ((CORBA::Octet *)d)[13] = ((CORBA::Octet *)s)[2];
    ((CORBA::Octet *)d)[14] = ((CORBA::Octet *)s)[1];
    ((CORBA::Octet *)d)[15] = ((CORBA::Octet *)s)[0];
}

MICO::CDREncoder::CDREncoder ()
{
#ifdef HAVE_BYTEORDER_BE
    mach_bo = CORBA::BigEndian;
#else
    mach_bo = CORBA::LittleEndian;
#endif
    data_bo = mach_bo;
}

MICO::CDREncoder::CDREncoder (CORBA::Buffer *b, CORBA::Boolean dofree_b,
			      CORBA::ByteOrder _bo,
                              CORBA::CodeSetCoder *c, CORBA::Boolean dofree_c,
			      ValueState *vs, CORBA::Boolean dofree_vs)
    : CORBA::DataEncoder (b, dofree_b, c, dofree_c,
			  vs, dofree_vs)
{
#ifdef HAVE_BYTEORDER_BE
    mach_bo = CORBA::BigEndian;
#else
    mach_bo = CORBA::LittleEndian;
#endif
    data_bo = (_bo == CORBA::DefaultEndian) ? mach_bo : _bo;
}

MICO::CDREncoder::~CDREncoder ()
{
}

void
MICO::CDREncoder::put_short (CORBA::Short s)
{
  buf->walign (2);
    if (mach_bo == data_bo) {
	buf->put2 (&s);
    } else {
	CORBA::Short s2;
	swap2 (&s2, &s);
	buf->put2 (&s2);
    }
}

void
MICO::CDREncoder::put_ushort (CORBA::UShort us)
{
  buf->walign (2);
    if (mach_bo == data_bo) {
	buf->put2 (&us);
    } else {
	CORBA::UShort us2;
	swap2 (&us2, &us);
	buf->put2 (&us2);
    }
}

void
MICO::CDREncoder::put_long (CORBA::Long l)
{
  buf->walign (4);
    if (mach_bo == data_bo) {
	buf->put4 (&l);
    } else {
	CORBA::Long l2;
	swap4 (&l2, &l);
	buf->put4 (&l2);
    }
}

void
MICO::CDREncoder::put_longlong (CORBA::LongLong l)
{
  buf->walign (8);
    if (mach_bo == data_bo) {
	buf->put8 (&l);
    } else {
	CORBA::LongLong l2;
	swap8 (&l2, &l);
	buf->put8 (&l2);
    }
}

void
MICO::CDREncoder::put_ulong (CORBA::ULong ul)
{
  buf->walign (4);
    if (mach_bo == data_bo) {
	buf->put4 (&ul);
    } else {
	CORBA::ULong ul2;
	swap4 (&ul2, &ul);
	buf->put4 (&ul2);
    }
}

void
MICO::CDREncoder::put_ulonglong (CORBA::ULongLong ul)
{
  buf->walign (8);
    if (mach_bo == data_bo) {
	buf->put8 (&ul);
    } else {
	CORBA::ULongLong ul2;
	swap8 (&ul2, &ul);
	buf->put8 (&ul2);
    }
}

void
MICO::CDREncoder::put_float (CORBA::Float f)
{
  buf->walign (4);
#ifdef HAVE_IEEE_FP
    if (mach_bo == data_bo) {
	buf->put4 (&f);
    } else {
	CORBA::Float f2;
	swap4 (&f2, &f);
	buf->put4 (&f2);
    }
#else
    CORBA::Octet b[4];
    mico_float2ieee (b, f);
    if (mach_bo == data_bo) {
	buf->put4 (b);
    } else {
	CORBA::Octet b2[4];
	swap4 (b2, b);
	buf->put4 (b2);
    }
#endif
}

void
MICO::CDREncoder::put_double (CORBA::Double d)
{
  buf->walign (8);
#ifdef HAVE_IEEE_FP
    if (mach_bo == data_bo) {
	buf->put8 (&d);
    } else {
	CORBA::Double d2;
	swap8 (&d2, &d);
	buf->put8 (&d2);
    }
#else
    CORBA::Octet b[8];
    mico_double2ieee (b, d);
    if (mach_bo == data_bo) {
	buf->put8 (b);
    } else {
	CORBA::Octet b2[8];
	swap8 (b2, b);
	buf->put8 (b2);
    }
#endif
}

void
MICO::CDREncoder::put_longdouble (CORBA::LongDouble d)
{
  buf->walign (8);
#if defined(HAVE_IEEE_FP) && SIZEOF_LONG_DOUBLE == 16
    if (mach_bo == data_bo) {
	buf->put16 (&d);
    } else {
	CORBA::LongDouble d2;
	swap_16 (&d2, &d);
	buf->put16 (&d2);
    }
#else
    CORBA::Octet b[16];
    mico_ldouble2ieee (b, d);
    if (mach_bo == data_bo) {
	buf->put16 (b);
    } else {
	CORBA::Octet b2[16];
	swap_16 (b2, b);
	buf->put16 (b2);
    }
#endif
}

void
MICO::CDREncoder::put_char (CORBA::Char c)
{
  if (!conv) {
    buf->put1 (&c);
    return;
  }
  conv->put_char (*this, c);
}

void
MICO::CDREncoder::put_char_raw (CORBA::Char c)
{
    buf->put1 (&c);
}

void
MICO::CDREncoder::put_wchar (CORBA::WChar c)
{
  if (!conv) {
    buf->put (&c, sizeof (CORBA::WChar));
    return;
  }
  conv->put_wchar (*this, c);
}

void
MICO::CDREncoder::put_octet (CORBA::Octet o)
{
    buf->put1 (&o);
}

void
MICO::CDREncoder::put_boolean (CORBA::Boolean b)
{
    buf->put1 (&b);
}

void
MICO::CDREncoder::put_shorts (const CORBA::Short *p, CORBA::ULong l)
{
    buf->walign (2);
    if (mach_bo == data_bo) {
	buf->put (p, 2*l);
    } else {
	buf->resize (2*l);
	CORBA::Short *d = (CORBA::Short *)buf->wdata();
	for (CORBA::Long i = l; --i >= 0; ++d, ++p)
	    swap2 (d, p);
	buf->wseek_rel (2*l);
    }
}

void
MICO::CDREncoder::put_ushorts (const CORBA::UShort *p, CORBA::ULong l)
{
    buf->walign (2);
    if (mach_bo == data_bo) {
	buf->put (p, 2*l);
    } else {
	buf->resize (2*l);
	CORBA::UShort *d = (CORBA::UShort *)buf->wdata();
	for (CORBA::Long i = l; --i >= 0; ++d, ++p)
	    swap2 (d, p);
	buf->wseek_rel (2*l);
    }
}

void
MICO::CDREncoder::put_longs (const CORBA::Long *p, CORBA::ULong l)
{
    buf->walign (4);
    if (mach_bo == data_bo) {
	buf->put (p, 4*l);
    } else {
	buf->resize (4*l);
	CORBA::Long *d = (CORBA::Long *)buf->wdata();
	for (CORBA::Long i = l; --i >= 0; ++d, ++p)
	    swap4 (d, p);
	buf->wseek_rel (4*l);
    }
}

void
MICO::CDREncoder::put_longlongs (const CORBA::LongLong *p, CORBA::ULong l)
{
    buf->walign (8);
    if (mach_bo == data_bo) {
	buf->put (p, 8*l);
    } else {
	buf->resize (8*l);
	CORBA::LongLong *d = (CORBA::LongLong *)buf->wdata();
	for (CORBA::Long i = l; --i >= 0; ++d, ++p)
	    swap8 (d, p);
	buf->wseek_rel (8*l);
    }
}

void
MICO::CDREncoder::put_ulongs (const CORBA::ULong *p, CORBA::ULong l)
{
    buf->walign (4);
    if (mach_bo == data_bo) {
	buf->put (p, 4*l);
    } else {
	buf->resize (4*l);
	CORBA::ULong *d = (CORBA::ULong *)buf->wdata();
	for (CORBA::Long i = l; --i >= 0; ++d, ++p)
	    swap4 (d, p);
	buf->wseek_rel (4*l);
    }
}

void
MICO::CDREncoder::put_ulonglongs (const CORBA::ULongLong *p, CORBA::ULong l)
{
    buf->walign (8);
    if (mach_bo == data_bo) {
	buf->put (p, 8*l);
    } else {
	buf->resize (8*l);
	CORBA::ULongLong *d = (CORBA::ULongLong *)buf->wdata();
	for (CORBA::Long i = l; --i >= 0; ++d, ++p)
	    swap8 (d, p);
	buf->wseek_rel (8*l);
    }
}

void
MICO::CDREncoder::put_floats (const CORBA::Float *p, CORBA::ULong l)
{
    for (CORBA::Long i = l; --i >= 0; ++p)
	put_float (*p);
}

void
MICO::CDREncoder::put_doubles (const CORBA::Double *p, CORBA::ULong l)
{
    for (CORBA::Long i = l; --i >= 0; ++p)
	put_double (*p);
}

void
MICO::CDREncoder::put_longdoubles (const CORBA::LongDouble *p, CORBA::ULong l)
{
    for (CORBA::Long i = l; --i >= 0; ++p)
	put_longdouble (*p);
}

void
MICO::CDREncoder::put_chars (const CORBA::Char *p, CORBA::ULong l)
{
  if (!conv) {
    buf->put (p, l);
    return;
  }
  conv->put_chars (*this, p, l);
}

void
MICO::CDREncoder::put_chars_raw (const CORBA::Char *p, CORBA::ULong l)
{
    buf->put (p, l);
}

void
MICO::CDREncoder::put_wchars (const CORBA::WChar *p, CORBA::ULong l)
{
  if (!conv) {
    buf->put (p, l * sizeof (CORBA::WChar));
    return;
  }
  conv->put_wchars (*this, p, l);
}

void
MICO::CDREncoder::put_booleans (const CORBA::Boolean *p, CORBA::ULong l)
{
    buf->put (p, l);
}

void
MICO::CDREncoder::put_string (const char *s)
{
  if (!conv) {
    CORBA::ULong len = strlen (s) + 1;
    put_ulong (len);
    put_octets (s, len);
    return;
  }
  
  conv->put_string (*this, s, 0);
}

void
MICO::CDREncoder::put_string_raw (const char *s)
{
    CORBA::ULong len = strlen (s) + 1;
    put_ulong (len);
    put_octets (s, len);
}

void
MICO::CDREncoder::put_wstring (const wchar_t *s)
{
    if (!conv) {
      CORBA::ULong len = xwcslen (s) + 1;
      put_ulong (len);
      while (len--) {
	put_ushort ((CORBA::UShort) *s++);
      }
      return;
    }

    conv->put_wstring (*this, s, 0);
}

void
MICO::CDREncoder::put_fixed (const FixedBase::FixedValue &value,
			     CORBA::UShort digits,
			     CORBA::Short scale)
{
    assert (value.length() == (CORBA::ULong)digits+1);

    int s = 0, n = value.length();

    if (n == 1) {
	put_octet ((CORBA::Octet)value[0] ? 0xd : 0xc);
	return;
    }
    if ((n & 1)) {
	put_octet ((CORBA::Octet)value[0]);
	s = 1;
    }
    for (int i = s; i < n-2; i+=2)
	put_octet (((CORBA::Octet)value[i] << 4) | (CORBA::Octet)value[i+1]);
    put_octet (((CORBA::Octet)value[n-2] << 4) |
	       ((CORBA::Octet)value[n-1] ? 0xd : 0xc));
}

CORBA::DataEncoder *
MICO::CDREncoder::clone () const
{
    return new CDREncoder (new CORBA::Buffer (*buf), TRUE, data_bo,
			   (conv ? conv->clone() : 0), TRUE,
			   (vstate ? new ValueState (*vstate) : 0), TRUE);
}

CORBA::DataEncoder *
MICO::CDREncoder::clone (CORBA::Buffer *b, CORBA::Boolean dofree_b,
                         CORBA::CodeSetCoder *c, CORBA::Boolean dofree_c,
			 ValueState *vs, CORBA::Boolean dofree_vs) const
{
    return new CDREncoder (b, dofree_b, data_bo, c, dofree_c,
			   vs, dofree_vs);
}

CORBA::DataDecoder *
MICO::CDREncoder::decoder () const
{
    return new CDRDecoder (new CORBA::Buffer (*buf), TRUE, data_bo,
			   (conv ? conv->clone() : 0), TRUE,
			   0, TRUE);
}

CORBA::DataDecoder *
MICO::CDREncoder::decoder (CORBA::Buffer *b, CORBA::Boolean dofree_b,
			   CORBA::CodeSetCoder *c, CORBA::Boolean dofree_c
			   ) const
{
    return new CDRDecoder (b, dofree_b, data_bo, c, dofree_c, 0, 1);
}

const char *
MICO::CDREncoder::type () const
{
    return "cdr";
}

CORBA::ByteOrder
MICO::CDREncoder::byteorder () const
{
    return data_bo;
}

void
MICO::CDREncoder::byteorder (CORBA::ByteOrder _bo)
{
    data_bo = _bo;
}

CORBA::ULong
MICO::CDREncoder::max_alignment () const
{
    return 8;
}


/**************************** CDRDecoder ******************************/


MICO::CDRDecoder::CDRDecoder ()
    : CORBA::DataDecoder (new CORBA::Buffer, TRUE)
{
#ifdef HAVE_BYTEORDER_BE
    mach_bo = CORBA::BigEndian;
#else
    mach_bo = CORBA::LittleEndian;
#endif
    data_bo = mach_bo;
}

MICO::CDRDecoder::CDRDecoder (CORBA::Buffer *b, CORBA::Boolean dofree_b,
			      CORBA::ByteOrder _bo,
                              CORBA::CodeSetCoder *c, CORBA::Boolean dofree_c,
			      ValueState *vs, CORBA::Boolean dofree_vs)
    : CORBA::DataDecoder (b, dofree_b, c, dofree_c,
			  vs, dofree_vs)
{
#ifdef HAVE_BYTEORDER_BE
    mach_bo = CORBA::BigEndian;
#else
    mach_bo = CORBA::LittleEndian;
#endif
    data_bo = (_bo == CORBA::DefaultEndian) ? mach_bo : _bo;
}

MICO::CDRDecoder::~CDRDecoder ()
{
}

CORBA::Boolean
MICO::CDRDecoder::get_short (CORBA::Short &s)
{
  if (!buf->ralign (2) || !check_chunk ()) {
    return FALSE;
  }

    if (data_bo == mach_bo)
	return buf->get2 (&s);

    CORBA::Short s2;
    if (!buf->get2 (&s2))
	return FALSE;
    swap2 (&s, &s2);
    return TRUE;
}

CORBA::Boolean
MICO::CDRDecoder::get_ushort (CORBA::UShort &us)
{
  if (!buf->ralign (2) || !check_chunk ()) {
    return FALSE;
  }

    if (data_bo == mach_bo)
	return buf->get2 (&us);

    CORBA::UShort us2;
    if (!buf->get2 (&us2))
	return FALSE;
    swap2 (&us, &us2);
    return TRUE;
}

CORBA::Boolean
MICO::CDRDecoder::get_long (CORBA::Long &l)
{
  if (!buf->ralign (4) || !check_chunk ()) {
    return FALSE;
  }

    if (data_bo == mach_bo)
	return buf->get4 (&l);

    CORBA::Long l2;
    if (!buf->get4 (&l2))
	return FALSE;
    swap4 (&l, &l2);
    return TRUE;
}

CORBA::Boolean
MICO::CDRDecoder::get_longlong (CORBA::LongLong &l)
{
  if (!buf->ralign (8) || !check_chunk ()) {
    return FALSE;
  }

    if (data_bo == mach_bo)
	return buf->get8 (&l);

    CORBA::LongLong l2;
    if (!buf->get8 (&l2))
	return FALSE;
    swap8 (&l, &l2);
    return TRUE;
}

CORBA::Boolean
MICO::CDRDecoder::get_ulong (CORBA::ULong &ul)
{
  if (!buf->ralign (4) || !check_chunk ()) {
    return FALSE;
  }

    if (data_bo == mach_bo)
	return buf->get4 (&ul);

    CORBA::ULong ul2;
    if (!buf->get4 (&ul2))
	return FALSE;
    swap4 (&ul, &ul2);
    return TRUE;
}

CORBA::Boolean
MICO::CDRDecoder::get_ulonglong (CORBA::ULongLong &ul)
{
  if (!buf->ralign (8) || !check_chunk ()) {
    return FALSE;
  }

    if (data_bo == mach_bo)
	return buf->get8 (&ul);

    CORBA::ULongLong ul2;
    if (!buf->get8 (&ul2))
	return FALSE;
    swap8 (&ul, &ul2);
    return TRUE;
}

CORBA::Boolean
MICO::CDRDecoder::get_float (CORBA::Float &f)
{
  if (!buf->ralign (4) || !check_chunk ()) {
    return FALSE;
  }

#ifdef HAVE_IEEE_FP
    if (data_bo == mach_bo)
	return buf->get4 (&f);

    CORBA::Float f2;
    if (!buf->get4 (&f2))
	return FALSE;
    swap4 (&f, &f2);
#else
    CORBA::Octet b[4];
    if (data_bo == mach_bo) {
	if (!buf->get4 (b))
	    return FALSE;
	mico_ieee2float (b, f);
	return TRUE;
    }

    CORBA::Octet b2[4];
    if (!buf->get4 (b2))
	return FALSE;
    swap4 (&b, &b2);
    mico_ieee2float (b, f);
#endif
    return TRUE;
}

CORBA::Boolean
MICO::CDRDecoder::get_double (CORBA::Double &d)
{
  if (!buf->ralign (8) || !check_chunk ()) {
    return FALSE;
  }

#ifdef HAVE_IEEE_FP
    if (data_bo == mach_bo)
	return buf->get8 (&d);

    CORBA::Double d2;
    if (!buf->get8 (&d2))
	return FALSE;
    swap8 (&d, &d2);
#else
    CORBA::Octet b[8];
    if (data_bo == mach_bo) {
	if (!buf->get8 (b))
	    return FALSE;
	mico_ieee2double (b, d);
	return TRUE;
    }

    CORBA::Octet b2[8];
    if (!buf->get8 (b2))
	return FALSE;
    swap8 (&b, &b2);
    mico_ieee2double (b, d);
#endif
    return TRUE;
}

CORBA::Boolean
MICO::CDRDecoder::get_longdouble (CORBA::LongDouble &d)
{
  if (!buf->ralign (8) || !check_chunk ()) {
    return FALSE;
  }

#if defined(HAVE_IEEE_FP) && SIZEOF_LONG_DOUBLE == 16
    if (data_bo == mach_bo)
	return buf->get16 (&d);

    CORBA::LongDouble d2;
    if (!buf->get16 (&d2))
	return FALSE;
    swap_16 (&d, &d2);
#else
    CORBA::Octet b[16];
    if (data_bo == mach_bo) {
	if (!buf->get16 (b))
	    return FALSE;
	mico_ieee2ldouble (b, d);
	return TRUE;
    }

    CORBA::Octet b2[16];
    if (!buf->get16 (b2))
	return FALSE;
    swap_16 (&b, &b2);
    mico_ieee2ldouble (b, d);
#endif
    return TRUE;
}

CORBA::Boolean
MICO::CDRDecoder::get_char (CORBA::Char &c)
{
  if (!conv) {
    return buf->get1 (&c);
  }

  return conv->get_char (*this, c);
}

CORBA::Boolean
MICO::CDRDecoder::get_char_raw (CORBA::Char &c)
{
    return buf->get1 (&c);
}

CORBA::Boolean
MICO::CDRDecoder::get_wchar (CORBA::WChar &c)
{
  if (!conv) {
    return buf->get (&c, sizeof (CORBA::WChar));
  }

  return conv->get_wchar (*this, c);
}

CORBA::Boolean
MICO::CDRDecoder::get_octet (CORBA::Octet &o)
{
    return buf->get1 (&o);
}

CORBA::Boolean
MICO::CDRDecoder::get_boolean (CORBA::Boolean &b)
{
    return buf->get1 (&b);
}

CORBA::Boolean
MICO::CDRDecoder::get_shorts (CORBA::Short *p, CORBA::ULong l)
{
    if (!buf->ralign (2) || !check_chunk())
	return FALSE;

    if (data_bo == mach_bo)
	return buf->get (p, 2*l);

    if (buf->length() < 2*l)
	return FALSE;

    CORBA::Short *s = (CORBA::Short *)buf->data();
    for (CORBA::Long i = l; --i >= 0; ++p, ++s)
	swap2 (p, s);
    buf->rseek_rel (2*l);

    return TRUE;
}

CORBA::Boolean
MICO::CDRDecoder::get_ushorts (CORBA::UShort *p, CORBA::ULong l)
{
    if (!buf->ralign (2) || !check_chunk ())
	return FALSE;

    if (data_bo == mach_bo)
	return buf->get (p, 2*l);

    if (buf->length() < 2*l)
	return FALSE;

    CORBA::UShort *s = (CORBA::UShort *)buf->data();
    for (CORBA::Long i = l; --i >= 0; ++p, ++s)
	swap2 (p, s);
    buf->rseek_rel (2*l);

    return TRUE;
}

CORBA::Boolean
MICO::CDRDecoder::get_longs (CORBA::Long *p, CORBA::ULong l)
{
    if (!buf->ralign (4) || !check_chunk ())
	return FALSE;

    if (data_bo == mach_bo)
	return buf->get (p, 4*l);

    if (buf->length() < 4*l)
	return FALSE;

    CORBA::Long *s = (CORBA::Long *)buf->data();
    for (CORBA::Long i = l; --i >= 0; ++p, ++s)
	swap4 (p, s);
    buf->rseek_rel (4*l);

    return TRUE;
}

CORBA::Boolean
MICO::CDRDecoder::get_longlongs (CORBA::LongLong *p, CORBA::ULong l)
{
    if (!buf->ralign (8) || !check_chunk ())
	return FALSE;

    if (data_bo == mach_bo)
	return buf->get (p, 8*l);

    if (buf->length() < 8*l)
	return FALSE;

    CORBA::LongLong *s = (CORBA::LongLong *)buf->data();
    for (CORBA::Long i = l; --i >= 0; ++p, ++s)
	swap8 (p, s);
    buf->rseek_rel (8*l);

    return TRUE;
}

CORBA::Boolean
MICO::CDRDecoder::get_ulongs (CORBA::ULong *p, CORBA::ULong l)
{
    if (!buf->ralign (4) || !check_chunk ())
	return FALSE;

    if (data_bo == mach_bo)
	return buf->get (p, 4*l);

    if (buf->length() < 4*l)
	return FALSE;

    CORBA::ULong *s = (CORBA::ULong *)buf->data();
    for (CORBA::Long i = l; --i >= 0; ++p, ++s)
	swap4 (p, s);
    buf->rseek_rel (4*l);

    return TRUE;
}

CORBA::Boolean
MICO::CDRDecoder::get_ulonglongs (CORBA::ULongLong *p, CORBA::ULong l)
{
    if (!buf->ralign (8) || !check_chunk ())
	return FALSE;

    if (data_bo == mach_bo)
	return buf->get (p, 8*l);

    if (buf->length() < 8*l)
	return FALSE;

    CORBA::ULongLong *s = (CORBA::ULongLong *)buf->data();
    for (CORBA::Long i = l; --i >= 0; ++p, ++s)
	swap8 (p, s);
    buf->rseek_rel (8*l);

    return TRUE;
}

CORBA::Boolean
MICO::CDRDecoder::get_floats (CORBA::Float *p, CORBA::ULong l)
{
    for (CORBA::Long i = l; --i >= 0; ++p) {
	if (!get_float (*p))
	    return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
MICO::CDRDecoder::get_doubles (CORBA::Double *p, CORBA::ULong l)
{
    for (CORBA::Long i = l; --i >= 0; ++p) {
	if (!get_double (*p))
	    return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
MICO::CDRDecoder::get_longdoubles (CORBA::LongDouble *p, CORBA::ULong l)
{
    for (CORBA::Long i = l; --i >= 0; ++p) {
	if (!get_longdouble (*p))
	    return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
MICO::CDRDecoder::get_chars (CORBA::Char *p, CORBA::ULong l)
{
  if (!conv) {
    return buf->get (p, l);
  }
  return conv->get_chars (*this, p, l);
}

CORBA::Boolean
MICO::CDRDecoder::get_chars_raw (CORBA::Char *p, CORBA::ULong l)
{
    return buf->get (p, l);
}

CORBA::Boolean
MICO::CDRDecoder::get_wchars (CORBA::WChar *p, CORBA::ULong l)
{
  if (!conv) {
    return buf->get (p, l * sizeof (CORBA::WChar));
  }
  return conv->get_wchars (*this, p, l);
}

CORBA::Boolean
MICO::CDRDecoder::get_booleans (CORBA::Boolean *p, CORBA::ULong l)
{
    return buf->get (p, l);
}

CORBA::Boolean
MICO::CDRDecoder::get_string (CORBA::String_out s)
{
  if (!conv) {
    CORBA::ULong len;
    if (!get_ulong (len))
        return FALSE;
    if (len == 0 || len > buf->length())
      return FALSE;

    char * p = CORBA::string_alloc (len-1);
    if (!buf->get (p, len)) {
      CORBA::string_free (p);
      return FALSE;
    }
    s = p;
    return TRUE;
  }

  return conv->get_string (*this, s, 0);
}

CORBA::Boolean
MICO::CDRDecoder::get_string_raw (CORBA::String_out s)
{
    CORBA::ULong len;
    if (!get_ulong (len))
        return FALSE;
    if (len == 0 || len > buf->length())
      return FALSE;

    s = CORBA::string_alloc (len-1);
    if (!buf->get (s.ptr(), len)) {
	CORBA::string_free (s);
	return FALSE;
    }
    if (s[len-1] != 0) {
      CORBA::string_free (s);
      return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
MICO::CDRDecoder::get_wstring (CORBA::WString_out s)
{
  if (!conv) {
    CORBA::ULong len;
    if (!get_ulong (len))
        return FALSE;
    if (len == 0 || len*sizeof(CORBA::UShort) > buf->length())
      return FALSE;

    CORBA::WChar * p = CORBA::wstring_alloc (len-1);
    CORBA::UShort u;
    CORBA::ULong i;

    for (i=0; i<len; i++) {
      if (!get_ushort (u)) {
	CORBA::wstring_free (p);
	return FALSE;
      }
      p[i] = u;
    }

    if (p[len-1] != 0) {
      CORBA::wstring_free (p);
      return FALSE;
    }

    s = p;
    return TRUE;
  }

  return conv->get_wstring (*this, s, 0);
}

CORBA::Boolean
MICO::CDRDecoder::get_fixed (FixedBase::FixedValue &value,
			     CORBA::UShort digits,
			     CORBA::Short scale)
{
    value.length (digits+1);

    int s = 0, n = value.length();
    CORBA::Octet o;

    if (n == 1) {
	if (!get_octet (o))
	    return FALSE;
	value[0] = ((o & 0xf) == 0xd);
	return TRUE;
    }
    if ((n & 1)) {
	if (!get_octet (o))
	    return FALSE;
	value[0] = (o & 0xf);
	s = 1;
    }
    for (int i = s; i < n-2; i+=2) {
	if (!get_octet (o))
	    return FALSE;
	value[i] = ((o >> 4) & 0xf);
	value[i+1] = (o & 0xf);
    }
    if (!get_octet (o))
	return FALSE;
    value[n-2] = ((o >> 4) & 0xf);
    value[n-1] = ((o & 0xf) == 0xd);
    return TRUE;
}

CORBA::DataDecoder *
MICO::CDRDecoder::clone () const
{
    return new CDRDecoder (new CORBA::Buffer (*buf), TRUE, data_bo,
			   (conv ? conv->clone() : 0), TRUE,
			   (vstate ? new ValueState (*vstate) : 0), TRUE);
}

CORBA::DataDecoder *
MICO::CDRDecoder::clone (CORBA::Buffer *b, CORBA::Boolean dofree_b,
                         CORBA::CodeSetCoder *c, CORBA::Boolean dofree_c,
			 ValueState *vs, CORBA::Boolean dofree_vs) const
{
    return new CDRDecoder (b, dofree_b, data_bo, c, dofree_c, vs, dofree_vs);
}

CORBA::DataEncoder *
MICO::CDRDecoder::encoder () const
{
  return new CDREncoder (new CORBA::Buffer (*buf), TRUE, data_bo,
			 (conv ? conv->clone() : 0), TRUE,
			 0, TRUE);
}

CORBA::DataEncoder *
MICO::CDRDecoder::encoder (CORBA::Buffer *b, CORBA::Boolean dofree_b,
			   CORBA::CodeSetCoder *c, CORBA::Boolean dofree_c
			   ) const
{
    return new CDREncoder (b, dofree_b, data_bo, c, dofree_c, 0, 1);
}

const char *
MICO::CDRDecoder::type () const
{
    return "cdr";
}

CORBA::ByteOrder
MICO::CDRDecoder::byteorder () const
{
    return data_bo;
}

void
MICO::CDRDecoder::byteorder (CORBA::ByteOrder _bo)
{
    data_bo = _bo;
}

CORBA::ULong
MICO::CDRDecoder::max_alignment () const
{
    return 8;
}
