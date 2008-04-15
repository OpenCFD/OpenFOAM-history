#include <iostream.h>
// sander include <sys/time.h>
#include <CORBA.h>
#include "hello.h"
#include <coss/CosLifeCycle.h>
#include <coss/RegisterHelper.h>


int
main (int argc, char *argv[])
{
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");
    CORBA::BOA_var boa = orb->BOA_init (argc, argv, "mico-local-boa");
    cout << "Binding to FactoryFinder ..." << flush;
    CORBA::ORB::ObjectTag_var 
	tagff = CORBA::ORB::string_to_tag ("FactoryFinder_impl");
    CORBA::Object_var 
    objff = orb->bind ("IDL:omg.org/CosLifeCycle/FactoryFinder:1.0", tagff.in());
    assert (!CORBA::is_nil (objff));
    CosLifeCycle::FactoryFinder_var factory_finder; 
    if (factory_finder = CosLifeCycle::FactoryFinder::_narrow (objff))
	cout << "done.\n" << flush;
    else
	cout << "ERROR!\n" << flush;

    cout << "Binding to GenericFactory ..." << flush;
    CORBA::ORB::ObjectTag_var 
	taggf = CORBA::ORB::string_to_tag ("GenericFactory_impl");
    CORBA::Object_var 
    objgf = orb->bind ("IDL:omg.org/CosLifeCycle/GenericFactory:1.0", taggf.in());
    assert (!CORBA::is_nil (objgf));
    CosLifeCycle::GenericFactory_var generic_factory; 
    if (generic_factory = CosLifeCycle::GenericFactory::_narrow (objgf))
	cout << "done.\n" << flush;
    else
	cout << "ERROR!\n" << flush;

    CosLifeCycle::Criteria init_param;
    init_param.length(1);
    init_param[0].name = CORBA::string_dup("hello string");
    init_param[0].value <<= CORBA::string_dup("Hello");
    CosLifeCycle::Criteria the_criteria;
    the_criteria.length(1);
    the_criteria[0].name = CORBA::string_dup("initialization");
    the_criteria[0].value <<= init_param;
    
    CosLifeCycle::Key factory_id_;
    factory_id_.length(4);
    factory_id_[0].id = CORBA::string_dup("::CosLifeCycle::LifeCycleObject");
    factory_id_[1].id = CORBA::string_dup("");
    factory_id_[2].id = CORBA::string_dup("::Hello");
    factory_id_[3].id = CORBA::string_dup("GenericFactory");
	factory_id_[0].kind = CORBA::string_dup( _lc_ks_object_interface );
	factory_id_[1].kind = CORBA::string_dup( _lc_ks_impl_equiv_class );
	factory_id_[2].kind = CORBA::string_dup( _lc_ks_object_implementation );
	factory_id_[3].kind = CORBA::string_dup( _lc_ks_factory_interface );


    cout << "Check GenericFactory::create_object() ...";
    CORBA::Boolean good=true;
    Hello_var hello;
    try {
	hello = Hello::_narrow(generic_factory->create_object(factory_id_,the_criteria));
    } catch(CosLifeCycle::NoFactory& ex) {
	good=false;
	cout << "Factory not found!";
    } catch(CosLifeCycle::InvalidCriteria& ex) {
	good=false;
	cout << "Criteria is invalid!";
    } catch(CosLifeCycle::CannotMeetCriteria& ex) {
	good=false;
	cout << "Can not meet criteria!";
    } catch(...) {
	good=false;
	cout << "System exception";
    }

    if (good)
	cout << "ok\n";
    else
	cout << "failed\n";

    cout << "Check GenericFactory::support() ...";
    good=FALSE;
    Hello_var hello1;
    try {
	good = generic_factory->support(factory_id_);
    } catch(CosLifeCycle::NoFactory& ex) {
	good=false;
	cout << "Factory not found!";
    } catch(CosLifeCycle::InvalidCriteria& ex) {
	good=false;
	cout << "Criteria is invalid!";
    } catch(CosLifeCycle::CannotMeetCriteria& ex) {
	good=false;
	cout << "Can not meet criteria!";
    } catch(...) {
	good=false;
	cout << "System exception";
    }

    if (good)
	cout << "ok\n";
    else
	cout << "failed\n";

    cout << "Check NoFactory exception...";
    factory_id_.length(4);
    factory_id_[0].id = CORBA::string_dup("::CosLifeCycle::LifeCycleObject");
    factory_id_[1].id = CORBA::string_dup("");
    factory_id_[2].id = CORBA::string_dup("::Hello_bad"); // <---- mistake
    factory_id_[3].id = CORBA::string_dup("GenericFactory");
	factory_id_[0].kind = CORBA::string_dup( _lc_ks_object_interface );
	factory_id_[1].kind = CORBA::string_dup( _lc_ks_impl_equiv_class );
	factory_id_[2].kind = CORBA::string_dup( _lc_ks_object_implementation );
	factory_id_[3].kind = CORBA::string_dup( _lc_ks_factory_interface );

    try {
	hello1 = Hello::_narrow(generic_factory->create_object(factory_id_,the_criteria));
    } catch(CosLifeCycle::NoFactory& ex) {
	good=true;
	cout << "Factory not found!";
    } catch(CosLifeCycle::InvalidCriteria& ex) {
	cout << "Criteria is invalid!";
    } catch(CosLifeCycle::CannotMeetCriteria& ex) {
	cout << "Can not meet criteria!";
    } catch(...) {
	cout << "System exception";
    }
    if (good)
	cout << "ok\n";
    else
	cout << "failed\n";
    
    CORBA::String_var s = hello->hello ("MICO");

    cout << "Check copy operation...";
    good=TRUE;
    try {
	hello1 = Hello::_narrow(hello->copy(factory_finder,the_criteria));
    } catch(CosLifeCycle::NoFactory& ex) {
	good=false;
	cout << "Factory not found!";
    } catch(CosLifeCycle::InvalidCriteria& ex) {
	good=false;
	cout << "Criteria is invalid!";
    } catch(CosLifeCycle::CannotMeetCriteria& ex) {
	good=false;
	cout << "Can not meet criteria!";
    } catch(CosLifeCycle::NotCopyable& ex) {
	good=false;
	cout << "Can not meet criteria!";
	cout << "reason - " << ex.reason << "\n";
    } catch(...) {
	good=false;
	cout << "System exception";
    }
    if (good)
	cout << "ok\n";
    else
	cout << "failed\n";

    s = hello->hello ("world");

    cout << "Check remove operation...";
    good=TRUE;
    try {
	hello->remove();
    } catch(CosLifeCycle::NoFactory& ex) {
	good=false;
	cout << "Factory not found!";
    } catch(CosLifeCycle::InvalidCriteria& ex) {
	good=false;
	cout << "Criteria is invalid!";
    } catch(CosLifeCycle::CannotMeetCriteria& ex) {
	good=false;
	cout << "Can not meet criteria!";
    } catch(CosLifeCycle::NotRemovable& ex) {
	good=false;
	cout << "Can not meet criteria!";
	cout << "reason - " << ex.reason << "\n";
    } catch(...) {
	good=false;
	cout << "System exception";
    }
    if (good)
	cout << "ok\n";
    else
	cout << "failed\n";
	
    return 0;
}
