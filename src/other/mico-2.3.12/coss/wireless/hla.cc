/*
 *  MIWCO --- Wireless CORBA support for MICO (OMG document formal/2004-04-02)
 *            Home Location Agent main program
 *  Copyright (C) 2001 Jaakko Kangasharju
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "HomeLocationAgent_impl.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else
#include <fstream.h>
#endif
#include <signal.h>
#include <coss/CosNaming.h>
#include <mico/impl.h>
#include <mico/watm.h>

using namespace std;

//  Useful helper functions
namespace
{

bool finished;

void sighandler (int sig)
{
    finished = true;
}

}

class HomeLocationService : public CORBA::ObjectAdapter {
public:

    const char *get_oaid () const;
    CORBA::Boolean has_object (CORBA::Object_ptr);
    CORBA::Boolean is_local () const;

#ifdef USE_CSL2
    CORBA::Principal_ptr get_principal (CORBA::Object_ptr);
#endif /* USE_CSL2 */

    CORBA::Boolean invoke (CORBA::ORBMsgId, CORBA::Object_ptr, CORBA::ORBRequest *,
			   CORBA::Principal_ptr, CORBA::Boolean);
    CORBA::Boolean bind (CORBA::ORBMsgId, const char *, const CORBA::ORB::ObjectTag &,
			 CORBA::Address *);
    CORBA::Boolean locate (CORBA::ORBMsgId, CORBA::Object_ptr);
    CORBA::Object_ptr skeleton (CORBA::Object_ptr);
    void cancel (CORBA::ORBMsgId);
    void shutdown (CORBA::Boolean);

    void answer_invoke (CORBA::ORBMsgId, CORBA::Object_ptr, CORBA::ORBRequest *,
			CORBA::InvokeStatus);

    HomeLocationService (CORBA::ORB_ptr, HomeLocationAgent_impl *);
    ~HomeLocationService ();

private:
    typedef HomeLocationAgent_impl::grid_data grid_data;

    CORBA::ORB_ptr _orb;
    HomeLocationAgent_impl *servant;

    grid_data::iterator find_terminal (CORBA::Object_ptr);
};

HomeLocationService::HomeLocationService (CORBA::ORB_ptr orb,
					  HomeLocationAgent_impl *hla)
{
    _orb = orb;
    servant = hla;
    _orb->register_oa(this);
}

HomeLocationService::~HomeLocationService ()
{
    _orb->unregister_oa(this);
}

#define check(exp) if(!(exp)) return servant->grid_map.end()

HomeLocationService::grid_data::iterator
HomeLocationService::find_terminal (CORBA::Object_ptr o)
{
    check(!CORBA::is_nil(o) && o->_ior());

    CORBA::IORProfile *prof =
	o->_ior()->profile(CORBA::IORProfile::TAG_MOBILE_TERMINAL_IOP);
    if (prof != NULL) {
	WATM::WATMProfile *wprof = dynamic_cast<WATM::WATMProfile *>(prof);
	check(wprof != NULL);
	CORBA::Long tidlen;
	const CORBA::Octet *tidbuf = wprof->terminalid(tidlen);
	CORBA::OctetSeq tid(tidlen);
	tid.length(tidlen);
	memcpy(tid.get_buffer(), tidbuf, tidlen);
	return servant->grid_map.find(tid);
    }

    prof = o->_ior()->profile();
    check(prof != NULL);
    CORBA::Long keylen;
    const CORBA::Octet *key = prof->objectkey(keylen);
    CORBA::Buffer buf(const_cast<CORBA::Octet *>(key));
#ifdef _WINDOWS
    MICO::CDRDecoder dc(&buf, FALSE, CORBA::DefaultEndian,
			0, TRUE, 0, TRUE);
#else
    MICO::CDRDecoder dc(&buf, FALSE);
#endif
    CORBA::Octet bo;
    check(dc.get_octet(bo));
    dc.byteorder(bo == 0 ? CORBA::BigEndian : CORBA::LittleEndian);

    char magic[4];
    check(dc.get_chars_raw(magic, 4));
    check(memcmp(magic, "MIOR", 4) == 0);
    check(dc.struct_begin());
    {
	check(dc.struct_begin());
	{
	    CORBA::Octet major, minor;
	    check(dc.get_octet(major));
	    check(dc.get_octet(minor));
	}
	check(dc.struct_end());
	CORBA::Octet blank;
	check(dc.get_octet(blank));
	CORBA::ULong tidlen;
	check(dc.seq_begin(tidlen));
	{
	    CORBA::Octet *tidbuf = CORBA::OctetSeq::allocbuf(tidlen);
	    check(dc.get_octets(tidbuf, tidlen));
	    CORBA::OctetSeq tid(tidlen, tidlen, tidbuf, TRUE);
	    return servant->grid_map.find(tid);
	}
    }
    return servant->grid_map.end();
}

const char *
HomeLocationService::get_oaid () const
{
    return "watm-terminal-locator";
}

CORBA::Boolean
HomeLocationService::has_object (CORBA::Object_ptr o)
{
    return find_terminal(o) != servant->grid_map.end();
}

CORBA::Boolean
HomeLocationService::is_local () const
{
    return TRUE;
}

#ifdef USE_CSL2
CORBA::Principal_ptr
HomeLocationService::get_principal (CORBA::Object_ptr o)
{
    assert(0);
    return (CORBA::Principal_ptr)0;
}
#endif /* USE_CSL2 */

CORBA::Boolean
HomeLocationService::invoke (CORBA::ORBMsgId msgid, CORBA::Object_ptr o,
			     CORBA::ORBRequest *req, CORBA::Principal_ptr pr,
			     CORBA::Boolean response_exp)
{
    /*
     * First make sure that GIOP 1.2 requests come with disposition
     * ReferenceAddr (Mobile Object Keys, even though MIWCO uses them,
     * are only for backwards compatibility).
     */
    MICO::GIOPRequest *giop_req = dynamic_cast<MICO::GIOPRequest *>(req);
    if (giop_req == NULL)
	// We only handle GIOP requests
	return FALSE;
    if (giop_req->version() >= 0x0102
	&& o->_ior()->addressing_disposition() != GIOP::ReferenceAddr) {
	_orb->answer_invoke(msgid, CORBA::InvokeAddrDisp, o, req,
			    GIOP::ReferenceAddr);
	return TRUE;
    }

    /*
     * Check that we really know the terminal and that it has an
     * associated Access Bridge.
     */
    grid_data::iterator i = find_terminal(o);
    if (i == servant->grid_map.end() || CORBA::is_nil(i->second.current_wab)) {
	CORBA::OBJECT_NOT_EXIST ex;
	req->set_out_args(&ex);
	_orb->answer_invoke(msgid, CORBA::InvokeSysEx, CORBA::Object::_nil(),
			    req, 0);
	return TRUE;
    }

    /*
     * Get all IIOP addresses for the Access Bridge.
     */
    list<const MICO::InetAddress *> ab_addrs;
    const CORBA::Address *addr = NULL;
    while ((addr = i->second.current_wab->_ior()->addr(CORBA::IORProfile::TAG_INTERNET_IOP, FALSE, addr)) != NULL) {
	ab_addrs.push_back(dynamic_cast<const MICO::InetAddress *>(addr));
    }
    if (ab_addrs.empty()) {
	// I wonder if this is the correct response
	CORBA::OBJECT_NOT_EXIST ex;
	req->set_out_args(&ex);
	_orb->answer_invoke(msgid, CORBA::InvokeSysEx, CORBA::Object::_nil(),
			    req, 0);
	return TRUE;
    }

    /*
     * Replace addresses in the IIOP profiles of the received Mobile
     * IOR with addresses of the Access Bridge.  KeyAddr needs to be
     * handled separately, since it only has a GIOPSimpleProf.
     */
    CORBA::IORProfile *prof = NULL;
    const MICO::InetAddress *iaddr = NULL;
    if (o->_ior()->addressing_disposition() == GIOP::KeyAddr) {
	prof = o->_ior()->profile();
	iaddr = ab_addrs.front();
	CORBA::Long len;
	const CORBA::Octet *key = prof->objectkey(len);
	CORBA::IORProfile *iprof =
	    iaddr->make_ior_profile(const_cast<CORBA::Octet *>(key), len,
				    CORBA::MultiComponent(), 0x0102);
	o->_ior()->del_profile(prof);
	o->_ior()->add_profile(iprof);
    } else {
	while ((prof = o->_ior()->profile(CORBA::IORProfile::TAG_INTERNET_IOP, FALSE, prof)) != NULL) {
	    MICO::IIOPProfile *iprof = dynamic_cast<MICO::IIOPProfile *>(prof);
	    if (!ab_addrs.empty()) {
		iaddr = ab_addrs.front();
		ab_addrs.pop_front();
	    }
	    /*
	     * iaddr is non-NULL at this point since ab_addrs is non-empty
	     * when we enter the loop.
	     */
	    iprof->addr(*iaddr);
	}
    }

    _orb->answer_invoke(msgid, CORBA::InvokeForward, o, req, 0);
    return TRUE;
}

CORBA::Boolean
HomeLocationService::bind (CORBA::ORBMsgId msgid, const char *repoid,
			   const CORBA::ORB::ObjectTag &oid,
			   CORBA::Address *addr)
{
    return FALSE;
}

CORBA::Boolean
HomeLocationService::locate (CORBA::ORBMsgId msgid, CORBA::Object_ptr o)
{
    /*
     * First make sure that GIOP 1.2 requests come with disposition
     * ReferenceAddr (Mobile Object Keys, even though MIWCO uses them,
     * are only for backwards compatibility).
     */
    MICO::GIOPRequest *giop_req =
	dynamic_cast<MICO::GIOPRequest *>(o->_orbnc()->request(msgid));
    if (giop_req == NULL)
	return FALSE;
    if (giop_req->version() >= 0x0102
	&& o->_ior()->addressing_disposition() != GIOP::ReferenceAddr) {
	_orb->answer_locate(msgid, CORBA::LocateAddrDisp, o,
			    GIOP::ReferenceAddr);
	return TRUE;
    }

    /*
     * Check that we really know the terminal and that it has an
     * associated Access Bridge.
     */
    grid_data::iterator i = find_terminal(o);
    if (i == servant->grid_map.end() || CORBA::is_nil(i->second.current_wab)) {
	_orb->answer_locate(msgid, CORBA::LocateUnknown,
			    CORBA::Object::_nil(), 0);
	return TRUE;
    }

    /*
     * Get all IIOP addresses for the Access Bridge.
     */
    list<const MICO::InetAddress *> ab_addrs;
    const CORBA::Address *addr = NULL;
    while ((addr = i->second.current_wab->_ior()->addr(CORBA::IORProfile::TAG_INTERNET_IOP, FALSE, addr)) != NULL) {
	ab_addrs.push_back(dynamic_cast<const MICO::InetAddress *>(addr));
    }
    if (ab_addrs.empty()) {
	_orb->answer_locate(msgid, CORBA::LocateUnknown,
			    CORBA::Object::_nil(), 0);
	return TRUE;
    }

    /*
     * Replace addresses in the IIOP profiles of the received Mobile
     * IOR with addresses of the Access Bridge.  KeyAddr needs to be
     * handled separately, since it only has a GIOPSimpleProf.
     */
    CORBA::IORProfile *prof = NULL;
    const MICO::InetAddress *iaddr = NULL;
    if (o->_ior()->addressing_disposition() == GIOP::KeyAddr) {
	prof = o->_ior()->profile();
	iaddr = ab_addrs.front();
	CORBA::Long len;
	const CORBA::Octet *key = prof->objectkey(len);
	CORBA::IORProfile *iprof =
	    iaddr->make_ior_profile(const_cast<CORBA::Octet *>(key), len,
				    CORBA::MultiComponent(), 0x0102);
	o->_ior()->del_profile(prof);
	o->_ior()->add_profile(iprof);
    } else {
	while ((prof = o->_ior()->profile(CORBA::IORProfile::TAG_INTERNET_IOP, FALSE, prof)) != NULL) {
	    MICO::IIOPProfile *iprof = dynamic_cast<MICO::IIOPProfile *>(prof);
	    if (!ab_addrs.empty()) {
		iaddr = ab_addrs.front();
		ab_addrs.pop_front();
	    }
	    /*
	     * iaddr is non-NULL at this point since ab_addrs is non-empty
	     * when we enter the loop.
	     */
	    iprof->addr(*iaddr);
	}
    }

    _orb->answer_locate(msgid, CORBA::LocateForward, o, 0);
    return TRUE;
}

CORBA::Object_ptr
HomeLocationService::skeleton (CORBA::Object_ptr o)
{
    return CORBA::Object::_nil();
}

void
HomeLocationService::cancel (CORBA::ORBMsgId msgid)
{
}

void
HomeLocationService::shutdown (CORBA::Boolean wait_for_completion)
{
}

void
HomeLocationService::answer_invoke (CORBA::ORBMsgId msgid, CORBA::Object_ptr o,
				    CORBA::ORBRequest *req,
				    CORBA::InvokeStatus status)
{
    assert(0);
}


int
main (int argc, char *argv[])
{
    signal (SIGINT, sighandler);
    signal (SIGTERM, sighandler);

    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var root_poa = PortableServer::POA::_narrow(obj);
    PortableServer::POAManager_var root_mgr = root_poa->the_POAManager();

    CORBA::PolicyList policies;
    policies.length(2);
    policies[0] = root_poa->create_lifespan_policy(PortableServer::PERSISTENT);
    policies[1] =
	root_poa->create_id_assignment_policy(PortableServer::USER_ID);

    PortableServer::POA_var hlr_poa =
	root_poa->create_POA("MobilitySupport",
			     PortableServer::POAManager::_nil(),
			     policies);
    PortableServer::POAManager_var hlr_mgr = hlr_poa->the_POAManager();

    CosNaming::NamingContext_var inc;
    try {
	obj = orb->resolve_initial_references("NameService");
	inc = CosNaming::NamingContext::_narrow(obj);
    } catch (const CORBA::Exception &) {
	inc = CosNaming::NamingContext::_nil();
    }

    string rcfile = "~/.hlarc";
    string prefix;
    MICOGetOpt::OptMap opts;
    opts["-WATMTerminalPrefix"]  = "arg-expected";
    opts["-WATMTerminalFile"]    = "arg-expected";

    MICOGetOpt opt_parser(opts);
    if (!opt_parser.parse(argc, argv, TRUE))
	mico_throw(CORBA::INITIALIZE());
    const MICOGetOpt::OptVec &o = opt_parser.opts();
    for (MICOGetOpt::OptVec::const_iterator i = o.begin(); i != o.end(); ++i) {
	string arg = (*i).first;
	string val = (*i).second;
	if (arg == "-WATMTerminalPrefix") {
	    prefix = val;
	} else if (arg == "-WATMTerminalFile") {
	    rcfile = val;
	}
    }

    HomeLocationAgent_impl *servant =
	new HomeLocationAgent_impl(orb, hlr_poa, inc, prefix, rcfile);
    HomeLocationService *hls = new HomeLocationService(orb, servant);

    PortableServer::ObjectId_var oid =
	PortableServer::string_to_ObjectId("HomeLocationAgent");
    hlr_poa->activate_object_with_id(oid, servant);
    MobileTerminal::HomeLocationAgent_var hlr = servant->_this();

    if (!CORBA::is_nil(inc)) {
	CosNaming::Name hlr_name;
	hlr_name.length(1);
	hlr_name[0].id = CORBA::string_dup("MobilitySupport");
	try {
	    inc->bind_new_context(hlr_name);
	} catch (const CosNaming::NamingContext::AlreadyBound &) {
	}
	hlr_name.length(2);
	hlr_name[1].id = CORBA::string_dup("HomeLocationAgent");
	inc->rebind(hlr_name, hlr);
    }

    ofstream of("home.ref");
    of << orb->object_to_string(hlr) << endl;
    of.close();

    root_mgr->activate();
    hlr_mgr->activate();
    finished = false;
    while (!finished) {
	orb->perform_work();
    }

    hlr_poa->destroy(TRUE, TRUE);
    root_poa->destroy(TRUE, TRUE);
    delete hls;
    delete servant;

    return 0;
}
