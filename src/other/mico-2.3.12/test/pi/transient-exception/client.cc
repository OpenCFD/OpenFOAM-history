
#include "bench.h"
#include <CORBA.h>
#include <iostream>
#include <string>
#include <unistd.h>


using namespace std;

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
	
    char*
    name()
    { return CORBA::string_dup(nm.c_str()); }

    void destroy()
    { cerr << this->name() << " destroy" << endl; }

    void
    send_request(PortableInterceptor::ClientRequestInfo_ptr ri)
    {}
    
    void
    send_poll(PortableInterceptor::ClientRequestInfo_ptr ri)
    {}
    
    void
    receive_reply(PortableInterceptor::ClientRequestInfo_ptr ri)
    {}
    
    void
    receive_exception(PortableInterceptor::ClientRequestInfo_ptr ri)
    {
	cout << "receive exception: " << ri->received_exception_id() << endl;
    }
    
    void
    receive_other(PortableInterceptor::ClientRequestInfo_ptr ri)
    {}
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
    	// register interceptor	
    	MyClientInterceptor * interceptor = new MyClientInterceptor("MyInterceptor");
    	info->add_client_request_interceptor(interceptor);
    }
    
    virtual void
    post_init(PortableInterceptor::ORBInitInfo_ptr info)
    {}
};

int
main (int argc, char* argv[])
{
    MyInitializer * ini = new MyInitializer;
    PortableInterceptor::register_orb_initializer(ini);
    CORBA::ORB_ptr orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

    char pwd[256], uri[300];
    sprintf (uri, "file://%s/ref", getcwd(pwd, 256));

    CORBA::Object_var obj = orb->string_to_object(uri);
    bench_var bench = bench::_narrow(obj);
    if (CORBA::is_nil(bench)) {
	cout << "oops: could not locate bench server" << endl;
	exit (1);
    }
    try {
	for (int i=0; i<10000; i++) {
	    bench->perform();
	}
    } catch (CORBA::Exception &ex) {
	cout << ex._repoid() << endl;
    }
    return 0;
}

