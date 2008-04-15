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


#include <coss/CosRelationships.h>
#include <coss/IdentifiableObject_impl.h>

#ifndef __Relationship_impl_h__
#define __Relationship_impl_h__


class Relationship_impl : virtual public IdentifiableObject_impl,
			  virtual public POA_CosRelationships::Relationship

{
protected:
  CosRelationships::NamedRoles n_roles;
public:
  Relationship_impl ();
  ~Relationship_impl ();
  Relationship_impl (CosRelationships::NamedRoles nr);
  virtual CosRelationships::NamedRoles* named_roles ();
  virtual void destroy ();  
};


#endif


