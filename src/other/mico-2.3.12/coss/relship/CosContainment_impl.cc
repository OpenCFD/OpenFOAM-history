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

#include <coss/CosContainment_impl.h> 
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else
#include <fstream.h>
#endif
#include <string.h>
#include <time.h>
#include <unistd.h>


//#define DEBUG 1


ContainsRole_impl::ContainsRole_impl ()
  : CosGraphs_impl::Role_impl (),
    POA_CosContainment::ContainsRole ()
{
}


ContainsRole_impl::ContainsRole_impl (CosRelationships::RelatedObject_ptr obj)
  : CosGraphs_impl::Role_impl (),
    POA_CosContainment::ContainsRole ()
{
#if DEBUG
  cout << "  <ContainsRole_impl> constructor with params\n";
#endif
  rel_obj = obj;
}


ContainedInRole_impl::ContainedInRole_impl ()
  : CosGraphs_impl::Role_impl (),
    POA_CosContainment::ContainedInRole ()
{
}


ContainedInRole_impl::ContainedInRole_impl 
(CosRelationships::RelatedObject_ptr obj)
  : CosGraphs_impl::Role_impl (),
    POA_CosContainment::ContainedInRole ()
{
#if DEBUG
  cout << "  <ContainedInRole_impl> constructor with params\n";
#endif
  rel_obj = obj;
}


Containment_impl::Containment_impl ()
  : Relationship_impl ()
   , POA_CosContainment::Relationship ()
{
}


Containment_impl::Containment_impl (CosRelationships::NamedRoles nr)
  : Relationship_impl (nr)
   , POA_CosContainment::Relationship ()
{
}





