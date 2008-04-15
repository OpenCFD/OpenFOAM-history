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

#include "TaxImpl.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else
#include <fstream.h>
#endif
#include <stdio.h>
#include <stdlib.h>


using namespace std;

const CORBA::Float region_rate = 0.05F;



void TaxImpl::_abort(const char* msg) {
    cerr << msg << endl << flush;
    exit(1);
}


TaxImpl::TaxImpl()
{
    // set tax rate applied to taxable goods
    m_regionRate = region_rate;
}

CORBA::Float TaxImpl::calculate_tax(CORBA::Float taxableAmount
)
{
    return taxableAmount*m_regionRate;
}

CORBA::Float TaxImpl::find_taxable_price(CORBA::Float itemPrice, 
                                         AStore::ItemTypes itemtype
)
{
    CORBA::Float taxprice;

    if (itemtype == AStore::OTHER)
        taxprice = itemPrice;
    else
        taxprice = 0.0;

    return taxprice;
}
