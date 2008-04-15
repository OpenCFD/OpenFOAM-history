#ifndef __typerepo_impl_h__
#define __typerepo_impl_h__

#include "CosTradingRepos.h"

#include <string>
#include <map>

class Trader;

class TypeRepository_impl : virtual public CosTradingRepos::ServiceTypeRepository_skel
{
public:
  TypeRepository_impl( Trader* _trader );
  
  virtual CosTradingRepos::ServiceTypeRepository::IncarnationNumber incarnation();
  virtual CosTradingRepos::ServiceTypeRepository::IncarnationNumber
          add_type( const char* name,
		    const char* if_name,
		    const CosTradingRepos::ServiceTypeRepository::PropStructSeq& props,
		    const CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq& super_types );
  virtual void remove_type( const char* name );
  virtual CosTradingRepos::ServiceTypeRepository::ServiceTypeNameSeq*
          list_types( const CosTradingRepos::ServiceTypeRepository::SpecifiedServiceTypes& which_types );
  virtual CosTradingRepos::ServiceTypeRepository::TypeStruct* describe_type( const char*name );
  virtual CosTradingRepos::ServiceTypeRepository::TypeStruct* fully_describe_type( const char* name );
  virtual void mask_type( const char* name );
  virtual void unmask_type( const char* name );

  // Extensions
  /**
   * @return true if 'sub' is a subtype of super. This means that one can return 'sub' if
   *         the client wants to have 'super'.
   */
  bool isSubTypeOf( const char* sub, const char* super );
  /**
   * @return true if the requested service type is known in the repository. This function does NOT raise
   *         an exception if the servicetype is unknown like all the others do.
   */
  bool isServiceTypeKnown( const char* name );
  
protected:
  /**
   * Helper function for the recursion.
   */
  void fully_describe_type( CosTradingRepos::ServiceTypeRepository::TypeStruct* result,
			    CosTradingRepos::ServiceTypeRepository::TypeStruct& super_type );
  /**
   * Increases the incarnation number.
   */
  void incIncarnationNumber();
  /**
   * Checks wether a certain service type is registered and returns an iterator
   * pointing at this service type. If the service type does not exist, then
   * the exception @ref CosTading::UnknownServiceType is thrown.
   */
#if 0
  //AP
  map<string,CosTradingRepos::ServiceTypeRepository::TypeStruct>::iterator
          checkServiceType( const char* name );
#else
  std::map<std::string, CosTradingRepos::ServiceTypeRepository::TypeStruct, std::less<std::string> >::iterator
          checkServiceType( const char* name );
#endif
    
  Trader* m_pTrader;

  CosTradingRepos::ServiceTypeRepository::IncarnationNumber m_incarnation;

#if 0
  //AP
  map<string,CosTradingRepos::ServiceTypeRepository::TypeStruct> m_mapTypes;
#else
  std::map<std::string, CosTradingRepos::ServiceTypeRepository::TypeStruct, std::less<std::string> > m_mapTypes;
#endif
};

#endif
