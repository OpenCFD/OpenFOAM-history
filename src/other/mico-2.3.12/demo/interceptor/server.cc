#include <CORBA.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif
#include "hello.h"


using namespace std;

class MyServerInterceptor : public Interceptor::ServerInterceptor {
public:
    Interceptor::Status
    input_message (CORBA::Buffer *buf,
		   CORBA::Environment_ptr env)
    {
	/*
	 * what you can do here:
	 * - abort the request with an exception
	 * - change (decompress, decrypt, ...) the incoming message:
	 *   'buf' contains the GIOP Request message header and
	 *   params between buf->rpos() and buf->wpos(). You must
	 *   not change buf->rpos() and the data contained in 'buf'
	 *   before buf->rpos(). You can change buf->wpos() and the
	 *   data between buf->rpos() and buf->wpos().
	 */

	cout << "server: input message ("
	     << buf->wpos() - buf->rpos()
	     << " bytes)" << endl;

	// simple decryption ...
	for (CORBA::ULong i = buf->rpos(); i < buf->wpos(); ++i)
	    (buf->buffer())[i] ^= 0x37;

	return Interceptor::INVOKE_CONTINUE;
    }

    Interceptor::Status
    initialize_request (Interceptor::LWServerRequest_ptr req,
			CORBA::Environment_ptr env)
    {
	/*
	 * what you can do here:
	 * - answer the request yourself by filling in out parameters
	 *   or raising an exception (using env parameter) and returning
	 *   INVOKE_ABORT
	 *
	 * one useful application for initialize_request() is to implement
	 * access control. see demo/ssl for an example.
	 */
	CORBA::String_var s = req->operation();
	cout << "server: init request for: " << s.in() << endl;
	return Interceptor::INVOKE_CONTINUE;
    }

    Interceptor::Status
    after_unmarshal (Interceptor::LWServerRequest_ptr req,
		     CORBA::Environment_ptr env)
    {
	/*
	 * what you can do here:
	 * - get the service context list from the request
	 * - modify the in/inout parameters of the request
	 * - remove piggybacked extra values from the request
	 * - answer the request yourself by filling in out parameters
	 *   or raising an exception (using env parameter) and returning
	 *   INVOKE_ABORT
	 */
	CORBA::String_var s = req->operation();
	cout << "server: after unmarshal for: " << s.in() << endl;
	return Interceptor::INVOKE_CONTINUE;
    }

    Interceptor::Status
    before_marshal (Interceptor::LWServerRequest_ptr req,
		    CORBA::Environment_ptr env)
    {
	/*
	 * what you can do here:
	 * - set the service context list for the reply
	 * - modify the out/inout parameters of the reply
	 * - piggyback extra values to the reply
	 * - answer the request yourself by filling in out parameters
	 *   or raising an exception (using env parameter) and returning
	 *   INVOKE_ABORT
	 */
	CORBA::String_var s = req->operation();
	cout << "server: before marshal for: " << s.in() << endl;
	return Interceptor::INVOKE_CONTINUE;
    }

    Interceptor::Status
    finish_request (Interceptor::LWServerRequest_ptr req,
		    CORBA::Environment_ptr env)
    {
	/*
	 * you cannot do anything useful here, because the reply has
	 * already been marshalled and cannot be changed anymore.
	 */
	CORBA::String_var s = req->operation();
	cout << "server: finish request for: " << s.in() << endl;
	return Interceptor::INVOKE_CONTINUE;
    }

    Interceptor::Status
    output_message (CORBA::Buffer *buf,
		    CORBA::Environment_ptr env)
    {
	/*
	 * what you can do here:
	 * - abort the request with an exception
	 * - change (compress, encrypt, ...) the outgoing message:
	 *   'buf' contains the GIOP Reply message header and
	 *   results between buf->rpos() and buf->wpos(). You must
	 *   not change buf->rpos() and the data contained in 'buf'
	 *   before buf->rpos(). You can change buf->wpos() and the
	 *   data between buf->rpos() and buf->wpos().
	 */
	cout << "server: output message ("
	     << buf->wpos() - buf->rpos()
	     << " bytes)" << endl;

	// simple encryption ...
	for (CORBA::ULong i = buf->rpos(); i < buf->wpos(); ++i)
	    (buf->buffer())[i] ^= 0x37;

	return Interceptor::INVOKE_CONTINUE;
    }
};

class MyConnInterceptor : public Interceptor::ConnInterceptor {
public:
    Interceptor::Status client_connect (const char *addr)
    {
	/*
	 * called whenever a new connection is opened from
	 * a client to this server
	 */
	cout << "server: connect from: " << addr << endl;
	return Interceptor::INVOKE_CONTINUE;
    }
    Interceptor::Status client_disconnect (const char *addr)
    {
	/*
	 * called whenever a client closes a connection to
	 * this server.
	 *
	 * note that this does *not* mean that the client
	 * exited; it may temporarily close the connection
	 * and reopen it later. it is even possible that
	 * a client opens a new connection for each request.
	 */
	cout << "server: disconnect from: " << addr << endl;
	return Interceptor::INVOKE_CONTINUE;
    }
};

class Hello_impl : virtual public POA_HelloWorld {
public:
    void hello ()
    {
    }
};

int
main (int argc, char *argv[])
{
    MyServerInterceptor sv_icept;
    sv_icept.activate (0);

    MyConnInterceptor orb_icept;
    orb_icept.activate (0);

    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);
    assert(!CORBA::is_nil(poa));
    PortableServer::POAManager_var mgr = poa->the_POAManager();

    Hello_impl* hello_impl = new Hello_impl;
    hello_impl->_this();
    mgr->activate();
    orb->run ();

    return 0;
}
