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

#ifndef __mico_mico_h__
#define __mico_mico_h__

namespace CORBA {
    
class Buffer {
    enum {
	MINSIZE = 128,
	RESIZE_THRESH = 10000,
	RESIZE_INCREMENT = 10000
    };
    Boolean _readonly;
    ULong _rptr, _wptr;
    ULong _ralignbase, _walignbase;
    ULong _len;
    Octet *_buf;

    Octet *alloc (ULong sz);
    Octet *realloc (Octet *, ULong osz, ULong nsz);
    void free (Octet *);
public:
    Buffer (void *);
    Buffer (ULong sz = 0);
    Buffer (const Buffer &);
    ~Buffer ();
    Buffer &operator= (const Buffer &);
    Boolean operator== (const Buffer &);

    void reset (ULong size = MINSIZE);
    void doresize (ULong needed);
    void resize (ULong needed)
    {
      if (_wptr + needed > _len)
	doresize (needed);
    }

    Boolean rseek_rel (Long offs)
    {
	if (_rptr + offs > _wptr)
            return FALSE;
	_rptr += offs;
	return TRUE;
    }
    Boolean rseek_beg (ULong offs)
    {
	if (offs > _wptr)
            return FALSE;
	_rptr = offs;
	return TRUE;
    }
    Boolean rseek_end (ULong offs)
    {
	if (offs > _len || _len - offs > _wptr)
            return FALSE;
	_rptr = _len - offs;
	return TRUE;
    }
    ULong ralign_base () const
    {
	return _ralignbase;
    }
    void ralign_base (ULong b)
    {
	_ralignbase = b;
    }
    ULong rpos () const
    {
	return _rptr;
    }

    Boolean ralign (ULong modulo)
    {
      assert (modulo >= 1);
      assert (_rptr >= _ralignbase);
      ULong r = modulo - ((_rptr - _ralignbase) % modulo);
      if (r != modulo) {
	_rptr += r;
	if (_rptr > _wptr) {
	  _rptr -= r;
	  return FALSE;
	}
      }
      return TRUE;
    }

    Boolean peek (void *, ULong blen);
    Boolean peek (Octet &);

    Boolean get (void *, ULong blen);
    Boolean get (Octet &);
    Boolean get1 (void *);
    Boolean get2 (void *);
    Boolean get4 (void *);
    Boolean get8 (void *);
    Boolean get16 (void *);

    void wseek_rel (Long offs)
    {
	assert (!_readonly);
	assert (_wptr + offs >= _rptr &&
		_wptr + offs <= _len);
	_wptr += offs;
    }
    void wseek_beg (ULong offs)
    {
	assert (!_readonly);
	assert (offs >= _rptr &&
		offs <= _len);
	_wptr = offs;
    }
    void wseek_end (ULong offs)
    {
	assert (!_readonly);
	assert (_len - offs >= _rptr);
	_wptr = _len - offs;
    }
    ULong walign_base () const
    {
	return _walignbase;
    }
    void walign_base (ULong b)
    {
	_walignbase = b;
    }
    ULong wpos () const
    {
	assert (!_readonly);
	return _wptr;
    }

    void walign (ULong modulo)
    {
      assert (!_readonly);
      assert (modulo >= 1);
      assert (_wptr >= _walignbase);
      ULong w = modulo - ((_wptr - _walignbase) % modulo);
      if (w != modulo) {
	resize (w);
	while (w--) {
	  _buf[_wptr++] = 0;
	}
      }
    }

    void replace (const void *, ULong blen);
    void replace (Octet);

    void put (const void *, ULong blen);
    void put (Octet);
    void put1 (const void *);
    void put2 (const void *);
    void put4 (const void *);
    void put8 (const void *);
    void put16 (const void *);

    ULong length () const
    {
	return _wptr - _rptr;
    }
    Octet *data ()
    {
	return &_buf[_rptr];
    }
    const Octet *data () const
    {
	return &_buf[_rptr];
    }
    Octet *wdata ()
    {
	return &_buf[_wptr];
    }
    const Octet *wdata () const
    {
	return &_buf[_wptr];
    }
    Octet *buffer ()
    {
	return _buf;
    }
    const Octet *buffer () const
    {
	return _buf;
    }

    void dump (const char *, std::ostream &) const;
};

typedef TVarVar<Buffer> Buffer_var;

}

#endif // __mico_mico_h__
