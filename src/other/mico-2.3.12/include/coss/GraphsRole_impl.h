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


#ifndef __GraphsRole_impl_h__
#define __GraphsRole_impl_h__

#include "CosGraphs.h"
#include "Role_impl.h"

namespace CosGraphs_impl {

  // Implementation for interface Role
				    
  class Role_impl 
    : virtual public ::Role_impl, 
      virtual public POA_CosGraphs::Role
  {
  protected:
    int findThisRole (CosRelationships::RelationshipHandle rh);
    //Edge get_edge (CosRelationships::RelationshipHandle rh);
    CosGraphs::Edge* get_edge (CosRelationships::RelationshipHandle rh);
  public:
    Role_impl ();
    Role_impl (CosRelationships::RelatedObject_ptr);

    void get_edges (CORBA::Long how_many, CosGraphs::Edges_out the_edges, 
		    CosGraphs::EdgeIterator_out the_rest);
  };

}

#endif


