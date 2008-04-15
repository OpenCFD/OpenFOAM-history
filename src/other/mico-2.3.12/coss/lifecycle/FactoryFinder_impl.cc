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
#include <coss/CosTradingRepos.h>
#include <coss/FactoryFinder_impl.h>
#include <string>

using std::string;

FactoryFinder_impl::FactoryFinder_impl()
    : POA_CosLifeCycle::FactoryFinder ()
{
    orb_ = CORBA::ORB::_nil();
    trader_ = CosTrading::Lookup::_nil();
	naming_ = CosNaming::NamingContext::_nil();
}

FactoryFinder_impl::FactoryFinder_impl(CORBA::ORB_ptr orb, RegisterHelper::FinderType type)
    : POA_CosLifeCycle::FactoryFinder ()
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
			break;
		}
	}
}

FactoryFinder_impl::FactoryFinder_impl(CORBA::ORB_ptr orb, CosTrading::Lookup_ptr trader)
    : POA_CosLifeCycle::FactoryFinder ()
{
    orb_ = CORBA::ORB::_duplicate(orb);
    trader_ = CosTrading::Lookup::_duplicate(trader);
	naming_ = CosNaming::NamingContext::_nil();
}

FactoryFinder_impl::FactoryFinder_impl(CORBA::ORB_ptr orb, CosNaming::NamingContext_ptr naming)
    : POA_CosLifeCycle::FactoryFinder ()
{
    orb_ = CORBA::ORB::_duplicate(orb);
    naming_ = CosNaming::NamingContext::_duplicate(naming);
    trader_ = CosTrading::Lookup::_nil();
}

FactoryFinder_impl::~FactoryFinder_impl()
{
}

CosLifeCycle::Factories*
FactoryFinder_impl::find_factories(const CosLifeCycle::Key& factory_key)
{
	if (!CORBA::is_nil(naming_.in()))
		return nfind_factories(factory_key);
	else if (!CORBA::is_nil(trader_.in()))
		return tfind_factories(factory_key);
	else
		return NULL;
}

CosLifeCycle::Factories*
FactoryFinder_impl::nfind_factories(const CosLifeCycle::Key& factory_key, CosNaming::NamingContext_ptr nc)
{
	if (CORBA::is_nil(nc))
		nc = naming_.in();

	if (!strcmp(factory_key[3].id.in(),""))
		return NULL;

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
			continue;
		else
		{
			tmp_n.length(tmp_n.length()+1);
			tmp_n[tmp_n.length()-1] = full[i];
		}
	try {
		obj = (*nc).resolve(tmp_n);
	} catch (...) {
		return result;
	}
	CosNaming::NamingContext_var tmp_nc = CosNaming::NamingContext::_narrow(obj);
	if (CORBA::is_nil(tmp_nc.in()))
	{
		(*result).length((*result).length()+1);
		(*result)[(*result).length()-1] = obj;
		return result;
	}
	CosNaming::BindingIterator_ptr bi;
	CosNaming::BindingList* bl;
	(*tmp_nc).list(0,bl,bi);
	CosNaming::Binding* bg;
	while((*bi).next_one(bg))
	{
		if ((*bg).binding_type == CosNaming::nobject)
		{
			(*result).length((*result).length()+1);
			try {
				(*result)[(*result).length()-1] = (*tmp_nc).resolve((*bg).binding_name);
			} catch (...) {}
		}
		else
		{
			CosLifeCycle::Factories* tmp_res = this->nfind_factories((*bg).binding_name,tmp_nc.in());
			for(CORBA::UShort i = 0;i < (*tmp_res).length();++i)
			{
				(*result).length((*result).length()+1);
				(*result)[(*result).length()-1] = (*tmp_res)[i];
			}
		}
		delete bg;
	}
	return result;

}

CosLifeCycle::Factories*
FactoryFinder_impl::tfind_factories(const CosLifeCycle::Key& factory_key)
{
	if (!strcmp(factory_key[3].id.in(),""))
		return NULL;

    CosLifeCycle::Factories* result = new CosLifeCycle::Factories;

    CosTrading::TypeRepository_var tr_obj = trader_ -> type_repos();
    CosTradingRepos::ServiceTypeRepository_var type_repo =
	    CosTradingRepos::ServiceTypeRepository::_narrow(tr_obj);
    CosTradingRepos::ServiceTypeRepository::SpecifiedServiceTypes all_;
    all_._d(CosTradingRepos::ServiceTypeRepository::all);
    CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq* list_of_type = 
	    type_repo -> list_types(all_);
    string filter;
    string type_;
    CORBA::UShort count=0;
    for(CORBA::ULong j = 0;j < factory_key.length();++j)
    {
	if (!strcmp(factory_key[j].kind,_lc_ks_object_interface) &&
	    strlen(factory_key[j].id.in()))
	{
	    ++count;
	    if (filter.size())
	    {
		if (count>2)
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
		if (count>2)
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
		if (count>2)
		    filter += ")";
		filter += " and ";
	    }
	    filter += "'";
	    filter += factory_key[j].id.in();
	    filter += "' ~ ";
		filter += _lc_ks_object_implementation;
	}
	if (!strcmp(factory_key[j].kind,_lc_ks_factory_interface))
	    type_ += factory_key[j].id.in();
    }
    if (count > 2)
    {
	string add_;
	for(CORBA::UShort i = 2;i < count;++i)
	    add_ += "(";
	add_ += filter;
	filter = add_;
    }
    for( CORBA::ULong i = 0;i < list_of_type->length();++i)
	if (!strcmp(type_.c_str(),(*list_of_type)[i]))
	{
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

	    trader_->query ((*list_of_type)[i],
			    filter.c_str(),
			    "",
			    policySeq,
			    specifiedProps,
			    100,
			    offerSeq_ptr,
			    offerIterator_ptr,
			    policyNameSeq_ptr);

	    if (policyNameSeq_ptr) delete policyNameSeq_ptr;
	    // Check if an offer was made
	    if (offerSeq_ptr != NULL)
	    {
		// Insert the pointer into the out class
		CosTrading::OfferSeq_var offerSeq_var(offerSeq_ptr);
	  
		// We need at least one offer.
		CORBA::ULong len_ = (*result).length();
		(*result).length(len_+offerSeq_var->length());
		for (CORBA::ULong k = 0;k < offerSeq_var->length();++k)
		    // now we are all set to read from the sequence the result
		    (*result)[len_+k] = CORBA::Object::_duplicate(offerSeq_var[k].reference.in());

	    }
	    if (!CORBA::is_nil(offerIterator_ptr))
	    {
		CosTrading::OfferIterator_var offerIterator(offerIterator_ptr);
		CosTrading::OfferSeq* tmp_offers;
		if (!offerIterator->next_n(offerIterator->max_left(),tmp_offers))
		    continue;
		CORBA::ULong len_ = (*result).length();
		(*result).length(len_+(*tmp_offers).length());
		for(CORBA::ULong k = 0;k < (*tmp_offers).length();++k)
		    (*result)[len_+k] = CORBA::Object::_duplicate((*tmp_offers)[k].reference.in());
	    }
	}
    delete list_of_type;
    return result;
}

