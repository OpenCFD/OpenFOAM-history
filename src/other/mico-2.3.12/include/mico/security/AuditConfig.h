#ifndef _auditconfig_h
#define _auditconfig_h
#include <mico/security/DomainManager_impl.h>
#include <mico/security/SecurityAdmin_impl.h>

class AuditConfig : public auFlexLexer
{
private:
    int error;
    Security::SecAttribute attr;
    std::string iface_name;
    std::string opt;
    std::string op_name;
    SecurityDomain::DomainAuthorityAdmin_var admt;
    SecurityAdmin::AuditPolicy_ptr cpol; // client policy
    SecurityAdmin::AuditPolicy_ptr spol; // server policy
    Security::AuditCombinator at_comb;
    Security::AuditEventTypeList eventtypelst;
    Security::SelectorValueList  selectorlst;

    MICOSDM::NameExt_impl namext;
    SecurityDomain::Name * nm;
    SecurityDomain::PolicyCombinator pol_comb;
    MICOSDM::DomainManagerFactory_impl * dm_factory;

protected:
    time_t parse_time(std::string strtime) {

        struct tm time_struct;
        //int len = strtime.length();
        int pos = strtime.find('/', 0);
        std::string year = strtime.substr(0, pos); // year
        pos++;
        int pos2 = strtime.find('/', pos);
        std::string month = strtime.substr(pos, pos2 - pos); // month
        pos2++;
        pos = strtime.find(':', pos2);
        std::string day = strtime.substr(pos2, pos - pos2); // day
        pos++;
        std::string hour = strtime.substr(pos, 2); // hour
        pos += 3;
        std::string min = strtime.substr(pos, 2); // minutes
        pos += 3;
        std::string sec = strtime.substr(pos, 2); // seconds
        time_struct.tm_sec = atoi(sec.c_str());
        time_struct.tm_min = atoi(min.c_str());
        time_struct.tm_hour = atoi(hour.c_str());
        time_struct.tm_mday = atoi(day.c_str());
        time_struct.tm_mon = atoi(month.c_str());
        time_struct.tm_mon = time_struct.tm_mon - 1;
        time_struct.tm_year = atoi(year.c_str());
        time_struct.tm_year = time_struct.tm_year - 1900;
        time_struct.tm_isdst = 1;
        return mktime(&time_struct);
    }

public:
    AuditConfig(std::istream* arg_yyin = 0, std::ostream* arg_yyout = &std::cout) : auFlexLexer(arg_yyin, arg_yyout)
    {
      selectorlst.length(7);
      error = 0;
    }
    virtual int yylex();
    void startParse(MICOSDM::DomainManagerFactory_impl * factory) {dm_factory = factory; while (yylex() != 0) ;}
    int geterror() { return error; }
};

#endif
