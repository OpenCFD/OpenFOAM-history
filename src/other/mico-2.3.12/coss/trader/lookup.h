#ifndef __lookup_h__
#define __lookup_h__

#include "misc.h"
#include "CosTrading.h"

class Trader;

class Lookup_impl : virtual public TraderComponents,
		    virtual public SupportAttributes,
		    virtual public ImportAttributes,
		    virtual public CosTrading::Lookup_skel
{
public:
  Lookup_impl( Trader *_trader, CORBA::Object_ptr _obj );
  Lookup_impl( Trader *_trader, const CORBA::BOA::ReferenceData &tag );

#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  virtual void *_narrow_helper( const char *repoid )
  {
      return this->CosTrading::Lookup::_narrow_helper( repoid );
  }
#endif
  
  virtual void query( const char* type, const char* constr, const char* pref, const CosTrading::PolicySeq& policies,
		      const CosTrading::Lookup::SpecifiedProps& desired_props, CORBA::ULong how_many,
		      CosTrading::OfferSeq_out offers, CosTrading::OfferIterator_out offer_itr,
		      CosTrading::PolicyNameSeq_out limits_applied );
};

#endif
