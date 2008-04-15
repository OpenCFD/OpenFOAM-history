/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>
#include <mico/throw.h>

#ifndef __REFLECTION_H__
#define __REFLECTION_H__


#ifdef _WIN32
// If you compile this file and would like to link
// it into the DLL, then please define ``BUILD_MICO_DLL'' symbol
// on your C++ compiler command-line.
// Depending on your compiler, you can use either
// ``-DBUILD_MICO_DLL'' or ``/DBUILD_MICO_DLL'' parameter
#ifdef BUILD_MICO_DLL
// Please do not forget to use gendef tool to correctly
// export all symbols from the resulting object file.
// If you would like to use more common __declspec(dllexport) way,
// then please use --windows-dll-with-export <name>
// IDL compiler parameter
#define MICO_EXPORT /**/
#else // BUILD_MICO_DLL
#define MICO_EXPORT __declspec(dllimport)
#endif // BUILD_MICO_DLL
#else // _WIN32
// all other platforms
#define MICO_EXPORT /**/
#endif // _WIN32




namespace Reflection
{

class IFRProvider;
typedef IFRProvider *IFRProvider_ptr;
typedef IFRProvider_ptr IFRProviderRef;
typedef ObjVar< IFRProvider > IFRProvider_var;
typedef ObjOut< IFRProvider > IFRProvider_out;

}






namespace Reflection
{

struct FormatNotSupported : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  FormatNotSupported();
  ~FormatNotSupported();
  FormatNotSupported( const FormatNotSupported& s );
  FormatNotSupported& operator=( const FormatNotSupported& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS


  #ifdef HAVE_STD_EH
  FormatNotSupported *operator->() { return this; }
  FormatNotSupported& operator*() { return *this; }
  operator FormatNotSupported*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static FormatNotSupported *_downcast( CORBA::Exception *ex );
  static const FormatNotSupported *_downcast( const CORBA::Exception *ex );
};

#ifdef HAVE_STD_EH
typedef FormatNotSupported FormatNotSupported_catch;
#else
typedef ExceptVar< FormatNotSupported > FormatNotSupported_var;
typedef TVarOut< FormatNotSupported > FormatNotSupported_out;
typedef FormatNotSupported_var FormatNotSupported_catch;
#endif // HAVE_STD_EH

extern MICO_EXPORT CORBA::TypeCodeConst _tc_FormatNotSupported;


/*
 * Base class and common definitions for interface IFRProvider
 */

class IFRProvider : 
  virtual public CORBA::Object
{
  public:
    virtual ~IFRProvider();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef IFRProvider_ptr _ptr_type;
    typedef IFRProvider_var _var_type;
    #endif

    static IFRProvider_ptr _narrow( CORBA::Object_ptr obj );
    static IFRProvider_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static IFRProvider_ptr _duplicate( IFRProvider_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static IFRProvider_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::Any* omg_get_ifr_metadata() = 0;
    virtual char* omg_get_xml_metadata() = 0;

  protected:
    IFRProvider() {};
  private:
    IFRProvider( const IFRProvider& );
    void operator=( const IFRProvider& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_IFRProvider;

// Stub for interface IFRProvider
class IFRProvider_stub:
  virtual public IFRProvider
{
  public:
    virtual ~IFRProvider_stub();
    CORBA::Any* omg_get_ifr_metadata();
    char* omg_get_xml_metadata();

  private:
    void operator=( const IFRProvider_stub& );
};

#ifndef MICO_CONF_NO_POA

class IFRProvider_stub_clp :
  virtual public IFRProvider_stub,
  virtual public PortableServer::StubBase
{
  public:
    IFRProvider_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~IFRProvider_stub_clp ();
    CORBA::Any* omg_get_ifr_metadata();
    char* omg_get_xml_metadata();

  protected:
    IFRProvider_stub_clp ();
  private:
    void operator=( const IFRProvider_stub_clp & );
};

#endif // MICO_CONF_NO_POA

}


#ifndef MICO_CONF_NO_POA



namespace POA_Reflection
{

class IFRProvider : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~IFRProvider ();
    Reflection::IFRProvider_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static IFRProvider * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual CORBA::Any* omg_get_ifr_metadata() = 0;
    virtual char* omg_get_xml_metadata() = 0;

  protected:
    IFRProvider () {};

  private:
    IFRProvider (const IFRProvider &);
    void operator= (const IFRProvider &);
};

}


#endif // MICO_CONF_NO_POA

void operator<<=( CORBA::Any &a, const ::Reflection::FormatNotSupported &e );
void operator<<=( CORBA::Any &a, ::Reflection::FormatNotSupported *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::Reflection::FormatNotSupported &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::Reflection::FormatNotSupported *&e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Reflection_FormatNotSupported;

void operator<<=( CORBA::Any &a, const Reflection::IFRProvider_ptr obj );
void operator<<=( CORBA::Any &a, Reflection::IFRProvider_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, Reflection::IFRProvider_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Reflection_IFRProvider;

#endif
