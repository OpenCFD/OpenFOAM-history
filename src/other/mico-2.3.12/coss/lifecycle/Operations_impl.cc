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
#include <coss/Operations_impl.h>
#include <coss/LifeTraversalCriteria_impl.h>
#include <coss/Traversal_impl.h>
#include <map>
#include <list>
#include <algorithm>

typedef std::map<RoleKey,CosGraphs::Role_ptr,std::less<RoleKey> > WorkedRoles;
typedef std::list<CosObjectIdentity::ObjectIdentifier> WorkedRelationships;
typedef std::list<CosGraphs::Traversal::TraversalScopedId> WorkedNodes;

Operations_impl::Operations_impl()
    : POA_CosCompoundLifeCycle::Operations()
{
}

CosCompoundLifeCycle::Node_ptr
Operations_impl::copy(CosCompoundLifeCycle::Node_ptr starting_node,
        	      CosLifeCycle::FactoryFinder_ptr there,
        	      const CosLifeCycle::Criteria& the_criteria)
{

    CosCompoundLifeCycle::Node_ptr new_root_node_ptr;
    WorkedRoles NewRoles;
// Copy Root Node
    {
	CosCompoundLifeCycle::Node_ptr current_node_ptr =
		    CosCompoundLifeCycle::Node::_narrow(starting_node);
	if (CORBA::is_nil(current_node_ptr))
	{
	    CosLifeCycle::NotCopyable ex;
	    ex.reason = CORBA::string_dup("Node not supported CosCompoundLifeCycle::Node interface.");
	    mico_throw(ex);
	}
	CosCompoundLifeCycle::Node_ptr new_node_ptr;
	CosGraphs::Node::Roles* new_roles_of_node_ptr;
	current_node_ptr->copy_node(there,the_criteria,new_node_ptr,new_roles_of_node_ptr);
	for(CORBA::ULong j = 0;j < (*new_roles_of_node_ptr).length();++j)
	{
	    CORBA::InterfaceDef_ptr tmp_intdef =
			(*new_roles_of_node_ptr)[j]->_get_interface();
	    CosObjectIdentity::ObjectIdentifier id = starting_node->constant_random_id();
	    RoleKey new_rec(id,tmp_intdef);
	    NewRoles[new_rec] = CosGraphs::Role::_duplicate((*new_roles_of_node_ptr)[j]);
	    CORBA::release(tmp_intdef);
	}
	new_root_node_ptr = new_node_ptr;
    }
    
// Copy others Node
    CosCompoundLifeCycle_impl::TraversalCriteria_impl* trav_crit_ptr =
		new CosCompoundLifeCycle_impl::TraversalCriteria_impl(CosCompoundLifeCycle::copy);
    CosGraphs::TraversalCriteria_ptr trav_criteria = trav_crit_ptr->_this();
    CosGraphs::NodeHandle root_node;
    root_node.the_node = starting_node;
    root_node.constant_random_id = starting_node->constant_random_id();
    CosGraphs::Mode mode = CosGraphs::depthFirst;
    Traversal_impl* trav_impl = new Traversal_impl(root_node,trav_criteria,mode);
    CosGraphs::Traversal_ptr traversal = trav_impl->_this();
    CosGraphs::Traversal::ScopedEdges* OriginalEdgeList =
	new CosGraphs::Traversal::ScopedEdges;
    (*OriginalEdgeList).length(0);
    CosGraphs::Traversal::ScopedEdge* tmp_edge = NULL;
    while (traversal->next_one(tmp_edge))
    {
	(*OriginalEdgeList).length((*OriginalEdgeList).length()+1);
	(*OriginalEdgeList)[(*OriginalEdgeList).length()-1] = *tmp_edge;
	if (tmp_edge) delete tmp_edge;
    }
    traversal->destroy();
    trav_criteria->destroy();
    for(CORBA::ULong i = 0;i < (*OriginalEdgeList).length();++i)
    {
	CosCompoundLifeCycle::Node_ptr current_node_ptr =
		    CosCompoundLifeCycle::Node::_narrow((*OriginalEdgeList)[i].from.point.the_node.the_node.in());
	if (CORBA::is_nil(current_node_ptr))
	{
	    CosLifeCycle::NotCopyable ex;
	    ex.reason = CORBA::string_dup("Node not supported CosCompoundLifeCycle::Node interface.");
	    mico_throw(ex);
	}
	CORBA::Boolean find = false;
	for(WorkedRoles::iterator j = NewRoles.begin();j != NewRoles.end();++j)
	    if ((*j).first.id == (*OriginalEdgeList)[i].from.point.the_node.constant_random_id)
	    {
		find = true;
		break;
	    }
	if (find)
	    continue;
	CosCompoundLifeCycle::Node_ptr new_node_ptr;
	CosGraphs::Node::Roles* new_roles_of_node_ptr;
	current_node_ptr->copy_node(there,the_criteria,new_node_ptr,new_roles_of_node_ptr);
	for(CORBA::ULong j = 0;j < (*new_roles_of_node_ptr).length();++j)
	{
	    CORBA::InterfaceDef_ptr tmp_intdef =
			(*new_roles_of_node_ptr)[j]->_get_interface();
	    CosObjectIdentity::ObjectIdentifier id =
		    (*OriginalEdgeList)[i].from.point.the_node.constant_random_id;
	    RoleKey new_rec(id,tmp_intdef);
	    NewRoles[new_rec] = CosGraphs::Role::_duplicate((*new_roles_of_node_ptr)[j]);
	    CORBA::release(tmp_intdef);
	}
	
    }
    WorkedRelationships workedRelationships;
    for(CORBA::ULong i = 0;i < (*OriginalEdgeList).length();++i)
    {
	if (std::find(workedRelationships.begin(),
		  workedRelationships.end(),
		  (*OriginalEdgeList)[i].the_relationship.scoped_relationship.constant_random_id) !=
	   workedRelationships.end())
	    continue;
	CosCompoundLifeCycle::Relationship_ptr current_relship_ptr =
		    CosCompoundLifeCycle::Relationship::_narrow((*OriginalEdgeList)[i].the_relationship.scoped_relationship.the_relationship.in());
	if (CORBA::is_nil(current_relship_ptr))
	{
	    if (OriginalEdgeList) delete OriginalEdgeList;
	    CosLifeCycle::NotCopyable ex;
	    ex.reason = CORBA::string_dup("Relationship not supported CosCompoundLifeCycle::Relationship interface.");
	    mico_throw(ex);
	}
	CosGraphs::NamedRoles new_roles;
	new_roles.length(0);
	CORBA::InterfaceDef_ptr tmp_intdef =
		(*OriginalEdgeList)[i].from.point.the_role.the_role->_get_interface();
	CosObjectIdentity::ObjectIdentifier id =
		(*OriginalEdgeList)[i].from.point.the_node.constant_random_id;
	RoleKey new_rec(id,tmp_intdef);
	CORBA::release(tmp_intdef);
	WorkedRoles::iterator it = NewRoles.find(new_rec);
	if (it == NewRoles.end())
	{
	    new_roles.length(new_roles.length()+1);
	    new_roles[new_roles.length()-1] = (*OriginalEdgeList)[i].from.point.the_role;
	}
	else
	{
	    new_roles.length(new_roles.length()+1);
	    new_roles[new_roles.length()-1].the_name = CORBA::string_dup((*OriginalEdgeList)[i].from.point.the_role.the_name);
	    new_roles[new_roles.length()-1].the_role = CosGraphs::Role::_duplicate((*it).second);
	}
	for(CORBA::ULong j = 0;j < (*OriginalEdgeList)[i].relatives.length();++j)
	{
	    CORBA::InterfaceDef_ptr tmp_intdef =
		    (*OriginalEdgeList)[i].relatives[j].point.the_role.the_role->_get_interface();
	    CosObjectIdentity::ObjectIdentifier id =
		    (*OriginalEdgeList)[i].relatives[j].point.the_node.constant_random_id;
	    RoleKey new_rec(id,tmp_intdef);
	    WorkedRoles::iterator it = NewRoles.find(new_rec);
	    CORBA::release(tmp_intdef);
	    if (it == NewRoles.end())
	    {
		new_roles.length(new_roles.length()+1);
		new_roles[new_roles.length()-1] = (*OriginalEdgeList)[i].relatives[j].point.the_role;
	    }
	    else
	    {
		new_roles.length(new_roles.length()+1);
		new_roles[new_roles.length()-1].the_name = CORBA::string_dup((*OriginalEdgeList)[i].relatives[j].point.the_role.the_name);
		new_roles[new_roles.length()-1].the_role = CosGraphs::Role::_duplicate((*it).second);
	    }
	}
	CORBA::Boolean same_for_all;
	if (current_relship_ptr->life_cycle_propagation(CosCompoundLifeCycle::copy,
		CORBA::string_dup((*OriginalEdgeList)[i].from.point.the_role.the_name),
		CORBA::string_dup((*OriginalEdgeList)[i].relatives[0].point.the_role.the_name),
		same_for_all) == CosGraphs::deep)
	    CosCompoundLifeCycle::Relationship_ptr new_relationship =
		    current_relship_ptr->copy_relationship(there,the_criteria,new_roles);

	workedRelationships.push_back(
	    (*OriginalEdgeList)[i].the_relationship.scoped_relationship.constant_random_id);
    }
    if (OriginalEdgeList) delete OriginalEdgeList;
    return new_root_node_ptr;

}

void
Operations_impl::move(CosCompoundLifeCycle::Node_ptr starting_node,
        	      CosLifeCycle::FactoryFinder_ptr there,
        	      const CosLifeCycle::Criteria& the_criteria)
{
    CosCompoundLifeCycle_impl::TraversalCriteria_impl* trav_crit_ptr =
		new CosCompoundLifeCycle_impl::TraversalCriteria_impl(CosCompoundLifeCycle::move);
    CosGraphs::TraversalCriteria_ptr trav_criteria = trav_crit_ptr->_this();
    CosGraphs::NodeHandle root_node;
    root_node.the_node = starting_node;
    root_node.constant_random_id = starting_node->constant_random_id();
    CosGraphs::Mode mode = CosGraphs::depthFirst;
    Traversal_impl* trav_impl = new Traversal_impl(root_node,trav_criteria,mode);
    CosGraphs::Traversal_ptr traversal = trav_impl->_this();
    CosGraphs::Traversal::ScopedEdges* OriginalEdgeList =
	new CosGraphs::Traversal::ScopedEdges;
    (*OriginalEdgeList).length(0);
    CosGraphs::Traversal::ScopedEdge* tmp_edge = NULL;
    while (traversal->next_one(tmp_edge))
    {
	(*OriginalEdgeList).length((*OriginalEdgeList).length()+1);
	(*OriginalEdgeList)[(*OriginalEdgeList).length()-1] = *tmp_edge;
	if (tmp_edge) delete tmp_edge;
    }
    traversal->destroy();
    trav_criteria->destroy();
    WorkedNodes MovedNodes;
    for(CORBA::ULong i = 0;i < (*OriginalEdgeList).length();++i)
    {
	CosCompoundLifeCycle::Node_ptr current_node_ptr =
		    CosCompoundLifeCycle::Node::_narrow((*OriginalEdgeList)[i].from.point.the_node.the_node.in());
	if (CORBA::is_nil(current_node_ptr))
	{
	    if (OriginalEdgeList) delete OriginalEdgeList;
	    CosLifeCycle::NotMovable ex;
	    ex.reason = CORBA::string_dup("Node not supported CosCompoundLifeCycle::Node interface.");
	    mico_throw (ex);
	}
	if (std::find(MovedNodes.begin(),MovedNodes.end(),(*OriginalEdgeList)[i].from.point.the_node.constant_random_id) !=
		MovedNodes.end())
	    continue;
	current_node_ptr->move_node(there,the_criteria);
	MovedNodes.push_back((*OriginalEdgeList)[i].from.point.the_node.constant_random_id);
    }
    if (OriginalEdgeList) delete OriginalEdgeList;
}

void
Operations_impl::remove(CosCompoundLifeCycle::Node_ptr& starting_node)
{

    CosCompoundLifeCycle_impl::TraversalCriteria_impl* trav_crit_ptr =
		new CosCompoundLifeCycle_impl::TraversalCriteria_impl(CosCompoundLifeCycle::remove);
    CosGraphs::TraversalCriteria_ptr trav_criteria = trav_crit_ptr->_this();
    CosGraphs::NodeHandle root_node;
    root_node.the_node = starting_node;
    root_node.constant_random_id = starting_node->constant_random_id();
    CosGraphs::Mode mode = CosGraphs::depthFirst;
    Traversal_impl* trav_impl = new Traversal_impl(root_node,trav_criteria,mode);
    CosGraphs::Traversal_ptr traversal = trav_impl->_this();
    CosGraphs::Traversal::ScopedEdges* OriginalEdgeList =
	new CosGraphs::Traversal::ScopedEdges;
    (*OriginalEdgeList).length(0);
    CosGraphs::Traversal::ScopedEdge* tmp_edge = NULL;
    WorkedNodes Nodes4Remove;
    WorkedRelationships Rel4Remove;
    while (traversal->next_one(tmp_edge))
    {
	(*OriginalEdgeList).length((*OriginalEdgeList).length()+1);
	(*OriginalEdgeList)[(*OriginalEdgeList).length()-1] = *tmp_edge;
	if (tmp_edge) delete tmp_edge;
    }
    traversal->destroy();
    trav_criteria->destroy();

    for(CORBA::ULong i = 0;i < (*OriginalEdgeList).length();++i)
    {
	if (std::find(Rel4Remove.begin(),Rel4Remove.end(),
		(*OriginalEdgeList)[i].the_relationship.scoped_relationship.constant_random_id) !=
	    Rel4Remove.end())
	    continue;
	CosCompoundLifeCycle::Relationship_ptr tmp_rel_ptr =
		    CosCompoundLifeCycle::Relationship::_narrow((*OriginalEdgeList)[i].the_relationship.scoped_relationship.the_relationship);
	if (CORBA::is_nil(tmp_rel_ptr))
	{
	    if (OriginalEdgeList) delete OriginalEdgeList;
	    CosLifeCycle::NotRemovable ex;
	    ex.reason = CORBA::string_dup("Relationship not supported CosCompoundLifeCycle::Relationship interface.");
	    mico_throw(ex);
	}
	CORBA::Boolean same_for_all;
	if (tmp_rel_ptr->life_cycle_propagation(CosCompoundLifeCycle::remove,
		CORBA::string_dup((*OriginalEdgeList)[i].from.point.the_role.the_name),
		CORBA::string_dup((*OriginalEdgeList)[i].relatives[0].point.the_role.the_name),
		same_for_all) == CosGraphs::deep)
	    tmp_rel_ptr->destroy();
	Rel4Remove.push_back((*OriginalEdgeList)[i].the_relationship.scoped_relationship.constant_random_id);
    }
    for(CORBA::ULong i = 0;i < (*OriginalEdgeList).length();++i)
    {
	if (std::find(Nodes4Remove.begin(),Nodes4Remove.end(),(*OriginalEdgeList)[i].from.point.the_node.constant_random_id) !=
	    Nodes4Remove.end())
	    continue;
	CosCompoundLifeCycle::Node_ptr tmp_node_ptr =
		    CosCompoundLifeCycle::Node::_narrow((*OriginalEdgeList)[i].from.point.the_node.the_node.in());
	if (CORBA::is_nil(tmp_node_ptr))
	{
	    if (OriginalEdgeList) delete OriginalEdgeList;
	    CosLifeCycle::NotRemovable ex;
	    ex.reason = CORBA::string_dup("Node not supported CosCompoundLifeCycle::Node interface.");
	    mico_throw(ex);
	}
	tmp_node_ptr->remove_node();
	tmp_node_ptr = (CosCompoundLifeCycle::Node_ptr)NULL;
	Nodes4Remove.push_back((*OriginalEdgeList)[i].from.point.the_node.constant_random_id);
    }
    starting_node = (CosCompoundLifeCycle::Node_ptr)NULL;
}

void
Operations_impl::destroy()
{
    PortableServer::ObjectId* oid = _default_POA ()->servant_to_id (this);
    _default_POA ()->deactivate_object (*oid);

    delete oid;
    delete this;
}




