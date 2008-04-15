#include <CORBA.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <string>
#include <iostream>
#include <fstream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <string.h>
#include <iostream.h>
#include <fstream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <mico/template_impl.h>


using namespace std;
using namespace CORBA;
using namespace PortableServer;


class CalcImpl : public DynamicImplementation {
public:
    CalcImpl(ORB_ptr orb, POA_ptr poa);

    virtual void
    invoke(ServerRequest_ptr svreq);

    virtual Long
    add(Long x, Long y, Context_ptr ctx);

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
CalcImpl::invoke(ServerRequest_ptr svreq)
{
    if (!strcmp (svreq->op_name(), "add")) {
	// create argument list
	NVList_ptr args;
	orb_->create_list (0, args);

	// fill in NamedValue's
	// XXX must set TypeCode's, too ...
	args->add (ARG_IN);
	args->item(0)->value()->set_type (_tc_long);
	args->add (ARG_IN);
	args->item(1)->value()->set_type (_tc_long);
	args->add (ARG_OUT);
	args->item(2)->value()->set_type (_tc_string);

	// get IN param values
	svreq->params (args);

	// get parameters
	Long x;
	*args->item(0)->value() >>= x;
	Long y;
	*args->item(1)->value() >>= y;

	// call user code
	Long z = this->add (x, y, svreq->ctx());

	// set result
	*args->item(2)->value() <<= "hello world";
	Any *res = new Any;
	*res <<= z;
	svreq->result (res);
    }
}

Long
CalcImpl::add (Long x, Long y, Context_ptr ctx)
{
    NVList_var vals;
    ctx->get_values ("", 0, "*", vals);
    for (ULong i = 0; i < vals->count(); ++i) {
	const char *s;
	Boolean r = (*vals->item(i)->value() >>= s);
	assert (r);
	cout << vals->item(i)->name() << ": " << s << endl;
    }
    return x + y;
}

char*
CalcImpl::_primary_interface
(const PortableServer::ObjectId& oid,
 PortableServer::POA_ptr poa)
{
    return CORBA::string_dup("IDL:Calc:1.0");
}

PortableServer::POA_ptr
CalcImpl::_default_POA()
{
    return PortableServer::POA::_duplicate(poa_);
}

int
main (int argc, char *argv[])
{
    ORB_ptr orb = ORB_init (argc, argv, "mico-local-orb");
    Object_var obj = orb->resolve_initial_references("RootPOA");
    POA_var poa = PortableServer::POA::_narrow(obj);
    POAManager_var mgr = poa->the_POAManager();
    CalcImpl* srv = new CalcImpl(orb, poa);
    obj = srv->_this();
    ofstream out("diiserver.ior");
    String_var ref = orb->object_to_string(obj);
    out << ref.in() << endl;
    mgr->activate();
    orb->run ();
    return 0;
}
