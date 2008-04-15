
#ifndef __VAL_IMPL_H__
#define __VAL_IMPL_H__

#include <val.h>

class Val_impl
    : virtual public OBV_Val,
      virtual public CORBA::DefaultValueRefCountBase
{
 public:
    Val_impl(CORBA::Long x)
	: OBV_Val(x)
    {}

    Val_impl()
    {}
};

class ValFactory_impl
    : virtual public CORBA::ValueFactoryBase
{
public:
    CORBA::ValueBase*
    create_for_unmarshal();
};

#endif // __VAL_IMPL_H__
