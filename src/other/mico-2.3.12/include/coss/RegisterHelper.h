#include <CORBA.h>
#include <coss/CosLifeCycle.h>
#include <coss/CosTrading.h>
#include <coss/CosTradingRepos.h>


#ifndef __RegisterHelper_h__
#define __RegisterHelper_h__

class RegisterHelper
{
public:
    enum FinderType
    {
	none = 0,
	naming,
	trading
    };
private:
    CORBA::ORB_var _orb;
    CORBA::Object_ptr _obj;
    FinderType _type;
    CORBA::Boolean naming_register_factory(const CORBA::Object_ptr factory, const CosNaming::Name& factory_id) const;
    CORBA::Boolean trading_register_factory(const CORBA::Object_ptr factory, const CosNaming::Name& factory_id) const;
public:
    RegisterHelper();
    RegisterHelper(CORBA::ORB_ptr orb, FinderType type = naming);
    ~RegisterHelper();
    CORBA::Boolean register_factory(const CORBA::Object_ptr factory, const CosNaming::Name& factory_id) const;
};

// Declare Life Cycle Key Specification
extern MICO_EXPORT const char* _lc_ks_object_interface;
extern MICO_EXPORT const char* _lc_ks_impl_equiv_class;
extern MICO_EXPORT const char* _lc_ks_object_implementation;
extern MICO_EXPORT const char* _lc_ks_factory_interface;

#endif /* __RegisterHelper_h__ */
