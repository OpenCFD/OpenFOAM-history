/*
 *  Test of COSS Relationship Service for MICO
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


#include "Test_impl.h" 
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else
#include <fstream.h>
#endif
#include <string.h>
#include <time.h>
#include <unistd.h>


//#define DEBUG 1


BaseRole_impl::BaseRole_impl ()
  : CosGraphs_impl::Role_impl (),
    POA_Test::BaseRole ()
{
#if DEBUG
  cout << "  <BaseRole_impl> constructor\n";
#endif
}



BaseRole_impl::BaseRole_impl (CosRelationships::RelatedObject_ptr obj)
  : CosGraphs_impl::Role_impl (obj),
    POA_Test::BaseRole ()
{
#if DEBUG
  cout << "  <BaseRole_impl> constructor with params\n";
#endif
  rel_obj = obj;
}



DerivedRole1_impl::DerivedRole1_impl ()
  : BaseRole_impl (),
    POA_Test::DerivedRole1 ()
{
}

DerivedRole1_impl::DerivedRole1_impl (CosRelationships::RelatedObject_ptr obj)
  : BaseRole_impl (obj),
    POA_Test::DerivedRole1 ()
{
#if DEBUG
  cout << "  <DerivedRole1_impl> constructor with params\n";
#endif
  rel_obj = obj;
}


DerivedRole2_impl::DerivedRole2_impl ()
  : BaseRole_impl (),
    POA_Test::DerivedRole2 ()
{
}

DerivedRole2_impl::DerivedRole2_impl (CosRelationships::RelatedObject_ptr obj)
  : BaseRole_impl (obj),
    POA_Test::DerivedRole2 ()
{
#if DEBUG
  cout << "  <DerivedRole2_impl> constructor with params\n";
#endif
  rel_obj = obj;
}


TestRelationship_impl::TestRelationship_impl ()
  : Relationship_impl (),
    POA_Test::TestRelationship ()
{
}



TestRelationship_impl::TestRelationship_impl (CosRelationships::NamedRoles nr)
  : Relationship_impl (nr),
    POA_Test::TestRelationship ()
{
}





