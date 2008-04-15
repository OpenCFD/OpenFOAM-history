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

class simple;
typedef simple *simple_ptr;
typedef simple_ptr simpleRef;
typedef ObjVar<simple> simple_var;
typedef simple_var simple_out;


// Common definitions for interface simple
class simple : 
  virtual public CORBA::Object
{
  public:
    virtual ~simple();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef simple_ptr _ptr_type;
    typedef simple_var _var_type;
    #endif

    static simple_ptr _narrow( CORBA::Object_ptr obj );
    static simple_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static simple_ptr _duplicate( simple_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static simple_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );
    static vector<CORBA::Narrow_proto> *_narrow_helpers;
    static bool _narrow_helper2( CORBA::Object_ptr obj );

    virtual void func( CORBA::Char id ) = 0;
    virtual void flush() = 0;

  protected:
    simple() {};
  private:
    simple( const simple& );
    void operator=( const simple& );
};

// Stub for interface simple
class simple_stub:
  virtual public simple
{
  public:
    virtual ~simple_stub();
    void func( CORBA::Char id );
    void flush();

  private:
    void operator=( const simple_stub& );
};

#ifndef MICO_CONF_NO_POA

class simple_stub_clp :
  virtual public simple_stub,
  virtual public PortableServer::StubBase
{
  public:
    simple_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~simple_stub_clp ();
    void func( CORBA::Char id );
    void flush();

  protected:
    simple_stub_clp ();
  private:
    void operator=( const simple_stub_clp & );
};

#endif // MICO_CONF_NO_POA

class simple_skel :
  virtual public StaticMethodDispatcher,
  virtual public simple
{
  public:
    simple_skel( const CORBA::BOA::ReferenceData & = CORBA::BOA::ReferenceData() );
    virtual ~simple_skel();
    simple_skel( CORBA::Object_ptr obj );
    virtual bool dispatch( CORBA::StaticServerRequest_ptr _req, CORBA::Environment &_env );
    simple_ptr _this();

};

#ifndef MICO_CONF_NO_POA

class POA_simple : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~POA_simple ();
    simple_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static POA_simple * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual void func( CORBA::Char id ) = 0;
    virtual void flush() = 0;

  protected:
    POA_simple () {};

  private:
    POA_simple (const POA_simple &);
    void operator= (const POA_simple &);
};

template<class T>
class POA_simple_tie : 
  virtual public POA_Tie_Base<T>,
  virtual public POA_simple
{
  public:
    POA_simple_tie (T &t)
      : POA_Tie_Base<T> (&t, PortableServer::POA::_nil(), FALSE)
    {}
    POA_simple_tie (T &t, PortableServer::POA_ptr _poa)
      : POA_Tie_Base<T> (&t, _poa, FALSE)
    {}
    POA_simple_tie (T *t, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, PortableServer::POA::_nil(), _rel)
    {}
    POA_simple_tie (T *t, PortableServer::POA_ptr _poa, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, _poa, _rel)
    {}
    virtual ~POA_simple_tie ();

    PortableServer::POA_ptr _default_POA ();

    void func( CORBA::Char id );
    void flush();

  protected:
    POA_simple_tie () {};

  private:
    POA_simple_tie (const POA_simple_tie<T> &);
    void operator= (const POA_simple_tie<T> &);
};

template<class T>
POA_simple_tie<T>::~POA_simple_tie ()
{
}

template<class T>
PortableServer::POA_ptr
POA_simple_tie<T>::_default_POA ()
{
  if (!CORBA::is_nil (POA_Tie_Base<T>::poa)) {
    return PortableServer::POA::_duplicate (POA_Tie_Base<T>::poa);
  }
  return PortableServer::ServantBase::_default_POA ();
}

template<class T>
void
POA_simple_tie<T>::func (CORBA::Char id)
{
  POA_Tie_Base<T>::ptr->func (id);
}

template<class T>
void
POA_simple_tie<T>::flush ()
{
  POA_Tie_Base<T>::ptr->flush ();
}

#endif // MICO_CONF_NO_POA

#if !defined(MICO_NO_TOPLEVEL_MODULES) || defined(MICO_MODULE__GLOBAL)

extern CORBA::StaticTypeInfo *_marshaller_simple;

#endif // !defined(MICO_NO_TOPLEVEL_MODULES) || defined(MICO_MODULE__GLOBAL)


#if !defined(MICO_NO_TOPLEVEL_MODULES) && !defined(MICO_IN_GENERATED_CODE)
#include <mico/template_impl.h>
#endif

#endif
