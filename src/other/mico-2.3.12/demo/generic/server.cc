
#include <CORBA.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif


using namespace std;

static CORBA::ORB_var                the_orb;
static CORBA::Repository_var         the_ir;
static DynamicAny::DynAnyFactory_var the_dyn_any_fact;


class ShowRequest
{
private:
    CORBA::ServerRequest_var _req;
    
public:
    ShowRequest (CORBA::ServerRequest_ptr req)
    {
	_req = CORBA::ServerRequest::_duplicate (req);
    }

    void show()
    {
	cout << "Invocation" << endl;
	CORBA::String_var name = _req->operation();
	cout << "  name: " << name << endl;
	
	CORBA::ContainedSeq_var cs = the_ir->lookup_name (name,
							  -1,
							  CORBA::dk_Operation,
							  false);
	assert (cs->length() == 1);
	CORBA::OperationDef_var op = CORBA::OperationDef::_narrow (cs[0u]);
	assert (!CORBA::is_nil (op));
	
	CORBA::ParDescriptionSeq_var params = op->params();
	
	cout << "  number of parameters: " << params->length() << endl;
	
	CORBA::NVList_ptr args;
	the_orb->create_list (params->length(), args);
	
	for (int i=0; i < params->length(); i++) {
	    assert (params.in()[i].mode == CORBA::PARAM_IN);
	    CORBA::TypeCode_ptr tc = params.in()[i].type;
	    DynamicAny::DynAny_var da =
		the_dyn_any_fact->create_dyn_any_from_type_code (tc);
	    CORBA::Any_var any = da->to_any();
	    args->add_value ("xxx", *any, CORBA::ARG_IN);
	}
	_req->arguments (args);

	cout << "  Parameters: ";
	for (int j=0; j < params->length(); j++) {
	    if (j != 0)
		cout << ", ";
	    show_any (*args->item(j)->value());
	}
	cout << endl;
    }
    
    void show_any (CORBA::Any& any)
    {
	DynamicAny::DynAny_var da = the_dyn_any_fact->create_dyn_any (any);
	show_dyn_any (da);
    }
    
    void show_dyn_any (DynamicAny::DynAny_ptr da)
    {
	CORBA::TypeCode_var tc = da->type();
	
	switch (tc->kind()) {
	case CORBA::tk_short:
	    cout << "Short(" << da->get_short() << ") ";
	    break;
	case CORBA::tk_long:
	    cout << "Long(" << da->get_long() << ") ";
	    break;
	case CORBA::tk_struct:
	    {
		cout << "struct { ";
		for (int i=0; i < da->component_count(); i++) {
		    if (i != 0)
			cout << ", ";
		    show_dyn_any (da->current_component());
		    da->next();
		}
		cout << "} ";
		break;
	    }
	default:
	    assert (0);
	}
    }
};

    
class GenericServant : virtual public PortableServer::DynamicImplementation 
{
    virtual void invoke (CORBA::ServerRequest_ptr req)
	{
	    ShowRequest r (req);
	    r.show();
	}
    
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &,
						    PortableServer::POA_ptr)
	{
	    return CORBA::string_dup ("IDL:GenericServer:1.0");
	}
};


int
main (int argc, char *argv[])
{
    /*
     * Initialize the ORB
     */

    CORBA::Object_var obj;
    
    the_orb = CORBA::ORB_init (argc, argv);
    obj = the_orb->resolve_initial_references ("InterfaceRepository");
    the_ir = CORBA::Repository::_narrow (obj);
    assert (!CORBA::is_nil (the_ir));
    obj = the_orb->resolve_initial_references ("DynAnyFactory");
    the_dyn_any_fact = DynamicAny::DynAnyFactory::_narrow (obj);
    assert (!CORBA::is_nil (the_dyn_any_fact));
    
    /*
     * Obtain a reference to the RootPOA and its Manager
     */

    CORBA::Object_var poaobj = the_orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
    PortableServer::POAManager_var mgr = poa->the_POAManager();

    /*
     * Create the Generic Servant
     */

    PortableServer::Servant generic_servant = new GenericServant;

    /*
     * Activate the servant
     */

    CORBA::Object_var the_server = generic_servant->_this();

    /*
     * Write the object's IOR to a file called "server.ior"
     */

    CORBA::String_var ior = the_orb->object_to_string (the_server);
    ofstream of ("server.ior");
    of << ior;
    of.close ();
  
    /*
     * Activate the POA and start serving requests
     */

    cout << "Running." << endl;

    mgr->activate ();
    the_orb->run();

    /*
     * Shutdown (never reached)
     */

    poa->destroy (TRUE, TRUE);
    delete generic_servant;

    return 0;
}
