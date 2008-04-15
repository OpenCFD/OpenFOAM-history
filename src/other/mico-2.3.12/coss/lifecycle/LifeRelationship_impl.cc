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
#include <coss/LifeRelationship_impl.h>
#include <coss/RegisterHelper.h>

CosCompoundLifeCycle_impl::Relationship_impl::Relationship_impl ()
    : IdentifiableObject_impl(),
      ::Relationship_impl(),
      POA_CosCompoundLifeCycle::Relationship()
{
    factory_id_.length(4);
    factory_id_[0].id = CORBA::string_dup("::CosCompoundLifeCycle::Relationship");
    factory_id_[1].id = CORBA::string_dup("");
    factory_id_[2].id = CORBA::string_dup("");
    factory_id_[3].id = CORBA::string_dup("RelationshipFactory");
	factory_id_[0].kind = CORBA::string_dup(_lc_ks_object_interface);
	factory_id_[1].kind = CORBA::string_dup(_lc_ks_impl_equiv_class);
	factory_id_[2].kind = CORBA::string_dup(_lc_ks_object_implementation);
	factory_id_[3].kind = CORBA::string_dup(_lc_ks_factory_interface);

}
CosCompoundLifeCycle_impl::Relationship_impl::Relationship_impl (CosRelationships::NamedRoles nr)
    : IdentifiableObject_impl(),
      ::Relationship_impl(),
      POA_CosCompoundLifeCycle::Relationship()
{
    factory_id_.length(4);
    factory_id_[0].id = CORBA::string_dup("::CosCompoundLifeCycle::Relationship");
    factory_id_[1].id = CORBA::string_dup("");
    factory_id_[2].id = CORBA::string_dup("");
    factory_id_[3].id = CORBA::string_dup("RelationshipFactory");
	factory_id_[0].kind = CORBA::string_dup(_lc_ks_object_interface);
	factory_id_[1].kind = CORBA::string_dup(_lc_ks_impl_equiv_class);
	factory_id_[2].kind = CORBA::string_dup(_lc_ks_object_implementation);
	factory_id_[3].kind = CORBA::string_dup(_lc_ks_factory_interface);
    n_roles = nr;
}

CosCompoundLifeCycle::Relationship_ptr
CosCompoundLifeCycle_impl::Relationship_impl::copy_relationship(CosLifeCycle::FactoryFinder_ptr there,
                                                		const CosLifeCycle::Criteria& the_criteria,
                                            			const CosGraphs::NamedRoles& new_roles)
{

    CosLifeCycle::Factories* rel_factories_ptr = there -> find_factories(factory_id_);
    if (!(*rel_factories_ptr).length())
    {
	if (rel_factories_ptr) delete rel_factories_ptr;
	CosLifeCycle::NoFactory ex;
	ex.search_key = factory_id_;
	mico_throw(ex);
    }

    CosCompoundLifeCycle::Relationship_ptr new_rel_ptr;
    CORBA::ULong i;
    for (i = 0;i < (*rel_factories_ptr).length();++i)
    {
	CosRelationships::RelationshipFactory_ptr relfactory_ptr =
		CosRelationships::RelationshipFactory::_narrow((*rel_factories_ptr)[i]);
	if (CORBA::is_nil(relfactory_ptr))
	    continue;
	CosRelationships::NamedRoles tmp_roles;
	tmp_roles.length(new_roles.length());
	for(CORBA::ULong i = 0;i < new_roles.length();++i)
	{
	    tmp_roles[i].name = CORBA::string_dup(new_roles[i].the_name);
	    tmp_roles[i].aRole = CosRelationships::Role::_duplicate(new_roles[i].the_role);
	}
#ifdef HAVE_EXCEPTIONS
	try {
#endif
	    new_rel_ptr = CosCompoundLifeCycle::Relationship::_narrow((*relfactory_ptr).create(tmp_roles));
#ifdef HAVE_EXCEPTIONS
	} catch(...) {
	    CORBA::release(relfactory_ptr);
	    continue; 
	}
#endif
	CORBA::release(relfactory_ptr); 
	if (!CORBA::is_nil(new_rel_ptr))
	    break;
    }
    
    if (i == (*rel_factories_ptr).length())
    {
	if (rel_factories_ptr) delete rel_factories_ptr;
	CosLifeCycle::NoFactory ex;
	ex.search_key = factory_id_;
	mico_throw(ex);
    }

    if (rel_factories_ptr) delete rel_factories_ptr;
    return new_rel_ptr;
}

void
CosCompoundLifeCycle_impl::Relationship_impl::move_relationship(CosLifeCycle::FactoryFinder_ptr there,
                        					const CosLifeCycle::Criteria& the_criteria)
{
}

CosGraphs::PropagationValue
CosCompoundLifeCycle_impl::Relationship_impl::life_cycle_propagation(CosCompoundLifeCycle::Operation op,
                                                		     const char* from_role_name,
                                                		     const char* to_role_name,
                                                		     CORBA::Boolean& same_for_all)
{

    CosCompoundLifeCycle::RelationshipHandle tmp_rel_hand;
    tmp_rel_hand.constant_random_id = this -> constant_random_id();
    tmp_rel_hand.the_relationship = CosCompoundLifeCycle::Relationship::_duplicate(this->_this());
    CosRelationships::NamedRoles* relation_roles_ptr = this -> named_roles();
    CosCompoundLifeCycle::Role_var from_role_var;
    CORBA::Boolean from_ = false;
    for(CORBA::ULong i = 0;i < (*relation_roles_ptr).length();++i)
	if (!from_ && !strcmp((*relation_roles_ptr)[i].name,from_role_name))
	{
	    from_ = true;
	    from_role_var = CosCompoundLifeCycle::Role::_narrow((*relation_roles_ptr)[i].aRole);
	}
    delete relation_roles_ptr;
    if (!from_)
	return CosGraphs::none;
    return from_role_var -> life_cycle_propagation(op,tmp_rel_hand,to_role_name,same_for_all);
}

