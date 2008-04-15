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

#ifndef __Role_impl_h__
#define __Role_impl_h__

class Role_impl : virtual public POA_CosRelationships::Role 
{
  CORBA::Long cardinality;
  CORBA::Long max_cardinality;
  CORBA::Long min_cardinality;
  //CORBA::InterfaceDef_var rel_ship_type;
  char* rel_ship_repoid;
  char* iterator_server; 
  CORBA::ORB_ptr _orb;
  PortableServer::POA_var _poa;
  /* it's RelshipsIteratorServer name */
protected:
  CosRelationships::RelationshipHandles rel_ships;
  // due to problem with calling constructors of parents in namespaces
  CosRelationships::RelatedObject_ptr rel_obj;

public:
  Role_impl ();
  Role_impl (CosRelationships::RelatedObject_ptr);

  ~Role_impl ();

  virtual CosRelationships::RelatedObject_ptr related_object ();
  virtual CosRelationships::RelatedObject_ptr 
    get_other_related_object (const CosRelationships::RelationshipHandle& rel,
			      const char* target_name);
  virtual CosRelationships::Role_ptr 
    get_other_role (const CosRelationships::RelationshipHandle& rel,
		    const char* target_name);
  virtual void get_relationships (CORBA::ULong how_many,
			 CosRelationships::RelationshipHandles_out rels,
			 CosRelationships::RelationshipIterator_out iterator);
  virtual void destroy_relationships ();
  virtual void destroy ();
  virtual CORBA::Boolean check_minimum_cardinality ();
  virtual void link (const CosRelationships::RelationshipHandle& rel, 
		     const CosRelationships::NamedRoles& named_roles);
  virtual void unlink (const CosRelationships::RelationshipHandle& rel);

  // extension
  
  virtual void set_min_cardinality (CORBA::Long); 
  virtual void set_max_cardinality (CORBA::Long); 
  virtual void set_cardinality (CORBA::Long); 
  virtual void set_relship_repoid (char*);
  virtual void set_orb (CORBA::ORB_ptr);
  virtual void set_poa (PortableServer::POA_var);
};


#endif

