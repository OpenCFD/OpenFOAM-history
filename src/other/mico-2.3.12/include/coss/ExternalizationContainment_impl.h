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
#include <coss/CosContainment_impl.h>
#include <coss/CosExternalizationContainment.h>


#ifndef __ExternalizationContainment_impl_h__
#define __ExternalizationContainment_impl_h__


class ExternalizationContainsRole_impl : virtual public CosCompoundExternalization_impl::Role_impl,
					 virtual public POA_CosExternalizationContainment::ContainsRole,
					 virtual public ::ContainsRole_impl
{
public:
    ExternalizationContainsRole_impl ();
    ExternalizationContainsRole_impl (CosRelationships::RelatedObject_ptr);
    virtual CosGraphs::PropagationValue externalize_propagation(const CosCompoundExternalization::RelationshipHandle& rel,
                                                	        const char* to_role_name,
                                                    	        CORBA::Boolean& same_for_all);
};


class ExternalizationContainedInRole_impl : virtual public CosCompoundExternalization_impl::Role_impl,
					    virtual public POA_CosExternalizationContainment::ContainedInRole,
					    virtual public ::ContainedInRole_impl
{
public:
    ExternalizationContainedInRole_impl ();
    ExternalizationContainedInRole_impl (CosRelationships::RelatedObject_ptr);
    virtual CosGraphs::PropagationValue externalize_propagation(const CosCompoundExternalization::RelationshipHandle& rel,
                                                	        const char* to_role_name,
                                                                CORBA::Boolean& same_for_all);
};


class ExternalizationContainment_impl : virtual public CosCompoundExternalization_impl::Relationship_impl,
					virtual public POA_CosExternalizationContainment::Relationship,
					virtual public ::Containment_impl
{
public:
    ExternalizationContainment_impl ();
    ExternalizationContainment_impl (CosRelationships::NamedRoles nr);
};


#endif /* __ExternalizationContainment_impl_h__ */

