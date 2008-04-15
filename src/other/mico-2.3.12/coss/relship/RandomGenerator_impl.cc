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

#include <CORBA.h>
#include <coss/RandomGenerator_impl.h>
#include <stdlib.h>
#include <time.h>


// Implementation for interface RandomGenerator

RandomGenerator_impl::RandomGenerator_impl (RandomGenerator::Mode _mode) 
  : POA_RandomGenerator ()
{
  srand (time (0));
  mode = _mode;
  size = 1000;
}

CORBA::Long
RandomGenerator_impl::rand ()
{
  CORBA::Long number;
  if (mode == RandomGenerator::generate)
    number = ::rand ();
  if (mode == RandomGenerator::proxy) {
    if (!random_numbers.empty ()) {
      number = *(random_numbers.begin ());
      random_numbers.pop_front ();
    }
    else {
      //::RandomGenerator::NoSuchNumber exception;
      //throw exception;
      mico_throw (RandomGenerator::NoSuchNumber());
    }
  }

  return number; 
}


void
RandomGenerator_impl::add_number (CORBA::Long number)
{
  if (random_numbers.size () < (unsigned long)size)
    random_numbers.push_back (number);
}


void
RandomGenerator_impl::cache_size (long _size)
{
  size = _size;
}
