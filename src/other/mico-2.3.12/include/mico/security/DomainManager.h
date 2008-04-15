/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>

#ifndef __DOMAINMANAGER_H__
#define __DOMAINMANAGER_H__


#ifdef _WIN32
#ifdef BUILD_MICO_DLL
#define MICO_EXPORT /**/
#else // BUILD_MICO_DLL
#define MICO_EXPORT __declspec(dllimport)
#endif // BUILD_MICO_DLL
#else // _WIN32
#define MICO_EXPORT /**/
#endif // _WIN32




namespace SecurityDomain
{

class NameIterator;
typedef NameIterator *NameIterator_ptr;
typedef NameIterator_ptr NameIteratorRef;
typedef ObjVar< NameIterator > NameIterator_var;
typedef ObjOut< NameIterator > NameIterator_out;

class DomainManagerAdmin;
typedef DomainManagerAdmin *DomainManagerAdmin_ptr;
typedef DomainManagerAdmin_ptr DomainManagerAdminRef;
typedef ObjVar< DomainManagerAdmin > DomainManagerAdmin_var;
typedef ObjOut< DomainManagerAdmin > DomainManagerAdmin_out;

class DomainManagerFactory;
typedef DomainManagerFactory *DomainManagerFactory_ptr;
typedef DomainManagerFactory_ptr DomainManagerFactoryRef;
typedef ObjVar< DomainManagerFactory > DomainManagerFactory_var;
typedef ObjOut< DomainManagerFactory > DomainManagerFactory_out;

class DomainAuthority;
typedef DomainAuthority *DomainAuthority_ptr;
typedef DomainAuthority_ptr DomainAuthorityRef;
typedef ObjVar< DomainAuthority > DomainAuthority_var;
typedef ObjOut< DomainAuthority > DomainAuthority_out;

class DomainAuthorityAdmin;
typedef DomainAuthorityAdmin *DomainAuthorityAdmin_ptr;
typedef DomainAuthorityAdmin_ptr DomainAuthorityAdminRef;
typedef ObjVar< DomainAuthorityAdmin > DomainAuthorityAdmin_var;
typedef ObjOut< DomainAuthorityAdmin > DomainAuthorityAdmin_out;

class NameExt;
typedef NameExt *NameExt_ptr;
typedef NameExt_ptr NameExtRef;
typedef ObjVar< NameExt > NameExt_var;
typedef ObjOut< NameExt > NameExt_out;

}






namespace SecurityDomain
{

typedef char* Istring;
typedef CORBA::String_var Istring_var;
typedef CORBA::String_out Istring_out;

struct NameComponent;
typedef TVarVar< NameComponent > NameComponent_var;
typedef TVarOut< NameComponent > NameComponent_out;


struct NameComponent {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef NameComponent_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  NameComponent();
  ~NameComponent();
  NameComponent( const NameComponent& s );
  NameComponent& operator=( const NameComponent& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  Istring_var id;
  Istring_var kind;
};

typedef SequenceTmpl< NameComponent,MICO_TID_DEF> Name;
typedef TSeqVar< SequenceTmpl< NameComponent,MICO_TID_DEF> > Name_var;
typedef TSeqOut< SequenceTmpl< NameComponent,MICO_TID_DEF> > Name_out;

typedef SequenceTmpl< Name,MICO_TID_DEF> NameList;
typedef TSeqVar< SequenceTmpl< Name,MICO_TID_DEF> > NameList_var;
typedef TSeqOut< SequenceTmpl< Name,MICO_TID_DEF> > NameList_out;

enum CombinatorSetType {
  Union = 0,
  Intersection,
  Negation,
  FirstFit
};

typedef CombinatorSetType& CombinatorSetType_out;

typedef CombinatorSetType PolicyCombinator;
typedef PolicyCombinator& PolicyCombinator_out;
struct CombinatorInfo;
typedef TFixVar< CombinatorInfo > CombinatorInfo_var;
typedef CombinatorInfo& CombinatorInfo_out;


struct CombinatorInfo {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef CombinatorInfo_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  CombinatorInfo();
  ~CombinatorInfo();
  CombinatorInfo( const CombinatorInfo& s );
  CombinatorInfo& operator=( const CombinatorInfo& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  ::CORBA::PolicyType policy_type;
  CombinatorSetType combinator;
};

typedef SequenceTmpl< CombinatorInfo,MICO_TID_DEF> CombinatorInfoList;
typedef TSeqVar< SequenceTmpl< CombinatorInfo,MICO_TID_DEF> > CombinatorInfoList_var;
typedef TSeqOut< SequenceTmpl< CombinatorInfo,MICO_TID_DEF> > CombinatorInfoList_out;


/*
 * Base class and common definitions for interface NameIterator
 */

class NameIterator : 
  virtual public CORBA::Object
{
  public:
    virtual ~NameIterator();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef NameIterator_ptr _ptr_type;
    typedef NameIterator_var _var_type;
    #endif

    static NameIterator_ptr _narrow( CORBA::Object_ptr obj );
    static NameIterator_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static NameIterator_ptr _duplicate( NameIterator_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static NameIterator_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::Boolean next_one( Name_out domainName ) = 0;
    virtual void destroy() = 0;

  protected:
    NameIterator() {};
  private:
    NameIterator( const NameIterator& );
    void operator=( const NameIterator& );
};


typedef IfaceSequenceTmpl< DomainManagerAdmin_var,DomainManagerAdmin_ptr> DomainManagerAdminList;
typedef TSeqVar< IfaceSequenceTmpl< DomainManagerAdmin_var,DomainManagerAdmin_ptr> > DomainManagerAdminList_var;
typedef TSeqOut< IfaceSequenceTmpl< DomainManagerAdmin_var,DomainManagerAdmin_ptr> > DomainManagerAdminList_out;


/*
 * Base class and common definitions for interface DomainManagerAdmin
 */

class DomainManagerAdmin : 
  virtual public CORBA::Object
{
  public:
    virtual ~DomainManagerAdmin();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef DomainManagerAdmin_ptr _ptr_type;
    typedef DomainManagerAdmin_var _var_type;
    #endif

    static DomainManagerAdmin_ptr _narrow( CORBA::Object_ptr obj );
    static DomainManagerAdmin_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static DomainManagerAdmin_ptr _duplicate( DomainManagerAdmin_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static DomainManagerAdmin_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual void set_domain_policy( ::CORBA::Policy_ptr policy ) = 0;
    virtual ::CORBA::Policy_ptr get_domain_policy( ::CORBA::PolicyType policyType ) = 0;
    virtual void delete_domain_policy( ::CORBA::PolicyType policyType ) = 0;
    virtual Name* get_name() = 0;
    virtual void set_name( const Name& name ) = 0;
    virtual DomainManagerAdminList* get_parent_domain_managers() = 0;
    virtual void set_parent_domain_managers( const DomainManagerAdminList& managers ) = 0;
    virtual DomainManagerAdminList* get_child_domain_managers() = 0;

  protected:
    DomainManagerAdmin() {};
  private:
    DomainManagerAdmin( const DomainManagerAdmin& );
    void operator=( const DomainManagerAdmin& );
};



/*
 * Base class and common definitions for interface DomainManagerFactory
 */

class DomainManagerFactory : 
  virtual public CORBA::Object
{
  public:
    virtual ~DomainManagerFactory();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef DomainManagerFactory_ptr _ptr_type;
    typedef DomainManagerFactory_var _var_type;
    #endif

    static DomainManagerFactory_ptr _narrow( CORBA::Object_ptr obj );
    static DomainManagerFactory_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static DomainManagerFactory_ptr _duplicate( DomainManagerFactory_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static DomainManagerFactory_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual DomainManagerAdmin_ptr get_root_domain_manager( const char* type ) = 0;
    virtual void add_root_domain_manager( const char* type ) = 0;
    virtual DomainManagerAdmin_ptr create_domain_manager() = 0;
    virtual void release_domain_manager( DomainManagerAdmin_ptr domainManager ) = 0;
    virtual CORBA::Boolean load_config_file( const char* filename, const char* type ) = 0;

  protected:
    DomainManagerFactory() {};
  private:
    DomainManagerFactory( const DomainManagerFactory& );
    void operator=( const DomainManagerFactory& );
};



/*
 * Base class and common definitions for interface DomainAuthority
 */

class DomainAuthority : 
  virtual public ::SecurityDomain::DomainManagerAdmin
{
  public:
    virtual ~DomainAuthority();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef DomainAuthority_ptr _ptr_type;
    typedef DomainAuthority_var _var_type;
    #endif

    static DomainAuthority_ptr _narrow( CORBA::Object_ptr obj );
    static DomainAuthority_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static DomainAuthority_ptr _duplicate( DomainAuthority_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static DomainAuthority_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::Boolean is_root() = 0;
    virtual CORBA::Boolean exists() = 0;
    virtual PolicyCombinator get_policy_combinator( ::CORBA::PolicyType policyType ) = 0;
    virtual PolicyCombinator get_policy_combinator_for_multi_parents( ::CORBA::PolicyType policyType, DomainManagerAdmin_ptr parent ) = 0;
    virtual NameIterator_ptr get_child_domain_names() = 0;
    virtual Name* get_domain_name( DomainManagerAdmin_ptr domainManager ) = 0;
    virtual DomainManagerAdmin_ptr get_domain_manager( const Name& domainName ) = 0;

  protected:
    DomainAuthority() {};
  private:
    DomainAuthority( const DomainAuthority& );
    void operator=( const DomainAuthority& );
};



/*
 * Base class and common definitions for interface DomainAuthorityAdmin
 */

class DomainAuthorityAdmin : 
  virtual public ::SecurityDomain::DomainAuthority
{
  public:
    virtual ~DomainAuthorityAdmin();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef DomainAuthorityAdmin_ptr _ptr_type;
    typedef DomainAuthorityAdmin_var _var_type;
    #endif

    static DomainAuthorityAdmin_ptr _narrow( CORBA::Object_ptr obj );
    static DomainAuthorityAdmin_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static DomainAuthorityAdmin_ptr _duplicate( DomainAuthorityAdmin_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static DomainAuthorityAdmin_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual void add_domain_manager( DomainManagerAdmin_ptr domainManager, const Name& domainName ) = 0;
    virtual void remove_domain_manager( DomainManagerAdmin_ptr child ) = 0;
    virtual void set_policy_combinator( ::CORBA::PolicyType policyType, PolicyCombinator combinator ) = 0;
    virtual void set_policy_combinator_for_multi_parents( ::CORBA::PolicyType policyType, PolicyCombinator combinator, DomainManagerAdmin_ptr parent ) = 0;

  protected:
    DomainAuthorityAdmin() {};
  private:
    DomainAuthorityAdmin( const DomainAuthorityAdmin& );
    void operator=( const DomainAuthorityAdmin& );
};



/*
 * Base class and common definitions for interface NameExt
 */

class NameExt : 
  virtual public CORBA::Object
{
  public:
    virtual ~NameExt();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef NameExt_ptr _ptr_type;
    typedef NameExt_var _var_type;
    #endif

    static NameExt_ptr _narrow( CORBA::Object_ptr obj );
    static NameExt_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static NameExt_ptr _duplicate( NameExt_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static NameExt_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual char* to_string( const Name& domainName ) = 0;
    virtual Name* to_name( const char* urlName ) = 0;

  protected:
    NameExt() {};
  private:
    NameExt( const NameExt& );
    void operator=( const NameExt& );
};


}


#ifndef MICO_CONF_NO_POA

#endif // MICO_CONF_NO_POA

#endif
