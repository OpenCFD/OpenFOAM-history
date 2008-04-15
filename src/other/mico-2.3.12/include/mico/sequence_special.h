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


#ifndef __MICO_SEQUENCE_SPECIAL_H__
#define __MICO_SEQUENCE_SPECIAL_H__


/*
 * This file hosts special sequence definitions (for sequence of
 * strings or sequences of interfaces) whose mapping differs slightly
 * from that of ordinary sequences (the "T* data" constructor). The
 * template parameter T must be CORBA::String_var. The reason we can't
 * explicitely write this here is because CORBA::String_var is not
 * known at this time.
 */




//---Template for unbounded sequence of strings--------------------------

template<class T>
class StringSequenceTmpl {
public:
    typedef T &ElementType; // Needed in TSeqVar (see var.h)
    typedef TSeqVar<StringSequenceTmpl<T> > _var_type;
private:
    std::vector<T> vec;
public:
    StringSequenceTmpl () {}
    StringSequenceTmpl (MICO_ULong maxval)
    {
	vec.reserve (maxval);
    }
    StringSequenceTmpl (MICO_ULong max, MICO_ULong length, char** value,
			MICO_Boolean rel = FALSE);

    StringSequenceTmpl (const StringSequenceTmpl<T> &s)
    {
	vec = s.vec;
    }
    
    ~StringSequenceTmpl ()
    {
    }
    
    void replace (MICO_ULong max, MICO_ULong length, char** value,
		  MICO_Boolean rel = FALSE);

    StringSequenceTmpl<T> &operator= (const StringSequenceTmpl<T> &s)
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

    char **get_buffer (MICO_Boolean orphan = FALSE)
    {
	// XXX
	assert (0);
        return 0;
    }

    const char **get_buffer () const
    {
	// XXX
	assert (0);
        return 0;
    }

    void length (MICO_ULong l);

    MICO_ULong length () const;
    T& operator[] (MICO_ULong idx);
    const T &operator[] (MICO_ULong idx) const;

    static char **allocbuf (MICO_ULong len)
    {
	return new char*[len];
    }

    static void freebuf (char **b)
    {
	delete[] b;
    }

#if defined( __SUNPRO_CC )
    friend MICO_Boolean
    operator== (const StringSequenceTmpl<T> &v1,
		const StringSequenceTmpl<T> &v2)
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

template<class T>
StringSequenceTmpl<T>::StringSequenceTmpl (MICO_ULong maxval,
					   MICO_ULong lengthval,
					   char** value,
					   MICO_Boolean rel)
{
    assert (lengthval <= maxval);
    vec.reserve (maxval);
    for( MICO_ULong i = 0; i < lengthval; i++ ) {
	if (rel) {
	    vec.push_back( value[i] );
	} else {
	    vec.push_back( (const char *)value[i] );
	}
    }
}

template<class T>
void
StringSequenceTmpl<T>::replace (MICO_ULong maxval, MICO_ULong lengthval,
				char** value, MICO_Boolean rel)
{
    assert (lengthval <= maxval);
    vec.erase (vec.begin(), vec.end());
    vec.reserve (maxval);
    for( MICO_ULong i = 0; i < lengthval; i++ ) {
	if (rel) {
	    vec.push_back( value[i] );
	} else {
	    vec.push_back( (const char *)value[i] );
	}
    }
}

template<class T>
void
StringSequenceTmpl<T>::length (MICO_ULong l)
{
    if (l < vec.size ()) {
	vec.erase (vec.begin() + l, vec.end());
    } else if (l > vec.size()) {
	T t;
	// the (long) cast is needed for SGI STL
	vec.insert (vec.end(), long(l - vec.size()), t);
    }
}

template<class T>
inline MICO_ULong
StringSequenceTmpl<T>::length () const
{
    return vec.size ();
}

template<class T>
inline T&
StringSequenceTmpl<T>::operator[] (MICO_ULong idx)
{
    return vec[idx];
}
    

template<class T>
inline const T&
StringSequenceTmpl<T>::operator[] (MICO_ULong idx) const
{
    return vec[idx];
}

#if !defined( __SUNPRO_CC )
template<class T>
MICO_Boolean
operator== (const StringSequenceTmpl<T> &v1, const StringSequenceTmpl<T> &v2)
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



//---Template for bounded sequence of strings----------------------------

template<class T, int max>
class BoundedStringSequenceTmpl {
public:
    typedef T &ElementType; // Needed in TSeqVar (see var.h)
private:
    std::vector<T> vec;
public:
    BoundedStringSequenceTmpl ()
    {
	vec.reserve (max);
    }
    BoundedStringSequenceTmpl (MICO_ULong length, char** value,
			       MICO_Boolean rel = TRUE);
    
    BoundedStringSequenceTmpl (const BoundedStringSequenceTmpl<T,max> &s)
    {
	vec = s.vec;
    }

    ~BoundedStringSequenceTmpl ()
    {
    }

    void replace (MICO_ULong length, char** value,
		  MICO_Boolean rel = TRUE);

    BoundedStringSequenceTmpl<T,max> &operator=
	(const BoundedStringSequenceTmpl<T,max> &s)
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

    char **get_buffer (MICO_Boolean orphan = FALSE)
    {
	// XXX
	assert (0);
        return 0;
    }

    const char **get_buffer () const
    {
	// XXX
	assert (0);
        return 0;
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

    static char **allocbuf (MICO_ULong len)
    {
	return new char*[len];
    }

    static void freebuf (char **b)
    {
	delete[] b;
    }

#if defined( __SUNPRO_CC )
    friend MICO_Boolean
    operator== (const BoundedStringSequenceTmpl<T,max> &v1,
		const BoundedStringSequenceTmpl<T,max> &v2)
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

template<class T, int max>
BoundedStringSequenceTmpl<T,max>::BoundedStringSequenceTmpl (MICO_ULong lengthval,
							     char** value,
							     MICO_Boolean rel)
{
    assert (lengthval <= max);
    vec.reserve (max);
    for( MICO_ULong i = 0; i < lengthval; i++ ) {
	if (rel) {
	    vec.push_back( value[i] );
	} else {
	    vec.push_back( (const char *)value[i] );
	}
    }
}

template<class T, int max>
void
BoundedStringSequenceTmpl<T,max>::replace (MICO_ULong lengthval,
					   char** value,
					   MICO_Boolean rel)
{
    assert (lengthval <= max);
    vec.erase (vec.begin(), vec.end());
    vec.reserve (max);
    for( MICO_ULong i = 0; i < lengthval; i++ ) {
	if (rel) {
	    vec.push_back( value[i] );
	} else {
	    vec.push_back( (const char *)value[i] );
	}
    }
}

template<class T, int max>
void
BoundedStringSequenceTmpl<T,max>::length (MICO_ULong l)
{
    assert (l <= max);
    if (l < vec.size ()) {
	vec.erase (vec.begin() + l, vec.end());
    } else if (l > vec.size()) {
	T t;
	vec.insert (vec.end(), long(l - vec.size()), t);
    }
}

#if !defined( __SUNPRO_CC )
template<class T, int max>
MICO_Boolean
operator== (const BoundedStringSequenceTmpl<T,max> &v1,
	    const BoundedStringSequenceTmpl<T,max> &v2)
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


//---Template for unbounded sequence of wstrings--------------------------

template<class T>
class WStringSequenceTmpl {
public:
    typedef T &ElementType; // Needed in TSeqVar (see var.h)
    typedef TSeqVar<WStringSequenceTmpl<T> > _var_type;
private:
    std::vector<T> vec;
public:
    WStringSequenceTmpl () {}
    WStringSequenceTmpl (MICO_ULong maxval)
    {
	vec.reserve (maxval);
    }
    WStringSequenceTmpl (MICO_ULong max, MICO_ULong length, wchar_t** value,
			 MICO_Boolean rel = FALSE);

    WStringSequenceTmpl (const WStringSequenceTmpl<T> &s)
    {
	vec = s.vec;
    }
    
    ~WStringSequenceTmpl ()
    {
    }
    
    void replace (MICO_ULong max, MICO_ULong length, wchar_t** value,
		  MICO_Boolean rel = FALSE);

    WStringSequenceTmpl<T> &operator= (const WStringSequenceTmpl<T> &s)
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

    wchar_t **get_buffer (MICO_Boolean orphan = FALSE)
    {
	// XXX
	assert (0);
        return 0;
    }

    const wchar_t **get_buffer () const
    {
	// XXX
	assert (0);
        return 0;
    }

    void length (MICO_ULong l);

    MICO_ULong length () const;
    T& operator[] (MICO_ULong idx);
    const T &operator[] (MICO_ULong idx) const;

    static wchar_t **allocbuf (MICO_ULong len)
    {
	return new wchar_t*[len];
    }

    static void freebuf (wchar_t **b)
    {
	delete[] b;
    }

#if defined( __SUNPRO_CC )
    friend MICO_Boolean
    operator== (const WStringSequenceTmpl<T> &v1,
		const WStringSequenceTmpl<T> &v2)
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

template<class T>
WStringSequenceTmpl<T>::WStringSequenceTmpl (MICO_ULong maxval,
					     MICO_ULong lengthval,
					     wchar_t** value,
					     MICO_Boolean rel)
{
    assert (lengthval <= maxval);
    vec.reserve (maxval);
    for( MICO_ULong i = 0; i < lengthval; i++ ) {
	if (rel) {
	    vec.push_back( value[i] );
	} else {
	    vec.push_back( (const wchar_t *)value[i] );
	}
    }
}

template<class T>
void
WStringSequenceTmpl<T>::replace (MICO_ULong maxval, MICO_ULong lengthval,
				 wchar_t** value, MICO_Boolean rel)
{
    assert (lengthval <= maxval);
    vec.erase (vec.begin(), vec.end());
    vec.reserve (maxval);
    for( MICO_ULong i = 0; i < lengthval; i++ ) {
	if (rel) {
	    vec.push_back( value[i] );
	} else {
	    vec.push_back( (const wchar_t *)value[i] );
	}
    }
}

template<class T>
void
WStringSequenceTmpl<T>::length (MICO_ULong l)
{
    if (l < vec.size ()) {
	vec.erase (vec.begin() + l, vec.end());
    } else if (l > vec.size()) {
	T t;
	// the (long) cast is needed for SGI STL
	vec.insert (vec.end(), long(l - vec.size()), t);
    }
}

template<class T>
inline MICO_ULong
WStringSequenceTmpl<T>::length () const
{
    return vec.size ();
}

template<class T>
inline T &
WStringSequenceTmpl<T>::operator[] (MICO_ULong idx)
{
    return vec[idx];
}
    

template<class T>
inline const T &
WStringSequenceTmpl<T>::operator[] (MICO_ULong idx) const
{
    return vec[idx];
}

#if !defined( __SUNPRO_CC )
template<class T>
MICO_Boolean
operator== (const WStringSequenceTmpl<T> &v1, const WStringSequenceTmpl<T> &v2)
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



//---Template for bounded sequence of wstrings----------------------------

template<class T, int max>
class BoundedWStringSequenceTmpl {
public:
    typedef T &ElementType; // Needed in TSeqVar (see var.h)
private:
    std::vector<T> vec;
public:
    BoundedWStringSequenceTmpl ()
    {
	vec.reserve (max);
    }
    BoundedWStringSequenceTmpl (MICO_ULong length, wchar_t** value,
				MICO_Boolean rel = TRUE);
    
    BoundedWStringSequenceTmpl (const BoundedWStringSequenceTmpl<T,max> &s)
    {
	vec = s.vec;
    }

    ~BoundedWStringSequenceTmpl ()
    {
    }

    void replace (MICO_ULong length, wchar_t** value,
		  MICO_Boolean rel = TRUE);

    BoundedWStringSequenceTmpl<T,max> &operator=
	(const BoundedWStringSequenceTmpl<T,max> &s)
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

    wchar_t **get_buffer (MICO_Boolean orphan = FALSE)
    {
	// XXX
	assert (0);
        return 0;
    }

    const wchar_t **get_buffer () const
    {
	// XXX
	assert (0);
        return 0;
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

    static wchar_t **allocbuf (MICO_ULong len)
    {
	return new wchar_t*[len];
    }

    static void freebuf (wchar_t **b)
    {
	delete[] b;
    }

#if defined( __SUNPRO_CC )
    friend MICO_Boolean
    operator== (const BoundedWStringSequenceTmpl<T,max> &v1,
		const BoundedWStringSequenceTmpl<T,max> &v2)
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

template<class T, int max>
BoundedWStringSequenceTmpl<T,max>::BoundedWStringSequenceTmpl (MICO_ULong lengthval,
							       wchar_t** value,
							       MICO_Boolean rel)
{
    assert (lengthval <= max);
    vec.reserve (max);
    for( MICO_ULong i = 0; i < lengthval; i++ ) {
	if (rel) {
	    vec.push_back( value[i] );
	} else {
	    vec.push_back( (const wchar_t *)value[i] );
	}
    }
}

template<class T, int max>
void
BoundedWStringSequenceTmpl<T,max>::replace (MICO_ULong lengthval,
					    wchar_t** value,
					    MICO_Boolean rel)
{
    assert (lengthval <= max);
    vec.erase (vec.begin(), vec.end());
    vec.reserve (max);
    for( MICO_ULong i = 0; i < lengthval; i++ ) {
	if (rel) {
	    vec.push_back( value[i] );
	} else {
	    vec.push_back( (const wchar_t *)value[i] );
	}
    }
}

template<class T, int max>
void
BoundedWStringSequenceTmpl<T,max>::length (MICO_ULong l)
{
    assert (l <= max);
    if (l < vec.size ()) {
	vec.erase (vec.begin() + l, vec.end());
    } else if (l > vec.size()) {
	T t;
	vec.insert (vec.end(), long(l - vec.size()), t);
    }
}

#if !defined( __SUNPRO_CC )
template<class T, int max>
MICO_Boolean
operator== (const BoundedWStringSequenceTmpl<T,max> &v1,
	    const BoundedWStringSequenceTmpl<T,max> &v2)
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


//---Template for unbounded sequence of interfaces-----------------------

template<class T, class T_ptr>
class IfaceSequenceTmpl {
public:
    typedef T &ElementType; // Needed in TSeqVar (see var.h)
    typedef TSeqVar<IfaceSequenceTmpl<T,T_ptr> > _var_type;
private:
    std::vector<T> vec;
public:
    IfaceSequenceTmpl () {}
    IfaceSequenceTmpl (MICO_ULong maxval)
    {
	vec.reserve (maxval);
    }
    IfaceSequenceTmpl (MICO_ULong max, MICO_ULong length, T_ptr* value,
		       MICO_Boolean rel = FALSE);

    IfaceSequenceTmpl (const IfaceSequenceTmpl<T,T_ptr> &s)
    {
	vec = s.vec;
    }
    
    ~IfaceSequenceTmpl ()
    {
    }
    
    void replace (MICO_ULong max, MICO_ULong length, T_ptr* value,
		  MICO_Boolean rel = FALSE);

    IfaceSequenceTmpl<T,T_ptr> &operator= (const IfaceSequenceTmpl<T,T_ptr> &s)
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

    T_ptr *get_buffer (MICO_Boolean orphan = FALSE)
    {
	// XXX
	assert (0);
        return 0;
    }

    const T_ptr *get_buffer () const
    {
	// XXX
	assert (0);
        return 0;
    }

    void length (MICO_ULong l);

    MICO_ULong length () const;
    T &operator[] (MICO_ULong idx);
    const T& operator[] (MICO_ULong idx) const;

    static T_ptr *allocbuf (MICO_ULong len)
    {
	return new T_ptr[len];
    }

    static void freebuf (T_ptr *b)
    {
	delete[] b;
    }

#if defined( __SUNPRO_CC )
    friend MICO_Boolean
    operator== (const IfaceSequenceTmpl<T,T_ptr> &v1,
		const IfaceSequenceTmpl<T,T_ptr> &v2)
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


template<class T, class T_ptr>
IfaceSequenceTmpl<T,T_ptr>::IfaceSequenceTmpl (MICO_ULong maxval,
					       MICO_ULong lengthval,
					       T_ptr* value,
					       MICO_Boolean rel)
{
    assert (lengthval <= maxval);
    vec.reserve (maxval);
    for( MICO_ULong i = 0; i < lengthval; i++ ) {
	if (rel) {
	    vec.push_back( value[i] );
	} else {
	    vec.push_back( T::duplicate (value[i]) );
	}
    }
}

template<class T, class T_ptr>
void
IfaceSequenceTmpl<T,T_ptr>::replace (MICO_ULong maxval, MICO_ULong lengthval,
				     T_ptr* value, MICO_Boolean rel)
{
    assert (lengthval <= maxval);
    vec.erase (vec.begin(), vec.end());
    vec.reserve (maxval);
    for( MICO_ULong i = 0; i < lengthval; i++ ) {
	if (rel) {
	    vec.push_back( value[i] );
	} else {
	    vec.push_back( T::duplicate (value[i]) );
	}
    }
}


template<class T, class T_ptr>
void
IfaceSequenceTmpl<T,T_ptr>::length (MICO_ULong l)
{
    if (l < vec.size ()) {
	vec.erase (vec.begin() + l, vec.end());
    } else if (l > vec.size()) {
	T t;
	// the (long) cast is needed for SGI STL
	vec.insert (vec.end(), long(l - vec.size()), t);
    }
}

template<class T, class T_ptr>
inline MICO_ULong
IfaceSequenceTmpl<T,T_ptr>::length () const
{
    return vec.size ();
}

template<class T, class T_ptr>
inline T &
IfaceSequenceTmpl<T,T_ptr>::operator[] (MICO_ULong idx)
{
    return vec[idx];
}
    

template<class T, class T_ptr>
inline const T &
IfaceSequenceTmpl<T,T_ptr>::operator[] (MICO_ULong idx) const
{
    return vec[idx];
}

#if !defined( __SUNPRO_CC )
template<class T, class T_ptr>
MICO_Boolean
operator== (const IfaceSequenceTmpl<T,T_ptr> &v1,
	    const IfaceSequenceTmpl<T,T_ptr> &v2)
{
    if (v1.length() != v2.length())
	return FALSE;
    for (MICO_ULong _i = 0; _i < v1.length(); ++_i) {
	if (!v1[_i]->_is_equivalent (v2[_i]))
	    return FALSE;
    }
    return TRUE;
}
#endif



//---Template for bounded sequence of interfaces-------------------------

template<class T, class T_ptr, int max>
class BoundedIfaceSequenceTmpl {
public:
    typedef T &ElementType; // Needed in TSeqVar (see var.h)
private:
    std::vector<T> vec;
public:
    BoundedIfaceSequenceTmpl ()
    {
	vec.reserve (max);
    }
    BoundedIfaceSequenceTmpl (MICO_ULong length, T_ptr* value,
			      MICO_Boolean rel = TRUE);
    
    BoundedIfaceSequenceTmpl (const BoundedIfaceSequenceTmpl<T,T_ptr,max> &s)
    {
	vec = s.vec;
    }

    ~BoundedIfaceSequenceTmpl ()
    {
    }

    void replace (MICO_ULong length, T_ptr* value,
		  MICO_Boolean rel = TRUE);

    BoundedIfaceSequenceTmpl<T,T_ptr,max> &operator=
	(const BoundedIfaceSequenceTmpl<T,T_ptr,max> &s)
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

    T_ptr* get_buffer (MICO_Boolean orphan = FALSE)
    {
	// XXX
	assert (0);
        return 0;
    }

    const T_ptr* get_buffer () const
    {
	// XXX
	assert (0);
        return 0;
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

    static T_ptr *allocbuf (MICO_ULong len)
    {
	return new T_ptr[len];
    }

    static void freebuf (T_ptr *b)
    {
	delete[] b;
    }

#if defined( __SUNPRO_CC )
    friend MICO_Boolean
    operator== (const BoundedIfaceSequenceTmpl<T,T_ptr,max> &v1,
		const BoundedIfaceSequenceTmpl<T,T_ptr,max> &v2)
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


template<class T, class T_ptr, int max>
BoundedIfaceSequenceTmpl<T,T_ptr,max>::
         BoundedIfaceSequenceTmpl (MICO_ULong lengthval,
				   T_ptr* value,
				   MICO_Boolean rel)
{
    assert (lengthval <= max);
    vec.reserve (max);
    for( MICO_ULong i = 0; i < lengthval; i++ ) {
	if (rel) {
	    vec.push_back( value[i] );
	} else {
	    vec.push_back( T::duplicate (value[i]) );
	}
    }
}

template<class T, class T_ptr, int max>
void
BoundedIfaceSequenceTmpl<T,T_ptr,max>::replace (MICO_ULong lengthval,
                                                T_ptr* value,
                                                MICO_Boolean rel)
{
    assert (lengthval <= max);
    vec.erase (vec.begin(), vec.end());
    vec.reserve (max);
    for( MICO_ULong i = 0; i < lengthval; i++ ) {
	if (rel) {
	    vec.push_back( value[i] );
	} else {
	    vec.push_back( T::duplicate (value[i]) );
	}
    }
}


template<class T, class T_ptr, int max>
void
BoundedIfaceSequenceTmpl<T,T_ptr,max>::length (MICO_ULong l)
{
    assert (l <= max);
    if (l < vec.size ()) {
	vec.erase (vec.begin() + l, vec.end());
    } else if (l > vec.size()) {
	T t;
	vec.insert (vec.end(), long(l - vec.size()), t);
    }
}

#if !defined( __SUNPRO_CC )
template<class T, class T_ptr, int max>
MICO_Boolean
operator== (const BoundedIfaceSequenceTmpl<T,T_ptr,max> &v1,
	    const BoundedIfaceSequenceTmpl<T,T_ptr,max> &v2)
{
    if (v1.length() != v2.length())
	return FALSE;
    for (MICO_ULong _i = 0; _i < v1.length(); ++_i) {
	if (!v1[_i]->_is_equivalent (v2[_i]))
	    return FALSE;
    }
    return TRUE;
}
#endif


#endif
