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

#ifndef __mico_value_h__
#define __mico_value_h__

namespace CORBA {

class ValueBase {
protected:
    // begin-mico-extension
    static ValueBase *_create (const std::vector<std::string> &repoids,
			       const std::string &myrepoid);
    // end-mico-extension
public:
    virtual void _add_ref () = 0;
    virtual void _remove_ref () = 0;
    virtual ULong _refcount_value () = 0;
    virtual ValueBase *_copy_value ();
    virtual ValueDef_ptr get_value_def () = 0;

    static ValueBase *_downcast (ValueBase *);

    // begin-mico-extension
    static void _marshal (DataEncoder &ec, ValueBase *vb);
    static Boolean _demarshal (DataDecoder &dc, ValueBase *&vb,
			       const std::string &myrepoid);
    virtual void * _narrow_helper (const char *);
    // end-mico-extension

protected:
    ValueBase ();
    ValueBase (const ValueBase &);
    virtual ~ValueBase ();

    // begin-mico-extension
    virtual void _get_marshal_info (std::vector<std::string> &repoids,
				    Boolean &chunked);
    virtual void _marshal_members (DataEncoder &ec);
    virtual Boolean _demarshal_members (DataDecoder &dc);
    // end-mico-extension

public:
    // support for arbitrarily recursive ValueTypes
    typedef std::set<ValueBase *, std::less<ValueBase *> > visited;
    virtual Long _count_refs (visited * = 0);
    virtual void _release_members ();
    bool _destructing;

private:
    void operator= (const ValueBase&);
};

typedef ValueVar<ValueBase> ValueBase_var;
typedef ValueOut<ValueBase> ValueBase_out;

static inline void add_ref (ValueBase *vb)
{
    if (vb)
	vb->_add_ref();
}

static inline void remove_ref (ValueBase *vb)
{
    if (vb)
	vb->_remove_ref();
}

class DefaultValueRefCountBase :
  virtual public ServerlessObject,
  virtual public ValueBase
{
public:
  DefaultValueRefCountBase ();
  virtual ~DefaultValueRefCountBase ();

  void _add_ref () { _ref(); }
  void _remove_ref () {
    if (_deref()) {
      delete this;
    }
    else if (!_destructing && _count_refs () == -1) {
      // orphaned circular graph
      _add_ref ();
      _release_members ();
      delete this;
    }
  }
  ULong _refcount_value () { return _refcnt(); }

private:
  void operator= (const DefaultValueRefCountBase &);
};

// begin-mico-extension
/*
 * This class can be used instead of DefaultValueRefCountBase. It does
 * not check for orphaned circular graphs and is much faster for value
 * types that are *potentially* recursive but that do not actually
 * employ recursion, for example lists or trees.
 */

class SimpleValueRefCountBase :
  virtual public ServerlessObject,
  virtual public ValueBase
{
public:
  SimpleValueRefCountBase ();
  virtual ~SimpleValueRefCountBase ();

  void _add_ref () { _ref(); }
  void _remove_ref () {
    if (_deref()) {
      delete this;
    }
  }
  ULong _refcount_value () { return _refcnt(); }

private:
  void operator= (const SimpleValueRefCountBase &);
};
// end-mico-extension

class ValueFactoryBase;
typedef ValueFactoryBase *ValueFactory;
typedef ValueVar<ValueFactoryBase> ValueFactoryBase_var;

class ValueFactoryBase : public ServerlessObject {
public:
    virtual ~ValueFactoryBase ();

    virtual void _add_ref () { _ref(); }
    virtual void _remove_ref () { if (_deref()) delete this; }

    static ValueFactory _downcast (ValueFactory);

    // begin-mico-extension
    virtual void * _narrow_helper (const char *);
    // end-mico-extension

protected:
    ValueFactoryBase ();

//private:
public:
    virtual ValueBase *create_for_unmarshal () = 0;
};

//-------------------

class AbstractBase;
typedef AbstractBase *AbstractBase_ptr;

class AbstractBase {
public:
  virtual ~AbstractBase () = 0;

  static AbstractBase_ptr _nil ()
  {
    return 0;
  }
  static AbstractBase_ptr _duplicate (AbstractBase_ptr);
  static AbstractBase_ptr _narrow (AbstractBase_ptr);

  // begin-mico-extension
  virtual Object_ptr _to_object ();
  virtual ValueBase * _to_value ();
  static void _marshal (DataEncoder &ec, AbstractBase *vb);
  static Boolean _demarshal (DataDecoder &dc, AbstractBase *&vb);
  virtual void * _narrow_helper (const char *);
  // end-mico-extension

protected:
  AbstractBase ();
  AbstractBase (const AbstractBase &);
};

typedef ObjVar<AbstractBase> AbstractBase_var;
typedef AbstractBase_var AbstractBase_out;

extern void release (AbstractBase_ptr);
static inline Boolean is_nil (AbstractBase_ptr ab)
{
  return !ab;
}

/*
 * We need MixedBase to resolve overloading problems; if a concrete
 * interface is derived from both AbstractBase and ServerlessObject,
 * the use of release() is ambiguous.
 */

class MixedBase :
  virtual public AbstractBase,
  virtual public Object
{
public:
  virtual ~MixedBase () = 0;
};

static inline void release (MixedBase *mb)
{
  CORBA::release ((Object_ptr) mb);
}

static inline Boolean is_nil (MixedBase *mb)
{
  return CORBA::is_nil ((Object_ptr) mb);
}

/*
 * UnknownAbstract is used when unmarshalling an AbstractBase type
 * where we don't know a concrete type and either represent an Object
 * or a Valuetype, and support narrowing/downcasting to both.
 */

class UnknownAbstract : virtual public AbstractBase
{
public:
  UnknownAbstract (Object_ptr, ValueBase *);
  ~UnknownAbstract ();

  Object_ptr _to_object ();
  ValueBase * _to_value ();
  void * _narrow_helper (const char *);

private:
  Object_var obj;
  ValueBase_var val;
};

}

#endif // __mico_value_h__
