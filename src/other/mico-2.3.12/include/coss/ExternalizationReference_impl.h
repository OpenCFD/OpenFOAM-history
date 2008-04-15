/*
 *  Externaization Service for MICO
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


#include <coss/CompoundExternalization_impl.h>
#include <coss/CosReference_impl.h>
#include <coss/CosExternalizationReference.h>


#ifndef __ExternalizationReference_impl_h__
#define __ExternalizationReference_impl_h__


class ExternalizationReferencesRole_impl : virtual public CosCompoundExternalization_impl::Role_impl,
					   virtual public POA_CosExternalizationReference::ReferencesRole,
					   virtual public ::ReferencesRole_impl
{
public:
    ExternalizationReferencesRole_impl ();
    ExternalizationReferencesRole_impl (CosRelationships::RelatedObject_ptr);
    virtual CosGraphs::PropagationValue externalize_propagation(const CosCompoundExternalization::RelationshipHandle& rel,
                                                                const char* to_role_name,
                                                                CORBA::Boolean& same_for_all);

};


class ExternalizationReferencedByRole_impl : virtual public CosCompoundExternalization_impl::Role_impl,
					     virtual public POA_CosExternalizationReference::ReferencedByRole,
					     virtual public ::ReferencedByRole_impl
{
public:
    ExternalizationReferencedByRole_impl ();
    ExternalizationReferencedByRole_impl (CosRelationships::RelatedObject_ptr);
    virtual CosGraphs::PropagationValue externalize_propagation(const CosCompoundExternalization::RelationshipHandle& rel,
                                                                const char* to_role_name,
                                                                CORBA::Boolean& same_for_all);

};


class ExternalizationReference_impl : virtual public CosCompoundExternalization_impl::Relationship_impl,
				      virtual public POA_CosExternalizationReference::Relationship,
				      virtual public ::Reference_impl
{
public:    
    ExternalizationReference_impl ();
    ExternalizationReference_impl (CosRelationships::NamedRoles nr);
};


#endif

