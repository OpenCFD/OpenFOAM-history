#include "base_impl.h"

base_impl::base_impl()
{
}

void base_impl::m( base_ptr x )
{
  assert( !CORBA::is_nil( x ) );
  val = base::_duplicate( x );
  assert( !CORBA::is_nil( val ) );
}

base_ptr base_impl::m()
{
  return base::_duplicate( val );
}


