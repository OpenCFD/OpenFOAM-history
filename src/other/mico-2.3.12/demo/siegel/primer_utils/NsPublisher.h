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

#ifndef INCLUDED_NSPUBLISHER_H
#define INCLUDED_NSPUBLISHER_H

#include <stdlib.h>
#if 0
#include <corba.h>
#else
// MICO
#include <coss/CosNaming.h>
#endif


class NsPublisherException
{
private:
    CORBA::String_var m_detailMessage;
    
public:
    NsPublisherException() :
        m_detailMessage(CORBA::string_dup("")) {}
    
    NsPublisherException(const char *msg) :
        m_detailMessage(CORBA::string_dup(msg)) {}
    
    const char* toString() const
    { return m_detailMessage; }
};


class NsPublisher
{
public:
    static void nsBind (CORBA::Object_ptr nsObj,
                        CORBA::Object_ptr bindObj,
                        const char * id,
                        const char * kind);
    static CORBA::Object_ptr nsResolve (CORBA::Object_ptr nsObj,
                                        const char * id,
                                        const char * kind);
};

#endif // INCLUDED_NSPUBLISHER_H
