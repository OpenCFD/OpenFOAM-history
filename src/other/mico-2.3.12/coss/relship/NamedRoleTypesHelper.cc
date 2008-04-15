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


#include <coss/NamedRoleTypesHelper.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif

//#define DEBUG 1


using namespace std;

NamedRoleTypesHelper::NamedRoleTypesHelper (CORBA::ORB_var orb)
{
 
#if DEBUG
  cout << "  <NamedRoleTypesHelper> constructor:\n";
#endif

#if DEBUG
  cout << "  <NamedRoleTypesHelper> Binding to Interface Repository..." 
       << flush;
#endif
  CORBA::Object_var ir_obj
      = orb->resolve_initial_references ("InterfaceRepository");
  repo = CORBA::Repository::_narrow (ir_obj);
#if DEBUG
  if (CORBA::is_nil (repo))
    cout << "ERROR!\n";
  else
    cout << "OK!\n";
#endif

  nr_types = new CosRelationships::RelationshipFactory::NamedRoleTypes;
  nr_types->length (0);

  //last = 0;
#if DEBUG
  cout << "  <NamedRoleTypesHelper> constructor done.\n";
#endif
}


NamedRoleTypesHelper::~NamedRoleTypesHelper ()
{
#if DEBUG
  cout << "  <NamedRoleTypesHelper> descructor..." << flush;
#endif
  CORBA::release (repo);
  delete nr_types;
#if DEBUG
  cout << "done.\n";
#endif
}


CosRelationships::RelationshipFactory::NamedRoleTypes*
NamedRoleTypesHelper::get_named_role_types ()
{
#if DEBUG
  cout << "  <NamedRoleTypesHelper> get_interfaces ();\n";
#endif
  return nr_types;
}


CORBA::Long
NamedRoleTypesHelper::length ()
{
#if DEBUG
  cout << "  <NamedRoleTypesHelper> length: " << nr_types->length () << "\n";
#endif
  return nr_types->length ();
}


void
NamedRoleTypesHelper::length (CORBA::Long x)
{
#if DEBUG
  cout << "  <NamedRoleTypesHelper> length (" << x << ");\n";
#endif
  nr_types->length (x);
}


void
NamedRoleTypesHelper::add (const char* id, const char* name)
{
#if DEBUG
  cout << "  <NamedRoleTypesHelper> add (" << id << ", " << name << ");\n";
#endif
  if (!CORBA::is_nil (repo->lookup_id (CORBA::string_dup (id)))) {
    nr_types->length (nr_types->length () + 1);
    (*nr_types)[nr_types->length () - 1].named_role_type = 
      CORBA::InterfaceDef::_narrow (repo->lookup_id (CORBA::string_dup (id)));
    (*nr_types)[nr_types->length () - 1].name = CORBA::string_dup (name);
  } 
  else {
    cout << "Error: type " << id << " is unknown.\n";
  }
}
