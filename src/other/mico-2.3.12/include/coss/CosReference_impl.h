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

#include "CosReference.h"
#include "CosGraphs.h"
#include "GraphsRole_impl.h"
#include "Relationship_impl.h"


#ifndef __CosReference_impl_h__
#define __CosReference_impl_h__


class ReferencesRole_impl : virtual public CosGraphs_impl::Role_impl,
                            virtual public POA_CosReference::ReferencesRole
{
public:
  ReferencesRole_impl ();
  ReferencesRole_impl (CosRelationships::RelatedObject_ptr);

};


class ReferencedByRole_impl : virtual public CosGraphs_impl::Role_impl,
                              virtual public POA_CosReference::ReferencedByRole
{
public:
  ReferencedByRole_impl ();
  ReferencedByRole_impl (CosRelationships::RelatedObject_ptr); 

};


class Reference_impl : virtual public Relationship_impl,
                       virtual public POA_CosReference::Relationship
{
public:    
  Reference_impl ();
  Reference_impl (CosRelationships::NamedRoles nr);

};


#endif

