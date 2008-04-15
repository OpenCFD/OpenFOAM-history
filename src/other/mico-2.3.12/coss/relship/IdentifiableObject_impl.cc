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


#include <unistd.h>
#include <coss/IdentifiableObject_impl.h>


using namespace std;

// static variables
CORBA::ORB_ptr IdentifiableObject_impl::orb;
RandomGenerator_ptr IdentifiableObject_impl::generator;


IdentifiableObject_impl::IdentifiableObject_impl ()
  : POA_CosObjectIdentity::IdentifiableObject ()
{
  if (CORBA::is_nil (generator)) {
    cerr << "server not binded to randomd.\n";
    exit (1);
  }
  bool setuped = false;
  while (!setuped) {
#ifdef HAVE_EXCEPTIONS
    try {
#endif
      random_id = generator->rand ();
#ifdef HAVE_EXCEPTIONS
    } catch (RandomGenerator::NoSuchNumber_catch& ex) {
#ifndef _WIN32
      sleep (1);
#endif
      break;
    }
#endif
    setuped = true;
  }
}

IdentifiableObject_impl::IdentifiableObject_impl (CORBA::ORB_ptr _orb)
  : POA_CosObjectIdentity::IdentifiableObject ()
{
  orb = CORBA::ORB::_duplicate (_orb);
  CORBA::Object_var obj = orb->bind ("IDL:RandomGenerator:1.0");
  generator = RandomGenerator::_narrow (obj);
  if (CORBA::is_nil (generator)) {
    cerr << "can't bind to randomd.\n";
    exit (1);
  }
  bool setuped = false;
  while (!setuped) {
#ifdef HAVE_EXCEPTIONS
    try {
#endif
      random_id = generator->rand ();
#ifdef HAVE_EXCEPTIONS
    } catch (RandomGenerator::NoSuchNumber_catch& ex) {
#ifndef _WIN32
      sleep (1);
#endif
      break;
    }
#endif
    setuped = true;
  }
}

CosObjectIdentity::ObjectIdentifier 
IdentifiableObject_impl::constant_random_id()
{
  return random_id;
}

CORBA::Boolean 
IdentifiableObject_impl::is_identical(CosObjectIdentity::IdentifiableObject_ptr other_object)
{
  if (other_object->constant_random_id() == random_id)
    return TRUE;
  else
    return FALSE;
}






