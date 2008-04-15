/*
 *  MICO --- a free CORBA implementation
 *  Copyright (C) 1997-98 Kay Roemer & Arno Puder
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#if !defined(__SIMPLE_H__) || defined(MICO_NO_TOPLEVEL_MODULES)
#define __SIMPLE_H__

#ifndef MICO_NO_TOPLEVEL_MODULES
#include <CORBA.h>
#include <mico/throw.h>
#endif

class simple1;
typedef simple1 *simple1_ptr;
typedef simple1_ptr simple1Ref;
typedef ObjVar<simple1> simple1_var;
typedef simple1_var simple1_out;


// Common definitions for interface simple1
class simple1 : 
  virtual public CORBA::Object
{
  public:
    virtual ~simple1();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef simple1_ptr _ptr_type;
    typedef simple1_var _var_type;
    #endif

    static simple1_ptr _narrow( CORBA::Object_ptr obj );
    static simple1_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static simple1_ptr _duplicate( simple1_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static simple1_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );
    static vector<CORBA::Narrow_proto> *_narrow_helpers;
    static bool _narrow_helper2( CORBA::Object_ptr obj );

    virtual void func( CORBA::Char id ) = 0;
    virtual void flush() = 0;

  protected:
    simple1() {};
  private:
    simple1( const simple1& );
    void operator=( const simple1& );
};

// Stub for interface simple1
class simple1_stub:
  virtual public simple1
{
  public:
    virtual ~simple1_stub();
    void func( CORBA::Char id );
    void flush();

  private:
    void operator=( const simple1_stub& );
};

#ifndef MICO_CONF_NO_POA

class simple1_stub_clp :
  virtual public simple1_stub,
  virtual public PortableServer::StubBase
{
  public:
    simple1_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~simple1_stub_clp ();
    void func( CORBA::Char id );
    void flush();

  protected:
    simple1_stub_clp ();
  private:
    void operator=( const simple1_stub_clp & );
};

#endif // MICO_CONF_NO_POA

class simple1_skel :
  virtual public StaticMethodDispatcher,
  virtual public simple1
{
  public:
    simple1_skel( const CORBA::BOA::ReferenceData & = CORBA::BOA::ReferenceData() );
    virtual ~simple1_skel();
    simple1_skel( CORBA::Object_ptr obj );
    virtual bool dispatch( CORBA::StaticServerRequest_ptr _req, CORBA::Environment &_env );
    simple1_ptr _this();

};

class simple2;
typedef simple2 *simple2_ptr;
typedef simple2_ptr simple2Ref;
typedef ObjVar<simple2> simple2_var;
typedef simple2_var simple2_out;


// Common definitions for interface simple2
class simple2 : 
  virtual public CORBA::Object
{
  public:
    virtual ~simple2();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef simple2_ptr _ptr_type;
    typedef simple2_var _var_type;
    #endif

    static simple2_ptr _narrow( CORBA::Object_ptr obj );
    static simple2_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static simple2_ptr _duplicate( simple2_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static simple2_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );
    static vector<CORBA::Narrow_proto> *_narrow_helpers;
    static bool _narrow_helper2( CORBA::Object_ptr obj );

    virtual void func( CORBA::Char id ) = 0;
    virtual void flush() = 0;

  protected:
    simple2() {};
  private:
    simple2( const simple2& );
    void operator=( const simple2& );
};

// Stub for interface simple2
class simple2_stub:
  virtual public simple2
{
  public:
    virtual ~simple2_stub();
    void func( CORBA::Char id );
    void flush();

  private:
    void operator=( const simple2_stub& );
};

#ifndef MICO_CONF_NO_POA

class simple2_stub_clp :
  virtual public simple2_stub,
  virtual public PortableServer::StubBase
{
  public:
    simple2_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~simple2_stub_clp ();
    void func( CORBA::Char id );
    void flush();

  protected:
    simple2_stub_clp ();
  private:
    void operator=( const simple2_stub_clp & );
};

#endif // MICO_CONF_NO_POA

class simple2_skel :
  virtual public StaticMethodDispatcher,
  virtual public simple2
{
  public:
    simple2_skel( const CORBA::BOA::ReferenceData & = CORBA::BOA::ReferenceData() );
    virtual ~simple2_skel();
    simple2_skel( CORBA::Object_ptr obj );
    virtual bool dispatch( CORBA::StaticServerRequest_ptr _req, CORBA::Environment &_env );
    simple2_ptr _this();

};

#ifndef MICO_CONF_NO_POA

class POA_simple1 : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~POA_simple1 ();
    simple1_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static POA_simple1 * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual void func( CORBA::Char id ) = 0;
    virtual void flush() = 0;

  protected:
    POA_simple1 () {};

  private:
    POA_simple1 (const POA_simple1 &);
    void operator= (const POA_simple1 &);
};

template<class T>
class POA_simple1_tie : 
  virtual public POA_Tie_Base<T>,
  virtual public POA_simple1
{
  public:
    POA_simple1_tie (T &t)
      : POA_Tie_Base<T> (&t, PortableServer::POA::_nil(), FALSE)
    {}
    POA_simple1_tie (T &t, PortableServer::POA_ptr _poa)
      : POA_Tie_Base<T> (&t, _poa, FALSE)
    {}
    POA_simple1_tie (T *t, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, PortableServer::POA::_nil(), _rel)
    {}
    POA_simple1_tie (T *t, PortableServer::POA_ptr _poa, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, _poa, _rel)
    {}
    virtual ~POA_simple1_tie ();

    PortableServer::POA_ptr _default_POA ();

    void func( CORBA::Char id );
    void flush();

  protected:
    POA_simple1_tie () {};

  private:
    POA_simple1_tie (const POA_simple1_tie<T> &);
    void operator= (const POA_simple1_tie<T> &);
};

template<class T>
POA_simple1_tie<T>::~POA_simple1_tie ()
{
}

template<class T>
PortableServer::POA_ptr
POA_simple1_tie<T>::_default_POA ()
{
  if (!CORBA::is_nil (POA_Tie_Base<T>::poa)) {
    return PortableServer::POA::_duplicate (POA_Tie_Base<T>::poa);
  }
  return PortableServer::ServantBase::_default_POA ();
}

template<class T>
void
POA_simple1_tie<T>::func (CORBA::Char id)
{
  POA_Tie_Base<T>::ptr->func (id);
}

template<class T>
void
POA_simple1_tie<T>::flush ()
{
  POA_Tie_Base<T>::ptr->flush ();
}

class POA_simple2 : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~POA_simple2 ();
    simple2_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static POA_simple2 * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual void func( CORBA::Char id ) = 0;
    virtual void flush() = 0;

  protected:
    POA_simple2 () {};

  private:
    POA_simple2 (const POA_simple2 &);
    void operator= (const POA_simple2 &);
};

template<class T>
class POA_simple2_tie : 
  virtual public POA_Tie_Base<T>,
  virtual public POA_simple2
{
  public:
    POA_simple2_tie (T &t)
      : POA_Tie_Base<T> (&t, PortableServer::POA::_nil(), FALSE)
    {}
    POA_simple2_tie (T &t, PortableServer::POA_ptr _poa)
      : POA_Tie_Base<T> (&t, _poa, FALSE)
    {}
    POA_simple2_tie (T *t, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, PortableServer::POA::_nil(), _rel)
    {}
    POA_simple2_tie (T *t, PortableServer::POA_ptr _poa, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, _poa, _rel)
    {}
    virtual ~POA_simple2_tie ();

    PortableServer::POA_ptr _default_POA ();

    void func( CORBA::Char id );
    void flush();

  protected:
    POA_simple2_tie () {};

  private:
    POA_simple2_tie (const POA_simple2_tie<T> &);
    void operator= (const POA_simple2_tie<T> &);
};

template<class T>
POA_simple2_tie<T>::~POA_simple2_tie ()
{
}

template<class T>
PortableServer::POA_ptr
POA_simple2_tie<T>::_default_POA ()
{
  if (!CORBA::is_nil (POA_Tie_Base<T>::poa)) {
    return PortableServer::POA::_duplicate (POA_Tie_Base<T>::poa);
  }
  return PortableServer::ServantBase::_default_POA ();
}

template<class T>
void
POA_simple2_tie<T>::func (CORBA::Char id)
{
  POA_Tie_Base<T>::ptr->func (id);
}

template<class T>
void
POA_simple2_tie<T>::flush ()
{
  POA_Tie_Base<T>::ptr->flush ();
}

#endif // MICO_CONF_NO_POA

#if !defined(MICO_NO_TOPLEVEL_MODULES) || defined(MICO_MODULE__GLOBAL)

extern CORBA::StaticTypeInfo *_marshaller_simple1;

extern CORBA::StaticTypeInfo *_marshaller_simple2;

#endif // !defined(MICO_NO_TOPLEVEL_MODULES) || defined(MICO_MODULE__GLOBAL)


#if !defined(MICO_NO_TOPLEVEL_MODULES) && !defined(MICO_IN_GENERATED_CODE)
#include <mico/template_impl.h>
#endif

#endif
