// -*- c++ -*-
//
//  MICOsec --- a free CORBA Security implementation
//  Copyright (C) 2000 ObjectSecurity Ltd.
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Library General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Library General Public License for more details.
//
//  You should have received a copy of the GNU Library General Public
//  License along with this library; if not, write to the Free
//  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

//  Send comments and/or bug reports to:
//                 micosec@objectsecurity.com
//


#ifndef __AUDITINTERCEPT_H__
#define __AUDITINTERCEPT_H__

#define MICO_CONF_INTERCEPT
#define MICO_CONF_IMR
#define MICO_CONF_IR

#include <CORBA.h>
#include <mico/impl.h>
#include <mico/template_impl.h>
#include <mico/util.h>
#include <mico/ioptypes.h>

#define SERVER (CORBA::Octet)4
#define CLIENT (CORBA::Octet)0

//abstract base class
class AuditInterceptor
{
protected:
    AuditInterceptor()
    {};
    ~AuditInterceptor()
    {};

    void
    audit_analyse(Security::EventType ev_type, const char* interface_name,
		  const char* object_ref, const char* operation,
		  const char* initiator, short success_failure,
		  CORBA::Octet c_s = SERVER);
};


// Server Interceptor
class AuditServerInterceptor
    : public AuditInterceptor,
      virtual public PortableInterceptor::ServerRequestInterceptor
{
private:
    Security::Opaque ret_;
    CORBA::Boolean access_control_;
    static AuditServerInterceptor* S_self_;
    std::string obj_reference_;
    std::string initiator_;
    typedef std::map<unsigned long, char*> RequestMap;
    typedef std::map<unsigned long, char*>::iterator
    RequestMapIterator;
    RequestMap request_map_;

protected:
    time_t
    timeT2epoch(TimeBase::TimeT timeV);

    TimeBase::TimeT
    epoch2timeT(time_t);

    void
    get_audit_id(CORBA::ULong& l);

    void
    get_object_ref(CORBA::ULong& l);

    void
    get_peer_address(CORBA::ULong& l);

public:
    AuditServerInterceptor(CORBA::Boolean acc = TRUE);
    ~AuditServerInterceptor();

    virtual char*
    name();

    virtual void
    destroy()
    {}

    virtual void
    receive_request_service_contexts
    (PortableInterceptor::ServerRequestInfo_ptr ri);

    virtual void
    receive_request(PortableInterceptor::ServerRequestInfo_ptr ri);

    virtual void
    send_reply(PortableInterceptor::ServerRequestInfo_ptr ri);

    virtual void
    send_exception(PortableInterceptor::ServerRequestInfo_ptr ri);

    virtual void
    send_other(PortableInterceptor::ServerRequestInfo_ptr ri);

    static void
    _exec_principal_auth(const char* cred);
};

// Server Interceptor Initializer
class AuditServerInitializer
    : virtual public PortableInterceptor::ORBInitializer
{
public:
    AuditServerInitializer()
    {}
    ~AuditServerInitializer()
    {}

    virtual void
    pre_init(PortableInterceptor::ORBInitInfo_ptr info);

    virtual void
    post_init(PortableInterceptor::ORBInitInfo_ptr info);
};

// Client Interceptor
class AuditClientInterceptor
    : virtual public PortableInterceptor::ClientRequestInterceptor,
      public AuditInterceptor
{
private:
    CORBA::Octet* ret_;
    static AuditClientInterceptor* S_self_;
	
protected:
    CORBA::Octet*
    get_audit_id
    (PortableInterceptor::ClientRequestInfo_ptr req,
     CORBA::ULong& l);

    CORBA::Octet*
    get_object_ref
    (PortableInterceptor::ClientRequestInfo_ptr req,
     CORBA::ULong& l);

    CORBA::Octet*
    get_interface_name
    (PortableInterceptor::ClientRequestInfo_ptr req,
     CORBA::ULong& l);

public:
    AuditClientInterceptor();
    ~AuditClientInterceptor();

    virtual char*
    name();

    virtual void
    destroy()
    {}

    virtual void
    send_request(PortableInterceptor::ClientRequestInfo_ptr ri);

    virtual void
    send_poll(PortableInterceptor::ClientRequestInfo_ptr ri);

    virtual void
    receive_reply(PortableInterceptor::ClientRequestInfo_ptr ri);

    virtual void
    receive_exception(PortableInterceptor::ClientRequestInfo_ptr ri);

    virtual void
    receive_other(PortableInterceptor::ClientRequestInfo_ptr ri);

    static void
    _exec_principal_auth (const char* cred);
};

// Client Interceptor Initializer
class AuditClientInitializer
    : virtual public PortableInterceptor::ORBInitializer
{
public:
    AuditClientInitializer()
    {}
    ~AuditClientInitializer()
    {}

    virtual void
    pre_init(PortableInterceptor::ORBInitInfo_ptr info);

    virtual void
    post_init(PortableInterceptor::ORBInitInfo_ptr info);
};

#endif // __AUDITINTERCEPT_H__

