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
#include <coss/LifeTraversalCriteria_impl.h>
#include <coss/CosGraphs.h>

CosCompoundLifeCycle_impl::TraversalCriteria_impl::TraversalCriteria_impl(
	    CosCompoundLifeCycle::Operation op)
    : ::TraversalCriteria_impl (CosGraphs::none)
{
    op_ = op;
};

// ----------------------------------------------------------------------
// CosCompoundLifeCycle_TraversalCriteria_impl public member implementation
// ----------------------------------------------------------------------

CORBA::Boolean
CosCompoundLifeCycle_impl::TraversalCriteria_impl::next_one(CosGraphs::TraversalCriteria::WeightedEdge*& the_edge)
{
    CORBA::Boolean all_none = TRUE;
    while (all_none && (edges_iterator != edges->end()))
    {
	the_edge = new CosGraphs::TraversalCriteria::WeightedEdge;
        the_edge->the_edge = *(*edges_iterator);
	the_edge->weight = 0;   // this feature is not implemented yet
	CosCompoundLifeCycle::Role_ptr tmp_role_ptr =
		    CosCompoundLifeCycle::Role::_narrow((*edges_iterator)->from.the_role.the_role);
	if (CORBA::is_nil(tmp_role_ptr))
	{
	    the_edge->next_nodes.length ((*edges_iterator)->relatives.length());
	    for (CORBA::ULong i = 0; i<the_edge->next_nodes.length (); i++)
		the_edge->next_nodes[i] = (*edges_iterator)->relatives[i].the_node;
	    all_none = FALSE;
	}
	else
	{
	    CosCompoundLifeCycle::RelationshipHandle tmp_relship;
	    tmp_relship.constant_random_id =
		(*edges_iterator)->the_relationship.constant_random_id;
	    tmp_relship.the_relationship =
		CosCompoundLifeCycle::Relationship::_narrow((*edges_iterator)->the_relationship.the_relationship);
	    CORBA::Boolean same_of_all;
	    CosGraphs::PropagationValue prop;
	    for(CORBA::ULong i = 0;i < (*edges_iterator)->relatives.length();++i)
	    {
		prop = (*tmp_role_ptr).life_cycle_propagation(
			this->op_,tmp_relship,(*edges_iterator)->relatives[i].the_role.the_name,same_of_all);
		switch (prop)
		{
		case CosGraphs::deep:
		    (*the_edge).next_nodes.length((*the_edge).next_nodes.length()+1);
		    (*the_edge).next_nodes[(*the_edge).next_nodes.length()-1] =
			(*edges_iterator)->relatives[i].the_node;
		    all_none = FALSE;
		    break;
		case CosGraphs::shallow:
		    all_none = FALSE;
		    break;
	        case CosGraphs::inhibit:
		    throw ::CORBA::NO_IMPLEMENT();
	        case CosGraphs::none:
		    break;
		}
	    }
	}
	if (all_none)
	{
	    delete the_edge;
	    edges_iterator++;
	    continue;
	}
	edges_iterator++;

	return true; 
    }
    the_edge = new CosGraphs::TraversalCriteria::WeightedEdge;
    return false;
}

