/*
 *  Relationship Service for MICO
 *  Copyright (C) 1998-99 Karel Gardas
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
 *                 gardask@alpha.inf.upol.cz
 */


#include <coss/TraversalCriteria_impl.h>

//#define DEBUG 1

#define USE_DUPLICATE 1


using namespace std;

TraversalCriteria_impl::TraversalCriteria_impl 
(CosGraphs::PropagationValue value)
  : POA_CosGraphs::TraversalCriteria ()
{
#if DEBUG
  cout << "TraversalCriteria_impl::TraversalCriteria_impl ()\n";
#endif
  propagation_value = value;
  edges = new vector<CosGraphs::Edge*>;
  iterator = NULL;
}

void
TraversalCriteria_impl::visit_node (const CosGraphs::NodeHandle& a_node, 
				    CosGraphs::Mode search_mode)
{
  if (edges != NULL) {
    vector<CosGraphs::Edge*>::iterator i;
    for (i = edges->begin (); i != edges->end (); i++)
      delete *i;
    delete edges;
  }

  edges = new vector<CosGraphs::Edge*>;
  assert (!CORBA::is_nil (a_node.the_node.in()));
  CosGraphs::Node::Roles* roles_of_node = a_node.the_node->roles_of_node ();
  for (CORBA::ULong i = 0; i<roles_of_node->length (); i++) {
    if (!CORBA::is_nil (iterator)) {
      iterator->destroy ();
      CORBA::release (iterator);
    }
    CosGraphs::Edges* tmp_edges;
    CosGraphs::Edge* edge;
    (*roles_of_node)[i]->get_edges (0, tmp_edges, iterator);
    while (iterator-> next_one (edge)) {
      //add_edge (edge);
      edges->push_back (edge);
    }
    delete tmp_edges;
    edges_iterator = edges->begin ();
  }
  delete roles_of_node;
}


CORBA::Boolean
TraversalCriteria_impl::next_one 
(CosGraphs::TraversalCriteria::WeightedEdge_out the_edge)
{
  if (edges_iterator == edges->end ()) {
    the_edge = new CosGraphs::TraversalCriteria::WeightedEdge;
    return false;
  }

  the_edge = new CosGraphs::TraversalCriteria::WeightedEdge;
#if USE_DUPLICATE
  // from
  the_edge->the_edge.from.the_role.the_role
    = CosGraphs::Role::_duplicate ((*edges_iterator)->from.the_role.the_role);
  the_edge->the_edge.from.the_role.the_name
    = CORBA::string_dup ((*edges_iterator)->from.the_role.the_name);
  the_edge->the_edge.from.the_node.the_node = CosGraphs::Node::_duplicate
    ((*edges_iterator)->from.the_node.the_node);
  the_edge->the_edge.from.the_node.constant_random_id
    = the_edge->the_edge.from.the_node.the_node->constant_random_id ();
  
  // the_relationship
  the_edge->the_edge.the_relationship.constant_random_id
    = (*edges_iterator)->the_relationship.constant_random_id;
  the_edge->the_edge.the_relationship.the_relationship
    = CosRelationships::Relationship::_duplicate
    ((*edges_iterator)->the_relationship.the_relationship);

  // relatives
  the_edge->the_edge.relatives.length ((*edges_iterator)->relatives.length ());
  for (CORBA::ULong i=0; i<(*edges_iterator)->relatives.length (); i++) {
    the_edge->the_edge.relatives[i].the_role.the_role
      = CosGraphs::Role::_duplicate
      ((*edges_iterator)->relatives[i].the_role.the_role);
    the_edge->the_edge.relatives[i].the_role.the_name
      = CORBA::string_dup ((*edges_iterator)->relatives[i].the_role.the_name);
    the_edge->the_edge.relatives[i].the_node.the_node 
      = CosGraphs::Node::_duplicate
      ((*edges_iterator)->relatives[i].the_node.the_node);
    the_edge->the_edge.relatives[i].the_node.constant_random_id
      = the_edge->the_edge.relatives[i].the_node.the_node->constant_random_id 
      ();
  }

  the_edge->weight = 0;
  the_edge->next_nodes.length ((*edges_iterator)->relatives.length ());
  {
    for (CORBA::ULong i = 0; i<the_edge->next_nodes.length (); i++) {
      the_edge->next_nodes[i].the_node = CosGraphs::Node::_duplicate 
        ((*edges_iterator)->relatives[i].the_node.the_node);
      the_edge->next_nodes[i].constant_random_id 
        = the_edge->next_nodes[i].the_node->constant_random_id ();
    }
  }
#else
  the_edge->the_edge = *(*edges_iterator);
  the_edge->weight = 0;   // this feature is not implemented yet
  the_edge->next_nodes.length ((*edges_iterator)->relatives.length ());
  {
    for (CORBA::ULong i = 0; i<the_edge->next_nodes.length (); i++) {
      the_edge->next_nodes[i] = (*edges_iterator)->relatives[i].the_node;
    }
  }
#endif
  edges_iterator++;

  return true; 
}


CORBA::Boolean
TraversalCriteria_impl::next_n 
(CORBA::Short how_many, 
 CosGraphs::TraversalCriteria::WeightedEdges_out the_edges)
{
  CORBA::Boolean retval = false;
  CORBA::Short counter = 0;
  the_edges = new CosGraphs::TraversalCriteria::WeightedEdges ();

  CosGraphs::TraversalCriteria::WeightedEdge* tmp_edge;
  while (this->next_one (tmp_edge)) {
    the_edges->length (the_edges->length () + 1);
    (*the_edges)[the_edges->length () - 1] = *tmp_edge;
    if (counter == how_many) {
      retval = true;
      break;
    }
  }

  return retval; 
}


void
TraversalCriteria_impl::destroy ()
{
  PortableServer::ObjectId* oid = _default_POA ()->servant_to_id (this);
  _default_POA ()->deactivate_object (*oid);

  delete oid;
  delete this;

}
