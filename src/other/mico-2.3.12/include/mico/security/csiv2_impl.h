// -*- c++ -*-
//
//  MICO CSIv2 --- an Open Source CSIv2 implementation
//  Copyright (C) 2002 ObjectSecurity Ltd.
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

//  Written by Karel Gardas, <kgardas@objectsecurity.com>


#ifndef __CSIV2_IMPL_H__
#define __CSIV2_IMPL_H__

#include <mico/security/csiv2.h>

namespace CSIv2
{
    class CMSetup_impl
	: virtual public CMSetup,
	  virtual public CORBA::LocalObject
    {
	CORBA::UShort use_auth_layer_;
	CORBA::UShort use_attr_layer_;
    public:
	CMSetup_impl()
	    : use_auth_layer_(1), use_attr_layer_(1)
	{}
	
	virtual void
	auth_layer(CORBA::UShort value)
	{ use_auth_layer_ = value; }

	virtual CORBA::UShort
	auth_layer()
	{ return use_auth_layer_; }

	virtual void
	attr_layer(CORBA::UShort value)
	{ use_attr_layer_ = value; }

	virtual CORBA::UShort
	attr_layer()
	{ return use_attr_layer_; }
    }; // CMSetup_impl

    class TSS_impl
	: virtual public CMSetup_impl,
	  virtual public TSS,
	  virtual public CORBA::LocalObject
    {
	SecurityManager_ptr sec_manager_;
	CORBA::TypeCode_ptr sas_body_tc_;
	typedef std::map<unsigned long, IOP::ServiceContext> RequestMap;
	typedef std::map<unsigned long, IOP::ServiceContext>::iterator
	RequestMapIterator;
	RequestMap request_map_;
    public:
	TSS_impl();

	virtual void
	security_manager(SecurityManager_ptr manager);

	virtual SecurityManager_ptr
	security_manager();

	virtual IOP::ServiceContext*
	accept_context(PortableInterceptor::ServerRequestInfo_ptr info, CORBA::Boolean_out throw_exc);

	virtual CSI::CompleteEstablishContext*
	establish_context(const CSI::EstablishContext& msg)
	    throw (CSIv2::GSSAuthError, CSIv2::TrustIdentityError, CSIv2::InvalidMechanism);

	virtual void 
	accept_transport_context();

	virtual void
	receive_request_service_contexts(PortableInterceptor::ServerRequestInfo_ptr info);

	virtual void
	receive_request(PortableInterceptor::ServerRequestInfo_ptr info);

	virtual void
	send_reply(PortableInterceptor::ServerRequestInfo_ptr info);

	virtual void
	send_exception(PortableInterceptor::ServerRequestInfo_ptr info);

	virtual void
	send_other(PortableInterceptor::ServerRequestInfo_ptr info);
    }; // TSS_impl

    class CSS_impl
	: virtual public CMSetup_impl,
	  virtual public CSS,
	  virtual public CORBA::LocalObject
    {
	SecurityManager_ptr sec_manager_;
	CORBA::TypeCode_ptr sas_body_tc_;
    public:
	CSS_impl();

	virtual void
	security_manager(SecurityManager_ptr manager);

	virtual SecurityManager_ptr
	security_manager();

	virtual void
	write_sas_context
	(PortableInterceptor::ClientRequestInfo_ptr info,
	 const CSIIOP::CompoundSecMechList& list);

	virtual void
	send_request(PortableInterceptor::ClientRequestInfo_ptr info);

	virtual void
	receive_reply(PortableInterceptor::ClientRequestInfo_ptr info);

	virtual void
	receive_exception(PortableInterceptor::ClientRequestInfo_ptr info);

	virtual void
	receive_other(PortableInterceptor::ClientRequestInfo_ptr info);
    }; // CSS_impl

    class SecurityManager_impl
	: public virtual SecurityManager,
	  virtual public CORBA::LocalObject
    {
    private:
	TSS_ptr tss_;
	// many users on server
	std::list<std::string> tss_users_;
	std::list<std::string> tss_passwds_;
	CSS_ptr css_;
	// TLS support
	DistinguishedNameList tls_user_list_;
	// Identity support
	UserIdentityList user_id_list_;
	CORBA::String_var client_identity_;
	// only one user for client
	std::string css_user_;
	std::string css_passwd_;
	std::string current_user_;
	std::string realm_;
	CORBA::Boolean csiv2_;
	IOP::CodecFactory_ptr codec_factory_;
	IOP::Codec_ptr codec_;
	CORBA::TypeCode_ptr init_token_tc_;

	CSIIOP::CompoundSecMechList csml_;

	CORBA::UShort giop_version_;

	CORBA::OctetSeq*
	string2octet_seq(std::string str);

	void
	auth_layer(CORBA::UShort value);

	void
	attr_layer(CORBA::UShort value);

	void
	recompute_cm_req();
    public:
	SecurityManager_impl();

	// Implemented for SecurityManager

	void
	tss(TSS_ptr value);

	TSS_ptr
	tss()
	{ return tss_; }

	void
	css(CSS_ptr value);

	CSS_ptr
	css()
	{ return css_; }
	
	void
	csiv2(CORBA::Boolean value)
	{ csiv2_ = value; }

	CORBA::Boolean
	csiv2()
	{ return csiv2_; }
	
	CSI::GSSToken*
	get_gss_init_token(const CSI::GSS_NT_ExportedName& target_name);

	void
	codec_factory(IOP::CodecFactory_ptr factory);

	IOP::CodecFactory_ptr
	codec_factory();

	IOP::Codec_ptr
	codec();

	void
	auth_token(const CSI::GSSToken& token)
	    throw (GSSAuthError);

	CSI::GSSToken*
	error_token(GSSUP::ErrorCode reason);

	CORBA::OctetSeq*
	ior_component_data();

	virtual void
	tls_user_list(const DistinguishedNameList& user_list);

	virtual DistinguishedNameList*
	tls_user_list();

	virtual UserIdentityList*
	user_id_list();

	virtual void
	user_id_list(const UserIdentityList& user_id_list);

	virtual CSI::GSS_NT_ExportedName*
	client_identity_token();

	virtual void
	verify_client_identity
	(const CSI::IdentityToken& identity,
	 CORBA::Boolean use_auth_layer)
	    throw(TrustIdentityError);

	virtual void
	client_identity(const char* identity);

	virtual char*
	client_identity();

	// SecurityManager_impl extension ie. not in IDL SecurityManager

	void
	add_server_user(char* name, char* passwd);
	
	void
	client_user(char* name, char* passwd);

	void
	realm_name(const char* name);

	CORBA::OctetSeq*
	gss_export_name(const char* oid, const char* name);

	char*
	gss_import_name(const char* oid, CORBA::OctetSeq* data);

	void
	giop_version(CORBA::UShort ver)
	{ giop_version_ = ver; }
    }; // SecurityManager_impl

    class ServerRequestInterceptor_impl
	: virtual public ServerRequestInterceptor,
	  virtual public CORBA::LocalObject
    {
	std::string name_;
	SecurityManager_ptr sec_manager_;
	IOP::CodecFactory_ptr codec_factory_;
	CSIv2::TSS_ptr tss_;
    public:
	ServerRequestInterceptor_impl()
	    : name_(""), sec_manager_(SecurityManager::_nil()),
	      codec_factory_(IOP::CodecFactory::_nil()),
	      tss_(CSIv2::TSS::_nil())
	{}
	ServerRequestInterceptor_impl
	(char* name,
	 SecurityManager_ptr manager,
	 IOP::CodecFactory_ptr factory)
	    : name_(name), sec_manager_(SecurityManager::_narrow(manager)),
	      codec_factory_(IOP::CodecFactory::_duplicate(factory)),
	      tss_(CSIv2::TSS::_duplicate(sec_manager_->tss()))
	{}

	char*
	name()
	{ return CORBA::string_dup(name_.c_str()); }

	void
	destroy();

	void
	receive_request_service_contexts(PortableInterceptor::ServerRequestInfo_ptr info);

	void
	receive_request(PortableInterceptor::ServerRequestInfo_ptr info);

	void
	send_reply(PortableInterceptor::ServerRequestInfo_ptr info);

	void
	send_exception(PortableInterceptor::ServerRequestInfo_ptr info);

	void
	send_other(PortableInterceptor::ServerRequestInfo_ptr info);
	
	CSIv2::TSS_ptr
	tss()
	{ return tss_; }
    }; // ServerRequestInterceptor_impl

    class ClientRequestInterceptor_impl
	: virtual public ClientRequestInterceptor,
	  virtual public CORBA::LocalObject
    {
	std::string name_;
	SecurityManager_ptr sec_manager_;
	IOP::CodecFactory_ptr codec_factory_;
	CSIv2::CSS_ptr css_;
    public:
	ClientRequestInterceptor_impl()
	    : name_(""), sec_manager_(SecurityManager::_nil()),
	      codec_factory_(IOP::CodecFactory::_nil()),
	      css_(CSIv2::CSS::_nil())
	{}
	ClientRequestInterceptor_impl
	(char* name,
	 SecurityManager_ptr manager,
	 IOP::CodecFactory_ptr factory)
	    : name_(name), sec_manager_(SecurityManager::_narrow(manager)),
	      codec_factory_(IOP::CodecFactory::_duplicate(factory)),
	      css_(CSIv2::CSS::_duplicate(sec_manager_->css()))
	{}

	char*
	name()
	{ return CORBA::string_dup(name_.c_str()); }

	void
	destroy();

	void
	send_request(PortableInterceptor::ClientRequestInfo_ptr info);

	void
	send_poll(PortableInterceptor::ClientRequestInfo_ptr info)
	{}

	void
	receive_reply(PortableInterceptor::ClientRequestInfo_ptr info);

	void
	receive_exception(PortableInterceptor::ClientRequestInfo_ptr info);

	void
	receive_other(PortableInterceptor::ClientRequestInfo_ptr info);

	CSIv2::CSS_ptr
	css()
	{ return css_; }
    }; // ClientRequestInterceptor_impl

    class InterceptorInitializer_impl
	: public virtual PortableInterceptor::ORBInitializer,
	  virtual public CORBA::LocalObject
    {
	SecurityManager_ptr sec_manager_;
    public:
	InterceptorInitializer_impl()
	    : sec_manager_(SecurityManager::_nil())
	{}
	InterceptorInitializer_impl(SecurityManager_ptr m)
	    : sec_manager_(SecurityManager::_duplicate(m))
	{}

	void
	pre_init(PortableInterceptor::ORBInitInfo_ptr info);

	void
	post_init(PortableInterceptor::ORBInitInfo_ptr info)
	{}
    };

    class IORInterceptor_impl
	: virtual public PortableInterceptor::IORInterceptor,
	  virtual public CORBA::LocalObject
    {
	std::string name_;
	CSIv2::SecurityManager_ptr sec_manager_;
    public:
	IORInterceptor_impl()
	    : name_(""), sec_manager_(CSIv2::SecurityManager::_nil())
	{}
	IORInterceptor_impl(char* name, CSIv2::SecurityManager_ptr sm)
	    : name_(name), sec_manager_(CSIv2::SecurityManager::_duplicate(sm))
	{}

	char*
	name()
	{ return CORBA::string_dup(name_.c_str()); }

	void
	name(char* name)
	{ name_ = name; }

	void
	destroy();

	void
	establish_components(PortableInterceptor::IORInfo_ptr info);
    };

    // CSIv2::Component

    class Component
	: public CORBA::Component
    {
	CSIIOP::CompoundSecMechList csml_;
	IOP::Codec_ptr codec_;
    public:
	Component()
	    : codec_(IOP::Codec::_nil())
	{}
	Component(const Component& c);
	Component(CSIIOP::CompoundSecMechList& list)
	    : csml_(list), codec_(IOP::Codec::_nil())
	{}
	Component(CSIIOP::CompoundSecMechList& list, IOP::Codec_ptr codec)
	    : csml_(list), codec_(IOP::Codec::_duplicate(codec))
	{}

	~Component()
	{ CORBA::release(codec_); }

	virtual void
	encode(CORBA::DataEncoder& encoder) const;

	virtual	ComponentId
	id() const;

	virtual void
	print(std::ostream& out) const;

	virtual CORBA::Component*
	clone () const;

	virtual CORBA::Long
	compare(const CORBA::Component& component) const;

	virtual CORBA::Boolean
	operator==(const CORBA::Component& component) const;

	virtual CORBA::Boolean
	operator<(const CORBA::Component& component) const;

	virtual void
	codec(IOP::Codec_ptr codec);

	virtual CSIIOP::CompoundSecMechList*
	mech_list();
    };

    class ComponentDecoder
	: public CORBA::ComponentDecoder
    {
	IOP::Codec_ptr codec_;
    public:
	ComponentDecoder();
	~ComponentDecoder();
	
	virtual CORBA::Component*
	decode(CORBA::DataDecoder&, ComponentId, CORBA::ULong) const;

	virtual CORBA::Boolean
	has_id (ComponentId) const;

	virtual void
	codec(IOP::Codec_ptr codec);
    };

} // CSIv2


#endif // __CSIV2_IMPL_H__
