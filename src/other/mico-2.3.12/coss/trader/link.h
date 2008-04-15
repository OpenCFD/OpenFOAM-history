#ifndef __trader_link_h__
#define __trader_link_h__

// class Link;
// class Trader;

#include "misc.h"
#include "CosTrading.h"

#if 0
//AP
#include <map>
#include <string>
#endif

class Link_impl :  virtual public TraderComponents,
		   virtual public SupportAttributes,
		   virtual public LinkAttributes,
		   virtual public CosTrading::Link_skel
{
public:
  Link_impl( Trader* trader );
  
#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  virtual void *_narrow_helper( const char *repoid )
  {
      return this->CosTrading::Link::_narrow_helper( repoid );
  }
#endif

  virtual void add_link( const char* name, CosTrading::Lookup_ptr target, CosTrading::FollowOption def_pass_on_follow_rule,
			 CosTrading::FollowOption limiting_follow_rule );
  virtual void remove_link( const char* name );
  virtual CosTrading::Link::LinkInfo* describe_link( const char* name );
  virtual CosTrading::LinkNameSeq* list_links();
  virtual void modify_link( const char* name, CosTrading::FollowOption def_pass_on_follow_rule,
			    CosTrading::FollowOption limiting_follow_rule );

  /////////////////
  // Extensions
  /////////////////
  /**
   * This iterator allows the trader to traverse all linked traders.
   */
  std::map<std::string, CosTrading::Link::LinkInfo, std::less<std::string> >::iterator begin()
    { return m_mapLinks.begin(); }
  std::map<std::string, CosTrading::Link::LinkInfo, std::less<std::string> >::iterator end()
    { return m_mapLinks.end(); }
  
protected:
  std::map<std::string, CosTrading::Link::LinkInfo, std::less<std::string> > m_mapLinks;
};

#endif
