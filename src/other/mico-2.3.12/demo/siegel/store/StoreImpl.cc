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
exclusion may not apply to you.

*/

#include "StoreImpl.h"
#include "StoreAccessImpl.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif
#include <stdio.h>
#include <stdlib.h>


using namespace std;

const int EMPTY = -1;


void StoreImpl::_abort(const char* msg) {
    cerr << msg << endl << flush;
    exit(1);
}


StoreImpl::StoreImpl(CosNaming::NamingContext_ptr ns, AStore::AStoreId storeID,
                     CORBA::Float storeMarkup)
    : m_POSTerminals(10)                // start off with space for 10 POSs
{
    m_ns = CosNaming::NamingContext::_duplicate(ns);
    m_storeTotal    = 0;
    m_storeTaxTotal = 0;
    m_storeMarkup   = storeMarkup;
    m_storeID       = storeID;

    // initialize m_POSTerminals to indicate no POS Logins have been received
    CORBA::ULong len      = m_POSTerminals.length();

    for (CORBA::ULong i = 0; i < len; i++) {
        m_POSTerminals[i].id = EMPTY;
    }
}

CORBA::Long StoreImpl::store_id(
) {
    return m_storeID;
}

AStore::Store::StoreTotals StoreImpl::totals(
) {
  
    AStore::Store::StoreTotals st;
  
    st.store_total = m_storeTotal;
  
    st.store_tax_total = m_storeTaxTotal;
  
    return st;
}

AStore::StoreAccess_ptr StoreImpl::login(CORBA::Long id
) {

    CORBA::ULong loc = _locatePOSEntry(id);

    m_POSTerminals[loc].id         = id;
    m_POSTerminals[loc].total_sales = 0;
    m_POSTerminals[loc].total_taxes = 0;

    // check to see of a StoreAccess object exists for this m_POSTerminal
    // allocate new one if needed.
    if (CORBA::is_nil(m_POSTerminals[loc].store_access_reference))
    {
        // create a local instance of the SToreAccess Object
        m_POSTerminals[loc].store_access_reference = 
#if 0
            new StoreAccessImpl(m_ns, this, m_storeMarkup);
#else
            // MICO
            (new StoreAccessImpl(m_ns, this->_this(), m_storeMarkup))->_this();
#endif
        if (CORBA::is_nil(m_POSTerminals[loc].store_access_reference))
            cerr << "Store_i::Login: Unable to create StoreAccess object for POS Login" << endl;
    }
        
    return AStore::StoreAccess::_duplicate(m_POSTerminals[loc].store_access_reference);
}

void StoreImpl::get_POS_totals(
    AStore::POSList_out POS_data
    
)
{
    POS_data = new AStore::POSList(m_POSTerminals);
}


void StoreImpl::update_store_totals(CORBA::Long  id,
                                    CORBA::Float price,
                                    CORBA::Float taxes
) {

    CORBA::ULong i = _locatePOSEntry(id);

    if (i != EMPTY) {
        m_POSTerminals[i].total_sales += price;
        m_POSTerminals[i].total_taxes += taxes;
        m_storeTotal += price;
        m_storeTaxTotal += taxes;
    }
    else
        cerr << "Store_i::UpdateStoreTotals: Could not locate POS Terminal " << id << endl;
}


CORBA::ULong StoreImpl::_locatePOSEntry(CORBA::Long id) {

    CORBA::ULong loc       = EMPTY;
    CORBA::ULong availloc  = EMPTY;
    CORBA::ULong len       = m_POSTerminals.length();
    int keepgoing          = 1;
    CORBA::ULong i         = 0;

    // locate POSId or first available slot
    while (loc == EMPTY && i < len) {
        if (m_POSTerminals[i].id == id)
            loc = i;
        else if (availloc == EMPTY && m_POSTerminals[i].id == EMPTY)
            availloc = i;
        else
            i++;
    }

    // if we did not find POSId then use the available slot or append new
    // slot at end of m_POSTerminals
    if (loc == EMPTY) {
        if (availloc != EMPTY)
            loc = availloc;
        else {
            m_POSTerminals.length(i+1);
            loc = i;
        }
    }

    return loc;
}

