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

#include <coss/CosReference_impl.h> 
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else
#include <fstream.h>
#endif
#include <string.h>
#include <time.h>
#include <unistd.h>

//#include "CosRelationships.h"
//#include <mico/CosReference.h>
//#include <mico/GraphsRole_impl.h>
//#include <mico/Relationship_impl.h>


//#define DEBUG 1


ReferencesRole_impl::ReferencesRole_impl ()
  : CosGraphs_impl::Role_impl (),
    POA_CosReference::ReferencesRole ()
{
}



ReferencesRole_impl::ReferencesRole_impl 
(CosRelationships::RelatedObject_ptr obj)
  : CosGraphs_impl::Role_impl (),
    POA_CosReference::ReferencesRole ()
{
#if DEBUG
  cout << "  <ReferencesRole_impl> constructor with params\n";
#endif
  rel_obj = obj;
}



ReferencedByRole_impl::ReferencedByRole_impl ()
  : CosGraphs_impl::Role_impl (),
    POA_CosReference::ReferencedByRole ()
{
}



ReferencedByRole_impl::ReferencedByRole_impl 
(CosRelationships::RelatedObject_ptr obj)
  : CosGraphs_impl::Role_impl (),
    POA_CosReference::ReferencedByRole ()
{
#if DEBUG
  cout << "  <ReferencedByRole_impl> constructor with params\n";
#endif
  rel_obj = obj;
}


Reference_impl::Reference_impl ()
  : Relationship_impl ()
   , POA_CosReference::Relationship ()
{
}



Reference_impl::Reference_impl (CosRelationships::NamedRoles nr)
  : Relationship_impl (nr)
   , POA_CosReference::Relationship ()
{
}



