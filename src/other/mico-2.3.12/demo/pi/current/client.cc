#include "hello.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef _WIN32
#include <direct.h>
#endif


using namespace std;

HelloWorld_ptr hello;
CORBA::Boolean callit = TRUE;
PortableInterceptor::SlotId slot_id;
CORBA::ORB_var orb_g = CORBA::ORB::_nil();

class MyClientInterceptor
    : virtual public PortableInterceptor::ClientRequestInterceptor,
      virtual public CORBA::LocalObject
{
    string nm;
public:
    MyClientInterceptor()
    { nm = ""; }

    MyClientInterceptor(char * name)
    { nm = name; }
	
    char* name()
    { return CORBA::string_dup(nm.c_str()); }

    void destroy()
    { cout << this->name() << " destroy" << endl; }

    void send_request( PortableInterceptor::ClientRequestInfo_ptr ri ) {
	CORBA::Any* recurse = ri->get_slot(slot_id);
	CORBA::TypeCode_var tc = recurse->type();
	if (tc->kind() == CORBA::tk_null
	    ||tc->kind() == CORBA::tk_void) {
	    CORBA::Any rec;
	    rec.insert((CORBA::Any::from_boolean)TRUE);
	    CORBA::Object_var obj = orb_g->resolve_initial_references("PICurrent");
	    PortableInterceptor::Current_var current
		= PortableInterceptor::Current::_narrow(obj);
	    current->set_slot(slot_id, rec);
	    cout << "client request interceptor(" << this->name() <<"): calling hello operation" << endl;
	    hello->hello();
	    cout << "client request interceptor(" << this->name() <<"): call done" << endl;
	}
    }
    
    void send_poll(PortableInterceptor::ClientRequestInfo_ptr ri) {
    }
    
    void receive_reply( PortableInterceptor::ClientRequestInfo_ptr ri ) {
    }
    
    void receive_exception( PortableInterceptor::ClientRequestInfo_ptr ri ) {
    }
    
    void receive_other( PortableInterceptor::ClientRequestInfo_ptr ri ) {
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
	// geting slol id
	slot_id = info->allocate_slot_id();
    	// register interceptor	
    	MyClientInterceptor * interceptor = new MyClientInterceptor("MyInterceptor");
    	info->add_client_request_interceptor(interceptor);
	//MyClientInterceptor * interceptor2 = new MyClientInterceptor("MyInterceptor2");
	//info->add_client_request_interceptor(interceptor2);
	//  	MyClientInterceptor * interceptor3 = new MyClientInterceptor("MyInterceptor3");
	//  	info->add_client_request_interceptor(interceptor3);
    }
    
    virtual void post_init( PortableInterceptor::ORBInitInfo_ptr info ) {
    	// nothing
    }
};



int
main (int argc, char *argv[])
{
  MyInitializer * ini = new MyInitializer;
  PortableInterceptor::register_orb_initializer(ini);

  orb_g = CORBA::ORB_init (argc, argv);

  /*
   * IOR is in hello.ref in the local directory
   */

  char pwd[256], uri[300];
  sprintf (uri, "file://%s/hello.ref", getcwd(pwd, 256));

  /*
   * Bind to Hello World server
   */

  CORBA::Object_var obj = orb_g->string_to_object (uri);
  hello = HelloWorld::_narrow (obj);

  if (CORBA::is_nil (hello)) {
    cout << "oops: could not locate HelloWorld server" << endl;
    exit (1);
  }

  cout << "client main: calling hello operation" << endl;
  hello->hello ();
  cout << "client main: call done" << endl;
  return 0;
}
