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


#include <coss/Traversal_impl.h>

#define USE_DUPLICATE 1


using namespace std;

bool operator== ( const Traversal_impl::EdgeId& e1,
		  const Traversal_impl::EdgeId& e2 )
{
    if ((e1.node_id == e2.node_id) && (e1.relation_id == e2.relation_id))
	return true;
    return false;
}

Traversal_impl::Traversal_impl (const CosGraphs::NodeHandle& root_node, 
				CosGraphs::TraversalCriteria_ptr the_criteria, 
				CosGraphs::Mode how) 
{
  root = root_node;
  criteria = CosGraphs::TraversalCriteria::_duplicate (the_criteria);
  assert (!CORBA::is_nil (criteria));
  mode = how;
  counter = 0;
  
  w_edges = new list<CosGraphs::TraversalCriteria::WeightedEdge*>;
  visited_ids = new vector<EdgeId>;

  traverse ();
}


Traversal_impl::~Traversal_impl ()
{
  list<CosGraphs::TraversalCriteria::WeightedEdge*>::iterator i;
  for (i = w_edges->begin (); i != w_edges->end (); i++)
    delete *i;
  delete w_edges;
  delete visited_ids;
}


void
Traversal_impl::traverse (CosGraphs::NodeHandle node) 
{
  criteria->visit_node (node, mode);
  CosGraphs::TraversalCriteria::WeightedEdge* wedge;
  while (criteria->next_one (wedge)) {
    if (mode == CosGraphs::depthFirst)
      w_edges->push_front (wedge);
    if (mode == CosGraphs::breadthFirst)
      w_edges->push_back (wedge);
    if (mode == CosGraphs::bestFirst)
      mico_throw (::CORBA::NO_IMPLEMENT ());
  }

}


void
Traversal_impl::traverse () 
{
  traverse (root);
}


CORBA::Boolean
Traversal_impl::visited (EdgeId id) 
{
  vector<EdgeId>::iterator i;
  for (i = visited_ids->begin (); i != visited_ids->end (); i++) {
    if (*i == id)
      return true;
  }
  return false;
}

void
Traversal_impl::was_visited (EdgeId id) 
{
  visited_ids->push_back (id);
}


CosGraphs::Traversal::ScopedEdge*
Traversal_impl::WeightedEdge2ScopedEdge 
(CosGraphs::TraversalCriteria::WeightedEdge* edge) 
{
  CosGraphs::Traversal::ScopedEdge* the_edge
    = new CosGraphs::Traversal::ScopedEdge;
#if USE_DUPLICATE
  the_edge->from.id = counter++;

  // from
  the_edge->from.point.the_role.the_role
    = CosGraphs::Role::_duplicate (edge->the_edge.from.the_role.the_role);
  the_edge->from.point.the_role.the_name
    = CORBA::string_dup (edge->the_edge.from.the_role.the_name);
  the_edge->from.point.the_node.the_node = CosGraphs::Node::_duplicate
    (edge->the_edge.from.the_node.the_node);
  the_edge->from.point.the_node.constant_random_id
    = the_edge->from.point.the_node.the_node->constant_random_id ();
 
  // the_relationship
  the_edge->the_relationship.scoped_relationship.constant_random_id
    = edge->the_edge.the_relationship.constant_random_id;
  the_edge->the_relationship.scoped_relationship.the_relationship
    = CosRelationships::Relationship::_duplicate
    (edge->the_edge.the_relationship.the_relationship);
  
  // relatives
  the_edge->relatives.length (edge->the_edge.relatives.length ());
  for (CORBA::ULong i=0; i<edge->the_edge.relatives.length (); i++) {
    the_edge->relatives[i].point.the_role.the_role
      = CosGraphs::Role::_duplicate
      (edge->the_edge.relatives[i].the_role.the_role);
    the_edge->relatives[i].point.the_role.the_name
      = CORBA::string_dup (edge->the_edge.relatives[i].the_role.the_name);
    the_edge->relatives[i].point.the_node.the_node = CosGraphs::Node::_duplicate
      (edge->the_edge.relatives[i].the_node.the_node);
    the_edge->relatives[i].point.the_node.constant_random_id
      = the_edge->relatives[i].point.the_node.the_node->constant_random_id ();
  }
  
#else
  the_edge->from.id = counter++;
  the_edge->from.point = edge->the_edge.from;
  the_edge->the_relationship.scoped_relationship 
    = edge->the_edge.the_relationship;
  the_edge->the_relationship.id = counter++;
  the_edge->relatives.length (edge->the_edge.relatives.length ());
  for (CORBA::ULong i = 0; i<edge->the_edge.relatives.length (); i++) {
    the_edge->relatives[i].point = edge->the_edge.relatives[i];
    the_edge->relatives[i].id = counter++;
  }
#endif

  return the_edge;
}
  


CORBA::Boolean
Traversal_impl::next_one (CosGraphs::Traversal::ScopedEdge_out the_edge) 
{
  the_edge = NULL;
  for (;;) {
    if (w_edges->size () > 0) {
      EdgeId _id((*w_edges->begin())->the_edge);
      if (!visited (_id)) {
	was_visited (_id);
	the_edge = WeightedEdge2ScopedEdge (*(w_edges->begin()));
	CosGraphs::TraversalCriteria::WeightedEdge* tmp_edge = *(w_edges->begin());
	w_edges->pop_front ();
	for (CORBA::ULong i = 0; i<tmp_edge->next_nodes.length (); i++) {
	  traverse (tmp_edge->next_nodes[i]);
	}
	delete tmp_edge;
	return true;
      }
      else {
	// remove visited edge
	CosGraphs::TraversalCriteria::WeightedEdge* tmp_edge = *(w_edges->begin());
	w_edges->pop_front ();
	delete tmp_edge;
      }
    }
    else {
#if 0
      // w_edges if empty
      if (the_edge == NULL) {
#endif
	// create edge for correct deleting it in marshalling code!!!
	the_edge = new CosGraphs::Traversal::ScopedEdge;
#if 0
      }
#endif
      return false; 
    }
  }
}


CORBA::Boolean
Traversal_impl::next_n (CORBA::Short how_many, 
			CosGraphs::Traversal::ScopedEdges_out the_edges)
{
  CORBA::Boolean retval = false;
  CORBA::Short counter = 0;
  the_edges = new CosGraphs::Traversal::ScopedEdges ();

  CosGraphs::Traversal::ScopedEdge* tmp_edge;
  while (this->next_one (tmp_edge)) {
    the_edges->length (the_edges->length () + 1);
    (*the_edges)[the_edges->length () - 1] = *tmp_edge;
    if (++counter == how_many) {
      retval = true;
      break;
    }
  }
  
  return retval; 
}


void
Traversal_impl::destroy()
{
  PortableServer::ObjectId* oid = _default_POA ()->servant_to_id (this);
  _default_POA ()->deactivate_object (*oid);

  delete oid;
  delete this;
}








