/*
 *  LifeCycle Service for MICO
 *  Copyright (C) 1999 Dmitry V. Sedov
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
 *                 sedov@postman.simcb.ru
 */

#include <CORBA.h>
#include <coss/LifePropagationCriteriaFactory_impl.h>
#include <coss/LifeTraversalCriteria_impl.h>

CosCompoundLifeCycle_impl::PropagationCriteriaFactory_impl::PropagationCriteriaFactory_impl()
  : POA_CosCompoundLifeCycle::PropagationCriteriaFactory ()
{}

CosGraphs::TraversalCriteria_ptr
CosCompoundLifeCycle_impl::PropagationCriteriaFactory_impl::create(CosCompoundLifeCycle::Operation op)
{
  CosCompoundLifeCycle_impl::TraversalCriteria_impl* tci = new CosCompoundLifeCycle_impl::TraversalCriteria_impl(op);
  CosGraphs::TraversalCriteria_ptr retval = tci->_this ();

  return retval; 
}    
