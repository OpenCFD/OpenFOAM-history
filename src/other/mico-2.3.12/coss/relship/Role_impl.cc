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


#include <coss/CosRelationships.h>
#include <coss/Role_impl.h>
#include <coss/RelationshipIterator_impl.h>
#include <string.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif
#include <unistd.h>


//#define DEBUG 1
//#define DEBUG2 1

Role_impl::Role_impl ()
  : POA_CosRelationships::Role ()
{
#if DEBUG2
  cout << "  <Role_impl> constructor\n";
#endif
#if DEBUG
  cout << "<Role_impl> Role_impl()\n" << flush;
#endif
  iterator_server = CORBA::string_dup ("");
  rel_ship_repoid = CORBA::string_dup ("");
  rel_ships.length (0);
  rel_obj = CosRelationships::RelatedObject::_nil ();
  cardinality = 0;
}

Role_impl::Role_impl (CosRelationships::RelatedObject_ptr obj)
  : POA_CosRelationships::Role ()
{
#if DEBUG2
  cout << "  <Role_impl> constructor with params\n";
#endif
  rel_obj = obj;
  iterator_server = CORBA::string_dup ("");
  rel_ship_repoid = CORBA::string_dup ("");
  cardinality = 0;
  rel_ships.length (0);
#if DEBUG
  if (CORBA::is_nil (rel_obj))
    cout << "  <Role_impl> rel_obj is NIL!\n" << flush;
  else 
    cout << "  <Role_impl> rel_obj is DEFINED!\n" << flush;
  cout << "  <Role_impl> rel_obj = " << rel_obj << "\n" << flush;
  cout << "  <Role_impl> constructor done.\n" << flush;
#endif
}

Role_impl::~Role_impl () 
{
#if DEBUG
  cout << "  ~Role_impl ()\n" << flush;
#endif
  delete rel_ship_repoid;
  delete iterator_server;
  //delete rel_ships;
}


CosRelationships::RelatedObject_ptr 
Role_impl::related_object ()
{
#if DEBUG
  cout << "  <Role_impl> related_object();\n" << flush;
  if (CORBA::is_nil (rel_obj))
      cout << "  <Role_impl> rel_obj is NIL!\n" << flush;
#endif
  return CosRelationships::RelatedObject::_duplicate (rel_obj);
}


CosRelationships::RelatedObject_ptr
Role_impl::get_other_related_object (
			    const CosRelationships::RelationshipHandle& rel,
			    const char *target_name)
{
  CosRelationships::Role_var tmp = get_other_role (rel, target_name);
  return tmp->related_object ();
}


CosRelationships::Role_ptr
Role_impl::get_other_role (const CosRelationships::RelationshipHandle& rel,
			   const char *target_name)
{
  // CosRelationships::Relationship tmp = rel.the_relationship

#if DEBUG
  cout << "  <Role_impl> get_other_role ();\n" << flush;
  cout << "  <Role_impl> target_name is " << target_name << "\n" << flush;
  cout << "  <Role_impl> random id: " << rel.constant_random_id << "\n" << flush;
#endif

  CORBA::Boolean is_in = FALSE;
  for (CORBA::ULong i = 0; i < rel_ships.length (); i++) {
    if (rel.constant_random_id == rel_ships[i].constant_random_id) {
      is_in = TRUE;
      CosRelationships::NamedRoles_var 
	nr = rel.the_relationship->named_roles();
      CORBA::ULong x = nr->length();
      for(CORBA::ULong j=0; j<x; j++) {
	if (strcmp (target_name, (*nr)[j].name) == 0) {
	  return CosRelationships::Role::_duplicate ((*nr)[j].aRole);
	}
      }
    }
  }
  if (!is_in) {
    CosRelationships::Role::UnknownRelationship ex;
    mico_throw (ex);
  }
  CosRelationships::Role::UnknownRoleName ex;
  mico_throw (ex);

  return CosRelationships::Role::_nil ();
}


void
Role_impl::get_relationships 
(CORBA::ULong how_many,
 CosRelationships::RelationshipHandles_out rels,
 CosRelationships::RelationshipIterator_out iter)
{
  
#if DEBUG
  cout << "  <Role_impl> get_relationships (...)\n" << flush;
  cout << "  <Role_impl> rel_ships.length (): " << rel_ships.length () << "\n" << flush;
  cout << "  <Role_impl> first of all relationship_handles:\n" << flush;
#endif

  CosRelationships::RelationshipHandles* 
    tmp_handles = new CosRelationships::RelationshipHandles;
  int length;
  if (rel_ships.length () < how_many)
    length = rel_ships.length ();
  else
    length = how_many;
  tmp_handles->length (length);
  for (int i=0; i<length; i++) {
#if DEBUG
    cout << "  <Role_impl> rand id: " << rel_ships[i].constant_random_id 
	 << "\n" << flush; 
#endif
    (*tmp_handles)[i].constant_random_id = rel_ships[i].constant_random_id;
    (*tmp_handles)[i].the_relationship 
      = CosRelationships::Relationship::_duplicate 
      (rel_ships[i].the_relationship);
  }
  rels = tmp_handles;

#if DEBUG
  cout << "  <Role_impl> Create handles for iterator:\n" << flush;
#endif

  if (how_many < rel_ships.length ()) {
#if DEBUG
    cout << "  <Role_impl> rel_ships.length (): " << rel_ships.length () 
	 << "\n" << flush;
    cout << "  <Role_impl> how_many: " << how_many << "\n" << flush;
#endif

    CosRelationships::RelationshipHandles tmp_relships;
    RelationshipIterator_impl* tmp_iter;
    int index = how_many;
    int max = rel_ships.length () - how_many;

#if DEBUG
    cout << "  <Role_impl> max: " << max << "\n" << flush;
    cout << "  <Role_impl> index: " << index << "\n" << flush;
#endif
 
    tmp_relships.length (max);
    for (CORBA::ULong i=0; i<tmp_relships.length (); i++) {
      tmp_relships[i].constant_random_id 
	= rel_ships[i + index].constant_random_id;
      tmp_relships[i].the_relationship 
	= CosRelationships::Relationship::_duplicate
	(rel_ships[i + index].the_relationship);
    }

#if DEBUG
    cout << "  <Role_impl> create iterator\n" << flush;
#endif

    tmp_iter = new RelationshipIterator_impl (tmp_relships);
    /*
    CosRelationships::RelationshipIterator_ptr tmp_iterator 
      = tmp_iter->_this ();
    iterator = CosRelationships::RelationshipIterator::_duplicate 
      (tmp_iterator);
      */
    iter = tmp_iter->_this ();
    assert (!CORBA::is_nil (iter.ptr()));

#if DEBUG
    cout << "  <Role_impl> iter->_repoid (): " << iter->_repoid () 
	 << "\n" << flush;
#endif
 
  }
  else {
#if DEBUG
    cout << "  <Role_impl> create iterator _nil ()\n" << flush;
#endif
    iter = CosRelationships::RelationshipIterator::_nil ();
  }

#if DEBUG
  cout << "  <Role_impl> get_relationships DONE!\n" << flush;
#endif
 
}


void 
Role_impl::destroy_relationships ()
{
  CORBA::Boolean error = FALSE;
  CosRelationships::Role::CannotDestroyRelationship ex;
  ex.offenders.length (0);
  int index = 0;

  for (CORBA::ULong i=0; i<rel_ships.length (); i++) {
#ifdef HAVE_EXCEPTIONS
    try {
#endif
      rel_ships[i].the_relationship->destroy ();
#ifdef HAVE_EXCEPTIONS
    } catch (CosRelationships::Relationship::CannotUnlink_catch &exception) {
      error = TRUE;
      ex.offenders.length (ex.offenders.length () + 1);
      index = ex.offenders.length () - 1;
      ex.offenders[index].constant_random_id 
	  = rel_ships[i].constant_random_id;
      ex.offenders[index].the_relationship 
	  = CosRelationships::Relationship::_duplicate 
	  (rel_ships[i].the_relationship);
      }
#endif
  }
  if (error) 
    mico_throw (ex);
    
}


void
Role_impl::destroy ()
{
  if (rel_ships.length () != 0) {
    CosRelationships::Role::ParticipatingInRelationship ex;
    ex.the_relationships = rel_ships;
    mico_throw (ex);
  }

  PortableServer::ObjectId_var oid = _default_POA ()->servant_to_id (this);
  _default_POA ()->deactivate_object (*oid);

  delete this;

}


CORBA::Boolean 
Role_impl::check_minimum_cardinality ()
{
  if (cardinality > min_cardinality)
    return TRUE;
  else
    return FALSE;
}


void 
Role_impl::link (const CosRelationships::RelationshipHandle& rel,
		 const CosRelationships::NamedRoles& named_roles)
{
#if DEBUG
  cout << "  <Role_impl> link (...)\n" << flush;
  cout << "  <Role_impl> cardinality = " << cardinality << "\n" << flush;
#endif

  //PortableServer::POA_var poa = _default_POA ();
  //CORBA::ORB_var o = orb;

  if (cardinality == max_cardinality) {
#if DEBUG
    cout << "  <Role_impl> MaxCardinalityExceeded\n" << flush;
#endif
    CosRelationships::RelationshipFactory::MaxCardinalityExceeded ex;
    ex.culprits.length (1);
#if DEBUG
    cout << "  <Role_impl> ex.culprits.length (1);\n" << flush;
#endif
    CosRelationships::Role_ptr tmp_this = _this ();
    ex.culprits[0].aRole = tmp_this;
    for (CORBA::ULong i=0; i<named_roles.length (); i++) {
#if DEBUG
      cout << "  <Role_impl> " << i << "\n" << flush;
#endif
      if (tmp_this->_is_equivalent (named_roles[i].aRole.in())) {
#if DEBUG
	cout << "  <Role_impl> is equivalent!, name is " 
	     << named_roles[i].name << "\n" << flush;
#endif
	ex.culprits[0].name = CORBA::string_dup (named_roles[i].name);
#if DEBUG
      cout << "  <Role_impl> ex.culprits[0].name is " << ex.culprits[0].name 
	   << "\n" << flush;
#endif
      
      }
    }
#if DEBUG
    cout << "  <Role_impl> throw maxcard except :))\n" << flush;
#endif
    mico_throw (ex);
  }
#if DEBUG
  cout << "  <Role_impl> cardinality is OK!\n" << flush;
#endif

  if (strcmp (rel_ship_repoid, "") != 0) {
    if (!rel.the_relationship->_get_interface ()->is_a (rel_ship_repoid)) {
      CosRelationships::Role::RelationshipTypeError ex;
      mico_throw (ex);
    }
  }
 
  rel_ships.length (rel_ships.length () + 1);
  rel_ships[rel_ships.length () - 1].constant_random_id 
    = rel.constant_random_id;
  rel_ships[rel_ships.length () - 1].the_relationship 
    = CosRelationships::Relationship::_duplicate (rel.the_relationship.in());
 
  cardinality++;
#if DEBUG
  cout << "  <Role_impl> last relship random_id: " 
       << rel_ships[rel_ships.length () - 1].constant_random_id << "\n" << flush;
  cout << "  <Role_impl> link (...) done!\n" << flush;
#endif

}


void 
Role_impl::unlink(const CosRelationships::RelationshipHandle& rel)
{
  CosRelationships::RelationshipHandle tmp_rel_handle;
  tmp_rel_handle.constant_random_id = 0;
  tmp_rel_handle.the_relationship = CosRelationships::Relationship::_nil ();
  int pos = 0;
  CORBA::Boolean is_in = FALSE;
#if DEBUG
  cout << "  <Role_impl> unlink (...)\n" << flush;
  cout << "  <Role_impl> random_id: " << rel.constant_random_id << "\n" << flush;
  cout << "  <Role_impl> rel_ships length: " << rel_ships.length () << "\n" << flush;
#endif
  for (CORBA::ULong i=0; i<rel_ships.length (); i++) {
#if DEBUG
    cout << "  <Role_impl> " << i << ". r_handle random id: " 
	 << rel_ships[i].constant_random_id << "\n" << flush;
#endif
    if (rel.constant_random_id == rel_ships[i].constant_random_id) {
      pos = i;
      is_in = TRUE;
#if DEBUG
      cout << "  <Role_impl> position is " << pos << "\n" << flush;
#endif
      break;
    }
  }
  if (is_in) {
    for (CORBA::ULong i=pos; i<rel_ships.length () - 1; i++) {
      rel_ships[i] = rel_ships[i + 1];
    }
    rel_ships.length (rel_ships.length () - 1);
  }
  else {
    CosRelationships::Role::UnknownRelationship ex;
    mico_throw (ex);
  }
  
  cardinality--;
}

// extension


void 
Role_impl::set_min_cardinality (CORBA::Long x)
{
  min_cardinality = x;
}


void 
Role_impl::set_max_cardinality (CORBA::Long x)
{
  max_cardinality = x;
}


void 
Role_impl::set_cardinality (CORBA::Long x)
{
  cardinality = x;
}


void 
Role_impl::set_relship_repoid (char* s)
{
  rel_ship_repoid = CORBA::string_dup (s);
}

void
Role_impl::set_orb (CORBA::ORB_ptr x)
{
  _orb = x;
}

void
Role_impl::set_poa (PortableServer::POA_var x)
{
  _poa = x;
}




