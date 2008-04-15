
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
PortableInterceptor::SlotId slot_id;
bool internal_call = false;

class HelloWorld_impl : virtual public POA_HelloWorld
{
public:
    void hello();
};

void
HelloWorld_impl::hello()
{
    cout << "server: Hello World" << endl;
    if (countx == 1) {
	//throw HelloWorld::Exc();
	throw CORBA::NO_PERMISSION(0, CORBA::COMPLETED_YES);
    }
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
	if (internal_call)
	    return;
	cout << "server: receive_request_service_contexts" << endl;
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
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->get_server_policy(0);
	} catch (CORBA::INV_POLICY& ex) {
	    assert(ex.minor() == 2 && ex.completed() == CORBA::COMPLETED_NO);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    CORBA::Any any;
	    CORBA::String_var str = "Hello";
	    any <<= str;
	    ri->set_slot(slot_id, any);
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
	    ri->add_reply_service_context(sc, FALSE);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    // ex.minor() == 11 => not replace previous sc
	    assert(ex.minor() == 11 && ex.completed() == CORBA::COMPLETED_NO);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	cout << "invalid operations" << endl;
	try {
	    ri->arguments();
	    assert(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(ex.minor() == 10 && ex.completed() == CORBA::COMPLETED_NO);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->exceptions();
	    assert(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(ex.minor() == 10 && ex.completed() == CORBA::COMPLETED_NO);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->contexts();
	    assert(0);
	} catch (CORBA::NO_RESOURCES& ex) {
	    assert(ex.minor() == 1 && ex.completed() == CORBA::COMPLETED_NO);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(ex.minor() == 10 && ex.completed() == CORBA::COMPLETED_NO);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->operation_context();
	    assert(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(ex.minor() == 10 && ex.completed() == CORBA::COMPLETED_NO);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
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
	    ri->sending_exception();
	    assert(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(ex.minor() == 10 && ex.completed() == CORBA::COMPLETED_NO);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->object_id();
	    assert(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(ex.minor() == 10 && ex.completed() == CORBA::COMPLETED_NO);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->adapter_id();
	    assert(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(ex.minor() == 10 && ex.completed() == CORBA::COMPLETED_NO);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->target_most_derived_interface();
	    assert(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(ex.minor() == 10 && ex.completed() == CORBA::COMPLETED_NO);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->target_is_a("IDL:org.omg/CORBA/Object:1.0");
	    assert(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(ex.minor() == 10 && ex.completed() == CORBA::COMPLETED_NO);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
    }

    virtual void
    receive_request(ServerRequestInfo_ptr ri)
    {
	if (internal_call)
	    return;
	cout << "server: receive_request" << endl;
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
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->object_id();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->adapter_id();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->target_most_derived_interface();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->get_server_policy(0);
	} catch (CORBA::INV_POLICY& ex) {
	    assert(ex.minor() == 2 && ex.completed() == CORBA::COMPLETED_NO);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    CORBA::Any any;
	    CORBA::String_var str = "Hello";
	    any <<= str;
	    ri->set_slot(slot_id, any);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    internal_call = true;
	    ri->target_is_a("IDL:org.omg/CORBA/Object:1.0");
	    internal_call = false;
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
	    ri->add_reply_service_context(sc, FALSE);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    // 11 is valid for not replacing previous sc
	    assert(ex.minor() == 11 && ex.completed() == CORBA::COMPLETED_NO);
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
	    ri->sending_exception();
	    assert(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(ex.minor() == 10 && ex.completed() == CORBA::COMPLETED_NO);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
    }

    virtual void
    send_reply(ServerRequestInfo_ptr ri)
    {
	if (internal_call)
	    return;
	cout << "server: send_reply" << endl;
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
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->get_reply_service_context(0);
	} catch (CORBA::BAD_PARAM& ex) {
	    assert(ex.minor() == 23 && ex.completed() == CORBA::COMPLETED_YES);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->object_id();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->adapter_id();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->get_server_policy(0);
	} catch (CORBA::INV_POLICY& ex) {
	    assert(ex.minor() == 2 && ex.completed() == CORBA::COMPLETED_YES);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    CORBA::Any any;
	    CORBA::String_var str = "Hello";
	    any <<= str;
	    ri->set_slot(slot_id, any);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    internal_call = true;
	    ri->target_is_a("IDL:org.omg/CORBA/Object:1.0");
	    internal_call = false;
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
	    ri->add_reply_service_context(sc, FALSE);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    // 11 is valid for not replacing previous sc
	    assert(ex.minor() == 11 && ex.completed() == CORBA::COMPLETED_YES);
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
	    ri->sending_exception();
	    assert(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(ex.minor() == 10 && ex.completed() == CORBA::COMPLETED_YES);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->target_most_derived_interface();
	    assert(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(ex.minor() == 10 && ex.completed() == CORBA::COMPLETED_YES);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
    }

    virtual void
    send_exception(ServerRequestInfo_ptr ri)
    {
	if (internal_call)
	    return;
	cout << "server: send_exception" << endl;
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
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->get_reply_service_context(0);
	} catch (CORBA::BAD_PARAM& ex) {
	    assert(ex.minor() == 23 && ex.completed() == CORBA::COMPLETED_YES);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->sending_exception();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->object_id();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->adapter_id();
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    ri->get_server_policy(0);
	} catch (CORBA::INV_POLICY& ex) {
	    assert(ex.minor() == 2 && ex.completed() == CORBA::COMPLETED_YES);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    CORBA::Any any;
	    CORBA::String_var str = "Hello";
	    any <<= str;
	    ri->set_slot(slot_id, any);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(0);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
	try {
	    internal_call = true;
	    ri->target_is_a("IDL:org.omg/CORBA/Object:1.0");
	    internal_call = false;
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
	    ri->add_reply_service_context(sc, FALSE);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    // 11 is valid for not replacing previous sc
	    assert(ex.minor() == 11 && ex.completed() == CORBA::COMPLETED_YES);
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
	    ri->operation_context();
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
	    ri->target_most_derived_interface();
	    assert(0);
	} catch (CORBA::BAD_INV_ORDER& ex) {
	    assert(ex.minor() == 10 && ex.completed() == CORBA::COMPLETED_YES);
	} catch (CORBA::Exception& ex) {
	    cout << ex._repoid() << endl;
	    assert(0);
	}
    }

    virtual void
    send_other(ServerRequestInfo_ptr ri)
    {
	if (internal_call)
	    return;
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
	slot_id = info->allocate_slot_id();
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
