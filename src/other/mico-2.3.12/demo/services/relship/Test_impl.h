/*
 *  Test of COSS Relationship Service for MICO
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

#ifndef __Test_impl_h__
#define __Test_impl_h__


#include "Test.h"
#include <coss/GraphsRole_impl.h>
#include <coss/Relationship_impl.h>


class BaseRole_impl : virtual public CosGraphs_impl::Role_impl,
		      virtual public POA_Test::BaseRole
{
public:
  BaseRole_impl ();
  BaseRole_impl (CosRelationships::RelatedObject_ptr);

};


class DerivedRole1_impl : virtual public BaseRole_impl,
			  virtual public POA_Test::DerivedRole1
{
public:
  DerivedRole1_impl ();
  DerivedRole1_impl (CosRelationships::RelatedObject_ptr);

};

class DerivedRole2_impl : virtual public BaseRole_impl,
			  virtual public POA_Test::DerivedRole2
{
public:
  DerivedRole2_impl ();
  DerivedRole2_impl (CosRelationships::RelatedObject_ptr);

};


class TestRelationship_impl : virtual public Relationship_impl,
			      virtual public POA_Test::TestRelationship
{
public:    
  TestRelationship_impl ();
  TestRelationship_impl (CosRelationships::NamedRoles nr);
};


#endif
