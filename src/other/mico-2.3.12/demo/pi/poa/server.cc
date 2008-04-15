/*
 * A Bank factory that creates Account objects
 */
#define MICO_CONF_INTERCEPT
#include "account.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else
#include <fstream.h>
#endif


using namespace std;

CORBA::ORB_ptr orb = CORBA::ORB::_nil();
CORBA::Boolean finished = FALSE;

/*
 * Implementation of the Account
 */

class Account_impl : virtual public POA_Account
{
public:
    Account_impl ();

    void deposit (CORBA::ULong, CORBA::ULong_out prev);
    void withdraw (CORBA::ULong, CORBA::ULong_out prev);
    CORBA::Long balance ();

private:
    CORBA::Long bal;
};

Account_impl::Account_impl ()
{
    bal = 0;
}

void
Account_impl::deposit (CORBA::ULong amount, CORBA::ULong_out prev)
{
    prev = bal;
    bal += amount;
}

void
Account_impl::withdraw (CORBA::ULong amount, CORBA::ULong_out prev)
{
    prev = bal;
    if (amount > 400) {
	mico_throw(Account::BigAmount());
	return;
    }
    bal -= amount;
}

CORBA::Long
Account_impl::balance ()
{
    return bal;
}

/*
 * Implementation of the Bank
 */

class Bank_impl : virtual public POA_Bank
{
public:
    Account_ptr create ();
    void shutdown();
};

Account_ptr
Bank_impl::create ()
{
    /*
     * Create a new account (which is never deleted)
     */

    Account_impl * ai = new Account_impl;

    /*
   * Obtain a reference using _this. This implicitely activates the
   * account servant (the RootPOA, which is the object's _default_POA,
   * has the IMPLICIT_ACTIVATION policy)
   */

    Account_ptr aref = ai->_this ();
    assert (!CORBA::is_nil (aref));

  /*
   * Return the reference
   */

    return aref;
}

void
Bank_impl::shutdown()
{
    finished = TRUE;
}

//////////////////////////////////////////////////
// Request interceptor

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
    { cout << this->name() << " destroy" << endl; }

    void receive_request_service_contexts( PortableInterceptor::ServerRequestInfo_ptr ri ) {
	cout << name() << ": " << "receive_request_service_contexts" << endl;
	cout << "Operation = " << ri->operation() << endl;
	//		mico_throw(CORBA::NO_IMPLEMENT());
    }
    
    void receive_request( PortableInterceptor::ServerRequestInfo_ptr ri ) {
	cout << name() << ": " << "receive_request" << endl;
	cout << "OA = ";
	CORBA::OctetSeq * oaid = ri->adapter_id();
	for (int i = 0; i < oaid->length(); i++)
	    cout << (*oaid)[i];
	cout << endl;
	cout << "ObjectID = `";
	CORBA::OctetSeq* oid = ri->object_id();
	for (int i = 0; i < oid->length(); i++)
	    cout << (*oid)[i];
	cout << "' Interface = ";
	cout << ri->target_most_derived_interface() << " Operation = " << ri->operation() << endl;
	cout << "Parameters: ";
    	Dynamic::ParameterList * arg = ri->arguments();
    	if (arg->length() > 0) {
	    for (int i = 0; i < arg->length(); i++) {
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
    	for (int i = 0; i < ctx->length(); i += 2) {
	    cout << (*ctx)[i] << " : " << (*ctx)[i + 1] << endl;
    	}
	//		mico_throw(CORBA::NO_IMPLEMENT());
    }
    
    void send_reply( PortableInterceptor::ServerRequestInfo_ptr ri ) {
	cout << name() << ": " << "send_reply" << endl;
	cout << "OA = ";
	CORBA::OctetSeq * oaid = ri->adapter_id();
	for (int i = 0; i < oaid->length(); i++)
	    cout << (*oaid)[i];
	cout << endl;
	cout << "ObjectID = `";
	CORBA::OctetSeq* oid = ri->object_id();
	for (int i = 0; i < oid->length(); i++)
	    cout << (*oid)[i];
	cout << "'" << endl;
	cout << "Parameters: ";
    	Dynamic::ParameterList * arg = ri->arguments();
    	if (arg->length() > 0) {
	    for (int i = 0; i < arg->length(); i++) {
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
    	for (int i = 0; i < ctx->length(); i += 2) {
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


class MyIORInterceptor
    : virtual public PortableInterceptor::IORInterceptor,
      virtual public CORBA::LocalObject
{
    string nm;
        
public:
        
    MyIORInterceptor() {
        nm = "";
    }
    MyIORInterceptor(char * name) {     
        nm = name;
    }
        
    char* name() {
        return (char *)nm.c_str();
    }

    void destroy()
    { cout << this->name() << " destroy" << endl; }

    void establish_components(PortableInterceptor::IORInfo_ptr info)
    {
        cout << this->name() << " establish_components" << endl;
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
        MyIORInterceptor * ior_interceptor = new MyIORInterceptor("MyIORInterceptor");
        info->add_ior_interceptor(ior_interceptor);
        MyIORInterceptor * ior_interceptor2 = new MyIORInterceptor("MyIORInterceptor2");
        info->add_ior_interceptor(ior_interceptor2);
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

    /*
     * Initialize the ORB
     */

    orb = CORBA::ORB_init (argc, argv);

    /*
     * Obtain a reference to the RootPOA and its Manager
     */

    CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
    PortableServer::POAManager_var mgr = poa->the_POAManager();
  
    CORBA::PolicyList pl;
    pl.length(1);
    pl[0] = poa->create_id_assignment_policy(PortableServer::USER_ID);

    PortableServer::POA_var poa2 = poa->create_POA("MyPOA", mgr, pl);

    cout << "RootPOA id: `";
    CORBA::OctetSeq* poa_id = poa->id();
    for(CORBA::ULong i=0; i<poa_id->length(); i++)
	cout << (*poa_id)[i];
    cout << "'" << endl;
    delete poa_id;

    cout << "MyPOA id: `";
    poa_id = poa2->id();
    for(CORBA::ULong i=0; i<poa_id->length(); i++)
	cout << (*poa_id)[i];
    cout << "'" << endl;

    /*
     * Create a Bank
     */

    Bank_impl * micocash = new Bank_impl;

    /*
     * Activate the Bank
     */
    PortableServer::ObjectId_var oid = PortableServer::string_to_ObjectId("Bank_impl");
    //PortableServer::ObjectId_var oid = poa->activate_object_with_id (oid, micocash);
    poa2->activate_object_with_id (oid, micocash);

    /*
     * Write reference to file
     */

    ofstream of ("Bank.ref");
    CORBA::Object_var ref = poa2->id_to_reference (oid.in());
    CORBA::String_var str = orb->object_to_string (ref.in());
    of << str.in() << endl;
    of.close ();

    /*
     * Activate the POA and start serving requests
     */

    cout << "Running." << endl;

    mgr->activate ();
    while(!finished) {
        orb->perform_work();
    }

    /*
     * Shutdown (never reached)
     */

    //poa->destroy (TRUE, TRUE);
    //delete micocash;
    cout << "server orb destroy" << endl;
    orb->destroy();

    return 0;
}
