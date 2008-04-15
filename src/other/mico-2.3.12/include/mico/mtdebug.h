/* -*- mode: c++; c-basic-offset: 4; -*-
 *
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
 * 
 *  spezial debug support
 *  Copyright (C) 1999 Andreas Schultz                                 
 * 
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  For more information, visit the MICO Home Page at
 *  http://www.mico.org/
 * 
 */


#ifndef __mico_mtdebug_h__
#define __mico_mtdebug_h__

//  #include <mico/util.h>

extern void __mtdebug_lock();
extern void __mtdebug_unlock();

#ifdef MTDEBUG
#  define DEBUG_NAMES
#  define __NAME( x )  x
#else
#  define __NAME( x )
#  undef DEBUG_NAMES
#endif // MTDEBUG

#ifdef DEBUG_NAMES
class NamedObject {
protected:
    char* _name;
public:
    NamedObject ();
    // get this object's Name (if assigned)
    const char *name (const char *new_name = NULL);
    ~NamedObject ();
};
#endif  //DEBUG_NAMES

namespace MICOMT
{

class AutoDebugLock
{
public:
    AutoDebugLock();
    ~AutoDebugLock();
}; // AutoDebugLock

} // MICOMT

#endif  //__mico_mtdebug_h__

