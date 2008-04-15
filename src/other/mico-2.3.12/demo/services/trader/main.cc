#include "demo.h"
#include <coss/CosTradingRepos.h>
#include <coss/CosTrading.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif
#include <string>


#define CORBA_CXX_PREFIX(x) _cxx_##x


using namespace std;

CORBA::ORB_var orb;
CORBA::BOA_var boa;

class A_impl : virtual public A_skel
{
public:
  A_impl( const char *_str ) 
  {
    m_str = _str;
  }

  virtual void hello()
  {
    cout << m_str << endl;
  }
  
protected:
  string m_str;
};

class B_impl : virtual public B_skel
{
public:
  B_impl( const char *_str ) 
  {
    m_str = _str;
  }

  void sayHi()
  {
    cout << "Hi from " << m_str << endl;
  }
  
protected:
  string m_str;
};

int main( int argc, char **argv )
{
  orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );
  boa = orb->BOA_init (argc, argv, "mico-local-boa");

  cerr << "Client started" << endl;

  CORBA::Object_var obj = orb->resolve_initial_references( "TradingService" );
  assert( !CORBA::is_nil( obj ) );
  CosTrading::Lookup_var l = CosTrading::Lookup::_narrow( obj );
  assert( !CORBA::is_nil( l ) );
  CosTrading::Register_var r = l->register_if();
  assert( !CORBA::is_nil( r ) );
  obj = l->type_repos();
  assert( !CORBA::is_nil( obj ) );
  CosTradingRepos::ServiceTypeRepository_var repo = CosTradingRepos::ServiceTypeRepository::_narrow( obj );
  assert( !CORBA::is_nil( repo ) );

  cerr << "Found trader" << endl;
  
  CosTradingRepos::ServiceTypeRepository::IncarnationNumber i1;
  {
    CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq super;
    super.length( 0 );
    CosTradingRepos::ServiceTypeRepository::PropStructSeq props;
    props.length( 2 );
    props[0].name = CORBA::string_dup( "Name" );
    props[0].value_type = CORBA::_tc_string;
    props[0].mode = CosTradingRepos::ServiceTypeRepository::PROP_MANDATORY;
    props[1].name = CORBA::string_dup( "Age" );
    props[1].value_type = CORBA::_tc_long;
    props[1].mode = CosTradingRepos::ServiceTypeRepository::PROP_MANDATORY;
    
    i1 = repo->add_type( "HelloClass", "IDL:A:1.0", props, super );
  }

  cerr << "Registered HelloClass" << endl;

  CosTradingRepos::ServiceTypeRepository::IncarnationNumber i2;
  {
    CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq super;
    super.length( 0 );
    CosTradingRepos::ServiceTypeRepository::PropStructSeq props;
    props.length( 3 );
    props[0].name = CORBA::string_dup( "Name" );
    props[0].value_type = CORBA::_tc_string;
    props[0].mode = CosTradingRepos::ServiceTypeRepository::PROP_MANDATORY;
    props[1].name = CORBA::string_dup( "Age" );
    props[1].value_type = CORBA::_tc_long;
    props[1].mode = CosTradingRepos::ServiceTypeRepository::PROP_MANDATORY;
    props[2].name = CORBA::string_dup( "FullName" );
    props[2].value_type = CORBA::_tc_string;
    props[2].mode = CosTradingRepos::ServiceTypeRepository::PROP_NORMAL;

    i2 = repo->add_type( "SayHiClass", "IDL:B:1.0", props, super );
  }

  cerr << "Registered SayHiClass" << endl;
  
  CosTrading::OfferId id1;
  CosTrading::OfferId id2;
  A_var a1 = new A_impl( "Heinz Schmidt" );
  A_var a2 = new A_impl( "Heinz Friedrichs" );
  {
    CosTrading::PropertySeq seq;
    seq.length(2);
    seq[0].name = CORBA::string_dup( "Name" );
    seq[0].value <<= CORBA::Any::from_string( "Heinz", 0 );
    seq[1].name = CORBA::string_dup( "Age" );
    seq[1].value <<= (CORBA::Long)30;    

    id1 = r->CORBA_CXX_PREFIX(export)( a1, "HelloClass", seq );

    seq[0].name = CORBA::string_dup( "Name" );
    seq[0].value <<= CORBA::Any::from_string( "Heinz", 0 );
    seq[1].name = CORBA::string_dup( "Age" );
    seq[1].value <<= (CORBA::Long)50;
    
    id2 = r->CORBA_CXX_PREFIX(export)( a2, "HelloClass", seq );
  }

  CosTrading::OfferId id3;
  CosTrading::OfferId id4;
  CosTrading::OfferId id5;
  B_var b1 = new B_impl( "Heinz Becker" );
  B_var b2 = new B_impl( "Heinz Fritsch" );
  B_var b3 = new B_impl( "Heinz Radgen" );
  {
    CosTrading::PropertySeq seq;
    seq.length(2);
    seq[0].name = CORBA::string_dup( "Name" );
    seq[0].value <<= CORBA::Any::from_string( "Heinz", 0 );
    seq[1].name = CORBA::string_dup( "Age" );
    seq[1].value <<= (CORBA::Long)30;    

    id3 = r->CORBA_CXX_PREFIX(export)( b1, "SayHiClass", seq );

    seq.length(3);
    seq[0].name = CORBA::string_dup( "Name" );
    seq[0].value <<= CORBA::Any::from_string( "Karl-Heinz", 0 );
    seq[1].name = CORBA::string_dup( "Age" );
    seq[1].value <<= (CORBA::Long)50;
    seq[2].name = CORBA::string_dup( "FullName" );
    seq[2].value <<= CORBA::Any::from_string( "Heinz Fritsch", 0 );
    
    id4 = r->CORBA_CXX_PREFIX(export)( b2, "SayHiClass", seq );

    seq.length(2);
    seq[0].name = CORBA::string_dup( "Name" );
    seq[0].value <<= CORBA::Any::from_string( "Heinz", 0 );
    seq[1].name = CORBA::string_dup( "Age" );
    seq[1].value <<= (CORBA::Long)40;    

    id5 = r->CORBA_CXX_PREFIX(export)( b3, "SayHiClass", seq );
  }
  
  cerr << "Exported more Persons" << endl;
  
  {
    CosTrading::ServiceTypeName_var type;
    CosTrading::Constraint_var constr;
    CosTrading::Lookup::Preference_var prefs;
    CosTrading::Lookup::SpecifiedProps desired;
    desired._d( CosTrading::Lookup::all );
    CosTrading::PolicySeq policyseq;
    policyseq.length( 0 );
    constr = CORBA::string_dup( "Name == 'Heinz'" );
    prefs = CORBA::string_dup( "max Age" );
    type = CORBA::string_dup( "HelloClass" );
    CosTrading::OfferSeq* offers = 0L;
    CosTrading::OfferIterator_ptr offer_itr = 0L;
    CosTrading::PolicyNameSeq* limits = 0L;
    l->query( type, constr, prefs, policyseq, desired, 100, offers, offer_itr, limits );

    if ( offers != 0L )
      cout << "Got " <<  offers->length() << "results" << endl;
    else
      cout << "Got no results" << endl;

    int max = offers->length();
    int i;
    for( i = 0; i < max; i++ )
    {
      CosTrading::PropertySeq &p = (*offers)[i].properties;
      int max2 = p.length();
      cout << max2 << " Properties" << endl;
      int j;
      for( j = 0; j < max2; j++ )
      {
	cout << "Property " << (const char*)p[j].name << " = ";
        const char* s;
	CORBA::Long l;
	if ( p[j].value >>= s )
	{    
	  cout << s;
	}
	else if ( p[j].value >>= l )
	  cout << l;
	cout << endl;
      }

      A_var a = A::_narrow( (*offers)[i].reference );
      assert( !CORBA::is_nil( a ) );
      a->hello();
    }
  }

  cerr << "---------------------------------------------------" << endl;
  
  {
    CosTrading::ServiceTypeName_var type;
    CosTrading::Constraint_var constr;
    CosTrading::Lookup::Preference_var prefs;
    CosTrading::Lookup::SpecifiedProps desired;
    desired._d( CosTrading::Lookup::all );
    CosTrading::PolicySeq policyseq;
    policyseq.length( 0 );
    constr = CORBA::string_dup( "'Heinz' ~ Name and Age > 30" );
    prefs = CORBA::string_dup( "min Age" );
    type = CORBA::string_dup( "SayHiClass" );
    CosTrading::OfferSeq* offers = 0L;
    CosTrading::OfferIterator_ptr offer_itr = 0L;
    CosTrading::PolicyNameSeq* limits = 0L;
    l->query( type, constr, prefs, policyseq, desired, 100, offers, offer_itr, limits );

    if ( offers != 0L )
      cout << "Got " <<  offers->length() << "results" << endl;
    else
      cout << "Got no results" << endl;

    int max = offers->length();
    int i;
    for( i = 0; i < max; i++ )
    {
      CosTrading::PropertySeq &p = (*offers)[i].properties;
      int max2 = p.length();
      cout << max2 << " Properties" << endl;
      int j;
      for( j = 0; j < max2; j++ )
      {
	cout << "Property " << (const char*)p[j].name << " = ";
	const char *s;
	CORBA::Long l;
	if ( p[j].value >>= s )
	{    
	  cout << s;
	}
	else if ( p[j].value >>= l )
	  cout << l;
	cout << endl;
      }

      B_var b = B::_narrow( (*offers)[i].reference );
      assert( !CORBA::is_nil( b ) );
      b->sayHi();
    }
  }
  
  cerr << "Done!" << endl;
  
  return 0;
}

