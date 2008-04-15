#define FORCE_MARSHALLING
#define MICO_CONF_POA
#include <CORBA-SMALL.h>
#include "calc.h"


using namespace std;

class Calc_impl : virtual public POA_Calc
{
private:
  CORBA::Long _total;

public:
  Calc_impl()
  {
    _total = 0;
  };
  void add( CORBA::Long x )
  {
    _total += x;
  };
  CORBA::Long total()
  {
    return _total;
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
  client->add( 3 );
  client->add( 4 );

  CORBA::Long z;
  z = client->total();
  cout << "Sum is " << z << endl;
  CORBA::release( calcref );
}
