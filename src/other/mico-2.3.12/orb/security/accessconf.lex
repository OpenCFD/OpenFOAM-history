%{
#include <mico/DomainManager_impl.h>
#include <mico/AccessConfig.h>


using namespace std;

// MICOSDM::DomainManagerFactory_impl * dm_factory;
// Security::SecAttribute attr;
// Security::RightsList rights;
// Security::Right right;
// MICOSDM::NameExt_impl namext;
// SecurityDomain::Name * nm;
// SecurityDomain::PolicyCombinator pol_comb;
%}

%option noyywrap
%option yyclass="AccessConfig"
%x section atvalue attype domain comment rtype polcomb rigs
%%

[[:blank:]\n]+
"#"						{// comment
						 BEGIN(comment);
						}
<comment>.*/\n			{
						 // comment here, nothing to do
						 BEGIN(INITIAL);
						}
[[:blank:]\n]*/"["		{// x509 started
						 BEGIN(atvalue);
						}
<<EOF>>					{// End of file
						 if (error)
						   *yyout << "\n** syntax error(s)!\n";
						 return 0;
						}
<atvalue>"["[^]]*"]"	{// attr. value here
						 string str(YYText());
						 int len = str.length();
						 attr.value.length(len - 1);
						 for (CORBA::ULong i = 1; i < len - 1; i++)
						 	attr.value[i - 1] = str[i];
						 attr.value[len - 2] = 0;	
						 	
						 BEGIN(attype);
						}

<attype>[[:alpha:]]+/":" {// AccessId, PrimaryGroupId, etc.
						  string str(YYText());
						  attr.attribute_type.attribute_family.family_definer = 0;
						  attr.attribute_type.attribute_family.family = 1;

						  if (str == "AccessId")
						  	attr.attribute_type.attribute_type = Security::AccessId;
						  else if (str == "PrimaryGroupId")
						    attr.attribute_type.attribute_type = Security::PrimaryGroupId;
						  else if (str == "GroupId")
						  	attr.attribute_type.attribute_type = Security::PrimaryGroupId;
						  else if (str == "Public")
						  	attr.attribute_type.attribute_type = Security::Public;
						  else {
						  	*yyout << "** Unknown attribute - " << str << endl;
						  	error = 1;
						  	return 0;
						  }
						  BEGIN(rtype);
						 }
<rtype>":"
<rtype>[[:alpha:]]+/":" {// rights type - "corba", etc.
						  string str(YYText());
						  right.rights_family.family_definer = 0; // OMG
						  if (str == "corba")
						    right.rights_family.family = 1; // corba
						  else {
						  	*yyout << "** Unknown type of rights - " << str << endl;
						  	error = 1;
						  	return 0;
						  }
						  BEGIN(polcomb);
						 }
<polcomb>":"
<polcomb>[[:alpha:]]+/":" { // Policy combinator
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
						    BEGIN(rigs);
						 }
<rigs>":"
<rigs>[[:alpha:]\-]+/[[:blank:]\n]+"{" {// Rights "msgu"
									  string str(YYText());
									  int len = str.length();
									  if (str != "-") { // "-" means "no rights at all"
									    rights.length(len);
									    string opt;
									    for (int i = 0; i < len; i++) {
									      opt = str.substr(i, 1);
									  	  right.right = CORBA::string_dup(opt.c_str());
									  	  rights[i] = right;
									    }
									  }
									  else {
									  	rights.length(0);
									  }
				  		 			  BEGIN(domain);
			 						 }
<domain>[[:blank:]\n]+"{"
<domain>[[:blank:]\n]+
<domain>"/"[[:alnum:]\/\:\.]+ { // domains here
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
								int i;
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
								SecurityDomain::DomainAuthorityAdmin_var admt = SecurityDomain::DomainAuthorityAdmin::_narrow(dmt);
								CORBA::Policy_var po = admt->get_domain_policy(Security::SecTargetInvocationAccess);
								SecurityAdmin::DomainAccessPolicy_var dpol =
								  SecurityAdmin::DomainAccessPolicy::_narrow(po);
								dpol->grant_rights(attr, Security::SecInitiator, rights);
								admt->set_domain_policy(dpol);
								admt->set_policy_combinator(Security::SecTargetInvocationAccess, pol_comb);
								
								delete nm;
								  	
				     	        BEGIN(domain);
				    	      }

<domain>[[:blank:]]*"}" BEGIN(INITIAL);

<*>. {error = 1; ECHO;}

%%
