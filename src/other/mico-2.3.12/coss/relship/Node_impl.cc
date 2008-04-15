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


#include <coss/Node_impl.h>

//#define DEBUG 1

#define USE_DUPLICATE 1

// Implementation for interface Node

Node_impl::Node_impl () 
  : IdentifiableObject_impl ()
    , POA_CosGraphs::Node ()
{
  _roles = new CosGraphs::Node::Roles ();
  _roles->length (0);

}

Node_impl::Node_impl (CosRelationships::RelatedObject_ptr related_object) 
  : IdentifiableObject_impl ()
    , POA_CosGraphs::Node ()
{
  _roles = new CosGraphs::Node::Roles ();
  _roles->length (0);
  _related_object = CosRelationships::RelatedObject::_duplicate 
    (related_object);
}


Node_impl::~Node_impl () 
{
#ifdef DEBUG
  cout << "~Node_impl ()\n";
#endif
  _roles->length (0);
  delete _roles;
  CORBA::release (_related_object);
}


CosRelationships::RelatedObject_ptr 
Node_impl::related_object ()
{
  return CosRelationships::RelatedObject::_duplicate (_related_object); 
}

CosGraphs::Node::Roles* 
Node_impl::roles_of_node ()
{
#ifdef DEBUG
  cout << "Node_impl::roles_of_node ()\n";
#endif

  CosGraphs::Node::Roles* tmp_roles = new CosGraphs::Node::Roles;
  int length = _roles->length ();
  tmp_roles->length (length);
  
  for (int i=0; i<length; i++) {
#if USE_DUPLICATE
    (*tmp_roles)[i] = CosGraphs::Role::_duplicate ((*_roles)[i]);
#else
    (*tmp_roles)[i] = (*_roles)[i];
#endif
  }

  return tmp_roles;
}


CosGraphs::Node::Roles*
Node_impl::roles_of_type (CORBA::InterfaceDef_ptr role_type)
{
  CosGraphs::Node::Roles* tmp_roles = new CosGraphs::Node::Roles;
  tmp_roles->length (0);
  int length = _roles->length ();
  //cout << "id: " << (role_type->describe_interface ())->id << "\n";
  //cout << "name: " << (role_type->describe_interface ())->name << "\n";
  char* repoid = CORBA::string_dup ((role_type->describe_interface ())->id);
  for (int i=0; i<length; i++) {
    //if (role_type->is_a ((*_roles)[i]->_repoid ())) {
    if ((*_roles)[i]->_get_interface ()->is_a (repoid)) {
      //cout << (*_roles)[i]->_repoid () << " is of this type\n";
      tmp_roles->length (tmp_roles->length () + 1);
      (*tmp_roles)[tmp_roles->length () - 1] = (*_roles)[i];
    }
  }

  return tmp_roles; 
}


void
Node_impl::add_role (CosGraphs::Role_ptr a_role)
{
#ifdef DEBUG
  cout << "Node_impl::add_role ()\n";
#endif
  // is role of same type in _roles ??
  for (unsigned int i = 0; i<_roles->length (); i++) {
    if (a_role->_get_interface ()->is_a ((*_roles)[i]->_repoid ()))
      mico_throw (CosGraphs::Node::DuplicateRoleType ());
    if ((*_roles)[i]->_get_interface ()->is_a (a_role->_repoid ()))
      mico_throw (CosGraphs::Node::DuplicateRoleType ());
  }
#ifdef DEBUG
  cout << "adding role...\n";
#endif
  _roles->length (_roles->length () + 1);
  (*_roles)[_roles->length () - 1] = CosGraphs::Role::_duplicate (a_role);

}

void
Node_impl::remove_role (unsigned int index) {
  for (unsigned int i=index; i<_roles->length () - 1; i++)
    (*_roles)[i] = (*_roles)[i+1];
  _roles->length (_roles->length () - 1);
}


void
Node_impl::remove_role (CORBA::InterfaceDef_ptr of_type)
{
  // find index of removed role
  CORBA::Boolean was_found = FALSE;
  char* repoid = CORBA::string_dup (of_type->describe_interface ()->id);
  //cout << "type: " << repoid << "\n";
  //cout << "length: " << _roles->length () << "\n";
  //unsigned int index = _roles->length () + 1;
  for (unsigned int i = 0; i<_roles->length (); i++) {
    //cout << "role: " << (*_roles)[i]->_repoid () << " : " << i << "\n";
    //if (of_type->is_a ((*_roles)[i]->_repoid ())) {
    if ((*_roles)[i]->_get_interface ()->is_a (repoid)) {
      //cout << "is of type: " << repoid << "\n";
      remove_role (i);
      //if (i > 0) {
      // we must change index :-))
      i--;
      //}
      was_found = TRUE;
    }
  }
  /*
  if (index <= _roles->length ()) {
    for (unsigned int i = index; i<_roles->length () - 1; i++) {
      (*_roles)[i] = (*_roles)[i + 1];
    }
    _roles->length (_roles->length () - 1);
  }
  else
  */
  if (!was_found)
    mico_throw (CosGraphs::Node::NoSuchRole ());
  
}

void
Node_impl::set_related_object (CosRelationships::RelatedObject_ptr o)
{
#ifdef DEBUG
  cout << "Node_impl::set_related_object (...)\n";
#endif
  _related_object = CosRelationships::RelatedObject::_duplicate (o);
}









