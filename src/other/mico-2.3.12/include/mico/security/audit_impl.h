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

#ifndef __AUDIT_IMPL_H__
#define __AUDIT_IMPL_H__

#include <mico/ssl.h>
#include <mico/intercept.h>
#include <mico/security/security.h>
#include <mico/security/securitylevel2.h>
#include <mico/security/SecurityAdmin_impl.h>

#ifdef  HAVE_PGSQL
#include <pgsql/libpq-fe.h>
#endif

namespace MICOSL2
{
    //class MICOSA::AuditPolicy_impl;
    class Archive
    {
    public:
	Archive();
	virtual ~Archive();

	virtual CORBA::Boolean
	create(const char* name);

	virtual CORBA::Boolean
	write
	(const Security::AuditEventType& event_type,
	 const SecurityLevel2::CredentialsList& creds,
	 const Security::UtcT& time,
	 const Security::SelectorValueList& descriptors,
	 const Security::Opaque& event_specific_data);
    protected:
	char* name_;

	std::string
	make_output_string
	(const Security::AuditEventType& event_type,
	 const Security::SelectorValueList& descriptors);
    };


    class FileArchive
	: public MICOSL2::Archive
    {
    public:
	FileArchive();
	virtual ~FileArchive();

	virtual CORBA::Boolean
	create(const char* name);

	virtual CORBA::Boolean
	write
	(const Security::AuditEventType& event_type,
	 const SecurityLevel2::CredentialsList& creds,
	 const Security::UtcT& time,
	 const Security::SelectorValueList& descriptors,
	 const Security::Opaque& event_specific_data);
    protected:
	FILE* file_;
    };

#ifdef  HAVE_PGSQL
    class DBArchive
	: public MICOSL2::Archive
    {
    public:
	DBArchive();
	~DBArchive();

	virtual CORBA::Boolean
	create(const char* name);

	virtual CORBA::Boolean
	write
	(const Security::AuditEventType& event_type,
	 const SecurityLevel2::CredentialsList& creds,
	 const Security::UtcT& time,
	 const Security::SelectorValueList& descriptors,
	 const Security::Opaque& event_specific_data);

    private:
	PGconn* conn_;
	char* table_name_;
    };
#endif

    class ConsoleArchive
	: public MICOSL2::Archive
    {
    public:
	ConsoleArchive(int priority);
	virtual ~ConsoleArchive();

	virtual CORBA::Boolean
	create(const char* name);

	virtual CORBA::Boolean
	write
	(const Security::AuditEventType& event_type,
	 const SecurityLevel2::CredentialsList& creds,
	 const Security::UtcT& time,
	 const Security::SelectorValueList& descriptors,
	 const Security::Opaque& event_specific_data);
    private:
	int priority_;
    };

    class AuditChannelList
    {
    public:
	AuditChannelList();
	~AuditChannelList();

	Security::AuditChannelId
	register_channel(SecurityLevel2::AuditChannel*);

	void
	unregister_channel(Security::AuditChannelId);

    private:
	IfaceSequenceTmpl<SecurityLevel2::AuditChannel*,SecurityLevel2::AuditChannel*> list_;
    };


    class AuditChannel_impl
	: virtual public SecurityLevel2::AuditChannel
    {
    public:
	AuditChannel_impl();
	AuditChannel_impl(const char *type_str,const char *archive_name = "");
	~AuditChannel_impl();

	virtual CORBA::Boolean
	create(const char* type_str, const char* archive_name = "");

	virtual Security::AuditChannelId
	audit_channel_id();

	virtual void
	audit_write
	(const Security::AuditEventType& event_type,
	 const SecurityLevel2::CredentialsList& creds,
	 const Security::UtcT& time,
	 const Security::SelectorValueList& descriptors,
	 const Security::Opaque& event_specific_data);

    private:
	Security::AuditChannelId channel_id_;
	MICOSL2::Archive* archive_;
    };


    class AuditDecision_impl
	: virtual public SecurityLevel2::AuditDecision
    {
    protected:
	MICOSL2::AuditChannel_impl* audit_chann_;

	CORBA::Boolean
	decision
	(const Security::AuditEventType& event_type,
	 const Security::SelectorValueList& value_list,
	 SecurityDomain::DomainManagerAdmin_ptr dm,
	 SecurityDomain::PolicyCombinator comb);

    public:
	AuditDecision_impl();
	AuditDecision_impl(Security::AuditChannelId channel_id);
	~AuditDecision_impl();

	CORBA::Boolean
	create(const char *type_str, const char *archive_name = "");

	virtual SecurityLevel2::AuditChannel_ptr
	audit_channel();

	virtual CORBA::Boolean
	audit_needed
	(const Security::AuditEventType& event_type,
	 const Security::SelectorValueList& value_list);
    };
} // MICOSL2

#endif // __AUDIT_IMPL_H__

