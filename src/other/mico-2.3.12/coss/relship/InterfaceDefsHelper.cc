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


#include <coss/InterfaceDefsHelper.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif

#define DEBUG 0


using namespace std;

InterfaceDefsHelper::InterfaceDefsHelper (CORBA::ORB_var orb)
{
 
#if DEBUG
  cout << "  <InterfaceDefsHelper> constructor:\n";
#endif

#if DEBUG
  cout << "  <InterfaceDefsHelper> Binding to Interface Repository..." 
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

  interfaces = new CosRelationships::InterfaceDefs;
  interfaces->length (0);

  //last = 0;
#if DEBUG
  cout << "  <InterfaceDefsHelper> constructor done.\n";
#endif
}


InterfaceDefsHelper::~InterfaceDefsHelper ()
{
#if DEBUG
  cout << "  <InterfaceDefsHelper> descructor..." << flush;
#endif
  CORBA::release (repo);
  delete interfaces;
#if DEBUG
  cout << "done.\n";
#endif
}


CosRelationships::InterfaceDefs* 
InterfaceDefsHelper::get_interfaces ()
{
#if DEBUG
  cout << "  <InterfaceDefsHelper> get_interfaces ();\n";
#endif
  return interfaces;
}


CORBA::Long
InterfaceDefsHelper::length ()
{
#if DEBUG
  cout << "  <InterfaceDefsHelper> length: " << interfaces->length () << "\n";
#endif
  return interfaces->length ();
}


void
InterfaceDefsHelper::length (CORBA::Long x)
{
#if DEBUG
  cout << "  <InterfaceDefsHelper> length (" << x << ");\n";
#endif
  interfaces->length (x);
}


void
InterfaceDefsHelper::add (char* id)
{
#if DEBUG
  cout << "  <InterfaceDefsHelper> add (" << id << ");\n";
#endif
  CORBA::InterfaceDef_ptr type;
  if (!CORBA::is_nil (type = CORBA::InterfaceDef::_narrow 
		      (repo->lookup_id (id)))) {
    interfaces->length (interfaces->length () + 1);
    (*interfaces)[interfaces->length () - 1] = type; 
  }
  else 
    cout << "Error: type " << id << " is unknown.\n";
}


