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

#ifndef __mico_sequence_h__
#define __mico_sequence_h__

template<class T> class TSeqVar;


/*
 * C++ template for unbounded sequences. The element type of the sequence
 * can be of any type except for arrays and recursive types (see
 * SequenceIndTmpl)
 */
template<class T, int TID>
class SequenceTmpl {
public:
    typedef T &ElementType; // Needed in TSeqVar (see var.h)
    typedef TSeqVar<SequenceTmpl<T,TID> > _var_type;
private:
    std::vector<T> vec;
public:
    SequenceTmpl () {}
    SequenceTmpl (MICO_ULong maxval)
    {
	vec.reserve (maxval);
    }
    SequenceTmpl (MICO_ULong max, MICO_ULong length, T *value,
		  MICO_Boolean rel = FALSE);

    SequenceTmpl (const SequenceTmpl<T,TID> &s)
    {
	vec = s.vec;
    }
    
    ~SequenceTmpl ()
    {
    }
    
    void replace (MICO_ULong max, MICO_ULong length, T *value,
		  MICO_Boolean rel = FALSE);

    SequenceTmpl<T,TID> &operator= (const SequenceTmpl<T,TID> &s)
    {
	vec = s.vec;
	return *this;
    }

    MICO_ULong maximum () const
    {
	return vec.capacity ();
    }

    MICO_Boolean release () const
    {
	// we always own the buffer
	return TRUE;
    }

    T* get_buffer (MICO_Boolean orphan = FALSE);

    const T* get_buffer () const
    {
	assert (vec.size() > 0);
	return &vec[0];
    }

    void length (MICO_ULong l);

    MICO_ULong length () const;
    T &operator[] (MICO_ULong idx);
    const T &operator[] (MICO_ULong idx) const;

    static T *allocbuf (MICO_ULong len)
    {
	return new T[len];
    }

    static void freebuf (T *b)
    {
	delete[] b;
    }

#if defined( __SUNPRO_CC )
    friend MICO_Boolean
    operator== (const SequenceTmpl<T,TID> &v1, const SequenceTmpl<T,TID> &v2)
    {
	if (v1.length() != v2.length())
	    return FALSE;
	for (MICO_ULong _i = 0; _i < v1.length(); ++_i) {
	    if (!(v1[_i] == v2[_i]))
		return FALSE;
	}
	return TRUE;
    }
#endif
};

template<class T, int TID>
SequenceTmpl<T,TID>::SequenceTmpl (MICO_ULong maxval, MICO_ULong lengthval, T *value,
				   MICO_Boolean rel)
{
    assert (lengthval <= maxval);
    vec.reserve (maxval);
    vec.insert (vec.begin(), value, value+lengthval);
    if (rel)
	freebuf (value);
}

template<class T, int TID>
void
SequenceTmpl<T,TID>::replace (MICO_ULong maxval, MICO_ULong lengthval, T *value,
			      MICO_Boolean rel)
{
    assert (lengthval <= maxval);
    vec.erase (vec.begin(), vec.end());
    vec.reserve (maxval);
    vec.insert (vec.begin(), value, value+lengthval);
    if (rel)
	freebuf (value);
}

template<class T, int TID>
void
SequenceTmpl<T,TID>::length (MICO_ULong l)
{
    if (l < vec.size ()) {
	vec.erase (vec.begin() + l, vec.end());
    } else if (l > vec.size()) {
	T* t = new T;
	// the (long) cast is needed for SGI STL
	vec.insert (vec.end(), long(l - vec.size()), *t);
	delete t;
    }
}

template<class T, int TID>
inline MICO_ULong
SequenceTmpl<T,TID>::length () const
{
    return vec.size ();
}

template<class T, int TID>
inline T &
SequenceTmpl<T,TID>::operator[] (MICO_ULong idx)
{
    return vec[idx];
}
    

template<class T, int TID>
inline const T &
SequenceTmpl<T,TID>::operator[] (MICO_ULong idx) const
{
    return vec[idx];
}

template<class T, int TID>
T *
SequenceTmpl<T,TID>::get_buffer (MICO_Boolean orphan)
{
    if (orphan) {
	T *b = allocbuf (vec.capacity());
	for (mico_vec_size_type i = 0; i < vec.size(); ++i)
	    b[i] = vec[i];
	vec.erase (vec.begin(), vec.end());
	return b;
    } else {
	assert (vec.size() > 0);
	return &vec[0];
    }
}

#if !defined( __SUNPRO_CC )
template<class T, int TID>
MICO_Boolean
operator== (const SequenceTmpl<T,TID> &v1, const SequenceTmpl<T,TID> &v2)
{
    if (v1.length() != v2.length())
	return FALSE;
    for (MICO_ULong _i = 0; _i < v1.length(); ++_i) {
	if (!(v1[_i] == v2[_i]))
	    return FALSE;
    }
    return TRUE;
}
#endif

/*
 * Same as SequenceTmpl except that this template is for bounded sequences.
 */
template<class T, int TID, int max>
class BoundedSequenceTmpl {
public:
    typedef T &ElementType; // Needed in TSeqVar (see var.h)
private:
    std::vector<T> vec;
public:
    BoundedSequenceTmpl ()
    {
	vec.reserve (max);
    }
    BoundedSequenceTmpl (MICO_ULong length, T *value,
			 MICO_Boolean rel = TRUE);
    
    BoundedSequenceTmpl (const BoundedSequenceTmpl<T, TID, max> &s)
    {
	vec = s.vec;
    }

    ~BoundedSequenceTmpl ()
    {
    }

    void replace (MICO_ULong length, T *value,
		  MICO_Boolean rel = TRUE);

    BoundedSequenceTmpl<T, TID, max> &operator=
	(const BoundedSequenceTmpl<T, TID, max> &s)
    {
	vec = s.vec;
	return *this;
    }

    MICO_ULong maximum () const
    {
	return max;
    }

    MICO_Boolean release () const
    {
	// we always own the buffer
	return TRUE;
    }

    T* get_buffer (MICO_Boolean orphan = FALSE);

    const T* get_buffer () const
    {
	assert (vec.size() > 0);
	return &vec[0];
    }

    void length (MICO_ULong l);

    MICO_ULong length () const
    {
	return vec.size ();
    }

    T &operator[] (MICO_ULong idx)
    {
	return vec[idx];
    }

    const T &operator[] (MICO_ULong idx) const
    {
	return vec[idx];
    }

    static T *allocbuf (MICO_ULong len)
    {
	return new T[len];
    }

    static void freebuf (T *b)
    {
	delete[] b;
    }

#if defined( __SUNPRO_CC )
    friend MICO_Boolean
    operator== (const BoundedSequenceTmpl<T,TID,max> &v1,
		const BoundedSequenceTmpl<T,TID,max> &v2)
    {
	if (v1.length() != v2.length())
	    return FALSE;
	for (MICO_ULong _i = 0; _i < v1.length(); ++_i) {
	    if (!(v1[_i] == v2[_i]))
		return FALSE;
	}
	return TRUE;
    }
#endif
};

template<class T, int TID, int max>
BoundedSequenceTmpl<T,TID,max>::BoundedSequenceTmpl (MICO_ULong lengthval,
						     T *value,
						     MICO_Boolean rel)
{
    assert (lengthval <= max);
    vec.reserve (max);
    vec.insert (vec.begin(), value, value+lengthval);
    if (rel)
	freebuf (value);
}

template<class T, int TID, int max>
void
BoundedSequenceTmpl<T,TID,max>::replace (MICO_ULong lengthval,
					 T *value, MICO_Boolean rel)
{
    assert (lengthval <= max);
    vec.erase (vec.begin(), vec.end());
    vec.reserve (max);
    vec.insert (vec.begin(), value, value+lengthval);
    if (rel)
	freebuf (value);
}

template<class T, int TID, int max>
void
BoundedSequenceTmpl<T,TID,max>::length (MICO_ULong l)
{
    assert (l <= max);
    if (l < vec.size ()) {
	vec.erase (vec.begin() + l, vec.end());
    } else if (l > vec.size()) {
	T t;
	vec.insert (vec.end(), long(l - vec.size()), t);
    }
}


template<class T, int TID, int max>
T *
BoundedSequenceTmpl<T,TID,max>::get_buffer (MICO_Boolean orphan)
{
    if (orphan) {
	T *b = allocbuf (vec.capacity());
	for (mico_vec_size_type i = 0; i < vec.size(); ++i)
	    b[i] = vec[i];
	vec.erase (vec.begin(), vec.end());
	return b;
    } else {
	assert (vec.size() > 0);
	return &vec[0];
    }
}

#if !defined( __SUNPRO_CC )
template<class T, int TID, int max>
MICO_Boolean
operator== (const BoundedSequenceTmpl<T,TID,max> &v1,
	    const BoundedSequenceTmpl<T,TID,max> &v2)
{
    if (v1.length() != v2.length())
	return FALSE;
    for (MICO_ULong _i = 0; _i < v1.length(); ++_i) {
	if (!(v1[_i] == v2[_i]))
	    return FALSE;
    }
    return TRUE;
}
#endif

/*
 * C++ template for sequences of arrays and recursive types. The difference
 * is, that this template maintains it members only indirectly through
 * pointers. This is especially necessary to break infinite recursion
 * for recursive types. If this template is used for arrays, then T_elem
 * denotes the element type of the array, T the array type itself and
 * n the total number of array elements. If the template is used for
 * recursive types then T_elem and T are the same and n should be set to 1.
 */
template<class T_elem, class T, MICO_ULong n>
class SequenceIndTmpl {
public:
  typedef T &ElementType; // Needed in TSeqVar (see var.h)
private:
  std::vector<T_elem*> vec;
public:
  SequenceIndTmpl () {}
  SequenceIndTmpl (MICO_ULong maxval)
  {
    vec.reserve (maxval);
  }
  SequenceIndTmpl (MICO_ULong max, MICO_ULong length, T* value,
		   MICO_Boolean rel = TRUE);

  void replace (MICO_ULong max, MICO_ULong length, T *value,
		MICO_Boolean rel = TRUE);

  SequenceIndTmpl (const SequenceIndTmpl<T_elem,T,n> &s);
    
  ~SequenceIndTmpl ();
  
  SequenceIndTmpl<T_elem,T,n>& 
  operator= (const SequenceIndTmpl<T_elem,T,n> &s);
  
  MICO_ULong maximum () const
  {
    return vec.capacity ();
  }
  
  void length (MICO_ULong l);
  
  MICO_ULong length () const
  {
    return vec.size ();
  }

  MICO_Boolean release () const
  {
    // we always own the buffer
    return TRUE;
  }

  // get_buffer() not supported ...
  
  T& operator[] (MICO_ULong idx)
  {
    return (T&) *vec[idx];
  }
  
  const T& operator[] (MICO_ULong idx) const
  {
    return (T&) *vec[idx];
  }
  
  static T *allocbuf (MICO_ULong len);
  
  static void freebuf( T* b );
  
#if defined( __SUNPRO_CC )
  friend MICO_Boolean
  operator== (const SequenceIndTmpl<T_elem,T,n> &v1,
	      const SequenceIndTmpl<T_elem,T,n> &v2)
  {
    if( v1.length() != v2.length() )
      return FALSE;
    for( MICO_ULong i = 0; i < v1.length(); i++ ) {
      for( MICO_ULong j = 0; j < n; j++ ) {
	T_elem e1 = ((T_elem*) v1[ i ])[ j ];
	T_elem e2 = ((T_elem*) v2[ i ])[ j ];
	if( !(e1 == e2) )
	  return FALSE;
      }
    }
    return TRUE;
  }
#endif
};

#if !defined( __SUNPRO_CC )
template<class T_elem, class T, MICO_ULong n>
MICO_Boolean
operator== (const SequenceIndTmpl<T_elem,T,n> &v1,
	    const SequenceIndTmpl<T_elem,T,n> &v2)
{
  if( v1.length() != v2.length() )
    return FALSE;
  for( MICO_ULong i = 0; i < v1.length(); i++ ) {
    for( MICO_ULong j = 0; j < n; j++ ) {
      T_elem e1 = ((T_elem*) v1[ i ])[ j ];
      T_elem e2 = ((T_elem*) v2[ i ])[ j ];
      if( !(e1 == e2) )
	return FALSE;
    }
  }
  return TRUE;
}
#endif

/*
 * Same as SequenceIndTmpl except that this one is for bounded sequences.
 */
template<class T_elem, class T, MICO_ULong n, int max>
class BoundedSequenceIndTmpl {
public:
  typedef T &ElementType; // Needed in TSeqVar (see var.h)
private:
  std::vector<T_elem*> vec;
public:
  BoundedSequenceIndTmpl ()
  {
    vec.reserve (max);
  }
  BoundedSequenceIndTmpl (MICO_ULong length, T *value,
			  MICO_Boolean rel = TRUE);
  
  BoundedSequenceIndTmpl
  (const BoundedSequenceIndTmpl<T_elem,T,n,max> &s);
  
  ~BoundedSequenceIndTmpl ();
  
  BoundedSequenceIndTmpl<T_elem,T,n, max> &operator=
  (const BoundedSequenceIndTmpl<T_elem,T,n, max> &s);
  
  void replace (MICO_ULong length, T *value,
		MICO_Boolean rel = TRUE);

  MICO_ULong maximum () const
  {
    return max;
  }
  
  void length (MICO_ULong l);
  
  MICO_ULong length () const
  {
    return vec.size ();
  }
  
  MICO_Boolean release () const
  {
    // we always own the buffer
    return TRUE;
  }

  // get_buffer() not supported ...

  T &operator[] (MICO_ULong idx)
  {
    return (T&) *vec[idx];
  }
  
  const T &operator[] (MICO_ULong idx) const
  {
    return (T&) *vec[idx];
  }
  
  static T *allocbuf (MICO_ULong len);
  
  static void freebuf (T *b);

#if defined( __SUNPRO_CC )
  friend MICO_Boolean
  operator== (const BoundedSequenceIndTmpl<T_elem,T,n,max> &v1,
	      const BoundedSequenceIndTmpl<T_elem,T,n,max> &v2)
  {
    if( v1.length() != v2.length() )
      return FALSE;
    for( MICO_ULong i = 0; i < v1.length(); i++ ) {
      for( MICO_ULong j = 0; j < n; j++ ) {
	T_elem e1 = ((T_elem*) v1[ i ])[ j ];
	T_elem e2 = ((T_elem*) v2[ i ])[ j ];
	if( !(e1 == e2) )
	  return FALSE;
      }
    }
    return TRUE;
  }
#endif
};

#if !defined( __SUNPRO_CC )
template<class T_elem, class T, MICO_ULong n, int max>
MICO_Boolean
operator== (const BoundedSequenceIndTmpl<T_elem,T,n,max> &v1,
	    const BoundedSequenceIndTmpl<T_elem,T,n,max> &v2)
{
  if( v1.length() != v2.length() )
    return FALSE;
  for( MICO_ULong i = 0; i < v1.length(); i++ ) {
    for( MICO_ULong j = 0; j < n; j++ ) {
      T_elem e1 = ((T_elem*) v1[ i ])[ j ];
      T_elem e2 = ((T_elem*) v2[ i ])[ j ];
      if( !(e1 == e2) )
	return FALSE;
    }
  }
  return TRUE;
}
#endif

#endif // __mico_sequence_h__
