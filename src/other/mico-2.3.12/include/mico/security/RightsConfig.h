#ifndef _rightsconfig_h
#define _rightsconfig_h
#include <mico/security/securitylevel2_impl.h>

class RightsConfig : public rrFlexLexer
{
 private:
 	int error;
 	// for new mapping
 	// MICOSL2::RequiredRights_impl * r_rights;
  	// Security::RightsCombinator combinator;
  	// string iface_name;
  	// string op_name;
  	Security::SecAttribute attr; // for new mapping
 	MICOSL2::AccessRights_impl * r_rights; // for new mapping 	
  	Security::Right right;
  	Security::RightsList rlist;

 public:
 	RightsConfig(std::istream* arg_yyin = 0, std::ostream* arg_yyout = &std::cout) : rrFlexLexer(arg_yyin, arg_yyout)
 	{
 		error = 0;
 	}
 	virtual int yylex();
 	void startParse(MICOSL2::AccessRights_impl * rrights) {r_rights = rrights; while (yylex() != 0) ;}
 	int geterror() { return error; }
};

#endif
