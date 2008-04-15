/*

Copyright (C) 1998, 2000 by the Object Management Group, Inc.  All  
rights reserved.  

Permission to use, copy, modify, and distribute this software for
any teaching or instructional purpose and without fee is hereby
granted, provided that the above copyright notice appear in all
copies and that both that copyright notice and this permission
notice appear in supporting documentation, and that the names of
Object Management Group, Inc. (OMG) and John Wiley and Sons, Inc.
(Wiley) not be used in advertising or publicity pertaining to
distribution of the software without specific, written prior
permission.

THIS SOFTWARE IS SOLD AS IS WITHOUT WARRANTY OF ANY KIND, EITHER
EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
WARRANTY OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE.  Neither OMG, Wiley, nor their dealers or distributors
assume any liability of any alleged or actual damages arising
from the use of or the inability to use this software.  (Some
states do not allow the exclusion of implied warranties, so the
exclusion may not apply to you).

*/

#include "DepotImpl.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif
#include <stdlib.h>
#include <stdio.h>


using namespace std;

const  int   NITEMTYPES=3;

const char *itemtypestext[NITEMTYPES] = {"FOOD","CLOTHES","OTHER"};

void DepotImpl::_abort(const char* msg) {
    cerr << msg << endl << flush;
    exit(1);
}

DepotImpl::DepotImpl(const char* data_file)
    : m_data_file(CORBA::string_dup(data_file))
{
    // Nothing else to do
}

void DepotImpl::loadDepot() {
    AStore::ItemInfo loaditem;
    loaditem.item = new char[30];
    loaditem.name = new char[40];
    char tempstring[30];
    int i;
	ifstream is(m_data_file, ios::in);
    if (!is)
    {
        char msg[200];
        sprintf(msg, "Could not open depot data file %s", (const char*) m_data_file);
        _abort(msg);
    }
    else 
    {
        cout << "Reading depot data file ... " << endl;
    }
  
    while (!is.eof())
    {
        is >> loaditem.item;
        is >> loaditem.name;
        is >> loaditem.quantity;
        is >> loaditem.item_cost;
        // Read in the string containing the itemtype and convert it to an
        // enumerated value
        is >> tempstring;
        i=0;
        while (i < NITEMTYPES && strcmp(tempstring,itemtypestext[i]) != 0)
            i++;
        if (i < NITEMTYPES)
        {
            loaditem.item_type=AStore::ItemTypes(i);
            m_items.insert(loaditem);
            // Echo the loaded item to standard output
            cout << "Read item: " 
                 << loaditem.item << " "
                 << loaditem.name << " "
                 << loaditem.quantity << " "
                 << loaditem.item_cost << " "
                 << itemtypestext[loaditem.item_type] << endl;
        }
    }
}

void DepotImpl::find_item_info(
    AStore::AStoreId store_id, 
    const char* item, 
    CORBA::Long quantity,
    AStore::ItemInfo_out item_info
)
{
    item_info = new AStore::ItemInfo;
    if ( m_items.locate(item, *item_info) == 0)
    {
#if 0
                // AP: Seems to be a bug
		CORBA::String n_item = CORBA::string_dup(item);
#else
                // MICO
		char* n_item = CORBA::string_dup(item);
#endif
        throw AStore::BarcodeNotFound(n_item);
    }
}
