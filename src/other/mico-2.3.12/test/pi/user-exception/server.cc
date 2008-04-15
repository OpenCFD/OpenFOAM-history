
#include "bench.h"
#include <CORBA.h>
#include <fstream>


using namespace std;

CORBA::ORB_ptr orb = NULL;
CORBA::Long ip_count = 0;
CORBA::Boolean do_test = FALSE;

void
throw_by_state()
{
    switch(ip_count) {
    case 4:
	cout << "server: throw CORBA::NO_PERMISSION()" << endl;
	throw CORBA::NO_PERMISSION();
	break;
    case 6:
	cout << "server: throw CORBA::NO_PERMISSION()" << endl;
	throw CORBA::NO_PERMISSION();
	break;
    case 10:
	cout << "server: throw CORBA::NO_PERMISSION()" << endl;
	throw CORBA::NO_PERMISSION();
	break;
    case 16:
	cout << "server: throw CORBA::NO_PERMISSION()" << endl;
	throw CORBA::NO_PERMISSION();
	break;
    case 24:
	cout << "server: throw CORBA::NO_PERMISSION()" << endl;
	throw CORBA::NO_PERMISSION();
	break;
    case 33:
	cout << "server: throw CORBA::NO_PERMISSION()" << endl;
	throw CORBA::NO_PERMISSION();
	break;
    case 43:
	cout << "server: throw CORBA::NO_PERMISSION()" << endl;
	throw CORBA::NO_PERMISSION();
	break;
    case 53:
	cout << "server: throw CORBA::NO_PERMISSION()" << endl;
	throw CORBA::NO_PERMISSION();
	break;
    case 63:
	cout << "server: throw CORBA::NO_PERMISSION()" << endl;
	throw CORBA::NO_PERMISSION();
	break;
    case 64:
	cout << "server: throw Exc()" << endl;
	throw Exc();
	break; 
    case 66:
	cout << "server: throw Exc()" << endl;
	throw Exc();
	break;
    case 70:
	cout << "server: throw Exc()" << endl;
	throw Exc();
	break;
    case 76:
	cout << "server: throw Exc()" << endl;
	throw Exc();
	break;
    case 84:
	cout << "server: throw Exc()" << endl;
	throw Exc();
	break;
    case 93:
	cout << "server: throw Exc()" << endl;
	throw Exc();
	break;
    case 103:
	cout << "server: throw Exc()" << endl;
	throw Exc();
	break;
    case 113:
	cout << "server: throw Exc()" << endl;
	throw Exc();
	break;
    case 123:
	cout << "server: throw Exc()" << endl;
	throw Exc();
	break;
    case 124:
	cout << "server: throw 10" << endl;
	throw 10;
	break;
    case 126:
	cout << "server: throw 10" << endl;
	throw 10;
	break;
    case 130:
	cout << "server: throw 10" << endl;
	throw 10;
	break;
    case 136:
	cout << "server: throw 10" << endl;
	throw 10;
	break;
    case 144:
	cout << "server: throw 10" << endl;
	throw 10;
	break;
    case 153:
	cout << "server: throw 10" << endl;
	throw 10;
	break;
    case 163:
	cout << "server: throw 10" << endl;
	throw 10;
	break;
    case 173:
	cout << "server: throw 10" << endl;
	throw 10;
	break;
    case 183:
	cout << "server: throw 10" << endl;
	throw 10;
	break;
    }
}


class bench_impl
    : public virtual POA_bench
{
public:
    void
    perform()
    {
	throw_by_state();
    }

    void
    start_test()
    {
	cout << "server: start server testing..." << endl;
	do_test = TRUE;
    }
};


class MyServerInterceptor
    : virtual public PortableInterceptor::ServerRequestInterceptor,
      virtual public CORBA::LocalObject
{
    string nm;
        
public:
    MyServerInterceptor()
    { nm = ""; }
    MyServerInterceptor(char * name)
    { nm = name; }
        
    char*
    name()
    { return (char *)nm.c_str(); }

    void
    destroy()
    { cerr << this->name() << " destroy" << endl; }

    void
    receive_request_service_contexts(PortableInterceptor::ServerRequestInfo_ptr ri)
    {
	cout << this->name() << "::receive_request_service_contexts" << endl;
	if (do_test) {
	    ip_count++;
	    throw_by_state();
	}
    }
    
    void
    receive_request(PortableInterceptor::ServerRequestInfo_ptr ri)
    {
	cout << this->name() << "::receive_request" << endl;
	if (do_test) {
	    ip_count++;
	    throw_by_state();
	}
    }
    
    void
    send_reply(PortableInterceptor::ServerRequestInfo_ptr ri)
    {
	cout << this->name() << "::send_reply" << endl;
	if (do_test) {
	    ip_count++;
	    throw_by_state();
	}
    }
    
    void
    send_exception(PortableInterceptor::ServerRequestInfo_ptr ri)
    {
	cout << this->name() << "::send_exception" << endl;
	if (do_test) {
	    ip_count++;
	    throw_by_state();
	}
    }
    
    void
    send_other(PortableInterceptor::ServerRequestInfo_ptr ri)
    {
	cout << this->name() << "::send_other" << endl;
	if (do_test) {
	    ip_count++;
	    throw_by_state();
	}
    }
        
};

class MyInitializer
    : virtual public PortableInterceptor::ORBInitializer,
      virtual public CORBA::LocalObject
{
public:
    MyInitializer()
    {}
    ~MyInitializer()
    {}

    virtual void
    pre_init(PortableInterceptor::ORBInitInfo_ptr info)
    {
        MyServerInterceptor* interceptor1 = new MyServerInterceptor("MyServerInterceptor1");
        info->add_server_request_interceptor(interceptor1);
	MyServerInterceptor* interceptor2 = new MyServerInterceptor("MyServerInterceptor2");
	info->add_server_request_interceptor(interceptor2);
	MyServerInterceptor* interceptor3 = new MyServerInterceptor("MyServerInterceptor3");
	info->add_server_request_interceptor(interceptor3);
    }
    
    virtual void
    post_init(PortableInterceptor::ORBInitInfo_ptr info)
    {}
};


int
main (int argc, char* argv[])
{
    //  cerr << "main thread: " << MICOMT::Thread::self() << endl;
    MyInitializer* ini = new MyInitializer;
    PortableInterceptor::register_orb_initializer(ini);
    orb = CORBA::ORB_init (argc, argv, "mico-local-orb");
    CORBA::Object_ptr obj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_ptr poa = PortableServer::POA::_narrow (obj);
    CORBA::release (obj);

    bench_impl* servant = new bench_impl;

    poa->activate_object (servant);
    CORBA::Object_ptr ref = poa->servant_to_reference (servant);
  
    CORBA::String_var ior = orb->object_to_string (ref);
    ofstream out("ref");
    out << ior << endl;
    out.close();

    CORBA::release (ref);

    PortableServer::POAManager_ptr manager = poa->the_POAManager ();
    manager->activate ();

    orb->run ();

    CORBA::release (manager);
    CORBA::release (poa);
    CORBA::release (orb);

    return 0;
}
