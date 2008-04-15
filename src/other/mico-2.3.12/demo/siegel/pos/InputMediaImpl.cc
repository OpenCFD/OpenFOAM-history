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

#include "InputMediaImpl.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif
#include <stdlib.h>
#include <ctype.h>


using namespace std;

void InputMediaImpl::_abort(const char* msg) {
    cerr << msg << endl << flush;
    exit(1);
}

InputMediaImpl::InputMediaImpl(POS::POSTerminal_ptr pTer)  
{
    // we will comunicate with the following terminal
    m_POSTerminalRef = POS::POSTerminal::_duplicate(pTer);
}

void  InputMediaImpl::barcode_input(const char *item
)
{
    try
    {
        m_POSTerminalRef->send_barcode(item);
    }
    catch (const AStore::BarcodeNotFound &)
    {
        _abort("Error in Sending barcode");
    }
}

void  InputMediaImpl::keypad_input(const char *cmd
)
{
    char *pStr = new char[255];
    strcpy(pStr, cmd);
    try {
        long lTot = 0;
        switch(toupper(*pStr)) {
        default : cerr << "Invalid entry" << endl;
            break;
        case 'X': break;
        case 'L': m_POSTerminalRef->login();
            break;
        case 'P': m_POSTerminalRef->print_POS_sales_summary();
            break;
        case 'S': m_POSTerminalRef->print_store_sales_summary();
            break;
        case 'T': m_POSTerminalRef->end_of_sale();
            break;
        case 'Q': 
            char* temp_pStr = pStr;
            while(*(++temp_pStr) == ' ');
            lTot = atol(temp_pStr);
            m_POSTerminalRef->item_quantity(lTot);
            break;
        }
        
        delete [] pStr;
        
    }
    catch(const CORBA::Exception& ) {
        delete [] pStr;
        cerr << "CORBA::Exception caught in InputMediaImpl::keypad_input" << endl;
        _abort("Error in transmitting command");
    }

}
