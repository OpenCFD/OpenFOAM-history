/*
 *  LifeCycle Service for MICO
 *  Copyright (C) 1999 Dmitry V. Sedov
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  Send comments and/or bug reports to:
 *                 mico@informatik.uni-frankfurt.de
 *  or to my private e-mail:
 *                 sedov@postman.simcb.ru
 */


#ifndef __OperationsFactory_impl_h__
#define __OperationsFactory_impl_h__

#include <CORBA.h>
#include <coss/CosCompoundLifeCycle.h>

template <class T>
class OperationsFactory_impl : virtual public POA_CosCompoundLifeCycle::OperationsFactory
{

public:
    OperationsFactory_impl();
    virtual CosCompoundLifeCycle::Operations_ptr create_compound_operations();
};

template <class T>
OperationsFactory_impl<T>::OperationsFactory_impl ()
    : POA_CosCompoundLifeCycle::OperationsFactory()
{
}

template <class T>
CosCompoundLifeCycle::Operations_ptr
OperationsFactory_impl<T>::create_compound_operations()
{
#ifdef DEBUG
  cout << "OperationsFactory_impl::create_compoundoperations (...)\n";
#endif
  T* tmp_operation = new T ();
  CosCompoundLifeCycle::Operations_ptr operation = tmp_operation->_this ();

  return operation;
}

#endif /* __OperationsFactory_impl_h__ */
