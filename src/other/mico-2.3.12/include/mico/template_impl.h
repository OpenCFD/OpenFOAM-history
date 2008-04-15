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

#ifndef __MICO_TEMPLATE_IMPL_H__
#define __MICO_TEMPLATE_IMPL_H__

/*
 * The following declarations belong to the ObjVar and
 * SequenceIndTmpl template.
 * The reason they are defined here is that these particular
 * methods make reference to interface methods (like _nil(),
 * _release(), etc) which are not known at the time these
 * templates are being used.
 */

//----- ObjVar implementations ---------------------------------------------

template<class T>
T *
ObjVar<T>::duplicate (T *t)
{
    return T::_duplicate (t);
}

template<class T>
void
ObjVar<T>::release (T *t)
{
    CORBA::release (t);
}

//----- ObjOut implementations ---------------------------------------------

template <class T>
ObjOut<T>::ObjOut (T*& p)
  : _ptr (p)
{
    _ptr = T::_nil();
}

template <class T>
ObjOut<T>::ObjOut (ObjVar<T>& p)
  : _ptr (p._ptr)
{
    CORBA::release (_ptr);
    _ptr = T::_nil();
}

template <class T>
ObjOut<T>&
ObjOut<T>::operator= (const ObjVar<T>& p)
{
    _ptr = T::_duplicate (p._ptr);
    return *this;
}


//----- ValueVar implementations -------------------------------------------

template<class T>
T *
ValueVar<T>::duplicate (T *t)
{
    if (t)
	t->_add_ref();
    return t;
}

template<class T>
void
ValueVar<T>::release (T *t)
{
    if (t)
	t->_remove_ref();
}

//----- SequenceIndTmpl implementations ------------------------------------

template<class T_elem, class T, MICO_ULong n>
SequenceIndTmpl<T_elem,T,n>::SequenceIndTmpl (MICO_ULong maxval,
					      MICO_ULong lengthval,
					      T* value,
					      MICO_Boolean rel)
{
  assert (lengthval <= maxval);
  vec.reserve (maxval);
  for (MICO_ULong i = 0; i < lengthval; ++i) {
    T_elem* new_elem = new T_elem[ n ];
    T_elem* dest = new_elem;
    T_elem* src = (T_elem*) (value + i);
    MICO_ULong j = 0;
    while( j < n ) {
      *dest = *src;
      ++dest;
      ++src;
      j++;
    }
    vec.push_back ( new_elem );
  }
  if (rel)
    freebuf (value);
}

template<class T_elem, class T, MICO_ULong n>
void
SequenceIndTmpl<T_elem,T,n>::replace (MICO_ULong maxval,
				      MICO_ULong lengthval,
				      T* value,
				      MICO_Boolean rel)
{
  assert (lengthval <= maxval);
  for( mico_vec_size_type i0 = 0; i0 < vec.size(); i0++ )
    delete[] vec[ i0 ];
  vec.erase (vec.begin(), vec.end());
  vec.reserve (maxval);
  for (MICO_ULong i1 = 0; i1 < lengthval; ++i1) {
    T_elem* new_elem = new T_elem[ n ];
    T_elem* dest = new_elem;
    T_elem* src = (T_elem*) (value + i1);
    MICO_ULong j = 0;
    while( j < n ) {
      *dest = *src;
      ++dest;
      ++src;
      j++;
    }
    vec.push_back ( new_elem );
  }
  if (rel)
    freebuf (value);
}

template<class T_elem, class T, MICO_ULong n>
SequenceIndTmpl<T_elem,T,n>::
SequenceIndTmpl (const SequenceIndTmpl<T_elem,T,n> &s)
{
  for( CORBA::ULong i = 0; i < s.length(); i++ ) {
    T_elem* new_elem = new T_elem[ n ];
    T_elem* dest = new_elem;
    T_elem* src = s.vec[ i ];
    MICO_ULong j = 0;
    while( j < n ) {
      *dest = *src;
      ++dest;
      ++src;
      j++;
    }
    vec.push_back ( new_elem );
  }
}

template<class T_elem, class T, MICO_ULong n>
SequenceIndTmpl<T_elem,T,n>::~SequenceIndTmpl ()
{
  for( mico_vec_size_type i = 0; i < vec.size(); i++ )
    delete[] vec[ i ];
}

template<class T_elem, class T, MICO_ULong n>
SequenceIndTmpl<T_elem,T,n>& 
SequenceIndTmpl<T_elem,T,n>::operator= (const SequenceIndTmpl<T_elem,T,n> &s)
{
  for( mico_vec_size_type i0 = 0; i0 < vec.size(); i0++ )
    delete[] vec[ i0 ];
  vec.erase( vec.begin(), vec.end() );
  for( MICO_ULong i1 = 0; i1 < s.length(); i1++ ) {
    T_elem* new_elem = new T_elem[ n ];
    T_elem* dest = new_elem;
    T_elem* src = s.vec[ i1 ];
    MICO_ULong j = 0;
    while( j < n ) {
      *dest = *src;
      ++dest;
      ++src;
      j++;
    }
    vec.push_back ( new_elem );
  }
  return *this;
}

template<class T_elem, class T, MICO_ULong n>
void
SequenceIndTmpl<T_elem,T,n>::length (MICO_ULong l)
{
  if (l < vec.size ()) {
    for( mico_vec_size_type i = l; i < vec.size(); i++ )
      delete[] vec[ i ];
    vec.erase (vec.begin() + l, vec.end());
  } else if (l > vec.size()) {
    int limit = l - vec.size();
    for( int i = 0; i < limit; i++ )
      vec.push_back( new T_elem[ n ] );
  }
}

template<class T_elem, class T, MICO_ULong n>
T*
SequenceIndTmpl<T_elem,T,n>::allocbuf (MICO_ULong len)
{
  return (T*) new T_elem[ n * len ];
}

template<class T_elem, class T, MICO_ULong n>
void
SequenceIndTmpl<T_elem,T,n>::freebuf( T* b )
{
  delete[] (T_elem*) b;
}


//----- BoundedSequenceIndTmpl implementations -----------------------------

template<class T_elem, class T, MICO_ULong n, int max>
BoundedSequenceIndTmpl<T_elem,T,n,max>::
BoundedSequenceIndTmpl (MICO_ULong lengthval, T *value,
			MICO_Boolean rel)
{
  assert (lengthval <= max);
  vec.reserve (max);
  for (MICO_ULong i = 0; i < lengthval; ++i) {
    T_elem* new_elem = new T_elem[ n ];
    T_elem* dest = new_elem;
    T_elem* src = (T_elem*) (value + i);
    MICO_ULong j = 0;
    while( j < n ) {
      *dest = *src;
      ++dest;
      ++src;
      j++;
    }
    vec.push_back ( new_elem );
  }
  if (rel)
    freebuf (value);
}

template<class T_elem, class T, MICO_ULong n, int max>
void
BoundedSequenceIndTmpl<T_elem,T,n,max>::replace (MICO_ULong lengthval, T *value,
						 MICO_Boolean rel)
{
  assert (lengthval <= max);
  for( mico_vec_size_type i0 = 0; i0 < vec.size(); i0++ )
    delete[] vec[ i0 ];
  vec.erase (vec.begin(), vec.end());
  vec.reserve (max);
  for (MICO_ULong i1 = 0; i1 < lengthval; ++i1) {
    T_elem* new_elem = new T_elem[ n ];
    T_elem* dest = new_elem;
    T_elem* src = (T_elem*) (value + i1);
    MICO_ULong j = 0;
    while( j < n ) {
      *dest = *src;
      ++dest;
      ++src;
      j++;
    }
    vec.push_back ( new_elem );
  }
  if (rel)
    freebuf (value);
}

template<class T_elem, class T, MICO_ULong n, int max>
BoundedSequenceIndTmpl<T_elem,T,n,max>::
BoundedSequenceIndTmpl (const BoundedSequenceIndTmpl<T_elem,T,n,max> &s)
{
  vec.reserve( max );
  for( CORBA::ULong i = 0; i < s.length(); i++ ) {
    T_elem* new_elem = new T_elem[ n ];
    T_elem* dest = new_elem;
    T_elem* src = s.vec[ i ];
    MICO_ULong j = 0;
    while( j < n ) {
      *dest = *src;
      ++dest;
      ++src;
      j++;
    }
    vec.push_back ( new_elem );
  }
}

template<class T_elem, class T, MICO_ULong n, int max>
BoundedSequenceIndTmpl<T_elem,T,n,max>::~BoundedSequenceIndTmpl ()
{
  for( mico_vec_size_type i = 0; i < vec.size(); i++ )
    delete[] vec[ i ];
}

template<class T_elem, class T, MICO_ULong n, int max>
BoundedSequenceIndTmpl<T_elem,T,n, max> &
BoundedSequenceIndTmpl<T_elem,T,n,max>::
operator= (const BoundedSequenceIndTmpl<T_elem,T,n, max> &s)
{
  for( mico_vec_size_type i0 = 0; i0 < vec.size(); i0++ )
    delete[] vec[ i0 ];
  vec.erase( vec.begin(), vec.end() );
  for( MICO_ULong i1 = 0; i1 < s.length(); i1++ ) {
    T_elem* new_elem = new T_elem[ n ];
    T_elem* dest = new_elem;
    T_elem* src = s.vec[ i1 ];
    MICO_ULong j = 0;
    while( j < n ) {
      *dest = *src;
      ++dest;
      ++src;
      j++;
    }
    vec.push_back ( new_elem );
  }
  return *this;
}

template<class T_elem, class T, MICO_ULong n, int max>
void
BoundedSequenceIndTmpl<T_elem,T,n,max>::length (MICO_ULong l)
{
  assert (l <= max);
  if (l < vec.size ()) {
    for( mico_vec_size_type i = l; i < vec.size(); i++ )
      delete[] vec[ i ];
    vec.erase (vec.begin() + l, vec.end());
  } else if (l > vec.size()) {
    int limit = l - vec.size();
    for( int i = 0; i < limit; i++ )
      vec.push_back( new T_elem[ n ] );
  }
}

template<class T_elem, class T, MICO_ULong n, int max>
T*
BoundedSequenceIndTmpl<T_elem,T,n,max>::allocbuf (MICO_ULong len)
{
  return (T*) new T_elem[ n * len ];
}

template<class T_elem, class T, MICO_ULong n, int max>
void
BoundedSequenceIndTmpl<T_elem,T,n,max>::freebuf (T *b)
{
  delete[] (T_elem*) b;
}

#endif

