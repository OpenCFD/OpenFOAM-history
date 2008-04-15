#include "register.h"
#include "trader_main.h"

Register_impl::Register_impl( Trader *_trader ) : ::TraderComponents( _trader ), ::SupportAttributes( _trader ),
	                                CosTrading::Register_skel()
{
}

char* Register_impl::CORBA_CXX_PREFIX(export) ( CORBA::Object_ptr reference,
                                                const char* type,
                                                const CosTrading::PropertySeq& properties )
{
  if ( CORBA::is_nil( reference ) )
  {
    CosTrading::Register::InvalidObjectRef exc;
    exc.ref = reference;
    mico_throw( exc );
  }
       
  Offer *offer = new Offer;
  offer->vReference = CORBA::Object::_duplicate( reference );
  offer->vType = CORBA::string_dup( type );
  offer->properties = properties;
  offer->isProxy = false;
  
  m_pTrader->export_offer ( offer );

  return CORBA::string_dup( offer->vOfferId.in() );
}

void Register_impl::withdraw( const char* id )
{
  m_pTrader->remove( id );
}

CosTrading::Register::OfferInfo* Register_impl::describe( const char* id )
{
  return m_pTrader->describe( id );
}

void Register_impl::modify( const char* id, const CosTrading::PropertyNameSeq& del_list,
		       const CosTrading::PropertySeq& modify_list )
{
  m_pTrader->modify( id, del_list, modify_list );
}

void Register_impl::withdraw_using_constraint( const char* type, const char* constr )
{
  m_pTrader->withdraw_using_constraint( type, constr );
}

CosTrading::Register_ptr Register_impl::resolve( const CosTrading::TraderName& name )
{
  CosTrading::Register::UnknownTraderName exc;
  exc.name = name;
  mico_throw( exc );
  return CosTrading::Register::_nil();
}

