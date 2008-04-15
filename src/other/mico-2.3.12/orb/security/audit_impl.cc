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


#include <CORBA.h>
#include <mico/impl.h>
#include <mico/security/audit_impl.h>
#include <syslog.h>
#include <time.h>
#include <mico/security/AuditIntercept.h>
#undef yyFlexLexer
#define yyFlexLexer auFlexLexer
#include <FlexLexer.h>
#include <mico/security/AuditConfig.h>
#include <mico/security/odm_impl.h>

#define NO_INFO "no_info"


using namespace std;

static MICOSL2::AuditChannelList S_channel_list;

MICOSL2::Archive::Archive()
{
    //name = 0;
}

MICOSL2::Archive::~Archive()
{
}

CORBA::Boolean
MICOSL2::Archive::write
(const Security::AuditEventType& event_type,
 const SecurityLevel2::CredentialsList& creds,
 const Security::UtcT& time,
 const Security::SelectorValueList& descriptors,
 const Security::Opaque& event_specific_data)
{
    return FALSE;
}


CORBA::Boolean
MICOSL2::Archive::create(const char* archive_name)
{
    return FALSE;
}


string
MICOSL2::Archive::make_output_string
(const Security::AuditEventType& event_type,
 const Security::SelectorValueList& descriptors)
{
    string res_str("");
    time_t t = time(0);
    char* time_str = ctime(&t);
    res_str += time_str;
  
    char str[256];
    char* event_name;
    if (event_type.event_type == Security::AuditPrincipalAuth)
	event_name = "[AuditPrincipalAuth]";
    else if (event_type.event_type == Security::AuditSessionAuth)
	event_name = "[AuditSessionAuth]";
    else if (event_type.event_type == Security::AuditAuthorization)
	event_name = "[AuditAuthorization]";
    else if (event_type.event_type == Security::AuditAuthorization)
	event_name = "[AuditAuthorization]";
    else if (event_type.event_type == Security::AuditInvocation)
	event_name = "[AuditInvocation]";
    else if (event_type.event_type == Security::AuditSecEnvChange)
	event_name = "[AuditSecEnvChange]";
    else if (event_type.event_type == Security::AuditObjectCreation)
	event_name = "[AuditObjectCreation]";
    else if (event_type.event_type == Security::AuditObjectDestruction)
	event_name = "[AuditObjectDestruction]";
    else if (event_type.event_type == Security::AuditNonRepudiation)
	event_name = "[AuditNonRepudiation]";
    sprintf(str, " Event=%s ", event_name);
    res_str += str;
    for (CORBA::ULong i = 0; i < descriptors.length(); i++) {
	if (descriptors[i].selector == Security::InterfaceName) {
	    const char* s;
	    descriptors[i].value >>= s;
	    if (strcmp(s,"") == 0) s = NO_INFO;
	    sprintf(str, "InterfaceName=[%s] ",  s);
	} else if (descriptors[i].selector == Security::ObjectRef) {
	    const char* s;
	    descriptors[i].value >>= s;
	    if (strcmp(s,"") == 0) s = NO_INFO;
	    sprintf(str, "ObjectRef=[%s] ", s);
	} else if (descriptors[i].selector == Security::Operation) {
	    const char* s;
	    descriptors[i].value >>= s;
	    if (strcmp(s,"") == 0) s = NO_INFO;
	    sprintf(str, "Operation=[%s] ", s);
	} else if (descriptors[i].selector == Security::Initiator) {
	    const char* s;
	    descriptors[i].value >>= s;
	    if (strcmp(s,"") == 0) s = NO_INFO;
	    sprintf(str, "Initiator=[%s] ", s);
	} else if (descriptors[i].selector == Security::SuccessFailure) {
	    short success;
	    const char* s;
	    descriptors[i].value >>= success;
	    if ((int)success == -1)
		s = NO_INFO; else
		    if ((int)success == 0)
			s = "failure";
		    else
			s = "success";
	    sprintf(str, "SuccessFailure=[%s] ", s);
	} else
	    continue;
	res_str += str;
    }
    return res_str;
}


MICOSL2::FileArchive::FileArchive()
{
}

MICOSL2::FileArchive::~FileArchive()
{
    if (file_) {
	fclose(file_);
	delete file_;
    }
}

CORBA::Boolean
MICOSL2::FileArchive::create(const char* archive_name)
{
    name_ = CORBA::string_dup(archive_name);
    file_ = fopen(name_, "a+");
  
    if (file_ == 0)
	return FALSE;
    return TRUE;
}

CORBA::Boolean
MICOSL2::FileArchive::write
(const Security::AuditEventType& event_type,
 const SecurityLevel2::CredentialsList& creds,
 const Security::UtcT& utct_time,
 const Security::SelectorValueList& descriptors,
 const Security::Opaque& event_specific_data)
{
    string str= this->make_output_string(event_type, descriptors);
    str += "clientserver=[";
    if (event_specific_data.length() != 0) {
	if (event_specific_data[0] == 4)
	    str += "server]";
	if (event_specific_data[0] == 0)
	    str += "client]";
    }
    else str += "no_info]";
  
    fputs(str.c_str(), file_);
    fputs("\n", file_);
    fflush(file_);
    return TRUE;
}

#ifdef HAVE_PGSQL

MICOSL2::DBArchive::DBArchive()
{
    conn_ = 0;
}

MICOSL2::DBArchive::~DBArchive()
{
    if (conn_)
	PQfinish(conn_);
}

CORBA::Boolean
MICOSL2::DBArchive::create(const char* archive_name)
{
    name_ = CORBA::string_dup(archive_name);
    table_name_ = "audittbl";
  
    char conn_str[512];
    char* ptr;
    conn_str[0] = 0;
    for (ptr = strtok(name_, ","); ptr != NULL; ptr = strtok(NULL, ",")) {
	strcat(conn_str, ptr);
	strcat(conn_str, " ");
    }
  
    conn_ = PQconnectdb(conn_str);  //connect to the database
    if (PQstatus(conn_) == CONNECTION_BAD) { //did the database connection fail?
	PQfinish(conn_);
	conn_ = 0;
	return FALSE;
    }
  
    char query_string[512];
    sprintf(query_string, "%s", "select relname from pg_class where (relname = 'audittbl') and (relkind='r')");
    PGresult *res;
    res = PQexec(conn_, query_string); /* send the query */
    if (PQresultStatus(res) != PGRES_TUPLES_OK) { /* did the query fail? */
	PQclear(res);
	PQfinish(conn_);
	conn_ = 0;
	return FALSE;
    }
    if (PQntuples(res)>0)
	return TRUE;

    string t_query = "CREATE TABLE audittbl (";
    t_query += "time timestamp,";
    t_query += "interfacename varchar(255),";
    t_query += "objectref varchar(255),";
    t_query += "operation varchar(255),";
    t_query += "initiator varchar(255),";
    t_query += "successfailure varchar(10),";
    t_query += "clientserver varchar(10),";
    t_query += "eventtype varchar(80),";
    t_query += "dayofweek varchar(10)";
    t_query += ")";

    sprintf(query_string, "%s", t_query.c_str());

    res = PQexec(conn_, query_string); /* send the query */
    if (PQresultStatus(res) != PGRES_COMMAND_OK) { // did the query fail?
	PQclear(res);
	PQfinish(conn_);
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
MICOSL2::DBArchive::write
(const Security::AuditEventType& event_type,
 const SecurityLevel2::CredentialsList& creds,
 const Security::UtcT& utct_time,
 const Security::SelectorValueList& descriptors,
 const Security::Opaque& event_specific_data)
{
    char str[256];
    string fields_str("");
    string values_str("");
    sprintf(str, "INSERT INTO %s (Time, EventType", table_name_);
    fields_str+= str;
    sprintf(str, "VALUES('now'");
    values_str+= str;
  
    if (event_type.event_type == Security::AuditPrincipalAuth)
	values_str += ",'AuditPrincipalAuth'";
    else if (event_type.event_type == Security::AuditSessionAuth)
	values_str += ",'AuditSessionAuth'";
    else if (event_type.event_type == Security::AuditAuthorization)
	values_str += ",'AuditAuthorization'";
    else if (event_type.event_type == Security::AuditAuthorization)
	values_str += ",'AuditAuthorization'";
    else if (event_type.event_type == Security::AuditInvocation)
	values_str += ",'AuditInvocation'";
    else if (event_type.event_type == Security::AuditSecEnvChange)
	values_str += ",'AuditSecEnvChange'";
    else if (event_type.event_type == Security::AuditObjectCreation)
	values_str += ",'AuditObjectCreation'";
    else if (event_type.event_type == Security::AuditObjectDestruction)
	values_str += ",'AuditObjectDestruction'";
    else if (event_type.event_type == Security::AuditNonRepudiation)
	values_str += ",'AuditNonRepudiation'";
    else
	values_str += NO_INFO;
    const char* test1 = fields_str.c_str();
    const char* test2 = values_str.c_str();
  
    for (CORBA::ULong i = 0; i < descriptors.length(); i++) {
	if (descriptors[i].selector == Security::InterfaceName) {
	    const char* s;
	    descriptors[i].value >>= s;
	    if (strcmp(s,"") == 0)
		s = NO_INFO;
	    fields_str += ",InterfaceName";
	    sprintf(str, ",'%s'", s);
	    values_str+= str;
	    test1 = fields_str.c_str();
	    test2 = values_str.c_str();
	} else if (descriptors[i].selector == Security::ObjectRef) {
	    const char* s;
	    descriptors[i].value >>= s;
	    if (strcmp(s,"") == 0) s = NO_INFO;
	    fields_str += ",ObjectRef";
	    sprintf(str, ",'%s'", s);
	    values_str+= str;
	    test1 = fields_str.c_str();
	    test2 = values_str.c_str();
	} else if (descriptors[i].selector == Security::Operation) {
	    const char* s;
	    descriptors[i].value >>= s;
	    if (strcmp(s,"") == 0)
		s = NO_INFO;
	    fields_str += ",Operation";
	    sprintf(str, ",'%s'", s);
	    values_str+= str;
	    test1 = fields_str.c_str();
	    test2 = values_str.c_str();
	} else if (descriptors[i].selector == Security::Initiator) {
	    const char* s;
	    descriptors[i].value >>= s;
	    if (strcmp(s,"") == 0) s = NO_INFO;
	    fields_str += ",Initiator";
	    sprintf(str, ",'%s'", s);
	    values_str+= str;
	    test1 = fields_str.c_str();
	    test2 = values_str.c_str();
	} else if (descriptors[i].selector == Security::SuccessFailure) {
	    short success;
	    char* s;
	    descriptors[i].value >>= success;
	    if ((int)success == -1)
		s = NO_INFO;
	    else if ((int)success == 0)
		s = "failure";
	    else
		s = "success";
	    fields_str += ",SuccessFailure";
	    sprintf(str, ",'%s'", s);
	    values_str+= str;
	    test1 = fields_str.c_str();
	    test2 = values_str.c_str();
	} else if (descriptors[i].selector == Security::DayOfWeek) {
	    short day_of_w;
	    char* s;
	    descriptors[i].value >>= day_of_w;
	    switch ((int)day_of_w)
		{
		case 0: s = "Sunday"; break;
		case 1: s = "Monday"; break;
		case 2: s = "Tuesday"; break;
		case 3: s = "Wednesday"; break;
		case 4: s = "Thursday"; break;
		case 5: s = "Friday"; break;
		case 6: s = "Saturday"; break;
		default: s = NO_INFO;
		}
	    fields_str += ",dayofweek";
	    sprintf(str, ",'%s'", s);
	    values_str+= str;
	}
	continue;
    }
    fields_str += ",clientserver)";
  
    char* c_s = NO_INFO;
    if (event_specific_data.length() != 0) {
	if (event_specific_data[0] == 4)
	    c_s = "server";
	if (event_specific_data[0] == 0)
	    c_s = "client";
    }
    sprintf(str, ",'%s') ", c_s);
    values_str+= str;
  
    test1 = fields_str.c_str();
    test2 = values_str.c_str();
    string query_string("");
    query_string = fields_str + values_str;
  
    //const char* test = query_string.c_str();
  
    PGresult* res;
    res = PQexec(conn_, query_string.c_str()); // send the query
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {// did the query fail?
	PQclear(res);
	return FALSE;
    }
  
    PQclear(res);
    return TRUE;
}

#endif

MICOSL2::ConsoleArchive::ConsoleArchive(int priority = 0)
{
    priority_ = priority;
}

MICOSL2::ConsoleArchive::~ConsoleArchive()
{
}

CORBA::Boolean
MICOSL2::ConsoleArchive::create(const char* archive_name = 0)
{
    return TRUE;
}

CORBA::Boolean
MICOSL2::ConsoleArchive::write
(const Security::AuditEventType& event_type,
 const SecurityLevel2::CredentialsList& creds,
 const Security::UtcT& utct_time,
 const Security::SelectorValueList& descriptors,
 const Security::Opaque& event_specific_data)
{
    int priority = (priority_ == 0) ? LOG_CONS|LOG_USER|LOG_INFO : priority_;
  
    string str = this->make_output_string(event_type, descriptors);
    str += "clientserver=[";
    if (event_specific_data.length() != 0) {
	if (event_specific_data[0] == 4)
	    str += "server]";
	if (event_specific_data[0] == 0)
	    str += "client]";
    }
    else str += "no_info]";
    syslog(priority, "%s", str.c_str());
    return TRUE;
}


MICOSL2::AuditChannelList::AuditChannelList()
{
}

MICOSL2::AuditChannelList::~AuditChannelList()
{
    list_.length(0);
}

Security::AuditChannelId
MICOSL2::AuditChannelList::register_channel(SecurityLevel2::AuditChannel* audit_channel)
{
  int len = list_.length();
  list_.length(len+1);
  list_[len] = audit_channel;
  return (Security::AuditChannelId) len;
}

void
MICOSL2::AuditChannelList::unregister_channel(Security::AuditChannelId id)
{
  int len = list_.length();
  assert(len > -1);
  assert(id < (CORBA::ULong)len);
  list_[id] = 0;
}

MICOSL2::AuditChannel_impl::AuditChannel_impl()
{
    archive_ = 0;
    channel_id_ = 0xFFFFFFFF;
}

MICOSL2::AuditChannel_impl::AuditChannel_impl
(const char *type_str,
 const char* archive_name)
{
    if (strcmp(type_str,"file") == 0)
	archive_ = new MICOSL2::FileArchive();
    else
#ifdef HAVE_PGSQL
	if (type_str == "db")
	    archive_ = new MICOSL2::DBArchive();
	else
#endif
	    if (strncmp(type_str,"syslog",6) == 0)
		archive_ = new MICOSL2::ConsoleArchive(atoi(type_str + 6));
  
    archive_->create(archive_name);
}

MICOSL2::AuditChannel_impl::~AuditChannel_impl()
{
    if (archive_)
	delete archive_;
  
    if (channel_id_ != 0xFFFFFFFF)
	S_channel_list.unregister_channel(channel_id_);
}

Security::AuditChannelId
MICOSL2::AuditChannel_impl::audit_channel_id()
{
    return channel_id_;
}

CORBA::Boolean
MICOSL2::AuditChannel_impl::create(const char* type_str, const char* name)
{
    if (archive_ != 0)
	delete archive_;

    if (strcmp(type_str,"file") == 0) {
	if (archive_)
	    delete archive_;
	archive_ = new MICOSL2::FileArchive();
    }
    else
#ifdef HAVE_PGSQL
	if (strcmp(type_str,"db") == 0) {
	    if (archive_)
		delete archive_;
	    archive_ = new MICOSL2::DBArchive();
	}
	else
#endif
	    if (strncmp(type_str,"syslog",6) == 0) {
		if (archive_)
		    delete archive_;
		archive_ = new MICOSL2::ConsoleArchive(atoi(type_str + 6));
	    }
	    else
		return FALSE;
  
    // finding in the global list - ...
    CORBA::Boolean ret = archive_->create(name);

    if (ret)
	channel_id_ = S_channel_list.register_channel(this);
    return ret;
}

void
MICOSL2::AuditChannel_impl::audit_write
(const Security::AuditEventType& event_type,
 const SecurityLevel2::CredentialsList& creds,
 const Security::UtcT& time,
 const Security::SelectorValueList& descriptors,
 const Security::Opaque& event_specific_data)
{
    archive_->write(event_type, creds, time, descriptors, event_specific_data);
}
/*
  void MICOSL2::AuditChannel_impl::policy_write(MICOSA::AuditPolicy_impl* policy) {
  archive -> policy_write(policy);
  }
  
  void MICOSL2::AuditChannel_impl::policy_get_write(MICOSA::AuditPolicy_impl* policy) {
  archive -> policy_get_write(policy);
  }
*/
////////////////////////////////////////////////////////////////////////////////////////////////
/*
  MICOSL2::AuditDecision_impl::AuditDecision_impl(Security::AuditChannelId channel_id) {
	audit_chann = new MICOSL2::AuditChannel_impl(channel_id);
}
*/

CORBA::Boolean
MICOSL2::AuditDecision_impl::create
(const char *type_str,
 const char* archive_name)
{
    if (!CORBA::is_nil(audit_chann_))
	CORBA::release(audit_chann_);
    audit_chann_ = new MICOSL2::AuditChannel_impl;
    CORBA::Boolean ret = audit_chann_->create(type_str, archive_name);
    if (!ret) {
	CORBA::release(audit_chann_);
	audit_chann_ = NULL;
	return FALSE;
    }
    CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb", FALSE);
    CORBA::Object_var policy_current_obj = orb->resolve_initial_references ("PolicyCurrent");
    SecurityLevel2::PolicyCurrent_var policy_current = SecurityLevel2::PolicyCurrent::_narrow(policy_current_obj);
    assert (!CORBA::is_nil (policy_current));

    CORBA::PolicyTypeSeq policy_types;
    policy_types.length(1);
    policy_types[0] = Security::SecClientInvocationAudit;
  
    CORBA::PolicyList_var policies = policy_current->get_policy_overrides(policy_types);
    CORBA::Policy_var policy = CORBA::Policy::_duplicate(policies[(CORBA::ULong)0]);
  
    SecurityAdmin::AuditPolicy_var audit_policy = SecurityAdmin::AuditPolicy::_narrow(policy);
  
    audit_policy->set_audit_channel(audit_chann_->audit_channel_id());
  
//  if (AuditServerInterceptor::_idle()) {
//    AuditServerInterceptor* server_intcpt = new AuditServerInterceptor;
//    server_intcpt -> activate (0);
//  }
//  if (AuditClientInterceptor::_idle()) {
//    AuditClientInterceptor* client_intcpt = new AuditClientInterceptor;
//    client_intcpt -> activate (0);
//  }
    return TRUE;
}

MICOSL2::AuditDecision_impl::AuditDecision_impl()
{
    audit_chann_ = NULL;
}

MICOSL2::AuditDecision_impl::~AuditDecision_impl()
{
    if (!CORBA::is_nil(audit_chann_))
	CORBA::release(audit_chann_);
}

SecurityLevel2::AuditChannel_ptr
MICOSL2::AuditDecision_impl::audit_channel()
{
    return SecurityLevel2::AuditChannel::_duplicate(audit_chann_);
}

CORBA::Boolean
MICOSL2::AuditDecision_impl::audit_needed
(const Security::AuditEventType& event_type,
 const Security::SelectorValueList& value_list)
{
    CORBA::ORB_var orb;
    SecurityLevel2::SecurityManager_var secman;
  
    orb = CORBA::ORB_instance ("mico-local-orb", FALSE);
    CORBA::Object_var securitymanager = orb->resolve_initial_references ("SecurityManager");
    if (CORBA::is_nil (securitymanager))
	return FALSE; // for now
  
    secman = SecurityLevel2::SecurityManager::_narrow(securitymanager);
    if (CORBA::is_nil (secman))
	return FALSE; // for now
  
    // make a domain key
    // first part of our key
    //own_cred = (*(secman->own_credentials()))[0];
    //SecurityLevel2::CredentialsList* t_clist = secman->own_credentials();
    SecurityLevel2::CredentialsList_var t_clist = secman->own_credentials();
    //SecurityLevel2::Credentials_ptr own_cred = (*t_clist)[0];
    SecurityLevel2::Credentials_var own_cred 
	= SecurityLevel2::Credentials::_duplicate(t_clist[(CORBA::ULong)0]);
    //delete t_clist;
    //we can request own (our server) credentials attributes, part of the domain key
    Security::ExtensibleFamily fam1;
    fam1.family_definer = 0;
    fam1.family = 1;
    Security::AttributeType at1;
    at1.attribute_family = fam1;
    at1.attribute_type = Security::AccessId;
    Security::AttributeTypeList atl1;
    atl1.length(1);
    atl1[0]=at1;
  
    //getting  the attributes from the Credentials
    //  Security::AttributeList_var al1 = own_cred->get_attributes(atl1);
    Security::AttributeList* al1 = own_cred->get_attributes(atl1);
    string key1 = "[";
    for (CORBA::ULong ctr = 0; ctr < (*al1).length(); ctr++) {
	key1 += (char *)(&(*al1)[ctr].value[0]);
    }
    delete al1;
    key1 += "] ";
    string key11 = key1;	
    string keyforoot = key1;
    keyforoot += "/";
    string rootkey = "/";
    // second part of our key
    CORBA::Object_var poao = orb->resolve_initial_references ("POACurrent");
    PortableServer::Current_var cpoa = PortableServer::Current::_narrow(poao);
    assert(!CORBA::is_nil(cpoa));
    PortableServer::POA_var poa;
    CORBA::Boolean rootflag = FALSE;
    string key2;
    string key22;
    try {
	poa = cpoa->get_POA();
    } catch (PortableServer::Current::NoContext_catch& ex) {
	//cerr << __FILE__ << ":" << __LINE__ << " " << ex._repoid() << endl;
	//assert(0);
	rootflag = TRUE;
    } catch (...) { // we have no poa
	rootflag = TRUE;
    }
    if (!rootflag) {
	PortableServer::POA_var np = poa; // ptr
	CORBA::String_var c_str = "";
	string tstr;
	while (!CORBA::is_nil(np)) {
	    c_str = np->the_name();
	    tstr = c_str.in();
	    if (key2.length() > 0)
		tstr += '/';
	    tstr += key2;
	    key2 = tstr;
	    np = np->the_parent();
	}
	np = poa;
	while (!CORBA::is_nil(np)) {
	    c_str = np->the_name();
	    tstr = c_str.in();
	    if (tstr == "RootPOA")
		break;
	    if (key22.length() > 0)
		tstr += '/';
	    tstr += key22;
	    key22 = tstr;
	    np = np->the_parent();
	}
    }
    else {
	key2 = "/";
    }
    key1 += key2; // got a key
    key11 += key22;
    Security::Opaque okey;
    Security::Opaque okey2;
    Security::Opaque okey3;
    int len = key1.length();
    okey.length(len);
    for (int i = 0; i < len; i++)
	okey[i] = key1[i];
  
    int len2 = key11.length();
    okey2.length(len2);
    for (int i = 0; i < len2; i++)
	okey2[i] = key11[i];
    int len3 = keyforoot.length();
    okey3.length(len3);
    for (int i = 0; i < len3; i++)
	okey3[i] = keyforoot[i];
  
    ObjectDomainMapping::Manager_var dmanager = ObjectDomainMapping::Manager::_nil();
    SecurityDomain::NameList* list; // domain name list
    CORBA::Boolean deflag = FALSE;
    if (!rootflag) {
	dmanager = poa->get_ODM();
    }
    PortableServer::LifespanPolicyValue polval = PortableServer::TRANSIENT;

    if (CORBA::is_nil(dmanager) || rootflag) {  // we don't have domain manager (poa without ODM)
	// or we don't have a poa at all
	CORBA::Object_var objodm = orb->resolve_initial_references ("ODM");
	ObjectDomainMapping::ODM_var odm = ObjectDomainMapping::ODM::_narrow(objodm);
	MICOSODM::Factory_impl* factory = dynamic_cast<MICOSODM::Factory_impl*>
	    (odm->current());
	if (!factory)
	    return FALSE;

	MICOSODM::DomainMap::iterator it = (factory->domains_->get_domain_map()).find(key2); //!
	if (it == (factory->domains_->get_domain_map()).end()) {
	    it = (factory->domains_->get_domain_map()).find(key22);
	    if (it == (factory->domains_->get_domain_map()).end()) {
		it = (factory->domains_->get_domain_map()).find(rootkey);
		if (it == (factory->domains_->get_domain_map()).end())
		    return FALSE;
		else
		    deflag = TRUE;
	    }
	    else {
	     	polval = PortableServer::PERSISTENT;
	    }
	    return FALSE;
	}
	dmanager = ObjectDomainMapping::Manager::_duplicate
	    (((*it).second)->dm_);
    }
    else if (!rootflag && !CORBA::is_nil(dmanager)) {
	polval = MICOSODM::get_lspolicy(dynamic_cast<MICOSODM::Manager_impl*>(dmanager.in()));
    }
    if (CORBA::is_nil(dmanager))
	return FALSE;
    //  PortableServer::LifespanPolicyValue polval = MICOSODM::get_lspolicy((MICOSODM::Manager_impl *) dmanager);
  
    if (deflag)
	list = dmanager->get_domain_names(okey3);
    else if (polval == PortableServer::TRANSIENT)
	list = dmanager->get_domain_names(okey);
    else
	list = dmanager->get_domain_names(okey2);
    // list contains appr. domain name list, go to get domain manager
    // first check do we have audit domain
    SecurityDomain::Name nm;
    for (CORBA::ULong i = 0; i < (*list).length(); i++) {
	if (strcmp(((*list)[i])[0].kind, "Audit") == 0) {
	    nm = (*list)[i];
	    break;
	}
    }

    delete list; // release it
    if (nm.length() == 0) { // not mapped to Audit Domain 
	return FALSE;
    }
    // correct the name (exclude root name)
    int ln = nm.length();
    if (ln == 1) {
	nm.length(0); // mapped to root domain
    }
    else {
	for (int k = 0; k < ln -1; k++)
	    nm[k] = nm[k + 1];
	nm.length(ln - 1);
    }
    CORBA::Object_var factobj = orb->resolve_initial_references("DomainManagerFactory");
    if (CORBA::is_nil (factobj))
	return FALSE; // for now
    SecurityDomain::DomainManagerFactory_var dmfactory = SecurityDomain::DomainManagerFactory::_narrow(factobj);
    if (CORBA::is_nil (dmfactory))
	return FALSE; // for now

    SecurityDomain::DomainManagerAdmin_var dmrootobj = dmfactory->get_root_domain_manager("Audit");
    if (CORBA::is_nil(dmrootobj))
	return FALSE;
    SecurityDomain::DomainAuthority_var admroot = SecurityDomain::DomainAuthority::_narrow(dmrootobj);

    SecurityDomain::DomainManagerAdmin_var dm;
    if (nm.length() == 0) { // root domain
	dm = SecurityDomain::DomainManagerAdmin::_duplicate(admroot);
    }
    else {
	dm = admroot->get_domain_manager(nm); // got it
    }
    // check return value
    if (CORBA::is_nil (dm))
	return FALSE; // domain stuff is broken, no such path, poa is registered with non-existent domain
  
    return this->decision(event_type, value_list, dm, SecurityDomain::Union);
}

CORBA::Boolean
MICOSL2::AuditDecision_impl::decision
(const Security::AuditEventType& event_type,
 const Security::SelectorValueList& value_list,
 SecurityDomain::DomainManagerAdmin_ptr dm,
 SecurityDomain::PolicyCombinator comb)
{
    SecurityDomain::DomainAuthority_var adm = SecurityDomain::DomainAuthority::_narrow(dm);
    SecurityDomain::PolicyCombinator _comb;
    try {
	_comb = adm->get_policy_combinator(Security::SecTargetInvocationAudit);
    } catch (...) {
	;//return FALSE;
    }
  
    CORBA::Policy_var policy;
    try {
	policy = dm->get_domain_policy(Security::SecTargetInvocationAudit);
    } catch (...) {
	return FALSE;
    }	
  
    SecurityAdmin::AuditPolicy_var apolicy = SecurityAdmin::AuditPolicy::_narrow(policy);
    if (CORBA::is_nil (apolicy))
	return FALSE;

    Security::SelectorValueList* pvaluelist = NULL;
    Security::AuditCombinator audit_combinator;
    const char* interface_name = "";
    const char* operation;
    const char* initiator;
    short s_f;
    for (CORBA::ULong i = 0; i < value_list.length(); i++) { // found some values if any
	if (value_list[i].selector == Security::InterfaceName)
	    value_list[i].value >>= interface_name;
	else if (value_list[i].selector == Security::Operation)
	    value_list[i].value >>= operation;
	else if (value_list[i].selector == Security::Initiator)
	    value_list[i].value >>= initiator;
	else if (value_list[i].selector == Security::SuccessFailure)
	    value_list[i].value >>= s_f;
    }

    apolicy->get_audit_selectors(interface_name, event_type, pvaluelist, audit_combinator);
  
    if (pvaluelist == NULL && *interface_name != 0)
	apolicy->get_audit_selectors("", event_type, pvaluelist, audit_combinator);
  
    if (pvaluelist == NULL)
	return FALSE;

    Security::SelectorValueList& default_selectors = *pvaluelist;
  
    CORBA::Boolean write_escape = FALSE;
    CORBA::Boolean flag;
    CORBA::Boolean sf_presence = FALSE;
    CORBA::Boolean selectors[6] = {FALSE,FALSE,FALSE,FALSE,FALSE,FALSE};
  
    if (default_selectors.length() == 0)
	return FALSE;
  
    for (CORBA::ULong i = 0; i < default_selectors.length(); i++){
	flag = TRUE;
	if (default_selectors[i].selector == Security::InterfaceName) {
	    const char* default_int_name;
	    default_selectors[i].value >>= default_int_name;
      
	    if (strcmp(interface_name ,"") == 0) {
		if (strcmp(default_int_name ,"") != 0)
		    if (audit_combinator == Security::SecAllSelectors)
			return FALSE;
		    else
			flag = FALSE;
	    } else if ((strcmp(default_int_name ,"") != 0) && (strcmp(default_int_name,interface_name) != 0)) {
		if (audit_combinator == Security::SecAllSelectors) 
		    return FALSE;
		else flag = FALSE;
	    }
	    if (flag) 
		selectors[0] = TRUE;
	} else if (default_selectors[i].selector == Security::Operation) {
	    if (strcmp(operation, "") != 0) {
		const char* def_oper;
		default_selectors[i].value >>= def_oper;
		if (strcmp(def_oper, operation) != 0 && strcmp(def_oper, "") != 0)
		    if (audit_combinator == Security::SecAllSelectors)
			return FALSE;
		    else
			flag = FALSE;
	    }
	    if (flag) 
		selectors[1] = TRUE;
	} else if (default_selectors[i].selector == Security::Initiator) {
	    if (strcmp(initiator, "") != 0) {
		const char* def_initiator;
		default_selectors[i].value >>= def_initiator;
		if (strcmp(def_initiator, initiator) != 0 && strcmp(def_initiator, "") != 0)
		    if (audit_combinator == Security::SecAllSelectors)
			return FALSE;
		    else
			flag = FALSE;
	    }
	    if (flag) 
		selectors[2] = TRUE;
	} else if (default_selectors[i].selector == Security::SuccessFailure) {
	    sf_presence = TRUE;
	    if (s_f != -1) {
		short def_s_f;
		default_selectors[i].value >>= def_s_f;
		if (def_s_f != s_f && def_s_f != -1)
		    if (audit_combinator == Security::SecAllSelectors)
			return FALSE;
		    else
			flag = FALSE;
	    }
	    if (flag) 
		selectors[3] = TRUE;
	} else if (default_selectors[i].selector == Security::Time) {
	    time_t t = time(0);
	    SecurityAdmin::AuditTimeInterval default_time_interval;
	    default_selectors[i].value >>= default_time_interval;
	    if (default_time_interval.begin < default_time_interval.end) {
		if ((t < default_time_interval.begin) || (t > default_time_interval.end))
		    if (audit_combinator == Security::SecAllSelectors) 
			return FALSE;
		    else
			flag = FALSE;
	    }
	    if (flag) 
		selectors[4] = TRUE;
	} else if (default_selectors[i].selector == Security::DayOfWeek)	{
	    time_t t = time(0);
	    tm* time_struct = localtime(&t);
	    short tmp;
	    default_selectors[i].value >>= tmp;
	    int default_day = (int) tmp;
	    if (( default_day != -1) && (default_day != time_struct -> tm_wday))
		if (audit_combinator == Security::SecAllSelectors)
		    return FALSE;
		else 
		    flag = FALSE;
	    if (flag) 
		selectors[5] = TRUE;
	}
    } // end for
    if (pvaluelist)
	delete pvaluelist;
    write_escape = selectors[0] || selectors[1] || selectors[2] || selectors[3] || selectors[4] || selectors[5];
    if (!write_escape) { // && !sf_presence) {
	SecurityDomain::DomainManagerAdminList_var parents = dm->get_parent_domain_managers();
	if (parents->length() == 0)
	    return FALSE;
	else 
	    return this->decision(event_type, value_list, parents[(CORBA::ULong)0], _comb);
    }
    return TRUE;
}


