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


#include <coss/TraversalFactory_impl.h>
#include <coss/Traversal_impl.h>

//#define DEBUG 1


TraversalFactory_impl::TraversalFactory_impl ()
  : POA_CosGraphs::TraversalFactory ()
{
#if DEBUG
  cout << "TraversalFactory_impl::TraversalFactory_impl ()\n";
#endif
}

CosGraphs::Traversal_ptr
TraversalFactory_impl::create_traversal_on 
(const CosGraphs::NodeHandle& root_node, 
 CosGraphs::TraversalCriteria_ptr the_criteria, CosGraphs::Mode how )
{
#if DEBUG
  cout << "TraversalFactory_impl::create_traversal_on (...)\n";
#endif
  Traversal_impl* tr = new Traversal_impl 
    (root_node, CosGraphs::TraversalCriteria::_duplicate (the_criteria), how);
  CosGraphs::Traversal_ptr traversal = tr->_this ();

  return traversal; 
}


