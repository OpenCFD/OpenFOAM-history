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


#ifndef __GenericCriteriaFactory_impl_h__
#define __GenericCriteriaFactory_impl_h__

#include "CosGraphsExtension.h"


class GenericCriteriaFactory_impl 
: virtual public POA_CosGraphsExtension::GenericCriteriaFactory
{
public:
  GenericCriteriaFactory_impl ();
  CosGraphs::TraversalCriteria_ptr create 
    (CosGraphs::PropagationValue propagation_value);
};


#endif

