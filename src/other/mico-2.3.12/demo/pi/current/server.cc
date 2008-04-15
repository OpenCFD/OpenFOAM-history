/*
 * A simple "Hello World" example that uses the POA
 */

#include "hello.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else
#include <fstream.h>
#endif


using namespace std;

PortableInterceptor::SlotId slot_id;
CORBA::ORB_ptr orb_g = CORBA::ORB::_nil();

/*
 * Hello World implementation inherits the POA skeleton class
 */

class HelloWorld_impl : virtual public POA_HelloWorld
{
public:
  void hello ();
};

void
HelloWorld_impl::hello ()
{
  cout << "Hello World" << endl;
  // obtaining what we need from PICurrent
  // and seting back some other value
  CORBA::Object_var picurr_obj = orb_g->resolve_initial_references("PICurrent");
  PortableInterceptor::Current_ptr picurrent
      = PortableInterceptor::Current::_narrow(picurr_obj);
  assert(picurrent != NULL);
  CORBA::Any a;
  a <<= (CORBA::Long)2131976;
  picurrent->set_slot(slot_id, a);
  // ucomment line below to see that send_exception
  // interception point is working too
  //throw CORBA::NO_PERMISSION();
}

class MyServerInterceptor
    : virtual public PortableInterceptor::ServerRequestInterceptor,
      virtual public CORBA::LocalObject
{
    string nm;
	
public:
	
    MyServerInterceptor() {
	nm = "";
    }
    MyServerInterceptor(char * name) {	
	nm = name;
    }
	
    char* name() {
	return (char *)nm.c_str();
    }

    void destroy()
    { cout << this->name() << " destroy" << endl; }

    void receive_request_service_contexts( PortableInterceptor::ServerRequestInfo_ptr ri ) {
	CORBA::Any* v = ri->get_slot(slot_id);
	CORBA::TypeCode_var tc = v->type();
	// set slot
	if (tc->kind() == CORBA::tk_null
	    ||tc->kind() == CORBA::tk_void) {
	    CORBA::Any a;
	    a <<= (CORBA::Long)213;
	    ri->set_slot(slot_id, a);
	}
	// and test if it's really set
	CORBA::Any* v2 = ri->get_slot(slot_id);
	assert(v2->type()->kind() == CORBA::tk_long);
	CORBA::Long extr = 0;
	(*v2) >>= extr;
	assert(extr == 213);
    }
    
    void receive_request( PortableInterceptor::ServerRequestInfo_ptr ri ) {
	// after receive_request_interception_point slots from SRI are copied
	// into PICurrent so now we can check if it's really true.
	CORBA::Object_var picurr_obj = orb_g->resolve_initial_references("PICurrent");
	PortableInterceptor::Current_ptr picurrent
	    = PortableInterceptor::Current::_narrow(picurr_obj);
	assert(picurrent != NULL);
	CORBA::Any* v2 = picurrent->get_slot(slot_id);
	assert(v2->type()->kind() == CORBA::tk_long);
	CORBA::Long extr = 0;
	(*v2) >>= extr;
	assert(extr == 213);
	// now we reset this slot on PICurrent and will see
	// if it change even on SRI which will be wrong of course
	(*v2) <<= (CORBA::Long)426;
	picurrent->set_slot(slot_id, *v2);
	delete v2;
	v2 = picurrent->get_slot(slot_id);
	assert(v2->type()->kind() == CORBA::tk_long);
	extr = 0;
	(*v2) >>= extr;
	assert(extr == 426);
	// test if SRI slots change (wrong), it has to contains
	// value independed from PICurrent
	CORBA::Any* v3 = ri->get_slot(slot_id);
	extr = 0;
	(*v3) >>= extr;
	assert(extr == 213);
    }
    
    void send_reply( PortableInterceptor::ServerRequestInfo_ptr ri ) {
	// user level code (HelloWorld_impl) set something into PICurrent
	// test if it's really true
	CORBA::Object_var picurr_obj = orb_g->resolve_initial_references("PICurrent");
	PortableInterceptor::Current_ptr picurrent
	    = PortableInterceptor::Current::_narrow(picurr_obj);
	assert(picurrent != NULL);
	CORBA::Any* v2 = picurrent->get_slot(slot_id);
	assert(v2->type()->kind() == CORBA::tk_long);
	CORBA::Long extr = 0;
	(*v2) >>= extr;
	assert(extr == 2131976);
    }
    
    void send_exception( PortableInterceptor::ServerRequestInfo_ptr ri ) {
	// user level code (HelloWorld_impl) set something into PICurrent
	// test if it's really true
	CORBA::Object_var picurr_obj = orb_g->resolve_initial_references("PICurrent");
	PortableInterceptor::Current_ptr picurrent
	    = PortableInterceptor::Current::_narrow(picurr_obj);
	assert(picurrent != NULL);
	CORBA::Any* v2 = picurrent->get_slot(slot_id);
	assert(v2->type()->kind() == CORBA::tk_long);
	CORBA::Long extr = 0;
	(*v2) >>= extr;
	assert(extr == 2131976);
    }
    
    void send_other( PortableInterceptor::ServerRequestInfo_ptr ri ) {
	// user level code (HelloWorld_impl) set something into PICurrent
	// test if it's really true
	CORBA::Object_var picurr_obj = orb_g->resolve_initial_references("PICurrent");
	PortableInterceptor::Current_ptr picurrent
	    = PortableInterceptor::Current::_narrow(picurr_obj);
	assert(picurrent != NULL);
	CORBA::Any* v2 = picurrent->get_slot(slot_id);
	assert(v2->type()->kind() == CORBA::tk_long);
	CORBA::Long extr = 0;
	(*v2) >>= extr;
	assert(extr == 2131976);
    }
	
};

class MyInitializer
    : virtual public PortableInterceptor::ORBInitializer,
      virtual public CORBA::LocalObject
{
public:
    MyInitializer() {}
    ~MyInitializer() {}

    virtual void pre_init( PortableInterceptor::ORBInitInfo_ptr info ) {
    	// register server interceptor(s)
	slot_id = info->allocate_slot_id();
    	MyServerInterceptor * interceptor = new MyServerInterceptor("MyServerInterceptor");
    	info->add_server_request_interceptor(interceptor);
//      MyServerInterceptor * interceptor2 = new MyServerInterceptor("MyServerInterceptor2");
//      info->add_server_request_interceptor(interceptor2);
    }
    
    virtual void post_init( PortableInterceptor::ORBInitInfo_ptr info ) {
    	// nothing
    }
};

int
main (int argc, char *argv[])
{
  /*
   * Initialize the ORB
   */
    MyInitializer * ini = new MyInitializer;
    PortableInterceptor::register_orb_initializer(ini);

  orb_g = CORBA::ORB_init (argc, argv);

  /*
   * Obtain a reference to the RootPOA and its Manager
   */

  CORBA::Object_var poaobj = orb_g->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
  PortableServer::POAManager_var mgr = poa->the_POAManager();

  /*
   * Create a Hello World object
   */

  HelloWorld_impl * hello = new HelloWorld_impl;

  /*
   * Activate the Servant
   */

  PortableServer::ObjectId_var oid = poa->activate_object (hello);

  /*
   * Write reference to file
   */

  ofstream of ("hello.ref");
  CORBA::Object_var ref = poa->id_to_reference (oid.in());
  CORBA::String_var str = orb_g->object_to_string (ref.in());
  of << str.in() << endl;
  of.close ();

  /*
   * Activate the POA and start serving requests
   */

  cout << "Running." << endl;

  mgr->activate ();
  orb_g->run();

  /*
   * Shutdown (never reached)
   */

  poa->destroy (TRUE, TRUE);
  delete hello;

  return 0;
}
