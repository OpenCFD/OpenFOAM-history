
#include "hello.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS


using namespace std;
using namespace PortableInterceptor;

CORBA::ORB_ptr orb_G;

class HelloWorld_impl : virtual public POA_HelloWorld
{
public:
  void hello ();
};

void
HelloWorld_impl::hello ()
{
    cout << "Hello World" << endl;
    CORBA::Object_ptr obj = orb_G->resolve_initial_references ("POACurrent");
    PortableServer::Current_var current = PortableServer::Current::_narrow(obj);
    if (CORBA::is_nil(current)) {
	cout << "current == NULL" << endl;
	return;
    }
    try {
	cout << "current->get_reference() ... ";
	CORBA::Object_ptr objx = current->get_reference();
	assert(!CORBA::is_nil(objx));
	cout << "OK!" << endl;
    } catch (PortableServer::Current::NoContext_catch& ex) {
	cout << "WRONG - caught NoContext();" << endl;
    }
    try {
	cout << "current->get_servant() ... ";
	HelloWorld_impl* t_hello = dynamic_cast<HelloWorld_impl*>(current->get_servant());
	assert(t_hello != NULL);
	cout << "OK!" << endl;
    } catch (PortableServer::Current::NoContext_catch& ex) {
	cout << "WRONG - caught NoContext();" << endl;
    }
    try {
	cout << "current->get_POA() ... ";
	PortableServer::POA_var t_poa = current->get_POA();
	assert(!CORBA::is_nil(t_poa));
	cout << "OK!" << endl;
    } catch (PortableServer::Current::NoContext_catch& ex) {
	cout << "WRONG - caught NoContext();" << endl;
    }
    try {
	cout << "current->get_object_id() ... ";
	PortableServer::ObjectId* t_oid = NULL;
	t_oid = current->get_object_id();
	assert(t_oid != NULL);
	cout << "OK!" << endl;
    } catch (PortableServer::Current::NoContext_catch& ex) {
	cout << "WRONG - caught NoContext();" << endl;
    }
    cout << "Finishing invocation" << endl;
//    mico_throw(CORBA::NO_PERMISSION());
//    PortableInterceptor::ForwardRequest fwd;
//    fwd.forward = HelloWorld::_nil();
//    fwd.permanent = FALSE;
//    mico_throw(fwd);
}

class HelloInterceptor
    : virtual public ServerRequestInterceptor,
      virtual public CORBA::LocalObject
{
    CORBA::String_var name_;
public:
    HelloInterceptor()
    { name_ = ""; }

    HelloInterceptor(const char* name)
    { name_ = CORBA::string_dup(name); }

    char*
    name()
    { return CORBA::string_dup(name_.in()); }

    void
    destroy()
    {}

    virtual void
    receive_request_service_contexts(ServerRequestInfo_ptr ri)
    {
	cout << "receive_request_service_contexts" << endl;
	CORBA::Object_ptr obj = orb_G->resolve_initial_references ("POACurrent");
	PortableServer::Current_var current = PortableServer::Current::_narrow(obj);
	if (CORBA::is_nil(current)) {
	    cout << "current == NULL" << endl;
	    return;
	}
	try {
	    cout << "current->get_reference() ... ";
	    CORBA::Object_ptr objx = current->get_reference();
	    cout << "WRONG!" << endl;
	} catch (PortableServer::Current::NoContext_catch& ex) {
	    cout << "OK - caught NoContext();" << endl;
	}
	try {
	    cout << "current->get_servant() ... ";
	    current->get_servant();
	    cout << "WRONG!" << endl;
	} catch (PortableServer::Current::NoContext_catch& ex) {
	    cout << "OK - caught NoContext();" << endl;
	}
	try {
	    cout << "current->get_POA() ... ";
	    current->get_POA();
	    cout << "WRONG!" << endl;
	} catch (PortableServer::Current::NoContext_catch& ex) {
	    cout << "OK - caught NoContext();" << endl;
	}
	try {
	    cout << "current->get_object_id() ... ";
	    current->get_object_id();
	    cout << "WRONG!" << endl;
	} catch (PortableServer::Current::NoContext_catch& ex) {
	    cout << "OK - caught NoContext();" << endl;
	}
    }

    virtual void
    receive_request(ServerRequestInfo_ptr ri)
    {
	// in receive_request interception point
	// we can access POACurrent, because it's already setuped
	cout << "receive_request" << endl;
	CORBA::Object_ptr obj = orb_G->resolve_initial_references ("POACurrent");
	PortableServer::Current_var current = PortableServer::Current::_narrow(obj);
	if (CORBA::is_nil(current)) {
	    cout << "current == NULL" << endl;
	    return;
	}
	try {
	    cout << "current->get_reference() ... ";
	    CORBA::Object_ptr objx = current->get_reference();
	    cout << "OK!" << endl;
	} catch (PortableServer::Current::NoContext_catch& ex) {
	    cout << "WRONG! - caught NoContext();" << endl;
	}
	try {
	    cout << "current->get_servant() ... ";
	    current->get_servant();
	    cout << "OK!" << endl;
	} catch (PortableServer::Current::NoContext_catch& ex) {
	    cout << "WRONG! - caught NoContext();" << endl;
	}
	try {
	    cout << "current->get_POA() ... ";
	    current->get_POA();
	    cout << "OK!" << endl;
	} catch (PortableServer::Current::NoContext_catch& ex) {
	    cout << "WRONG! - caught NoContext();" << endl;
	}
	try {
	    cout << "current->get_object_id() ... ";
	    current->get_object_id();
	    cout << "OK!" << endl;
	} catch (PortableServer::Current::NoContext_catch& ex) {
	    cout << "WRONG! - caught NoContext();" << endl;
	}
    }

    virtual void
    send_reply(ServerRequestInfo_ptr ri)
    {
	cout << "send_reply" << endl;
	CORBA::Object_ptr obj = orb_G->resolve_initial_references ("POACurrent");
	PortableServer::Current_var current = PortableServer::Current::_narrow(obj);
	if (CORBA::is_nil(current)) {
	    cout << "current == NULL" << endl;
	    return;
	}
	try {
	    cout << "current->get_reference() ... ";
	    CORBA::Object_ptr objx = current->get_reference();
	    cout << "WRONG!" << endl;
	} catch (PortableServer::Current::NoContext_catch& ex) {
	    cout << "OK - caught NoContext();" << endl;
	}
	try {
	    cout << "current->get_servant() ... ";
	    current->get_servant();
	    cout << "WRONG!" << endl;
	} catch (PortableServer::Current::NoContext_catch& ex) {
	    cout << "OK - caught NoContext();" << endl;
	}
	try {
	    cout << "current->get_POA() ... ";
	    current->get_POA();
	    cout << "WRONG!" << endl;
	} catch (PortableServer::Current::NoContext_catch& ex) {
	    cout << "OK - caught NoContext();" << endl;
	}
	try {
	    cout << "current->get_object_id() ... ";
	    current->get_object_id();
	    cout << "WRONG!" << endl;
	} catch (PortableServer::Current::NoContext_catch& ex) {
	    cout << "OK - caught NoContext();" << endl;
	}
    }

    virtual void
    send_exception(ServerRequestInfo_ptr ri)
    {
	cout << "send_exception" << endl;
	CORBA::Object_ptr obj = orb_G->resolve_initial_references ("POACurrent");
	PortableServer::Current_var current = PortableServer::Current::_narrow(obj);
	if (CORBA::is_nil(current)) {
	    cout << "current == NULL" << endl;
	    return;
	}
	try {
	    cout << "current->get_reference() ... ";
	    CORBA::Object_ptr objx = current->get_reference();
	    cout << "WRONG!" << endl;
	} catch (PortableServer::Current::NoContext_catch& ex) {
	    cout << "OK - caught NoContext();" << endl;
	}
	try {
	    cout << "current->get_servant() ... ";
	    current->get_servant();
	    cout << "WRONG!" << endl;
	} catch (PortableServer::Current::NoContext_catch& ex) {
	    cout << "OK - caught NoContext();" << endl;
	}
	try {
	    cout << "current->get_POA() ... ";
	    current->get_POA();
	    cout << "WRONG!" << endl;
	} catch (PortableServer::Current::NoContext_catch& ex) {
	    cout << "OK - caught NoContext();" << endl;
	}
	try {
	    cout << "current->get_object_id() ... ";
	    current->get_object_id();
	    cout << "WRONG!" << endl;
	} catch (PortableServer::Current::NoContext_catch& ex) {
	    cout << "OK - caught NoContext();" << endl;
	}
    }

    virtual void
    send_other(ServerRequestInfo_ptr ri)
    {
	cout << "send_other" << endl;
	CORBA::Object_ptr obj = orb_G->resolve_initial_references ("POACurrent");
	PortableServer::Current_var current = PortableServer::Current::_narrow(obj);
	if (CORBA::is_nil(current)) {
	    cout << "current == NULL" << endl;
	    return;
	}
	try {
	    cout << "current->get_reference() ... ";
	    CORBA::Object_ptr objx = current->get_reference();
	    cout << "WRONG!" << endl;
	} catch (PortableServer::Current::NoContext_catch& ex) {
	    cout << "OK - caught NoContext();" << endl;
	}
	try {
	    cout << "current->get_servant() ... ";
	    current->get_servant();
	    cout << "WRONG!" << endl;
	} catch (PortableServer::Current::NoContext_catch& ex) {
	    cout << "OK - caught NoContext();" << endl;
	}
	try {
	    cout << "current->get_POA() ... ";
	    current->get_POA();
	    cout << "WRONG!" << endl;
	} catch (PortableServer::Current::NoContext_catch& ex) {
	    cout << "OK - caught NoContext();" << endl;
	}
	try {
	    cout << "current->get_object_id() ... ";
	    current->get_object_id();
	    cout << "WRONG!" << endl;
	} catch (PortableServer::Current::NoContext_catch& ex) {
	    cout << "OK - caught NoContext();" << endl;
	}
    }
};

class HelloInitializer
    : virtual public ORBInitializer,
      virtual public CORBA::LocalObject
{
public:
    HelloInitializer()
    {}

    ~HelloInitializer()
    {}

    virtual void
    pre_init(ORBInitInfo_ptr info)
    {
        // register server interceptor(s)       
        HelloInterceptor* interceptor = new HelloInterceptor("HelloInterceptor");
        info->add_server_request_interceptor(interceptor);
    }
    
    virtual void
    post_init(ORBInitInfo_ptr info)
    {
        // nothing
    }
};



int
main (int argc, char *argv[])
{
    HelloInitializer* init = new HelloInitializer;
    PortableInterceptor::register_orb_initializer(init);

    orb_G = CORBA::ORB_init (argc, argv);

    CORBA::Object_var poaobj = orb_G->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
    PortableServer::POAManager_var mgr = poa->the_POAManager();

    HelloWorld_impl * hello = new HelloWorld_impl;

    PortableServer::ObjectId_var oid = poa->activate_object (hello);

    CORBA::Object_var ref = poa->id_to_reference (oid.in());
    CORBA::String_var str = orb_G->object_to_string (ref.in());
    //cout << str.in() << endl;
    ofstream of ("hello.ref");
    of << str.in() << endl;
    of.close ();

    printf ("Running.\n");

    mgr->activate ();

//      CORBA::Object_var obj = orb->string_to_object (str);
//      HelloWorld_var h = HelloWorld::_narrow (obj);

//      if (CORBA::is_nil (h)) {
//  	printf ("oops: could not locate HelloWorld server\n");
//  	exit (1);
//      }

//      h->hello ();

    orb_G->run();
    poa->destroy (TRUE, TRUE);
    delete hello;

    return 0;
}
