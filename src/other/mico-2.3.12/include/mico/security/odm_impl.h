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

#ifndef __ODM_IMPL_H__
#define __ODM_IMPL_H__

#include <CORBA.h>

namespace MICOSODM
{
    class Manager_impl;
    struct ODMRecord;

    extern void _init ();

    extern ODMRecord*
    find_record(std::string& key, MICOSODM::Manager_impl* odm);

    extern void
    remove_record(std::string& key, MICOSODM::Manager_impl* odm);

    extern void
    set_lspolicy
    (MICOSODM::Manager_impl* odm,
     PortableServer::LifespanPolicyValue p);

    extern PortableServer::LifespanPolicyValue
    get_lspolicy(MICOSODM::Manager_impl* odm);

    // ODM options (-ODMConfig)

    class ODMOptions
    {
    public:
	CORBA::Boolean
	parse (CORBA::ORB_ptr, int&, char* []);

	const char*
	operator[](const char *);

    private:
	std::map<std::string, std::string, std::less<std::string> > options_;
    };

    extern MICO_EXPORT ODMOptions odmopts;

    struct ODMRecord
    {
	ODMRecord()
	{}
	~ODMRecord()
	{}
	ODMRecord(const ODMRecord& s);

	ODMRecord&
	operator=(const ODMRecord& s);
    
	ObjectDomainMapping::Manager::ODMGranularity gran_;
	SecurityDomain::NameList dnamelist_; // Domain Namelist
	Manager_impl* dm_;
	std::string x509_;
	PortableServer::LifespanPolicyValue pol_;
    };

    typedef std::map<std::string, ODMRecord *, std::less<std::string> > DomainMap;

    class AllDomainList
    {
    public:
	AllDomainList(std::ostream* o = &std::cout)
	{ out_ = o;}
	~AllDomainList()
	{}

	DomainMap&
	get_domain_map();

	CORBA::Boolean
	load_config(const char* conffile);

	CORBA::Boolean
	load_config();

	CORBA::Boolean
	save_config(const char* conffile);
    
    private:
	DomainMap all_domains_;
	std::ostream* out_;
    };

  
    // Manager interface

    class Manager_impl
	: public ObjectDomainMapping::Manager
    {
    private:
	Manager_impl* parent_;
	Manager_impl* def_parent_;
	//    ObjectDomainMapping::Manager::ODMGranularity gran;
	//    string key;
	std::string def_key_;
	//    PortableServer::POA_ptr poa;
	PortableServer::LifespanPolicyValue pol_;
	AllDomainList* domains_;

    protected:
	std::string
	opaque_to_string(const Security::Opaque& key);
      
    public:
	Manager_impl();
	Manager_impl(AllDomainList* domains);
	~Manager_impl();

	virtual void
	set_domain_name_key
	(ODMGranularity granularity,
	 const Security::Opaque& key,
	 const SecurityDomain::NameList& domain_name_list);

	virtual SecurityDomain::NameList*
	get_domain_names(const Security::Opaque& key);

	virtual void
	remove_domain_names(const Security::Opaque& key);

	virtual void
	set_parent_odm_key
	(const Security::Opaque& key,
	 ObjectDomainMapping::Manager_ptr odm);

	virtual void
	set_default_parent_odm(ObjectDomainMapping::Manager_ptr odm);

	virtual void
	set_default_name_key(const SecurityDomain::NameList& domain_name_list);
      
	friend ODMRecord*
	find_record(std::string& key, MICOSODM::Manager_impl* odm);

	friend void
	remove_record(std::string& key, MICOSODM::Manager_impl* odm);

	friend void
	set_lspolicy
	(MICOSODM::Manager_impl* odm,
	 PortableServer::LifespanPolicyValue p);

	friend PortableServer::LifespanPolicyValue
	get_lspolicy(MICOSODM::Manager_impl* odm);
    };
  
  
    // Manager Factory interface

    class Factory_impl
	: public ObjectDomainMapping::Factory {
    public:
	Factory_impl();
	~Factory_impl();
      
	virtual ObjectDomainMapping::Manager_ptr
	create();

	// extention
	virtual CORBA::Boolean
	load_config_file(const char* cf);

	virtual CORBA::Boolean
	save_config_file(const char* cf);

	AllDomainList* domains_;
    };

    class ODM_impl
	: public ObjectDomainMapping::ODM
    {
    private:
	Factory_impl* cur_factory_;
    
    public:
	ODM_impl()
	{ cur_factory_ = NULL; }
	~ODM_impl()
	{}
    
	virtual ObjectDomainMapping::Factory_ptr
	create()
	{
	    cur_factory_ = new Factory_impl;
	    return cur_factory_;
	}
    
	virtual ObjectDomainMapping::Factory_ptr current()
	{ return cur_factory_; }
    };
} // MICOSODM

#endif // __ODM_IMPL_H__
