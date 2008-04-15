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
#include <coss/ExternalizationReference_impl.h>

// ----------------------------------------------------------------------
// ReferencesRole_impl constructor and destructor
// ----------------------------------------------------------------------

ExternalizationReferencesRole_impl::ExternalizationReferencesRole_impl ()
    : CosCompoundExternalization_impl::Role_impl (),
      CosGraphs_impl::Role_impl (),
      ::Role_impl(),
      POA_CosExternalizationReference::ReferencesRole(),
      POA_CosReference::ReferencesRole()
{
    external_from_id_[2].id = CORBA::string_dup("::CosExternalizationReference::ReferencesRole");
}


ExternalizationReferencesRole_impl::ExternalizationReferencesRole_impl (CosRelationships::RelatedObject_ptr obj)
    : CosCompoundExternalization_impl::Role_impl(),
      CosGraphs_impl::Role_impl(),
      ::Role_impl (),
      ::ReferencesRole_impl(),
      POA_CosExternalizationReference::ReferencesRole()
{
    external_from_id_[2].id = CORBA::string_dup("::CosExternalizationReference::ReferencesRole");
    if (!CORBA::is_nil(obj) && CORBA::is_nil(CosCompoundExternalization::Node::_narrow(obj)))
    {
	CosRelationships::RoleFactory::RelatedObjectTypeError ex;
	mico_throw (ex);
    }
    rel_obj = obj;
}

CosGraphs::PropagationValue
ExternalizationReferencesRole_impl::externalize_propagation(const CosCompoundExternalization::RelationshipHandle& rel,
                                    			    const char* to_role_name,
                                			    CORBA::Boolean& same_for_all)
{
    same_for_all = false;
    return CosGraphs::none;
}

// ----------------------------------------------------------------------
// ReferencedByRole_impl constructor and destructor
// ----------------------------------------------------------------------

ExternalizationReferencedByRole_impl::ExternalizationReferencedByRole_impl ()
    : CosCompoundExternalization_impl::Role_impl(),
      CosGraphs_impl::Role_impl (),
      ::Role_impl (),
      ::ReferencedByRole_impl(),
      POA_CosExternalizationReference::ReferencedByRole()
{
    external_from_id_[2].id = CORBA::string_dup("::CosExternalizationReference::ReferencedByRole");
}


ExternalizationReferencedByRole_impl::ExternalizationReferencedByRole_impl(CosRelationships::RelatedObject_ptr obj)
    : CosCompoundExternalization_impl::Role_impl(),
      CosGraphs_impl::Role_impl (),
      ::Role_impl (),
      ::ReferencedByRole_impl(),
      POA_CosExternalizationReference::ReferencedByRole()
{
    external_from_id_[2].id = CORBA::string_dup("::CosExternalizationReference::ReferencedByRole");
    if (!CORBA::is_nil(obj) && CORBA::is_nil(CosCompoundExternalization::Node::_narrow(obj)))
    {
	CosRelationships::RoleFactory::RelatedObjectTypeError ex;
	mico_throw (ex);
    }
    rel_obj = obj;
}

CosGraphs::PropagationValue
ExternalizationReferencedByRole_impl::externalize_propagation(const CosCompoundExternalization::RelationshipHandle& rel,
                                    			      const char* to_role_name,
                                    			      CORBA::Boolean& same_for_all)
{
    same_for_all = false;
    return CosGraphs::none;
}


// ----------------------------------------------------------------------
// Reference_impl constructor and destructor
// ----------------------------------------------------------------------

ExternalizationReference_impl::ExternalizationReference_impl ()
    : CosCompoundExternalization_impl::Relationship_impl(),
      ::Relationship_impl(),
      ::Reference_impl(),
      POA_CosExternalizationReference::Relationship()
{
    external_from_id_[2].id = CORBA::string_dup("::CosExternalizationReference::Relationship");
}


ExternalizationReference_impl::ExternalizationReference_impl (CosRelationships::NamedRoles nr)
    : CosCompoundExternalization_impl::Relationship_impl(),
      ::Relationship_impl (),
      ::Reference_impl(),
      POA_CosExternalizationReference::Relationship()
{
    external_from_id_[2].id = CORBA::string_dup("::CosExternalizationReference::Relationship");
    n_roles = nr;
}
