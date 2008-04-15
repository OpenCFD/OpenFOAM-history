#define MICO_CONF_INTERCEPT
#include "account.h"

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
    { return (char *)nm.c_str(); }

    void destroy()
    { cout << this->name() << " destroy" << endl; }

    void send_request( PortableInterceptor::ClientRequestInfo_ptr ri ) {
    	char * op = ri->operation();
    	cout << name() << ": "<< "send_request, repoid = " << ri->target()->_repoid() << " operation = " << op << endl;
    	Dynamic::ParameterList * arg = ri->arguments();
    	if (arg->length() > 0) {
	    for (int i = 0; i < arg->length(); i++) {
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
    	for (int i = 0; i < ctx->length(); i += 2) {
	    cout << (*ctx)[i] << " : " << (*ctx)[i + 1] << endl;
    	}
    }
    
    void send_poll(PortableInterceptor::ClientRequestInfo_ptr ri) {
    }
    
    void receive_reply( PortableInterceptor::ClientRequestInfo_ptr ri ) {
    	char * op = ri->operation();
    	cout << name() << ": " << "receive_reply, repoid = "<< ri->target()->_repoid() << " operation = " << op << endl;

    	Dynamic::ParameterList * arg = ri->arguments();
    	if (arg->length() > 0) {
	    for (int i = 0; i < arg->length(); i++) {
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
    	for (int i = 0; i < ctx->length(); i += 2) {
	    cout << (*ctx)[i] << " : " << (*ctx)[i + 1] << endl;
    	}
    	
    }
    
    void receive_exception( PortableInterceptor::ClientRequestInfo_ptr ri ) {
    	
    	char * op = ri->operation();
    	cout << name() << ": " << "receive_exception, repoid = "<< ri->target()->_repoid() << " operation = " << op << endl;
    	char * exid = ri->received_exception_id();
    	CORBA::Any * ex = ri->received_exception();
    	cout << "exception ID = " << exid << endl;
	//    	mico_throw(CORBA::BAD_PARAM());
    }
    
    void receive_other( PortableInterceptor::ClientRequestInfo_ptr ri ) {
    	
    	cout << name() << ": " << "receive_other, repoid = "<< ri->target()->_repoid() << " operation = " << ri->operation() << endl;
    	cout << "exception ID = " << ri->received_exception_id() << endl;
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
    	MyClientInterceptor * interceptor2 = new MyClientInterceptor("MyInterceptor2");
    	info->add_client_request_interceptor(interceptor2);
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
    MyInitializer * ini = new MyInitializer;
    PortableInterceptor::register_orb_initializer(ini);
  
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    /*
     * IOR is in Bank.ref in the local directory
     */

    char pwd[256], uri[300];
    sprintf (uri, "file://%s/Bank.ref", getcwd(pwd, 256));

    /*
     * Connect to the Bank
     */

    CORBA::Object_var obj = orb->string_to_object (uri);
    Bank_var bank = Bank::_narrow (obj);

    if (CORBA::is_nil (bank)) {
	cout << "Client: oops: could not locate Bank" << endl;
	exit (1);
    }

    /*
     * Open an account
     */
    cout << "Client: create()" << endl;
    Account_var account = bank->create ();

    if (CORBA::is_nil (account)) {
	cout << "Client: oops: account is nil" << endl;
	exit (1);
    }

    /*
     * Deposit and withdraw some money
     */
    CORBA::ULong prev;
    cout << "Client: deposit()" << endl;
    account->deposit (700, prev);
    cout << "Client: withdraw()" << endl;
    try {
  	account->withdraw (450, prev);
    } catch (...) {
  	cout << "Client: Exception occured, try again..." << endl;
	cout << "Client: withdraw()" << endl;
  	account->withdraw (400, prev);
    }
	
    cout << "Client: balance()" << endl;
    CORBA::Long bal = account->balance ();
    cout << "Client: Balance is " << bal << "." << endl;
    bank->shutdown();
    cout << "client orb destroy" << endl;
    orb->destroy();
    sleep(4);
    return 0;
}
