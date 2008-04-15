/*
 *  MIWCO --- Wireless CORBA support for MICO (OMG document formal/2004-04-02)
 *            Access Bridge main program
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

#include "AccessBridge_impl.h"
#include <coss/CosNaming.h>
#include <coss/CosEventComm.h>
#include <coss/CosEventChannelAdmin.h>
#include <coss/GTP.h>
#include <mico/impl.h>
#include "glue.h"
#include "proxy.h"
#include "bridge.h"

using namespace std;

int
main (int argc, char *argv[])
{
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    // We create all our profiles below
    orb->ior_template()->del_profile(orb->ior_template()->profile());

    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var root_poa = PortableServer::POA::_narrow(obj);
    PortableServer::POAManager_var root_mgr = root_poa->the_POAManager();

    CORBA::PolicyList policies;
    policies.length(2);
    policies[0] = root_poa->create_lifespan_policy(PortableServer::PERSISTENT);
    policies[1] =
	root_poa->create_id_assignment_policy(PortableServer::USER_ID);

    CosNaming::NamingContext_var inc;
    try {
	obj = orb->resolve_initial_references("NameService");
	inc = CosNaming::NamingContext::_narrow(obj);
	if (!CORBA::is_nil(inc)) {
	    CosNaming::Name name;
	    name.length(1);
	    name[0].id = CORBA::string_dup("MobilitySupport");
	    try {
		inc->bind_new_context(name);
	    } catch (const CosNaming::NamingContext::AlreadyBound &) {
		// This is what we'd expect
	    }
	}
    } catch (const CORBA::Exception &) {
	inc = CosNaming::NamingContext::_nil();
    }

    MICOGetOpt::OptMap opts;
    opts["-WATMGIOPAddr"]    = "arg-expected";
    opts["-WATMGTPAddr"]     = "arg-expected";
    opts["-WATMBridgeName"]  = "arg-expected";

    vector<string> giop_addr_strs, gtp_addr_strs;
    string bridge_name_str = "AccessBridge";
    MICOGetOpt opt_parser(opts);
    if (!opt_parser.parse(argc, argv, TRUE))
	mico_throw(CORBA::INITIALIZE());
    const MICOGetOpt::OptVec &o = opt_parser.opts();
    for (MICOGetOpt::OptVec::const_iterator i = o.begin(); i != o.end(); ++i) {
	string arg = (*i).first;
	string val = (*i).second;
	if (arg == "-WATMGIOPAddr") {
	    giop_addr_strs.push_back(val);
	} else if (arg == "-WATMGTPAddr") {
	    gtp_addr_strs.push_back(val);
	} else if (arg == "-WATMBridgeName") {
	    bridge_name_str = val;
	}
    }

    // Find and create Event channel
    CosEventComm::PushConsumer_var mob_chan =
	CosEventComm::PushConsumer::_nil();
    CosEventChannelAdmin::EventChannel_var channel =
	CosEventChannelAdmin::EventChannel::_nil();
    if (!CORBA::is_nil(inc)) {
	try {
	    CosEventChannelAdmin::SupplierAdmin_var supplier;
	    CosNaming::Name chan_name;
	    chan_name.length(2);
	    chan_name[0].id = CORBA::string_dup("MobilitySupport");
	    chan_name[1].id = CORBA::string_dup("NetworkChannel");
	    try {
		obj = inc->resolve(chan_name);
		channel = CosEventChannelAdmin::EventChannel::_narrow(obj);
		supplier = channel->for_suppliers();
	    } catch (CORBA::Exception &) {
		CosNaming::Name fact_name;
		fact_name.length(1);
		fact_name[0].id = CORBA::string_dup("EventChannelFactory");
		obj = inc->resolve(fact_name);
		SimpleEventChannelAdmin::EventChannelFactory_var factory =
		    SimpleEventChannelAdmin::EventChannelFactory::_narrow(obj);
		channel = factory->create_eventchannel();
		supplier = channel->for_suppliers();
		inc->rebind(chan_name, channel);
	    }
	    CosEventChannelAdmin::ProxyPushConsumer_var consumer =
		supplier->obtain_push_consumer();
	    consumer->connect_push_supplier(CosEventComm::PushSupplier::_nil());
	    mob_chan = CosEventComm::PushConsumer::_narrow(consumer);
	} catch (const CORBA::Exception &) {
	    /*
	     * We don't really need a channel, so there is no need to bomb
	     * if we can't have one.
	     */
	}
    }

    vector<CORBA::Address *> gtp_addrs;
    for (mico_vec_size_type iv = 0; iv < gtp_addr_strs.size(); ++iv) {
	CORBA::Address *tem_addr =
	    CORBA::Address::parse(gtp_addr_strs[iv].c_str());
	if (tem_addr != NULL)
	    gtp_addrs.push_back(tem_addr);
    }
    if (gtp_addrs.size() <= 0) {
	if (gtp_addr_strs.size() > 0) {
	    cerr << "None of the given GTP addresses";
	    for (mico_vec_size_type ev = 0; ev < gtp_addr_strs.size(); ++ev) {
		cerr << " " << gtp_addr_strs[ev];
	    }
	    cerr << " are valid MICO addresses, aborting" << endl;
	} else {
	    cerr << "No valid GTP addresses specified, aborting" << endl;
	}
	exit(1);
    }
    WATMProxy::GTPNetwork *proxy =
	new WATMProxy::GTPNetwork(orb, gtp_addrs, 0x0100, mob_chan);

    vector<CORBA::Address *> giop_addrs;
    for (mico_vec_size_type ivt = 0; ivt < giop_addr_strs.size(); ++ivt) {
	CORBA::Address *tem_addr =
	    CORBA::Address::parse(giop_addr_strs[ivt].c_str());
	if (tem_addr != NULL)
	    giop_addrs.push_back(tem_addr);
    }
    if (giop_addrs.size() <= 0) {
	if (giop_addr_strs.size() > 0) {
	    cerr << "None of the given GIOP addresses";
	    for (mico_vec_size_type gv = 0; gv < giop_addr_strs.size(); ++gv) {
		cerr << " " << giop_addr_strs[gv];
	    }
	    cerr << " are valid MICO addresses, aborting" << endl;
	} else {
	    cerr << "No valid GIOP addresses specified, aborting" << endl;
	}
	exit(1);
    }
    WATMBridge::AccessBridge *ap =
	new WATMBridge::AccessBridge(orb, giop_addrs, proxy);
    // Proxy takes responsibility of bridge deallocation
    proxy->brselect(ap);

    PortableServer::POA_var ab_poa;
    ab_poa = root_poa->create_POA("MobilitySupport",
				  PortableServer::POAManager::_nil(),
				  policies);
    PortableServer::POAManager_var ab_mgr = ab_poa->the_POAManager();

    // Servant takes responsibility of proxy deallocation
    AccessBridge_impl *servant = new AccessBridge_impl(orb, proxy, inc,
						       gtp_addrs, channel);
    PortableServer::ObjectId_var oid =
	PortableServer::string_to_ObjectId(bridge_name_str.c_str());
    ab_poa->activate_object_with_id(oid, servant);
    MobileTerminal::AccessBridge_var ab = servant->_this();
    proxy->abselect(ab);
    ap->register_local_object(ab);

    if (!CORBA::is_nil(inc)) {
	CosNaming::Name ab_name;
	ab_name.length(2);
	ab_name[0].id = CORBA::string_dup("MobilitySupport");
	ab_name[1].id = CORBA::string_dup(bridge_name_str.c_str());
	inc->rebind(ab_name, ab);
    }

    cout << orb->object_to_string(ab) << endl;

    root_mgr->activate();
    ab_mgr->activate();
    orb->run();

    ab_poa->destroy(TRUE, TRUE);
    root_poa->destroy(TRUE, TRUE);
    delete servant;
}
