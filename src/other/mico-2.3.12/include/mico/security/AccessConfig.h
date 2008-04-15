#ifndef _accessconfig_h
#define _accessconfig_h
#include <mico/security/DomainManager_impl.h>

class AccessConfig : public acFlexLexer
{
private:
    int error;
    // for new mapping
    //Security::SecAttribute attr;
    Security::RightsCombinator combinator; 		 	// for new mapping
    std::string iface_name; 								// for new mapping
    std::string op_name;    								// for new mapping
    SecurityDomain::DomainAuthorityAdmin_var admt;	// for new mapping
    SecurityAdmin::ObjectAccessPolicy_var dpol;		// for new mapping

    Security::RightsList rights;
    Security::Right right;
    MICOSDM::NameExt_impl namext;
    SecurityDomain::Name * nm;
    SecurityDomain::PolicyCombinator pol_comb;
    MICOSDM::DomainManagerFactory_impl * dm_factory;

public:
    AccessConfig(std::istream* arg_yyin = 0, std::ostream* arg_yyout = &std::cout) : acFlexLexer(arg_yyin, arg_yyout)
    {
      error = 0;
 	}
    virtual int yylex();
    void startParse(MICOSDM::DomainManagerFactory_impl * factory) {dm_factory = factory; while (yylex() != 0) ;}
    int geterror() { return error; }
};

#endif
