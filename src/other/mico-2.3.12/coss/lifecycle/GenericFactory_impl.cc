/*
 *  LifeCycle Service for MICO
 *  Copyright (C) 1999 Dmitry V. Sedov
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  Send comments and/or bug reports to:
 *                 mico@informatik.uni-frankfurt.de
 *  or to my private e-mail:
 *                 sedov@postman.simcb.ru
 */

#include <CORBA.h>
#include <coss/GenericFactory_impl.h>

using std::string;

GenericFactory_impl::GenericFactory_impl (void)
    : POA_CosLifeCycle::GenericFactory ()
{
    orb_ = CORBA::ORB::_nil();
    trader_ = CosTrading::Lookup::_nil();
}

GenericFactory_impl::GenericFactory_impl (CORBA::ORB_ptr orb, RegisterHelper::FinderType type)
    : POA_CosLifeCycle::GenericFactory ()
{
	trader_ = CosTrading::Lookup::_nil();
	naming_ = CosNaming::NamingContext::_nil();
    orb_ = CORBA::ORB::_duplicate(orb);
	switch (type)
	{
		case RegisterHelper::none:
		{
			break;
		}
		case RegisterHelper::naming:
		{
		    CORBA::Object_var nm_obj;
#ifdef HAVE_EXCEPTIONS
			try {
#endif
					nm_obj = orb -> resolve_initial_references("NameService");
#ifdef HAVE_EXCEPTIONS
			} catch(CORBA::ORB::InvalidName&) {
			    std::cerr << "Can`t resolve Naming Service. Abort" << std::endl;
				exit(1);
			}
#endif
			if (CORBA::is_nil(nm_obj))
			{
			    std::cerr << "Can`t resolve Naming Service. Abort" << std::endl;
				exit(1);
			}
			naming_ = CosNaming::NamingContext::_narrow (nm_obj);
			break;
		}
		case RegisterHelper::trading:
		{
			CORBA::Object_var tr_obj;
#ifdef HAVE_EXCEPTIONS
			try {
#endif
				tr_obj = orb -> resolve_initial_references("TradingService");
#ifdef HAVE_EXCEPTIONS
			} catch(CORBA::ORB::InvalidName&) {
			    std::cerr << "Can`t resolve Trading Service. Abort" << std::endl;
				exit(1);
			}
#endif
			if (CORBA::is_nil(tr_obj))
			{
			    std::cerr << "Can`t resolve Trading Service. Abort" << std::endl;
				exit(1);
			}
			trader_ = CosTrading::Lookup::_narrow (tr_obj);
		}
	}
}

GenericFactory_impl::GenericFactory_impl (CORBA::ORB_ptr orb, CosTrading::Lookup_ptr trader)
    : POA_CosLifeCycle::GenericFactory ()
{
    orb_ = CORBA::ORB::_duplicate(orb);
    trader_ = CosTrading::Lookup::_duplicate(trader);
    naming_ = CosNaming::NamingContext::_nil();
}

GenericFactory_impl::GenericFactory_impl (CORBA::ORB_ptr orb, CosNaming::NamingContext_ptr naming)
    : POA_CosLifeCycle::GenericFactory ()
{
    orb_ = CORBA::ORB::_duplicate(orb);
    naming_ = CosNaming::NamingContext::_duplicate(naming);
    trader_ = CosTrading::Lookup::_nil();
}

GenericFactory_impl::~GenericFactory_impl (void)
{
}

CORBA::Boolean
GenericFactory_impl::support (const CosLifeCycle::Key &factory_key)
{
	if (!CORBA::is_nil(naming_.in()))
		return nsupport(factory_key);
	else if (!CORBA::is_nil(trader_.in()))
		return tsupport(factory_key);
	else
		return FALSE;

}

CORBA::Object_ptr
GenericFactory_impl::create_object (const CosLifeCycle::Key &factory_key, 
                                            const CosLifeCycle::Criteria &the_criteria)
{
	if (!CORBA::is_nil(naming_.in()))
		return ncreate_object(factory_key,the_criteria);
	else if (!CORBA::is_nil(trader_.in()))
		return tcreate_object(factory_key,the_criteria);
	else
		return CORBA::Object::_nil();

}

CORBA::Boolean
GenericFactory_impl::nsupport (const CosLifeCycle::Key &factory_key)
{
	if (!strcmp(factory_key[3].id.in(),""))
		return FALSE;

	CosNaming::Name full(4);
	full.length(4);
	full[0] = factory_key[3];
	full[1] = factory_key[0];
	full[2] = factory_key[2];
	full[3] = factory_key[1];
	CosNaming::Name tmp_n;
	CORBA::Object_ptr obj;
	for(CORBA::UShort i = 0;i < full.length();++i)
		if (!strcmp(full[i].id.in(),""))
			break;
		else
		{
			tmp_n.length(tmp_n.length()+1);
			tmp_n[tmp_n.length()-1] = full[i];

		}
	try {
		obj = (*naming_).resolve(tmp_n);
	} catch (...) {
		return FALSE;
	}
	return TRUE;
}

CORBA::Boolean
GenericFactory_impl::tsupport (const CosLifeCycle::Key &factory_key)
{
	if (!strcmp(factory_key[3].id.in(),""))
		return FALSE;

    CosTrading::TypeRepository_var tr_obj = trader_ -> type_repos();
    CosTradingRepos::ServiceTypeRepository_var type_repo =
	    CosTradingRepos::ServiceTypeRepository::_narrow(tr_obj);
    CosTradingRepos::ServiceTypeRepository::SpecifiedServiceTypes all_;
    all_._d(CosTradingRepos::ServiceTypeRepository::all);
    CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq* list_of_type = 
	    type_repo -> list_types(all_);
    CORBA::Boolean support_ = FALSE;
    CORBA::String_var type_name;
    CORBA::ULong i;
    for(i = 0;i < list_of_type->length();++i)
    {
	if (!strcmp("GenericFactory",(*list_of_type)[i]))
	{
	    type_name = CORBA::string_dup((*list_of_type)[i]);
	    break;
	}
	if (type_name.in())
	    break;
    }
    if (i == list_of_type->length())
    {
	if (list_of_type) delete list_of_type;
	return FALSE;
    }
    string filter;
    CORBA::UShort count = 0;
    for(CORBA::ULong j = 0;j < factory_key.length();++j)
    {
	if (!strcmp(factory_key[j].kind,_lc_ks_object_interface) &&
	    strlen(factory_key[j].id.in()))
	{
	    ++count;
	    if (filter.size())
	    {
		if (count > 2)
		    filter += ")";
		filter += " and ";
	    }
	    filter += "'";
	    filter += factory_key[j].id.in();
	    filter += "' ~ ";
		filter += _lc_ks_object_interface;
	}
	if (!strcmp(factory_key[j].kind,_lc_ks_impl_equiv_class) &&
	    strlen(factory_key[j].id.in()))
	{
	    ++count;
	    if (filter.size())
	    {
		if (count > 2)
		    filter += ")";
		filter += " and ";
	    }
	    filter += "'";
	    filter += factory_key[j].id.in();
	    filter += "' ~ ";
		filter += _lc_ks_impl_equiv_class;
	}
	if (!strcmp(factory_key[j].kind,_lc_ks_object_implementation) &&
	    strlen(factory_key[j].id.in()))
	{
	    ++count;
	    if (filter.size())
	    {
		if (count > 2)
		    filter += ")";
		filter += " and ";
	    }
	    filter += "'";
	    filter += factory_key[j].id.in();
	    filter += "' ~ ";
		filter += _lc_ks_object_implementation;
	}
    }
    if (count > 2)
    {
	string add_;
	for(CORBA::UShort j = 2;j < count;++j)
	    add_ += "(";
	add_ += filter;
	filter = add_;
    }
    
    CosTrading::Lookup::SpecifiedProps specifiedProps;
    specifiedProps._default();
    specifiedProps._d(CosTrading::Lookup::all);

    // Get some pointers for the out parameters of the call.
    CosTrading::OfferSeq* offerSeq_ptr = NULL;
    CosTrading::OfferIterator* offerIterator_ptr = NULL;
    CosTrading::PolicyNameSeq* policyNameSeq_ptr = NULL;

    // An empty policy sequence
    CosTrading::PolicySeq policySeq;
    policySeq.length(0);

    CORBA::Object_ptr genericFactoryObj_ptr = NULL;
    trader_->query (type_name,
		    filter.c_str(),
		    "",
		    policySeq,
		    specifiedProps,
		    1,
		    offerSeq_ptr,
		    offerIterator_ptr,
		    policyNameSeq_ptr);

    if (policyNameSeq_ptr) delete policyNameSeq_ptr;
    // Check if an offer was made
    if (offerSeq_ptr != NULL)
	support_ = TRUE;
    else
	support_ = FALSE;
    if (offerSeq_ptr) delete offerSeq_ptr;
    if (offerIterator_ptr) delete offerIterator_ptr;
    if (list_of_type) delete list_of_type;
    return support_;
}

CORBA::Object_ptr
GenericFactory_impl::ncreate_object (const CosLifeCycle::Key &factory_key, 
									 const CosLifeCycle::Criteria &the_criteria)
{
	if (!strcmp(factory_key[3].id.in(),""))
		return CORBA::Object::_nil();
    CosLifeCycle::Factories* result = new CosLifeCycle::Factories;
	CosNaming::Name full(4);
	full.length(4);
	full[0] = factory_key[3];
	full[1] = factory_key[0];
	full[2] = factory_key[2];
	full[3] = factory_key[1];
	CosNaming::Name tmp_n;
	CORBA::Object_ptr obj;
	for(CORBA::UShort i = 0;i < full.length();++i)
		if (!strcmp(full[i].id.in(),""))
			break;
		else
		{
			tmp_n.length(tmp_n.length()+1);
			tmp_n[tmp_n.length()-1] = full[i];
		}
	try {
		obj = (*naming_).resolve(tmp_n);
	} catch (...) {
		return CORBA::Object::_nil();
	}
	CosNaming::NamingContext_var tmp_nc = CosNaming::NamingContext::_narrow(obj);
	if (CORBA::is_nil(tmp_nc.in()))
	{
		CosLifeCycle::GenericFactory_var gfac = CosLifeCycle::GenericFactory::_narrow(obj);
		if (CORBA::is_nil(gfac.in()))
			return CORBA::Object::_nil();
		else
		{
			CORBA::Object_ptr object_ptr = (*gfac).create_object(factory_key,the_criteria);
			if (CORBA::is_nil (object_ptr))
				return CORBA::Object::_nil();
			else
				return object_ptr;
		}
	}

	CosNaming::BindingIterator_ptr bi;
	CosNaming::BindingList* bl;
	(*tmp_nc).list(0,bl,bi);
	CORBA::Object_ptr genericFactoryObj_ptr(CORBA::Object::_nil());
	CosNaming::Binding* bg;
	while((*bi).next_one(bg))
	{
		if ((*bg).binding_type == CosNaming::nobject)
		{
			try {
				genericFactoryObj_ptr = (*naming_).resolve((*bg).binding_name);
			} catch (...) {
				delete bg;
				continue;
			}

			if (CORBA::is_nil (genericFactoryObj_ptr))
			{
				delete bg;
				continue;
			}
			CosLifeCycle::GenericFactory_var genericFactory_var = 
					CosLifeCycle::GenericFactory::_narrow (genericFactoryObj_ptr);
			// Check if it is a valid Generic Factory reference
			if (CORBA::is_nil (genericFactory_var.in()))
			{
				delete bg;
				continue;
			}
			// Now we have a proper reference to a Generic Factory
			// the create_object call will be forwarded to this factory
			CORBA::Object_ptr object_ptr =
					genericFactory_var->create_object (factory_key,the_criteria);
			// Now retrieve the Object obj ref corresponding to the key.
			if (CORBA::is_nil (object_ptr))
			{
				delete bg;
				continue;
			}
			delete bg;
			return object_ptr;

		}
		else
		{
			delete bg;
			return this->ncreate_object((*bg).binding_name,the_criteria);
		}
		delete bg;
	}
	return CORBA::Object::_nil();
}

CORBA::Object_ptr
GenericFactory_impl::tcreate_object (const CosLifeCycle::Key &factory_key, 
									 const CosLifeCycle::Criteria &the_criteria)
{
  // Exceptions are forwarded, not handled !!
	if (!strcmp(factory_key[3].id.in(),""))
		return CORBA::Object::_nil();

    const char* filter_ = NULL;
    const char* preferences = NULL;
    CORBA::ULong i;
    for(i = 0;i < the_criteria.length();++i)
	if (!strcmp(the_criteria[i].name,"filter"))
	{
	    the_criteria[i].value >>= filter_;
	    break;
	}
	
    for(i = 0;i < the_criteria.length();++i)
	if (!strcmp(the_criteria[i].name,"preferences"))
	{
	    the_criteria[i].value >>= preferences;
	    break;
	}

    CosTrading::TypeRepository_var tr_obj = trader_ -> type_repos();
    CosTradingRepos::ServiceTypeRepository_var type_repo =
	    CosTradingRepos::ServiceTypeRepository::_narrow(tr_obj);
    CosTradingRepos::ServiceTypeRepository::SpecifiedServiceTypes all_;
    all_._default();
    all_._d(CosTradingRepos::ServiceTypeRepository::all);
    CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq* list_of_type = 
	    type_repo -> list_types(all_);
    CORBA::String_var type_name;
    for(i = 0;i < list_of_type->length();++i)
    {
	if (!strcmp("GenericFactory",(*list_of_type)[i]))
	{
	    type_name = CORBA::string_dup((*list_of_type)[i]);
	    break;
	}
	if (type_name.in())
	    break;
    }
    if (i == list_of_type->length())
    {
	if (list_of_type) delete list_of_type;
	return (CORBA::Object_ptr)NULL;
    }
    if (list_of_type) delete list_of_type;

    string filter;
    CORBA::UShort count = 0;
    for(CORBA::ULong j = 0;j < factory_key.length();++j)
    {
	if (!strcmp(factory_key[j].kind,_lc_ks_object_interface) &&
	    strlen(factory_key[j].id.in()))
	{
	    ++count;
	    if (filter.size())
	    {
		if (count > 2)
		    filter += ")";
		filter += " and ";
	    }
	    filter += "'";
	    filter += factory_key[j].id.in();
	    filter += "' ~ ";
		filter += _lc_ks_object_interface;
	}
	if (!strcmp(factory_key[j].kind,_lc_ks_impl_equiv_class) &&
	    strlen(factory_key[j].id.in()))
	{
	    ++count;
	    if (filter.size())
	    {
		if (count > 2)
		    filter += ")";
		filter += " and ";
	    }
	    filter += "'";
	    filter += factory_key[j].id.in();
	    filter += "' ~ ";
		filter += _lc_ks_impl_equiv_class;
	}
	if (!strcmp(factory_key[j].kind,_lc_ks_object_implementation) &&
	    strlen(factory_key[j].id.in()))
	{
	    ++count;
	    if (filter.size())
	    {
		if (count > 2)
		    filter += ")";
		filter += " and ";
	    }
	    filter += "'";
	    filter += factory_key[j].id.in();
	    filter += "' ~ ";
		filter += _lc_ks_object_implementation;
	}
    }
    if (count > 2)
    {
	string add_;
	for(CORBA::UShort j = 2;j < count;++j)
	    add_ += "(";
	add_ += filter;
	filter = add_;
    }
    if (filter.size() && filter_ != NULL)
    {
	string add_;
	add_ += "(";
	add_ += filter;
	add_ += ") and ";
	add_ += filter_;
	filter = add_;
    }
    
    CosTrading::Lookup::SpecifiedProps specifiedProps;
    specifiedProps._default();
    specifiedProps._d(CosTrading::Lookup::all);

    // Get some pointers for the out parameters of the call.
    CosTrading::OfferSeq* offerSeq_ptr = NULL;
    CosTrading::OfferIterator* offerIterator_ptr = NULL;
    CosTrading::PolicyNameSeq* policyNameSeq_ptr = NULL;

    // An empty policy sequence
    CosTrading::PolicySeq policySeq;
    policySeq.length(0);

    CORBA::Object_ptr genericFactoryObj_ptr = NULL;
    trader_->query (type_name,
		    filter.c_str(),
		    preferences,
		    policySeq,
		    specifiedProps,
		    0,
		    offerSeq_ptr,
		    offerIterator_ptr,
		    policyNameSeq_ptr);

    if (policyNameSeq_ptr) delete policyNameSeq_ptr;
    // Check if an offer was made
    if (( CORBA::is_nil(offerIterator_ptr) ) || ( !offerIterator_ptr->max_left() ))
    {
	CosLifeCycle::NoFactory ex;
	ex.search_key = factory_key;
	mico_throw(ex);
	return (CORBA::Object_ptr) NULL;
    }
    offerIterator_ptr->next_n(1,offerSeq_ptr);
    do
    {
	if (offerSeq_ptr != NULL)
	{
	// Insert the pointer into the out class
	    CosTrading::OfferSeq_var offerSeq_var(offerSeq_ptr);
	  
	// We need at least one offer.
	    for(CORBA::ULong i = 0;i < offerSeq_var->length();++i)
	    {
	    // now we are all set to read from the sequence the result
		genericFactoryObj_ptr = CORBA::Object::_duplicate (offerSeq_var[(CORBA::ULong)0].reference.in());

		if (CORBA::is_nil (genericFactoryObj_ptr))
		    continue;
		CosLifeCycle::GenericFactory_var genericFactory_var = 
			CosLifeCycle::GenericFactory::_narrow (genericFactoryObj_ptr);
		// Check if it is a valid Generic Factory reference
		if (CORBA::is_nil (genericFactory_var.in()))
		    continue;
		// Now we have a proper reference to a Generic Factory
		// the create_object call will be forwarded to this factory
		CORBA::Object_var object_var =
		    genericFactory_var->create_object (factory_key,
						       the_criteria);
		// Now retrieve the Object obj ref corresponding to the key.
		if (CORBA::is_nil (object_var.in()))
		    continue;
		return CORBA::Object::_duplicate (object_var.in());
	    }
	}
	else
	    continue;
    }
    while (offerIterator_ptr->next_n(1,offerSeq_ptr));
    CosLifeCycle::NoFactory ex;
    ex.search_key = factory_key;
    mico_throw(ex);
    return (CORBA::Object_ptr) NULL;
}
