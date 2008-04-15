
#include "hello.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS


using namespace std;
using namespace PortableInterceptor;

class HelloWorld_impl : virtual public POA_HelloWorld
{
public:
    void hello();
};

void
HelloWorld_impl::hello()
{
    cout << "server: Hello World" << endl;
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

class HelloManager : virtual public POA_PortableServer::ServantActivator
{
    CORBA::Object_var forward_;
    public:
	HelloManager(CORBA::Object_ptr forward) 
	    : forward_(forward)
	    { }
	PortableServer::Servant incarnate(const PortableServer::ObjectId &,
					  PortableServer::POA_ptr);
	void etherealize(const PortableServer::ObjectId &,
			 PortableServer::POA_ptr,
			 PortableServer::Servant,
			 CORBA::Boolean, CORBA::Boolean);
};

PortableServer::Servant 
HelloManager::incarnate(const PortableServer::ObjectId &oid,
				  PortableServer::POA_ptr poa) 
{
    throw PortableServer::ForwardRequest(forward_);
    /*
    HelloWorld_impl *hello = new HelloWorld_impl;
    return hello;
    */
}

void 
HelloManager::etherealize(const PortableServer::ObjectId &oid,
		 PortableServer::POA_ptr poa,
		 PortableServer::Servant serv,
		 CORBA::Boolean cleanup_in_progress, 
		 CORBA::Boolean remaining_activations) 
{
    if (!remaining_activations) {
	delete serv;
    }
}

int
main (int argc, char *argv[])
{
    HelloInitializer* init = new HelloInitializer;
    PortableInterceptor::register_orb_initializer(init);

    CORBA::ORB_ptr orb = CORBA::ORB_init (argc, argv);

    CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
    PortableServer::POAManager_var mgr = poa->the_POAManager();

    /*
     * Create POA
     */
    CORBA::PolicyList pl;
    pl.length(3);
    pl[0] = poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER);
    pl[1] = poa->create_lifespan_policy (PortableServer::PERSISTENT);
    pl[2] = poa->create_id_assignment_policy (PortableServer::USER_ID);
    PortableServer::POA_var hello_poa = poa->create_POA ("Hello", PortableServer::POAManager::_nil(), pl);
    PortableServer::POAManager_var hello_mgr = hello_poa->the_POAManager();

    /* create HelloWorld reference */
    HelloWorld_impl *hello = new HelloWorld_impl;
    PortableServer::ObjectId_var hello_id = 
	PortableServer::string_to_ObjectId("trueHello");
    hello_poa->activate_object_with_id(hello_id.in(), hello);
    HelloWorld_var hello_var = hello->_this();
    /* 
     * Activate HelloManager
     */
    
    HelloManager *hm = new HelloManager(hello_var);
    PortableServer::ServantManager_var hmref = hm->_this();
    hello_poa->set_servant_manager(hmref);

    PortableServer::ObjectId_var oid 
	= PortableServer::string_to_ObjectId("Hello");
    CORBA::Object_var ref 
	= hello_poa->create_reference_with_id (oid.in(), "IDL:HelloWorld:1.0");

    CORBA::String_var str = orb->object_to_string (ref.in());
    ofstream of ("hello.ref");
    of << str.in() << endl;
    of.close ();

    cout << "Running." << endl;

    mgr->activate ();
    hello_mgr->activate();
    //orb->run();
    while (1) {
	//try {
	    orb->perform_work();
	//}
	//catch (CORBA::SystemException &ex) {
	//    cout << ex << endl;
	//}
	//catch (CORBA::UserException &ex) {
	//    cout << ex << endl;
	//}
	//catch(...) {
	//    cout << "unknown exception caught" << endl;
//	}
    }
    poa->destroy (TRUE, TRUE);
    delete hm;
    delete init;

    return 0;
}
