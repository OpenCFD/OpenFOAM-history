// -*- c++ -*-
/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2004 by The Mico Team
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

#ifndef __mico_template_h__
#define __mico_template_h__


/*
 * Template to generate _var types for object references
 */
template<class T> class ObjOut;

template<class T>
class ObjVar
{

    friend class ObjOut<T>;
    
private:
  T* _ptr;

  static T* nil()
  {
      return 0;
  }

public:
  static T* duplicate (T *);
  static void release (T *);

private:
  void free();
  void reset( T* ptr )
  {
      free();
      _ptr = ptr;
  }
    
public:
  ObjVar()
  {
      _ptr = nil();
  }
  ObjVar( T* ptr )
  {
      _ptr = ptr;
  }
  ObjVar( const ObjVar<T>& var )
  {
      _ptr = duplicate (var._ptr);
  }
  ~ObjVar()
  {
      free();
  }
  ObjVar<T>& operator=( const ObjVar<T> &var )
  {
      if (this != &var) {
	  free();
	  _ptr = duplicate( var._ptr );
      }
      return *this;
  }
  ObjVar<T>& operator=( T* ptr )
  {
      reset( ptr );
      return *this;
  }
// g++ const overload problem
#ifdef HAVE_CONST_OVERLOAD
  operator T*() const
  {
      return _ptr;
  }
#endif
  operator T* ()
  {
     return _ptr;
  }
  T* operator->() const
  {
      assert (_ptr);
      return _ptr;
  }

  T* in() const
  {
    return _ptr;
  }
  
  T*& inout()
  {
    return _ptr;
  }
  
  T*& out()
  {
    free();
    return _ptr;
  }

  T*& _for_demarshal()
  {
    return out();
  }
  
  T* _retn()
  {
      T *_p = _ptr;
      _ptr = nil();
      return _p;
  }
};

template<class T>
inline void
ObjVar<T>::free()
{
    release( _ptr );
    _ptr = nil();
}


/*
 * Template to generate _out types for object references
 */
template<class T>
class ObjOut
{
private:
    T*& _ptr;
    
public:
    ObjOut (T*& p);
    ObjOut (ObjVar<T>& p);
    ObjOut (const ObjOut<T>& p)
	: _ptr (p._ptr)
	{
	}
    ObjOut<T>& operator= (const ObjOut<T>& p)
	{
	    _ptr = p._ptr;
	    return *this;
	}
    ObjOut<T>& operator= (const ObjVar<T>& p);
    ObjOut<T>& operator= (T* p)
	{
	    _ptr = p;
	    return *this;
	}
    operator T& ()
	{
	    return *_ptr;
	}
    operator T*& ()
	{
	    return _ptr;
	}
    T* operator-> ()
	{
	    assert (_ptr);
	    return _ptr;
	}
    T*& ptr ()
	{
	    return _ptr;
	}
};

    

/*
 * Template to generate _var types for value types
 */

template <class T> class ValueOut;

template<class T>
class ValueVar
{

    friend class ValueOut<T>;
    
private:
  T* _ptr;

  static T* nil()
  {
      return 0;
  }
  static T* duplicate (T *);
  static void release (T *);

  void free();
  void reset( T* ptr )
  {
      free();
      _ptr = ptr;
  }
public:
  ValueVar()
  {
      _ptr = nil();
  }
  ValueVar( T* ptr )
  {
      _ptr = ptr;
  }
  ValueVar( const ValueVar<T>& var )
  {
      _ptr = duplicate (var._ptr);
  }
  ~ValueVar()
  {
      free();
  }
  ValueVar<T>& operator=( const ValueVar<T> &var )
  {
      if (this != &var) {
	  free();
	  _ptr = duplicate( var._ptr );
      }
      return *this;
  }
  ValueVar<T>& operator=( T* ptr )
  {
      if (_ptr != ptr)
	  reset( ptr );
      return *this;
  }
// g++ const overload problem
#ifdef HAVE_CONST_OVERLOAD
  operator T*() const
  {
      return _ptr;
  }
#endif
  operator T* ()
  {
    return _ptr;
  }
  T* operator->() const
  {
      assert (_ptr);
      return _ptr;
  }

  T* in() const
  {
    return _ptr;
  }
  
  T*& inout()
  {
    return _ptr;
  }
  
  T*& out()
  {
    free();
    return _ptr;
  }
  
  T*& _for_demarshal()
  {
    return out();
  }

  T* _retn()
  {
      T *_p = _ptr;
      _ptr = nil();
      return _p;
  }
};

template<class T>
inline void
ValueVar<T>::free()
{
    release( _ptr );
    _ptr = nil();
}


/*
 * Template to generate _out types for value types.
 */
template<class T>
class ValueOut
{
private:
    T*& _ptr;

    static T* nil()
	{
	    return 0;
	}
    
public:
    ValueOut (T*& p)
	: _ptr (p)
	{
	    _ptr = nil();
	}
    ValueOut (ValueVar<T>& p)
	: _ptr (p._ptr)
	{
	    release (_ptr);
	    _ptr = nil();
	}
    ValueOut (const ValueOut<T>& p)
	: _ptr (p._ptr)
	{
	}
    ValueOut<T>& operator= (const ValueOut<T>& p)
	{
	    _ptr = p._ptr;
	    return *this;
	}
    ValueOut<T>& operator= (T* p)
	{
	    _ptr = p;
	    return *this;
	}
    operator T* ()
	{
	    return _ptr;
	}
    T* operator-> ()
	{
	    assert (_ptr);
	    return _ptr;
	}
    T*& ptr ()
	{
	    return _ptr;
	}

private:
    void operator= (const ValueVar<T>& p);
};

	    
/*
 * Template to generate _var types for fixed length data types.
 * We don't need a class TFixOut for generating _out types of fixed
 * length data types. The _out types will be a simple refence to the
 * original type.
 */
template<class T>
class TFixVar
{
private:
  T* _ptr;
  
  void free();
  void reset( T* ptr )
  {
    free();
    _ptr = ptr;
  }
public:
  TFixVar()
  {
    _ptr = NULL;
  }
  TFixVar( T* ptr )
  {
    _ptr = ptr;
  }
  TFixVar( const TFixVar<T>& var )
  {
    if( !var._ptr )
      _ptr = NULL;
    else
      _ptr = new T (*var._ptr);
  }
  TFixVar( const T& t) : _ptr(new T(t)) { }
  ~TFixVar()
  {
    free();
  }
  TFixVar<T>& operator=( T* ptr )
  {
    if( _ptr != ptr )
      reset( ptr );
    return *this;
  }
  TFixVar<T>& operator=( const TFixVar<T>& var )
  {
    if( this != &var ) {
      free();
      if( !var._ptr )
	_ptr = NULL;
      else
	_ptr = new T (*var._ptr);
    }
    return *this;
  }
  TFixVar<T>& operator=(const T& t)
  {
    reset ( new T(t) );
    return *this;
  }
#ifdef HAVE_CONST_OVERLOAD
  operator const T&() const
  {
    assert( _ptr );
    return *_ptr;
  }
  operator T&()
  {
    assert (_ptr);
    return *_ptr;
  }
#else
  operator T&() const
  {
    assert (_ptr);
    return *_ptr;
  }
#endif
  operator T* ()
  {
    return _ptr;
  }
  T* operator->() const
  {
    assert( _ptr );
    return _ptr;
  }
  T& operator*() const
  {
    assert (_ptr);
    return *_ptr;
  }

  const T& in() const
  {
    assert (_ptr);
    return *_ptr;
  }
  
  T& inout()
  {
    assert (_ptr);
    return *_ptr;
  }
  
  T& out()
  {
    if (!_ptr) {
      _ptr = new T();
    }
    return *_ptr;
  }
    
  T& _for_demarshal()
  {
    return out();
  }
  
  T* _retn()
  {
    T *_p = _ptr;
    _ptr = NULL;
    return _p;
  }
};

template<class T>
inline void
TFixVar<T>::free()
{
    if( _ptr )
	delete _ptr;
    _ptr = NULL;
}


/*
 * Template to generate _var types for variable length data types.
 */

template <class T> class TVarOut;

template<class T>
class TVarVar
{

    friend class TVarOut<T>;
    
private:
  T* _ptr;
  
  void free();
  void reset( T* ptr )
  {
    free();
    _ptr = ptr;
  }
public:
  TVarVar()
  {
    _ptr = NULL;
  }
  TVarVar( T* ptr )
  {
    _ptr = ptr;
  }
  TVarVar( const TVarVar<T>& var )
  {
    if( !var._ptr )
      _ptr = NULL;
    else
      _ptr = new T (*var._ptr);
  }
  ~TVarVar()
  {
    free();
  }
  TVarVar<T>& operator=( T* ptr )
  {
    if( _ptr != ptr )
      reset( ptr );
    return *this;
  }
  TVarVar<T>& operator=( const TVarVar<T>& var )
  {
    if( this != &var ) {
      free();
      if( !var._ptr )
	_ptr = NULL;
      else
	_ptr = new T (*var._ptr);
    }
    return *this;
  }
#ifdef HAVE_CONST_OVERLOAD
  operator const T&() const
  {
    assert (_ptr);
    return *_ptr;
  }
  operator T&()
  {
    assert (_ptr);
    return *_ptr;
  }
#else
  operator T&() const
  {
    assert (_ptr);
    return *_ptr;
  }
#endif
  operator T* ()
  {
    return _ptr;
  }
  T* operator->() const
  {
    assert( _ptr );
    return _ptr;
  }
  T& operator*() const
  {
    assert (_ptr);
    return *_ptr;
  }
  const T& in() const
  {
    return *_ptr;
  }
  
  T& inout()
  {
    return *_ptr;
  }
  
  T*& out()
  {
    free();
    return _ptr;
  }

  T& _for_demarshal()
  {
    free();
    return *_ptr;
  }
  
  T* _retn()
  {
    T *_p = _ptr;
    _ptr = NULL;
    return _p;
  }
};

template<class T>
inline void
TVarVar<T>::free()
{
    if( _ptr )
	delete _ptr;
    _ptr = NULL;
}


/*
 * Template to generate _out types for variable length data types.
 */
template<class T>
class TVarOut
{
private:
    T*& _ptr;
    
public:
    TVarOut (T*& p)
	: _ptr (p)
	{
	    _ptr = NULL;
	}
    TVarOut (TVarVar<T>& p)
	: _ptr (p._ptr)
	{
	    delete _ptr;
	    _ptr = NULL;
	}
    TVarOut (const TVarOut<T>& p)
	: _ptr (p._ptr)
	{
	}
    TVarOut<T>& operator= (const TVarOut<T>& p)
	{
	    _ptr = p._ptr;
	    return *this;
	}
    TVarOut<T>& operator= (T* p)
	{
	    _ptr = p;
	    return *this;
	}
    operator T& ()
	{
	    return *_ptr;
	}
    operator T*& ()
	{
	    return _ptr;
	}
    T*& ptr ()
	{
	    return _ptr;
	}
    T* operator-> ()
	{
	    assert (_ptr);
	    return _ptr;
	}

private:
    void operator= (const TVarVar<T>&);
};

	    


/*
 * Template to generate _var types for sequences. The difference
 * to the TVarVar template is, that operator[] is overloaded as well.
 */

template <class T> class TSeqOut;

template<class T>
class TSeqVar
{

    friend class TSeqOut<T>;
    
private:
  T* _ptr;
  
  void free();
  void reset( T* ptr )
  {
    free();
    _ptr = ptr;
  }
public:
  TSeqVar()
  {
    _ptr = NULL;
  }
  TSeqVar( T* ptr )
  {
    _ptr = ptr;
  }
  TSeqVar( const TSeqVar<T>& var )
  {
    if (!var._ptr)
      _ptr = NULL;
    else
      _ptr = new T (*var._ptr);
  }
  ~TSeqVar()
  {
    free();
  }
  TSeqVar<T>& operator=( T* ptr )
  {
    if (_ptr != ptr)
      reset( ptr );
    return *this;
  }
  TSeqVar<T>& operator=( const TSeqVar<T>& var )
  {
    if (this != &var) {
      free();
      if (!var._ptr)
        _ptr = NULL;
      else
        _ptr = new T (*var._ptr);
    }
    return *this;
  }
#ifdef HAVE_CONST_OVERLOAD
  operator const T&() const
  {
    assert (_ptr);
    return *_ptr;
  }
  operator T&()
  {
    assert (_ptr);
    return *_ptr;
  }
#else
  operator T&() const
  {
    assert (_ptr);
    return *_ptr;
  }
#endif
  operator T*()
  {
    return _ptr;
  }
  T* operator->() const
  {
    assert (_ptr);
    return _ptr;
  }
  T& operator*() const
  {
    assert (_ptr);
    return *_ptr;
  }

  typename T::ElementType operator[] (MICO_ULong idx)
  {
    assert (_ptr);
    return (*_ptr)[ idx ];
  }

  const T& in() const
  {
    return *_ptr;
  }
  
  T& inout()
  {
    return *_ptr;
  }
  
  T*& out()
  {
    free();
    return _ptr;
  }
  
  T& _for_demarshal()
  {
    free();
    return *_ptr;
  }

  T* _retn()
  {
    T *_p = _ptr;
    _ptr = NULL;
    return _p;
  }
};

template<class T>
inline void
TSeqVar<T>::free()
{
    if( _ptr )
	delete _ptr;
    _ptr = NULL;
}


/*
 * Template to generate _out types for sequences.
 */
template<class T>
class TSeqOut
{
private:
    T*& _ptr;
    
public:
    TSeqOut (T*& p)
	: _ptr (p)
	{
	    _ptr = NULL;
	}
    TSeqOut (TSeqVar<T>& p)
	: _ptr (p._ptr)
	{
	    delete _ptr;
	    _ptr = NULL;
	}
    TSeqOut (const TSeqOut<T>& p)
	: _ptr (p._ptr)
	{
	}
    typename T::ElementType operator[] (MICO_ULong idx)
	{
	    assert (_ptr);
	    return (*_ptr)[ idx ];
	}

    TSeqOut<T>& operator= (const TSeqOut<T>& p)
	{
	    _ptr = p._ptr;
	    return *this;
	}
    TSeqOut<T>& operator= (T* p)
	{
	    _ptr = p;
	    return *this;
	}
    operator T& ()
	{
	    return *_ptr;
	}
    operator T*& ()
	{
	    return _ptr;
	}
    T* operator-> ()
	{
	    assert (_ptr);
	    return _ptr;
	}
    T*& ptr ()
	{
	    return _ptr;
	}

private:
    void operator= (const TSeqVar<T>&);
};

	    
#ifndef	HAVE_STD_EH

/*
 * Template to generate _var types for exceptions. Difference to
 * TVarVar is that _clone() is used instead of the copy constructor.
 * We can use the TVarOut template to generate the _out type for
 * exceptions.
 */

template<class T>
class ExceptVar
{
private:
  T* _ptr;

  void free();
  void reset( T* ptr )
  {
    free();
    _ptr = ptr;
  }
public:
  ExceptVar()
  {
    _ptr = NULL;
  }
  ExceptVar( T* ptr )
  {
    _ptr = ptr;
  }
  ExceptVar( const ExceptVar<T>& var )
  {
    if (!var._ptr)
      _ptr = NULL;
    else
      _ptr = (T *)var._ptr->_clone();
  }
  ~ExceptVar()
  {
    free();
  }
  ExceptVar<T>& operator=( T* ptr )
  {
    if (_ptr != ptr)
      reset( ptr );
    return *this;
  }
  ExceptVar<T>& operator=( const ExceptVar<T>& var )
  {
    if (this != &var) {
      free();
      if (!var._ptr)
	_ptr = NULL;
      else
	_ptr = (T *)var._ptr->_clone();
    }
    return *this;
  }
  T* operator->() const
  {
    assert (_ptr);
    return _ptr;
  }
  T& operator*() const
  {
    assert (_ptr);
    return *_ptr;
  }

  T* _retn()
  {
    T *_p = _ptr;
    _ptr = NULL;
    return _p;
  }
};

template<class T>
inline void
ExceptVar<T>::free()
{
    if( _ptr )
	delete _ptr;
    _ptr = NULL;
}

#endif // ! HAVE_STD_EH

#endif
