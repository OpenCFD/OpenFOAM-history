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

#include <mico/impl.h>
#include <mico/template_impl.h>
#include <mico/util.h>
#undef yyFlexLexer
#define yyFlexLexer acFlexLexer
#include <FlexLexer.h>
#include <mico/security/DomainManager_impl.h>
#include <mico/security/SecurityAdmin_impl.h>
#include <mico/security/AccessConfig.h>
#undef yyFlexLexer
#define yyFlexLexer auFlexLexer
#include <FlexLexer.h>
#include <mico/security/AuditConfig.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else
#include <fstream.h>
#endif


using namespace std;

namespace MICOSDM {
    SDMOptions sdmopts; // ConfigFilename = sdmopts[-SDMConfig]
}

static class SDMInit
    : public Interceptor::InitInterceptor
{
public:
    SDMInit()
	: Interceptor::InitInterceptor(0)
    {}
  
    ~SDMInit()
    { this->deactivate (); }

    Interceptor::Status initialize(CORBA::ORB_ptr orb, const char *orbid,
				   int &argc, char *argv[])
    {
	if (!MICOSDM::sdmopts.parse (orb, argc, argv))
	    return Interceptor::INVOKE_ABORT;
      
	if (MICOSDM::sdmopts["-AccessConfig"] == NULL && MICOSDM::sdmopts["-AuditConfig"] == NULL)
	    return Interceptor::INVOKE_CONTINUE;
    
	MICOSDM::DomainManagerFactory_impl * dmfactory = new MICOSDM::DomainManagerFactory_impl;
	CORBA::Boolean ac = dmfactory->load_config_file((const char *)MICOSDM::sdmopts["-AccessConfig"], "Access");
	CORBA::Boolean ad = dmfactory->load_config_file((const char *)MICOSDM::sdmopts["-AuditConfig"], "Audit");

	if (ac || ad)
	    orb->set_initial_reference ("DomainManagerFactory", dmfactory);
    
	return Interceptor::INVOKE_CONTINUE;
    }
} InitSDM;

void
MICOSDM::_init ()
{
    (void)InitSDM;
}


// Options parser

CORBA::Boolean
MICOSDM::SDMOptions::parse (CORBA::ORB_ptr orb, int& argc, char* argv[])
{
    MICOGetOpt::OptMap opts;
    opts["-AccessConfig"]  = "arg-expected";
    opts["-AuditConfig"] = "arg-expected";
  
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
MICOSDM::SDMOptions::operator[](const char* opt)
{
    map<string,string,less<string> >::iterator it = options_.find (opt);
    if (it == options_.end()) {
	return NULL;
    }
    return (*it).second.c_str();
}

//
// DomainManagerFactory
//

MICOSDM::DomainManagerFactory_impl::DomainManagerFactory_impl()
{
    //root = new MICOSDM::DomainAuthorityAdmin_impl;
}

SecurityDomain::DomainManagerAdmin_ptr
MICOSDM::DomainManagerFactory_impl::get_root_domain_manager(const char* type)
{
    //cerr << "get_root_domain_manager(" << type << ");" << endl;
    SecurityDomain::Name_var tmp;
  
    for (CORBA::ULong i = 0; i < roots_.length(); i++) {
	//cerr << "i: " << i << endl;
	tmp = roots_[i]->get_name();
	if (strcmp(tmp[(CORBA::ULong)0].kind, type) == 0) {
	    //cerr << "-> roots_[" << i << "] - ref:" << roots_[i]->_refcnt() << endl;
	    return SecurityDomain::DomainManagerAdmin::_duplicate(roots_[i]);
	}
    }
    return SecurityDomain::DomainManagerAdmin::_nil();
}

void
MICOSDM::DomainManagerFactory_impl::add_root_domain_manager(const char* type)
{
    SecurityDomain::Name_var tmp;

    for (CORBA::ULong i = 0; i < roots_.length(); i++) { // check for existent name
	tmp = roots_[i]->get_name();
	if (strcmp(tmp[(CORBA::ULong)0].id, type) == 0 && strcmp(tmp[(CORBA::ULong)0].kind, type) == 0) {
	    //cerr << "add_root_domain_manager(" << type << "); -- skip" << endl;
	    return;
	}
    }
  
    MICOSDM::DomainAuthorityAdmin_impl* dm = new MICOSDM::DomainAuthorityAdmin_impl;
    SecurityDomain::Name rootname;
    rootname.length(1);
    rootname[0].id = CORBA::string_dup(type);
    rootname[0].kind = CORBA::string_dup(type);
    dm->set_name(rootname);
    int l = roots_.length();
    roots_.length(l + 1);
    roots_[l] = dm;
    //cerr << "add_root_domain_manager(" << type << "); -- position: " << l << endl;
}

SecurityDomain::DomainManagerAdmin_ptr
MICOSDM::DomainManagerFactory_impl::create_domain_manager()
{
    return new MICOSDM::DomainAuthorityAdmin_impl;
}

void
MICOSDM::DomainManagerFactory_impl::release_domain_manager
(SecurityDomain::DomainManagerAdmin_ptr domain_manager)
{
    SecurityDomain::DomainManagerAdminList* cn = domain_manager->get_child_domain_managers();
    SecurityDomain::DomainManagerAdminList* pt = domain_manager->get_parent_domain_managers();
    SecurityDomain::DomainAuthorityAdmin_ptr adm = SecurityDomain::DomainAuthorityAdmin::_narrow(domain_manager);
  
    if (cn->length() == 0 && pt->length() == 0 && !adm->is_root())
	delete domain_manager;
}

CORBA::Boolean
MICOSDM::DomainManagerFactory_impl::load_config_file
(const char* filename,
 const char* type)
{
    if (filename != NULL) {
	ifstream input(filename);
	if (!input.is_open())
	    return FALSE;
	if (strcmp(type, "Access") == 0) {
	    AccessConfig acparser(&input);
	    acparser.startParse(this);
	    if (acparser.geterror() != 0)
		return FALSE;
	    return TRUE;
	}
	else if (strcmp(type, "Audit") == 0) {
	    AuditConfig adparser(&input);
	    adparser.startParse(this);
	    if (adparser.geterror() != 0)
		return FALSE;
	    return TRUE;
	}
    }
    return FALSE;
}

//
// DomainManagerAdmin
//
MICOSDM::DomainManagerAdmin_impl::~DomainManagerAdmin_impl()
{
    /*  if (policies[0])       
	delete policies[0];
	if (policies[1])
	delete policies[1];
	if (policies[2])
	delete policies[3];
    */
}

MICOSDM::DomainManagerAdmin_impl::DomainManagerAdmin_impl()
{
    // some initial policies ??
    policies_.length(3);
    // for new mapping
    // policies[0] = new MICOSA::DomainAccessPolicy_impl;
    policies_[0] = new MICOSA::ObjectAccessPolicy_impl;
    policies_[1] = new MICOSA::AuditTargetPolicy_impl;
    policies_[2] = new MICOSA::AuditClientPolicy_impl;
}

CORBA::Policy_ptr
MICOSDM::DomainManagerAdmin_impl::get_domain_policy
(CORBA::PolicyType policy_type)
{
    for (CORBA::ULong i = 0; i < policies_.length(); ++i) {
	if (policies_[i]->policy_type() == policy_type)
	    return CORBA::Policy::_duplicate(policies_[i]);
    }
    mico_throw (CORBA::INV_POLICY());
    // never reached
    return CORBA::Policy::_nil();
}

void
MICOSDM::DomainManagerAdmin_impl::set_domain_policy(CORBA::Policy_ptr policy)
{
    CORBA::PolicyType policy_type = policy->policy_type();
    for (CORBA::ULong i = 0; i < policies_.length(); ++i) {
	if (policies_[i]->policy_type() == policy_type) {
	    policies_[i] = CORBA::Policy::_duplicate(policy);
	    return;
	}
    }
    policies_.length(policies_.length() + 1);
    policies_[policies_.length() - 1] = CORBA::Policy::_duplicate(policy);
}

void
MICOSDM::DomainManagerAdmin_impl::delete_domain_policy
(CORBA::PolicyType policy_type)
{
    int len = policies_.length();
    int where = -1;
    for (int i = 0; i < len; i++) {
	if (policies_[i]->policy_type() == policy_type) {
	    where = i;
	    break;
	}
    }
    if (where < 0)
	return;
    for (int i = where; i < len - 1; i++) {
	policies_[i] = policies_[i + 1];
    }
    policies_.length(len - 1);
}

SecurityDomain::Name*
MICOSDM::DomainManagerAdmin_impl::get_name()
{
    return new SecurityDomain::Name(name_);
}

void
MICOSDM::DomainManagerAdmin_impl::set_name(const SecurityDomain::Name& dname)
{
    name_ = dname;
}

SecurityDomain::DomainManagerAdminList*
MICOSDM::DomainManagerAdmin_impl::get_parent_domain_managers()
{
    return new SecurityDomain::DomainManagerAdminList(parents_);
}

void
MICOSDM::DomainManagerAdmin_impl::set_parent_domain_managers
(const SecurityDomain::DomainManagerAdminList& managers)
{
    parents_ = managers;
}

SecurityDomain::DomainManagerAdminList*
MICOSDM::DomainManagerAdmin_impl::get_child_domain_managers()
{
    return new SecurityDomain::DomainManagerAdminList(children_);
}


//
// DomainAuthority
//

MICOSDM::DomainAuthority_impl::~DomainAuthority_impl()
{
}

MICOSDM::DomainAuthority_impl::DomainAuthority_impl()
{
}

CORBA::Boolean
MICOSDM::DomainAuthority_impl::is_root()
{
    if (name_.length() != 1)
	return FALSE;
    if (strcmp(name_[0].id, name_[0].kind) == 0 && parents_.length() < 1)
	return TRUE; // new submission requires id == "" for root domain, just for now
    return FALSE;
}


CORBA::Boolean
MICOSDM::DomainAuthority_impl::exists()
{
    // may be we don't need it at all
    return FALSE;
}

SecurityDomain::PolicyCombinator
MICOSDM::DomainAuthority_impl::get_policy_combinator
(CORBA::PolicyType policy_type)
{
    for (CORBA::ULong i = 0; i < combinators_.length(); i++) {
	if (combinators_[i].policy_type == policy_type)
	    return combinators_[i].combinator;
    }
    if (this->is_root())
	return SecurityDomain::Union; // doesn't matter what if root
    mico_throw (CORBA::INV_POLICY());
    // never reached
    return SecurityDomain::Union;
}

SecurityDomain::PolicyCombinator
MICOSDM::DomainAuthority_impl::get_policy_combinator_for_multi_parents
(CORBA::PolicyType policy_type,
 SecurityDomain::DomainManagerAdmin_ptr parent)
{
    // may be we don't need it at all
    return SecurityDomain::Union;
}

SecurityDomain::NameIterator_ptr
MICOSDM::DomainAuthority_impl::get_child_domain_names()
{
    // may be we don't need it at all
    return SecurityDomain::NameIterator::_nil();
}


SecurityDomain::Name*
MICOSDM::DomainAuthority_impl::get_domain_name
(SecurityDomain::DomainManagerAdmin_ptr domain_manager)
{ 
    // short name
    SecurityDomain::Name_var nm = domain_manager->get_name();
    SecurityDomain::Name_var tmp;
    for (CORBA::ULong i = 0; i < children_.length(); i++) {
	tmp = children_[i]->get_name();
	if (tmp->length() == 0)
	    continue;
	if (strcmp(tmp[(CORBA::ULong)0].id, nm[(CORBA::ULong)0].id) == 0
	    && strcmp(tmp[(CORBA::ULong)0].kind, nm[(CORBA::ULong)0].kind) == 0)
	    return new SecurityDomain::Name(tmp);
    }
    return NULL;
}

SecurityDomain::DomainManagerAdmin_ptr
MICOSDM::DomainAuthority_impl::get_domain_manager
(const SecurityDomain::Name& domain_name)
{
    // full relative name
    CORBA::ULong i;
    SecurityDomain::DomainManagerAdmin_ptr dm;
    SecurityDomain::DomainAuthority_var adm;
    SecurityDomain::Name nm;
    SecurityDomain::Name_var tmp;
  
    if (domain_name.length() == 0)
	return SecurityDomain::DomainManagerAdmin::_duplicate(this); // root

    nm.length(domain_name.length() - 1);
    for (i = 0; i < nm.length(); i++) {
	nm[i] = domain_name[i + 1];
    }
    for (i = 0; i < children_.length(); i++) {
	dm = children_[i];
	tmp = children_[i]->get_name();
	if (strcmp((*tmp)[0].id, domain_name[0].id) == 0 && strcmp((*tmp)[0].kind, domain_name[0].kind) == 0) {
	    if (nm.length() == 0) // found it, last name in domain_name
		return SecurityDomain::DomainManagerAdmin::_duplicate(dm);
	    else {
		adm = SecurityDomain::DomainAuthority::_narrow(dm);
		return adm->get_domain_manager(nm); // continue search 1 level down
	    }
	}
    }
    // not found such a name
    return SecurityDomain::DomainManagerAdmin::_nil();
}

//
// DomainAuthorityAdmin
//

MICOSDM::DomainAuthorityAdmin_impl::~DomainAuthorityAdmin_impl()
{
    // TODO
}

MICOSDM::DomainAuthorityAdmin_impl::DomainAuthorityAdmin_impl()
{
}

void
MICOSDM::DomainAuthorityAdmin_impl::add_domain_manager
(SecurityDomain::DomainManagerAdmin_ptr domain_manager,
 const SecurityDomain::Name& domain_name)
{
    //short name
    SecurityDomain::Name_var tmp;

    for (CORBA::ULong i = 0; i < children_.length(); i++) {
	// check for existent name
	tmp = children_[i]->get_name();
	if (strcmp(tmp[(CORBA::ULong)0].id, domain_name[0].id) == 0
	    && strcmp(tmp[(CORBA::ULong)0].kind, domain_name[0].kind) == 0) {
	    return;
	}
    }
    domain_manager->set_name(domain_name);
    CORBA::ULong l = children_.length();
    children_.length(l + 1);
    children_[l] = SecurityDomain::DomainManagerAdmin::_duplicate(domain_manager);
  
    SecurityDomain::DomainManagerAdminList_var parents = domain_manager->get_parent_domain_managers();
    l = parents->length();
    parents->length(l + 1);
    parents[l] = SecurityDomain::DomainManagerAdmin::_duplicate(this);
    domain_manager->set_parent_domain_managers(parents);
}

void
MICOSDM::DomainAuthorityAdmin_impl::remove_domain_manager
(SecurityDomain::DomainManagerAdmin_ptr child)
{
    // short name
    SecurityDomain::Name_var cname = child->get_name();
    SecurityDomain::Name_var tmp;
    int l = children_.length();
  
    for (int i = 0; i < l; i++) { // check for existent name
	tmp = children_[i]->get_name();
	if (strcmp(tmp[(CORBA::ULong)0].id, cname[(CORBA::ULong)0].id) == 0
	    && strcmp(tmp[(CORBA::ULong)0].kind, cname[(CORBA::ULong)0].kind) == 0) {
	    for (int j = i; j < l - 1; j++) {
		children_[j] = children_[j + 1];
	    }
	    children_.length(l - 1);
	    break;
	}
    }
}

void
MICOSDM::DomainAuthorityAdmin_impl::set_policy_combinator
(CORBA::PolicyType policy_type,
 SecurityDomain::PolicyCombinator combinator)
{
    int len = combinators_.length();
  
    for (CORBA::ULong i = 0; i < combinators_.length(); i++) {
	if (combinators_[i].policy_type == policy_type) {
	    combinators_[i].combinator = combinator;
	    return;
	}
    }
    combinators_.length(len + 1);
    SecurityDomain::CombinatorInfo cinf;
    cinf.policy_type = policy_type;
    cinf.combinator = combinator;
    combinators_[len] = cinf;
}

void
MICOSDM::DomainAuthorityAdmin_impl::set_policy_combinator_for_multi_parents
(CORBA::PolicyType policy_type,
 SecurityDomain::PolicyCombinator combinator,
 SecurityDomain::DomainManagerAdmin_ptr parent)
{
    // not need it
}

//
// NameIterator
//
MICOSDM::NameIterator_impl::~NameIterator_impl()
{
    // not need it
}

CORBA::Boolean
MICOSDM::NameIterator_impl::next_one(SecurityDomain::Name*& domain_name)
{
    // not need it
    return FALSE;
}

void
MICOSDM::NameIterator_impl::destroy()
{
    // not need it
}

//
// Name_ext
//
char*
MICOSDM::NameExt_impl::to_string(const SecurityDomain::Name& domain_name)
{
    string res = "";
  
    for (CORBA::ULong i = 0; i < domain_name.length(); i++) {
	SecurityDomain::NameComponent nc = domain_name[i];
	res += '/';
	char* str = nc.id;
	while (*str) {
	    if (*str == '/' || *str == '\\' || *str == '.') {
		res += '\\';
	    }
	    res += *str++;
	}
	if (*nc.kind) {
	    res += '.';
	    res += nc.kind;
	}
    }
    return CORBA::string_dup(res.c_str());
}

SecurityDomain::Name*
MICOSDM::NameExt_impl::to_name(const char* url_name)
{
    // probably will add find('#') first?
    string str = CORBA::string_dup(url_name);
    SecurityDomain::Name* name = new SecurityDomain::Name;
  
    int len = str.length();
    int i = 0, j;
    string part;
    for (; (j = str.find('/', i)) >= 0; j++, i += j - i) {
	part = str.substr(i, j - i);
	if (part.length() == 0)
	    continue;
	const char * ptr = part.c_str();
	parse_name(ptr);
	CORBA::ULong l = name->length();
	name->length(l + 1);
	(*name)[l] = nc_;
    }
    if (i < len) { // there was no '/' at the end
	part = str.substr(i, len - i);
	const char * ptr = part.c_str();
	parse_name(ptr);
	CORBA::ULong l = name->length();
	name->length(l + 1);
	(*name)[l] = nc_;
    }
    return name;
}

void
MICOSDM::NameExt_impl::parse_name(const char* name)
{
    string id = "";
    string kind = "";
    while (*name) {
	if (*name != '\\' && *(name + 1) == '.') {
	    id += *name;
	    name += 2;
	    kind += name;
	    break;
	}
	if (*name == '\\') continue;
	id += *name++;
    }
    nc_.id = CORBA::string_dup(id.c_str());
    nc_.kind = CORBA::string_dup(kind.c_str());
}
