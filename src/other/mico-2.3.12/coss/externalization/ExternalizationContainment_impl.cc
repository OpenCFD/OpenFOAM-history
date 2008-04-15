/*
 *  Externalization Service for MICO
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
#include <coss/ExternalizationContainment_impl.h>

// ----------------------------------------------------------------------
// ContainsRole_impl constructor and destructor
// ----------------------------------------------------------------------

ExternalizationContainsRole_impl::ExternalizationContainsRole_impl()
    : CosCompoundExternalization_impl::Role_impl(),
      CosGraphs_impl::Role_impl(),
      ::Role_impl(),
      ::ContainsRole_impl(),
      POA_CosExternalizationContainment::ContainsRole()
{
    external_from_id_[2].id = CORBA::string_dup("::CosExternalizationContainment::ContainsRole");
}


ExternalizationContainsRole_impl::ExternalizationContainsRole_impl(CosRelationships::RelatedObject_ptr obj)
    : CosCompoundExternalization_impl::Role_impl(),
      CosGraphs_impl::Role_impl(),
      ::Role_impl(),
      ::ContainsRole_impl(),
      POA_CosExternalizationContainment::ContainsRole()
{
    external_from_id_[2].id = CORBA::string_dup("::CosExternalizationContainment::ContainsRole");
    if (!CORBA::is_nil(obj) && CORBA::is_nil(CosCompoundExternalization::Node::_narrow(obj)))
    {
	CosRelationships::RoleFactory::RelatedObjectTypeError ex;
	mico_throw (ex);
    }
    rel_obj = obj;
}

CosGraphs::PropagationValue
ExternalizationContainsRole_impl::externalize_propagation(const CosCompoundExternalization::RelationshipHandle& rel,
                                			  const char* to_role_name,
                                    			  CORBA::Boolean& same_for_all)
{
    same_for_all = false;
    return CosGraphs::deep;
}

// ----------------------------------------------------------------------
// ContainedInRole_impl constructor and destructor
// ----------------------------------------------------------------------

ExternalizationContainedInRole_impl::ExternalizationContainedInRole_impl ()
    : CosCompoundExternalization_impl::Role_impl(),
      CosGraphs_impl::Role_impl(),
      ::Role_impl(),
      ::ContainedInRole_impl(),
      POA_CosExternalizationContainment::ContainedInRole()
{
    external_from_id_[2].id = CORBA::string_dup("::CosExternalizationContainment::ContainedInRole");
}


ExternalizationContainedInRole_impl::ExternalizationContainedInRole_impl(CosRelationships::RelatedObject_ptr obj)
    : CosCompoundExternalization_impl::Role_impl(),
      CosGraphs_impl::Role_impl(),
      ::Role_impl(),
      ::ContainedInRole_impl(),
      POA_CosExternalizationContainment::ContainedInRole()
{
    external_from_id_[2].id = CORBA::string_dup("::CosExternalizationContainment::ContainedInRole");
    if (!CORBA::is_nil(obj) && CORBA::is_nil(CosCompoundExternalization::Node::_narrow(obj)))
    {
	CosRelationships::RoleFactory::RelatedObjectTypeError ex;
	mico_throw (ex);
    }
    rel_obj = obj;
}

CosGraphs::PropagationValue
ExternalizationContainedInRole_impl::externalize_propagation(const CosCompoundExternalization::RelationshipHandle& rel,
                                    			     const char* to_role_name,
                                			     CORBA::Boolean& same_for_all)
{
    same_for_all = false;
    return CosGraphs::none;
}

// ----------------------------------------------------------------------
// Containment_impl constructor and destructor
// ----------------------------------------------------------------------

ExternalizationContainment_impl::ExternalizationContainment_impl()
    : CosCompoundExternalization_impl::Relationship_impl(),
      ::Relationship_impl(),
      ::Containment_impl(),
      POA_CosExternalizationContainment::Relationship()
{
    external_from_id_[2].id = CORBA::string_dup("::CosExternalizationContainment::Relationship");
}


ExternalizationContainment_impl::ExternalizationContainment_impl(CosRelationships::NamedRoles nr)
    : CosCompoundExternalization_impl::Relationship_impl(),
      ::Relationship_impl(),
      ::Containment_impl(),
      POA_CosExternalizationContainment::Relationship()
{
    external_from_id_[2].id = CORBA::string_dup("::CosExternalizationContainment::Relationship");
    n_roles = nr;
}
