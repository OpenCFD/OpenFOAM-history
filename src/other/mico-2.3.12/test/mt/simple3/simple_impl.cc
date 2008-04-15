
#include "simple_impl.h"


// Implementation for interface simple

void
simple_impl::func( CORBA::Char id ) throw( ::CORBA::SystemException) {
  //  cout << "(implementation) is in method: simple_impl::func( " << id << " )" << endl;
  cout << id;
}

void
simple_impl::flush() throw( ::CORBA::SystemException) {

  cout << endl;
}
