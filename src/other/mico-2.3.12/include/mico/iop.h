// -*- c++ -*-
/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2005 by The Mico Team
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

#ifndef __mico_iop_h__
#define __mico_iop_h__


#ifndef HAVE_THREADS
#  define USE_IOP_CACHE
#endif

// KG-NOTE: Quick and dirty fix
#undef USE_IOP_CACHE

namespace MICO {

class GIOPCodec;

class GIOPInContext {
    CORBA::DataDecoder *_dc;
    CORBA::Buffer *_buf;
    CORBA::Boolean _delete_buf;
    CORBA::Boolean _delete_dc;

public:
    GIOPInContext (GIOPCodec *, CORBA::Buffer *,
		   CORBA::CodeSetCoder *csc = 0);
    ~GIOPInContext ();
    void converters (CORBA::CodeSetCoder *csc);
    CORBA::DataDecoder *dc()
    { return _dc; }

    void buffer (CORBA::Buffer *);
    CORBA::DataDecoder *_retn();
};

class GIOPOutContext {
    CORBA::DataEncoder *_ec;
    CORBA::Buffer *_buf;
    CORBA::Boolean _delete_buf;
    CORBA::Boolean _delete_ec;

public:
    GIOPOutContext (GIOPCodec *, CORBA::CodeSetCoder *csc = 0);
    GIOPOutContext (CORBA::DataEncoder *);
    ~GIOPOutContext ();
    void converters (CORBA::CodeSetCoder *csc);
    CORBA::DataEncoder *ec()
    { return _ec; }

    void reset ();

    CORBA::Buffer *_retn();
};

#define GIOP_BYTEORDER_BIT 1
#define GIOP_FRAGMENT_BIT 2

class GIOPCodec;
typedef GIOPCodec *GIOPCodec_ptr;
typedef ObjVar<GIOPCodec> GIOPCodec_var;

class GIOPCodec : public CORBA::ServerlessObject {
    CORBA::DataDecoder *_dc_proto;
    CORBA::DataEncoder *_ec_proto;
    CORBA::ULong _headerlen;
    CORBA::ULong _size_offset;
    CORBA::UShort _giop_ver;
    CORBA::CodeSetCoder * _conv;
    CORBA::Codeset::CodesetId _csid;
    CORBA::Codeset::CodesetId _wcsid;
    CORBA::Boolean _codesets_sent;

    CORBA::ULong put_header (GIOPOutContext &out, GIOP::MsgType);
    void put_size (GIOPOutContext &out, CORBA::ULong key);
    void put_contextlist (GIOPOutContext &out,
			  const IOP::ServiceContextList &ctx,
			  CORBA::Boolean codesets = FALSE);
    void put_target (GIOPOutContext &out, CORBA::Object_ptr obj);
    CORBA::Boolean put_args (GIOPOutContext &out, CORBA::ORBRequest *,
			     CORBA::Boolean inp);

    CORBA::Boolean get_contextlist (GIOPInContext &in,
				    IOP::ServiceContextList &ctx,
				    CORBA::Boolean codesets = FALSE);
    CORBA::Boolean get_target (GIOPInContext &in, CORBA::Object_ptr obj);
public:
    GIOPCodec (CORBA::DataDecoder *dc, CORBA::DataEncoder *ec,
               CORBA::UShort giop_ver = 0x0100);
    ~GIOPCodec ();

    CORBA::DataDecoder *dc_proto()
    { return _dc_proto; }

    CORBA::DataEncoder *ec_proto()
    { return _ec_proto; }

    CORBA::UShort version () const
    { return _giop_ver; }

    void version (CORBA::UShort ver)
    { _giop_ver = ver; }

    CORBA::ULong header_length ()
    { return _headerlen; }

    CORBA::CodeSetCoder * converter ()
    { return _conv; }

    CORBA::Codeset::CodesetId csid ()
    { return _csid; }

    CORBA::Codeset::CodesetId wcsid ()
    { return _wcsid; }

    CORBA::Boolean setup_codeset_ids (CORBA::Object_ptr);

    CORBA::Boolean put_invoke_request (GIOPOutContext &out,
				       CORBA::ULong req_id,
				       CORBA::Octet response_flags,
				       CORBA::Object_ptr obj,
				       CORBA::ORBRequest *req,
				       CORBA::Principal_ptr pr);

    CORBA:: Boolean put_invoke_reply (GIOPOutContext &out,
				      CORBA::ULong req_id,
				      GIOP::ReplyStatusType,
				      CORBA::Object_ptr obj,
				      CORBA::ORBRequest *req,
				      GIOP::AddressingDisposition);

    CORBA::Boolean put_invoke_reply_offset (GIOPOutContext &out,
					    CORBA::ORBRequest *req);

    CORBA::Boolean put_cancel_request (GIOPOutContext &out,
				       CORBA::ULong req_id);

    CORBA::Boolean put_locate_request (GIOPOutContext &out,
				       CORBA::ULong req_id,
				       CORBA::Object_ptr obj);

    CORBA::Boolean put_locate_reply (GIOPOutContext &out,
				     CORBA::ULong req_id,
				     GIOP::LocateStatusType,
				     CORBA::Object_ptr obj,
				     GIOP::AddressingDisposition);

    CORBA::Boolean put_bind_request (GIOPOutContext &out,
				     CORBA::ULong req_id,
				     const char *repoid,
				     const CORBA::ORB::ObjectTag &);

    CORBA::Boolean put_bind_reply (GIOPOutContext &out,
				   CORBA::ULong req_id,
				   GIOP::LocateStatusType,
				   CORBA::Object_ptr obj);

    CORBA::Boolean put_close_msg (GIOPOutContext &out);

    CORBA::Boolean put_error_msg (GIOPOutContext &out);

    CORBA::Boolean get_header (GIOPInContext &in, GIOP::MsgType &,
			       CORBA::ULong &sz, CORBA::Octet &flags);
    CORBA::Boolean check_header (GIOPInContext &in, GIOP::MsgType &,
				 CORBA::ULong &sz, CORBA::Octet &flags);

    CORBA::Boolean get_invoke_request (GIOPInContext &in,
				       CORBA::ULong &req_id,
				       CORBA::Octet &response_flags,
				       CORBA::Object_ptr obj,
				       CORBA::ORBRequest * &req,
				       CORBA::Principal_ptr pr);


    CORBA::Boolean get_invoke_reply1 (GIOPInContext &in,
				      CORBA::ULong &req_id,
				      GIOP::ReplyStatusType &,
				      IOP::ServiceContextList &ctx);

    CORBA::Boolean get_invoke_reply2 (GIOPInContext &in,
				      CORBA::ULong req_id,
				      GIOP::ReplyStatusType,
				      CORBA::Object_ptr &obj,
				      CORBA::ORBRequest *req,
				      GIOP::AddressingDisposition &,
				      IOP::ServiceContextList &ctx);

    CORBA::Boolean get_cancel_request (GIOPInContext &in,
				       CORBA::ULong &req_id);

    CORBA::Boolean get_locate_request (GIOPInContext &in,
				       CORBA::ULong &req_id,
				       CORBA::Object_ptr obj);

    CORBA::Boolean get_locate_reply (GIOPInContext &in,
				     CORBA::ULong &req_id,
				     GIOP::LocateStatusType &,
				     CORBA::Object_ptr &obj,
				     GIOP::AddressingDisposition &);

    CORBA::Boolean get_bind_request (GIOPInContext &in,
				     CORBA::String_out repoid,
				     CORBA::ORB::ObjectTag &);

    CORBA::Boolean get_bind_reply (GIOPInContext &in,
				   GIOP::LocateStatusType &,
				   CORBA::Object_ptr &obj);

    CORBA::Boolean get_close_msg (GIOPInContext &in);

    CORBA::Boolean get_error_msg (GIOPInContext &in);

    CORBA::Boolean get_fragment_header (GIOPInContext &in,
					CORBA::ULong &req_id,
					CORBA::Boolean &has_id);


    static GIOPCodec_ptr _duplicate (GIOPCodec_ptr o)
    {
	if (o)
	    o->_ref();
	return o;
    }
    static GIOPCodec_ptr _nil ()
    {
	return 0;
    }
};


class GIOPRequest : public CORBA::ORBRequest {
    std::string _opname;

    CORBA::DataDecoder *_idc;
    CORBA::ULong _istart;

    CORBA::DataEncoder *_oec;
    CORBA::DataDecoder *_odc;
    CORBA::ULong _ostart;
    CORBA::Buffer _obuf;

    GIOPCodec_ptr _codec;

    CORBA::Boolean _is_except;
public:
    GIOPRequest (const char *op, CORBA::DataDecoder *indata, GIOPCodec *);
    ~GIOPRequest ();
    
    const char *op_name();
    CORBA::UShort version () const
    { return _codec->version (); }
    
    CORBA::Boolean get_in_args (CORBA::NVList_ptr iparams,
                                CORBA::Context_ptr &ctx);
    CORBA::Boolean get_in_args (StaticAnyList *iparams,
                                CORBA::Context_ptr &ctx);
    CORBA::Boolean get_in_args (CORBA::DataEncoder *);
    
    CORBA::Boolean get_out_args (CORBA::Any *res, CORBA::NVList_ptr oparams,
                                 CORBA::Exception *&ex);
    CORBA::Boolean get_out_args (CORBA::StaticAny *res, StaticAnyList *oparams,
                                 CORBA::Exception *&ex);
    CORBA::Boolean get_out_args (CORBA::DataEncoder *,
                                 CORBA::Boolean &is_except);
    
    CORBA::Boolean set_out_args (CORBA::Any *res, CORBA::NVList_ptr oparams);
    CORBA::Boolean set_out_args (CORBA::StaticAny *res,
                                 StaticAnyList *oparams);
    void set_out_args (CORBA::Exception *ex);
    CORBA::Boolean set_out_args (CORBA::DataDecoder *,
                                 CORBA::Boolean is_except);

    CORBA::Boolean copy_out_args (CORBA::ORBRequest *);
    CORBA::Boolean copy_in_args (CORBA::ORBRequest *);
    
    const char *type ();
    CORBA::ByteOrder input_byteorder ();
    CORBA::ByteOrder output_byteorder ();
    CORBA::CodeSetCoder * converter ();

    static GIOPRequest* _nil ()
    {
	return 0;
    }
    static GIOPRequest* _duplicate (GIOPRequest *o)
    {
	if (o)
	    o->_ref();
	return o;
    }
};


class GIOPSimpleProf : public CORBA::IORProfile {
public:
    CORBA::Octet *_objkey;
    CORBA::ULong _length;
public:
    GIOPSimpleProf ();
    GIOPSimpleProf (const GIOPSimpleProf &);
    ~GIOPSimpleProf ();
    GIOPSimpleProf &operator= (const GIOPSimpleProf &);

    void encode (CORBA::DataEncoder &) const;
    const CORBA::Address *addr () const;
    ProfileId id () const;
    ProfileId encode_id () const;
    void objectkey (CORBA::Octet *, CORBA::Long length);
    const CORBA::Octet *objectkey (CORBA::Long &length) const;
    CORBA::Boolean reachable ();
    void print (std::ostream &) const;
    
    CORBA::IORProfile *clone () const;
    CORBA::Long compare (const CORBA::IORProfile &) const;
    CORBA::Boolean operator== (const CORBA::IORProfile &) const;
    CORBA::Boolean operator< (const CORBA::IORProfile &) const;
};


#ifdef HAVE_THREADS
class GIOPConnReader;
class GIOPConnWriter;

enum GIOPSide
{
    UNSPECIFIED,
    CLIENT_SIDE,
    SERVER_SIDE
};
#endif // HAVE_THREADS
struct GIOPConnCallback;

#ifdef HAVE_THREADS
class GIOPConn : public CORBA::TransportCallback,
		 public CORBA::DispatcherCallback,
                 public MICOMT::StateRefCnt {
#else
class GIOPConn : public CORBA::TransportCallback,
		 public CORBA::DispatcherCallback {
#endif
    CORBA::Dispatcher *_disp;
    CORBA::Transport *_transp;
    std::list<CORBA::Buffer *> _outbufs;
    MICOMT::Mutex write_lock_;
    CORBA::Buffer *_inbufs;
    CORBA::Buffer *_inbuf, *_infrag;
    CORBA::ULong _inlen;
    CORBA::Octet _inflags;
    GIOPConnCallback *_cb;
    GIOPCodec_ptr _codec;
    CORBA::Long _refcnt;
    MICOMT::Mutex refcnt_lock_;
    CORBA::Long _idle_tmout;
    CORBA::Boolean _have_tmout;
    CORBA::Boolean _have_wselect;
    GIOPInContext _inctx;
    CORBA::ULong _max_message_size;
    CORBA::ULong _id;

    CORBA::ULong _total_fragsize;
    std::map<CORBA::ULong, CORBA::Buffer *, std::less<CORBA::ULong> > _fragments;
    void do_write ();

#ifdef HAVE_THREADS
    GIOPConnReader *_reader;
    GIOPConnWriter *_writer;
    MICOMT::Mutex _excl_mutex;

    GIOPSide side_;

    static MICOMT::Thread::ThreadKey S_reader_key_;
    static bool S_reader_key_initialized_;

    CORBA::Boolean _M_use_reader_thread;
    CORBA::Boolean _M_use_writer_thread;

    CORBA::Boolean input_ready();
    void close_connection();

    void check_idle () {};
    void check_busy () {};
#else
    CORBA::Boolean input_ready() {
	return  input_ready_callback( input() );
    };
//      void close_connection() {
//  	_cb->callback (this, GIOPConnCallback::Closed);
//      };
    void close_connection();
    void check_idle ();
    void check_busy ();
#endif // HAVE_THREADS

public:
    void do_read ( const CORBA::Boolean break_after_read = FALSE );
    void output_handler (CORBA::Buffer *);

#ifdef HAVE_THREADS
    void terminate();

    GIOPSide side()
    { return side_; }

    GIOPConn (CORBA::Dispatcher *, CORBA::Transport *, GIOPConnCallback *,
	      GIOPCodec *, CORBA::Long tmout = 0, CORBA::ULong max_size = 0,
              MICO::GIOPSide side = UNSPECIFIED,
	      CORBA::Boolean __reader_thread = FALSE,
	      CORBA::Boolean __writer_thread = FALSE,
	      CORBA::ULong id = 0xFFFFFFFFUL);
#else
    GIOPConn (CORBA::Dispatcher *, CORBA::Transport *, GIOPConnCallback *,
	      GIOPCodec *, CORBA::Long tmout = 0, CORBA::ULong max_size = 0,
	      CORBA::ULong id = 0xFFFFFFFFUL);
#endif
    virtual ~GIOPConn ();

#ifdef HAVE_THREADS
    void start();
    void output (CORBA::Buffer *);

    CORBA::Boolean check_events ()
    { return FALSE; };

    void ref ();
    CORBA::Boolean deref (CORBA::Boolean all = FALSE);

    void get_exclusive ()
    { _excl_mutex.lock(); };

    void release_exclusive ()
    { _excl_mutex.unlock(); };

    // added for debuging reason
    // because I'm not able to add breakpoint on line in .h file :-((
    CORBA::Boolean
    active_ref();

    void
    active_deref();

    // three methods using TLS for `detecting' reader thread
    static CORBA::Boolean
    is_this_reader_thread();

    static void
    set_as_reader_thread();

    static void
    initialize_reader_key();
#else
    void start()
    {};

    void output (CORBA::Buffer *b)
    { this->output_handler(b); };

    CORBA::Boolean check_events ();

    void ref ();
    CORBA::Boolean deref (CORBA::Boolean all = FALSE);

    void get_exclusive ()
    {};
    void release_exclusive ()
    {};
#endif

    CORBA::ULong id () { return _id; }
    void id (CORBA::ULong i) { _id = i; }

    CORBA::Boolean input_ready_callback(CORBA::Buffer *b);

    virtual void callback (CORBA::Transport *,
			   CORBA::TransportCallback::Event);
    virtual void callback (CORBA::Dispatcher *,
			   CORBA::DispatcherCallback::Event);

    CORBA::Buffer *input ();
    void flush ();

    CORBA::Transport *transport ()
    { return _transp; }

    GIOPCodec *codec ()
    { return _codec; }

    void cancel (CORBA::ULong reqid);
    void buffering (CORBA::Boolean dobuffering);

    CORBA::Dispatcher* dispatcher()
    { return _disp; }
};

struct GIOPConnCallback {
    enum Event { InputReady, Closed, Idle };
    virtual CORBA::Boolean callback (GIOPConn *, Event) = 0;
    virtual CORBA::Boolean input_callback (GIOPConn *, CORBA::Buffer *) = 0;
    virtual ~GIOPConnCallback ();
#ifdef HAVE_THREADS
    void send_orb_msg (GIOPConn *, ORBMsg::Event);
    //#else
    //    void kill_conn (GIOPConn *conn, CORBA::Boolean redo = FALSE) { kill_conn_callback( conn, redo ); };
#endif
};

inline CORBA::Boolean
GIOPConn::input_ready_callback(CORBA::Buffer *b) {
    return _cb->input_callback (this, b);
}

struct GIOPConnMsg {
    GIOPConnMsg(GIOPConn *_conn, CORBA::Buffer *_b, GIOPConnCallback::Event _ev):
	conn(_conn), b(_b), ev(_ev) {};

    GIOPConn                *conn;
    CORBA::Buffer           *b;
    GIOPConnCallback::Event ev;
};

#ifdef HAVE_THREADS

class GIOPConnWriter: public ActiveOperation {
protected:
    GIOPConn *conn;
public:
    GIOPConnWriter( GIOPConn *_conn );
    virtual ~GIOPConnWriter();
    
    void process( msg_type * msg );
    void send_msg( msg_type * msg);
    virtual Operation *copy() const { return NULL; };
};

class GIOPConnReader: public MICOMT::Thread {
protected:
    GIOPConn *conn;
public:
    GIOPConnReader( GIOPConn *_conn );
    void _run(void*);
};

class InputHandler: public PassiveOperation {
public:
    InputHandler();
    //    InputHandler( const MICO::InputHandler::InputHandler & );
    InputHandler( ThreadPool& _tp );

    void process( msg_type * msg );
    virtual Operation *copy() const;
};

class DummyDispatcher: public CORBA::Dispatcher {
public:
    virtual ~DummyDispatcher () {};

    void rd_event (CORBA::DispatcherCallback *, CORBA::Long fd) {};
    void wr_event (CORBA::DispatcherCallback *, CORBA::Long fd) {};
    void ex_event (CORBA::DispatcherCallback *, CORBA::Long fd) {};
    void tm_event (CORBA::DispatcherCallback *, CORBA::ULong tmout) {};
    void remove (CORBA::DispatcherCallback *, CORBA::Dispatcher::Event) {};
    void run (CORBA::Boolean infinite = TRUE) {};
    void move (CORBA::Dispatcher *) {};
    CORBA::Boolean idle () const { return TRUE; };
 
    virtual void block (CORBA::Boolean b) {};
    virtual CORBA::Boolean isblocking () { return TRUE; };
};                                                                              

class GIOPConnMgr {
protected:
    DummyDispatcher dummyDisp;

public:
    virtual ~GIOPConnMgr()
    {}

    virtual CORBA::Dispatcher *Dispatcher() { return &dummyDisp; };
};

#else

class GIOPConnMgr {
public:
    virtual ~GIOPConnMgr()
    {}

    //CORBA::Dispatcher *Dispatcher() { return _orb->dispatcher(); };
    virtual CORBA::Dispatcher* Dispatcher() = 0;
};

#endif // HAVE_THREADS

class IIOPProxyInvokeRec {
public:
    typedef CORBA::ULong MsgId;
    typedef CORBA::Codeset::CodesetId CodesetId;
private:
    CORBA::ORBMsgId _id;
    MsgId _msgid;
    GIOPConn *_conn;
    CORBA::ORBRequest *_req;

    CORBA::Boolean _active;

public:
    IIOPProxyInvokeRec ()
    {}

    ~IIOPProxyInvokeRec ()
    {
	if (this->id() != NULL) {
	    this->id()->set_invoke_hint(NULL);
	}
    }

    void free ()
    {}

    void init (CORBA::ORBMsgId idval, GIOPConn *connval, CORBA::ORBRequest *req = 0)
    {
	_id = idval;
	_msgid = CORBA::ORB::get_msgid(idval);
	_conn = connval;
	_req = req;
	_active = TRUE;
    }

    CORBA::Boolean active() const
    { return _active; };

    void deactivate()
    { _active = FALSE; };

    GIOPConn *conn () const
    { return _conn; }

    CORBA::ORBRequest *request () const
    { return _req; }

    CORBA::ORBMsgId id () const
    { return _id; }

    MsgId msgid () const
    { return _msgid; }
};

class IIOPProxy : public CORBA::ObjectAdapter, public GIOPConnCallback, public GIOPConnMgr {
    typedef std::map<MsgId, IIOPProxyInvokeRec *, std::less<MsgId> > MapIdConn;

    struct addrcomp :
	std::binary_function<const CORBA::Address *, const CORBA::Address *, bool> {
	bool operator() (const CORBA::Address *a1,
			 const CORBA::Address *a2) const
	{ return a1->compare (*a2) < 0; }
    };

    struct iorcomp :
	std::binary_function<const CORBA::IORProfile *, const CORBA::IORProfile *, bool> {
	  bool operator() (const CORBA::IORProfile *ior1,
			   const CORBA::IORProfile *ior2) const
	{
	    if (ior1 && ior2)
		return  (ior1->compare (*ior2) < 0);
	    else if (ior1 && !ior2)
		return true;
	    else
		return false;
        }
    };

    typedef std::map<const CORBA::Address *, GIOPConn *, addrcomp> MapAddrConn;
    typedef std::map<CORBA::UShort, MapAddrConn, std::less<CORBA::UShort> > MapVerAddrConn;
    typedef std::map<const CORBA::IORProfile *, GIOPConn *,
	              iorcomp > MapProfConn;
    typedef std::vector<CORBA::IORProfile::ProfileId> ProfIdVec;

    ProfIdVec _valid_profiles;

    MICOMT::Locked<MapIdConn> _ids;
    MICOMT::Locked<MapVerAddrConn> _conns;
    MICOMT::Locked<MapProfConn> _prof_conns;

    CORBA::ORB_ptr _orb;
    CORBA::UShort _giop_ver;
    CORBA::ULong _max_message_size;

    CORBA::Address *_reroute;

#ifdef USE_IOP_CACHE
    IIOPProxyInvokeRec *_cache_rec;
    CORBA::Boolean _cache_used;
#endif

    IIOPProxyInvokeRec *create_invoke ();
    IIOPProxyInvokeRec *pull_invoke (CORBA::ORBMsgId);
    void add_invoke (IIOPProxyInvokeRec *);
    void del_invoke (IIOPProxyInvokeRec *);
    void abort_invoke (CORBA::ORBMsgId);
    void redo_invoke  (CORBA::ORBMsgId);

    GIOPConn *make_conn (CORBA::Object_ptr);
    GIOPConn *make_conn (const CORBA::Address *, CORBA::Boolean create = TRUE,
			 CORBA::UShort version = 0);
    void conn_error (GIOPConn *, CORBA::Boolean send_error = TRUE);

    void deref_conn (GIOPConn *conn, CORBA::Boolean all = FALSE );

    CORBA::Boolean handle_input (GIOPConn *, CORBA::Buffer *);
    void exec_invoke_reply (GIOPInContext &, CORBA::ORBMsgId,
			    GIOP::ReplyStatusType,
			    CORBA::Object_ptr, CORBA::ORBRequest *,
			    GIOP::AddressingDisposition,
			    GIOPConn *conn);
    CORBA::Boolean handle_invoke_reply (GIOPConn *conn, GIOPInContext &);
    CORBA::Boolean handle_locate_reply (GIOPConn *conn, GIOPInContext &);
public:
    IIOPProxy (CORBA::ORB_ptr,
               CORBA::UShort giop_ver = 0x0100,
	       CORBA::ULong max_size = 0);
    ~IIOPProxy ();

    void register_profile_id (CORBA::ULong id);
    void unregister_profile_id (CORBA::ULong id);

    const char *get_oaid () const;
    CORBA::Boolean has_object (CORBA::Object_ptr);
    CORBA::Boolean is_local () const;
    
#ifdef USE_CSL2
    CORBA::Principal_ptr get_principal (CORBA::Object_ptr); 
#endif /* USE_CSL2  */

    CORBA::Boolean invoke (CORBA::ORBMsgId, CORBA::Object_ptr,
    			   CORBA::ORBRequest *,
    			   CORBA::Principal_ptr,
    			   CORBA::Boolean repsonse_exp);
    CORBA::Boolean bind (CORBA::ORBMsgId, const char *repoid,
    			 const CORBA::ORB::ObjectTag &,
    			 CORBA::Address *addr);
    CORBA::Boolean locate (CORBA::ORBMsgId, CORBA::Object_ptr);
    CORBA::Object_ptr skeleton (CORBA::Object_ptr);
    void cancel (CORBA::ORBMsgId);
    void shutdown (CORBA::Boolean wait_for_completion);
    
    void answer_invoke (CORBA::ORBMsgId, CORBA::Object_ptr,
    			CORBA::ORBRequest *,
    			CORBA::InvokeStatus);
    
    CORBA::Boolean callback (GIOPConn *, GIOPConnCallback::Event);
    CORBA::Boolean input_callback (GIOPConn *, CORBA::Buffer *);

    void kill_conn (GIOPConn *, CORBA::Boolean redo = FALSE);

    virtual CORBA::Dispatcher* Dispatcher();

    void redirect (CORBA::Address *addr) { _reroute = addr; }

};


class IIOPServerInvokeRec {
public:
    typedef CORBA::ULong MsgId;
private:
    CORBA::ORBRequest *_req;
    CORBA::Object_ptr _obj;
    CORBA::Principal_ptr _pr;
    CORBA::ORBMsgId _orbid;
    MsgId _orbmsgid;
    MsgId _reqid;
    GIOPConn *_conn;
    CORBA::Boolean _active;
public:
    IIOPServerInvokeRec ();
    ~IIOPServerInvokeRec ();
    void free ();

    void init_invoke (GIOPConn *conn, MsgId reqid, CORBA::ORBMsgId orbid,
		      CORBA::ORBRequest *req, CORBA::Object_ptr obj,
		      CORBA::Principal_ptr pr);
    void init_locate (GIOPConn *conn, MsgId reqid, CORBA::ORBMsgId orbid,
		      CORBA::Object_ptr obj);

    CORBA::Boolean active() const
    { return _active; };

    void deactivate()
    { _active = FALSE; };

    CORBA::ORBRequest *req()
    { return _req; }

    CORBA::ORBMsgId orbid()
    { return _orbid; }

    MsgId orbmsgid()
    { return _orbmsgid; }

    void orbid (CORBA::ORBMsgId id)
    { _orbid = id; }

    MsgId reqid()
    { return _reqid; }

    GIOPConn *conn()
    { return _conn; }
};

class IIOPServer : public CORBA::ObjectAdapter,
		   public CORBA::ORBCallback, public GIOPConnCallback, public GIOPConnMgr,
		   public CORBA::TransportServerCallback {
    typedef CORBA::ULong MsgId;
    typedef std::map<MsgId, IIOPServerInvokeRec *, std::less<MsgId> > MapIdConn;
    typedef std::list<GIOPConn *> ListConn;
    typedef FastArray<CORBA::TransportServer> VecTranspServ;

    VecTranspServ _tservers;

    MICOMT::Locked<ListConn> _conns;

    MapIdConn _orbids;
    MICOMT::Mutex _orbids_mutex;

    CORBA::ORB_ptr _orb;

#ifdef USE_IOP_CACHE
    IIOPServerInvokeRec *_cache_rec;
    CORBA::Boolean _cache_used;
#endif

    CORBA::UShort _iiop_ver;
    CORBA::ULong _max_message_size;

    IIOPServerInvokeRec *create_invoke();
    IIOPServerInvokeRec *pull_invoke_reqid (MsgId, GIOPConn *conn);
    IIOPServerInvokeRec *pull_invoke_orbid (CORBA::ORBMsgId);
    void add_invoke (IIOPServerInvokeRec *);
    void del_invoke_reqid (MsgId, GIOPConn *conn);
    void del_invoke_orbid (IIOPServerInvokeRec *);
    void abort_invoke_orbid (IIOPServerInvokeRec *);

    void conn_error (GIOPConn *, CORBA::Boolean send_error = TRUE);
    void conn_closed (GIOPConn *);

    void deref_conn (GIOPConn *conn, CORBA::Boolean all = FALSE );

    CORBA::Boolean handle_input (GIOPConn *, CORBA::Buffer *);
    CORBA::ORBMsgId exec_invoke_request (GIOPInContext &, CORBA::Object_ptr,
			       CORBA::ORBRequest *,
			       CORBA::Principal_ptr, CORBA::Boolean resp_exp,
			       GIOPConn *conn,
			       CORBA::ORBMsgId msgid);
    CORBA::Boolean handle_invoke_request (GIOPConn *conn, GIOPInContext &);
    CORBA::Boolean handle_locate_request (GIOPConn *conn, GIOPInContext &);
    CORBA::Boolean handle_cancel_request (GIOPConn *conn, GIOPInContext &);

    void handle_invoke_reply (CORBA::ORBMsgId);
    void handle_locate_reply (CORBA::ORBMsgId);
    void handle_bind_reply   (CORBA::ORBMsgId);
public:
    IIOPServer (CORBA::ORB_ptr, CORBA::UShort iiop_ver = 0x0100,
		CORBA::ULong max_size = 0);
    ~IIOPServer ();

    CORBA::Boolean listen (CORBA::Address *, CORBA::Address *);
    CORBA::Boolean listen ();

    // ObjectAdapter methods
    const char *get_oaid () const;
    CORBA::Boolean has_object (CORBA::Object_ptr);
    CORBA::Boolean is_local () const;
    
#ifdef USE_CSL2
    CORBA::Principal_ptr get_principal (CORBA::Object_ptr);
#endif /* USE_CSL2  */

    CORBA::Boolean invoke (CORBA::ORBMsgId, CORBA::Object_ptr,
    			   CORBA::ORBRequest *,
    			   CORBA::Principal_ptr,
    			   CORBA::Boolean repsonse_exp);
    CORBA::Boolean bind (CORBA::ORBMsgId, const char *repoid,
    			 const CORBA::ORB::ObjectTag &,
    			 CORBA::Address *addr);
    CORBA::Boolean locate (CORBA::ORBMsgId, CORBA::Object_ptr);
    CORBA::Object_ptr skeleton (CORBA::Object_ptr);
    void cancel (CORBA::ORBMsgId);
    void shutdown (CORBA::Boolean wait_for_completion);
    
    void answer_invoke (CORBA::ORBMsgId, CORBA::Object_ptr,
    			CORBA::ORBRequest *,
    			CORBA::InvokeStatus);

    // ORBCallback interface
    CORBA::Boolean waitfor (CORBA::ORB_ptr, CORBA::ORBMsgId,  CORBA::ORBCallback::Event, CORBA::Long);
    void notify (CORBA::ORB_ptr, CORBA::ORBMsgId, CORBA::ORBCallback::Event);

    CORBA::Boolean callback (GIOPConn *, GIOPConnCallback::Event);
    CORBA::Boolean input_callback (GIOPConn *, CORBA::Buffer *);
    void callback (CORBA::TransportServer *,
		   CORBA::TransportServerCallback::Event);

    void kill_conn (GIOPConn *, CORBA::Boolean redo = FALSE);

    virtual CORBA::Dispatcher* Dispatcher();
};

}

#endif // __mico_iop_h__
