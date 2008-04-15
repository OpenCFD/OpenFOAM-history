#include <CORBA.h>
#include <mico/template_impl.h>
#include <mico/ir_creator.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS


using namespace std;

void print_contents_of_ir( CORBA::Repository_ptr repo )
{
  CORBA::ContainedSeq_var c;

  // Get the contents of the IR, but only objects of type
  // InterfaceDef and only objects directly contained in the IR
  c = repo->contents( CORBA::dk_Interface, 1 );
  for( CORBA::ULong j = 0; j < c->length(); j++ ) {
    CORBA::InterfaceDef_var in = CORBA::InterfaceDef::_narrow( c[ j ] );
    assert( !CORBA::is_nil( in ) );
    CORBA::String_var name;
    name = in->name();
    cout << "Interface: " << name.in() << endl;

    // Get all operations defined in this interface
    CORBA::InterfaceDef::FullInterfaceDescription_var desc;
    desc = in->describe_interface();
    for( CORBA::ULong k = 0; k < desc->operations.length(); k++ ) {
      CORBA::OperationDescription op;
      op = desc->operations[ k ];
      cout << "Operation: " << op.name.in() << endl;
      CORBA::ParDescriptionSeq& p = op.parameters;
      for( CORBA::ULong l = 0; l < p.length(); l++ ) {
	cout << "Param #" << l + 1 << " " << p[ l ].name.in() << endl;
      }
    }
    cout << endl;
  }
}



int main( int argc, char *argv[] )
{
  // ORB initialization
  CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );

  // Create a local IR
  CORBA::Repository_var repo =
    MICO::create_interface_repository (orb);

  // Create an interface called "calc"
  CORBA::InterfaceDefSeq base_interfaces;
  CORBA::InterfaceDef_var calc = repo->create_interface( "IDL:calc:1.0",
							 "calc",
							 "1.0",
							 base_interfaces );

  // Create an interface called "simple_calc"
  CORBA::InterfaceDef_var simple_calc =
    repo->create_interface( "IDL:simple_calc:1.0",
			    "simple_calc",
			    "1.0",
			    base_interfaces );

  // Add an operation to interface "calc" whose signature is:
  //     void add( in short x, in short y, out long z );
  CORBA::ParDescriptionSeq params;

  CORBA::ParameterDescription p1, p2, p3;
  p1.name = (const char *) "x";
  p1.type_def = repo->get_primitive( CORBA::pk_short );
  p1.type = p1.type_def->type();
  p1.mode = CORBA::PARAM_IN;
  
  p2.name = (const char *) "y";
  p2.type_def = repo->get_primitive( CORBA::pk_short );
  p2.type = p2.type_def->type();
  p2.mode = CORBA::PARAM_IN;
  
  p3.name = (const char *) "z";
  p3.type_def = repo->get_primitive( CORBA::pk_long );
  p3.type = p3.type_def->type();
  p3.mode = CORBA::PARAM_OUT;
  
  params.length( 3 );
  params[ 0 ] = p1;
  params[ 1 ] = p2;
  params[ 2 ] = p3;
  
  CORBA::IDLType_var op_result = repo->get_primitive( CORBA::pk_void );

  CORBA::ExceptionDefSeq exceptions;
  CORBA::ContextIdSeq contexts;
  
  CORBA::OperationDef_var add = calc->create_operation( "IDL:add:1.0",
							"add",
							"1.0",
							op_result,
							CORBA::OP_NORMAL,
							params,
							exceptions,
							contexts );

  // Add an operation to interface "calc" whose signature is:
  //     void sub( in short x, in short y, out long z );
  CORBA::OperationDef_var sub = calc->create_operation( "IDL:sub:1.0",
							"sub",
							"1.0",
							op_result,
							CORBA::OP_NORMAL,
							params,
							exceptions,
							contexts );

  cout << "-1------------------------------------" << endl << endl;

  // Print the content of the IR
  print_contents_of_ir( repo );
  
  cout << "-2------------------------------------" << endl << endl;

  // Move the "add" operation from interface "calc" to "simple_calc"
  add->move( simple_calc, "add", "1.0" );
  print_contents_of_ir( repo );

  cout << "-3------------------------------------" << endl << endl;

  // Remove the "sub" operation from interface "calc"
  sub->destroy();
  print_contents_of_ir( repo );

  cout << "-4------------------------------------" << endl << endl;

  // Remove the complete interface "simple_calc"
  simple_calc->destroy();
  print_contents_of_ir( repo );
  
  return 0;
}
