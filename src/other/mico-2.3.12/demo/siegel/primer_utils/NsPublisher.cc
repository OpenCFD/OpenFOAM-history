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

#include "NsPublisher.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif


using namespace std;

// Include Naming Service.
// Each ORB implementation will provide a different header file 
// to include.


void NsPublisher::nsBind(CORBA::Object_ptr nsObj,
                         CORBA::Object_ptr bindObj,
                         const char * id,
                         const char * kind ) {

    // Narrow Name Service reference here for type checking
    // and so that we can handle general objects obtained from
    // resolve_initial_references
    CosNaming::NamingContext_var ns     = CosNaming::NamingContext::_narrow(nsObj);


    // Must have valid non null parameters

    if (CORBA::is_nil(ns))
        throw NsPublisherException("NsPublisher::bind - object is null or not of NamingContext type");

    if (CORBA::is_nil(bindObj))
        throw NsPublisherException("NsPublisher::bind - null object reference parameter passed");

    if ( (id == (const char *)0)  )
        throw NsPublisherException("NsPublisher::bind - null String ID passed");

    if ( (kind == (const char *)0))
        throw NsPublisherException("NsPublisher::bind - null String ID passed");


    CosNaming::Name_var name;

    try
    {
        name = new CosNaming::Name(1);
        name->length(1);
        name[(CORBA::ULong)0].id = id;
        name[(CORBA::ULong)0].kind = kind;

        try
        {
            ns->bind(name, bindObj);
        }
        catch(CosNaming::NamingContext::AlreadyBound ab)
        {
            ns->rebind(name, bindObj);
        }
    }
    catch (const CORBA::SystemException& se)
    {
        cerr << "CORBA::SystemException caught" << endl;
        throw se;
    }
    catch (const CORBA::UserException& )
    {
        cerr << "CORBA::UserException caught" << endl;
        throw NsPublisherException( "NsPublisher::bind - CORBA User Exception");
    }
}

CORBA::Object_ptr NsPublisher::nsResolve (CORBA::Object_ptr nsObj,
                                          const char * id,
                                          const char * kind) {

    CORBA::Object_ptr resolveObj = NULL;

        // Narrow Name Service reference here for type checking
        // and so that we can handle general objects obtained from
        // org.omg.CORBA.ORB.resolve_initial_references
    CosNaming::NamingContext_var ns     = CosNaming::NamingContext::_narrow(nsObj);


    // Must have  valid non null parameters

    if (CORBA::is_nil(ns))
        throw NsPublisherException( "NsPublisher::bind - object is null or not of NamingContext type");

    if ( (id == (const char *)0) )
        throw NsPublisherException( "NsPublisher::bind - null String ID passed");

    if ( (kind == (const char *)0) )
        throw NsPublisherException( "NsPublisher::bind - null String ID passed");


    CosNaming::Name_var name;

    try
    {
        name = new CosNaming::Name(1);
        name->length(1);
        name[(CORBA::ULong)0].id = id;
        name[(CORBA::ULong)0].kind = kind;

        resolveObj = ns->resolve( name );
    }
    catch (const CORBA::SystemException& se)
    {
        cerr << "CORBA::SystemException caught" << endl;
        throw se;
    }
    catch (const CORBA::UserException& )
    {
        cerr << "CORBA::UserException caught" << endl;
        throw NsPublisherException( "NsPublisher::resolve - CORBA User Exception");
    }

    return resolveObj;

}
