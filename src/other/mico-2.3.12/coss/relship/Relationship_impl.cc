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
#include <coss/Relationship_impl.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif

//#define DEBUG 1
//#define USE_VAR 1

Relationship_impl::Relationship_impl ()
  : IdentifiableObject_impl ()
    , POA_CosRelationships::Relationship ()
{
#if DEBUG
  cout << "  <Relationship_impl> Relationship_impl ()\n" << flush;
#endif
  
  // n_roles = new CosRelationships::NamedRoles;
  //n_roles->length (0);
  n_roles.length (0);
  
#if DEBUG
  cout << "  <Relationship_impl> Relationship_impl () done.\n" << flush;
#endif
 
}


Relationship_impl::Relationship_impl (CosRelationships::NamedRoles nr)
  : IdentifiableObject_impl ()
    , POA_CosRelationships::Relationship ()
{
#if DEBUG
  cout << "  <Relationship_impl>" 
       << " Relationship_impl (CosRelationships::NamedRoles nr)\n" << flush;
#endif
  n_roles = nr;
#if DEBUG
  cout << "  <Relationship_impl> Relationship_impl (...) done\n" << flush;
#endif

}


Relationship_impl::~Relationship_impl ()
{
#ifdef DEBUG
  cout << "  ~Relationship_impl ()\n" << flush;
#endif
}


CosRelationships::NamedRoles* 
Relationship_impl::named_roles ()
{

#if DEBUG
  cout << "  <Relationship_impl> named_roles ()\n" << flush;
#endif

  CosRelationships::NamedRoles* tmp_nrs = new CosRelationships::NamedRoles;
  tmp_nrs->length (n_roles.length ());
  for (CORBA::ULong i=0; i<n_roles.length (); i++) {
    //  (*tmp_nrs)[i] = n_roles[i];
    (*tmp_nrs)[i].name = CORBA::string_dup (n_roles[i].name);
    (*tmp_nrs)[i].aRole = CosRelationships::Role::_duplicate 
      (n_roles[i].aRole);
  }

  return tmp_nrs;
}


void 
Relationship_impl::destroy ()
{
#if DEBUG
  cout << "  <Relationship_impl> destroy ()\n" << flush;
#endif
  CosRelationships::RelationshipHandle rh;
  rh.constant_random_id = random_id;
  rh.the_relationship = _this ();
  for (CORBA::ULong i=0; i<n_roles.length (); i++) {
#ifdef HAVE_EXCEPTIONS
    try {
#endif
      n_roles[i].aRole->unlink (rh);
#ifdef HAVE_EXCEPTIONS
    }
    catch (CosRelationships::Role::UnknownRelationship_catch &ex) {
#if DEBUG
      cout << "  <Relationship_impl> exception when unlink role!\n" << flush;
#endif
    }
#endif
  }

  //  _default_POA ()->deactivate_object (*(_default_POA ()->servant_to_id (this)));
#if USE_VAR
  PortableServer::ObjectId_var oid = _default_POA ()->servant_to_id (this);
#else
  PortableServer::ObjectId* oid = _default_POA ()->servant_to_id (this);
#endif
  _default_POA ()->deactivate_object (*oid);
#ifndef USE_VAR
  delete oid;
#endif
  delete this;

}






