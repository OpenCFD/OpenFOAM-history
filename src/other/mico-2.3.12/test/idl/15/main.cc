#define FORCE_MARSHALLING
#define MICO_CONF_POA
#include <CORBA-SMALL.h>
#include "inheritance.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS

#define VARIANT2


using namespace std;

class Base_impl : virtual public POA_Base
{
public:
  Base_impl()
  {
  };
  void op1()
  {
    cout << "Base::op1()" << endl;
  };
};


class Derived_impl : 
#ifdef VARIANT2
  virtual public POA_Base,
#else
  virtual public Base_impl,
#endif
  virtual public POA_Derived
{
public:
  Derived_impl()
  {
  };
#ifdef VARIANT2
  void op1()
  {
    cout << "Derived::op1()" << endl;
  };
#endif
  void op2()
  {
    cout << "Derived::op2()" << endl;
  };
};


int main( int argc, char *argv[] )
{
  // ORB initialization
  CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );
  CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);
  PortableServer::POAManager_var mgr = poa->the_POAManager();
  mgr->activate();

  // server side
  Base_impl*    base = new Base_impl;
  Base_ptr base_obj = base->_this();
  Derived_impl* derived = new Derived_impl;
  Derived_ptr derived_obj = derived->_this();
  
  CORBA::String_var base_ref = orb->object_to_string( base_obj );
  CORBA::String_var derived_ref = orb->object_to_string( derived_obj );
  
  //----------------------------------------------------------------
  
  // client side
#ifdef FORCE_MARSHALLING
  obj = new CORBA::Object( new CORBA::IOR( base_ref ) );
#else
  obj = orb->string_to_object( base_ref );
#endif
  Base_var base_client = Base::_narrow( obj );

#ifdef FORCE_MARSHALLING
  obj = new CORBA::Object( new CORBA::IOR( derived_ref ) );
#else
  obj = orb->string_to_object( derived_ref );
#endif
  Derived_var derived_client = Derived::_narrow( obj );

  base_client->op1();
  derived_client->op1();
  derived_client->op2();

  assert (derived_client->_is_a ("IDL:Base:1.0"));
  assert (!base_client->_is_a ("IDL:Derived:1.0"));

  CORBA::release( base_obj );
  CORBA::release( derived_obj );
}
