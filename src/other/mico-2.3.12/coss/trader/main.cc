#include "trader_main.h"
#include "CosTradingRepos.h"
#include "typerepo_impl.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif


using namespace std;

CORBA::ORB_var orb;
CORBA::BOA_var boa;

class TraderLoader : public CORBA::BOAObjectRestorer
{
public:
    TraderLoader() { }
  
    CORBA::Boolean restore( CORBA::Object_ptr _obj )
    {
      if ( strcmp( _obj->_repoid(), "IDL:omg.org/CosTrading/Lookup:1.0" ) == 0 )
      {
	cerr << "restoring ... " << endl;
	Trader* trader = new Trader( _obj );
	cerr << "restored ... " << endl;
	return TRUE;
      }
      cout << "cannot restore " << _obj->_repoid() << " objects" << endl;
      return FALSE;
    }

    CORBA::Boolean bind( const char *repoid, const CORBA::ORB::ObjectTag &tag )
    {
      if ( strcmp( repoid, "IDL:omg.org/CosTrading/Lookup:1.0" ) == 0 )
      {
	cerr << "creating Trader" << endl;
	Trader* trader = new Trader( tag );
	return TRUE;
      }
      return FALSE;
    }
};

int main( int argc, char **argv )
{
  TraderLoader loader;

  orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );
  boa = orb->BOA_init (argc, argv, "mico-local-boa");

  cout << "Trader running ..." << endl;
  boa->impl_is_ready( CORBA::ImplementationDef::_nil() );
  orb->run();
  return 0;
}

