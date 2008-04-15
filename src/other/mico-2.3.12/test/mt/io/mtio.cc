/*  -*- mode: c++; c-basic-offset: 4; -*-
 *
 */


#include <CORBA.h>
#include <mico/impl.h>

class SockConn;
class SockWriter;
class SockReader;

struct SockMsg {
    SockConn      *conn;
    CORBA::Buffer *b;
};

//we should define an abstract Conn object, 
// which defines the basic interface
class SockConn {
protected:
    SockReader *_reader;
    SockWriter *_writer;

    CORBA::Transport *_trans;
public:
    SockConn(CORBA::Transport *);
    ~SockConn();
    void start();

    CORBA::Long read(CORBA::Buffer *);
    CORBA::Long write(CORBA::Buffer *);

    void do_output(CORBA::Buffer *);
    void do_input(CORBA::Buffer *);
};

class SockWriter: public MICO::activeOperation {
protected:
    SockConn *conn;
public:
    SockWriter( SockConn * );
    virtual void __clean_up();

    void process( MICO::msg_type *msg );
    void send_msg( MICO::msg_type *msg);
    virtual Operation *copy() const { return NULL; };
};

class SockReader: public MICO::Operation {
protected:
    SockConn *conn;
public:
    SockReader( SockConn * );
    void _run();
    virtual Operation *copy() const { return NULL; };
};

class SockInputHandler: public MICO::passiveOperation {
public:
    SockInputHandler();
    SockInputHandler( MICO::ThreadPool& _tp );

    void process( MICO::msg_type * msg );
    virtual Operation *copy() const;
};

#define SockReaderID        100
#define SockInputHandlerID  101
#define SockWriterID        102

/******************************* SockConn *******************************/

SockConn::SockConn(CORBA::Transport *trans)
{
    _trans = trans;
    _reader = new SockReader( this );
    _writer = new SockWriter( this );

    //    _trans->setown( _reader->Thread().id() );
}

SockConn::~SockConn()
{
    _writer->initShutdown();
    _reader->initShutdown();
    _trans->close();
    delete _trans;
    _writer->finalizeShutdown();
    _reader->finalizeShutdown();
    delete _writer;
    delete _reader;
}

/*
 * THIS is IMPORTANT
 *
 * never ever start threads from constructors
 *  - always use an explicit start methode
 */
void SockConn::start()
{
    _reader->start();
    _writer->start();
}

CORBA::Long 
SockConn::read(CORBA::Buffer *b)
{
    //do something more apropriate for your app. here
    return _trans->read(*b, 1);
}
 
CORBA::Long
SockConn::write(CORBA::Buffer *b)
{
    return _trans->write(*b, b->length());
}

void SockConn::do_output(CORBA::Buffer *b)
{
    // post the Buffer to the write thread
    MICO::msg_type *msg;

    msg = new MICO::msg_type();
    msg->ptr = b;
	
    _writer->send_msg( msg );
}

void SockConn::do_input(CORBA::Buffer *b)
{
    //short circuit for this test !
    do_output(b);
}

/******************************* SockReader *****************************/

SockReader::SockReader( SockConn *_conn ) {

    MICO::WorkerThread *kt;
    conn = _conn;
    kt = MICO::_tpm->get_idle_thread( SockReaderID );
    kt->registerOperation( this );
    Info().set_nextOP( MICO::_tpm->getThreadPool( SockInputHandlerID ) );
    kt->mark_busy();
}

void
SockReader::_run()
{
    MICO::msg_type *msg;
    SockMsg        *m;

    while (42) {

	CORBA::Buffer *b = new CORBA::Buffer();

	CORBA::Long r = conn->read(b);
	if (r < 0 || r == 0) {
	    // connection broken or EOF
	    break;
	}
	msg = new MICO::msg_type();
	m = new SockMsg;
	
	m->conn  = conn;
	m->b     = b;
	msg->ptr = m;
	
	send_msg( SockInputHandlerID, msg );
    }
}

/******************************* SockWriter *****************************/

SockWriter::SockWriter( SockConn *_conn ) {

    MICO::WorkerThread *kt;
    MICO::wt_msg_queue *mq;

    conn = _conn;
    mq = new MICO::wt_msg_queue();
    input_mc = mq;
    kt = MICO::_tpm->get_idle_thread( SockWriterID );
    kt->registerOperation( this );
    kt->mark_busy();
}

void SockWriter::__clean_up() 
{
    delete input_mc;
}

void
SockWriter::process( MICO::msg_type * msg ) {

    CORBA::Buffer *b = (CORBA::Buffer *)msg->ptr;

    //do some error handling here !!!
    __MTOUT( MT_TRACE( MICODebug::instance()->printer() 
		       << "SockWriter::process: (" << this << ") msg: " << endl ));
    conn->write(b);
    delete b;
    delete msg;
}

void
SockWriter::send_msg( MICO::msg_type * msg) {

    getInputMC()->put_msg( SockWriterID, msg );
}

/******************************* SockInputHandler ***************************/


SockInputHandler::SockInputHandler(): passiveOperation() {

    Info().set_OP_id( SockInputHandlerID );
};


SockInputHandler::SockInputHandler( MICO::ThreadPool& _tp ): passiveOperation( _tp) {

    Info().set_OP_id( SockInputHandlerID );
};

void
SockInputHandler::process( MICO::msg_type * msg ) {

    SockMsg *m = (SockMsg *)msg->ptr;

    __MTOUT( MT_TRACE( MICODebug::instance()->printer() 
		       << "SockInputHandler::process: (" << this << ") msg: " << endl ));
    m->conn->do_input( m->b );

    delete m;
    delete msg;
}

MICO::Operation *
SockInputHandler::copy() const {

    return new SockInputHandler( *this );
}

/**********************************************************/

int main( int argc, char *argv[] )
{
    MICO::ThreadPool       *tp;
    CORBA::TransportServer *tserv;
    CORBA::Transport       *trans;
    SockConn               *conn;

    CORBA::Address *addr = CORBA::Address::parse ("inet:localhost:5678");
    if (!addr) {
	__MTSAVE( MICODebug::instance()->printer()
		  << "bad address: " << endl );
	exit (1);
    }

    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

    
    tp = new MICO::ThreadPool( 10, 0, 0 );
    MICO::_tpm->register_tp( SockReaderID, *tp );
    tp->start_threads();

    tp = new MICO::ThreadPool( 10, 0, 0 );
    MICO::_tpm->register_tp( SockWriterID, *tp );
    tp->start_threads();

    tp = new MICO::ThreadPool( 10, 0, 0 );
    MICO::_tpm->register_tp( SockInputHandlerID , *tp );
    tp->setOperation( new SockInputHandler() );
    tp->registerInputMC( *new MICO::msg_queue() );
    tp->start_threads();

    tserv = addr->make_transport_server();
    if (!tserv->bind (addr)) {
        __MTSAVE( MICODebug::instance()->printer()
		  << "cannot bind to " << addr->stringify() << ": "
		  << tserv->errormsg() << endl );
        return 1;
    }
    tserv->block ( TRUE );
    trans = tserv->accept();
    conn = new SockConn( trans );
    conn->start();
    
    //    while (42) {
	sleep( 30 );
	//    }

    delete conn;
    delete tserv;
}
