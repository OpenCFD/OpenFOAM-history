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

#ifndef __SECURITYADMIN_IMPL_H__
#define __SECURITYADMIN_IMPL_H__

#include <CORBA.h>
#include <mico/impl.h>
#include <mico/throw.h>
#include <mico/template_impl.h>
#include <mico/util.h>
#include <mico/ssl.h>
#include <mico/intercept.h>

namespace MICOSA
{
    // Audit implementation

    struct AuditStruct {
	Security::SelectorValueList selectors_;
	Security::AuditEventType event_;
	Security::AuditCombinator audit_combin_;
    
	AuditStruct(const AuditStruct& s);
	AuditStruct& operator=(const AuditStruct& s);
    
	AuditStruct()
	{}
	~AuditStruct()
	{}
    };

    typedef std::map<std::string, AuditStruct*, std::less<std::string> > AuditStructMap;

    class AuditPolicy_impl
	: virtual public SecurityAdmin::AuditPolicy,
	  virtual public MICO::Policy_impl
    {
    private:
	Security::AuditChannelId channel_id_;
	  
    protected:
	AuditStructMap audit_map_;
	//      string mapkey;
	Security::AuditEventTypeList standard_events_;

	//	void mkkey(const char * name, Security::AuditEventType event_type);
	char*
	mkkey(const char* name, Security::AuditEventType event_type);
	
    public:
	AuditPolicy_impl();
	~AuditPolicy_impl();
	CORBA::Policy_ptr copy ();
	//operators
      
	virtual CORBA::PolicyType
	policy_type()
	{ return Security::SecApplicationAudit; }

	virtual void
	set_audit_selectors
	(const char* object_type, const Security::AuditEventTypeList& events,
	 const Security::SelectorValueList& selectors,
	 Security::AuditCombinator audit_combinator);

	virtual void
	clear_audit_selectors
	(const char* object_type, const Security::AuditEventTypeList& events);

	virtual void
	replace_audit_selectors
	(const char* object_type, const Security::AuditEventTypeList& events,
	 const Security::SelectorValueList& selectors,
	 Security::AuditCombinator audit_combinator);

	virtual void
	get_audit_selectors
	(const char* object_type, const Security::AuditEventType& event_type,
	 Security::SelectorValueList_out selectors,
	 Security::AuditCombinator_out audit_combinator);

	virtual void
	set_audit_channel(Security::AuditChannelId audit_channel_id);
    };
  
    // ??
    class AuditClientPolicy_impl
	: virtual public SecurityAdmin::AuditClientPolicy,
	  virtual public MICOSA::AuditPolicy_impl
    {
    public:
	AuditClientPolicy_impl();
	CORBA::Policy_ptr copy ();

	CORBA::PolicyType
	policy_type()
	{ return Security::SecClientInvocationAudit; }
    };
    // ??
    class AuditTargetPolicy_impl
	: virtual public SecurityAdmin::AuditClientPolicy,
	  virtual public MICOSA::AuditPolicy_impl
    {
    public:
	AuditTargetPolicy_impl();

	CORBA::Policy_ptr
	copy ();

	CORBA::PolicyType
	policy_type()
	{ return Security::SecTargetInvocationAudit; }
    };
  
    // Access implementation
  
    typedef std::map<std::string, Security::RightsList *, std::less<std::string> > SecAttributeMap;
  
    class AccessPolicy_impl
	: virtual public SecurityAdmin::AccessPolicy,
	  virtual public MICO::Policy_impl
    {
    public:
	AccessPolicy_impl();
	~AccessPolicy_impl();

	CORBA::Policy_ptr
	copy();

	virtual Security::RightsList*
	get_effective_rights
	(const Security::AttributeList& attribute_list,
	 const Security::ExtensibleFamily& rights_family);

	virtual Security::RightsList*
	get_all_effective_rights(const Security::AttributeList& attrib_list);
      
    protected:
	void attribute_to_key(const Security::SecAttribute& attr);
	std::string key_;
	SecAttributeMap atrmap_;
    };
  
    // 
    class AccessClientPolicy
	: virtual public MICOSA::AccessPolicy_impl
    {
    public:
	AccessClientPolicy();

	CORBA::PolicyType
	policy_type()
	{ return Security::SecClientInvocationAccess; }
    };
    // 
    class AccessTargetPolicy
	: virtual public MICOSA::AccessPolicy_impl
    {
    public:
	AccessTargetPolicy();

	CORBA::PolicyType
	policy_type()
	{ return Security::SecTargetInvocationAccess; }
    };
    // 
    class AccessApplicationPolicy
	: virtual public MICOSA::AccessPolicy_impl
    {
    public:
	AccessApplicationPolicy();

	CORBA::PolicyType
	policy_type()
	{ return Security::SecApplicationAccess; }
    };

    class DomainAccessPolicy_impl
	: virtual public SecurityAdmin::DomainAccessPolicy,
	  virtual public AccessTargetPolicy
	//  virtual public AccessPolicy_impl
    {
    public:
	DomainAccessPolicy_impl();
	~DomainAccessPolicy_impl();

	virtual void
	grant_rights
	(const Security::SecAttribute& priv_attr,
	 Security::DelegationState del_state,
	 const Security::RightsList& rights);

	virtual void
	revoke_rights
	(const Security::SecAttribute& priv_attr,
	 Security::DelegationState del_state,
	 const Security::RightsList& rights);

	virtual void
	replace_rights
	(const Security::SecAttribute& priv_attr,
	 Security::DelegationState del_state,
	 const Security::RightsList& rights);

	Security::RightsList*
	get_rights
	(const Security::SecAttribute& priv_attr,
	 Security::DelegationState del_state,
	 const Security::ExtensibleFamily& rights_family);

	Security::RightsList*
	get_all_rights
	(const Security::SecAttribute& priv_attr,
	 Security::DelegationState del_state);
    };
  
    // for new mapping
  
    struct RightsRecord {
	RightsRecord()
	{}
	~RightsRecord()
	{}
	RightsRecord(const RightsRecord& s);

	RightsRecord&
	operator=(const RightsRecord& s);
    
	Security::RightsList rightslist_;        // List of required rights
	std::string opname_;                          // operation name
	Security::RightsCombinator combinator_;  // Rights combinator
    };
  
    typedef std::map<std::string, RightsRecord *, std::less<std::string> > RightsMap;
  
    class ObjectAccessPolicy_impl
	: virtual public SecurityAdmin::ObjectAccessPolicy,
	  virtual public MICO::Policy_impl
    {
    public:
	ObjectAccessPolicy_impl();
	~ObjectAccessPolicy_impl();

	CORBA::Policy_ptr
	copy();

	virtual void
	get_required_rights
	(CORBA::Object_ptr obj,
	 const char* operation_name,
	 const char* interface_name,
	 Security::RightsList_out rights,
	 Security::RightsCombinator& rights_combinator);

	virtual void
	set_required_rights
	(const char* operation_name,
	 const char* interface_name,
	 const Security::RightsList& rights,
	 Security::RightsCombinator rights_combinator);

	CORBA::PolicyType
	policy_type()
	{ return Security::SecTargetInvocationAccess; }
      
    private:
	RightsMap rightsmap_;
    };
  
} // end namespace

#endif // __SECURITYADMIN_IMPL_H__
