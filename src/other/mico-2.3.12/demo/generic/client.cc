

#include <CORBA.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <sstream>
#include <fstream>
#else
#include <iostream.h>
#include <strstream.h>
#include <fstream.h>
#endif


using namespace std;

class RequestConstructor
{
private:
    CORBA::Repository_var         _ir;
    CORBA::Object_var             _server;
    DynamicAny::DynAnyFactory_var _fac;
    
public:
    RequestConstructor (CORBA::ORB_ptr orb, CORBA::Object_ptr server)
    {
	CORBA::Object_var obj;
	obj = orb->resolve_initial_references ("InterfaceRepository");
	_ir = CORBA::Repository::_narrow (obj);
	assert (!CORBA::is_nil (_ir));
	
	obj = orb->resolve_initial_references ("DynAnyFactory");
	_fac = DynamicAny::DynAnyFactory::_narrow (obj);
	assert (!CORBA::is_nil (_fac));
	
	_server = CORBA::Object::_duplicate (server);
    }
    
    CORBA::Request_ptr create_request (const char* opname,
				       istream& in)
    {
	CORBA::ContainedSeq_var cs = _ir->lookup_name (opname,
						       -1,
						       CORBA::dk_Operation,
						       false);
	assert (cs->length() == 1);
	CORBA::OperationDef_var op = CORBA::OperationDef::_narrow (cs[0u]);
	assert (!CORBA::is_nil (op));
	
	CORBA::String_var op_name = op->name ();
	
	CORBA::Request_ptr req = _server->_request (op_name);
	
	CORBA::ParDescriptionSeq_var params = op->params();
	
	build_params (req, *params, in);
	
	return req;
    }
    
    void build_params (CORBA::Request_ptr req,
		       CORBA::ParDescriptionSeq& params,
		       istream& in)
    {
	for (int i=0; i < params.length(); i++) {
	    assert (params[i].mode == CORBA::PARAM_IN);
	    
	    CORBA::TypeCode_ptr tc = params[i].type;
	    DynamicAny::DynAny_var da =
		_fac->create_dyn_any_from_type_code(tc);
	    build_arg (da, tc, in);
	    CORBA::Any_var arg = da->to_any();
	    req->add_in_arg ("xxx") = *arg;
	}
    }
    
    void build_arg (DynamicAny::DynAny* da,
		    CORBA::TypeCode_ptr tc,
		    istream& in)
    {
	
	switch (tc->kind()) {
	case CORBA::tk_short:
	    {
		CORBA::Short s;
		in >> s;
		da->insert_short (s);
		break;
	    }
	case CORBA::tk_long:
	    {
		CORBA::Long l;
		in >> l;
		da->insert_long (l);
		break;
	    }
	case CORBA::tk_struct:
	    {
		build_struct_arg (da, tc, in);
		break;
	    }
	    
	default:
	    assert (0);
	}
    }
    
    void build_struct_arg (DynamicAny::DynAny* da,
			   CORBA::TypeCode_ptr tc,
			   istream& in )
    {
	DynamicAny::DynStruct_var ds = DynamicAny::DynStruct::_narrow (da);
	assert (!CORBA::is_nil (ds));
	
	for (int i=0; i < tc->member_count(); i++) {
	    CORBA::TypeCode_var tc_mem = tc->member_type (i);
	    build_arg (ds, tc_mem, in);
	    ds->next ();
	}
    }
};

    
	    
int
main (int argc, char *argv[])
{
    /*
     * Initialize the ORB
     */

    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    if (argc != 2) {
	cout << "usage: client <ior>" << endl;
	exit (1);
    }
  
    /*
     * Connect to the generic server
     */
    CORBA::Object_var obj = orb->string_to_object (argv[1]);
    assert (!CORBA::is_nil (obj));

    RequestConstructor constructor (orb, obj);
    
    ifstream in ("test_cases");
    assert (in);
    
    char rawline [1000];
    
    while (in.getline (rawline, sizeof (rawline))) {
	if (rawline[0] == '%')
	    continue;
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
	istringstream istr (rawline);
#else
	istrstream istr (rawline);
#endif
	string opname;
	
	istr >> opname;
	
	CORBA::Request_var req = constructor.create_request (opname.c_str(),
							     istr);
	req->invoke();
    }
    
    return 0;
}
