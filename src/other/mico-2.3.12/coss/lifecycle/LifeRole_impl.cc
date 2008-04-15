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
#include <coss/LifeRole_impl.h>
#include <coss/RegisterHelper.h>

CosCompoundLifeCycle_impl::Role_impl::Role_impl()
    : ::Role_impl(),
      CosGraphs_impl::Role_impl(),
      POA_CosCompoundLifeCycle::Role ()
{
    factory_id_.length(4);
    factory_id_[0].id = CORBA::string_dup("::CosCompoundLifeCycle::Role");
    factory_id_[1].id = CORBA::string_dup("");
    factory_id_[2].id = CORBA::string_dup("");
    factory_id_[3].id = CORBA::string_dup("RoleFactory");
    factory_id_[0].kind = CORBA::string_dup(_lc_ks_object_interface);
    factory_id_[1].kind = CORBA::string_dup(_lc_ks_impl_equiv_class);
    factory_id_[2].kind = CORBA::string_dup(_lc_ks_object_implementation);
    factory_id_[3].kind = CORBA::string_dup(_lc_ks_factory_interface);
}

CosCompoundLifeCycle_impl::Role_impl::Role_impl(CosRelationships::RelatedObject_ptr obj)
    : ::Role_impl(),
      CosGraphs_impl::Role_impl(),
      POA_CosCompoundLifeCycle::Role ()
{
    if (!CORBA::is_nil(obj) && CORBA::is_nil(CosCompoundLifeCycle::Node::_narrow(obj)))
    {
	CosRelationships::RoleFactory::RelatedObjectTypeError ex;
	mico_throw(ex);
    }
    factory_id_.length(4);
    factory_id_[0].id = CORBA::string_dup("::CosCompoundLifeCycle::Role");
    factory_id_[1].id = CORBA::string_dup("");
    factory_id_[2].id = CORBA::string_dup("");
    factory_id_[3].id = CORBA::string_dup("RoleFactory");
    factory_id_[0].kind = CORBA::string_dup(_lc_ks_object_interface);
    factory_id_[1].kind = CORBA::string_dup(_lc_ks_impl_equiv_class);
    factory_id_[2].kind = CORBA::string_dup(_lc_ks_object_implementation);
    factory_id_[3].kind = CORBA::string_dup(_lc_ks_factory_interface);
    rel_obj = obj;
}

CosCompoundLifeCycle::Role_ptr
CosCompoundLifeCycle_impl::Role_impl::copy_role(CosLifeCycle::FactoryFinder_ptr there,
                            			const CosLifeCycle::Criteria& the_criteria)
{

    CosLifeCycle::Factories* role_factories_ptr = there -> find_factories(factory_id_);
    if (!role_factories_ptr || !(*role_factories_ptr).length())
    {
	if (role_factories_ptr) delete role_factories_ptr;
	CosLifeCycle::NoFactory ex;
	ex.search_key = factory_id_;
	mico_throw(ex);
    }
    CosRelationships::RelatedObject_ptr tmp_rel_ptr;
    CORBA::ULong i;
    for(i = 0;i < the_criteria.length();++i)
	if (!strcmp(the_criteria[i].name,"reference to owner node"))
	{
	    the_criteria[i].value >>= tmp_rel_ptr;
	    break;
	}
    if (i == the_criteria.length())
    {
	if (role_factories_ptr) delete role_factories_ptr;
	CosLifeCycle::InvalidCriteria ex;
	ex.invalid_criteria = the_criteria;
	mico_throw(ex);
    }

    CosCompoundLifeCycle::Role_ptr new_role_ptr;
    for (i = 0;i < (*role_factories_ptr).length();++i)
    {
	CosRelationships::RoleFactory_ptr rolefactory_ptr =
		CosRelationships::RoleFactory::_narrow((*role_factories_ptr)[i]);
	if (CORBA::is_nil(rolefactory_ptr))
	    continue;
#ifdef HAVE_EXCEPTIONS
	try {
#endif
	    new_role_ptr = CosCompoundLifeCycle::Role::_narrow(rolefactory_ptr->create_role(tmp_rel_ptr));
#ifdef HAVE_EXCEPTIONS
	} catch(...) {
	    CORBA::release(rolefactory_ptr);
	    continue;
	}
#endif
	CORBA::release(rolefactory_ptr);
	if (!CORBA::is_nil(new_role_ptr))
	    break;
    }
    
    if (i == (*role_factories_ptr).length())
    {
	if (role_factories_ptr) delete role_factories_ptr;
	CosLifeCycle::NoFactory ex;
	ex.search_key = factory_id_;
	mico_throw(ex);
    }

    if (role_factories_ptr) delete role_factories_ptr;

    return new_role_ptr;
}

void
CosCompoundLifeCycle_impl::Role_impl::move_role(CosLifeCycle::FactoryFinder_ptr there,
                				const CosLifeCycle::Criteria& the_criteria)
{

    for(CORBA::ULong i = 0;i < the_criteria.length();++i)
	if (!strcmp(the_criteria[i].name,"reference to owner node"))
	{
	    CosRelationships::RelatedObject_ptr tmp_rel_ptr;
	    the_criteria[i].value >>= tmp_rel_ptr;
	    CORBA::release(this->rel_obj);
	    this -> rel_obj = CosRelationships::RelatedObject::_duplicate(tmp_rel_ptr);
	    return;
	}
    CosLifeCycle::InvalidCriteria ex;
    ex.invalid_criteria = the_criteria;
    mico_throw(ex);
}

