#include <CORBA.h>
#include <string.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif
#include <mico/template_impl.h>
#include <mico/throw.h>
#include <unistd.h>


using namespace std;

class MyClientInterceptor
    : virtual public PortableInterceptor::ClientRequestInterceptor,
      virtual public CORBA::LocalObject
{
    string nm;
	
public:
	
    MyClientInterceptor() {
	nm = "";
    }
    MyClientInterceptor(char * name) {	
	nm = name;
    }
	
    char* name() {
	return (char *)nm.c_str();
    }

    void destroy()
    {}

    void send_request( PortableInterceptor::ClientRequestInfo_ptr ri ) {
    
    	char * op = ri->operation();
    	cout << name() << ": "<< "send_request, objectID = " << ri->target()->_repoid() << " operation = " << op << endl;
    	Dynamic::ParameterList * arg = ri->arguments();
    	if (arg->length() > 0) {
	    for (CORBA::ULong i = 0; i < arg->length(); i++) {
		if ((*arg)[i].mode == CORBA::PARAM_IN)
		    cout << "Parameter mode=" << "input" << endl;
		else if ((*arg)[i].mode == CORBA::PARAM_OUT)
		    cout << "Parameter mode=" << "out" << endl;
		else if ((*arg)[i].mode == CORBA::PARAM_INOUT)
		    cout << "Parameter mode=" << "inout" << endl;
		cout << "Parameter value=";
		CORBA::ULong v;
		CORBA::Long v2;
		const char * v3;
		CORBA::TypeCode_ptr t = (*arg)[i].argument.type();
		switch (t->unalias()->kind()) {
		case CORBA::tk_null :
		case CORBA::tk_void :
		    cout << endl;
		    break;
		case CORBA::tk_ulong :
		    (*arg)[i].argument >>= v;
		    cout << v << endl;
		    break;
		case CORBA::tk_long :
		    (*arg)[i].argument >>= v2;
		    cout << v2 << endl;
		    break;
		case CORBA::tk_string :
		    (*arg)[i].argument >>= v3;
		    cout << v3 << endl;
		    break;
		}
	    }
    	}
    	else 
	    cout << "operation without parameters!" << endl;
    	
    	cout << "operation context:" << endl;
    	Dynamic::RequestContext* ctx = ri->operation_context();
    	for (CORBA::ULong i = 0; i < ctx->length(); i += 2) {
	    cout << (*ctx)[i] << " : " << (*ctx)[i + 1] << endl;
    	}
    }
    
    void send_poll( PortableInterceptor::ClientRequestInfo_ptr ri ) {
	
	char * op = ri->operation();
	cout << name() << ": " << "send_poll, objectID = " << ri->target()->_repoid() << " operation = " << op << endl;
    }
    
    void receive_reply( PortableInterceptor::ClientRequestInfo_ptr ri ) {
    	
    	char * op = ri->operation();
    	cout << name() << ": " << "receive_reply, objectID = "<< ri->target()->_repoid() << " operation = " << op << endl;

    	Dynamic::ParameterList * arg = ri->arguments();
    	if (arg->length() > 0) {
	    for (CORBA::ULong i = 0; i < arg->length(); i++) {
		if ((*arg)[i].mode == CORBA::PARAM_IN)
		    cout << "Parameter mode=" << "input" << endl;
		else if ((*arg)[i].mode == CORBA::PARAM_OUT)
		    cout << "Parameter mode=" << "out" << endl;
		else if ((*arg)[i].mode == CORBA::PARAM_INOUT)
		    cout << "Parameter mode=" << "inout" << endl;
		cout << "Parameter value=";
		CORBA::ULong v;
		CORBA::Long v2;
		const char * v3;
		CORBA::TypeCode_ptr t = (*arg)[i].argument.type();
		switch (t->unalias()->kind()) {
		case CORBA::tk_null :
		case CORBA::tk_void :
		    cout << endl;
		    break;
		case CORBA::tk_ulong :
		    (*arg)[i].argument >>= v;
		    cout << v << endl;
		    break;
		case CORBA::tk_long :
		    (*arg)[i].argument >>= v2;
		    cout << v2 << endl;
		    break;
		case CORBA::tk_string :
		    (*arg)[i].argument >>= v3;
		    cout << v3 << endl;
		    break;
		}
	    }
    	}
    	else 
	    cout << "operation without parameters!" << endl;

    	cout <<"result = ";
    	CORBA::Any * res = ri->result();
	CORBA::ULong v;
	CORBA::Long v2;
	const char * v3;
	CORBA::TypeCode_ptr t = res->type();
	switch (t->unalias()->kind()) {
	case CORBA::tk_null :
	case CORBA::tk_void :
	    cout << endl;
	    break;
	case CORBA::tk_ulong :
	    *res >>= v;
	    cout << v << endl;
	    break;
	case CORBA::tk_long :
	    *res >>= v2;
	    cout << v2 << endl;
	    break;
	case CORBA::tk_string :
	    *res >>= v3;
	    cout << v3 << endl;
	    break;
	}
    	cout << "operation context:" << endl;
    	Dynamic::RequestContext* ctx = ri->operation_context();
    	for (CORBA::ULong i = 0; i < ctx->length(); i += 2) {
	    cout << (*ctx)[i] << " : " << (*ctx)[i + 1] << endl;
    	}
    	
    }
    
    void receive_exception( PortableInterceptor::ClientRequestInfo_ptr ri ) {
    	
    	char * op = ri->operation();
    	cout << name() << ": " << "receive_exception, objectID = "<< ri->target()->_repoid() << " operation = " << op << endl;
    	char * exid = ri->received_exception_id();
    	cout << "exception ID = " << exid << endl;
	//    	mico_throw(CORBA::BAD_PARAM());
    }
    
    void receive_other( PortableInterceptor::ClientRequestInfo_ptr ri ) {
    	
    	cout << name() << ": " << "receive_other, objectID = "<< ri->target()->_repoid() << " operation = " << ri->operation() << endl;
    }
};

class MyInitializer
    : virtual public PortableInterceptor::ORBInitializer,
      virtual public CORBA::LocalObject
{
public:
    MyInitializer() {}
    ~MyInitializer() {}

    virtual void pre_init( PortableInterceptor::ORBInitInfo_ptr info ) {
    	// register interceptor	
    	MyClientInterceptor * interceptor = new MyClientInterceptor("MyInterceptor");
    	info->add_client_request_interceptor(interceptor);
	//    	MyClientInterceptor * interceptor2 = new MyClientInterceptor("MyInterceptor2");
	//    	info->add_client_request_interceptor(interceptor2);
	//    	MyClientInterceptor * interceptor3 = new MyClientInterceptor("MyInterceptor3");
	//    	info->add_client_request_interceptor(interceptor3);
    }
    
    virtual void post_init( PortableInterceptor::ORBInitInfo_ptr info ) {
    	// nothing
    }
};


int
main (int argc, char *argv[])
{
    if (argc != 2) {
        cout << "usage: diiclient <ior>" << endl;
        exit (1);
    }

    MyInitializer * ini = new MyInitializer;
    PortableInterceptor::register_orb_initializer(ini);

    CORBA::ORB_ptr orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

    CORBA::Object_var obj = orb->string_to_object(argv[1]);
    CORBA::Any any;
    CORBA::Context_var ctx, ctx2;
    orb->get_default_context (ctx);

    ctx->create_child ("child", ctx2);

    any <<= "aa";
    ctx->set_one_value ("aa", any);

    any <<= "ab";
    ctx->set_one_value ("ab", any);

    any <<= "bb";
    ctx->set_one_value ("bb", any);

    any <<= "aa-child";
    ctx2->set_one_value ("aa", any);

    CORBA::Request_var req = obj->_request ("add");
    req->ctx (ctx2);
    req->contexts()->add ("a*");
    req->add_in_arg() <<= (CORBA::Long)1;
    req->add_in_arg() <<= (CORBA::Long)2;
    req->add_out_arg ();
    req->arguments()->item(2)->value()->set_type (CORBA::_tc_string);
    req->result()->value()->set_type (CORBA::_tc_long);

    //    req->invoke ();
    //    if (req->env()->exception())
    //	mico_throw (*req->env()->exception());
    req->send_deferred();
    req->poll_response();
    req->get_response();
    CORBA::Long z;
    *req->result()->value() >>= z;
    cout << "Client: result: " << z << endl;

    const char *str;
    *req->arguments()->item(2)->value() >>= str;
    cout << "Client: "<< str << endl;

    return 0;
}
