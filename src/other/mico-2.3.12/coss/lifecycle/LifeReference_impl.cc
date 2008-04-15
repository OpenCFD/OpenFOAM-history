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
#include <coss/LifeReference_impl.h>

// ----------------------------------------------------------------------
// ReferencesRole_impl constructor and destructor
// ----------------------------------------------------------------------

LifeCycleReferencesRole_impl::LifeCycleReferencesRole_impl ()
    : CosCompoundLifeCycle_impl::Role_impl (),
      CosGraphs_impl::Role_impl (),
      ::Role_impl (),
      ::ReferencesRole_impl (),
      POA_CosLifeCycleReference::ReferencesRole()
{
    factory_id_[2].id = CORBA::string_dup("::CosLifeCycleReference::ReferencesRole");
}


LifeCycleReferencesRole_impl::LifeCycleReferencesRole_impl (CosRelationships::RelatedObject_ptr obj)
    : CosCompoundLifeCycle_impl::Role_impl (),
      CosGraphs_impl::Role_impl (),
      ::Role_impl (),
      ::ReferencesRole_impl (),
      POA_CosLifeCycleReference::ReferencesRole()
{
    factory_id_[2].id = CORBA::string_dup("::CosLifeCycleReference::ReferencesRole");
    if (!CORBA::is_nil(obj) && CORBA::is_nil(CosCompoundLifeCycle::Node::_narrow(obj)))
    {
	CosRelationships::RoleFactory::RelatedObjectTypeError ex;
	mico_throw(ex);
    }
    rel_obj = obj;
}

CosGraphs::PropagationValue
LifeCycleReferencesRole_impl::life_cycle_propagation(CosCompoundLifeCycle::Operation op,
                                        	     const CosCompoundLifeCycle::RelationshipHandle& rel,
                                        	     const char* to_role_name,
                                        	     CORBA::Boolean& same_for_all)
{
    same_for_all = false;
    switch (op)
    {
	case CosCompoundLifeCycle::copy: return CosGraphs::shallow;
	case CosCompoundLifeCycle::move: return CosGraphs::shallow;
	case CosCompoundLifeCycle::remove: return CosGraphs::shallow;
    }
    return CosGraphs::none;
}

// ----------------------------------------------------------------------
// ReferencedByRole_impl constructor and destructor
// ----------------------------------------------------------------------

LifeCycleReferencedByRole_impl::LifeCycleReferencedByRole_impl ()
    : CosCompoundLifeCycle_impl::Role_impl(),
      CosGraphs_impl::Role_impl (),
      ::Role_impl (),
      ::ReferencedByRole_impl (),
      POA_CosLifeCycleReference::ReferencedByRole()
{
    factory_id_[2].id = CORBA::string_dup("::CosLifeCycleReference::ReferencedByRole");
}


LifeCycleReferencedByRole_impl::LifeCycleReferencedByRole_impl(CosRelationships::RelatedObject_ptr obj)
    : CosCompoundLifeCycle_impl::Role_impl(),
      CosGraphs_impl::Role_impl (),
      ::Role_impl (),
      ::ReferencedByRole_impl (),
      POA_CosLifeCycleReference::ReferencedByRole()
{
    factory_id_[2].id = CORBA::string_dup("::CosLifeCycleReference::ReferencedByRole");
    if (!CORBA::is_nil(obj) && CORBA::is_nil(CosCompoundLifeCycle::Node::_narrow(obj)))
    {
	CosRelationships::RoleFactory::RelatedObjectTypeError ex;
	mico_throw(ex);
    }
    rel_obj = obj;
}

CosGraphs::PropagationValue
LifeCycleReferencedByRole_impl::life_cycle_propagation(CosCompoundLifeCycle::Operation op,
                                        	       const CosCompoundLifeCycle::RelationshipHandle& rel,
                                        	       const char* to_role_name,
                                    		       CORBA::Boolean& same_for_all)
{
    same_for_all = false;
    switch (op)
    {
	case CosCompoundLifeCycle::copy: return CosGraphs::none;
	case CosCompoundLifeCycle::move: return CosGraphs::shallow;
	case CosCompoundLifeCycle::remove: return CosGraphs::shallow;
    }
    return CosGraphs::none;
}


// ----------------------------------------------------------------------
// Containment_impl constructor and destructor
// ----------------------------------------------------------------------

LifeCycleReference_impl::LifeCycleReference_impl ()
    : CosCompoundLifeCycle_impl::Relationship_impl(),
      ::Relationship_impl(),
      ::Reference_impl (),
      POA_CosLifeCycleReference::Relationship()
{
    factory_id_[2].id = CORBA::string_dup("::CosLifeCycleReference::Relationship");
}


LifeCycleReference_impl::LifeCycleReference_impl (CosRelationships::NamedRoles nr)
    : CosCompoundLifeCycle_impl::Relationship_impl(),
      ::Relationship_impl (),
      ::Reference_impl (),
      POA_CosLifeCycleReference::Relationship()
{
    factory_id_[2].id = CORBA::string_dup("::CosLifeCycleReference::Relationship");
    n_roles = nr;
}
