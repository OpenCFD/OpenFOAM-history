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


#ifndef __Node_impl_h__
#define __Node_impl_h__

//#include "CosObjectIdentity.h"
#include "CosRelationships.h"
#include "CosGraphs.h"
#include "IdentifiableObject_impl.h"


// Implementation for interface Node
class Node_impl : 
  virtual public IdentifiableObject_impl,
  virtual public POA_CosGraphs::Node
{
protected:
  CosRelationships::RelatedObject_ptr _related_object;
  CosGraphs::Node::Roles* _roles;
  //CORBA::InterfaceDef_ptr

public:
  Node_impl ();
  Node_impl (CosRelationships::RelatedObject_ptr related_object);
  
  ~Node_impl ();

  CosRelationships::RelatedObject_ptr related_object();
  
  CosGraphs::Node::Roles* roles_of_node();
  
  CosGraphs::Node::Roles* roles_of_type (CORBA::InterfaceDef_ptr role_type);
  
  void add_role (CosGraphs::Role_ptr a_role);
  
  void remove_role (CORBA::InterfaceDef_ptr of_type);

  // extension but not visible from CORBA infrastructure - used only for constructor designed pattern :-))
  void set_related_object (CosRelationships::RelatedObject_ptr o);
private:
  void remove_role (unsigned int index);
};

#endif



