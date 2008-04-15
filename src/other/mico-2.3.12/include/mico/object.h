// -*- c++ -*-
/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
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

#ifndef __mico_object_h__
#define __mico_object_h__

namespace CORBA {

// base class for all serverless objects
class ServerlessObject : public MagicChecker {
    Long refs;
    MICOMT::Mutex refslock;
public:
    virtual ~ServerlessObject ();

    void _ref ();
    Boolean _deref ();
    Long _refcnt () const;

    static ServerlessObject* _nil ()
    {
	return 0;
    }
    static ServerlessObject* _duplicate (ServerlessObject *o)
    {
	if (o)
	    o->_ref();
	return o;
    }
protected:
    ServerlessObject ()
    { refs = 1; }

    ServerlessObject (const ServerlessObject &so)
        : CORBA::MagicChecker(so)
    { refs = 1; }

    ServerlessObject &operator= (const ServerlessObject &)
    { return *this; }
};

typedef ServerlessObject *ServerlessObject_ptr;
typedef ObjVar<ServerlessObject> ServerlessObject_var;
typedef ObjOut<ServerlessObject> ServerlessObject_out;

static inline void release (ServerlessObject *o)
{
    if (o && o->_deref())
	delete o;
}

static inline Boolean is_nil (ServerlessObject *o)
{
    return !o;
}


// forward declarations from policy.idl
class Policy;
typedef Policy *Policy_ptr;
typedef ObjVar<Policy> Policy_var;
typedef ObjOut<Policy> Policy_out;
typedef IfaceSequenceTmpl<CORBA::Policy_var,CORBA::Policy_ptr> PolicyList;

class DomainManager;
typedef DomainManager *DomainManager_ptr;
typedef ObjVar<DomainManager> DomainManager_var;
typedef ObjOut<DomainManager> DomainManager_out;
typedef IfaceSequenceTmpl<CORBA::DomainManager_var,CORBA::DomainManager_ptr> DomainManagerList;

typedef CORBA::ULong PolicyType;

enum SetOverrideType {
    SET_OVERRIDE,
    ADD_OVERRIDE
};

// object, base class for all serverbased objects
class Object : public ServerlessObject {
    IOR *ior;
    IOR *fwd_ior;
    ORB_ptr orb;
    std::string ident;
protected:
    DomainManagerList _managers;
    PolicyList _policies;
public:
    ImplementationDef *_get_implementation ();
    InterfaceDef *_get_interface ();
    Object *_get_component ();

    void _create_request (Context_ptr ctx,
			  const char *operation,
			  NVList_ptr arg_list,
			  NamedValue_ptr result,
			  Request_out request,
			  Flags req_flags);

    void _create_request (Context_ptr ctx,
			  const char *operation,
			  NVList_ptr arg_list,
			  NamedValue_ptr result,
			  ExceptionList_ptr,
			  ContextList_ptr,
			  Request_out request,
			  Flags req_flags);

    Request_ptr _request (const char *operation);

    Policy_ptr _get_policy (PolicyType policy_type);
    DomainManagerList *_get_domain_managers ();
    Object_ptr _set_policy_overrides (const PolicyList &policies,
				      SetOverrideType set_add);

    // begin-not-in-appendix
    Boolean _is_a (const char *logical_type_id);
    Boolean _is_a_remote (const char *logical_type_id);
    Boolean _non_existent ();
    Boolean _is_equivalent (Object_ptr);
    ULong _hash (ULong max);
    // end-not-in-appendix

    // begin-mico-extension
    Object (IOR * = 0);
    Object (const Object &);
    Object &operator= (const Object &);
    virtual ~Object ();

    void _setup_domains (CORBA::Object_ptr parent);

    void _forward (CORBA::Object_ptr);
    void _unforward ();
    Boolean _is_forwarded ()
    { return !!fwd_ior; }

    ORB_ptr _orbnc ();

    IOR *_ior ()
    { return ior; }

    IOR *_ior_fwd ()
    { return fwd_ior ? fwd_ior : ior; }

    const char *_repoid ()
    { return ior ? ior->objid() : 0; }

    const char *_ident ();
    virtual void *_narrow_helper (const char *repoid);
    // end-mico-extension

    static Object_ptr _duplicate (Object_ptr o)
    {
	if (o)
	    o->_ref();
	return o;
    }
    static Object_ptr _nil ()
    {
	return 0;
    }
};

class LocalObject;
typedef LocalObject *LocalObject_ptr;
typedef ObjVar<LocalObject> LocalObject_var;
typedef ObjOut<LocalObject> LocalObject_out;

class LocalObject : virtual public Object {
public:

    // ref-counting added in ptc/03-03-09

    virtual void
    _add_ref();

    virtual void
    _remove_ref();

    virtual ULong
    _refcount_value() const;

    // non-implemented methods of CORBA::Object

    InterfaceDef*
    _get_interface();

    DomainManagerList*
    _get_domain_managers();

    Policy_ptr
    _get_policy(PolicyType __policy_type);

    Object_ptr
    _set_policy_overrides(const PolicyList& __policies,
			  SetOverrideType __set_add);

    // implementation specific for LocalObject

    Boolean
    _non_existent()
    { return FALSE; }

    ULong
    _hash(ULong __max);

    Boolean
    _is_equivalent(Object_ptr);
};

typedef ObjVar<Object> Object_var;
// defined in basic.h
// typedef ObjOut<Object> Object_out;

}

#endif // __mico_object_h__
