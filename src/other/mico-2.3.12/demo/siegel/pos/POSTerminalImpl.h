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

#ifndef POSTERMINALIMPL_H
#define POSTERMINALIMPL_H


// Include Naming Service.
// Each ORB implementation will provide a different header file 
// to include.

#if 1
// MICO
#include <coss/CosNaming.h>
#endif


// Include Store type information via the skeleton.
// The file to include depends on the output of
// your ORBs IDL compiler.

#if 1
// MICO
#include <POS.h>
#include <Store.h>
#endif


// The super class of your implementation class 
// depends on the output of your ORBs IDL compiler.
#if 0
class POSTerminalImpl
#else
// MICO
class POSTerminalImpl : public virtual POA_POS::POSTerminal
#endif
{
private:
    POS::Barcode_var m_itemBarcode;
    CORBA::Long m_itemQuantity;
    AStore::Tax_var m_taxRef;
    AStore::Store_var m_storeRef;
    AStore::StoreAccess_var m_storeAccessRef;
    POS::OutputMedia_var m_outputMediaRef;
    CORBA::Float m_itemExtension;
    CORBA::Float m_saleSubTotal;
    CORBA::Float m_saleTaxableSubTotal;
    CORBA::Float m_POSTotal;
    CORBA::Float m_POSTaxTotal;
    CORBA::Long m_id;

    unsigned char _loggedIn();
    static void _abort(const char *msg);
       
public:
    POSTerminalImpl(
                    CosNaming::NamingContext_ptr naming_service,
                    CORBA::Long storeid,
                    POS::POSId id);
        
    void login (
    );
        
    void print_POS_sales_summary (
    );
        
    void print_store_sales_summary (
    );
        
    void send_barcode (const char * item
    );

    void item_quantity (CORBA::Long quantity
    );
                
    void end_of_sale (
    );

};

#endif
