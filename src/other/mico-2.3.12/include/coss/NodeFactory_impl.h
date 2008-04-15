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


#ifndef __NodeFactory_impl_h__
#define __NodeFactory_impl_h__

#include "CosGraphs.h"

// Implementation for interface NodeFactory
template <class N>
class NodeFactory_impl : virtual public POA_CosGraphs::NodeFactory
{
 public:
  NodeFactory_impl (CORBA::ORB_ptr _orb);
  CosGraphs::Node_ptr create_node (CORBA::Object_ptr related_object);
};


template <class N>
NodeFactory_impl<N>::NodeFactory_impl (CORBA::ORB_ptr _orb)
{
  // this is for binding to random server
  IdentifiableObject_impl* tmp = new IdentifiableObject_impl (_orb);
}
template <class N>
CosGraphs::Node_ptr
NodeFactory_impl<N>::create_node (CORBA::Object_ptr related_object)
{
#ifdef DEBUG
  cout << "NodeFactory_impl::create_node (...)\n";
#endif
  N* tmp_node = new N (CosRelationships::RelatedObject::_duplicate
		       (related_object));
  CosGraphs::Node_ptr node = tmp_node->_this ();

  return node;
}

#endif

