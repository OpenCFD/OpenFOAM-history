#include "hello.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef _WIN32
#include <direct.h>
#endif


using namespace std;

PortableInterceptor::SlotId slot_id;

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
    { cerr << this->name() << " destroy" << endl; }

    void send_request(PortableInterceptor::ClientRequestInfo_ptr ri)
    {
	cout << "client: send_request" << endl;
	cout << "valid operations" << endl;
	try {
	    ri->request_id();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->operation();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->arguments();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->exceptions();
	} catch (CORBA::NO_RESOURCES& ex) {
	    assert(ex.minor() == 1 && ex.completed() == CORBA::COMPLETED_NO);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->contexts();
	} catch (CORBA::NO_RESOURCES& ex) {
	    assert(ex.minor() == 1 && ex.completed() == CORBA::COMPLETED_NO);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->operation_context();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->response_expected();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->sync_scope();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->get_slot(slot_id);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->get_request_service_context(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::BAD_PARAM& ex) {
	    assert(ex.minor() == 23 && ex.completed() == CORBA::COMPLETED_NO);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->target();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->effective_target();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->effective_profile();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->get_effective_component(0);
	} catch (CORBA::BAD_PARAM& ex) {
	    assert(ex.minor() == 25 && ex.completed() == CORBA::COMPLETED_NO);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->get_effective_components(0);
	} catch (CORBA::BAD_PARAM& ex) {
	    assert(ex.minor() == 25 && ex.completed() == CORBA::COMPLETED_NO);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->get_request_policy(0);
	} catch (CORBA::INV_POLICY& ex) {
	    assert(ex.minor() == 1 && ex.completed() == CORBA::COMPLETED_NO);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    IOP::ServiceContext sc;
	    sc.context_id = 0;
	    sc.context_data.length(0);
	    ri->add_request_service_context(sc, FALSE);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	cout << "invalid operations" << endl;
	try {
	    ri->result();
	    assert(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(ex.minor() == 10 && ex.completed() == CORBA::COMPLETED_NO);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->reply_status();
	    assert(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(ex.minor() == 10 && ex.completed() == CORBA::COMPLETED_NO);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->forward_reference();
	    assert(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(ex.minor() == 10 && ex.completed() == CORBA::COMPLETED_NO);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->get_reply_service_context(0);
	    assert(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(ex.minor() == 10 && ex.completed() == CORBA::COMPLETED_NO);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->received_exception();
	    assert(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(ex.minor() == 10 && ex.completed() == CORBA::COMPLETED_NO);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->received_exception_id();
	    assert(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(ex.minor() == 10 && ex.completed() == CORBA::COMPLETED_NO);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
    }
    
    void send_poll(PortableInterceptor::ClientRequestInfo_ptr ri)
    {
	cout << "client: send_poll" << endl;
    }
    
    void receive_reply(PortableInterceptor::ClientRequestInfo_ptr ri)
    {
	cout << "client: receive_reply" << endl;
	cout << "valid operations" << endl;
	try {
	    ri->request_id();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->operation();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->arguments();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->exceptions();
	} catch (CORBA::NO_RESOURCES& ex) {
	    assert(ex.minor() == 1 && ex.completed() == CORBA::COMPLETED_YES);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->contexts();
	} catch (CORBA::NO_RESOURCES& ex) {
	    assert(ex.minor() == 1 && ex.completed() == CORBA::COMPLETED_YES);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->operation_context();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->result();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->response_expected();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->sync_scope();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->reply_status();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->get_slot(slot_id);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->get_request_service_context(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::BAD_PARAM& ex) {
	    assert(ex.minor() == 23 && ex.completed() == CORBA::COMPLETED_YES);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->get_reply_service_context(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->target();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->effective_target();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->effective_profile();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->get_effective_component(0);
	} catch (CORBA::BAD_PARAM& ex) {
	    assert(ex.minor() == 25 && ex.completed() == CORBA::COMPLETED_YES);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->get_effective_components(0);
	} catch (CORBA::BAD_PARAM& ex) {
	    assert(ex.minor() == 25 && ex.completed() == CORBA::COMPLETED_YES);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->get_request_policy(0);
	} catch (CORBA::INV_POLICY& ex) {
	    assert(ex.minor() == 1 && ex.completed() == CORBA::COMPLETED_YES);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	cout << "invalid operations" << endl;
	try {
	    ri->forward_reference();
	    assert(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(ex.minor() == 10 && ex.completed() == CORBA::COMPLETED_YES);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->received_exception();
	    assert(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(ex.minor() == 10 && ex.completed() == CORBA::COMPLETED_YES);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->received_exception_id();
	    assert(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(ex.minor() == 10 && ex.completed() == CORBA::COMPLETED_YES);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    IOP::ServiceContext sc;
	    sc.context_id = 0;
	    sc.context_data.length(0);
	    ri->add_request_service_context(sc, FALSE);
	    assert(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(ex.minor() == 10 && ex.completed() == CORBA::COMPLETED_YES);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
    }
    
    void receive_exception(PortableInterceptor::ClientRequestInfo_ptr ri)
    {
	cout << "client: receive_exception" << endl;
	cout << "valid operations" << endl;
	try {
	    ri->request_id();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->operation();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->exceptions();
	} catch (CORBA::NO_RESOURCES& ex) {
	    assert(ex.minor() == 1 && ex.completed() == CORBA::COMPLETED_YES);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->contexts();
	} catch (CORBA::NO_RESOURCES& ex) {
	    assert(ex.minor() == 1 && ex.completed() == CORBA::COMPLETED_YES);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->operation_context();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->response_expected();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->sync_scope();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->reply_status();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->get_slot(slot_id);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->get_request_service_context(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::BAD_PARAM& ex) {
	    assert(ex.minor() == 23 && ex.completed() == CORBA::COMPLETED_YES);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->get_reply_service_context(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->target();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->effective_target();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->effective_profile();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->received_exception();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->received_exception_id();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->get_effective_component(0);
	} catch (CORBA::BAD_PARAM& ex) {
	    assert(ex.minor() == 25 && ex.completed() == CORBA::COMPLETED_YES);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->get_effective_components(0);
	} catch (CORBA::BAD_PARAM& ex) {
	    assert(ex.minor() == 25 && ex.completed() == CORBA::COMPLETED_YES);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->get_request_policy(0);
	} catch (CORBA::INV_POLICY& ex) {
	    assert(ex.minor() == 1 && ex.completed() == CORBA::COMPLETED_YES);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	cout << "invalid operations" << endl;
	try {
	    ri->arguments();
	    assert(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(ex.minor() == 10 && ex.completed() == CORBA::COMPLETED_YES);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->result();
	    assert(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(ex.minor() == 10 && ex.completed() == CORBA::COMPLETED_YES);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->forward_reference();
	    assert(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(ex.minor() == 10 && ex.completed() == CORBA::COMPLETED_YES);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    IOP::ServiceContext sc;
	    sc.context_id = 0;
	    sc.context_data.length(0);
	    ri->add_request_service_context(sc, FALSE);
	    assert(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(ex.minor() == 10 && ex.completed() == CORBA::COMPLETED_YES);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
    }
    
    void receive_other(PortableInterceptor::ClientRequestInfo_ptr ri)
    {
	cout << "client: receive_other" << endl;
    }
};

class MyInitializer
    : virtual public PortableInterceptor::ORBInitializer,
      virtual public CORBA::LocalObject
{
public:
    MyInitializer() {}
    ~MyInitializer() {}

    virtual void pre_init(PortableInterceptor::ORBInitInfo_ptr info)
    {
    	// register interceptor	
	slot_id = info->allocate_slot_id();
    	MyClientInterceptor * interceptor = new MyClientInterceptor("MyInterceptor");
    	info->add_client_request_interceptor(interceptor);
	//MyClientInterceptor * interceptor2 = new MyClientInterceptor("MyInterceptor2");
	//info->add_client_request_interceptor(interceptor2);
	//  	MyClientInterceptor * interceptor3 = new MyClientInterceptor("MyInterceptor3");
	//  	info->add_client_request_interceptor(interceptor3);
    }
    
    virtual void post_init(PortableInterceptor::ORBInitInfo_ptr info)
    {
    	// nothing
    }
};


int
main (int argc, char *argv[])
{

    MyInitializer* ini = new MyInitializer;
    PortableInterceptor::register_orb_initializer(ini);

    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    /*
     * IOR is in hello.ref in the local directory
     */

    char pwd[256], uri[300];
    sprintf (uri, "file://%s/hello.ref", getcwd(pwd, 256));

    /*
     * Bind to Hello World server
     */

    CORBA::Object_var obj = orb->string_to_object (uri);
    HelloWorld_var hello = HelloWorld::_narrow (obj);

    if (CORBA::is_nil (hello)) {
	cout << "client: oops: could not locate HelloWorld server" << endl;
	exit (1);
    }
    try {
	hello->hello();
	hello->hello();
    } catch (CORBA::Exception& ex) {
	cout << "client: caught: " << ex._repoid() << endl;
    }
    return 0;
}
