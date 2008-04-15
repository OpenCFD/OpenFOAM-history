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
#include "POSTerminalImpl.h"
#include <stdlib.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif


using namespace std;

// Include Naming Service.
// Each ORB implementation will provide a different header file 
// to include.
#if 1
// MICO
#include <coss/CosNaming.h>
#endif


int main(int argc, char **argv)
{
    if (argc<3) {
        cerr << "usage: " << argv[0] << " <Store Number> <POS Number> <vendor specific options>" << endl;
        return 1;
    }

    CORBA::ORB_ptr orb;
#if 0
    POS::POSTerminal_var ter;
#else
    // MICO
    POSTerminalImpl* ter;
#endif

    try {
#if 0
        orb  = CORBA::ORB_init(argc, argv, "PRIMER_ORB"); 
#else
        // MICO
        orb  = CORBA::ORB_init(argc, argv, "mico-local-orb");
	CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
	PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
	PortableServer::POAManager_var mgr = poa->the_POAManager ();
	mgr->activate ();
#endif


        CORBA::Object_var obj = orb->resolve_initial_references("NameService");
        CosNaming::NamingContext_var naming_service =
            CosNaming::NamingContext::_narrow(obj);
    
        if (CORBA::is_nil(naming_service)) {
            cerr << "ERROR: cant narrow naming service" << endl;

            return 1;
        }
        
        cout <<"Creating POSTerminalImpl" << endl;
        ter = new POSTerminalImpl(naming_service, atol(argv[1]), atol(argv[2]));
#if 1
	// MICO
	ter->_this();
#endif
        cout << "Terminal started" <<endl <<endl;

    }
    catch(const CORBA::Exception& ) {
        cerr << "ERROR Starting POS Terminal Media" << endl;
                
        return 1;
    }

#if 0
    POS::InputMedia_var input;
#else
    // MICO
    InputMediaImpl* input;
#endif

    char caBuff[255];

    try {
        input = new InputMediaImpl(ter->_this ());
#if 1
	// MICO
	input->_this();
#endif
    }
    catch(const CORBA::Exception& ) {
        cerr << "Error starting InputMedia" << endl;

    }

    cout << "Command Summary :-" << endl;
    cout << "L : Login    P : POS Sales Summary    S : Store Sales Summary" << endl;
    cout << "T : Total    Q : Quantity             X : Exit" << endl << endl;

    do {
        cout << "Enter code, command (L,P,Q,S,T) or X to exit : ";
                
        try {
            cin.getline(caBuff,250);
            // the next 4 lines of code were added to remove
            // the newline character that is added to the
            // input text.
            // This seemed to be a problem on windows platforms.
            int c = cin.gcount();
            
            for( int ic = 0; ic< c; ic++ ) {
                if( caBuff[ic] == '\r' ) caBuff[ic] = '\0';
            }

            if ((caBuff[0] >= '0') && (caBuff[0] <= '9'))
                input->barcode_input(caBuff);
            else
                input->keypad_input(caBuff);
        }
        catch(const CORBA::Exception& ) {
            cerr << "ERROR using Input Media" << endl;
            caBuff[0] = 'x';
        }
    } 
    while (caBuff[0] != 'x' && caBuff[0] != 'X');

    return 0;
}
