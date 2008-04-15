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


#include <CORBA-SMALL.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif

#include "RandomGenerator.h"
#include "random.h"


using namespace std;

void usage (char* args[])
{
  cout << "usage: " << args[0] << " -f <file> | --file <file>\n\n";
  cout << "\twhere <file> is file which contains reference\n"; 
  cout << "\tfor random service on www.random.org\n";
}


int main (int argc, char* argv[]) {

  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

  if (argc != 3) {
    usage (argv);
    exit (1);
  }
  ifstream in;
  if (strcmp (argv[1], "--file") == 0 || strcmp (argv[1], "-f") == 0) {
    in.open (argv[2]);
    if (!in.good ()) {
      cerr << "can't find " << argv[2] << " file.\n";
      exit (1);
    }
  }
  else {
    usage (argv);
    exit (1);
  }

  CORBA::Object_var obj = orb->bind ("IDL:RandomGenerator:1.0");
  RandomGenerator_var generator = RandomGenerator::_narrow (obj);
  if (CORBA::is_nil (generator)) {
    cerr << "can't bind to randomd.\n";
    exit (1);
  }

  char ref[1024];
  in >> ref;
  
  CORBA::Object_var obj2 = orb->string_to_object (ref);
  Random_ptr random = Random::_narrow (obj2);
  if (CORBA::is_nil (random)) {
    cerr << "can't bind to random service on www.random.org\n";
    exit (1);
  }

  for (;;) {
    CORBA::Long number = random->lrand48 ();
    generator->add_number (number);
  }

  return 0;
}
