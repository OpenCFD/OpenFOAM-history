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

#ifndef __SECURITYLEVEL1_IMPL_H__
#define __SECURITYLEVEL1_IMPL_H__

#define FAMILY_NUMBER 4

namespace MICOSL1
{
    class Current_impl
	: virtual public SecurityLevel1::Current
    {
	CORBA::ORB_ptr orb_;
    public:
	Current_impl(CORBA::ORB_ptr _orb);
	Current_impl();
	~Current_impl();

	virtual Security::AttributeList*
	get_attributes(const Security::AttributeTypeList& attributes);

	virtual Security::AttributeList*
	get_target_attributes
	(const Security::AttributeTypeList& attributes,
	 CORBA::Object_ptr obj);
    };
}

#endif // __SECURITYLEVEL1_IMPL_H__
