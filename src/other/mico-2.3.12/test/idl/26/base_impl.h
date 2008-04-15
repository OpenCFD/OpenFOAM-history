
#ifndef __BASE_IMPL_H__
#define __BASE_IMPL_H__

#include "base.h"

class base_impl : virtual public POA_base
{
  base_var val;
public:
  base_impl();
  void m( base_ptr x );
  base_ptr m();
};

#endif

