#include <coss/RegisterHelper.h>

#ifdef USE_CORBA_2_1
#define CORBA_CXX_PREFIX(x) _##x
#else
#define CORBA_CXX_PREFIX(x) _cxx_##x
#endif

const char* _lc_ks_object_interface = "ObjectInterface";
const char* _lc_ks_impl_equiv_class = "ImplEquivClass";
const char* _lc_ks_object_implementation = "ObjectImpl";
const char* _lc_ks_factory_interface = "FactoryInterface";

RegisterHelper::RegisterHelper()
    : _obj(CORBA::Object::_nil()), _type(none)
{
};

RegisterHelper::RegisterHelper(CORBA::ORB_ptr orb, FinderType type)
    : _orb(CORBA::ORB::_duplicate(orb)), _obj(CORBA::Object::_nil()), _type(type)
{
 
#if DEBUG
    cout << "  <RegisterHelper> constructor:\n";
    cout << "  <RegisterHelper> Binding to Trading Service..." 
	 << flush;
#endif
	switch (_type)
	{
		case naming:
		{
#if DEBUG
    		cout << "  <RegisterHelper> Binding to Naming Service..."
	 			 << flush;
#endif
#ifdef HAVE_EXCEPTIONS
			try {
#endif
				_obj = _orb -> resolve_initial_references("NameService");
				if (!CORBA::is_nil (_obj))
	    		{
					_type = naming;
					return;
	    		}
#ifdef HAVE_EXCEPTIONS
			} catch(CORBA::ORB::InvalidName&) {
#if DEBUG
			    std::cerr << "Can`t resolve Naming Service." << std::endl;
#endif
			}
#endif
		}
		case trading:
		{
#ifdef HAVE_EXCEPTIONS
			try {
#endif
	    		_obj = _orb -> resolve_initial_references("TradingService");
	    		if (!CORBA::is_nil (_obj))
	    		{
					_type = trading;
					return;
	    		}
#ifdef HAVE_EXCEPTIONS
			} catch(CORBA::ORB::InvalidName&) {
#if DEBUG
			    std::cerr << "Can`t resolve Trading Service." << std::endl;
#endif
			}
#endif
		}
		case none: break;
	}
#if DEBUG
  cout << "  <InterfaceDefsHelper> constructor done.\n";
#endif

};

RegisterHelper::~RegisterHelper()
{
};

CORBA::Boolean
RegisterHelper::register_factory(const CORBA::Object_ptr factory, const CosNaming::Name& factory_id) const
{
	CORBA::Boolean good = FALSE;
	CosNaming::Name id(4);
	id.length(4);
	id[0].kind = CORBA::string_dup( _lc_ks_object_interface );
	id[1].kind = CORBA::string_dup( _lc_ks_impl_equiv_class );
	id[2].kind = CORBA::string_dup( _lc_ks_object_implementation );
	id[3].kind = CORBA::string_dup( _lc_ks_factory_interface );
	for(CORBA::UShort i = 0;i < factory_id.length();++i)
	{
		if (!strcmp(_lc_ks_object_interface,factory_id[i].kind.in()))
			id[0].id = factory_id[i].id;
		if (!strcmp(_lc_ks_impl_equiv_class,factory_id[i].kind.in()))
			id[1].id = factory_id[i].id;
		if (!strcmp(_lc_ks_object_implementation,factory_id[i].kind.in()))
			id[2].id = factory_id[i].id;
		if (!strcmp(_lc_ks_factory_interface,factory_id[i].kind.in()))
		{
			id[3].id = factory_id[i].id;
			good = TRUE;
		}
	}
	if (!good)
		return FALSE;

    switch (_type)
    {
        case none: return FALSE;
        case naming: return naming_register_factory(factory, id);
        case trading: return trading_register_factory(factory, id);
    }
};

CORBA::Boolean
RegisterHelper::naming_register_factory(const CORBA::Object_ptr factory, const CosNaming::Name& factory_id) const
{
	if (CORBA::is_nil(_obj))
	{
	    std::cerr << "Can`t resolve Naming Service. Abort" << std::endl;
	    return FALSE;
	}
	CosNaming::NamingContext_var nc = CosNaming::NamingContext::_narrow(_obj);
	if (CORBA::is_nil(nc.in()))
	{
	    std::cerr << "Invalid Naming Service. Abort" << std::endl;
	    return FALSE;
	}
	CosNaming::Name full(4);
	full.length(4);
	full[0] = factory_id[3];
	full[1] = factory_id[0];
	full[2] = factory_id[2];
	full[3] = factory_id[1];
	CosNaming::Name tmp_n;
	for(CORBA::UShort i = 0;i < full.length();++i)
		if (!strcmp(full[i].id.in(),""))
			continue;
		else
		{
			tmp_n.length(tmp_n.length()+1);
			tmp_n[tmp_n.length()-1] = full[i];
		}

	CosNaming::Name tmp_name;
	for(CORBA::UShort i = 0;i < tmp_n.length();++i)
	{
		CosNaming::NamingContext_var tmp_nc;
		tmp_name.length(tmp_name.length()+1);
		tmp_name[tmp_name.length()-1] = tmp_n[i];
		try {
			if (i == tmp_n.length()-1)
				(*nc).bind(tmp_name,factory);
			else
				tmp_nc = (*nc).bind_new_context(tmp_name);
		} catch (const CosNaming::NamingContext::AlreadyBound& ex) {
			if (i == tmp_n.length()-1)
				return FALSE;
			continue;
		} catch (...) {
			return FALSE;
		}
	}
	return TRUE;
};

CORBA::Boolean
RegisterHelper::trading_register_factory(const CORBA::Object_ptr factory, const CosNaming::Name& factory_id) const
{
    if (CORBA::is_nil(_obj))
    {
	std::cerr << "Can`t resolve Trading Service. Abort" << std::endl;
	return FALSE;
    }
    CosTrading::Lookup_var trader_ = CosTrading::Lookup::_narrow (_obj);
    CosTrading::Register_var reg = trader_->register_if();
    if (CORBA::is_nil(reg))
    {
	std::cerr << "Invalid Trading Service. Abort" << std::endl;
	return FALSE;
    }
    CORBA::Object_var obj = trader_->type_repos();
    CosTradingRepos::ServiceTypeRepository_var repo =
	CosTradingRepos::ServiceTypeRepository::_narrow(obj);
    if (CORBA::is_nil(repo))
    {
	std::cerr << "Invalid Trading Service. Abort" << std::endl;
	return FALSE;
    }

    CORBA::Boolean factory_type = FALSE;
    CosTradingRepos::ServiceTypeRepository::SpecifiedServiceTypes all_;
    all_._d(CosTradingRepos::ServiceTypeRepository::all);
    CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq* list_of_type = 
	repo -> list_types(all_);
    for( CORBA::ULong i = 0;i < list_of_type->length();++i)
    {
	CosTradingRepos::ServiceTypeRepository::TypeStruct* tmp_type =
		repo -> describe_type((*list_of_type)[i]);
	if (!strcmp(factory_id[3].id.in(),(*list_of_type)[i]) &&
	    factory->_is_a(tmp_type->if_name.in()))
		factory_type = true;
	delete tmp_type;
	if (factory_type) break;
    }
    delete list_of_type;
    if (!factory_type)
    {
	CosTradingRepos::ServiceTypeRepository::IncarnationNumber i1;
	{
	    CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq super;
	    super.length( 0 );
	    CosTradingRepos::ServiceTypeRepository::PropStructSeq props;
	    props.length( 3 );
	    props[0].name = CORBA::string_dup( _lc_ks_object_interface );
	    props[0].value_type = CORBA::_tc_string;
	    props[0].mode = CosTradingRepos::ServiceTypeRepository::PROP_MANDATORY;
	    props[1].name = CORBA::string_dup( _lc_ks_impl_equiv_class );
	    props[1].value_type = CORBA::_tc_string;
	    props[1].mode = CosTradingRepos::ServiceTypeRepository::PROP_NORMAL;
	    props[2].name = CORBA::string_dup( _lc_ks_object_implementation );
	    props[2].value_type = CORBA::_tc_string;
	    props[2].mode = CosTradingRepos::ServiceTypeRepository::PROP_NORMAL;

	    CORBA::InterfaceDef_var factory_type = factory->_get_interface();
	    CORBA::InterfaceDef::FullInterfaceDescription_var full_of_factory = factory_type->describe_interface ();
 
	    i1 = repo->add_type( factory_id[3].id.in(), full_of_factory->id.in(), props, super );
	}
    }
    CosTrading::OfferId id1;

    CosTrading::PropertySeq seq;
    seq.length(3);
    seq[0].name = CORBA::string_dup( _lc_ks_object_interface );
    seq[0].value <<= CORBA::Any::from_string( factory_id[0].id, 0 );
    seq[1].name = CORBA::string_dup( _lc_ks_impl_equiv_class );
    seq[1].value <<= CORBA::Any::from_string(factory_id[1].id, 0);
    seq[2].name = CORBA::string_dup( _lc_ks_object_implementation );
    seq[2].value <<= CORBA::Any::from_string(factory_id[2].id, 0);

    id1 = reg->CORBA_CXX_PREFIX(export)(factory, factory_id[3].id.in(), seq );
	return TRUE;
};


