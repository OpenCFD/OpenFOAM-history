// -*- c++ -*-
/*
 *  MIWCO --- Wireless CORBA support for MICO (OMG document formal/2004-04-02)
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

#ifndef __mico_bridge_h__
#define __mico_bridge_h__

/*
 * The bridge classes understand GIOP to the extent wireless CORBA
 * bridges need to.  This means mostly parsing GIOP headers to get
 * needed information.  In some cases modifications to the message may
 * be necessary.
 */
namespace WATMBridge {

class GIOPBridge : public CORBA::TransportServerCallback,
		   public WATMGlue::BridgeRequestCallback,
		   public MICO::GIOPConnCallback {
protected:
    typedef std::map<CORBA::ULong, MICO::GIOPConn *> MapCidConn;
    typedef std::map<CORBA::ULong, WATMGlue::BridgeRequest *> MapReqTarget;
    typedef std::map<CORBA::ULong, MapReqTarget> MapCidReqMap;
    typedef std::map<CORBA::ULong, std::pair<CORBA::ULong, CORBA::UShort> > MapIdPair;
    typedef std::map<CORBA::ULong, CORBA::ULong> MapIdId;
    typedef std::map<CORBA::ULong, MapIdId> MapCidIdMap;

    struct ac :
	std::binary_function<const CORBA::Address *, const CORBA::Address *, bool> {
	bool operator() (const CORBA::Address *a1,
			 const CORBA::Address *a2) const
	    { return *a1 < *a2; }
    };
    typedef std::map<const CORBA::Address *, MICO::GIOPConn *, ac> MapAddrConn;

    struct kc :
	std::binary_function<const CORBA::OctetSeq, const CORBA::OctetSeq, bool>
    {
	bool operator() (const CORBA::OctetSeq key1,
			 const CORBA::OctetSeq key2) const
	    {
		if (key1.length() != key2.length())
		    return key1.length() < key2.length();
		return mico_key_compare(key1.get_buffer(), key2.get_buffer(),
					key1.length()) < 0;
	    }
    };
    typedef std::set<CORBA::OctetSeq, kc> SetKey;

    struct ioc :
	std::binary_function<const CORBA::IOR *, const CORBA::IOR *, bool>
    {
	bool operator() (const CORBA::IOR *ior1, const CORBA::IOR *ior2) const
	    {
		if (ior1 == NULL)
		    return ior2 != NULL;
		if (ior2 == NULL)
		    return FALSE;
		return *ior1 < *ior2;
	    }
    };
    typedef std::map<const CORBA::IOR *, CORBA::ULong, ioc> MapIorId;
    typedef std::map<CORBA::ULong, MapIorId> MapCidIorMap;

    /*
     * The bridges use various data structures heavily.  Of the ones
     * here, _conns contains a mapping from connection identifiers to
     * actual GIOP connections.  The identifiers here come from the
     * proxy so many of them may map into the same connection.  This
     * identifier is therefore different from that given by the id()
     * method of GIOPConn.
     *
     * The bridges store connections mapped by address into
     * _conn_cache.  Then, when a connection request to a known
     * address comes, the bridge can just take a connection from its
     * cache.
     */
    MapCidConn _conns;
    MapAddrConn _conn_cache;

    /*
     * The first message on a properly functioning GIOP connection
     * should be a Request or a LocateRequest, from which it is
     * possible to get the target object.  But after that the various
     * messages are identified only by a request identifier.
     * Therefore the bridges store for each connection a mapping from
     * request identifiers to BridgeRequests (it might be possible to
     * just store connection identifiers for the proxy; I remember
     * having a reason for doing it this way).  _conn_addrs is a map
     * associating such a mapping with each GIOP connection
     * identifier.
     */
    MapCidReqMap _conn_addrs;

    /*
     * The bridge needs to map request identifiers in incoming GIOP
     * messages uniquely in the case that the same request identifier
     * is used on two different connections.  The _request_proxy map
     * maps each GIOP connection identifier to a map from original
     * identifiers to transformed identifiers.  Since the transformed
     * identifiers are globally unique, the reverse mapping
     * _proxy_request does not need a connection identifier.
     *
     * For optimisation purposes each GIOP connection has a map
     * associating object references to proxy connection identifiers.
     * This lets the bridge to avoid doing superfluous opens when many
     * invocations are made to the same object.
     */
    MapIdPair _proxy_request;
    MapCidIdMap _request_proxy;
    MapCidIorMap _cached_iors;

    /*
     * The bridge is the first one to receive any GIOP messages sent
     * to its address.  When the message is not for the terminal but
     * rather for a CORBA object on the bridge, the message is not for
     * forwarding.  Therefore every local CORBA object registers its
     * object key into the set _local_keys, which the bridge consults
     * for each received request.  If the key on the message matches,
     * it sends it through a specially created IIOP server up to the
     * object.
     */
    SetKey _local_keys;

    CORBA::ORB_ptr _orb;
    MICO::IIOPServer *_upcall;
    WATMGlue::BridgeRequestCallback *_proxy;
    std::vector<CORBA::TransportServer *> _tservers;
    CORBA::UShort _giop_ver;

    WATMGlue::IdGenerator _id_gen;

    /*
     * These implement the handling of local CORBA objects.  The
     * message processor queries local_target for each message and, if
     * local_target returns TRUE, calls local_input and returns.
     */
    virtual CORBA::Boolean local_target (WATMGlue::TargetAddress *);
    virtual CORBA::Boolean local_input (MICO::GIOPConn *, CORBA::Buffer *);

    /*
     * Objects on terminals do not necessarily implement the Mobile
     * Object Key format.  Therefore the bridge sends a request for
     * IOR addressing disposition if the received request was GIOP 1.2
     * or higher.  put_header and put_size are practically just lifted
     * from MICO's GIOP handler.
     */
    CORBA::ULong put_header (CORBA::DataEncoder *, GIOP::MsgType,
			     CORBA::UShort);
    void put_size (CORBA::DataEncoder *ec, CORBA::ULong offset);
    void send_req_addr_disp (MICO::GIOPConn *, CORBA::ULong req_id);
    void send_loc_addr_disp (MICO::GIOPConn *, CORBA::ULong req_id);

    /*
     * When the terminal is not at this Access Bridge, the Access
     * Bridge must forward requests and locate requests to the correct
     * Access Bridge (or the HLA).  These functions send a proper
     * reply.
     */
    void send_req_forw (MICO::GIOPConn *, CORBA::ULong req_id,
			CORBA::IOR *forward);
    void send_loc_forw (MICO::GIOPConn *, CORBA::ULong req_id,
			CORBA::IOR *forward);
    void send_req_nonexist (MICO::GIOPConn *, CORBA::ULong req_id);
    void send_loc_nonexist (MICO::GIOPConn *, CORBA::ULong req_id);

    /*
     * When receiving a message, the bridge needs to determine the
     * message's actual target.  get_target returns a BridgeRequest
     * with a properly filled target member.  It calls the
     * get_*_target functions when the target cannot be found in the
     * _conn_addrs map.  Context lists in GIOP 1.0 and 1.1 messages
     * need to be passed by, for which skip_contextlist is used.
     */
    CORBA::Boolean skip_contextlist (MICO::GIOPInContext &, MICO::GIOPCodec *);
    WATMGlue::TargetAddress *get_request_target (MICO::GIOPInContext &,
						 CORBA::ULong &,
						 MICO::GIOPCodec *);
    WATMGlue::TargetAddress *get_locate_target (MICO::GIOPInContext &,
						CORBA::ULong &,
						MICO::GIOPCodec *);
    WATMGlue::BridgeRequest *get_target (MICO::GIOPInContext &,
					 MICO::GIOPConn *);

    /*
     * The request identifiers in GIOP messages need to be mapped into
     * unique identifiers and back again.  transform_request_id does
     * the former, revert_request_id the latter.  These also transform
     * object addressing information from GIOP 1.2 to a lower version
     * if needed.  For this latter task need_version_downgrade tells
     * whether this is a bridge that can receive too highly versioned
     * GIOP messages (Terminal Bridge is such a bridge).
     */
    CORBA::Boolean transform_request_id (MICO::GIOPConn *,
					 WATMGlue::BridgeRequest *);
    CORBA::Boolean revert_request_id (MICO::GIOPConn *,
				      WATMGlue::BridgeRequest *);
    virtual CORBA::Boolean need_version_downgrade ()
	{ return FALSE; }

    /*
     * key_to_profile is mainly intended for the Terminal Bridge.
     * When it receives an OpenConnectionRequest with only an object
     * key, it needs to know the address of the object.  Objects on
     * the terminal register their profiles to the Terminal Bridge at
     * creation time.
     */
    virtual CORBA::IORProfile *key_to_profile (CORBA::OctetSeq *);

    MICO::GIOPConn *make_conn (const CORBA::Address *);
    virtual MICO::GIOPConn *make_conn (const WATMGlue::TargetAddress *);
    void kill_conn (MICO::GIOPConn *);
    void conn_error (MICO::GIOPConn *, CORBA::Boolean send_error = TRUE);

    virtual CORBA::Boolean send_data (MICO::GIOPConn *,
				      WATMGlue::BridgeRequest *);
    CORBA::Boolean handle_input (MICO::GIOPConn *, CORBA::Buffer *);
    virtual CORBA::Boolean handle_input (MICO::GIOPConn *);
public:
    GIOPBridge (CORBA::ORB_ptr, std::vector<CORBA::Address *>,
		WATMGlue::BridgeRequestCallback *,
		WATMGlue::IdGenerator id_gen = WATMGlue::IdGenerator());
    virtual ~GIOPBridge ();

    void register_local_object (CORBA::Object_ptr);

    void callback (CORBA::TransportServer *,
		   CORBA::TransportServerCallback::Event);
    CORBA::Boolean callback (WATMGlue::BridgeRequest *,
			     WATMGlue::BridgeRequestCallback::Event);
    CORBA::Boolean callback (MICO::GIOPConn *, MICO::GIOPConnCallback::Event);
    CORBA::Boolean input_callback (MICO::GIOPConn *, CORBA::Buffer *);
};

class TerminalBridge : public GIOPBridge {
    struct kpc :
	std::binary_function<const CORBA::OctetSeq *, const CORBA::OctetSeq *,
	bool> {
	bool operator() (const CORBA::OctetSeq *k1,
			 const CORBA::OctetSeq *k2) const
	    {
		if (k1->length() != k2->length())
		    return k1->length() < k2->length();
		return mico_key_compare(k1->get_buffer(), k2->get_buffer(),
					k1->length()) < 0;
	    }
    };
    typedef std::map<const CORBA::OctetSeq *, CORBA::IORProfile *, kpc> MapKeyProf;

    MapKeyProf _terminal_objs;
    CORBA::IORProfile *key_to_profile (CORBA::OctetSeq *);

    /*
     * When MICO receives a request using GIOP 1.2 and non-key
     * addressing mode, it checks the profile.  Unfortunately the
     * address in the profile is that of the Access Bridge so this
     * check fails.  So we go around it by transforming all GIOP 1.2
     * (and higher) messages to use key addressing.
     */
    void transform_target (WATMGlue::BridgeRequest *);
    CORBA::Boolean send_data (MICO::GIOPConn *, WATMGlue::BridgeRequest *);

    CORBA::Boolean need_version_downgrade ()
	{ return TRUE; }

    /*
     * Hmm, I'm not actually sure why this is overridden.  I do
     * remember the base class's method failed.
     */
    MICO::GIOPConn *make_conn (const WATMGlue::TargetAddress *);
public:
    TerminalBridge (CORBA::ORB_ptr orb, std::vector<CORBA::Address *> addrs,
		    WATMGlue::BridgeRequestCallback *proxy);
    ~TerminalBridge ();

    void register_object (CORBA::OctetSeq *, CORBA::IORProfile *);
};

class AccessBridge : public GIOPBridge {
public:
    AccessBridge (CORBA::ORB_ptr orb, std::vector<CORBA::Address *> addrs,
		  WATMGlue::BridgeRequestCallback *proxy);
    ~AccessBridge ();
};

}

#endif // __mico_bridge_h__
