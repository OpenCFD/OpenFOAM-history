/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
 * 
 *  os-thread wrapper
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

//
// Define HAVE_THREADS if any type of thread implementations are defined.
// supported "implementations" are:
//
// HAVE_PTHREADS        - POSIX Draft 10 PThreads [Single UNIX Spec V.2 & X/Open Interfaces]
//                        (i.e. HP/UX 11, Linux PThreads (Xavier LeRoy@inria.fr))
// HAVE_DCE_THREADS     - POSIX Draft 4 (DCE) Threads (i.e. HP/UX 10)
// HAVE_WIN_THREADS     - M$ Windows threads or whatever M$ calls them...
// HAVE_SOLARIS_THREADS - SUN Solaris Threads.
// HAVE_OSPACE_THREADS  - Object Space(r) Thread<ToolKit>(tm) via D10/D4/Linux
//                                                                              

#ifdef FAST_PCH
#include "orb_pch.h"
#endif // FAST_PCH
#ifdef __COMO__
#pragma hdrstop
#endif // __COMO__

#ifndef FAST_PCH

#define MICO_CONF_IMR

#include <CORBA-SMALL.h>
#ifndef _WIN32
#include <string.h>
#endif
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <mico/impl.h>

#endif // FAST_PCH

#ifdef HAVE_PTHREADS
#include "os-thread/pthreads.cc"
#endif // HAVE_PTHREADS

#ifdef HAVE_DCE_THREADS
#include "os-thread/dcethreads.cc"
#endif //HAVE_DCE_THREADS

#ifdef HAVE_PTH_THREADS
#include "os-thread/pththreads.cc"
#endif // HAVE_PTH_THREADS

#ifdef HAVE_WIN_THREADS
#error not yet
#endif //HAVE_WIN_THREADS

#ifdef HAVE_SOLARIS_THREADS
#include "os-thread/solaris-threads.cc"
#endif //HAVE_SOLARIS_THREADS


