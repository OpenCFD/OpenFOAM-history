
#include <val_impl.h>

CORBA::ValueBase*
ValFactory_impl::create_for_unmarshal()
{
    return new Val_impl;
}

