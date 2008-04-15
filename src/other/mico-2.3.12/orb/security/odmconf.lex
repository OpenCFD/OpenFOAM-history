%{
#include <mico/odm_impl.h>
#include <mico/ODMConfig.h>


using namespace std;

//MICOSODM::DomainMap dmap;
//MICOSODM::ODMRecord * record;
//string key;
%}

%option noyywrap
%option yyclass="ODMConfig"
%x section x509dn poa domain comment
%%

[[:blank:]\n]+
"#"						{// comment
						 BEGIN(comment);
						}
<comment>.*/\n			{
						 // comment here, nothing to do
						 BEGIN(INITIAL);
						}
[[:blank:]\n]*/"["		{// x509 DN started
						 BEGIN(x509dn);
						}
<<EOF>>					{// End of file
						 if (error)
						   *yyout << "\n** syntax error(s)!\n";
						 return 0;
						}
<x509dn>"["[^]]+"]"		{// x509 DN here
						 granularity = 1;
						 x509key = YYText();
						 BEGIN(poa);
						}

<poa>"/"[[:alnum:]\/\:\.]*/[[:blank:]]+			{// poa, root or object here
												 string str(YYText());
												 int len;
												 if ((len = str.length()) > 1) {
												 	if (str[len-1] == '/') {
												 		granularity = 2;
												 		key = str.substr(1, len - 2);
													}
												 	else {
											 			granularity = 3;
											 			key = str.substr(1, len - 1);
													}
											 	 }
											 	 else if (str[0] == '/') { // root, default domain
											 	 	granularity = 1;
											 	 	key = str;
											 	 }

				  		 						 BEGIN(domain);
				 								}
<domain>[[:blank:]]+
<domain>"/"[[:alnum:]\/\:\.]+/[\n[:blank:]]+	{ // domain here
													MICOSODM::DomainMap::iterator it = dmap.find(key);
													if (it == dmap.end()) {// no such key
														record = new MICOSODM::ODMRecord;
													}
													else { // found it
														record = (*it).second;
													}
													record->dm_ = NULL; // no manager yet
													record->gran_ = granularity;
													record->x509_ = x509key;
													record->pol_ = PortableServer::PERSISTENT; // default
													CORBA::ULong slen = record->dnamelist_.length();
													//
													SecurityDomain::Name name;
													SecurityDomain::NameComponent nc;
													string str(YYText());
													if (str[0] != '/') {
														error = 1;
						   								*yyout << "\n** syntax error - " << str << endl;
						 								return 0;
													}
													int j = str.find('/', 1);
													type = j > 0 ? str.substr(1, j - 1) : str.substr(1);
												    
													int len = str.length();
											 	 	int i = 0;
											 	 	string part;
											 	 	for (; (j = str.find('/', i)) >= 0; j++, i += j - i) {
														part = str.substr(i, j - i);
														if (part.length() == 0)
															continue;
														nc.id = CORBA::string_dup(part.c_str());
														nc.kind = CORBA::string_dup(type.c_str());
														CORBA::ULong l = name.length();
														name.length(l + 1);
														name[l] = nc;
											 	 	}
											 	 	if (i < len) { // there was no '/' at the end
														part = str.substr(i, len - i);
														nc.id = CORBA::string_dup(part.c_str());
														nc.kind = CORBA::string_dup(type.c_str());
														CORBA::ULong l = name.length();
														name.length(l + 1);
														name[l] = nc;
											 	 	}
											 	 	record->dnamelist_.length(slen + 1);
											 	 	record->dnamelist_[slen] = name;

											 	 	dmap[key] = record;

				     	     	 				 BEGIN(INITIAL);
				    	     					}

<*>. {error = 1; ECHO;}

%%

