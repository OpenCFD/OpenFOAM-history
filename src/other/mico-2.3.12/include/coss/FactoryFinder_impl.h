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


#ifndef __FactoryFinder_impl_h__
#define __FactoryFinder_impl_h__

#include <coss/CosLifeCycle.h>
#include <coss/RegisterHelper.h>

class FactoryFinder_impl : virtual public POA_CosLifeCycle::FactoryFinder
{
    CORBA::ORB_var orb_;
    CosTrading::Lookup_var trader_;
	CosNaming::NamingContext_var naming_;
    CosLifeCycle::Factories* tfind_factories(const CosLifeCycle::Key& factory_key);
    CosLifeCycle::Factories* nfind_factories(const CosLifeCycle::Key& factory_key,
											 CosNaming::NamingContext_ptr nc=CosNaming::NamingContext::_nil());
public:
    FactoryFinder_impl();
    FactoryFinder_impl(CORBA::ORB_ptr, RegisterHelper::FinderType );
    FactoryFinder_impl(CORBA::ORB_ptr, CosTrading::Lookup_ptr );
    FactoryFinder_impl(CORBA::ORB_ptr, CosNaming::NamingContext_ptr );
    ~FactoryFinder_impl();
    virtual CosLifeCycle::Factories* find_factories(const CosLifeCycle::Key& factory_key);
};

#endif /* __FactoryFinder_impl_h__ */
