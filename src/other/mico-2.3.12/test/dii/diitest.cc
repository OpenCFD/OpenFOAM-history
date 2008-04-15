#include <CORBA.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <string>
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <string.h>
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <mico/template_impl.h>


using namespace std;

class CalcImpl : public PortableServer::DynamicImplementation {
public:
    CalcImpl(CORBA::ORB_ptr orb, PortableServer::POA_ptr poa);

    virtual void
    invoke(CORBA::ServerRequest_ptr svreq);

    virtual CORBA::Long
    add(CORBA::Long x, CORBA::Long y);

    virtual char*
    _primary_interface
    (const PortableServer::ObjectId& oid,
     PortableServer::POA_ptr poa);

    virtual PortableServer::POA_ptr
    _default_POA();
private:
    CORBA::ORB_var orb_;
    PortableServer::POA_var poa_;
};

CalcImpl::CalcImpl(CORBA::ORB_ptr orb, PortableServer::POA_ptr poa)
    : orb_(CORBA::ORB::_duplicate(orb)), poa_(PortableServer::POA::_duplicate(poa))
{
}

void
CalcImpl::invoke (CORBA::ServerRequest_ptr svreq)
{
    if (!strcmp (svreq->op_name(), "add")) {
	// create argument list
	CORBA::NVList_ptr args;
	orb_->create_list (0, args);

	// fill in NamedValue's
	// XXX must set TypeCode's, too ...
	args->add (CORBA::ARG_IN);
	args->item(0)->value()->set_type(CORBA::_tc_long);
	args->add (CORBA::ARG_IN);
	args->item(1)->value()->set_type(CORBA::_tc_long);
	args->add (CORBA::ARG_OUT);
	args->item(2)->value()->set_type(CORBA::_tc_string);

	// get IN param values
	svreq->params (args);

	// get parameters
	CORBA::Long x;
	*args->item(0)->value() >>= x;
	CORBA::Long y;
	*args->item(1)->value() >>= y;

	// call user code
	CORBA::Long z = this->add (x, y);

	// set result
	*args->item(2)->value() <<= "hello world";
	CORBA::Any *res = new CORBA::Any;
	*res <<= z;
	svreq->result (res);
    }
}

CORBA::Long
CalcImpl::add (CORBA::Long x, CORBA::Long y)
{
    cout << "add " << x << " " << y << endl;
    return x + y;
}

char*
CalcImpl::_primary_interface
(const PortableServer::ObjectId& oid,
 PortableServer::POA_ptr poa)
{
    return CORBA::string_dup("IDL::Calc:1.0");
}

PortableServer::POA_ptr
CalcImpl::_default_POA()
{
    return PortableServer::POA::_duplicate(poa_);
}

int
main (int argc, char *argv[])
{
    // client side
    CORBA::ORB_ptr orb = CORBA::ORB_init (argc, argv, "mico-local-orb");
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);
    PortableServer::POAManager_var mgr = poa->the_POAManager();
    mgr->activate();
    // server side
    CalcImpl* srv = new CalcImpl(orb, poa);
    obj = srv->_this();

    CORBA::Request_var req = obj->_request ("add");
    req->add_in_arg() <<= (CORBA::Long)1L;
    req->add_in_arg() <<= (CORBA::Long)2L;
    req->add_out_arg ();
    req->invoke ();
    CORBA::Long z;
    *req->result()->value() >>= z;
    cout << "result: " << z << endl;

    const char *str;
    *req->arguments()->item(2)->value() >>= str;
    cout << str << endl;
    return 0;
}
