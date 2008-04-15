#include <CORBA.h>
#include "request.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

CORBA::Request_ptr 
create_request( CORBA::Object_ptr obj, const char* opname )
{
    CORBA::InterfaceDef_var ir = obj->_get_interface();
    if (CORBA::is_nil (ir)) {
      std::cout << "oops: could not locate InterfaceRepository" << std::endl;
      exit (1);
    }
    
    // Load exceptions for operation
    CORBA::ExceptionDefSeq_var op_excpts;
    CORBA::ContainedSeq_var cs = ir->lookup_name( "dii", -1, CORBA::dk_Operation, false);
    if ( cs->length() != 1 ) {
      std::cout << "oops: no operation dii()" << std::endl;
      exit(1);
    }
    CORBA::OperationDef_var op = CORBA::OperationDef::_narrow (cs[0u]);
    op_excpts = op->exceptions();

    // create request and fill it with exceptions
    CORBA::Request_ptr req = obj->_request (opname);
    for( CORBA::ULong i = 0; i < op_excpts->length(); i++ ) {
      req->exceptions()->add( op_excpts[i]->type() );
    }

    return req;
}
