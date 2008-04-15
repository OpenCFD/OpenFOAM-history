#include "hello.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef _WIN32
#include <direct.h>
#endif


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
	
    char* name()
    { return CORBA::string_dup(nm.c_str()); }

    void destroy()
    { cerr << this->name() << " destroy" << endl; }

    void send_request(PortableInterceptor::ClientRequestInfo_ptr ri)
    {
	cout << "client: send_request" << endl;
    }
    
    void send_poll(PortableInterceptor::ClientRequestInfo_ptr ri)
    {
	cout << "client: send_poll" << endl;
    }
    
    void receive_reply(PortableInterceptor::ClientRequestInfo_ptr ri)
    {
	cout << "client: receive_reply" << endl;
    }
    
    void receive_exception(PortableInterceptor::ClientRequestInfo_ptr ri)
    {
	cout << "client: receive_exception" << endl;
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
