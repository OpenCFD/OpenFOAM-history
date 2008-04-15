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


#include <CORBA.h>
#include <mico/impl.h>
#include <mico/poa_impl.h>
#include <mico/security/securitylevel2_impl.h>


MICOSL1::Current_impl::~Current_impl()
{
}

MICOSL1::Current_impl::Current_impl(CORBA::ORB_ptr orb)
{
  orb_ = CORBA::ORB::_duplicate(orb);
  orb_->set_initial_reference("SecurityCurrent", this);
}

MICOSL1::Current_impl::Current_impl()
{
}

//static char MICO_defining_authority[] = "";

Security::AttributeList* 
MICOSL1::Current_impl::get_attributes
(const Security::AttributeTypeList& attributes)
{
    SecurityLevel2::ReceivedCredentials_ptr rc = new MICOSL2::ReceivedCredentials_impl(orb_);
    return rc->get_attributes(attributes);
}

Security::AttributeList*
MICOSL1::Current_impl::get_target_attributes
(const Security::AttributeTypeList& attributes,
 CORBA::Object_ptr obj)
{
    SecurityLevel2::TargetCredentials_ptr tc = new MICOSL2::TargetCredentials_impl(orb_, obj);
    return tc->get_attributes(attributes);
}

