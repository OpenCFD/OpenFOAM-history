/*
 *  MICO --- a free CORBA implementation
 *  Copyright (C) 1997 Kay Roemer & Arno Puder
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
 */

#include "browser.h"
#include "cg.h"

class IRBrowser_impl : public IRBrowser_skel {
    CORBA::Container_var current;
    CORBA::Repository_var ir;
    CORBA::ORB_var orb;
    string current_wd;

    ConceptualGraph *mkConNode (const string &name, const string &inst,
				ConceptualGraph *wd, ConceptualGraph *wd2);
    ConceptualGraph *mkConNode (const string &name,
				ConceptualGraph *wd, ConceptualGraph *wd2);
    ConceptualGraph *mkRelNode (const string &name,
				ConceptualGraph *wd, ConceptualGraph *wd2);
    ConceptualGraph *tcToCg (CORBA::TypeCode_ptr);
public:
    IRBrowser_impl();

    virtual char* getwd();
    virtual CORBA::Boolean back();
    virtual CORBA::Boolean cd( const char* el );
    virtual IRBrowser::NameList* list();
    virtual char* type( const char* el );
    virtual char* describe( const char* el );
    virtual IRBrowser::NameList* types();
    virtual char* optemplate (const char *el);
    virtual char* repoid ();
};
