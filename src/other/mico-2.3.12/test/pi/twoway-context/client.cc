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
	cout << "send_request" << endl;
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
    
    void send_poll(PortableInterceptor::ClientRequestInfo_ptr ri)
    {
	// operation context is not valid in this interception point
    }
    
    void receive_reply(PortableInterceptor::ClientRequestInfo_ptr ri)
    {
	cout << "receive_reply" << endl;
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
    
    void receive_exception(PortableInterceptor::ClientRequestInfo_ptr ri)
    {
	cout << "receive_exception" << endl;
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
    
    void receive_other(PortableInterceptor::ClientRequestInfo_ptr ri)
    {
	cout << "receive_other" << endl;
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

  CORBA::Any any;
  CORBA::Context_var ctx, ctx2;
  orb->get_default_context( ctx );

  ctx->create_child ("child", ctx2);

  any <<= "aa";
  ctx->set_one_value ("aa", any);

  any <<= "ab";
  ctx->set_one_value ("ab", any);

  any <<= "bb";
  ctx->set_one_value ("bb", any);

  any <<= "aa-child";
  ctx2->set_one_value ("aa", any);

  if (CORBA::is_nil (hello)) {
    printf ("oops: could not locate HelloWorld server\n");
    exit (1);
  }

  hello->hello (ctx2);
  return 0;
}
