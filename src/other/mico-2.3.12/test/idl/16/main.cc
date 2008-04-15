#define FORCE_MARSHALLING
#define MICO_CONF_POA
#include <CORBA-SMALL.h>
#include "forward.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS


using namespace std;

class A_impl : virtual public POA_A
{
private:
  int counter;
public:
  A_impl()
  {
    counter = 0;
  };
  void op( B_ptr b )
  {
    cout << "A::op() [" << counter++ << "]" << endl;
    if( counter < 3 ) {
      //XXX b->op( _this() );
      A_var a = this->_this();
      b->op( a );
    }
  };
};


class B_impl : virtual public POA_B
{
private:
  int counter;
public:
  B_impl()
  {
    counter = 0;
  };
  void op( A_ptr a )
  {
    cout << "B::op() [" << counter++ << "]" << endl;
    if( counter < 3 ) {
      //XXX a->op( _this() );
      B_var b = this->_this();
      a->op( b );
    }
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
  A_impl* a = new A_impl;
  A_ptr a_obj = a->_this();
  B_impl* b = new B_impl;
  B_ptr b_obj = b->_this();
  
  CORBA::String_var a_ref = orb->object_to_string( a_obj );
  CORBA::String_var b_ref = orb->object_to_string( b_obj );
  
  //----------------------------------------------------------------
  
  // client side
#ifdef FORCE_MARSHALLING
  obj = new CORBA::Object( new CORBA::IOR( a_ref ) );
#else
  obj = orb->string_to_object( a_ref );
#endif
  A_var a_client = A::_narrow( obj );

#ifdef FORCE_MARSHALLING
  obj = new CORBA::Object( new CORBA::IOR( b_ref ) );
#else
  obj = orb->string_to_object( b_ref );
#endif
  B_var b_client = B::_narrow( obj );

  a_client->op( b_client );
  //b_client->op( a_client );
  
  CORBA::release( a_obj );
  CORBA::release( b_obj );
}
