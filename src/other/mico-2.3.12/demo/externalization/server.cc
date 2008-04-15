#define MICO_CONF_IMR
#include <CORBA.h>
#include <iostream.h>
#include "hello.h"
#include <coss/IdentifiableObject_impl.h>
#include <coss/RegisterHelper.h>

class Hello_impl : virtual public IdentifiableObject_impl,
		   virtual public POA_Hello,
		   virtual public POA_CosStream::Streamable
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
    
    virtual CosLifeCycle::Key* external_form_id();

    virtual void externalize_to_stream( CosStream::StreamIO_ptr targetStreamIO );
    virtual void internalize_from_stream( CosStream::StreamIO_ptr sourceStreamIO, CosLifeCycle::FactoryFinder_ptr there );
    char * hello (const char *s)
    {
	if (hello_string_)
	    cout << hello_string_<<' ';
	cout << s << endl;
	return CORBA::string_dup (s);
    }
};
CosLifeCycle::Key*
Hello_impl::external_form_id()
{
    CosLifeCycle::Key* factory_id_ = new CosLifeCycle::Key();
    (*factory_id_).length(4);
    (*factory_id_)[0].id = CORBA::string_dup("::CosStream::Streamable");
    (*factory_id_)[1].id = CORBA::string_dup("");
    (*factory_id_)[2].id = CORBA::string_dup("::Hello");
    (*factory_id_)[3].id = CORBA::string_dup("StreamableFactory");
	(*factory_id_)[0].kind = CORBA::string_dup( _lc_ks_object_interface );
	(*factory_id_)[1].kind = CORBA::string_dup( _lc_ks_impl_equiv_class );
	(*factory_id_)[2].kind = CORBA::string_dup( _lc_ks_object_implementation );
	(*factory_id_)[3].kind = CORBA::string_dup( _lc_ks_factory_interface );

    return factory_id_;
}

void
Hello_impl::externalize_to_stream( CosStream::StreamIO_ptr targetStreamIO )
{
    targetStreamIO -> write_string(this->hello_string_);
};

void
Hello_impl::internalize_from_stream( CosStream::StreamIO_ptr sourceStreamIO, CosLifeCycle::FactoryFinder_ptr there )
{
    CORBA::string_free(hello_string_);
    hello_string_ = sourceStreamIO -> read_string();
};

class HelloFactory_impl : virtual public POA_CosStream::StreamableFactory
{
public:
    HelloFactory_impl(CORBA::ORB_ptr _orb)
	: POA_CosStream::StreamableFactory()
    {
	IdentifiableObject_impl* tmp = new IdentifiableObject_impl (_orb);
    };
    virtual CosStream::Streamable_ptr create_uninitialized()
    {
	Hello_impl* hello_impl = new Hello_impl();
	Hello_ptr hello = hello_impl->_this();
//	CORBA::string_free(str);
	return hello;
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


    HelloFactory_impl* hello = new HelloFactory_impl(orb);
    poa->activate_object (hello);
    mgr->activate ();


    RegisterHelper reg_help(orb,type);
	CosNaming::Name hf_id(4);
	hf_id.length(4);
	hf_id[0].kind = CORBA::string_dup( _lc_ks_object_interface );
	hf_id[0].id = CORBA::string_dup( "::CosStream::Streamable" );
	hf_id[1].kind = CORBA::string_dup( _lc_ks_impl_equiv_class );
	hf_id[1].id = CORBA::string_dup("");
	hf_id[2].kind = CORBA::string_dup( _lc_ks_object_implementation );
	hf_id[2].id = CORBA::string_dup("::Hello");
	hf_id[3].kind = CORBA::string_dup( _lc_ks_factory_interface );
	hf_id[3].id = CORBA::string_dup("StreamableFactory");

	reg_help.register_factory(hello->_this(),hf_id);

    orb->run ();
    return 0;
}
