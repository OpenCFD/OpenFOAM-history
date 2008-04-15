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


#ifndef __NamedRoleTypesHelper_h__
#define __NamedRoleTypesHelper_h__


#include "CosRelationships.h"


class NamedRoleTypesHelper
{
  CORBA::Repository_ptr repo;
  CosRelationships::RelationshipFactory::NamedRoleTypes* nr_types;
  //CORBA::Long last;
public:
  NamedRoleTypesHelper (CORBA::ORB_var orb);
  ~NamedRoleTypesHelper ();

  CosRelationships::RelationshipFactory::NamedRoleTypes* 
    get_named_role_types ();
  CORBA::Long length ();
  void length (CORBA::Long x);
  void add (const char* id, const char* name);
};

#endif
