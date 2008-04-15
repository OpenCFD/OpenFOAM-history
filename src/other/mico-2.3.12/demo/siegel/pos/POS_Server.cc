/*

Copyright (C) 1998,2000 by the Object Management Group, Inc.  All  
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
from the use of or the inability to use this software.  Some
states do not allow the exclusion of implied warranties, so the
exclusion may not apply to you.

*/

#include "OutputMediaImpl.h"
#include <stdlib.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif
#include <stdio.h>


using namespace std;

// Include Naming Service.
// Each ORB implementation will provide a different header file 
// to include.

#include <NsPublisher.h>


int main(int argc, char* argv[])
{
    if (argc<2) {
        cerr << "usage: " << argv[0] << " <POS Number> <vendor specific options>" << endl;
        return 1;
    }

#if 0  
    POS::OutputMedia_ptr outputMedia;
#else
    // MICO
    OutputMediaImpl* outputMedia;
#endif
  
    try {
#if 0
        CORBA::ORB_ptr orb  = CORBA::ORB_init(argc, argv,"PRIMER_ORB"); 
        CORBA::BOA_ptr boa  = orb->BOA_init(argc, argv, "PRIMER_BOA");
#else
        // MICO
        CORBA::ORB_ptr orb  = CORBA::ORB_init(argc, argv, "mico-local-orb");
	CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
	PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
	PortableServer::POAManager_var mgr = poa->the_POAManager ();
#endif
        
        CORBA::Object_var obj = orb->resolve_initial_references("NameService");

        CosNaming::NamingContext_var naming_service = 
            CosNaming::NamingContext::_narrow(obj);

        if (CORBA::is_nil(naming_service)) {
            cerr << "ERROR: cant narrow naming service" << endl;
            return 1;
        }
                
        long posID = atol(argv[1]);

        outputMedia = new OutputMediaImpl();
          
        // Publish OutputMedia to the Name Service
        char regStr[256];
        sprintf(regStr,"OutputMedia_%ld", posID);
                
        try {
#if 0
            NsPublisher::nsBind(obj, outputMedia, regStr, "Primer_Example" );
#else
	    // MICO
            NsPublisher::nsBind(obj, outputMedia->_this (),
				regStr, "Primer_Example" );
#endif
        }
        catch(const NsPublisherException& npe) {
            cerr << npe.toString() << endl;

            return 1;
        }

        // Everything is ok at this point.
        // Wait for incomming requests. 
#if 0
        boa->impl_is_ready(); 
#else
        // MICO
	mgr->activate ();
        orb->run ();
#endif
    }
    catch (const CORBA::Exception& )
    {
        cout << "CORBA::Exception caught. ERROR Starting OutputMedia Server" << endl;
                        
        return 1;
    }

    return 0;
}
