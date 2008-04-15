#include "simple_impl.h"


// Implementation for interface simple1

void
simple_impl1::func( CORBA::Char id ) throw( ::CORBA::SystemException)
{
  __MTSAVE( MICODebug::instance()->printer() 
	     << "(implementation) is in method: simple_impl1::func( " << id << " )" << endl );
  cout << id;
}

void
simple_impl1::flush() throw( ::CORBA::SystemException) {

  cout << endl;
}


// Implementation for interface simple2

void
simple_impl2::func( CORBA::Char id ) throw( ::CORBA::SystemException)
{
  __MTSAVE( MICODebug::instance()->printer() 
	     << "(implementation) is in method: simple_impl1::func( " << id << " )" << endl );
  cout << id;
}

void
simple_impl2::flush() throw( ::CORBA::SystemException) {

  cout << endl;
}
