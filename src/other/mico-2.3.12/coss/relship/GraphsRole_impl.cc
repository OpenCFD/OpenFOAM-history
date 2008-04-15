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


#include <coss/GraphsRole_impl.h>
#include <coss/EdgeIterator_impl.h>

//#define DEBUG 1


CosGraphs_impl::Role_impl::Role_impl ()
  : ::Role_impl (),
      POA_CosGraphs::Role ()
{
#if DEBUG
  cout << "  <CosGraphs::Role_impl> constructor\n";
#endif
}


CosGraphs_impl::Role_impl::Role_impl (CosRelationships::RelatedObject_ptr obj)
  : POA_CosGraphs::Role (), ::Role_impl ()
{
#if DEBUG
  cout << "  <CosGraphs::Role_impl> constructor with params\n";
#endif
  rel_obj = obj;
}


int
CosGraphs_impl::Role_impl::findThisRole (CosRelationships::RelationshipHandle rh)
{
#ifdef DEBUG
  cout << "CosGraphs::Role_impl::findThisRole (rh)\n";
#endif

  CosRelationships::Role_ptr role = _this ();
  CosRelationships::NamedRoles* roles = rh.the_relationship->named_roles ();
  int index = -1;
  for (unsigned int i = 0; i<roles->length (); i++) {
    if (role->_is_equivalent ((*roles)[i].aRole)) {
      index = i;
      break;
    }
  }
  delete roles;
  CORBA::release (role);
#ifdef DEBUG 
  cout << "CosGraphs::Role_impl::findThisRole (rh) -> " << index << "\n";
#endif
  return index;
}
      

CosGraphs::Edge*
CosGraphs_impl::Role_impl::get_edge (CosRelationships::RelationshipHandle rh)
{
#ifdef DEBUG
  cout << "CosGraphs::Role_impl::get_edge (rh)\n";
#endif

  CosRelationships::NamedRoles* roles = rh.the_relationship->named_roles ();
  int index = findThisRole (rh);
  
  CosGraphs::Edge* edge = new CosGraphs::Edge;

  // from
  edge->from.the_role.the_role = CosGraphs::Role::_narrow 
    ((*roles)[index].aRole);
  edge->from.the_role.the_name = CORBA::string_dup ((*roles)[index].name);
  edge->from.the_node.the_node = CosGraphs::Node::_narrow 
    ((*roles)[index].aRole->related_object ());
  edge->from.the_node.constant_random_id 
    = CosGraphs::Node::_narrow ((*roles)[index].aRole->related_object ())
    ->constant_random_id (); 

  // the_relationship
  //edge->the_relationship = rh;
  edge->the_relationship.constant_random_id = rh.constant_random_id;
  edge->the_relationship.the_relationship
    = CosRelationships::Relationship::_duplicate (rh.the_relationship);

  // relatives
  edge->relatives.length (roles->length () - 1);
  for (int i = 0; i<index; i++) {
#ifdef DEBUG
    cout << 1 << ". role: " << (*roles)[i].name << "\n";
#endif

    edge->relatives[i].the_role.the_role = CosGraphs::Role::_narrow 
      ((*roles)[i].aRole);
    edge->relatives[i].the_role.the_name 
      = CORBA::string_dup ((*roles)[i].name);

    edge->relatives[i].the_node.the_node 
      = CosGraphs::Node::_narrow ((*roles)[i].aRole->related_object ());
    edge->relatives[i].the_node.constant_random_id
      = CosGraphs::Node::_narrow 
      ((*roles)[i].aRole->related_object ())->constant_random_id ();
#ifdef DEBUG
    cout << i << ". node: " << edge->relatives[i].the_node.constant_random_id 
	 << "\n";
#endif
  }
  {
    for (unsigned int i = index + 1; i<roles->length (); i++) {
      edge->relatives[i-1].the_role.the_role = CosGraphs::Role::_narrow 
        ((*roles)[i].aRole);
      edge->relatives[i-1].the_role.the_name 
        = CORBA::string_dup ((*roles)[i].name);
#ifdef DEBUG
      cout << i << ". role: " << edge->relatives[i-1].the_role.the_name << "\n";
#endif
      edge->relatives[i-1].the_node.the_node 
        = CosGraphs::Node::_narrow ((*roles)[i].aRole->related_object ());
      edge->relatives[i-1].the_node.constant_random_id
        = CosGraphs::Node::_narrow 
      ((*roles)[i].aRole->related_object ())->constant_random_id ();
#ifdef DEBUG
      cout << i << ". node: " << edge->relatives[i-1].the_node.constant_random_id 
           << "\n";
#endif
    }
  }

  delete roles;
  return edge;
}


void
CosGraphs_impl::Role_impl::get_edges (CORBA::Long how_many, 
                                      CosGraphs::Edges_out the_edges, 
                                      CosGraphs::EdgeIterator_out the_rest)
{
#ifdef DEBUG
  cout << "CosGraphs::Role_impl::get_edges (" << how_many << ", the_edges, "
       << "the_rest)\n";
#endif
  the_edges = new CosGraphs::Edges ();
  CORBA::ULong max = (CORBA::ULong)how_many <= rel_ships.length () 
    ? how_many : rel_ships.length ();
  the_edges->length (max);
#ifdef DEBUG
  cout << "CosGraphs::Role_impl::get_edges: max = " << max 
       << " : relship.length () = " << rel_ships.length () << "\n";
#endif
  CosGraphs::Edge* tmp_edge;
  for (CORBA::ULong i=0; i<max; i++) {
    tmp_edge = get_edge (rel_ships[i]);
    (*the_edges)[i] = (*tmp_edge);
    delete tmp_edge;
    // (*the_edges)[i] = (*get_edge (rel_ships[i]));
  }

  CosGraphs::Edges* tmp_edges = new CosGraphs::Edges ();
  if (max != rel_ships.length ()) {
    tmp_edges->length (rel_ships.length () - max);

    for (CORBA::ULong i=max; i<rel_ships.length (); i++) {
      tmp_edge = get_edge (rel_ships[i]);
      (*tmp_edges)[i - max] = (*tmp_edge);
      delete tmp_edge;
      //(*tmp_edges)[i - max] = (*get_edge (rel_ships[i]));
    }
  }
#ifdef DEBUG
  cout << "creating EdgeIterator ...\n";
#endif

  EdgeIterator_impl* iterator = new EdgeIterator_impl (tmp_edges);
  the_rest = iterator->_this ();
  
}
