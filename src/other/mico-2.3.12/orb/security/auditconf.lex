%{
#include <mico/AuditConfig.h>
#include <mico/template_impl.h>
#include <mico/util.h>


using namespace std;

// 	int error;
//  Security::SecAttribute attr;
//  string iface_name;
//  string opt;
//  SecurityDomain::DomainAuthorityAdmin_var admt;
//  SecurityAdmin::AuditPolicy_var cpol; // client policy
//  SecurityAdmin::AuditPolicy_var spol; // server policy
//	Security::AuditCombinator at_comb;
//	Security::AuditEventTypeList eventtypelst;
//	Security::SelectorValueList  selectorlst;
//	
//  MICOSDM::NameExt_impl namext;
//  SecurityDomain::Name * nm;
//  SecurityDomain::PolicyCombinator pol_comb;
// 	MICOSDM::DomainManagerFactory_impl * dm_factory;
%}

%option noyywrap
%option yyclass="AuditConfig"
%x comment evt atcomb iface option vle rec combp recom optcom
%%

[[:blank:]\n]+
"#"						{// comment
						 BEGIN(comment);
						}
<comment>.*/\n			{
						 // comment here, nothing to do
						 BEGIN(INITIAL);
						}
<<EOF>>					{// End of file
						 if (error)
						   *yyout << "\n** syntax error(s)!\n";
						 return 0;
						}
"/"[[:alnum:]\/\:\.]+/[[:blank:]\n]+		{// domain name here
											nm = namext.to_name(YYText());
											for (CORBA::ULong i = 0; i < nm->length(); i++) {
											  (*nm)[i].kind = CORBA::string_dup((*nm)[0].id);
											}
											dm_factory->add_root_domain_manager((*nm)[0].id);
											SecurityDomain::DomainManagerAdmin_var dmroot = 
											  dm_factory->get_root_domain_manager((*nm)[0].id);
											SecurityDomain::Name tempname;
											tempname.length(1);
											SecurityDomain::DomainManagerAdmin_var dmt =
											  SecurityDomain::DomainManagerAdmin::_duplicate(dmroot);
											CORBA::ULong i;
											for (i = 1; i < nm->length(); i++) { // check full name and create if necessary
											  SecurityDomain::DomainAuthorityAdmin_var adm =
												SecurityDomain::DomainAuthorityAdmin::_narrow(dmt);
											  tempname[0] = (*nm)[i];
											  SecurityDomain::DomainManagerAdmin_var dm = adm->get_domain_manager(tempname); 
											  if (CORBA::is_nil(dm)) { // no such name
												SecurityDomain::DomainManagerAdmin_var ndm = dm_factory->create_domain_manager();
												adm->add_domain_manager(ndm, tempname);
												dmt = SecurityDomain::DomainManagerAdmin::_duplicate(ndm);
											  }
											  else {
												dmt = SecurityDomain::DomainManagerAdmin::_duplicate(dm);
											  }
											}
											//
											tempname.length(nm->length() - 1);
											for (i = 1; i < nm->length(); i++)
												tempname[i -1] = (*nm)[i];

											SecurityDomain::DomainAuthorityAdmin_var admroot =
											  SecurityDomain::DomainAuthorityAdmin::_narrow(dmroot);

											dmt = admroot->get_domain_manager(tempname); // our target domain manager, not nil !
											admt = SecurityDomain::DomainAuthorityAdmin::_narrow(dmt);
											CORBA::Policy_ptr po = admt->get_domain_policy(Security::SecTargetInvocationAudit);
											spol = SecurityAdmin::AuditPolicy::_narrow(po);
											delete nm;											  
											BEGIN(rec);
										}
<combp>[[:blank:]]+
<combp>"Combinator"[[:blank:]]*"="[[:blank:]]*/[[:alpha:]]+
<combp>[[:alpha:]]+/[[:blank:]\n]+"{"	   {// Policy combinator
											string str(YYText());
											if (str == "Union")
											  pol_comb = SecurityDomain::Union;
											else if (str == "Intersection")
											  pol_comb = SecurityDomain::Intersection;
											else if (str == "Negation")
											  pol_comb = SecurityDomain::Negation;
											else {
											  *yyout << "** Unknown Policy Combinator type - " << str << endl;
											  error = 1;
											  return 0;
											}
											BEGIN(rec);
										   }
										   
<recom>.*/\n							   { BEGIN(rec); }		   
										   										   
<rec>[[:blank:]\n]+
<rec>"#"									{ BEGIN(recom); }
<rec>"{"
<rec>[[:blank:]\n]*
<rec>"}" 									{ // end of policy fulfilment
												admt->set_domain_policy(spol);
												admt->set_policy_combinator(Security::SecTargetInvocationAudit, SecurityDomain::Union);
												BEGIN(INITIAL);
											}
<rec>"("[[:blank:]\n]*
<rec>("server"|"client")/[[:blank:]]*":"	{ // server/client, not used in this mapping (object->domain)
											string str(YYText());
											BEGIN(atcomb);
										}
<atcomb>[[:blank:]]*":"
<atcomb>[[:blank:]]*
<atcomb>("All"|"Any")/[[:blank:]]+		{ // audit combinator
											string str(YYText());
											if (str == "Any")
												at_comb = Security::SecAnySelector;
											else if (str == "All")
												at_comb = Security::SecAllSelectors;
											else {
											  *yyout << "** Unknown Audit Combinator type - " << str << endl;
											  error = 1;
											  return 0;
											}
											BEGIN(iface);
										}
<iface>[[:blank:]]+	
<iface>([[:alnum:]\/\:\.]+|"*")/[[:blank:]]+"[" { // interface name
											iface_name = YYText();
											if (iface_name == "*")
												iface_name = "";
											eventtypelst.length(0);
											
											BEGIN(evt);
										}
<evt>[[:blank:]]+"["
<evt>[[:alpha:][:blank:]\,]+/"]"		{ // event types list, remove blanks first
											string tmp(YYText());
											string list("");
											CORBA::ULong i, j;
											for (i = 0; i < tmp.length(); i++) {
												if (tmp[i] == ' ') continue;
												list += tmp[i];
											}
											list += ',';
											i = 0;
											int len = 0;
											string part;
											Security::AuditEventType evtype;
											evtype.event_family.family_definer = 0; // OMG
											evtype.event_family.family = 12; // Audit event family
											for (; (j = list.find(',', i)) >= 0; j++, i += j - i) {
												part = list.substr(i, j - i);
												if (part == "All") {
													evtype.event_type = Security::AuditAll;
												}
												else if (part == "PrincipalAuth")  {
													evtype.event_type = Security::AuditPrincipalAuth;
												}
												else if (part == "SessionAuth") {
													evtype.event_type = Security::AuditSessionAuth;
												}
												else if (part == "Authorization") {
													evtype.event_type = Security::AuditAuthorization;
												}
												else if (part == "Invocation") {
													evtype.event_type = Security::AuditInvocation;
												}
//												else if (part == "FailedAuthorization")
//													evtype.event_type = Security::AuditFailedAuthorization;
												else {
											  		*yyout << "** Unknown Audit Event type - " << part << endl;
											  		error = 1;
											  		return 0;
												}
												eventtypelst.length(len + 1);
												eventtypelst[len] = evtype;
												len++;
											}
//											selectorlst->length(7);
											selectorlst[0].selector = Security::InterfaceName;
											selectorlst[0].value <<= "";

											selectorlst[1].selector = Security::ObjectRef;
											selectorlst[1].value <<= "";

											selectorlst[2].selector = Security::Operation;
											selectorlst[2].value <<= "";

											selectorlst[3].selector = Security::Initiator;
											selectorlst[3].value <<= "";

											selectorlst[4].selector = Security::SuccessFailure;
											selectorlst[4].value <<= (short)(-1);

											selectorlst[5].selector = Security::Time;
											SecurityAdmin::AuditTimeInterval time_interval;
											time_interval.begin = 1;
											time_interval.end =  1;
											selectorlst[5].value <<= time_interval; //any time interval;

											selectorlst[6].selector = Security::DayOfWeek;
											selectorlst[6].value <<= (short)(-1); //any day of week
											
											BEGIN(option);
										}
										
<optcom>.*/\n							{ BEGIN(option); }
										
<option>"]"
<option>[[:blank:]\n]*
<option>"#"									{ BEGIN(optcom); }
<option>")" 								{ // end of options
												selectorlst[0].value <<= CORBA::string_dup(iface_name.c_str());
												spol->set_audit_selectors(CORBA::string_dup(iface_name.c_str()),
																		 eventtypelst, selectorlst, at_comb);
												BEGIN(rec);
											}
<option>[[:alpha:]]+/[[:blank:]]*"="		{ // option here
												opt = YYText();
												BEGIN(vle);
											}
<vle>[[:blank:]]*"="
<vle>[[:blank:]]*
<vle>"["[[:alnum:][:blank:]\:\-\/\.\+\\\,\;\?\!\=\@]+"]"		{ // option value here
												string val1(YYText());
												int i = 0;
												while (val1[i] == '[' || val1[i] == ' ') { // leading blanks and [
													i++;
												}
												int j = val1.length() - 1;
												while (val1[j] == ']' || val1[j] == ' ') { // trailing blanks and ]
													j--;
												}
												string val = val1.substr(i, j - i + 1);
												Security::SelectorValue sval;
												if (opt == "Operation") {
													sval.value <<= CORBA::string_dup(val.c_str());
													sval.selector = Security::Operation;
													selectorlst[2] = sval;
												}
												else if (opt == "Initiator") {
													sval.value <<= CORBA::string_dup(val.c_str());
													sval.selector = Security::Initiator;
													selectorlst[3] = sval;
												}
												else if (opt == "SuccessFailure") {
													sval.selector = Security::SuccessFailure;
													if (val == "true")
														sval.value <<= (short)1;
													else if (val == "false")
														sval.value <<= (short)0;
													else {
											  			*yyout << "** Unknown SuccessFailure value - " << val << endl;
											  			error = 1;
											  			return 0;
													}
													selectorlst[4] = sval;
												}
												else if (opt == "Time") {
													sval.selector = Security::Time;
													int pos = val.find('-', 0);
													string strbeg = val.substr(0, pos);
													string strend = val.substr(pos + 1, val.length() - pos - 1);
													time_t tbeg = parse_time(strbeg);
													time_t tend = parse_time(strend);
													if (tbeg == (time_t)(-1) || tend == (time_t)(-1)) {
											  			*yyout << "** Wrong Time value - " << val << endl;
											  			error = 1;
											  			return 0;
											  		}
													SecurityAdmin::AuditTimeInterval tmint;
													tmint.begin = tbeg;
													tmint.end = tend;
													sval.value <<= tmint;
													selectorlst[5] = sval;
												}
												else if (opt == "DayOfWeek") {
													sval.selector = Security::DayOfWeek;
													if (val == "Sun")
														sval.value <<= (short)0;
													else if (val == "Mon")
														sval.value <<= (short)1;
													else if (val == "Tue")
														sval.value <<= (short)2;
													else if (val == "Wed")
														sval.value <<= (short)3;
													else if (val == "Thu")
														sval.value <<= (short)4;
													else if (val == "Fri")
														sval.value <<= (short)5;
													else if (val == "Sat")
														sval.value <<= (short)6;
													else {
											  			*yyout << "** Wrong DayOfWeek value - " << val << endl;
											  			error = 1;
											  			return 0;
											  		}
											  		selectorlst[6] = sval;
												}
												else {
													*yyout << "** Wrong selector value - " << val << endl;
													error = 1;
													return 0;
												}
													
//												int len = selectorlst.length();
//												selectorlst.length(len + 1);
//												selectorlst[len] = sval;
												
												BEGIN(option);
											}
    

<*>. {error = 1; ECHO;}

%%
