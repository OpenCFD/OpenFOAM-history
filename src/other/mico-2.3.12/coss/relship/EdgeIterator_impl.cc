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

#include <coss/EdgeIterator_impl.h>

//#define DEBUG 1


#define USE_DUPLICATE 1

// Implementation for interface EdgeIterator

EdgeIterator_impl::EdgeIterator_impl (CosGraphs::Edges* edges)
{
#ifdef DEBUG
  cout << "EdgeIterator_impl::EdgeIterator_impl (...)\n";
#endif
  _edges = edges;
  index = 0;
}

CORBA::Boolean
EdgeIterator_impl::next_one (CosGraphs::Edge_out the_edge)
{
#ifdef DEBUG
  cout << "EdgeIterator_impl::next_one (...);\n";
  cout << "_edges->length () : " << _edges->length () << "\n";
  cout << "index : " << index << "\n";
#endif

  CORBA::Boolean retval = false;

  // i thing this is special hack for mico which can't handle returned nil 
  // reference but it can be defined in idl2c++ mapping
  the_edge = new CosGraphs::Edge ();

  if (_edges->length () > index) {
#if USE_DUPLICATE
#if DEBUG
    cout << "EdgeIterator_impl::next_one (...) USE_DUPLICATE\n";
#endif
    // from
    the_edge->from.the_role.the_role 
      = CosGraphs::Role::_duplicate ((*_edges)[index].from.the_role.the_role);
    the_edge->from.the_role.the_name 
      = CORBA::string_dup ((*_edges)[index].from.the_role.the_name);
    the_edge->from.the_node.the_node = CosGraphs::Node::_duplicate 
      ((*_edges)[index].from.the_node.the_node);
    the_edge->from.the_node.constant_random_id 
      = the_edge->from.the_node.the_node->constant_random_id (); 

    // the_relationship
    the_edge->the_relationship.constant_random_id 
      = (*_edges)[index].the_relationship.constant_random_id;
    the_edge->the_relationship.the_relationship
      = CosRelationships::Relationship::_duplicate 
      ((*_edges)[index].the_relationship.the_relationship);
    
    // relatives
    the_edge->relatives.length ((*_edges)[index].relatives.length ());
    for (CORBA::ULong i=0; i<(*_edges)[index].relatives.length (); i++) {
      the_edge->relatives[i].the_role.the_role 
	= CosGraphs::Role::_duplicate 
	((*_edges)[index].relatives[i].the_role.the_role);
      the_edge->relatives[i].the_role.the_name 
	= CORBA::string_dup ((*_edges)[index].relatives[i].the_role.the_name);
      the_edge->relatives[i].the_node.the_node = CosGraphs::Node::_duplicate 
	((*_edges)[index].relatives[i].the_node.the_node);
      the_edge->relatives[i].the_node.constant_random_id 
	= the_edge->relatives[i].the_node.the_node->constant_random_id (); 
    }
#else
    the_edge->from = (*_edges)[index].from;
    the_edge->the_relationship = (*_edges)[index].the_relationship;
    the_edge->relatives = (*_edges)[index].relatives;
#endif

    index++;
    retval = true;
  }
  
  return retval; 
}


CORBA::Boolean
EdgeIterator_impl::next_n (CORBA::ULong how_many, 
			   CosGraphs::Edges_out the_edges)
{
  CORBA::Boolean retval = how_many < _edges->length () - index ? true : false;

  the_edges = new CosGraphs::Edges ();
  CORBA::ULong max 
    = how_many <= _edges->length () - index ? how_many 
    : _edges->length () - index;

  the_edges->length (max);
  CosGraphs::Edge* tmp_edge;
  for (CORBA::ULong i = 0; i<max; i++) {
    this->next_one (tmp_edge);
    /*
    (*the_edges)[i].from = tmp_edge->from;
    (*the_edges)[i].the_relationship = tmp_edge->the_relationship;
    (*the_edges)[i].relatives = tmp_edge->relatives;
    */
    (*the_edges)[i] = *tmp_edge;
    delete tmp_edge;
  }

  return retval; 
}


void
EdgeIterator_impl::destroy()
{

  PortableServer::ObjectId* oid = _default_POA ()->servant_to_id (this);
  _default_POA ()->deactivate_object (*oid);
  delete oid;
  
  delete _edges;
  delete this;
}

