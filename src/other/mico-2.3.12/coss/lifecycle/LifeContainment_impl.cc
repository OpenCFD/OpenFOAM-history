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


#include <CORBA.h>
#include <coss/LifeContainment_impl.h>

// ----------------------------------------------------------------------
// ContainsRole_impl constructor and destructor
// ----------------------------------------------------------------------
LifeCycleContainsRole_impl::LifeCycleContainsRole_impl()
    : CosCompoundLifeCycle_impl::Role_impl(),
      CosGraphs_impl::Role_impl(),
      ::Role_impl(),
      ::ContainsRole_impl(),
    POA_CosLifeCycleContainment::ContainsRole()
{
    factory_id_[2].id = CORBA::string_dup("::CosLifeCycleContainment::ContainsRole");
}


LifeCycleContainsRole_impl::LifeCycleContainsRole_impl(CosRelationships::RelatedObject_ptr obj)
    : CosCompoundLifeCycle_impl::Role_impl(),
      CosGraphs_impl::Role_impl(),
      ::Role_impl(),
      ::ContainsRole_impl(),
      POA_CosLifeCycleContainment::ContainsRole()
{
    factory_id_[2].id = CORBA::string_dup("::CosLifeCycleContainment::ContainsRole");
    if (!CORBA::is_nil(obj) && CORBA::is_nil(CosCompoundLifeCycle::Node::_narrow(obj)))
    {
	CosRelationships::RoleFactory::RelatedObjectTypeError ex;
	mico_throw(ex);
    }
    rel_obj = obj;
}

CosGraphs::PropagationValue
LifeCycleContainsRole_impl::life_cycle_propagation(CosCompoundLifeCycle::Operation op,
                                            	   const CosCompoundLifeCycle::RelationshipHandle& rel,
                                                   const char* to_role_name,
                                                   CORBA::Boolean& same_for_all)
{
    same_for_all = false;
    switch (op)
    {
	case CosCompoundLifeCycle::copy:   return CosGraphs::deep;
	case CosCompoundLifeCycle::move:   return CosGraphs::deep;
	case CosCompoundLifeCycle::remove: return CosGraphs::deep;
    }
    return CosGraphs::none;
}

// ----------------------------------------------------------------------
// ContainedInRole_impl constructor and destructor
// ----------------------------------------------------------------------

LifeCycleContainedInRole_impl::LifeCycleContainedInRole_impl ()
    : CosCompoundLifeCycle_impl::Role_impl(),
      CosGraphs_impl::Role_impl(),
      ::Role_impl(),
      ::ContainedInRole_impl(),
      POA_CosLifeCycleContainment::ContainedInRole()
{
    factory_id_[2].id = CORBA::string_dup("::CosLifeCycleContainment::ContainedInRole");
}


LifeCycleContainedInRole_impl::LifeCycleContainedInRole_impl(CosRelationships::RelatedObject_ptr obj)
    : CosCompoundLifeCycle_impl::Role_impl(),
      CosGraphs_impl::Role_impl(),
      ::Role_impl(),
      ::ContainedInRole_impl(),
      POA_CosLifeCycleContainment::ContainedInRole()
{
    factory_id_[2].id = CORBA::string_dup("::CosLifeCycleContainment::ContainedInRole");
    if (!CORBA::is_nil(obj) && CORBA::is_nil(CosCompoundLifeCycle::Node::_narrow(obj)))
    {
	CosRelationships::RoleFactory::RelatedObjectTypeError ex;
	mico_throw(ex);
    }
    rel_obj = obj;
}

CosGraphs::PropagationValue
LifeCycleContainedInRole_impl::life_cycle_propagation(CosCompoundLifeCycle::Operation op,
                                        	      const CosCompoundLifeCycle::RelationshipHandle& rel,
                                        	      const char* to_role_name,
                                    		      CORBA::Boolean& same_for_all)
{
    same_for_all = false;
    switch (op)
    {
	case CosCompoundLifeCycle::copy:   return CosGraphs::shallow;
	case CosCompoundLifeCycle::move:   return CosGraphs::shallow;
	case CosCompoundLifeCycle::remove: return CosGraphs::shallow;
    }
    return CosGraphs::none;
}

// ----------------------------------------------------------------------
// Containment_impl constructor and destructor
// ----------------------------------------------------------------------

LifeCycleContainment_impl::LifeCycleContainment_impl()
    : CosCompoundLifeCycle_impl::Relationship_impl(),
      ::Relationship_impl(),
      ::Containment_impl(),
      POA_CosLifeCycleContainment::Relationship()
{
    factory_id_[2].id = CORBA::string_dup("::CosLifeCycleContainment::Relationship");
}


LifeCycleContainment_impl::LifeCycleContainment_impl(CosRelationships::NamedRoles nr)
    : CosCompoundLifeCycle_impl::Relationship_impl(),
      ::Relationship_impl(),
      Containment_impl(),
      POA_CosLifeCycleContainment::Relationship()
{
    factory_id_[2].id = CORBA::string_dup("::CosLifeCycleContainment::Relationship");
    n_roles = nr;
}
