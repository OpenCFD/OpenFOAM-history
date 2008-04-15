// -*- c++ -*-
/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
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
 */

#ifndef __corba_small_h__
#define __corba_small_h__

// dont need Interface Repository?
#ifndef MICO_CONF_IR
#define MICO_CONF_NO_IR
#endif

// dont need Implementation Repository?
#ifndef MICO_CONF_IMR
#define MICO_CONF_NO_IMR
#endif

// dont need Portable Object Adaptor?
#ifndef MICO_CONF_POA
#define MICO_CONF_NO_POA
#endif

// dont need Interceptors?
#ifndef MICO_CONF_INTERCEPT
#define MICO_CONF_NO_INTERCEPT
#endif

// dont need Dynamic Any?
#ifndef MICO_CONF_DYNANY
#define MICO_CONF_NO_DYNANY
#endif

#include <CORBA.h>

#endif // __corba_small_h__
