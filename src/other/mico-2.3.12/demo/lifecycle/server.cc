#define MICO_CONF_IMR
#include <CORBA.h>
#include <iostream.h>
#include "hello.h"
#include <coss/GenericFactory_impl.h>
#include <coss/RegisterHelper.h>


class Hello_impl : virtual public POA_Hello,
		   virtual public POA_CosLifeCycle::LifeCycleObject
{
    char * hello_string_;
public:
    Hello_impl() { hello_string_ = (char*)NULL; };
    Hello_impl(const char * str) { hello_string_ = CORBA::string_dup(str); };
    virtual char* hello_string() { return CORBA::string_dup(hello_string_); };
    virtual void hello_string( const char* value )
	{
	    CORBA::string_free(hello_string_);
	    hello_string_ = CORBA::string_dup(value);
	};
    
    virtual CosLifeCycle::LifeCycleObject_ptr
	    copy( CosLifeCycle::FactoryFinder_ptr there, const CosLifeCycle::Criteria& the_criteria );
    virtual void move( CosLifeCycle::FactoryFinder_ptr there, const CosLifeCycle::Criteria& the_criteria );
    virtual void remove();
    char * hello (const char *s)
    {
	if (hello_string_)
	    cout << hello_string_<<' ';
	cout << s << endl;
	return CORBA::string_dup (s);
    }
};

CosLifeCycle::LifeCycleObject_ptr
Hello_impl::copy(CosLifeCycle::FactoryFinder_ptr there, const CosLifeCycle::Criteria& the_criteria )
{
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

    CosLifeCycle::Factories* object_factories_ptr = there -> find_factories(factory_id_);
    if (!(*object_factories_ptr).length())
    {
	if (object_factories_ptr) delete object_factories_ptr;
	CosLifeCycle::NoFactory ex;
	ex.search_key = factory_id_;
	mico_throw(ex);
    }

    CORBA::ULong i;
    CosLifeCycle::GenericFactory_ptr hellofactory_ptr;
    Hello_ptr new_obj;
    for (i = 0;i < (*object_factories_ptr).length();++i)
    {
	hellofactory_ptr = CosLifeCycle::GenericFactory::_narrow((*object_factories_ptr)[i]);
	if (CORBA::is_nil(hellofactory_ptr))
	    continue;
	new_obj = Hello::_narrow(hellofactory_ptr->create_object(factory_id_,the_criteria));
	if (CORBA::is_nil(new_obj))
	    continue;
	break;
    }
    
    if (i == (*object_factories_ptr).length())
    {
	if (object_factories_ptr) delete object_factories_ptr;
	CosLifeCycle::NoFactory ex;
	ex.search_key = factory_id_;
	mico_throw(ex);
    }
    delete object_factories_ptr;
    return new_obj;
}

void
Hello_impl::move(CosLifeCycle::FactoryFinder_ptr there, const CosLifeCycle::Criteria& the_criteria )
{
};

void
Hello_impl::remove()
{
#if USE_VAR
    PortableServer::ObjectId_var oid = _default_POA ()->servant_to_id (this);
#else
    PortableServer::ObjectId* oid = _default_POA ()->servant_to_id (this);
#endif
    _default_POA ()->deactivate_object (*oid);

#ifndef USE_VAR
    delete oid;
#endif
    delete this;
}

class HelloFactory_impl : virtual public GenericFactory_impl
{
public:
    CORBA::Object_ptr create_object (const CosLifeCycle::Key &factory_key,
				     const CosLifeCycle::Criteria &the_criteria)
    {
	char * str = (char *)NULL;
	CosLifeCycle::Criteria init_param;
	init_param.length(0);
	for(CORBA::ULong i = 0;i < the_criteria.length();++i)
	    if (!strcmp(the_criteria[i].name,"initialization"))
	    {
		the_criteria[i].value >>= init_param;
		break;
	    }
	Hello_impl* hello_impl = NULL;
	if (!init_param.length())
	    hello_impl = new Hello_impl;
	else
	{
	    for(CORBA::ULong i = 0;i < init_param.length();++i)
		if (!strcmp(init_param[i].name,"hello string"))
		{
		    init_param[i].value >>= str;
		    break;
		}
	    if (str)
		hello_impl = new Hello_impl(CORBA::string_dup(str));
	    else
		hello_impl = new Hello_impl;
	}
	Hello_ptr hello = hello_impl->_this();
//	CORBA::string_free(str);
	return Hello::_duplicate(hello);
    };
};

int
main (int argc, char *argv[])
{
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");
    CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
    PortableServer::POAManager_var mgr = poa->the_POAManager();
	RegisterHelper::FinderType type(RegisterHelper::none);
	if (argc>0)
		if (!strcmp(argv[1],"-t"))
			type=RegisterHelper::trading;
		else
			type=RegisterHelper::naming;


    HelloFactory_impl* hello = new HelloFactory_impl();
    poa->activate_object (hello);
    mgr->activate ();

//	RegisterHelper reg_help(orb,RegisterHelper::trading);
	RegisterHelper reg_help(orb,type);
	CosNaming::Name hf_id(4);
	hf_id.length(4);
	hf_id[0].kind = CORBA::string_dup( _lc_ks_object_interface );
	hf_id[0].id = CORBA::string_dup( "::CosLifeCycle::LifeCycleObject" );
	hf_id[1].kind = CORBA::string_dup( _lc_ks_impl_equiv_class );
	hf_id[1].id = CORBA::string_dup("");
	hf_id[2].kind = CORBA::string_dup( _lc_ks_object_implementation );
	hf_id[2].id = CORBA::string_dup("::Hello");
	hf_id[3].kind = CORBA::string_dup( _lc_ks_factory_interface );
	hf_id[3].id = CORBA::string_dup("GenericFactory");

	reg_help.register_factory(hello->_this(),hf_id);

    orb->run ();
    return 0;
}
