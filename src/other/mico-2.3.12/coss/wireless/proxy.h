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

#ifndef __mico_proxy_h__
#define __mico_proxy_h__

/*
 * The proxy classes implement the GIOP Tunneling Protocol (GTP).  On
 * the lower levels, the implementation follows closely MICO's own
 * GIOP implementation (in iop.h and iop.cc).  The actual protocol
 * machine (implemented by GTPTerminal and GTPNetwork for the terminal
 * and network sides, respectively) differs more from MICO's IIOP
 * machines.  In all contexts, the term proxy refers to the GTP
 * handling part.
 */
namespace WATMProxy {

class GTPCodec;

/*
 * The GTP context classes are almost the same as the GIOP context
 * classes.  The only difference is that the sequence numbers required
 * in the GTP header are also stored here, since they are needed in
 * various places during message processing.
 */
class GTPInContext {
    CORBA::DataDecoder *_dc;
    CORBA::Buffer *_buf;
    CORBA::Boolean _delete_buf;
    CORBA::UShort _own_seq, _peer_seq;
public:
    GTPInContext (GTPCodec *, CORBA::Buffer *);
    ~GTPInContext ();
    CORBA::DataDecoder *dc()
	{ return _dc; }

    void buffer (CORBA::Buffer *);
    CORBA::DataDecoder *_retn();
    void get_seq_nos (CORBA::UShort &own, CORBA::UShort &peer);
    void set_seq_nos (CORBA::UShort own, CORBA::UShort peer);
};

class GTPOutContext {
    CORBA::DataEncoder *_ec;
    CORBA::Buffer *_buf;
    CORBA::Boolean _delete_buf;
    CORBA::UShort _own_seq, _peer_seq;
public:
    GTPOutContext (GTPCodec *, CORBA::Buffer *buf = NULL);
    GTPOutContext (CORBA::DataEncoder *);
    ~GTPOutContext ();
    CORBA::DataEncoder *ec()
	{ return _ec; }

    CORBA::Buffer *_retn();
    void get_seq_nos (CORBA::UShort &own, CORBA::UShort &peer);
    void set_seq_nos (CORBA::UShort own, CORBA::UShort peer);
};

#define GTP_BYTEORDER_BIT 0x80

class GTPCodec;
typedef GTPCodec *GTPCodec_ptr;
typedef ObjVar<GTPCodec> GTPCodec_var;

class GTPCodec : public CORBA::ServerlessObject {
    CORBA::DataDecoder *_dc_proto;
    CORBA::DataEncoder *_ec_proto;
    CORBA::ULong _headerlen;
    CORBA::UShort _gtp_ver;

    CORBA::ULong put_header (GTPOutContext &, GTP::MsgType type);
    void put_size (GTPOutContext &, CORBA::ULong offset);
    void put_target (GTPOutContext &, const WATMGlue::TargetAddress *);
    void put_transport_address (GTPOutContext &,
				MobileTerminal::AccessBridgeTransportAddress);

    CORBA::Boolean get_target (GTPInContext &, WATMGlue::TargetAddress *);
    CORBA::Boolean get_transport_address (GTPInContext &,
					  MobileTerminal::AccessBridgeTransportAddress &);
public:
    GTPCodec (CORBA::DataDecoder *, CORBA::DataEncoder *, CORBA::UShort ver);
    ~GTPCodec ();

    CORBA::DataDecoder *dc_proto ()
	{ return _dc_proto; }
    CORBA::DataEncoder *ec_proto ()
	{ return _ec_proto; }
    CORBA::UShort version () const
	{ return _gtp_ver; }
    CORBA::ULong header_length ()
	{ return _headerlen; }

    CORBA::Boolean check_header (GTPInContext &, GTP::MsgType &type,
				 CORBA::UShort &size, CORBA::Octet &flags);

    CORBA::Boolean put_idle_sync (GTPOutContext &);

    CORBA::Boolean put_initial_request (GTPOutContext &, CORBA::OctetSeq tid,
					MobileTerminal::HomeLocationAgent_ptr,
					CORBA::ULong ttl);
    CORBA::Boolean put_initial_reply (GTPOutContext &, GTP::AccessStatus,
				      MobileTerminal::AccessBridge_ptr,
				      CORBA::ULong ttl);
    CORBA::Boolean put_recovery_request (GTPOutContext &, GTP::RequestType,
					 CORBA::OctetSeq tid,
					 MobileTerminal::HomeLocationAgent_ptr,
					 MobileTerminal::AccessBridge_ptr old,
					 CORBA::ULong old_ttl,
					 CORBA::UShort old_seq,
					 CORBA::ULong ttl);
    CORBA::Boolean put_recovery_reply (GTPOutContext &, GTP::ReplyType,
				       GTP::AccessStatus,
				       MobileTerminal::AccessBridge_ptr,
				       CORBA::ULong old_ttl,
				       CORBA::UShort old_seq,
				       CORBA::ULong ttl);
    CORBA::Boolean put_release_request (GTPOutContext &, CORBA::ULong ttl);
    CORBA::Boolean put_release_reply (GTPOutContext &, CORBA::ULong ttl);

    CORBA::Boolean put_handoff_request (GTPOutContext &,
					MobileTerminal::AccessBridgeTransportAddressList);
    CORBA::Boolean put_handoff_completed (GTPOutContext &,
					  MobileTerminal::HandoffStatus);

    CORBA::Boolean put_open_request (GTPOutContext &,
				     CORBA::ULong req_id,
				     CORBA::ULong tout,
				     const WATMGlue::TargetAddress *);
    CORBA::Boolean put_open_reply (GTPOutContext &,
				   CORBA::ULong req_id,
				   CORBA::ULong conn_id,
				   GTP::OpenConnectionStatus);
    CORBA::Boolean put_close_request (GTPOutContext &,
				      CORBA::ULong conn_id);
    CORBA::Boolean put_close_reply (GTPOutContext &,
				    CORBA::ULong conn_id,
				    GTP::CloseConnectionStatus);
    CORBA::Boolean put_close_indication (GTPOutContext &, CORBA::ULong conn_id,
					 GTP::ConnectionCloseReason);

    CORBA::Boolean put_giop_data (GTPOutContext &,
				  CORBA::ULong conn_id,
				  CORBA::ULong mesg_id,
				  const CORBA::Buffer *);
    CORBA::Boolean put_giop_data_error (GTPOutContext &,
					CORBA::ULong mesg_id,
					GTP::DeliveryStatus);
    CORBA::Boolean put_gtp_forward (GTPOutContext &,
				    MobileTerminal::AccessBridge_ptr,
				    CORBA::ULong mesg_id,
				    const CORBA::Buffer *);
    CORBA::Boolean put_gtp_forward_reply (GTPOutContext &,
					  CORBA::ULong mesg_id,
					  GTP::ForwardStatus);

    CORBA::Boolean put_error (GTPOutContext &, GTP::ErrorCode,
			      CORBA::UShort seq_no);

    CORBA::Boolean get_header (GTPInContext &, GTP::MsgType &type,
			       CORBA::UShort &size, CORBA::Octet &flags);

    CORBA::Boolean get_establish_request (GTPInContext &, GTP::RequestType &,
					  CORBA::OctetSeq *tid,
					  MobileTerminal::HomeLocationAgent_out,
					  MobileTerminal::AccessBridge_out old,
					  CORBA::ULong &old_ttl,
					  CORBA::UShort &old_seq,
					  CORBA::ULong &ttl);
    CORBA::Boolean get_establish_reply (GTPInContext &, GTP::ReplyType &,
					GTP::AccessStatus &,
					MobileTerminal::AccessBridge_out,
					CORBA::ULong &old_ttl,
					CORBA::UShort &old_seq,
					CORBA::ULong &ttl);
    CORBA::Boolean get_release_request (GTPInContext &, CORBA::ULong &ttl);
    CORBA::Boolean get_release_reply (GTPInContext &, CORBA::ULong &ttl);

    CORBA::Boolean get_handoff_request (GTPInContext &,
					MobileTerminal::AccessBridgeTransportAddressList_out);
    CORBA::Boolean get_handoff_completed (GTPInContext &,
					  MobileTerminal::HandoffStatus &);

    CORBA::Boolean get_open_request (GTPInContext &,
				     CORBA::ULong &req_id,
				     CORBA::ULong &tout,
				     WATMGlue::TargetAddress *);
    CORBA::Boolean get_open_reply (GTPInContext &,
				   CORBA::ULong &req_id,
				   CORBA::ULong &conn_id,
				   GTP::OpenConnectionStatus &);
    CORBA::Boolean get_close_request (GTPInContext &,
				      CORBA::ULong &conn_id);
    CORBA::Boolean get_close_reply (GTPInContext &,
				    CORBA::ULong &conn_id,
				    GTP::CloseConnectionStatus &);
    CORBA::Boolean get_close_indication (GTPInContext &, CORBA::ULong &conn_id,
					 GTP::ConnectionCloseReason &);

    CORBA::Boolean get_giop_data (GTPInContext &,
				  CORBA::ULong &conn_id,
				  CORBA::ULong &mesg_id,
				  CORBA::Buffer *);
    CORBA::Boolean get_giop_data_error (GTPInContext &,
					CORBA::ULong &mesg_id,
					GTP::DeliveryStatus &);
    CORBA::Boolean get_gtp_forward (GTPInContext &,
				    MobileTerminal::AccessBridge_out,
				    CORBA::ULong &mesg_id,
				    CORBA::Buffer *);
    CORBA::Boolean get_gtp_forward_reply (GTPInContext &,
					  CORBA::ULong &mesg_id,
					  GTP::ForwardStatus &);

    CORBA::Boolean get_error (GTPInContext &, GTP::ErrorCode &,
			      CORBA::UShort &seq_no);
};

struct GTPConnCallback;

class GTPConn : public CORBA::TransportCallback,
		public CORBA::DispatcherCallback {
    CORBA::Dispatcher *_disp;
    CORBA::ORB_ptr _orb;
    CORBA::Transport *_transp;
    GTPConnCallback *_cb;
    GTPCodec_ptr _codec;
    CORBA::ULong _id;
    CORBA::Boolean _idle;

    CORBA::Buffer *_inbuf;
    CORBA::Buffer *_inbufs;
    CORBA::UShort _inlen;
    CORBA::Octet _inflags;
    GTPInContext _inctx;

    std::list<CORBA::Buffer *> _outbufs;

    void do_read ();
    void do_write ();
public:
    GTPConn (CORBA::ORB_ptr, CORBA::Transport *, GTPConnCallback *,
	     GTPCodec *, CORBA::Long tout = 0,
	     CORBA::ULong id = WATMGlue::IdGenerator::INVALID_ID);
    virtual ~GTPConn ();

    void callback (CORBA::Dispatcher *, CORBA::Dispatcher::Event);
    void callback (CORBA::Transport *, CORBA::TransportCallback::Event);

    void output (CORBA::Buffer *);
    CORBA::Buffer *input ();

    void id (CORBA::ULong id) { _id = id; }
    CORBA::ULong id () { return _id; }

    void idle (CORBA::Boolean idle) { _idle = idle; }
    CORBA::Boolean idle () { return _idle; }

    GTPCodec *codec ()
	{ return _codec; }
    CORBA::Transport *transport ()
	{ return _transp; }
};

struct GTPConnCallback {
    enum Event { InputReady, Closed, Idle };
    virtual CORBA::Boolean callback (GTPConn *, Event) = 0;
    virtual ~GTPConnCallback () {}
};

/*
 * GTPAddress models MobileTerminal::AccessBridgeTransportAddress.
 * The purpose is to convert transport addresses in handoff to MICO's
 * CORBA::Address type.
 */
struct GTPAddress {
    CORBA::Octet protocol;
    CORBA::Octet level;
    CORBA::Short version;
    CORBA::OctetSeq address;

    GTPAddress ();
    GTPAddress (MobileTerminal::AccessBridgeTransportAddress);
    CORBA::Address *make_address ();
    MobileTerminal::AccessBridgeTransportAddress make_transport_address ();

private:
    std::string proto_str (CORBA::Octet proto_id);
    std::string stringify (CORBA::Octet proto_id, CORBA::OctetSeq address);
};

enum MobilityEvent { NET_DEPART, NET_ARRIVE, NET_DROPOUT,
		     NET_RECOVERY, TERM_HANDOFF, TERM_DROPOUT,
		     TERM_RECOVERY };

/*
 * GTPProxy is a common base class for GTPTerminal and GTPNetwork.
 * Since GTP handling is mostly similar on both terminal and network
 * sides, this class contains implementations for almost all required
 * functions.
 */
class GTPProxy : public WATMGlue::BridgeRequestCallback,
		 public GTPConnCallback {
protected:
    /*
     * An IdleTimer object is created for every GTP connection.  It is
     * enabled after receiving a message and disabled after sending a
     * message.  If the timeout event happens, an IdleSync message is
     * sent to the other side.  After sending this, the connection is
     * marked idle, so new IdleSync messages are not sent until some
     * real data goes over the connection (by giving up this, the
     * IdleTimer would become a heartbeat, sending IdleSyncs
     * periodically).  REC_TIMEOUT is a suitable timeout value.
     */
    class IdleTimer : public CORBA::DispatcherCallback {
	CORBA::ORB_ptr orb;
	CORBA::Long timeout;
	GTPConn *conn;
	GTPProxy *parent;
	CORBA::Boolean dirty;
    public:
	static const CORBA::Long REC_TIMEOUT;

	IdleTimer (CORBA::ORB_ptr o, GTPProxy *p, GTPConn *c,
		   CORBA::Long t = REC_TIMEOUT)
	    : orb(o), conn(c), parent(p) {
	    timeout = t * 1000L;
	    if (timeout > 0)
		orb->dispatcher()->tm_event(this, timeout);
	    dirty = TRUE;
	}
	~IdleTimer () {
	    if (timeout > 0)
		orb->dispatcher()->remove(this, CORBA::Dispatcher::Timer);
	}
	void enable () {
	    dirty = TRUE;
	}
	void disable () {
	    dirty = FALSE;
	}
	void callback (CORBA::Dispatcher *d, CORBA::Dispatcher::Event ev) {
	    if (ev == CORBA::Dispatcher::Timer) {
		if (dirty && !conn->idle()) {
		    GTPOutContext out(conn->codec());
		    parent->set_seq_nos(out, conn);
		    conn->codec()->put_idle_sync(out);
		    parent->output(out, conn);
		    dirty = FALSE;
		    conn->idle(TRUE);
		}
		if (timeout > 0)
		    orb->dispatcher()->tm_event(this, timeout);
	    }
	}
    };
    friend class IdleTimer;

    /*
     * A ReleaseTimer is used to establish a Timer event in the
     * dispatcher when a GTP connection is released (the ReleaseTimer
     * is created at connection establishment and activated at
     * connection release).  The proxy needs to know about the other
     * side until the event occurs, at which point all information
     * regarding the connection can be forgotten.  REC_TIMEOUT gives a
     * suitable value for the timeout.  The value given below is
     * suitable for only testing purposes where we need to see the
     * release happen; in real conditions a better value would be
     * something like 5-15 minutes depending on the connection type.
     */
    class ReleaseTimer : public CORBA::DispatcherCallback {
	CORBA::ORB_ptr _orb;
	CORBA::Long _timeout;
	GTPProxy *_parent;
	GTPConn *_conn;
	CORBA::Boolean _active;
    public:
	static const CORBA::Long REC_TIMEOUT;

	ReleaseTimer (CORBA::ORB_ptr o, GTPProxy *p, GTPConn *c,
		      CORBA::Long t = REC_TIMEOUT)
	    : _orb(o), _parent(p), _conn(c), _active(FALSE) {
	    _timeout = t * 1000L;
	}
	~ReleaseTimer () {
	    if (_active)
		_orb->dispatcher()->remove(this, CORBA::Dispatcher::Timer);
	}
	void timeout (CORBA::ULong t) {
	    if (!_active)
		_timeout = t * 1000L;
	}
	void activate () {
	    _orb->dispatcher()->tm_event(this, _timeout);
	    _active = TRUE;
	}
	void deactivate () {
	    _orb->dispatcher()->remove(this, CORBA::Dispatcher::Timer);
	    _active = FALSE;
	}
	// A crude hack so that release is handled only by the timer
	void force () {
	    if (_active) {
		callback(_orb->dispatcher(), CORBA::Dispatcher::Timer);
		deactivate();
	    }
	}
	void callback (CORBA::Dispatcher *d, CORBA::Dispatcher::Event ev) {
	    if (ev == CORBA::Dispatcher::Timer) {
		if (_parent->timed_release(_conn))
		    _parent->release_conn(_conn);
	    }
	}
    };
    friend class ReleaseTimer;

    typedef std::map<CORBA::ULong, std::vector<WATMGlue::BridgeRequest *> > MapIdReq;
    typedef std::map<std::pair<CORBA::ULong, CORBA::ULong>, CORBA::ULong> MapPairId;
    typedef std::map<CORBA::ULong, std::pair<CORBA::ULong, CORBA::ULong> > MapIdPair;
    typedef std::map<CORBA::ULong, IdleTimer *> MapIdITimer;
    typedef std::map<CORBA::ULong, ReleaseTimer *> MapIdRTimer;

    typedef std::list<std::pair<CORBA::ULong, CORBA::Buffer *> > MessQueue;
    typedef std::map<CORBA::UShort, CORBA::ULong> MapSnoId;
    struct SequenceInfo {
	CORBA::UShort last_rec;
	CORBA::UShort last_sent;
	MessQueue unacked;
	MapSnoId giops;
	SequenceInfo () : last_rec(0), last_sent(0) {}
    };
    typedef std::map<CORBA::ULong, SequenceInfo *> MapIdSeq;

    /*
     * There are quite many maps that use some sort of id as their key
     * field.  Of the maps below, _pending_requests is used to store
     * Invoke events received after an Open event but before an
     * OpenReply message has been received.  _conn_statuses maps each
     * GTP connection id to a SequenceInfo that stores the relevant
     * sequence numbers and all unacked messages (in case they need to
     * be retransmitted) of a GTP connection.
     *
     * Since an Open callback returns before an OpenReply is received,
     * the GIOP connection id given to the bridge cannot be the same
     * as the GIOP connection id used on the GTP connection.  The
     * required conversions between these ids are handled by two maps
     * between the bridge's connection ids and pairs containing the
     * GTP connection id and the GIOP connection id that was received
     * on the identified connection.  Of course, when this end is the
     * receiver of the OpenRequest, the GIOP connection ids can be
     * (and are) made equal.
     */
    MapIdReq _pending_requests;
    MapIdSeq _conn_statuses;
    MapIdPair _bridge_to_proxy;
    MapPairId _proxy_to_bridge;
    MapIdITimer _idle_timers;
    MapIdRTimer _rel_timers;

    /*
     * This set is used to store the Bridge identifiers of GIOP
     * connections for which a CloseConnectionRequest message has been
     * sent but reply has not yet been received.  This is needed so
     * that a CloseConnectionRequest message is not sent twice for the
     * same connection.
     */
    std::set<CORBA::ULong> _pending_closes;

    WATMGlue::IdGenerator _id_gen;

    CORBA::ORB_ptr _orb;
    CosEventComm::PushConsumer_ptr _mob_chan;
    WATMGlue::BridgeRequestCallback *_cb;
    CORBA::UShort _gtp_ver;

    virtual void set_seq_nos (GTPOutContext &, GTPConn *);
    virtual void replace_seq_nos (GTPOutContext &);
    /*
     * Outputs a message on the given connection.  Needs to be
     * overridden in derived classes since GTPProxy does not have the
     * machinery for handoff.
     */
    virtual void output (GTPOutContext &, GTPConn *);
    /*
     * Deletes all messages in the connection's unacked queue that
     * were sent before message number seq_no.
     */
    virtual void clear_unacked (SequenceInfo *, CORBA::UShort seq_no);

    /*
     * These are for managing the maps _bridge_to_proxy and
     * _proxy_to_bridge.  Originally they were pure virtual since
     * these maps were different in GTPTerminal and GTPNetwork, but
     * now everything is here.
     */
    virtual void insert_correspondence (CORBA::ULong brid, CORBA::ULong prid,
					CORBA::ULong cid);
    virtual CORBA::Boolean fetch_proxy (CORBA::ULong brid, CORBA::ULong &prid,
					CORBA::ULong &cid);
    virtual CORBA::Boolean fetch_bridge (CORBA::ULong &brid, CORBA::ULong prid,
					 CORBA::ULong cid);
    virtual void delete_from_proxy (CORBA::ULong prid, CORBA::ULong cid);
    virtual void delete_from_bridge (CORBA::ULong brid);

    /*
     * These are all related to GTP connection closing and associated
     * resource releasing.  close_conn sends ReleaseRequest, kill_conn
     * closes a connection and activates the release timer,
     * release_conn releases all associated resources (and should be
     * called only by the release timer), timed_release is called by
     * the release timer just before resource release, conn_error
     * sends an Error message and forcibly shuts the connection down
     * and conn_closed is called when the connection is closed for any
     * reason.
     */
    void close_conn (GTPConn *);
    virtual void kill_conn (GTPConn *) = 0;
    virtual void release_conn (GTPConn *) = 0;
    virtual CORBA::Boolean timed_release (GTPConn *)
	{ return TRUE; }
    void conn_error (GTPConn *, GTPInContext &, GTP::ErrorCode);
    virtual void conn_closed (GTPConn *) = 0;

    virtual CORBA::Boolean open_connection (WATMGlue::BridgeRequest *) = 0;
    virtual CORBA::Boolean invoke (WATMGlue::BridgeRequest *) = 0;
    virtual CORBA::Boolean close_connection (WATMGlue::BridgeRequest *) = 0;

    CORBA::Boolean handle_input (GTPConn *);
    CORBA::Boolean handle_input (GTPConn *, CORBA::Buffer *);

    virtual CORBA::Boolean handle_establish_request (GTPConn *,
						     GTPInContext &);
    virtual CORBA::Boolean handle_establish_reply (GTPConn *, GTPInContext &);
    virtual CORBA::Boolean handle_release_request (GTPConn *, GTPInContext &);
    virtual CORBA::Boolean handle_release_reply (GTPConn *, GTPInContext &);
    virtual CORBA::Boolean handle_handoff_request (GTPConn *, GTPInContext &);
    virtual CORBA::Boolean handle_handoff_reply (GTPConn *, GTPInContext &);

    virtual CORBA::Boolean handle_open_request (GTPConn *, GTPInContext &);
    virtual CORBA::Boolean handle_open_reply (GTPConn *, GTPInContext &);
    virtual CORBA::Boolean handle_close_request (GTPConn *, GTPInContext &);
    virtual CORBA::Boolean handle_close_reply (GTPConn *, GTPInContext &);
    virtual CORBA::Boolean handle_close_indication (GTPConn *, GTPInContext &);
    virtual CORBA::Boolean handle_giop_data (GTPConn *, GTPInContext &);
    virtual CORBA::Boolean handle_giop_data_error (GTPConn *, GTPInContext &);

    virtual CORBA::Boolean handle_gtp_forward (GTPConn *, GTPInContext &);
    virtual CORBA::Boolean handle_gtp_forward_reply (GTPConn *,
						     GTPInContext &);
public:
    GTPProxy (CORBA::ORB_ptr, CORBA::UShort gtp_ver = 0x0100,
	      WATMGlue::IdGenerator id_gen = WATMGlue::IdGenerator(),
	      CosEventComm::PushConsumer_ptr chan
	      = CosEventComm::PushConsumer::_nil());
    virtual ~GTPProxy ();

    void brselect (WATMGlue::BridgeRequestCallback *);
    void mobility_event (MobilityEvent, MobileTerminal::TerminalId,
			 MobileTerminal::AccessBridge_ptr);

    CORBA::Boolean callback (GTPConn *, GTPConnCallback::Event);
    CORBA::Boolean callback (WATMGlue::BridgeRequest *,
			     WATMGlue::BridgeRequestCallback::Event);
};

/*
 * GTPTerminal implements the GTP part of the Terminal Bridge.  GTP
 * connections are managed with ConnInfo structures.  Each ConnInfo
 * contains the actual connection, the Access Bridge reference (used
 * in handoffs and forwarding) and the Access Bridge's address (for
 * reconnecting after connection loss).  One of these connections is
 * designated active (identified by _active_id) and is the connection
 * through which all traffic goes.
 */
class GTPTerminal : public GTPProxy {
    struct ConnInfo {
	GTPConn *conn;
	MobileTerminal::AccessBridge_ptr ab;
	CORBA::Address *ab_addr;
	CORBA::ULong ttl;

	ConnInfo () : conn(NULL), ab(MobileTerminal::AccessBridge::_nil()),
		      ab_addr(NULL), ttl(0)
	    {}
	~ConnInfo () {
	    delete conn;
	    delete ab_addr;
	    CORBA::release(ab);
	}
    };

    typedef std::map<CORBA::ULong, ConnInfo *> MapIdInfo;

    struct rc :
	std::binary_function<const CORBA::Object_ptr, const CORBA::Object_ptr, bool>
    {
	bool operator() (const CORBA::Object_ptr o1,
			 const CORBA::Object_ptr o2) const
	    {
		if (CORBA::is_nil(o1))
		    return !CORBA::is_nil(o2);
		if (CORBA::is_nil(o2))
		    return FALSE;
		return *o1->_ior() < *o2->_ior();
	    }
    };
    typedef std::map<CORBA::Object_ptr, CORBA::ULong, rc> MapRefId;

    CORBA::OctetSeq _terminal_id;
    MobileTerminal::HomeLocationAgent_ptr _hla;

    CORBA::ULong _active_id;
    CORBA::Boolean _connected;
    CORBA::Boolean _establish_in_progress;
    MapIdInfo _all_conns;
    MapRefId _past_conns;

    GTPConn *make_conn (CORBA::Address *,
			CORBA::ULong id = WATMGlue::IdGenerator::INVALID_ID);
    void kill_conn (GTPConn *);
    void release_conn (GTPConn *);
    void conn_closed (GTPConn *);

    void output (GTPOutContext &, GTPConn *);

    CORBA::Boolean open_connection (WATMGlue::BridgeRequest *);
    CORBA::Boolean invoke (WATMGlue::BridgeRequest *);
    CORBA::Boolean close_connection (WATMGlue::BridgeRequest *);

    CORBA::Boolean handle_establish_reply (GTPConn *, GTPInContext &);
    CORBA::Boolean handle_handoff_request (GTPConn *, GTPInContext &);

    CORBA::Boolean handle_gtp_forward (GTPConn *, GTPInContext &);
    CORBA::Boolean handle_gtp_forward_reply (GTPConn *, GTPInContext &);
public:
    GTPTerminal (CORBA::ORB_ptr, CORBA::Address *,
		 CORBA::UShort gtp_ver = 0x0100,
		 CosEventComm::PushConsumer_ptr chan
		 = CosEventComm::PushConsumer::_nil());
    ~GTPTerminal ();

    CORBA::Boolean active (CORBA::ULong conn_id);

    CORBA::Boolean ensure_connectivity (CORBA::Address *addr = NULL);
    MobileTerminal::AccessBridge_ptr current_connect_point ();

    /*
     * These are useful functions for debugging.  The terminal admin
     * calls them in response to control messages.  break_connectivity
     * releases the connection gracefully and handoff does a terminal
     * initiated handoff to the address given.
     */
    void break_connectivity (CORBA::Boolean notify_ab);
    CORBA::Boolean handoff (CORBA::Address *);
};

/*
 * GTPNetwork implements the GTP part of the Access Bridge.  GTP
 * connections are represented by ConnInfo structures.  Each ConnInfo
 * contains the actual connection, the id of the terminal on the other
 * end (used in forwarding), an indication whether the connection is
 * actually up and (after handoff) the Access Bridge that is currently
 * serving the terminal (used in forwarding).
 */
class GTPNetwork : public GTPProxy, public CORBA::TransportServerCallback {
    /*
     * The ForwardTimer is used when the terminal loses connectivity
     * and recovers later at a different Access Bridge.  The problem
     * in this case is that the old Access Bridge cannot forward the
     * unsent messages in recovery_request, since that is too early,
     * but after recovery_request the old Access Bridge no longer has
     * anything to do with the terminal.  So it sets up a timer that
     * does the forwarding when going off.
     */
    class ForwardTimer : public CORBA::DispatcherCallback {
	CORBA::ORB_ptr orb;
	GTPNetwork *parent;
	CORBA::ULong conn_id;
	CORBA::Long timeout;
	CORBA::Boolean is_triggered;
    public:
	static const CORBA::Long REC_TIMEOUT;

	ForwardTimer (CORBA::ORB_ptr o, GTPNetwork *p, CORBA::ULong c,
		      CORBA::Long t = REC_TIMEOUT)
	    : orb(o), parent(p), conn_id(c) {
	    timeout = t * 1000L;
	    if (timeout > 0)
		orb->dispatcher()->tm_event(this, timeout);
	    is_triggered = FALSE;
	}
	~ForwardTimer () {
	    if (timeout > 0)
		orb->dispatcher()->remove(this, CORBA::Dispatcher::Timer);
	}
	void force () {
	    if (!is_triggered && timeout > 0) {
		parent->forward_old_messages(conn_id);
		is_triggered = TRUE;
	    }
	}
	void callback (CORBA::Dispatcher *d, CORBA::Dispatcher::Event ev) {
	    if (ev == CORBA::Dispatcher::Timer) {
		if (!is_triggered) {
		    parent->forward_old_messages(conn_id);
		    is_triggered = TRUE;
		}
	    }
	}
    };
    friend class ForwardTimer;
    typedef std::map<CORBA::ULong, ForwardTimer *> MapIdFTimer;

    struct ConnInfo {
	typedef std::list<MobileTerminal::AccessBridge_ptr> ABList;

	GTPConn *conn;
	CORBA::OctetSeq terminal_id;
	CORBA::Boolean is_up;
	MobileTerminal::AccessBridge_ptr forward;
	MobileTerminal::HomeLocationAgent_ptr hla;
	CORBA::ULong ttl;
	ABList past_abs;

	ConnInfo () : conn(NULL), is_up(FALSE),
		      forward(MobileTerminal::AccessBridge::_nil()),
		      hla(MobileTerminal::HomeLocationAgent::_nil()),
		      ttl(0)
	    {}
	~ConnInfo () {
	    delete conn;
	    CORBA::release(forward);
	    CORBA::release(hla);
	    for (ABList::iterator i = past_abs.begin();
		 i != past_abs.end(); ++i) {
		CORBA::release(*i);
	    }
	}
    };
    typedef std::map<CORBA::ULong, ConnInfo *> MapIdInfo;

    struct ic :
	std::binary_function<const MobileTerminal::TerminalId,
	                     const MobileTerminal::TerminalId,
			     bool>
    {
	bool operator() (const MobileTerminal::TerminalId id1,
			 const MobileTerminal::TerminalId id2) const
	    {
		if (id1.length() != id2.length())
		    return id1.length() < id2.length();
		return mico_key_compare(id1.get_buffer(), id2.get_buffer(),
					id1.length()) < 0;
	    }
    };
    typedef std::map<MobileTerminal::TerminalId, CORBA::ULong, ic> MapTidId;

    /*
     * The _conn_ids map maps a terminal id to a GTP connection id
     * which can then be used to index other maps.  It must be done
     * this way, since sometimes the available information is just the
     * terminal id, sometimes just the connection id.
     */
    MapTidId _conn_ids;
    MapIdInfo _all_conns;
    MapIdFTimer _pending_forwards;

    /*
     * This one is a temporary map so we can send gtp_acknowledge's.
     */
    typedef std::map<CORBA::ULong,
		     std::pair<CORBA::ULong,
			       MobileTerminal::AccessBridge_ptr>
                    > MapIdForw;
    MapIdForw _forwarded_gtps;

    MobileTerminal::AccessBridge_ptr _ab_ref;
    std::vector<CORBA::TransportServer *> _tservers;

    void kill_conn (GTPConn *);
    void release_conn (GTPConn *);
    void conn_closed (GTPConn *);
    CORBA::Boolean timed_release (GTPConn *);

    GTP::AccessStatus update_home (MobileTerminal::HomeLocationAgent_ptr,
				   CORBA::OctetSeq *tid);

    void output (GTPOutContext &, GTPConn *);

    CORBA::Boolean open_connection (WATMGlue::BridgeRequest *);
    CORBA::Boolean invoke (WATMGlue::BridgeRequest *);
    CORBA::Boolean close_connection (WATMGlue::BridgeRequest *);

    CORBA::Boolean handle_establish_request (GTPConn *, GTPInContext &);
    CORBA::Boolean handle_release_request (GTPConn *, GTPInContext &);
    CORBA::Boolean handle_handoff_reply (GTPConn *, GTPInContext &);

    CORBA::Boolean handle_gtp_forward (GTPConn *, GTPInContext &);
    CORBA::Boolean handle_gtp_forward_reply (GTPConn *, GTPInContext &);

    WATMGlue::TargetAddress *map_to_forward (WATMGlue::TargetAddress *,
					     CORBA::IOR *ior = NULL);
    void forward_old_messages (CORBA::ULong conn_id);
public:
    GTPNetwork (CORBA::ORB_ptr, std::vector<CORBA::Address *>,
		CORBA::UShort gtp_ver = 0x0100,
		CosEventComm::PushConsumer_ptr chan
		= CosEventComm::PushConsumer::_nil());
    ~GTPNetwork ();

    CORBA::Boolean active (CORBA::ULong conn_id);

    void callback (CORBA::TransportServer *,
		   CORBA::TransportServerCallback::Event);
    void abselect (MobileTerminal::AccessBridge_ptr);

    CORBA::Boolean is_here (MobileTerminal::TerminalId);
    CORBA::Boolean is_known (MobileTerminal::TerminalId);
    MobileTerminal::HomeLocationAgent_ptr
        terminal_home (MobileTerminal::TerminalId);
    CORBA::Boolean change_forward (MobileTerminal::TerminalId,
				   MobileTerminal::AccessBridge_ptr);
    CORBA::Boolean do_handoff (MobileTerminal::TerminalId,
			       MobileTerminal::AccessBridgeTransportAddressList *);
    CORBA::Boolean add_notice (MobileTerminal::TerminalId,
			       MobileTerminal::AccessBridge_ptr);
    void notify_handoff (MobileTerminal::TerminalId);
    void prepare_forwarding (MobileTerminal::TerminalId);

    /*
     * terminal_moved() is called to indicate that the terminal has
     * moved to a new Access Bridge and forwarding needs to be set up.
     * If the terminal is not here, the return value is 0xFFFFFFFFUL.
     */
    CORBA::ULong terminal_moved (MobileTerminal::TerminalId,
				 MobileTerminal::AccessBridge_ptr,
				 CORBA::ULong seqno = 0xFFFFFFFFUL);
    CORBA::Boolean terminal_forward (MobileTerminal::TerminalId,
				     MobileTerminal::AccessBridge_ptr,
				     CORBA::ULong gtp_mesg_id,
				     MobileTerminal::GTPEncapsulation);
    CORBA::Boolean net_forward (MobileTerminal::TerminalId,
				MobileTerminal::GTPEncapsulation);
};

}

#endif // __mico_proxy_h__
