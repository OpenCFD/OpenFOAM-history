//
//  MICOsec --- a free CORBA Security implementation
//  Copyright (C) 2000,2001 ObjectSecurity Ltd.
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
#include <mico/template_impl.h>
#include <mico/util.h>
#include <mico/security/SecurityAdmin_impl.h>


using namespace std;

MICOSA::AuditStruct::AuditStruct(const AuditStruct& _s)
{
    selectors_ = ((AuditStruct&)_s).selectors_;
    event_ = ((AuditStruct&)_s).event_;
    audit_combin_ = ((AuditStruct&)_s).audit_combin_;
}

MICOSA::AuditStruct&
MICOSA::AuditStruct::operator=(const AuditStruct& _s)
{
    selectors_ = ((AuditStruct&)_s).selectors_;
    event_ = ((AuditStruct&)_s).event_;
    audit_combin_ = ((AuditStruct&)_s).audit_combin_;
    return *this;
}

MICOSA::AuditPolicy_impl::AuditPolicy_impl()
{
    standard_events_.length(4);
    Security::AuditEventType evt;
    evt.event_family.family_definer = 0; // OMG
    evt.event_family.family = 12; // Audit event family
    standard_events_[3] = standard_events_[2] = standard_events_[1] = standard_events_[0] = evt;
    standard_events_[0].event_type = Security::AuditPrincipalAuth;
    standard_events_[1].event_type = Security::AuditSessionAuth;
    standard_events_[2].event_type = Security::AuditAuthorization;
    standard_events_[3].event_type = Security::AuditInvocation;
}

MICOSA::AuditPolicy_impl::~AuditPolicy_impl()
{
}

CORBA::Policy_ptr
MICOSA::AuditPolicy_impl::copy()
{
    MICOSA::AuditPolicy_impl* p = new MICOSA::AuditPolicy_impl;
    p->audit_map_ = audit_map_;
    return p;
}

void
MICOSA::AuditPolicy_impl::set_audit_selectors
(const char* object_type,
 const Security::AuditEventTypeList& events,
 const Security::SelectorValueList& selectors,
 Security::AuditCombinator audit_combinator)
{
    CORBA::ULong i;
    Security::AuditEventTypeList _events = events;
    for (i = 0; i < events.length(); i++) {
	if (events[i].event_type == Security::AuditAll) {
	    _events = standard_events_;
	    break;
	}
    }
  
    for (i = 0; i < _events.length(); i++) {
	CORBA::String_var c_str = this->mkkey(object_type, _events[i]);
	string mapkey = c_str.in();
	MICOSA::AuditStruct * as = new MICOSA::AuditStruct;
	as->selectors_ = selectors;
	as->audit_combin_ = audit_combinator;
	as->event_ = _events[i];
	audit_map_[mapkey] = as;
    }
}

char*
MICOSA::AuditPolicy_impl::mkkey
(const char* name,
 Security::AuditEventType eventtype)
{
    char buf[512];
    sprintf(buf, "%d~", eventtype.event_type);
    string str;
    str = (char *)buf;
    str += name;
    return CORBA::string_dup(str.c_str());
}

void
MICOSA::AuditPolicy_impl::clear_audit_selectors
(const char* object_type,
 const Security::AuditEventTypeList& events)
{
    CORBA::ULong i;
    Security::AuditEventTypeList _events = events;
    for (i = 0; i < events.length(); i++) {
	if (events[i].event_type == Security::AuditAll) {
	    _events = standard_events_;
	    break;
	}
    }
    for (i = 0; i < _events.length(); i++) {
	CORBA::String_var c_str = this->mkkey(object_type, _events[i]);
	string mapkey = c_str.in();
	audit_map_.erase(mapkey);
    }
}

void
MICOSA::AuditPolicy_impl::replace_audit_selectors
(const char* object_type,
 const Security::AuditEventTypeList& events,
 const Security::SelectorValueList& selectors,
 Security::AuditCombinator audit_combinator)
{
    CORBA::ULong i;
    Security::AuditEventTypeList _events = events;
    for (i = 0; i < events.length(); i++) {
	if (events[i].event_type == Security::AuditAll) {
	    _events = standard_events_;
	    break;
	}
    }
  
    for (i = 0; i < _events.length(); i++) {
	CORBA::String_var c_str = this->mkkey(object_type, _events[i]);
	string mapkey = c_str.in();
	if (audit_map_.erase(mapkey)) {
	    MICOSA::AuditStruct * as = new MICOSA::AuditStruct;
	    as->selectors_ = selectors;
	    as->audit_combin_ = audit_combinator;
	    as->event_ = _events[i];
	    audit_map_[mapkey] = as;
	}
    }	
}


void
MICOSA::AuditPolicy_impl::get_audit_selectors
(const char* object_type,
 const Security::AuditEventType& event_type,
 Security::SelectorValueList_out selectors,
 Security::AuditCombinator_out audit_combinator)
{
    CORBA::String_var c_str = this->mkkey(object_type, event_type);
    string mapkey = c_str.in();
    MICOSA::AuditStructMap::iterator it = audit_map_.find(mapkey);

    if (selectors)
	delete selectors;

    if (it != audit_map_.end()) {
	selectors = new Security::SelectorValueList(((*it).second)->selectors_);
	audit_combinator = ((*it).second)->audit_combin_;
    }
    else {
	selectors = NULL;
    }
}


void
MICOSA::AuditPolicy_impl::set_audit_channel
(Security::AuditChannelId audit_channel_id)
{
    channel_id_ = audit_channel_id;
}


MICOSA::AuditClientPolicy_impl::AuditClientPolicy_impl()
{
    Security::AuditEventTypeList events;
    events.length(4);
    Security::ExtensibleFamily family;
    family.family_definer = 0;
    family.family = 12;
  
    events[0].event_family = family;
    events[0].event_type = Security::AuditAll;
    
    //Selectors
    Security::SelectorValueList selectors;
    selectors.length(7);
  
    selectors[0].selector = Security::InterfaceName;
    selectors[0].value <<= "";
  
    selectors[1].selector = Security::ObjectRef;
    selectors[1].value <<= "";
  
    selectors[2].selector = Security::Operation;
    selectors[2].value <<= "";
  
    selectors[3].selector = Security::Initiator;
    selectors[3].value <<= "";
  
    selectors[4].selector = Security::SuccessFailure;
    selectors[4].value <<= (short)(-1);
  
    selectors[5].selector = Security::Time;
    SecurityAdmin::AuditTimeInterval time_interval;
    time_interval.begin = 1;
    time_interval.end =  1;
    selectors[5].value <<= time_interval; //any time interval;

    selectors[6].selector = Security::DayOfWeek;
    selectors[6].value <<= (short)(-1); //any day of week

    Security::AuditCombinator audit_combinator =  Security::SecAllSelectors;
    set_audit_selectors("",events,selectors,audit_combinator );
}


CORBA::Policy_ptr
MICOSA::AuditClientPolicy_impl::copy()
{
    MICOSA::AuditClientPolicy_impl* p = new MICOSA::AuditClientPolicy_impl;
    p->audit_map_ = audit_map_;
    return p;
}


MICOSA::AuditTargetPolicy_impl::AuditTargetPolicy_impl()
{
    /*
      Security::AuditEventTypeList events;
      events.length(4);
    
      Security::ExtensibleFamily family;
      family.family_definer = 0;
      family.family = 12;
      
      events[0].event_family = family;
      events[0].event_type = Security::AuditAll;
      
      
      //Selectors
      Security::SelectorValueList selectors;
      selectors.length(7);
      
      selectors[0].selector = Security::InterfaceName;
      selectors[0].value <<= "";
      
      selectors[1].selector = Security::ObjectRef;
      selectors[1].value <<= "";
      
      selectors[2].selector = Security::Operation;
      selectors[2].value <<= "";
      
      selectors[3].selector = Security::Initiator;
      selectors[3].value <<= "";
      
      selectors[4].selector = Security::SuccessFailure;
      selectors[4].value <<= (short)(-1);
      
      selectors[5].selector = Security::Time;
      SecurityAdmin::AuditTimeInterval time_interval;
      time_interval.begin = 1;
      time_interval.end =  1;
      selectors[5].value <<= time_interval; //any time interval;
      
      selectors[6].selector = Security::DayOfWeek;
      selectors[6].value <<= (short)(-1); //any day of week
      
      Security::AuditCombinator audit_combinator =  Security::SecAllSelectors;
      set_audit_selectors("",events,selectors,audit_combinator );
    */
}

CORBA::Policy_ptr
MICOSA::AuditTargetPolicy_impl::copy()
{
    MICOSA::AuditTargetPolicy_impl* p = new MICOSA::AuditTargetPolicy_impl;
    p->audit_map_ = audit_map_;
    return p;
}


//
// Access Policy
//
MICOSA::AccessPolicy_impl::AccessPolicy_impl()
{
}


MICOSA::AccessPolicy_impl::~AccessPolicy_impl()
{
}


CORBA::Policy_ptr
MICOSA::AccessPolicy_impl::copy ()
{
    MICOSA::AccessPolicy_impl* p = new MICOSA::AccessPolicy_impl;
    p->atrmap_ = atrmap_;
    return p;
}


void
MICOSA::AccessPolicy_impl::attribute_to_key(const Security::SecAttribute& attr)
{
    char buf[512];
    key_ = "";
  
    sprintf(buf, "%d~", attr.attribute_type.attribute_family.family_definer);
    key_ += (char *)buf;
    sprintf(buf, "%d~", attr.attribute_type.attribute_family.family);
    key_ += (char *)buf;
    sprintf(buf, "%ld~", attr.attribute_type.attribute_type);
    key_ += (char *)buf;
    for (CORBA::ULong i = 0; i < attr.value.length(); i++) {
	key_ += attr.value[i];
    }
}

Security::RightsList*
MICOSA::AccessPolicy_impl::get_effective_rights
(const Security::AttributeList& attribute_list,
 const Security::ExtensibleFamily& rights_family)
{
    Security::RightsList* irights;
    Security::RightsList* ret_rights = new Security::RightsList;
  
    for (CORBA::ULong i = 0; i < attribute_list.length(); i++) {
	this->attribute_to_key(attribute_list[i]);
	SecAttributeMap::iterator it = atrmap_.find(key_);
	if (it != atrmap_.end()) {
	    irights = (*it).second;
	    int len = ret_rights->length();
	    for (CORBA::ULong j = 0; j < irights->length(); j++) {
		if ((*irights)[i].rights_family.family_definer == rights_family.family_definer
		    && (*irights)[i].rights_family.family == rights_family.family) {
		    len++;
		    ret_rights->length(len);
		    (*ret_rights)[len - 1] = (*irights)[j];
		}
	    }
	}
    }
    return ret_rights;
}


Security::RightsList*
MICOSA::AccessPolicy_impl::get_all_effective_rights
(const Security::AttributeList& attrib_list)
{
    Security::RightsList* irights;
    Security::RightsList* ret_rights = new Security::RightsList;
  
    for (CORBA::ULong i = 0; i < attrib_list.length(); i++) {
	this->attribute_to_key(attrib_list[i]); 
	SecAttributeMap::iterator it = atrmap_.find(key_);
	if (it != atrmap_.end()) {
	    irights = (*it).second;
	    int len = ret_rights->length();
	    for (CORBA::ULong j = 0; j < irights->length(); j++) {
		len++;
		ret_rights->length(len);
		(*ret_rights)[len - 1] = (*irights)[j];
	    }
	}
    }
    return ret_rights;
}

MICOSA::AccessTargetPolicy::AccessTargetPolicy()
{
}

//MICOSA::AccessTargetPolicy::~AccessTargetPolicy() {
//
//}

//
// Domain Access Policy
//

MICOSA::DomainAccessPolicy_impl::DomainAccessPolicy_impl()
{
}

MICOSA::DomainAccessPolicy_impl::~DomainAccessPolicy_impl()
{
}

void
MICOSA::DomainAccessPolicy_impl::grant_rights
(const Security::SecAttribute& priv_attr,
 Security::DelegationState del_state,
 const Security::RightsList& rights)
{
    Security::RightsList* irights;
  
    this->attribute_to_key(priv_attr); 
    SecAttributeMap::iterator it = atrmap_.find(key_);
    if (it != atrmap_.end()) { // found such a key, already exists
	irights = (*it).second;
	for (CORBA::ULong i = 0; i < rights.length(); i++) {
	    CORBA::Boolean f = TRUE;
	    for (CORBA::ULong j = 0; j < irights->length(); j++) {
		if (rights[i].rights_family.family_definer == (*irights)[j].rights_family.family_definer
		    && rights[i].rights_family.family == (*irights)[j].rights_family.family) {
		    if (strcmp(rights[i].rights_list, (*irights)[j].rights_list) == 0) { // has it already
			f = FALSE;
			break;
		    }
		}
	    }
	    if (f) { // needs to be added
		int len = irights->length();
		irights->length(len + 1);
		(*irights)[len] = rights[i];
	    }
	}
    }
    else {
	if (atrmap_[key_])
	    delete atrmap_[key_];
	atrmap_[key_] = new Security::RightsList(rights); // not found key, new
    }
}


void
MICOSA::DomainAccessPolicy_impl::revoke_rights
(const Security::SecAttribute& priv_attr,
 Security::DelegationState del_state,
 const Security::RightsList& rights)
{
    Security::RightsList* irights;
	
    this->attribute_to_key(priv_attr); 
    SecAttributeMap::iterator it = atrmap_.find(key_);
    if (it != atrmap_.end()) { // found such a key, already exists
	irights = (*it).second;
	for (CORBA::ULong i = 0; i < rights.length(); i++) {
	    for (CORBA::ULong j = 0; j < irights->length(); j++) {
		if (rights[i].rights_family.family_definer == (*irights)[j].rights_family.family_definer
		    && rights[i].rights_family.family == (*irights)[j].rights_family.family) {
		    if (strcmp(rights[i].rights_list, (*irights)[j].rights_list) == 0) {
			int len = irights->length(); // remove it
			for (int k = j; k < len - 1; k++)
			    (*irights)[k] = (*irights)[k + 1];
			irights->length(len -1);
			break;
		    }
		}
	    }
	}
    }
}


void
MICOSA::DomainAccessPolicy_impl::replace_rights
(const Security::SecAttribute& priv_attr,
 Security::DelegationState del_state,
 const Security::RightsList& rights)
{
    //Security::RightsList * irights;
  
    this->attribute_to_key(priv_attr); 
    SecAttributeMap::iterator it = atrmap_.find(key_);
    if (it != atrmap_.end()) { // found such a key, already exists
	atrmap_.erase(it);
	if (atrmap_[key_])
	    delete atrmap_[key_];
	atrmap_[key_] = new Security::RightsList(rights);
    }
}


Security::RightsList*
MICOSA::DomainAccessPolicy_impl::get_rights
(const Security::SecAttribute& priv_attr,
 Security::DelegationState del_state,
 const Security::ExtensibleFamily& rights_family)
{
    Security::RightsList* ret_rights = new Security::RightsList;
    Security::RightsList* irights;
  
    this->attribute_to_key(priv_attr); 
    SecAttributeMap::iterator it = atrmap_.find(key_);
    if (it != atrmap_.end()) {
	irights = (*it).second;
	int len = ret_rights->length();
	for (CORBA::ULong j = 0; j < irights->length(); j++) {
	    if ((*irights)[j].rights_family.family_definer == rights_family.family_definer
		&& (*irights)[j].rights_family.family == rights_family.family) {
		len++;
		ret_rights->length(len);
		(*ret_rights)[len - 1] = (*irights)[j];
	    }
	}
    }
    return ret_rights;
}

Security::RightsList*
MICOSA::DomainAccessPolicy_impl::get_all_rights
(const Security::SecAttribute& priv_attr,
 Security::DelegationState del_state)
{
    Security::RightsList* ret_rights = new Security::RightsList;
    Security::RightsList* irights;
  
    this->attribute_to_key(priv_attr); 
    SecAttributeMap::iterator it = atrmap_.find(key_);
    if (it != atrmap_.end()) {
	irights = (*it).second;
	int len = ret_rights->length();
	for (CORBA::ULong j = 0; j < irights->length(); j++) {
	    len++;
	    ret_rights->length(len);
	    (*ret_rights)[len - 1] = (*irights)[j];
	}
    }
    return ret_rights;
}

// for new mapping

MICOSA::RightsRecord::RightsRecord(const RightsRecord& _s)
{
    opname_ = ((RightsRecord&)_s).opname_;
    rightslist_ = ((RightsRecord&)_s).rightslist_;
    combinator_ = ((RightsRecord&)_s).combinator_;
}

MICOSA::RightsRecord&
MICOSA::RightsRecord::operator=(const RightsRecord& _s)
{
    opname_ = ((RightsRecord&)_s).opname_;
    rightslist_ = ((RightsRecord&)_s).rightslist_;
    combinator_ = ((RightsRecord&)_s).combinator_;
    return *this;
}


MICOSA::ObjectAccessPolicy_impl::ObjectAccessPolicy_impl()
{
}

MICOSA::ObjectAccessPolicy_impl::~ObjectAccessPolicy_impl()
{
}

CORBA::Policy_ptr
MICOSA::ObjectAccessPolicy_impl::copy ()
{
    MICOSA::ObjectAccessPolicy_impl* p = new MICOSA::ObjectAccessPolicy_impl;
    p->rightsmap_ = rightsmap_;
    return p;
}


void
MICOSA::ObjectAccessPolicy_impl::get_required_rights
(CORBA::Object_ptr obj,
 const char* operation_name,
 const char* interface_name,
 Security::RightsList_out rights,
 Security::RightsCombinator& rights_combinator)
{
    RightsRecord* rec;
    string key;
  
    const char* iname = 0;
    if (!CORBA::is_nil(obj))
  	iname = obj->_ior()->objid();
    if (iname == NULL || strlen(iname) == 0)
	key = interface_name;
    else
	key = iname;
  
    key += "~";
    key += operation_name;
    RightsMap::iterator it = rightsmap_.find(key);
    if (it != rightsmap_.end()) {
	rec = (*it).second;
	if (rec->opname_ == operation_name) {
	    rights_combinator = rec->combinator_;
	    rights = new Security::RightsList(rec->rightslist_);
	}
    }
}


void
MICOSA::ObjectAccessPolicy_impl::set_required_rights
(const char* operation_name,
 const char* interface_name,
 const Security::RightsList& rights,
 Security::RightsCombinator rights_combinator)
{
    string key;
	
    key = interface_name;
    key += "~";
    key += operation_name;
    RightsRecord* rec = new RightsRecord;
    rec->rightslist_ = rights;
    rec->opname_ = CORBA::string_dup(operation_name);
    rec->combinator_ = rights_combinator;
    rightsmap_[key] = rec;
}
