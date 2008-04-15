// -*- c++ -*-
/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (C) 1998 Frank Pilhofer
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
 *  For more information, visit the MICO Home Page at
 *  http://www.mico.org/
 */

#ifndef __POA_IMPL_H__
#define __POA_IMPL_H__

#include <CORBA.h>
#include <mico/impl.h>

#ifdef USE_CSL2
#include <mico/security/odm_impl.h>
#endif /* USE_CSL2  */

/*
 * Template definition for Policy objects must be outside
 * a "struct" (namespace)
 */

template<class T, class V>
class POA_Policy :
  virtual public T,
  virtual public MICO::Policy_impl
{
private:
  V _value;

public:
  POA_Policy (CORBA::PolicyType t, V val)
      : MICO::Policy_impl(t), _value (val) {};
  V value () { return _value; };

  CORBA::Policy_ptr copy ()
  {
    return new POA_Policy<T,V> (policy_type(), _value);
  }
};

/*
 * ----------------------------------------------------------------------
 *
 * All in the MICOPOA Namespace
 *
 * ----------------------------------------------------------------------
 */

namespace MICOPOA {

extern void _init ();

class POA_impl;
class POACurrent_impl;

/*
 * Remove all POA options from the command line so that we can delay
 * the actual POA initialization until a POA is actually needed.
 */

class POAOptions {
public:
  CORBA::Boolean parse (CORBA::ORB_ptr, int &, char *[]);
  const char * operator[] (const char *);
  
private:
  std::map<std::string, std::string, std::less<std::string> > options;
};

extern MICO_EXPORT POAOptions poaopts;

/*
 * ----------------------------------------------------------------------
 *
 * Policy Interfaces
 *
 * ----------------------------------------------------------------------
 */

typedef POA_Policy<PortableServer::ThreadPolicy, PortableServer::ThreadPolicyValue> ThreadPolicy_impl;
typedef POA_Policy<PortableServer::LifespanPolicy, PortableServer::LifespanPolicyValue> LifespanPolicy_impl;
typedef POA_Policy<PortableServer::IdUniquenessPolicy, PortableServer::IdUniquenessPolicyValue> IdUniquenessPolicy_impl;
typedef POA_Policy<PortableServer::IdAssignmentPolicy, PortableServer::IdAssignmentPolicyValue> IdAssignmentPolicy_impl;
typedef POA_Policy<PortableServer::ImplicitActivationPolicy, PortableServer::ImplicitActivationPolicyValue> ImplicitActivationPolicy_impl;
typedef POA_Policy<PortableServer::ServantRetentionPolicy, PortableServer::ServantRetentionPolicyValue> ServantRetentionPolicy_impl;
typedef POA_Policy<PortableServer::RequestProcessingPolicy, PortableServer::RequestProcessingPolicyValue> RequestProcessingPolicy_impl;

/*
 * ----------------------------------------------------------------------
 *
 * POAManager interface
 *
 * ----------------------------------------------------------------------
 */

class POAManager_impl : virtual public PortableServer::POAManager
{
private:
//    typedef vector<PortableServer::POA_ptr> POAPtrVec;

  State _state;

  std::vector<PortableServer::POA_ptr> managed;

  MICOMT::Mutex managed_lock;

  void change_state (State,
		     CORBA::Boolean = FALSE,
		     CORBA::Boolean = FALSE);

public:
  POAManager_impl ();
  virtual ~POAManager_impl ();

  void activate         ();
  void hold_requests    (CORBA::Boolean);
  void discard_requests (CORBA::Boolean);
  void deactivate       (CORBA::Boolean, CORBA::Boolean);
  State get_state       ();

  // begin-mico-extension
  void add_managed_poa  (PortableServer::POA_ptr);
  void del_managed_poa  (PortableServer::POA_ptr);
  // end-mico-extension
};

/*
 * ----------------------------------------------------------------------
 *
 * Helper Classes
 *
 * ----------------------------------------------------------------------
 */

/*
 * Keeps an Object Id as a vector of octets so that we can compare
 * them faster than using a CORBA sequence. We can then use this
 * ObjectId as the key in a STL map, which should be implemented
 * efficiently.
 */

class ObjectId {
public:
  ObjectId ();
  ObjectId (const ObjectId &, bool = true);
  ObjectId (const PortableServer::ObjectId &);
  ObjectId (const char *, CORBA::ULong, bool = true);
  ~ObjectId ();

  ObjectId &     operator=  (const ObjectId &);
  CORBA::Boolean operator== (const ObjectId &);
  bool           operator<  (const ObjectId &) const;

  // get reference
  const char * get_data (CORBA::ULong &) const;
  const PortableServer::ObjectId & get_id ();

  // get copy
  PortableServer::ObjectId * id ();

private:
  bool own;
  char * octets;
  CORBA::ULong idlength;
  PortableServer::ObjectId * oid;
};

/*
 * Generating and examining Object References
 *
 * An Object Reference encapsulates the POA's identity (IP address,
 * PID, fully qualified name) and a unique identifier for the object
 * within that ORB
 */

class POAObjectReference {
public:
  POAObjectReference (POA_impl *,
		      const PortableServer::ObjectId &,
		      const char *,
		      PortableServer::Servant = NULL);
  POAObjectReference (POA_impl *, CORBA::Object_ptr);
  POAObjectReference (const POAObjectReference &);
  ~POAObjectReference ();

  bool is_legal ();
  
  POAObjectReference & operator= (const CORBA::Object_ptr);
  POAObjectReference & operator= (const POAObjectReference &);

  // get reference
  const ObjectId & get_oid ();
  CORBA::Object_ptr get_ref ();
  void set_ref (CORBA::Object_ptr);
  const PortableServer::ObjectId & get_id ();

  // get copy
  CORBA::Object_ptr ref ();
  PortableServer::ObjectId * id ();

  // which POA do we belong to?
  const char * poa_name ();
  bool in_poa (const char *);
  bool in_descendant_poa (const char *, const char *);
  char * next_descendant_poa (const char *, const char *);
  
private:
  MICOMT::Mutex _ref_lock;

  void make_ref ();
  bool decompose_ref ();

  /*
   * Our POA
   */
  
  POA_impl * poa;

  /*
   * Object Identity
   */

  bool iddirty;
  std::string poaname;
  std::string repoid;
  MICOPOA::ObjectId oid;
  CORBA::Object_ptr obj;
  PortableServer::Servant servant;
};

/*
 * Data structure for our Active Object Map.
 *
 * We maintain two maps so that we can search efficiently for Object Ids
 * and Servants.
 */

class ObjectMap {
public:
  struct DeletionRecord {
    CORBA::Long  cnt;
    CORBA::Short signal;
    MICOMT::CondVar condition;

    DeletionRecord(MICOMT::Mutex* m)
      : cnt(0), signal(0), condition(m)
    {}
  };

  struct ObjectRecord {
    ObjectRecord (POAObjectReference *,
		  PortableServer::Servant = NULL);
    ~ObjectRecord ();

    CORBA::Boolean active;
    CORBA::Long invoke_cnt;
    DeletionRecord * delref;

    POAObjectReference * por;
    PortableServer::Servant serv;
  };

  typedef std::map<ObjectId, ObjectRecord *, std::less<ObjectId> > IdMap;
  typedef IdMap::iterator iterator;

public:
  ~ObjectMap ();

  bool empty () const;
  void clear ();

  iterator begin ();
  iterator end ();

  ObjectRecord * pop ();

  ObjectRecord * add (POAObjectReference *, PortableServer::Servant);
  ObjectRecord * del (const ObjectId &);
  ObjectRecord * del (const PortableServer::ObjectId &);

  bool exists (const ObjectId &);
  bool exists (const PortableServer::ObjectId &);
  bool exists (const POAObjectReference &);
  bool exists (PortableServer::Servant);

  ObjectRecord * find (const ObjectId &);
  ObjectRecord * find (const PortableServer::ObjectId &);
  ObjectRecord * find (const POAObjectReference &);
  ObjectRecord * find (POA_impl *, CORBA::Object_ptr);
  ObjectRecord * find (PortableServer::Servant);

private:
  typedef std::map<PortableServer::Servant, std::vector<ObjectRecord *>,
    std::less<PortableServer::Servant> > SvMap;

  IdMap objs;
  SvMap servants;
};

/*
 * Unique Id generator. Can an ULong as Id be too weak? Sure!
 */

class UniqueIdGenerator {
public:
  UniqueIdGenerator ();
  UniqueIdGenerator (const char *);
  ~UniqueIdGenerator ();

  char * new_id ();

  char * state ();
  void state (const char *);

private:
  int ulen, pfxlen;
  char * uid;
  char * prefix;
};

/*
 * ----------------------------------------------------------------------
 *
 * The POA
 *
 * ----------------------------------------------------------------------
 */

/*
 * POA
 */

class POA_impl : public PortableServer::POA,
		 public CORBA::ObjectAdapter
{
public:
  /*
   * InvocationRecord to queue incoming invocations
   */

  class InvocationRecord;
  typedef InvocationRecord *InvocationRecord_ptr;
  typedef ObjVar<InvocationRecord> InvocationRecord_var;
  
  class InvocationRecord : public CORBA::ServerlessObject {
  public:
    InvocationRecord (CORBA::ORBMsgId,
		      POAObjectReference *,
		      CORBA::ORBRequest *,
		      CORBA::Principal_ptr);
    ~InvocationRecord ();

    void exec (POA_impl *);

    CORBA::ORBMsgId id ();
    CORBA::ORBRequest * get_or ();
    POAObjectReference * get_por ();
    CORBA::ServerRequestBase_ptr make_req (POA_impl *,
					   PortableServer::Servant);
    CORBA::ServerRequest_ptr make_dyn_req (POA_impl *);

    static InvocationRecord_ptr _duplicate (InvocationRecord_ptr o)
    {
      if (o)
	o->_ref();
      return o;
    }
    static InvocationRecord_ptr _nil ()
    {
      return 0;
    }

  private:
    CORBA::ORBMsgId orbid;
    POAObjectReference * por;
    CORBA::ORBRequest * req;
    CORBA::Principal_ptr pr;
    CORBA::ServerRequestBase_ptr svr;
  };

private:
  /*
   * POA Policies
   */

  PortableServer::ThreadPolicy_var thread_policy;
  PortableServer::LifespanPolicy_var lifespan_policy;
  PortableServer::IdUniquenessPolicy_var id_uniqueness_policy;
  PortableServer::IdAssignmentPolicy_var id_assignment_policy;
  PortableServer::ImplicitActivationPolicy_var implicit_activation_policy;
  PortableServer::ServantRetentionPolicy_var servant_retention_policy;
  PortableServer::RequestProcessingPolicy_var request_processing_policy;

  /*
   * POA Attributes
   */

  std::string name;            // name relative to parent
  std::string fqn;             // fully qualified name
  std::string oaid;            // Unique Identifier
  static std::string oaprefix; // Prefix for Unique Names

  POA_impl * parent;
  PortableServer::POAManager_ptr manager;
  PortableServer::Servant default_servant;
  PortableServer::ServantManager_var servant_manager;
  static MICOMT::Mutex S_servant_manager_lock;
  PortableServer::AdapterActivator_var adapter_activator;

  /*
   * POAMediator Stuff
   */

  static std::string impl_name;
  static CORBA::IOR poamed_ior;
  static CORBA::POAMediator_var poamed;
  static CORBA::Boolean ever_been_active;

  /*
   * Data
   */

  int destructed;
  MICOMT::RWLock destroy_lock_;
  CORBA::ULong unique_id;
  CORBA::ORB_ptr orb;
  CORBA::IOR* ior_template_;

  ObjectMap ActiveObjectMap;
  MICOMT::Mutex ObjectActivationLock;

  std::vector<InvocationRecord_ptr> InvocationQueue;
  PortableServer::POAManager::State state;

  /*
   * serialization
   */
  MICOMT::Mutex serialize_invoke;
  static MICOMT::Mutex S_global_invoke_lock;

  /*
   * Map of our children
   */

  typedef std::map<std::string, POA_impl *, std::less<std::string> > POAMap;

  void register_child (const char *, POA_impl *);
  void unregister_child (const char *);

  POAMap children;

  /*
   * Map of all POAs
   */

  static void register_poa (const char *, POA_impl *);
  static void unregister_poa (const char *);

  static POAMap AllPOAs;
  static UniqueIdGenerator poauid;
  static UniqueIdGenerator idfactory;

  /*
   * Current data
   */

  static POACurrent_impl * current;

#ifdef USE_CSL2
#ifdef HAVE_SSL // ###ras
  // ODM Manager
  MICOSODM::Manager_impl* odm_manager_;
  // ODM Factory
  MICOSODM::Factory_impl* odm_factory_;
#endif // HAVE_SSL
#endif // USE_CSL2

  /*
   * private ops
   */

  void set_policies (const CORBA::PolicyList &);
  void etherealize ();

  POA_impl * _find_POA (const char *, CORBA::Boolean);

  /*
   * Constructor for my children
   */

  POA_impl (const char *,
	    PortableServer::POAManager_ptr,
	    const CORBA::PolicyList &,
	    POA_impl *,
	    CORBA::ORB_ptr);

  
  /*
   * private object activation and deactivation
   */
  PortableServer::ObjectId * __activate_object (PortableServer::Servant);
  void remove_object (const PortableServer::ObjectId & id);

public:
  POA_impl (CORBA::ORB_ptr);
  virtual ~POA_impl ();

  /*
   * POA creation and destruction
   */

  PortableServer::POA_ptr create_POA (const char *,
				      PortableServer::POAManager_ptr,
				      const CORBA::PolicyList &);
  PortableServer::POA_ptr find_POA   (const char *,
				      CORBA::Boolean);
  void destroy (CORBA::Boolean, CORBA::Boolean);

  /*
   * POA Attributes
   */

  char * the_name ();
  PortableServer::POA_ptr the_parent ();
  PortableServer::POAManager_ptr the_POAManager ();
  PortableServer::AdapterActivator_ptr the_activator ();
  PortableServer::POAList * the_children ();
  void the_activator (PortableServer::AdapterActivator_ptr);
  CORBA::OctetSeq* id();

  /*
   * Factories for Policy objects
   */

  PortableServer::ThreadPolicy_ptr create_thread_policy (PortableServer::ThreadPolicyValue);
  PortableServer::LifespanPolicy_ptr create_lifespan_policy (PortableServer::LifespanPolicyValue);
  PortableServer::IdUniquenessPolicy_ptr create_id_uniqueness_policy (PortableServer::IdUniquenessPolicyValue);
  PortableServer::IdAssignmentPolicy_ptr create_id_assignment_policy (PortableServer::IdAssignmentPolicyValue);
  PortableServer::ImplicitActivationPolicy_ptr create_implicit_activation_policy (PortableServer::ImplicitActivationPolicyValue);
  PortableServer::ServantRetentionPolicy_ptr create_servant_retention_policy (PortableServer::ServantRetentionPolicyValue);
  PortableServer::RequestProcessingPolicy_ptr create_request_processing_policy (PortableServer::RequestProcessingPolicyValue);

  /*
   * ServantManager registration
   */

  PortableServer::ServantManager_ptr get_servant_manager ();
  void set_servant_manager (PortableServer::ServantManager_ptr);

  /*
   * Default servant registration
   */

  PortableServer::Servant get_servant ();
  void set_servant (PortableServer::Servant);

  /*
   * object activation and deactivation
   */

  PortableServer::ObjectId * activate_object (PortableServer::Servant);
  void activate_object_with_id (const PortableServer::ObjectId &,
				PortableServer::Servant);

  void deactivate_object (const PortableServer::ObjectId &);

  /*
   * Reference creation operations
   */

  CORBA::Object_ptr create_reference (const char *);
  CORBA::Object_ptr create_reference_with_id (const PortableServer::ObjectId &,
					      const char *);
  
  /*
   * Perform activation upon _this();
   */

  CORBA::Object_ptr activate_for_this (PortableServer::Servant);

  /*
   * Which IOR to use
   */

  CORBA::IOR * ior_template ();

  /*
   * Identity mapping operations
   */

  PortableServer::ObjectId * servant_to_id (PortableServer::Servant);
  CORBA::Object_ptr servant_to_reference (PortableServer::Servant);
  PortableServer::Servant reference_to_servant (CORBA::Object_ptr);
  PortableServer::ObjectId * reference_to_id (CORBA::Object_ptr);
  PortableServer::Servant id_to_servant (const PortableServer::ObjectId &);
  CORBA::Object_ptr id_to_reference (const PortableServer::ObjectId &);

  /*
   * POA Manager callback
   */

  void poa_manager_callback (PortableServer::POAManager::State,
			     CORBA::Boolean, CORBA::Boolean);

  /*
   * Service for colocated stubs
   */

  PortableServer::Servant preinvoke (CORBA::Object_ptr);
  void postinvoke ();

  /*
   * ObjectAdapter Interface
   */

  const char *   get_oaid   () const;
  CORBA::Boolean has_object (CORBA::Object_ptr);
  CORBA::Boolean is_local   () const;

#ifdef USE_CSL2
  CORBA::Principal_ptr get_principal (CORBA::Object_ptr);
#endif /* USE_CSL2  */

  CORBA::Boolean invoke (CORBA::ORBMsgId,
			 CORBA::Object_ptr,
			 CORBA::ORBRequest *,
			 CORBA::Principal_ptr,
			 CORBA::Boolean = TRUE);
  CORBA::Boolean bind   (CORBA::ORBMsgId, const char *,
			 const CORBA::ORB::ObjectTag &,
			 CORBA::Address *);
  CORBA::Boolean locate (CORBA::ORBMsgId, CORBA::Object_ptr);
  CORBA::Object_ptr skeleton (CORBA::Object_ptr);

  void cancel   (CORBA::ORBMsgId);
  void shutdown (CORBA::Boolean);

  void answer_invoke (CORBA::ORBMsgId, CORBA::Object_ptr,
		      CORBA::ORBRequest *,
		      CORBA::InvokeStatus);

#ifdef USE_CSL2
  // ODM stuff ###ras
  virtual void register_ODM_factory(ObjectDomainMapping::Factory_ptr fry);
  virtual ObjectDomainMapping::Manager_ptr get_ODM();
#endif /* USE_CSL2  */

  /*
   * Helper functions for OA interface
   */

  void builtin_is_a   (InvocationRecord_ptr, PortableServer::Servant);
  void builtin_interface (InvocationRecord_ptr, PortableServer::Servant);
  void builtin_component (InvocationRecord_ptr, PortableServer::Servant);
  void builtin_non_existent (InvocationRecord_ptr, PortableServer::Servant);
  bool builtin_invoke (InvocationRecord_ptr, PortableServer::Servant);

  bool is_builtin     (InvocationRecord_ptr);
  void local_invoke   (InvocationRecord_ptr);
  void perform_invoke (InvocationRecord_ptr);
};

/*
 * ----------------------------------------------------------------------
 *
 * Current Interface
 *
 * ----------------------------------------------------------------------
 */

class POACurrent_impl : public PortableServer::Current
{
private:
  CORBA::ORB_ptr orb;

  struct CurrentState {
    CurrentState ();
    CurrentState (PortableServer::POA_ptr,
		  POAObjectReference *,
		  PortableServer::Servant);
    CurrentState (const CurrentState &);
    ~CurrentState ();
    PortableServer::POA_ptr poa;
    POAObjectReference * por;
    PortableServer::Servant serv;
  };

public:
  typedef std::vector<CurrentState> CurrentStateStack;

private:
#ifndef HAVE_THREADS
  CurrentStateStack* state_stack_;
#else // HAVE_THREADS
  MICOMT::Thread::ThreadKey current_key_;
#endif // HAVE_THREADS

public:
  POACurrent_impl (CORBA::ORB_ptr);
  ~POACurrent_impl ();

  PortableServer::POA_ptr get_POA ();
  PortableServer::ObjectId * get_object_id ();
  CORBA::Object_ptr get_reference();
  PortableServer::Servant get_servant();
  // begin-mico-extension
  CORBA::Boolean iscurrent ();
  POAObjectReference * get_por ();

  void set (PortableServer::POA_ptr, POAObjectReference *,
	    PortableServer::Servant);
  void unset ();

#ifndef HAVE_THREADS
  //
  // In single thread mode, only one POA can be active at any given time
  // a stack of POA/Currents is OK here, because nested invocation are not possible 
  //
  CurrentStateStack* get_current()
  { return state_stack_; }

  void set_current(CurrentStateStack* stack)
  { state_stack_ = stack; }

#else // HAVE_THREADS
  //
  // In multi threaded mode, more than one POA can be active (doing invocations),
  // but there can be only one active Object per thread 
  // 
  CurrentStateStack* get_current()
  { return static_cast<CurrentStateStack *>(MICOMT::Thread::get_specific(current_key_)); }

  void set_current(CurrentStateStack* current)
  { MICOMT::Thread::set_specific(current_key_, current); }

#endif // HAVE_THREADS
  // end-mico-extension
};

}  // namespace MICOPOA

#endif
