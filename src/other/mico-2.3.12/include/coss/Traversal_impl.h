// -*- c++ -*-
/*
 *  Relationship Service for MICO
 *  Copyright (C) 1998-99 Karel Gardas 
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  Send comments and/or bug reports to:
 *                 mico@informatik.uni-frankfurt.de
 *  or to my private e-mail:
 *                 gardask@alpha.inf.upol.cz
 */


#ifndef __Traversal_impl__
#define __Traversal_impl__

#include "CosObjectIdentity.h"
#include "CosGraphs.h"


class Traversal_impl : virtual public POA_CosGraphs::Traversal
{
public:
    struct EdgeId
    {
	EdgeId() { };
	EdgeId(const EdgeId& r) : node_id(r.node_id), relation_id(r.relation_id) { };
	EdgeId(const CosGraphs::Edge& edge)
	    : node_id(edge.from.the_node.constant_random_id),
	      relation_id(edge.the_relationship.constant_random_id) { };
	~EdgeId() { };
	EdgeId& operator=(const EdgeId& r)
	{
	    if (this != &r)
	    {
		node_id = r.node_id;
		relation_id = r.relation_id;
	    }
	    return *this;
	}
	CosObjectIdentity::ObjectIdentifier node_id;
	CosObjectIdentity::ObjectIdentifier relation_id;
    };
private:
  CosGraphs::NodeHandle root;
  CosGraphs::TraversalCriteria* criteria;
  CosGraphs::Mode mode;

  std::list<CosGraphs::TraversalCriteria::WeightedEdge*>* w_edges;
  std::vector<EdgeId>* visited_ids;

  CosGraphs::Traversal::TraversalScopedId counter;
protected:
  void traverse ();
  void traverse (CosGraphs::NodeHandle node);
  CORBA::Boolean visited (EdgeId id);
  void was_visited (EdgeId id);
  CosGraphs::Traversal::ScopedEdge *WeightedEdge2ScopedEdge 
    (CosGraphs::TraversalCriteria::WeightedEdge* edge);

public:
  Traversal_impl (const CosGraphs::NodeHandle& root_node, 
		  CosGraphs::TraversalCriteria_ptr the_criteria, 
		  CosGraphs::Mode how);
  ~Traversal_impl ();
  CORBA::Boolean next_one (CosGraphs::Traversal::ScopedEdge_out the_edge);
  CORBA::Boolean next_n (CORBA::Short how_many, 
			 CosGraphs::Traversal::ScopedEdges_out the_edges);
  void destroy();
};

#endif

