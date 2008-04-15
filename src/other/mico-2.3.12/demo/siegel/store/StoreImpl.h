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


#ifndef STOREIMPL_H
#define STOREIMPL_H

// Include Naming Service
#if 1
// MICO
#include <coss/CosNaming.h>
#endif


// Include Store and Central type information via the skeleton.
// The file to include depends on the output of
// your ORBs IDL compiler.
#if 1
// MICO
#include <Central.h>
#include <Store.h>
#endif



// The super class of your implementation class 
// depends on the output of your ORBs IDL compiler.
#if 0
class StoreImpl
#else
// MICO
class StoreImpl : public virtual POA_AStore::Store
#endif
{
private:
    CORBA::ULong _locatePOSEntry(CORBA::Long);
    AStore::AStoreId m_storeID;
    CORBA::Float m_storeTotal;
    CORBA::Float m_storeTaxTotal;
    CORBA::Float m_storeMarkup;
    AStore::POSList m_POSTerminals;
    CosNaming::NamingContext_var m_ns;  

    static void _abort(const char *msg);
  
public:
    StoreImpl(
        CosNaming::NamingContext_ptr ns,
        AStore::AStoreId storeID,
        CORBA::Float storeMarkup
    );

    AStore::AStoreId store_id(
        
    );

    AStore::Store::StoreTotals totals(
        
    );

    AStore::StoreAccess_ptr login(
        CORBA::Long id
    
    );
    
    void get_POS_totals(
        AStore::POSList_out POS_data
    
    );
    
    void update_store_totals(
        CORBA::Long  id,
        CORBA::Float price,
        CORBA::Float taxes
    
    );

};


#endif
