//
//  MICOsec --- a free CORBA Security implementation
//  Copyright (C) 2000, 2001 ObjectSecurity Ltd.
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

#include <mico/impl.h>
#include <mico/template_impl.h>
#include <mico/util.h>
#undef yyFlexLexer
#define yyFlexLexer odmFlexLexer
#include <mico/security/odm_impl.h>
#include <FlexLexer.h>
#include <mico/security/ODMConfig.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else
#include <fstream.h>
#endif


using namespace std;

namespace MICOSODM {
    ODMOptions odmopts; // ConfigFilename = odmopts[-ODMConfig]
}

// Options Initializing

static class ODMInit
    : public Interceptor::InitInterceptor
{
public:
    ODMInit ()
	: Interceptor::InitInterceptor(0)
    {}
    ~ODMInit () {
	this->deactivate();
    }

    Interceptor::Status
    initialize
    (CORBA::ORB_ptr orb,
     const char *orbid,
     int& argc,
     char* argv[])
    {
	if (!MICOSODM::odmopts.parse (orb, argc, argv))
	    return Interceptor::INVOKE_ABORT;
    
	if (MICOSODM::odmopts["-ODMConfig"] == NULL)
	    return Interceptor::INVOKE_CONTINUE;
      
	MICOSODM::ODM_impl* odm = new MICOSODM::ODM_impl;
	MICOSODM::Factory_impl* factory = dynamic_cast<MICOSODM::Factory_impl*>
	    (odm->create());
	if (!factory->load_config_file((const char *)MICOSODM::odmopts["-ODMConfig"]))
	    return Interceptor::INVOKE_CONTINUE;
    
	string defkey = "/";
	MICOSODM::DomainMap::iterator it
	    = factory->domains_->get_domain_map().begin();
	MICOSODM::DomainMap::iterator it2
	    = factory->domains_->get_domain_map().find(defkey);
	for (; it != (factory->domains_->get_domain_map()).end(); it++) {
	    ((*it).second)->dm_ = dynamic_cast<MICOSODM::Manager_impl*>
		(factory->create());
	    if (it2 != (factory->domains_->get_domain_map()).end())  // found default
		(((*it).second)->dm_)->set_default_parent_odm
		    (((*it).second)->dm_);
	}
	
	orb->set_initial_reference ("ODM", odm);
	return Interceptor::INVOKE_CONTINUE;
    }
} InitODM;

void
MICOSODM::_init ()
{
    (void)InitODM;
}


// Options parser

CORBA::Boolean
MICOSODM::ODMOptions::parse
(CORBA::ORB_ptr orb,
 int& argc,
 char* argv[])
{
    MICOGetOpt::OptMap opts;
    opts["-ODMConfig"]  = "arg-expected";

    MICOGetOpt opt_parser (opts);
    CORBA::Boolean r = opt_parser.parse (orb->rcfile(), TRUE);
    if (!r)
	return FALSE;
    r = opt_parser.parse (argc, argv, TRUE);
    if (!r)
	return FALSE;
    const MICOGetOpt::OptVec &o = opt_parser.opts();
  
    for (MICOGetOpt::OptVec::const_iterator i = o.begin(); i != o.end(); ++i) {
	string arg = (*i).first;
	string val = (*i).second;
	options_[arg] = val;
    }
    return TRUE;
}

const char *
MICOSODM::ODMOptions::operator[](const char * opt)
{
    map<string,string,less<string> >::iterator it = options_.find (opt);
    if (it == options_.end()) {
	return NULL;
    }
    return (*it).second.c_str();
}

// ODMRecord structure

MICOSODM::ODMRecord::ODMRecord(const ODMRecord& _s)
{
    gran_ = ((ODMRecord&)_s).gran_;
    dnamelist_ = ((ODMRecord&)_s).dnamelist_;
    dm_ = ((ODMRecord&)_s).dm_;
    x509_ = ((ODMRecord&)_s).x509_;
    pol_ = ((ODMRecord&)_s).pol_;
}

MICOSODM::ODMRecord&
MICOSODM::ODMRecord::operator=(const ODMRecord& _s)
{
    gran_ = ((ODMRecord&)_s).gran_;
    dnamelist_ = ((ODMRecord&)_s).dnamelist_;
    dm_ = ((ODMRecord&)_s).dm_;
    x509_ = ((ODMRecord&)_s).x509_;
    pol_ = ((ODMRecord&)_s).pol_;
    return *this;
}


CORBA::Boolean
MICOSODM::AllDomainList::load_config(const char* conffile)
{
    ifstream input(conffile);
    if (!input.is_open())
	return FALSE;
    ODMConfig parser(&input, out_);
    parser.startParse(all_domains_);
    if (parser.geterror() != 0)
	return FALSE;
    all_domains_ = parser.getMap();
    return TRUE;
}

CORBA::Boolean
MICOSODM::AllDomainList::load_config()
{
    if (odmopts["-ODMConfig"] == NULL)
	return FALSE;
    return this->load_config((const char *)odmopts["-ODMConfig"]);
}

CORBA::Boolean
MICOSODM::AllDomainList::save_config(const char* conffile)
{
    // to do //
    return FALSE;
}

MICOSODM::DomainMap&
MICOSODM::AllDomainList::get_domain_map()
{
    return all_domains_;
}

// Manager

MICOSODM::Manager_impl::Manager_impl()
{
    parent_ = NULL;
    def_parent_ = NULL;
    domains_ = NULL;
    def_key_ = "/";
    // ###ras
    pol_ = PortableServer::TRANSIENT;
}

MICOSODM::Manager_impl::Manager_impl(AllDomainList * _domains)
{ //, PortableServer::POA_ptr _poa) {
    parent_ = NULL;
    def_parent_ = NULL;
    domains_ = _domains;
    def_key_ = "/";
    // ###ras
    pol_ = PortableServer::TRANSIENT;
}

MICOSODM::Manager_impl::~Manager_impl()
{
}

string
MICOSODM::Manager_impl::opaque_to_string(const Security::Opaque& key)
{
    string fullkey;
    for (CORBA::ULong i = 0; i < key.length(); i++)
	fullkey += key[i];
    return fullkey;
}

void
MICOSODM::Manager_impl::set_domain_name_key
(ODMGranularity granularity,
 const Security::Opaque& _key,
 const SecurityDomain::NameList& domain_name_list)
{
    string fullkey = this->opaque_to_string(_key);
    CORBA::ULong i = fullkey.find(']', 0);
    assert(i > 0); // error, no x509dn stuff
  
    i++;
    string dn = fullkey.substr(0, i);
    string shortkey = fullkey.substr(i + 1);

    ODMRecord* rec = find_record(shortkey, this);
    if (!rec) { // not found
	assert(domains_ != NULL);
	rec = new ODMRecord;
    }
    rec->dm_ = this;
    rec->gran_ = granularity;
    rec->x509_ = dn;
    // change it for multi-root
    //rec->dnamelist = domainNameList;
    int l = (rec->dnamelist_).length();
    for (i = 0; i < domain_name_list.length(); i++) {
	(rec->dnamelist_).length(l + 1);
	(rec->dnamelist_)[l] = domain_name_list[i];
	l++;
    }
    rec->pol_ = pol_;
    (domains_->get_domain_map())[shortkey] = rec;
}

SecurityDomain::NameList*
MICOSODM::Manager_impl::get_domain_names(const Security::Opaque& _key)
{
    string fullkey = this->opaque_to_string(_key);
    int i = fullkey.find(']', 0);
    assert(i > 0); // error, no x509dn stuff

    i++;
    string dn = fullkey.substr(0, i);
    string shortkey = fullkey.substr(i + 1);
  
    SecurityDomain::NameList* nl = new SecurityDomain::NameList;

    ODMRecord* rec = find_record(shortkey, this);
  
    if (rec != NULL && rec->x509_ == dn) { // found it
	*nl = rec->dnamelist_;
	return nl;
    }
    if (def_parent_) // if we have default parent
	rec = find_record(def_key_, def_parent_); // try default
    else if (parent_) // we don't have default parent, may be parent has
	rec = find_record(def_key_, parent_);
  
    if (rec != NULL && rec->x509_ == dn)
	*nl = rec->dnamelist_;

    return nl;
}

void
MICOSODM::Manager_impl::remove_domain_names(const Security::Opaque& key)
{
    string fullkey = this->opaque_to_string(key);
    int i = fullkey.find(']', 0);
    assert(i > 0); // error, no x509dn stuff

    remove_record(fullkey, this);
}

void
MICOSODM::Manager_impl::set_parent_odm_key
(const Security::Opaque& key,
 ObjectDomainMapping::Manager_ptr odm)
{
    if (!CORBA::is_nil(parent_))
	CORBA::release(parent_);
    parent_ = dynamic_cast<Manager_impl*>
	(ObjectDomainMapping::Manager::_duplicate(odm)); // don't know for what key here, it's unusable.
}

void
MICOSODM::Manager_impl::set_default_parent_odm
(ObjectDomainMapping::Manager_ptr odm)
{
    if (!CORBA::is_nil(def_parent_))
	CORBA::release(def_parent_);
    def_parent_ = dynamic_cast<Manager_impl*>
	(ObjectDomainMapping::Manager::_duplicate(odm));
}

void
MICOSODM::Manager_impl::set_default_name_key
(const SecurityDomain::NameList& domain_name_list)
{
    ODMRecord* rec = find_record(def_key_, this);
    assert(rec != NULL); // no default ODM
    rec->dnamelist_ = domain_name_list;
}

MICOSODM::ODMRecord*
MICOSODM::find_record(string& key, MICOSODM::Manager_impl* odm)
{
    MICOSODM::DomainMap::iterator it = (odm->domains_->get_domain_map()).find(key);
    if (it == (odm->domains_->get_domain_map()).end()) { // continue search
	if (odm->parent_)
	    return find_record(key, odm->parent_);
	else
	    return NULL;
    }
    return (*it).second;
}

void
MICOSODM::remove_record(string& key, MICOSODM::Manager_impl* odm)
{
    int i = key.find(']', 0);
    i++;
    string dn = key.substr(0, i);
    string shortkey = key.substr(i + 1);

    MICOSODM::DomainMap::iterator it = (odm->domains_->get_domain_map()).find(shortkey);
    if (it == (odm->domains_->get_domain_map()).end()) { // continue search
	if (odm->parent_)
	    remove_record(key, odm->parent_);
	else
	    return;
    }

    ODMRecord* rec = (*it).second;
    if (rec->x509_ == dn)
	(odm->domains_->get_domain_map()).erase(it);
}


void
MICOSODM::set_lspolicy
(MICOSODM::Manager_impl* odm,
 PortableServer::LifespanPolicyValue p)
{
    odm->pol_ = p;
}

PortableServer::LifespanPolicyValue
MICOSODM::get_lspolicy(MICOSODM::Manager_impl* odm)
{
    return odm->pol_;
}

// Factory

MICOSODM::Factory_impl::Factory_impl()
{
    domains_ = new MICOSODM::AllDomainList();
}


MICOSODM::Factory_impl::~Factory_impl()
{
    delete domains_;
}

ObjectDomainMapping::Manager_ptr
MICOSODM::Factory_impl::create()
{
    return new MICOSODM::Manager_impl(domains_);
}

CORBA::Boolean
MICOSODM::Factory_impl::load_config_file(const char* cf)
{
    CORBA::Boolean ret;
    if (!cf)
	ret = domains_->load_config();
    else
	ret = domains_->load_config(cf);
  
    if (!ret)
	return ret;
		
    string defkey = "/";
    MICOSODM::DomainMap::iterator it = (domains_->get_domain_map()).begin();
    MICOSODM::DomainMap::iterator it2 = (domains_->get_domain_map()).find(defkey);
    for (; it != (domains_->get_domain_map()).end(); it++) {
	((*it).second)->dm_ = (MICOSODM::Manager_impl *)create();
	if (it2 != (domains_->get_domain_map()).end()) // found default
	    (((*it).second)->dm_)->set_default_parent_odm(((*it).second)->dm_);
    }
    return ret;
}


CORBA::Boolean
MICOSODM::Factory_impl::save_config_file(const char* cf)
{
    ofstream out(cf);
    if (!out.is_open())
	return FALSE;
    for (DomainMap::iterator it = (domains_->get_domain_map()).begin(); it != (domains_->get_domain_map()).end(); it++) {
	ODMRecord * rec = (*it).second;
	//		if (rec->pol == PortableServer::TRANSIENT) continue;
	SecurityDomain::NameList nl = rec->dnamelist_;
	int len = nl.length();
	for (int i = 0; i < len; i++) {
	    out << rec->x509_ << "/" ;
	    if ((*it).first != "/")
		out << (*it).first;
	    if (rec->gran_ == (CORBA::UShort)2)
		out << "/";
	    out << " ";
	    SecurityDomain::Name nm = nl[i];
	    for (CORBA::ULong j = 0; j < nm.length(); j++) {
		SecurityDomain::NameComponent nc = nm[j];
		out << "/" << nc.id;
	    }
	    out << endl;
	}
    }
  
    out << flush;
  
    return TRUE;
}


