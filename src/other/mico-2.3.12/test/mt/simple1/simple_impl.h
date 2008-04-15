
#ifndef __SIMPLE_IMPL_H__
#define __SIMPLE_IMPL_H__

#include "simple.h"


// Implementation for interface simple
class simple_impl : virtual public POA_simple
{
  public:

    void func( CORBA::Char id ) throw( ::CORBA::SystemException);
    void flush() throw( ::CORBA::SystemException);
};

#endif
