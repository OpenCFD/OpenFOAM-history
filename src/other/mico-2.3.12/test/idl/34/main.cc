//
// This is a test for testing any type ability to extract object
// reference several times to different types, i.e. Calc, Object
//
#define FORCE_MARSHALLING
#define MICO_CONF_POA
#include <CORBA-SMALL.h>
#include "calc.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS


using namespace std;

class Calc_impl : virtual public POA_Calc
{
public:
  Calc_impl()
  {
  };
  void add( CORBA::Short x, CORBA::Short y, CORBA::Long& z )
  {
    z = x + y;
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
  Calc_impl* server = new Calc_impl;
  Calc_ptr calcref = server->_this();
  CORBA::String_var ref = orb->object_to_string( calcref );
  
  //----------------------------------------------------------------
  
  // client side
#ifdef FORCE_MARSHALLING
  obj = new CORBA::Object( new CORBA::IOR( ref ) );
#else
  obj = orb->string_to_object( ref );
#endif
  Calc_var client = Calc::_narrow( obj );

  CORBA::Any calc_any;
  calc_any <<= client;
  CORBA::Object_ptr calc_obj = CORBA::Object::_nil();
  CORBA::Boolean result = FALSE;
  result = (calc_any >>= calc_obj);
  assert(result);
  // ^ this is widening of object reference so we need to call release
  CORBA::release(calc_obj);
  Calc_ptr calc = Calc::_nil();
  result = (calc_any >>= calc);
  assert(result);
  //cout << "calc->_refcnt(): " << calc->_refcnt() << endl;
  CORBA::Long a;
  client->add( 3, 4, a );
  cout << "Sum is " << a << endl;

  a = 0;
  calc->add(3, 4, a);
  cout << "Sum is " << a << endl;

  a = 0;
  Calc_ptr calc2 = Calc::_nil();
  result = (calc_any >>= calc2);
  assert(result);
  calc2->add(3, 4, a);
  cout << "Sum is " << a << endl;

  result = (calc_any >>= calc_obj);
  assert(result);
  // ^ this is widening of object reference so we need to call release
  CORBA::release(calc_obj);

  a = 0;
  Calc_ptr calc3 = Calc::_nil();
  result = (calc_any >>= calc3);
  assert(result);
  calc3->add(3, 4, a);
  cout << "Sum is " << a << endl;

  CORBA::release( calcref );
}
