#include "hello.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef _WIN32
#include <direct.h>
#endif


bool inside_orb_init = false;

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
        if (inside_orb_init) {
            cout << "client: send_request invoked inside ORB_init!" << endl;
            assert(0);
        }
	cout << "client: send_request" << endl;
    }
    
    void send_poll(PortableInterceptor::ClientRequestInfo_ptr ri)
    {
        if (inside_orb_init) {
            cout << "client: send_poll invoked inside ORB_init!" << endl;
            assert(0);
        }
	cout << "client: send_poll" << endl;
    }
    
    void receive_reply(PortableInterceptor::ClientRequestInfo_ptr ri)
    {
        if (inside_orb_init) {
            cout << "client: receive_reply invoked inside ORB_init!" << endl;
            assert(0);
        }
	cout << "client: receive_reply" << endl;
    }
    
    void receive_exception(PortableInterceptor::ClientRequestInfo_ptr ri)
    {
        if (inside_orb_init) {
            cout << "client: receive_exception invoked inside ORB_init!" << endl;
            assert(0);
        }
	cout << "client: receive_exception" << endl;
    }
    
    void receive_other(PortableInterceptor::ClientRequestInfo_ptr ri)
    {
        if (inside_orb_init) {
            cout << "client: receive_other invoked inside ORB_init!" << endl;
            assert(0);
        }
	cout << "client: receive_other" << endl;
    }
};

class MyInitializer
    : virtual public PortableInterceptor::ORBInitializer,
      virtual public CORBA::LocalObject
{
public:
    MyInitializer(const string& ref)
        : ref_(ref)
    {}
    virtual
    ~MyInitializer()
    {}

    virtual void pre_init(PortableInterceptor::ORBInitInfo_ptr info)
    {
    	// register interceptor	
    	MyClientInterceptor * interceptor = new MyClientInterceptor("MyInterceptor");
    	info->add_client_request_interceptor(interceptor);
    }
    
    virtual void post_init(PortableInterceptor::ORBInitInfo_ptr info)
    {
        CORBA::ORB_var orb = CORBA::ORB_instance("mico-local-orb", false);
        assert(!CORBA::is_nil(orb));
        CORBA::Object_var obj = orb->string_to_object(ref_.c_str());
        hello_ = HelloWorld::_narrow(obj);
        assert(!CORBA::is_nil(hello_));
        hello_->hello();
    }

private:
    string ref_;
    HelloWorld_var hello_;
};


int
main (int argc, char *argv[])
{
    /*
     * IOR is in hello.ref in the local directory
     */

    char pwd[256], uri[300];
    sprintf (uri, "file://%s/hello.ref", getcwd(pwd, 256));

    MyInitializer* ini = new MyInitializer(uri);
    PortableInterceptor::register_orb_initializer(ini);
    inside_orb_init = true;
    cout << "before ORB_init..." << endl;
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
    cout << "after ORB_init..." << endl;
    inside_orb_init = false;
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
    } catch (CORBA::Exception& ex) {
	cout << "client: caught: " << ex._repoid() << endl;
    }
    return 0;
}
