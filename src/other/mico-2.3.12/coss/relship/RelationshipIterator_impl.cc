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
#include <coss/RelationshipIterator_impl.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif

//#define DEBUG 1


RelationshipIterator_impl::RelationshipIterator_impl
(CosRelationships::RelationshipHandles handles)
  : POA_CosRelationships::RelationshipIterator ()
{
  rel_handles = handles;
  index = 0;
}

RelationshipIterator_impl::~RelationshipIterator_impl ()
{
#if DEBUG
  cout << "  ~RelationshipIterator_impl ()\n" << flush;
#endif
}

CORBA::Boolean
RelationshipIterator_impl::next_one 
(CosRelationships::RelationshipHandle_out rel)
{
#if DEBUG
  cout << "  <RelationshipIterator_impl> next_one (...);\n";
#endif
  if ( (rel_handles.length () - 1) < (CORBA::ULong)index) {
    return FALSE;
  }
  
  CosRelationships::RelationshipHandle*
    tmp = new CosRelationships::RelationshipHandle;
  tmp->constant_random_id = rel_handles[index].constant_random_id;
  tmp->the_relationship = CosRelationships::Relationship::_duplicate 
    (rel_handles[index].the_relationship);
  rel = tmp;
  index++;
  return TRUE;
}

CORBA::Boolean
RelationshipIterator_impl::next_n (CORBA::ULong how_many,
				   CosRelationships::RelationshipHandles_out rel)
{
#if DEBUG
  cout << "  <RelationshipIterator_impl> next_n (...);\n";
#endif

  if ( (rel_handles.length () - 1) < (CORBA::ULong)index)
    return FALSE;

  long my_how_many;

  if ((rel_handles.length () - 1) < index + how_many - 1)
    my_how_many = rel_handles.length () - index;
  else
    my_how_many = how_many;

  CosRelationships::RelationshipHandles* tmp;
  tmp = new CosRelationships::RelationshipHandles;

  tmp->length (my_how_many);
#if DEBUG
  cout << "  <RelationshipIterator_impl> saving" << flush;
#endif
  for (int i=0; i<my_how_many; i++) {
#if DEBUG
    cout << "." << flush;
#endif
    (*tmp)[i].constant_random_id = rel_handles[i + index].constant_random_id;
    (*tmp)[i].the_relationship = CosRelationships::Relationship::_duplicate
      (rel_handles[i + index].the_relationship);
  }
#if DEBUG
  cout << "done.\n";
  cout << "  <RelationshipIterator_impl> length: " << tmp->length () << "\n";
#endif
  rel = tmp;
  index += my_how_many;
  return TRUE;
}


void
RelationshipIterator_impl::destroy ()
{
  //  _default_POA ()->deactivate_object (*(_default_POA ()->servant_to_id (this)));

  PortableServer::ObjectId_var oid = _default_POA ()->servant_to_id (this);
  _default_POA ()->deactivate_object (*oid);

  delete this;

}

