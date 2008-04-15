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
#include <coss/LifeNode_impl.h>
#include <coss/RegisterHelper.h>

CosCompoundLifeCycle_impl::Node_impl::Node_impl ()
    : IdentifiableObject_impl ()
    , ::Node_impl()
    , POA_CosCompoundLifeCycle::Node ()
{
    factory_id_.length(4);
    factory_id_[0].id = CORBA::string_dup("::CosCompoundLifeCycle::Node");
    factory_id_[1].id = CORBA::string_dup("");
    factory_id_[2].id = CORBA::string_dup("");
    factory_id_[3].id = CORBA::string_dup("NodeFactory");
	factory_id_[0].kind = CORBA::string_dup(_lc_ks_object_interface);
	factory_id_[1].kind = CORBA::string_dup(_lc_ks_impl_equiv_class);
	factory_id_[2].kind = CORBA::string_dup(_lc_ks_object_implementation);
	factory_id_[3].kind = CORBA::string_dup(_lc_ks_factory_interface);

};

CosCompoundLifeCycle_impl::Node_impl::Node_impl (CosRelationships::RelatedObject_ptr related_object)
    : IdentifiableObject_impl ()
    , ::Node_impl()
    , POA_CosCompoundLifeCycle::Node ()
{
    factory_id_.length(4);
    factory_id_[0].id = CORBA::string_dup("::CosCompoundLifeCycle::Node");
    factory_id_[1].id = CORBA::string_dup("");
    factory_id_[2].id = CORBA::string_dup("");
    factory_id_[3].id = CORBA::string_dup("NodeFactory");
	factory_id_[0].kind = CORBA::string_dup(_lc_ks_object_interface);
	factory_id_[1].kind = CORBA::string_dup(_lc_ks_impl_equiv_class);
	factory_id_[2].kind = CORBA::string_dup(_lc_ks_object_implementation);
	factory_id_[3].kind = CORBA::string_dup(_lc_ks_factory_interface);
    _related_object = CosRelationships::RelatedObject::_duplicate(related_object);
};

void
CosCompoundLifeCycle_impl::Node_impl::copy_node(CosLifeCycle::FactoryFinder_ptr there,
					  const CosLifeCycle::Criteria& the_criteria,
					  CosCompoundLifeCycle::Node_out new_node,
					  CosGraphs::Node::Roles_out roles_of_new_node)
{
    CosLifeCycle::Factories* node_factories_ptr = there -> find_factories(factory_id_);
    if (!(*node_factories_ptr).length())
    {
	if (node_factories_ptr) delete node_factories_ptr;
	CosLifeCycle::NoFactory ex;
	ex.search_key = factory_id_;
	mico_throw(ex);
    }

    CosLifeCycle::LifeCycleObject_ptr life_object_ptr =
	CosLifeCycle::LifeCycleObject::_narrow(this -> related_object());
    if (CORBA::is_nil(life_object_ptr))
    {
	if (node_factories_ptr) delete node_factories_ptr;
	if (life_object_ptr) delete life_object_ptr;
	CosLifeCycle::NotCopyable ex;
	ex.reason = CORBA::string_dup("Related object of node not supported CosLifeCycle::LifeCycleObject interface");
	mico_throw(ex);
    }
    CosLifeCycle::LifeCycleObject_ptr newlife_object_ptr;
#ifdef HAVE_EXCEPTIONS
    try {
#endif
	newlife_object_ptr = life_object_ptr -> copy(there,the_criteria);
#ifdef HAVE_EXCEPTIONS
    } catch(...) {
	if (node_factories_ptr) delete node_factories_ptr;
	CORBA::release(life_object_ptr);
	CosLifeCycle::NotCopyable ex;
	ex.reason = CORBA::string_dup("Can`t copy Nodes related object because copy return exception.");
	mico_throw(ex);
    }	
#endif

    CosCompoundLifeCycle::Node_ptr new_node_ptr;
    CORBA::ULong i;
    for (i = 0;i < (*node_factories_ptr).length();++i)
    {
	CosGraphs::NodeFactory_ptr nodefactory_ptr =
		CosGraphs::NodeFactory::_narrow((*node_factories_ptr)[i]);
	if (CORBA::is_nil(nodefactory_ptr))
	    continue;
#ifdef HAVE_EXCEPTIONS
	try {
#endif
	    new_node_ptr = CosCompoundLifeCycle::Node::_narrow((*nodefactory_ptr).create_node(newlife_object_ptr));
#ifdef HAVE_EXCEPTIONS
	} catch(...) {
	    CORBA::release(nodefactory_ptr);
	    continue;
	}
#endif
	CORBA::release(nodefactory_ptr);
	if (!CORBA::is_nil(new_node_ptr))
	    break;
    }
    
    if (i == (*node_factories_ptr).length())
    {
	if (node_factories_ptr) delete node_factories_ptr;
	CORBA::release(life_object_ptr);
	(*newlife_object_ptr).remove();
	CosLifeCycle::NoFactory ex;
	ex.search_key = factory_id_;
	mico_throw(ex);
    }
    if (node_factories_ptr) delete node_factories_ptr;
    CORBA::release(life_object_ptr);
    CORBA::release(newlife_object_ptr);

    CosLifeCycle::Criteria new_criteria = the_criteria;
    new_criteria.length(new_criteria.length()+1);
    new_criteria[new_criteria.length()-1].name = CORBA::string_dup("reference to owner node");
    new_criteria[new_criteria.length()-1].value <<= new_node_ptr;

    CosGraphs::Node::Roles* roles_of_node_ = this->roles_of_node();
    for(i = 0;i < (*roles_of_node_).length();++i)
    {
	CosCompoundLifeCycle::Role_ptr tmp_role_ptr =
		    CosCompoundLifeCycle::Role::_narrow((*roles_of_node_)[i]);
	if (CORBA::is_nil(tmp_role_ptr))
	{
	    (*new_node_ptr).remove_node();
	    delete roles_of_node_;
	    CosLifeCycle::NotCopyable ex;
	    ex.reason = CORBA::string_dup("Role of node not supported CosCompoundLifeCycle::Role interface");
	    mico_throw(ex);
	}
	CosCompoundLifeCycle::Role_ptr new_role_ptr;
#ifdef HAVE_EXCEPTIONS
	try {
#endif
	    new_role_ptr = (*tmp_role_ptr).copy_role(there,new_criteria);
#ifdef HAVE_EXCEPTIONS
	} catch(...) {
	    (*new_node_ptr).remove_node();
	    delete roles_of_node_;
	    CosLifeCycle::NotCopyable ex;
	    ex.reason = CORBA::string_dup("Can`t create Node object because CosCompoundLifeCycle::Role::copy_role return exception.");
	    mico_throw(ex);
	}
#endif
	(*new_node_ptr).add_role(CosGraphs::Role::_duplicate(new_role_ptr));
    }
    new_node = new_node_ptr;
    roles_of_new_node = (*new_node_ptr).roles_of_node();
    delete roles_of_node_;
}

void
CosCompoundLifeCycle_impl::Node_impl::move_node(CosLifeCycle::FactoryFinder_ptr there,
					  const CosLifeCycle::Criteria& the_criteria)
{
    CosLifeCycle::LifeCycleObject_ptr life_object_ptr =
		CosLifeCycle::LifeCycleObject::_narrow(this -> related_object());
    if (CORBA::is_nil(life_object_ptr))
    {
	CosLifeCycle::NotMovable ex;
	ex.reason = CORBA::string_dup("Related object of node not supported CosLifeCycle::LifeCycleObject interface");
	mico_throw(ex);
    }

#ifdef HAVE_EXCEPTIONS
    try {
#endif
	life_object_ptr -> move(there,the_criteria);
#ifdef HAVE_EXCEPTIONS
    } catch(...) {
	CosLifeCycle::NotMovable ex;
	ex.reason = CORBA::string_dup("Can`t move Node object because CosLifeCycle::LifeObject::move return exception.");
	mico_throw(ex);
    }
#endif
    CosLifeCycle::Criteria new_criteria = the_criteria;
    new_criteria.length(new_criteria.length()+1);
    new_criteria[new_criteria.length()-1].name = CORBA::string_dup("reference to owner node");
    new_criteria[new_criteria.length()-1].value <<= (*this)._this();

    CosGraphs::Node::Roles* roles_of_node_ = this->roles_of_node();
    for(CORBA::ULong i = 0;i < (*roles_of_node_).length();++i)
    {
	CosCompoundLifeCycle::Role_ptr tmp_role_ptr =
		    CosCompoundLifeCycle::Role::_narrow((*roles_of_node_)[i]);
	if (CORBA::is_nil(tmp_role_ptr))
	{
	    delete roles_of_node_;
	    CosLifeCycle::NotMovable ex;
	    ex.reason = CORBA::string_dup("Role of node not supported CosCompoundLifeCycle::Role interface");
	    mico_throw(ex);
	}
#ifdef HAVE_EXCEPTIONS
	try {
#endif
	    (*tmp_role_ptr).move_role(there,new_criteria);
#ifdef HAVE_EXCEPTIONS
	} catch(...) {
	    delete roles_of_node_;
	    CosLifeCycle::NotMovable ex;
	    ex.reason = CORBA::string_dup("Can`t move Node object because CosCompoundLifeCycle::Role::move_role return exception.");
	    mico_throw(ex);
	}
#endif
    }
}

void
CosCompoundLifeCycle_impl::Node_impl::remove_node()
{
    CosGraphs::Node::Roles* node_roles_ptr = this -> roles_of_node();
    CORBA::ULong len_ = node_roles_ptr->length();
    for(CORBA::ULong i = 0; i < len_;++i)
    {
	this->remove_role((*node_roles_ptr)[i].in()->_get_interface());
#ifdef HAVE_EXCEPTIONS
	try {
#endif
	    (*node_roles_ptr)[i]->destroy();
#ifdef HAVE_EXCEPTIONS
	} catch(...) {
	    delete node_roles_ptr;
	    CosLifeCycle::NotRemovable ex;
	    ex.reason = CORBA::string_dup("Cann`t destroy role.");
	    mico_throw(ex);
	}
#endif
    }
    delete node_roles_ptr;
#if USE_VAR
    PortableServer::ObjectId_var oid = _default_POA ()->servant_to_id (this);
#else
    PortableServer::ObjectId* oid = _default_POA ()->servant_to_id (this);
#endif
    _default_POA ()->deactivate_object (*oid);

#ifndef USE_VAR
    delete oid;
#endif
    delete this;
}

CosLifeCycle::LifeCycleObject_ptr
CosCompoundLifeCycle_impl::Node_impl::get_life_cycle_object()
{
    CosLifeCycle::LifeCycleObject_ptr narrow_node_ptr =
	    CosLifeCycle::LifeCycleObject::_narrow((CosCompoundLifeCycle::Node_ptr)this);
    if (!CORBA::is_nil(narrow_node_ptr))
	return CosLifeCycle::LifeCycleObject::_duplicate(narrow_node_ptr);
    CosCompoundLifeCycle::Node::NotLifeCycleObject ex;
    mico_throw (ex);
    return (CosLifeCycle::LifeCycleObject_ptr)NULL;
}

