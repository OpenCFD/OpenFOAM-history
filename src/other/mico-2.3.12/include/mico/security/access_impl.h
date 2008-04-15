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

#ifndef __ACCESS_IMPL_H__
#define __ACCESS_IMPL_H__

#include <mico/security.h>
#include <mico/securitylevel2.h>
#include <mico/SecurityAdmin_impl.h>


// Access implementation
namespace MICOSL2
{

    class AccessDecision_impl
	: virtual public SecurityLevel2::AccessDecision
    {
    public:
	AccessDecision_impl();
	~AccessDecision_impl();

	virtual CORBA::Boolean
	access_allowed
	(const securitylevel2::CredentialsList& cred_list,
	 CORBA::Object_ptr target,
	 const char* operation_name,
	 const char* target_interface_name);
    };


    class RequiredRights_impl
	: virtual public SecurityLevel2::RequiredRights
    {
    public:
	RequiredRights_impl();
	~RequiredRights_impl();

	virtual void
	get_required_rights
	(CORBA::Object_ptr obj,
	 const char* operation_name,
	 const char* interface_name,
	 Security::RightsList*& rights,
	 Security::RightsCombinator& rights_combinator);

	virtual void
	set_required_rights
	(const char* operation_name,
	 const char* interface_name,
	 const Security::RightsList& rights,
	 Security::RightsCombinator rights_combinator);
    };

}; // MICOSL2

#endif // __ACCESS_IMPL_H__
