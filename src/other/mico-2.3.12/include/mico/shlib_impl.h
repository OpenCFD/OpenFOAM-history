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

#ifndef __mico_shlib_impl_h__
#define __mico_shlib_impl_h__

namespace MICO {

class UnixSharedLib : public MICO::SharedLib {
    void *_handle;
    std::string _error;
    std::string _name;
public:
    UnixSharedLib (const char *name);
    virtual ~UnixSharedLib ();

    virtual void *symbol (const char *);
    virtual const char *error ();
    virtual operator CORBA::Boolean ();

    virtual const char *name ();
};

}

#endif // __mico_shlib_impl_h__
