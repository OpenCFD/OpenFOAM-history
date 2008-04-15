#ifndef _odmconfig_h
#define _odmconfig_h
#include <mico/security/odm_impl.h>

// Configuration File Parser
class ODMConfig : public odmFlexLexer
{
private:

int error;
MICOSODM::DomainMap dmap;
MICOSODM::ODMRecord * record;
std::string key;
std::string x509key;
std::string type;
ObjectDomainMapping::Manager::ODMGranularity granularity;

public:
	ODMConfig(std::istream* arg_yyin = 0, std::ostream* arg_yyout = 0) : odmFlexLexer(arg_yyin, arg_yyout)
	{
		error = 0;
	}
	virtual int yylex();
	void startParse(MICOSODM::DomainMap& dm) {dmap = dm; while (yylex() != 0) ;}
	int geterror() { return error; }
	MICOSODM::DomainMap& getMap() {return dmap;}
};

#endif
