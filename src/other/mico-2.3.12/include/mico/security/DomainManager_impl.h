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

#ifndef __SDM_IMPL_H__
#define __SDM_IMPL_H__

#include <CORBA.h>
#include <mico/impl.h>
#include <mico/template_impl.h>
#include <mico/util.h>
#include <mico/throw.h>


namespace MICOSDM
{

    extern void _init ();

    // SDM options (-SDMConfig)

    class SDMOptions
    {
    public:
	CORBA::Boolean
	parse(CORBA::ORB_ptr, int&, char* []);

	const char*
	operator[](const char*);

    private:
	std::map<std::string, std::string, std::less<std::string> > options_;
    };

    extern MICO_EXPORT SDMOptions sdmopts;


    class DomainAuthorityAdmin_impl;
    class DomainManagerAdmin_impl;
    class DomainAuthority_impl;

    class DomainManagerFactory_impl
	: virtual public SecurityDomain::DomainManagerFactory
    {
    public:
	DomainManagerFactory_impl();
	~DomainManagerFactory_impl()
	{};

	virtual SecurityDomain::DomainManagerAdmin_ptr
	get_root_domain_manager(const char* type);

	virtual void
	add_root_domain_manager(const char* type);

	virtual SecurityDomain::DomainManagerAdmin_ptr
	create_domain_manager();

	virtual void
	release_domain_manager(SecurityDomain::DomainManagerAdmin_ptr domain_manager);

	virtual CORBA::Boolean
	load_config_file(const char* filename, const char* type);
	
    private:
	SecurityDomain::DomainManagerAdminList roots_;
    };

    class DomainManagerAdmin_impl
	: virtual public SecurityDomain::DomainManagerAdmin// ,
	//public MICO::DomainManager_impl
    {
    public:
	~DomainManagerAdmin_impl();
	DomainManagerAdmin_impl();

	virtual CORBA::Policy_ptr
	get_domain_policy(CORBA::PolicyType policy_type); //

	virtual void
	set_domain_policy(CORBA::Policy_ptr policy); //

	virtual void
	delete_domain_policy(CORBA::PolicyType policyType);

	// extension
	virtual SecurityDomain::Name*
	get_name();

	virtual void
	set_name(const SecurityDomain::Name& dname);

	virtual SecurityDomain::DomainManagerAdminList*
	get_child_domain_managers();

	virtual SecurityDomain::DomainManagerAdminList*
	get_parent_domain_managers();

	virtual void
	set_parent_domain_managers
	(const SecurityDomain::DomainManagerAdminList& managers);
    protected:
	SecurityDomain::Name name_;
	SecurityDomain::DomainManagerAdminList parents_;
	SecurityDomain::DomainManagerAdminList children_;
	SecurityDomain::CombinatorInfoList combinators_;

    private:
	CORBA::PolicyList policies_;
    };

    class DomainAuthority_impl
	: virtual public SecurityDomain::DomainAuthority,
	  virtual public DomainManagerAdmin_impl
    {
    public:
	~DomainAuthority_impl();
	DomainAuthority_impl();

	virtual CORBA::Boolean
	is_root();

	virtual CORBA::Boolean
	exists();

	virtual SecurityDomain::PolicyCombinator
	get_policy_combinator(CORBA::PolicyType policyType);

	virtual SecurityDomain::PolicyCombinator
	get_policy_combinator_for_multi_parents
	(CORBA::PolicyType policy_type,
	 SecurityDomain::DomainManagerAdmin_ptr parent);

	virtual SecurityDomain::NameIterator_ptr
	get_child_domain_names();

	virtual SecurityDomain::Name*
	get_domain_name(SecurityDomain::DomainManagerAdmin_ptr domain_manager);

	virtual SecurityDomain::DomainManagerAdmin_ptr
	get_domain_manager(const SecurityDomain::Name& domain_name);
    };

    class DomainAuthorityAdmin_impl
	: virtual public SecurityDomain::DomainAuthorityAdmin,
	  virtual public DomainAuthority_impl //,
	// virtual public DomainManagerAdmin_impl
    {
    public:
	~DomainAuthorityAdmin_impl();
	DomainAuthorityAdmin_impl();

	virtual void
	add_domain_manager
	(SecurityDomain::DomainManagerAdmin_ptr domain_manager,
	 const SecurityDomain::Name& domainName);

	virtual void
	remove_domain_manager(SecurityDomain::DomainManagerAdmin_ptr child);

	virtual void set_policy_combinator
	(CORBA::PolicyType policy_type,
	 SecurityDomain::PolicyCombinator combinator);

	virtual void
	set_policy_combinator_for_multi_parents
	(CORBA::PolicyType policy_type, SecurityDomain::PolicyCombinator combinator, 
	 SecurityDomain::DomainManagerAdmin_ptr parent);
    };


    class NameIterator_impl
	: virtual public SecurityDomain::NameIterator
    {
    public:
	~NameIterator_impl();
	NameIterator_impl() {};

	virtual CORBA::Boolean
	next_one(SecurityDomain::Name*& domain_name);

	virtual void
	destroy();
    };


    class NameExt_impl : virtual public SecurityDomain::NameExt
    {
    public:
	~NameExt_impl() {};
	NameExt_impl() {};

	virtual char*
	to_string(const SecurityDomain::Name& domain_name);

	virtual SecurityDomain::Name*
	to_name(const char* url_name);

    protected:
	void
	parse_name(const char* name);

    private:
	SecurityDomain::NameComponent nc_;
    };


} // MICOSDM

#endif // __SDM_IMPL_H__
