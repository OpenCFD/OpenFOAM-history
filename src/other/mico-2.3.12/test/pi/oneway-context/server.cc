
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
  void hello (CORBA::Context_ptr context);
};

void
HelloWorld_impl::hello (CORBA::Context_ptr context)
{
    printf ("Hello World\n");
    CORBA::NVList_var vals;
    context->get_values("", 0, "*", vals);
    for (CORBA::ULong i = 0; i < vals->count(); i++) {
	const char *s;
	CORBA::Boolean r = (*vals->item(i)->value() >>= s);
	assert (r);
	cout << vals->item(i)->name() << ": " << s << endl;
    }
    // uncomment line below to test receive_exception interception points too
    //throw CORBA::NO_PERMISSION();
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
	// operation context is not valid in this interception point
    }

    virtual void
    receive_request(ServerRequestInfo_ptr ri)
    {
	cout << "receive_request" << endl;
	cout << "operation_context" << endl;
	try {
	    Dynamic::RequestContext* ctx = ri->operation_context();
	    for (CORBA::ULong i = 0; i < ctx->length(); i += 2) {
		cout << (*ctx)[i] << " : " << (*ctx)[i + 1] << endl;
	    }
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	}
	cout << "contexts" << endl;
	try {
	    Dynamic::ContextList* ctx_list = ri->contexts();
	    for (CORBA::ULong i = 0; i < ctx_list->length(); i += 2) {
		cout << (*ctx_list)[i] << endl;
	    }
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	}
    }

    virtual void
    send_reply(ServerRequestInfo_ptr ri)
    {
	cout << "send_reply" << endl;
	cout << "operation_context" << endl;
	try {
	    Dynamic::RequestContext* ctx = ri->operation_context();
	    for (CORBA::ULong i = 0; i < ctx->length(); i += 2) {
		cout << (*ctx)[i] << " : " << (*ctx)[i + 1] << endl;
	    }
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	}
	cout << "contexts" << endl;
	try {
	    Dynamic::ContextList* ctx_list = ri->contexts();
	    for (CORBA::ULong i = 0; i < ctx_list->length(); i += 2) {
		cout << (*ctx_list)[i] << endl;
	    }
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	}
    }

    virtual void
    send_exception(ServerRequestInfo_ptr ri)
    {
	cout << "send_exception" << endl;
	// operation context is not valid in this interception point
	cout << "operation_context is not valid here" << endl;
	cout << "contexts" << endl;
	try {
	    Dynamic::ContextList* ctx_list = ri->contexts();
	    for (CORBA::ULong i = 0; i < ctx_list->length(); i += 2) {
		cout << (*ctx_list)[i] << endl;
	    }
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	}	
    }

    virtual void
    send_other(ServerRequestInfo_ptr ri)
    {
	cout << "send_other" << endl;
	// operation context is not valid in this interception point
	cout << "operation_context is not valid here" << endl;
	cout << "contexts" << endl;
	try {
	    Dynamic::ContextList* ctx_list = ri->contexts();
	    for (CORBA::ULong i = 0; i < ctx_list->length(); i += 2) {
		cout << (*ctx_list)[i] << endl;
	    }
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
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
