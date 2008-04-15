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


#ifndef __Operations_impl_h__
#define __Operations_impl_h__

#include <CORBA.h>
#include <coss/CosCompoundLifeCycle.h>

class Operations_impl : virtual public POA_CosCompoundLifeCycle::Operations
{

public:
    Operations_impl();
    virtual CosCompoundLifeCycle::Node_ptr copy(CosCompoundLifeCycle::Node_ptr starting_node,
                                                CosLifeCycle::FactoryFinder_ptr there,
                                                const CosLifeCycle::Criteria& the_criteria);

    virtual void move(CosCompoundLifeCycle::Node_ptr starting_node,
                      CosLifeCycle::FactoryFinder_ptr there,
                      const CosLifeCycle::Criteria& the_criteria);

    virtual void remove(CosCompoundLifeCycle::Node_ptr& starting_node);

    virtual void destroy();
};

struct RoleKey
{
    CosObjectIdentity::ObjectIdentifier id;
    CORBA::InterfaceDef_var IDef;
    RoleKey(CosObjectIdentity::ObjectIdentifier& id_, CORBA::InterfaceDef_ptr IDef_)
		{ id = id_; IDef = CORBA::InterfaceDef::_duplicate(IDef_); };
    RoleKey(const RoleKey& key)
		{ id = key.id; IDef = CORBA::InterfaceDef::_duplicate(key.IDef.in()); };
    CORBA::Boolean operator<(RoleKey& key) const
		{
		    if (id<key.id)
			return TRUE;
		    if (id>key.id)
			return FALSE;
		    return (strcmp(IDef->describe_interface()->id,key.IDef->describe_interface()->id) < 0);
		};
    CORBA::Boolean operator<(const RoleKey& key) const
		{
		    if (id<key.id)
			return TRUE;
		    if (id>key.id)
			return FALSE;
		    return (strcmp(IDef->describe_interface()->id,key.IDef->describe_interface()->id) < 0);
		};
};

#endif /* __Operations_impl_h__ */
