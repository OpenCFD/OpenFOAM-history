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

#include <coss/GenericCriteriaFactory_impl.h>
#include <coss/TraversalCriteria_impl.h>

//#define DEBUG 1

GenericCriteriaFactory_impl::GenericCriteriaFactory_impl () 
  : POA_CosGraphsExtension::GenericCriteriaFactory ()
{
#if DEBUG
  cout << "GenericCriteriaFactory_impl::GenericCriteriaFactory_impl ()\n";
#endif
}


CosGraphs::TraversalCriteria_ptr
GenericCriteriaFactory_impl::create 
(CosGraphs::PropagationValue propagation_value)
{
  TraversalCriteria_impl* tci = new TraversalCriteria_impl (propagation_value);
  CosGraphs::TraversalCriteria_ptr retval = tci->_this ();

  return retval; 
}

