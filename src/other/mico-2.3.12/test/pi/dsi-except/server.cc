
#include "hello.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS


using namespace std;
using namespace PortableInterceptor;
using namespace PortableServer;

CORBA::ORB_ptr orb_G;
CORBA::Long countx = 0;

//  class HelloWorld_impl : virtual public POA_HelloWorld
//  {
//  public:
//      void hello();
//  };

class HelloWorld_impl : virtual public DynamicImplementation
{
public:
    HelloWorld_impl(POA_ptr poa)
	: poa_(POA::_duplicate(poa))
    {}

    virtual void invoke(CORBA::ServerRequest_ptr svreq);

    virtual void
    hello();

    virtual char*
    _primary_interface
    (const PortableServer::ObjectId& oid,
     PortableServer::POA_ptr poa)
    { return CORBA::string_dup("IDL:HelloWorld:1.0"); }

    virtual PortableServer::POA_ptr
    _default_POA()
    { return POA::_duplicate(poa_); }
private:
    PortableServer::POA_var poa_;
};

void
HelloWorld_impl::invoke(CORBA::ServerRequest_ptr svreq)
{
    if (!strcmp (svreq->op_name(), "hello")) {
        // create argument list
        CORBA::NVList_ptr args;
        orb_G->create_list (0, args);
	try {
	    // get IN param values
	    svreq->arguments(args);
	    // call user code
	    this->hello();
	} catch (const CORBA::Exception& ex) {
	    CORBA::Any ax;
	    ax <<= ex;
	    svreq->set_exception(ax);
	} catch (...) {
	    CORBA::UNKNOWN ex;
	    CORBA::Any ax;
	    ax <<= ex;
	    svreq->set_exception(ax);
	}
    }
}

void
HelloWorld_impl::hello()
{
    cout << "server: Hello World" << endl;
    countx++;
    if (countx == 2) {
	cout << "server: throw NO_PERMISSION exception from hello operation" << endl;
	throw CORBA::NO_PERMISSION();
    }
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
	cout << "server(" << name_.in() << "): receive_request_service_contexts" << endl;
//  	if (strcmp(name_.in(), "HelloInterceptor1") == 0
//  	    && countx == 2) {
//  	    cout << "server(" << name_.in() << "): throw NO_RESOURCES exception" << endl;
//  	    mico_throw(CORBA::NO_RESOURCES());
//  	}
    }

    virtual void
    receive_request(ServerRequestInfo_ptr ri)
    {
	cout << "server(" << name_.in() << "): receive_request" << endl;
	if (strcmp(name_.in(), "HelloInterceptor3") == 0
	    && countx == 2) {
	    cout << "server(" << name_.in() << "): throw NO_RESOURCES exception" << endl;
	    mico_throw(CORBA::NO_RESOURCES());
	}
    }

    virtual void
    send_reply(ServerRequestInfo_ptr ri)
    {
	cout << "server(" << name_.in() << "): send_reply" << endl;
    }

    virtual void
    send_exception(ServerRequestInfo_ptr ri)
    {
	cout << "server(" << name_.in() << "): send_exception" << endl;
    }

    virtual void
    send_other(ServerRequestInfo_ptr ri)
    {
	cout << "server(" << name_.in() << "): send_other" << endl;
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
        HelloInterceptor* interceptor = new HelloInterceptor("HelloInterceptor1");
        info->add_server_request_interceptor(interceptor);
        HelloInterceptor* interceptor2 = new HelloInterceptor("HelloInterceptor2");
        info->add_server_request_interceptor(interceptor2);
        HelloInterceptor* interceptor3 = new HelloInterceptor("HelloInterceptor3");
        info->add_server_request_interceptor(interceptor3);
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

    HelloWorld_impl * hello = new HelloWorld_impl(poa);

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
