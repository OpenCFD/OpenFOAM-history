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


#include <coss/LifeRole_impl.h>
#include <coss/LifeRelationship_impl.h>
#include <coss/CosContainment_impl.h>
#include <coss/CosLifeCycleContainment.h>


#ifndef __LifeContainment_impl_h__
#define __LifeContainment_impl_h__


class LifeCycleContainsRole_impl : virtual public CosCompoundLifeCycle_impl::Role_impl,
				   virtual public POA_CosLifeCycleContainment::ContainsRole,
				   virtual public ::ContainsRole_impl
{
public:
    LifeCycleContainsRole_impl ();
    LifeCycleContainsRole_impl (CosRelationships::RelatedObject_ptr);
    virtual CosGraphs::PropagationValue life_cycle_propagation(CosCompoundLifeCycle::Operation op,
                                                	       const CosCompoundLifeCycle::RelationshipHandle& rel,
                                                	       const char* to_role_name,
                                                    	       CORBA::Boolean& same_for_all);
};


class LifeCycleContainedInRole_impl : virtual public CosCompoundLifeCycle_impl::Role_impl,
				      virtual public POA_CosLifeCycleContainment::ContainedInRole,
				      virtual public ::ContainedInRole_impl
{
public:
    LifeCycleContainedInRole_impl ();
    LifeCycleContainedInRole_impl (CosRelationships::RelatedObject_ptr);
    virtual CosGraphs::PropagationValue life_cycle_propagation(CosCompoundLifeCycle::Operation op,
                                                    	       const CosCompoundLifeCycle::RelationshipHandle& rel,
                                                	       const char* to_role_name,
                                                               CORBA::Boolean& same_for_all);
};


class LifeCycleContainment_impl : virtual public CosCompoundLifeCycle_impl::Relationship_impl,
				  virtual public POA_CosLifeCycleContainment::Relationship,
				  virtual public Containment_impl
{
public:
    LifeCycleContainment_impl ();
    LifeCycleContainment_impl (CosRelationships::NamedRoles nr);
};


#endif

