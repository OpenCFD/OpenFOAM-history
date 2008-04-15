
#ifndef __SIMPLE_IMPL_H__
#define __SIMPLE_IMPL_H__

#include "simple.h"


// Implementation for interface simple
class simple_impl1 : virtual public POA_simple1
{
  public:

    void func( CORBA::Char id ) throw( ::CORBA::SystemException);
    void flush() throw( ::CORBA::SystemException);
};

// Implementation for interface simple
class simple_impl2 : virtual public POA_simple2
{
  public:

    void func( CORBA::Char id ) throw( ::CORBA::SystemException);
    void flush() throw( ::CORBA::SystemException);
};

#endif
