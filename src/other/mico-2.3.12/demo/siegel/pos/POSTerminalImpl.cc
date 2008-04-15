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

#include "POSTerminalImpl.h"
#include <NsPublisher.h>
#include <stdio.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif
#include <stdlib.h>


using namespace std;

void POSTerminalImpl::_abort(const char* msg) {
    cerr << msg << endl << flush;
    exit(1);
}


POSTerminalImpl::POSTerminalImpl(
								 CosNaming::NamingContext_ptr ns,
                                 CORBA::Long storeid,
                                 POS::POSId id)
{
    // Load in the stringified object references from the files and
    // construct references to these objects
    char caStr[512];
        
    sprintf(caStr, "Tax_%ld", storeid);
    CORBA::Object_var taxObj;
    try {
        taxObj = NsPublisher::nsResolve(ns, caStr, "Primer_Example");
    }
    catch(const NsPublisherException& npe) {
        _abort(npe.toString());
    }
        
    m_taxRef = AStore::Tax::_narrow(taxObj);
        
        
    sprintf(caStr, "Store_%ld", storeid);
    CORBA::Object_var storeObj;
    try {
        storeObj = NsPublisher::nsResolve(ns, caStr, "Primer_Example");
    }
    catch(const NsPublisherException& npe) {
        _abort(npe.toString());
    }
                
    m_storeRef = AStore::Store::_narrow(storeObj);
        
    sprintf(caStr,"OutputMedia_%ld", id);
    CORBA::Object_var outputObj;
    try {
        outputObj = NsPublisher::nsResolve(ns, caStr, "Primer_Example");
    }
    catch(const NsPublisherException& npe) {
        _abort(npe.toString());
    }

    m_outputMediaRef = POS::OutputMedia::_narrow(outputObj);

    // Make sure all the _narrow calls succeeded
    if ( CORBA::is_nil(m_taxRef) || CORBA::is_nil(m_storeRef) || CORBA::is_nil(m_outputMediaRef) ) {
        // Just exit for the demo, in real life it would be better
        // to raise an exception.  
        _abort("ERROR: POSTerminal failed to narrow objecst from Naming Service");                      
    }
  
    // Initialize the member variables to their appropriate values
    m_itemBarcode = (char*)0;
    m_storeAccessRef = AStore::StoreAccess::_nil();
    m_id = id;
    m_POSTaxTotal = m_POSTotal = 0.0;
    m_saleTaxableSubTotal = m_saleSubTotal = 0.0;
}


void POSTerminalImpl::login(
) {
    if (!CORBA::is_nil(m_storeAccessRef)) {
        cerr << "Cant log in twice" << endl;
        return;
    }

    // Get a reference to the StoreAccess object for this POS
    try
    {
        m_storeAccessRef = m_storeRef->login(m_id);
    }
    catch (const CORBA::Exception& )
    {
        _abort("CORBA::Exception caught. Error in Login");
    }

    m_saleSubTotal = m_POSTotal = m_POSTaxTotal = 0.0;
    m_itemQuantity = 1;
}


void POSTerminalImpl::print_POS_sales_summary(
) 
{
    if (!_loggedIn())
        return;

    if ((m_itemBarcode != 0) || (m_saleSubTotal != 0.0))
        return;

    char caOpStr[255];
    sprintf(caOpStr,"%25s %8.2f\n%25s %8.2f\n","Point of Sale Total := ", 
            m_POSTotal, "Tax Total := ",m_POSTaxTotal);

    try
    {
        m_outputMediaRef->output_text(caOpStr);
    }
    catch (const CORBA::Exception& )
    {
        _abort("CORBA::Exception caught. Error printing Sales Summary");
    }
}


void POSTerminalImpl::print_store_sales_summary (
) 
{
    if (!_loggedIn())
        return;

    if ((m_itemBarcode != 0) || (m_saleSubTotal != 0.0))
        return;

    char caOpStr[255];

    // Find and output the total sales and tax for the store
    AStore::Store::StoreTotals tots;
    try {
        tots = m_storeRef->totals();
    }
    catch(const CORBA::Exception& ) {
        _abort("CORBA::Exception caught. Error finding store Totals");
    }

    sprintf(caOpStr,"%s %7.2f\n%s %7.2f\n"," Total Sales :=",tots.store_total,
            " Total Tax   :=",tots.store_tax_total);

    try {
        m_outputMediaRef->output_text("STORE TOTALS");
        m_outputMediaRef->output_text(caOpStr);
    }
    catch(const CORBA::Exception& ) {
        _abort("CORBA::Exception caught. Error with Output Media");
    }

    // Output the Totals for each POS in turn
    AStore::POSList_var pList;
    try {
        m_storeRef->get_POS_totals(pList);
    }
    catch(const CORBA::Exception& ) {
        _abort("CORBA::Exception caught. Error Getting Store Totals");
    }

    for (CORBA::ULong i = 0; i < pList->length(); i++)
    {
        if (pList[i].id > 0) {
            sprintf(caOpStr,"%15s %ld\n%15s %9.2f\n%15s %9.2f\n","POS   I.D.  :=",
                    pList[i].id,"Total Sales :=",pList[i].total_sales,"Total Tax   :=",
                    pList[i].total_taxes);
                        
            try {
                m_outputMediaRef->output_text(caOpStr);
            }
            catch(const CORBA::Exception& ) {
                _abort("CORBA::Exception caught. Error with Output Media");
            }
        }
    }
}


void POSTerminalImpl::send_barcode(const char *item
)
{
    if (!_loggedIn())
        return;

    AStore::ItemInfo_var itemInf;
    CORBA::Float price;
    CORBA::Float taxablePrice;
    m_itemBarcode = item;
    
    try
    {
        m_storeAccessRef->find_price(m_itemBarcode, m_itemQuantity,
                                     price, taxablePrice, itemInf);
    }
    catch (const AStore::BarcodeNotFound &)
    {
        m_itemQuantity = 1;    
        cerr << "Invalid Barcode Found" << endl;
        return;
    }
    catch(const CORBA::Exception&) {
        _abort("CORBA::Exception caught. Error in find Price");
    }

    CORBA::Float itemExt = (float)m_itemQuantity * price;
    m_saleSubTotal += itemExt;
  
    char caOpStr[255];
    char * szFmtStr;
    szFmtStr = "%3d %10s %20s %7.2f %7.2f %s";
    sprintf(caOpStr, szFmtStr, m_itemQuantity, (const char*)m_itemBarcode,
            (const char*)itemInf->name, price, itemExt,
            ((taxablePrice > 0.0) ? " *" : "") );

    try
    {
        m_outputMediaRef->output_text(caOpStr);
    }
    catch (const CORBA::Exception &)
    {
        _abort("CORBA::Exception caught. Error with Output Media");
    }
    m_saleTaxableSubTotal += taxablePrice * (CORBA::Float)m_itemQuantity;
    m_itemQuantity = 1;
}


void POSTerminalImpl::item_quantity(CORBA::Long quantity
)
{
    if (!_loggedIn())
        return;

    if (quantity > 0)
        m_itemQuantity = quantity;
}


void POSTerminalImpl::end_of_sale(
) 
{
    char caOpStr[255];
    if (!_loggedIn())
        return;

    sprintf(caOpStr,"Taxable Sub-Total := %8.2f",m_saleTaxableSubTotal);
    m_outputMediaRef->output_text(caOpStr);
    CORBA::Float saleTax = m_taxRef->calculate_tax(m_saleTaxableSubTotal);
    sprintf(caOpStr,"Taxes             := %8.2f",saleTax);
    m_outputMediaRef->output_text(caOpStr);
    CORBA::Float saleTotal = saleTax + m_saleSubTotal;
    sprintf(caOpStr,"Total             := %8.2f",saleTotal);
    m_outputMediaRef->output_text(caOpStr);
    m_POSTotal += saleTotal;
    m_POSTaxTotal += saleTax;

    try {
        m_storeRef->update_store_totals(m_id,saleTotal,saleTax);
    }
    catch(const CORBA::Exception& ) {
        _abort("CORBA::Exception caught. Error Ending sale");
    }

    m_saleSubTotal = m_saleTaxableSubTotal = 0;
    m_itemQuantity = 1;
    m_itemBarcode = (char*)0;
    return;
}


unsigned char POSTerminalImpl::_loggedIn()
{
    if (CORBA::is_nil(m_storeAccessRef)) {
        cerr << "Need to log in first" << endl;
        return 0;
    }
    else
        return 1;
}
