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

#include "StoreAccessImpl.h"
#include "StoreImpl.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else
#include <fstream.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <NsPublisher.h>


using namespace std;

void StoreAccessImpl::_abort(const char* msg) {
    cerr << msg << endl << flush;
    exit(1);
}


StoreAccessImpl::StoreAccessImpl(CosNaming::NamingContext_ptr ns,
                                 AStore::Store_ptr store,
                                 CORBA::Float markup)
{
    m_storeMarkup = markup;
        
    try
    {
        char refstr[256];
        AStore::AStoreId id = store->store_id();
        sprintf(refstr,"Tax_%ld",id);

        CORBA::Object_var obj;
                
        // Resolve Tax
        try {
            obj = NsPublisher::nsResolve(ns, refstr, "Primer_Example");
        }
        catch(const NsPublisherException& nse) {
            _abort(nse.toString());
        }

        m_tax = AStore::Tax::_narrow(obj);      


        // Resolve Depot
        try {
            obj = NsPublisher::nsResolve(ns, "Depot", "Primer_Example");
        }
        catch(const NsPublisherException& nse) {
            _abort(nse.toString());
        }

        m_depot = CentralOffice::Depot::_narrow(obj);

    
        // Make sure all the _narrow calls succeeded
        if (CORBA::is_nil(m_tax) || CORBA::is_nil(m_depot))
        {
            // We just exit for the demo, in real life it would be better
            // to raise an exception.
            _abort("ERROR: StoreAccess failed to narrow objects from Naming Service");
        }
            
        m_store = AStore::Store::_duplicate(store);
        
    }
    catch(const CORBA::Exception& ) 
    {
        _abort("CORBA::Exception caught. Trouble finding tax, store, or depot.");
    }

}


void StoreAccessImpl::find_price(
    const char* item, 
    CORBA::Long quantity, 
    CORBA::Float_out item_price, 
    CORBA::Float_out item_tax_price, 
    AStore::ItemInfo_out item_info
) 
{

    m_depot->find_item_info(m_store->store_id(), item, quantity, item_info);

    item_price    = m_storeMarkup * item_info->item_cost;

    item_tax_price = m_tax->find_taxable_price(item_price, item_info->item_type);

}
