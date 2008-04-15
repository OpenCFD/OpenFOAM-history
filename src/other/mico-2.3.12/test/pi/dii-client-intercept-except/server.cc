
#include "hello.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS


using namespace std;
using namespace PortableInterceptor;

CORBA::ORB_ptr orb_G;
CORBA::Long countx = 0;

class HelloWorld_impl : virtual public POA_HelloWorld
{
public:
    void hello();
    void oneway_hello();
};

void
HelloWorld_impl::hello()
{
    cout << "server: Hello World" << endl;
    if (countx == 1)
	throw CORBA::NO_PERMISSION();
    countx++;
}

void
HelloWorld_impl::oneway_hello()
{
    cout << "server: oneway: Hello World" << endl;
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
	cout << "server: receive_request_service_contexts" << endl;
    }

    virtual void
    receive_request(ServerRequestInfo_ptr ri)
    {
	cout << "server: receive_request" << endl;
    }

    virtual void
    send_reply(ServerRequestInfo_ptr ri)
    {
	cout << "server: send_reply" << endl;
    }

    virtual void
    send_exception(ServerRequestInfo_ptr ri)
    {
	cout << "server: send_exception" << endl;
    }

    virtual void
    send_other(ServerRequestInfo_ptr ri)
    {
	cout << "server: send_other" << endl;
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
    ofstream of ("hello.ref");
    of << str.in() << endl;
    of.close ();

    cout << "Running." << endl;

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
