// -*- c++ -*-
//
//  MICOsec --- a free CORBA Security implementation
//  Copyright (C) 2000 ObjectSecurity Ltd.
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Library General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Library General Public License for more details.
//
//  You should have received a copy of the GNU Library General Public
//  License along with this library; if not, write to the Free
//  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

//  Send comments and/or bug reports to:
//                 micosec@objectsecurity.com
//


#ifndef __AUDITOPTIONS_H__
#define __AUDITOPTIONS_H__

#include <CORBA.h>
#include <mico/template_impl.h>
#include <mico/util.h>

class AuditOptions
{
public:
    CORBA::Boolean
    parse(CORBA::ORB_ptr, int&, char* []);
    
    const char*
    operator[](const char*);

private:
    std::map<std::string, std::string, std::less<std::string> > options_;
};

#endif // __AUDITOPTIONS_H__
