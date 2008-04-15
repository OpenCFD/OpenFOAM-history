#ifndef __proxy_h__
#define __proxy_h__

#include "CosTrading.h"
#include "misc.h"

class Trader;
class Proxy_impl : virtual public TraderComponents, virtual public SupportAttributes,
	      virtual public CosTrading::Proxy_skel
{
public:  
  Proxy_impl( Trader* _trader );
  
#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  virtual void *_narrow_helper( const char *repoid )
  {
      return this->CosTrading::Proxy::_narrow_helper( repoid );
  }
#endif

  char* export_proxy( CosTrading::Lookup_ptr target, const char* type,
		      const CosTrading::PropertySeq& properties, CORBA::Boolean if_match_all,
		      const char* recipe, const CosTrading::PolicySeq& policies_to_pass_on );
  void withdraw_proxy( const char* id );
  CosTrading::Proxy::ProxyInfo* describe_proxy( const char* id );

protected:
  Trader* m_pTrader5;
};

#endif
