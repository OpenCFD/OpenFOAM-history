#define FORCE_MARSHALLING
#define MICO_CONF_POA
#include <CORBA-SMALL.h>
#include "context.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS


using namespace std;

class A_impl : virtual public POA_A
{
public:
  A_impl()
  {
  };
  void bar( const char* str, CORBA::Context_ptr ctx )
  {
    cout << str << endl;
    CORBA::NVList_var vals;
    ctx->get_values( "", 0, "*", vals );
    for( CORBA::ULong i = 0; i < vals->count(); i++ ) {
      const char *s;
      CORBA::Boolean r = ( *vals->item( i )->value() >>= s );
      assert (r);
      cout << vals->item( i )->name() << ": " << s << endl;
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
  A_ptr server = a->_this();
  CORBA::String_var ref = orb->object_to_string( server );
  
  //----------------------------------------------------------------
  
  // client side
#ifdef FORCE_MARSHALLING
  obj = new CORBA::Object( new CORBA::IOR( ref ) );
#else
  obj = orb->string_to_object( ref );
#endif
  A_var client = A::_narrow( obj );

  CORBA::Any any;
  CORBA::Context_var ctx, ctx2;
  orb->get_default_context( ctx );

  ctx->create_child ("child", ctx2);

  any <<= "aa";
  ctx->set_one_value ("aa", any);

  any <<= "ab";
  ctx->set_one_value ("ab", any);

  any <<= "bb";
  ctx->set_one_value ("bb", any);

  any <<= "aa-child";
  ctx2->set_one_value ("aa", any);

  client->bar( "hello", ctx2 );

  CORBA::release( server );
}
