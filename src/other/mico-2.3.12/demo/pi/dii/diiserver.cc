#include <CORBA.h>
#include <string.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif
//#include <mico/template_impl.h>


using namespace std;

class CalcImpl : virtual public PortableServer::DynamicImplementation
{
    CORBA::ORB_var orb_;
public:
    CalcImpl(CORBA::ORB_ptr orb);
    virtual void invoke(CORBA::ServerRequest_ptr svreq);
    virtual CORBA::Long add(CORBA::Long x, CORBA::Long y, CORBA::Context_ptr);

    virtual CORBA::RepositoryId _primary_interface
    (const PortableServer::ObjectId&,
     PortableServer::POA_ptr)
    {
	return CORBA::string_dup("IDL:Calc:1.0");
    }
};

CalcImpl::CalcImpl(CORBA::ORB_ptr orb)
{
    orb_ = CORBA::ORB::_duplicate(orb);
}

void
CalcImpl::invoke(CORBA::ServerRequest_ptr svreq)
{
    if (!strcmp (svreq->op_name(), "add")) {
	// create argument list
	CORBA::NVList_ptr args;
	orb_->create_list(0, args);

	// fill in NamedValue's
	// XXX must set TypeCode's, too ...
	args->add(CORBA::ARG_IN);
	args->item(0)->value()->set_type(CORBA::_tc_long);
	args->add (CORBA::ARG_IN);
	args->item(1)->value()->set_type(CORBA::_tc_long);
	args->add (CORBA::ARG_OUT);
	args->item(2)->value()->set_type(CORBA::_tc_string);

	// get IN param values
	svreq->params(args);

	// get parameters
	CORBA::Long x;
	*args->item(0)->value() >>= x;
	CORBA::Long y;
	*args->item(1)->value() >>= y;

	// call user code
	CORBA::Long z = this->add(x, y, svreq->ctx());

	// set result
	*args->item(2)->value() <<= "hello world";
	CORBA::Any *res = new CORBA::Any;
	*res <<= z;
	svreq->result(res);
    }
}

CORBA::Long
CalcImpl::add(CORBA::Long x, CORBA::Long y, CORBA::Context_ptr ctx)
{
    CORBA::NVList_var vals;
    ctx->get_values("", 0, "*", vals);
    for (CORBA::ULong i = 0; i < vals->count(); ++i) {
	const char *s;
	CORBA::Boolean r = (*vals->item(i)->value() >>= s);
	assert(r);
	cout << "Server: operation context - " << vals->item(i)->name() << ": " << s << endl;
    }
    return x + y;
}

// Portable server interceptor

class MyServerInterceptor
    : virtual public PortableInterceptor::ServerRequestInterceptor,
      virtual public CORBA::LocalObject
{
    string nm;
public:
    MyServerInterceptor() {
	nm = "";
    }
    MyServerInterceptor(char * name) {	
	nm = name;
    }
	
    char* name() {
	return (char *)nm.c_str();
    }

    void destroy()
    {}

    void receive_request_service_contexts( PortableInterceptor::ServerRequestInfo_ptr ri ) {
	cout << name() << ": " << "receive_request_service_contexts" << endl;
	cout << "Operation = " << ri->operation() << endl;
	//		mico_throw(CORBA::NO_IMPLEMENT());
    }
    
    void receive_request( PortableInterceptor::ServerRequestInfo_ptr ri ) {
	cout << name() << ": " << "receive_request" << endl;
	cout << "OA = ";
	CORBA::OctetSeq * oaid = ri->adapter_id();
	for (CORBA::ULong i = 0; i < oaid->length(); i++)
	    cout << (*oaid)[i];
	cout << endl;
	cout << "Interface = ";
	cout << ri->target_most_derived_interface() << " Operation = " << ri->operation() << endl;
	cout << "Parameters: ";
    	Dynamic::ParameterList * arg = ri->arguments();
    	if (arg->length() > 0) {
	    for (CORBA::ULong i = 0; i < arg->length(); i++) {
		if ((*arg)[i].mode == CORBA::PARAM_IN)
		    cout << " mode=" << "input";
		else if ((*arg)[i].mode == CORBA::PARAM_OUT)
		    cout << " mode=" << "out";
		else if ((*arg)[i].mode == CORBA::PARAM_INOUT)
		    cout << " mode=" << "inout";
		cout << " value=";
		CORBA::ULong v;
		CORBA::Long v2;
		const char * v3;
		CORBA::TypeCode_ptr t = (*arg)[i].argument.type();
		switch (t->unalias()->kind()) {
		case CORBA::tk_ulong :
		    (*arg)[i].argument >>= v;
		    cout << v;
		    break;
		case CORBA::tk_long :
		    (*arg)[i].argument >>= v2;
		    cout << v2;
		    break;
		case CORBA::tk_string :
		    (*arg)[i].argument >>= v3;
		    cout << v3;
		    break;
		}
	    }
	    cout << endl;
    	}
    	else 
	    cout << "operation without parameters!" << endl;
    	
    	cout << "operation context:" << endl;
    	Dynamic::RequestContext* ctx = ri->operation_context();
    	for (CORBA::ULong i = 0; i < ctx->length(); i += 2) {
	    cout << (*ctx)[i] << " : " << (*ctx)[i + 1] << endl;
    	}
	//		mico_throw(CORBA::NO_IMPLEMENT());
    }
    
    void send_reply( PortableInterceptor::ServerRequestInfo_ptr ri ) {
	cout << name() << ": " << "send_reply" << endl;
	cout << "OA = ";
	CORBA::OctetSeq * oaid = ri->adapter_id();
	for (CORBA::ULong i = 0; i < oaid->length(); i++)
	    cout << (*oaid)[i];
	cout << endl;
	cout << "Operation = " << ri->operation() << endl;
		
	cout << "Parameters: ";
    	Dynamic::ParameterList * arg = ri->arguments();
    	if (arg->length() > 0) {
	    for (CORBA::ULong i = 0; i < arg->length(); i++) {
		if ((*arg)[i].mode == CORBA::PARAM_IN)
		    cout << " mode=" << "input";
		else if ((*arg)[i].mode == CORBA::PARAM_OUT)
		    cout << " mode=" << "out";
		else if ((*arg)[i].mode == CORBA::PARAM_INOUT)
		    cout << " mode=" << "inout";
		cout << " value=";
		CORBA::ULong v;
		CORBA::Long v2;
		const char * v3;
		CORBA::TypeCode_ptr t = (*arg)[i].argument.type();
		switch (t->unalias()->kind()) {
		case CORBA::tk_ulong :
		    (*arg)[i].argument >>= v;
		    cout << v;
		    break;
		case CORBA::tk_long :
		    (*arg)[i].argument >>= v2;
		    cout << v2;
		    break;
		case CORBA::tk_string :
		    (*arg)[i].argument >>= v3;
		    cout << v3;
		    break;
		}
	    }
	    cout << endl;
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
	//		mico_throw(CORBA::NO_IMPLEMENT());    	
    }
    
    void send_exception( PortableInterceptor::ServerRequestInfo_ptr ri ) {
	cout << name() << ": " << "send_exception" << endl;
	CORBA::Any* ex = ri->sending_exception();
	CORBA::TypeCode_ptr tc = ex->type();
	const char * id = tc->id ();
	cout << "Exception ID = "<< id << endl;
	//		cout << "throw NO_PERMISSION" << endl;
	//		mico_throw(CORBA::NO_PERMISSION());
    }
    
    void send_other( PortableInterceptor::ServerRequestInfo_ptr ri ) {
	cout << name() << ": " << "send_other" << endl;
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
    	// register server interceptor(s)	
    	MyServerInterceptor * interceptor = new MyServerInterceptor("MyServerInterceptor");
    	info->add_server_request_interceptor(interceptor);
    	MyServerInterceptor * interceptor2 = new MyServerInterceptor("MyServerInterceptor2");
    	info->add_server_request_interceptor(interceptor2);
    }
    
    virtual void post_init( PortableInterceptor::ORBInitInfo_ptr info ) {
    	// nothing
    }
};
//

int
main (int argc, char *argv[])
{
    MyInitializer * ini = new MyInitializer;
    PortableInterceptor::register_orb_initializer(ini);

    CORBA::ORB_ptr orb = CORBA::ORB_init (argc, argv, "mico-local-orb");
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);
    PortableServer::POAManager_var mgr = poa->the_POAManager();

    CalcImpl* calc = new CalcImpl(orb);
    CORBA::Object_ptr oo = calc->_this();

    ofstream of("server.ior");
    of << orb->object_to_string(oo);
    of.close();

    mgr->activate();
    cout << "Running..." << endl;
    orb->run();
    return 0;
}
