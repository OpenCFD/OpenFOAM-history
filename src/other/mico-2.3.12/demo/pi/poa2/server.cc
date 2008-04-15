/*
 * A Bank factory that creates Account objects
 *
 * In this example, we set up a ServantManager. The bank creates
 * references to "virtual" objects. When an account object is invoked
 * for the first time, the ServantManager is called to incarnate the
 * account.
 */
#define MICO_CONF_INTERCEPT
#include "account.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else
#include <fstream.h>
#endif


using namespace std;

/*
 * Implementation of the Account
 */

class Account_impl : virtual public POA_Account
{
public:
    Account_impl ();

    void deposit (CORBA::ULong);
    void withdraw (CORBA::ULong);
    CORBA::Long balance ();

private:
    CORBA::Long bal;
};

Account_impl::Account_impl ()
{
    bal = 0;
}

void
Account_impl::deposit (CORBA::ULong amount)
{
    bal += amount;
}

void
Account_impl::withdraw (CORBA::ULong amount)
{
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
    Bank_impl (PortableServer::POA_ptr);
    Account_ptr create ();

private:
    PortableServer::POA_var mypoa;
};

Bank_impl::Bank_impl (PortableServer::POA_ptr _poa)
{
    mypoa = PortableServer::POA::_duplicate (_poa);
}

Account_ptr
Bank_impl::create ()
{
    /*
     * We don't create an account, but only a reference to a "virtual"
     * object. The Servant Manager is responsible for incarnating the
     * account on request.
   */

    CORBA::Object_var obj = mypoa->create_reference ("IDL:Account:1.0");
    Account_ptr aref = Account::_narrow (obj);
    assert (!CORBA::is_nil (aref));

    /*
   * Return the reference
   */

    return aref;
}

/*
 * Implementation of the Server Manager (a ServantActivator, actually)
 */

class AccountManager : public virtual POA_PortableServer::ServantActivator
{
public:
    PortableServer::Servant incarnate (const PortableServer::ObjectId &,
				       PortableServer::POA_ptr);
    void etherealize (const PortableServer::ObjectId &,
		      PortableServer::POA_ptr,
		      PortableServer::Servant,
		      CORBA::Boolean, CORBA::Boolean);
};

PortableServer::Servant
AccountManager::incarnate (const PortableServer::ObjectId & oid,
			   PortableServer::POA_ptr poa)
{
    /*
     * Incarnate a new Account
     */

    return new Account_impl;
}

void
AccountManager::etherealize (const PortableServer::ObjectId & oid,
			     PortableServer::POA_ptr poa,
			     PortableServer::Servant serv,
			     CORBA::Boolean cleanup_in_progress,
			     CORBA::Boolean remaining_activations)
{
    /*
     * If there are no remaining activations for that servant (which
     * actually could only happen with the MULTIPLE_ID policy, when
     * one servant can incarnate many objects), delete the account.
     */

    if (!remaining_activations) {
	delete serv;
    }
}

// PI interceptors
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
	for (int i = 0; i < oaid->length(); i++)
	    cout << (*oaid)[i];
	cout << endl;
	cout << "Interface = ";
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
	cout << "Operation = " << ri->operation() << endl;
		
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

//////////////////////////////////////////////////
// IOR interceptor


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
    {}

    void establish_components(PortableInterceptor::IORInfo_ptr info) {
	
	cout << "MyIORInterceptor: establish_components" << endl;
	cout << "get_effective_policy" << endl;
	CORBA::Policy_ptr pol;
	try {
	    pol = info->get_effective_policy((CORBA::PolicyType)111111);
	} catch (...) {
	    cout << "Exception occured..." << endl;
	    return;
	}
	cout << "PolicyType = " << pol->policy_type() << endl;
    }

};

////////////////////////////
// Policy just for example

class MyPolicy : virtual public CORBA::Policy
{
    CORBA::PolicyType pt;
	
public:
    MyPolicy() {
	pt = (CORBA::PolicyType)111111;
    }
    ~MyPolicy() {}
  	
    CORBA::PolicyType policy_type() {
	return pt;
    }
    CORBA::Policy_ptr copy() {
	return _duplicate(this);
    }
    void destroy() {}
};
	
///////////////////////////
// Policy Factory

class MyPolicyFactory
    : virtual public PortableInterceptor::PolicyFactory,
      virtual public CORBA::LocalObject
{
public:	
    MyPolicyFactory() {}
    ~MyPolicyFactory() {}
	
    CORBA::Policy_ptr create_policy( CORBA::PolicyType type, const CORBA::Any& value ) {
	if (type != (CORBA::PolicyType)111111)
	    mico_throw(CORBA::PolicyError());
	return new MyPolicy;
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
	//    	MyServerInterceptor * interceptor2 = new MyServerInterceptor("MyServerInterceptor2");
	//    	info->add_server_request_interceptor(interceptor2);
	MyIORInterceptor * ior_interceptor = new MyIORInterceptor("MyIORInterceptor");
	info->add_ior_interceptor(ior_interceptor);
	MyPolicyFactory * factory = new MyPolicyFactory;
	info->register_policy_factory((CORBA::PolicyType)111111, factory);
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

    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    /*
     * Obtain a reference to the RootPOA and its Manager
     */

    CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
    PortableServer::POAManager_var mgr = poa->the_POAManager();

    /*
     * The RootPOA has the USE_ACTIVE_OBJECT_MAP_ONLY policy; to register
     * our ServantManager, we must create our own POA with the
     * USE_SERVANT_MANAGER policy
     */

    CORBA::PolicyList pl;
    pl.length(2);
    pl[0] = poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER);
    // test our policy factory, create MyPolicy and add it
    CORBA::Any any; // for this example we don't need
    pl[1] = orb->create_policy((CORBA::PolicyType)111111, any);
    //
    PortableServer::POA_var mypoa = poa->create_POA ("MyPOA", mgr, pl);

    /*
     * Activate our ServantManager
     */

    AccountManager * am = new AccountManager;
    PortableServer::ServantManager_var amref = am->_this ();
    mypoa->set_servant_manager (amref);

    /*
     * Create a Bank
     */

    Bank_impl * micocash = new Bank_impl (mypoa);

    /*
     * Activate the Bank
     */

    PortableServer::ObjectId_var oid = poa->activate_object (micocash);

    /*
     * Write reference to file
     */

    ofstream of ("Bank.ref");
    CORBA::Object_var ref = poa->id_to_reference (oid.in());
    CORBA::String_var str = orb->object_to_string (ref.in());
    of << str.in() << endl;
    of.close ();

    /*
     * Activate both POAs and start serving requests
     */

    cout << "Running." << endl;

    mgr->activate ();
    orb->run();

    /*
     * Shutdown (never reached). This would call etherealize() for all
     * our accounts.
     */

    poa->destroy (TRUE, TRUE);
    delete am;
    delete micocash;

    return 0;
}
