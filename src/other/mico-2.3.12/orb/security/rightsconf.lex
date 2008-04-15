%{
#include <mico/RightsConfig.h>


using namespace std;

//	int error;
// 	MICOSL2::RequiredRights_impl * r_rights;
// 	Security::RightsCombinator combinator;
// 	Security::Right right;
// 	Security::RightsList rlist;
// 	string iface_name;
// 	string op_name;
%}

%option noyywrap
%option yyclass="RightsConfig"
%x comment operation rtype combr rghts
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
						
[[:alnum:]\:\.]+/[[:blank:]]+	     {// interface name
								      iface_name = YYText();
								      BEGIN(operation);
								     }

<operation>[[:blank:]]+
<operation>[[:alnum:]]+/[[:blank:]]+ { // operation
									  op_name = YYText();
									  BEGIN(rtype);
								     }
								
<rtype>[[:blank:]]+
<rtype>[[:alnum:]]+/":"				 { // rights type
									  string str(YYText());
									  right.rights_family.family_definer = 0; // OMG
									  if (str == "corba")
						                right.rights_family.family = 1; // corba
						              else {
						  	            *yyout << "** Unknown type of rights - " << str << endl;
						  	            error = 1;
						  	            return 0;
						              }
						              BEGIN(combr);
								     }
<combr>":"
<combr>[[:alpha:]]+/":"	 			 { // combinator
									  string str(YYText());
									  if (str == "SecAllRights")
									  	combinator = Security::SecAllRights;
									  else if (str == "SecAnyRight")
									  	combinator = Security::SecAnyRight;
									  else {
									  	*yyout << "** Unknown Rights Combinator - " << str << endl;
									  	error = 1;
									  	return 0;
									  }
									  BEGIN(rghts);
								     }
								     
<rghts>":"
<rghts>[[:alpha:]\-]+/[[:blank:]\n]+   { // rights "usgm"
									  string str(YYText());
									  if (str != "-") { // "-" means "no rights at all"
									  	int len = str.length();
									  	rlist.length(len);
									    string opt;
									    for (int i = 0; i < len; i++) {
									      opt = str.substr(i, 1);
									  	  right.right = CORBA::string_dup(opt.c_str());
									  	  rlist[i] = right;
									    }
									  }
									  else {
									  	rlist.length(0);
									  }
									  
									  r_rights->set_required_rights((const char *)op_name.c_str(),
									    (const char *)iface_name.c_str(), rlist, combinator);
									  
									  BEGIN(INITIAL);
									 }
								     
<*>. {error = 1; ECHO;}

%%
