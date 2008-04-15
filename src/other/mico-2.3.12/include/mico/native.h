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


#ifndef __mico_native_h__
#define __mico_native_h__

/*
 * These are definitions for "native" idl types. They are realized as
 * macros rather than typedefs because they may refer to types in
 * other contexts (namespaces) that we don't have access to.
 *
 * The idl definition
 *
 * module ABC {
 *   native DEF;
 * };
 *
 * results in
 *
 * namespace ABC {
 *   typedef MICO_Native_ABC_DEF DEF;
 * };
 */

#define MICO_Native_CORBA_AbstractBase CORBA::AbstractBase*
#define MICO_Native_PortableServer_Servant PortableServer::ServantBase*
#define MICO_Native_PortableServer_ServantLocator_Cookie void*

#endif
