
#include "hello.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS

#include <mico/poa_impl.h>

using namespace std;
using namespace PortableInterceptor;
using namespace PortableServer;

CORBA::Long countx = 0;

class HelloWorld_impl : virtual public POA_HelloWorld
{
public:
    void hello();
};

void
HelloWorld_impl::hello()
{
    cout << "server: Hello World" << endl;
    if (countx == 1)
	throw CORBA::NO_PERMISSION();
    countx++;
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
        CORBA::String_var name = ri->operation();
        CORBA::String_var type = ri->target_most_derived_interface();
        cout << "server: receive_request: " << name.in() << " executed on: " << type.in() << endl;
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

class HelloLocator : virtual public PortableServer::ServantLocator
{
public:
    HelloLocator();

    virtual Servant
    preinvoke(const ObjectId& oid, POA_ptr adapter, const char* operation, Cookie_out the_cookie);

    virtual void
    postinvoke(const ObjectId& oid, POA_ptr adapter, const char* operation, Cookie the_cookie, Servant the_servant);
private:
    HelloWorld_impl* servant_;
};

HelloLocator::HelloLocator()
    : servant_(NULL)
{
}

Servant
HelloLocator::preinvoke
(const ObjectId& oid,
 POA_ptr adapter,
 const char* operation,
 Cookie_out the_cookie)
{
    CORBA::String_var str_oid = ObjectId_to_string(oid);
    MICOPOA::POA_impl* poa_impl = dynamic_cast<MICOPOA::POA_impl*>(adapter);
    assert(poa_impl != NULL);
    cout << "server: locator preinvoke: oid: `" << str_oid.in()
         << "', operation: `" << operation << "'" << endl;
    if (this->servant_ == NULL) {
        servant_ = new HelloWorld_impl;
    }
    return this->servant_;
}

void
HelloLocator::postinvoke
(const ObjectId& oid,
 POA_ptr adapter,
 const char* operation,
 Cookie the_cookie,
 Servant the_servant)
{
    CORBA::String_var str_oid = ObjectId_to_string(oid);
    MICOPOA::POA_impl* poa_impl = dynamic_cast<MICOPOA::POA_impl*>(adapter);
    assert(poa_impl != NULL);
    cout << "server: locator postinvoke: oid: `" << str_oid.in()
         << "', operation: `" << operation << endl;
}

int
main (int argc, char *argv[])
{
    HelloInitializer* init = new HelloInitializer;
    PortableInterceptor::register_orb_initializer(init);

    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
    PortableServer::POAManager_var mgr = poa->the_POAManager();

    CORBA::PolicyList policies;
    CORBA::ULong current_length = 0;

    policies.length (current_length + 1);
    policies[current_length++] =
        poa->create_request_processing_policy(PortableServer::USE_SERVANT_MANAGER);
    policies.length (current_length + 1);
    policies[current_length++] =
        poa->create_servant_retention_policy(PortableServer::NON_RETAIN);
    policies.length (current_length + 1);
    policies[current_length++] =
        poa->create_id_assignment_policy(PortableServer::USER_ID);

    PortableServer::POA_var child_poa = poa->create_POA("child", mgr, policies);

    HelloLocator* locator = new HelloLocator;
    child_poa->set_servant_manager(locator);

    ObjectId_var oid = string_to_ObjectId("hello");
    CORBA::Object_var hello = child_poa->create_reference_with_id(oid, "IDL:HelloWorld:1.0");

    CORBA::String_var str = orb->object_to_string (hello);
    ofstream of ("hello.ref");
    of << str.in() << endl;
    of.close ();

    cout << "Running." << endl;

    mgr->activate ();

    orb->run();
    poa->destroy (TRUE, TRUE);

    return 0;
}
