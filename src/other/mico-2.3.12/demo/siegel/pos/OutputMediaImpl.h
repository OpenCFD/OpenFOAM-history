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

#ifndef OUTPUTMEDIAIMPL_H
#define OUTPUTMEDIAIMPL_H


// Include POS type information via the skeleton.
// The file to include depends on the output of
// your ORBs IDL compiler.

#if 1
// MICO
#include <POS.h>
#endif


// The super class of your implementation class 
// depends on the output of your ORBs IDL compiler.
#if 0
class OutputMediaImpl
#else
// MICO
class OutputMediaImpl : public virtual POA_POS::OutputMedia
#endif
{
private:
    static void _abort(const char *msg);

public:
    OutputMediaImpl();

    CORBA::Boolean output_text(const char * stringToPrint
    );
};

#endif
