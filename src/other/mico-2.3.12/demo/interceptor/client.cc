#include <CORBA.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif
#ifdef _WIN32
#include <time.h>
#else
#include <sys/time.h>
#endif
#include "hello.h"


using namespace std;

class MyClientInterceptor : public Interceptor::ClientInterceptor {
public:
    Interceptor::Status
    initialize_request (Interceptor::LWRequest_ptr req,
			CORBA::Environment_ptr env)
    {
	/*
	 * what you can do here:
	 * - set the service context list for the request
	 * - modify the in/inout parameters of the request
	 * - piggyback extra values to the request
	 * - change the target object reference
	 * - change the operation name
	 * - answer the request yourself by filling in out parameters
	 *   or raising an exception (using env parameter) and returning
	 *   INVOKE_ABORT
	 */
	CORBA::String_var s = req->operation();
	cout << "client: init request for: " << s.in() << endl;
	return Interceptor::INVOKE_CONTINUE;
    }

    Interceptor::Status
    after_marshal (Interceptor::LWRequest_ptr req,
		   CORBA::Environment_ptr env)
    {
	/*
	 * what you can do here:
	 * - answer the request yourself by filling in out parameters
	 *   or raising an exception (using env parameter) and returning
	 *   INVOKE_ABORT
	 */
	CORBA::String_var s = req->operation();
	cout << "client: after marshal for: " << s.in() << endl;
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
	 *   'buf' contains the GIOP Request message header and
	 *   params between buf->rpos() and buf->wpos(). You must
	 *   not change buf->rpos() and the data contained in 'buf'
	 *   before buf->rpos(). You can change buf->wpos() and the
	 *   data between buf->rpos() and buf->wpos().
	 */
	cout << "client: output message ("
	     << buf->wpos() - buf->rpos()
	     << " bytes)" << endl;

	// simple encryption ...
	for (CORBA::ULong i = buf->rpos(); i < buf->wpos(); ++i)
	    (buf->buffer())[i] ^= 0x37;

	return Interceptor::INVOKE_CONTINUE;
    }

    Interceptor::Status
    input_message (CORBA::Buffer *buf,
		   CORBA::Environment_ptr env)
    {
	/*
	 * what you can do here:
	 * - abort the request with an exception
	 * - change (decompress, decrypt, ...) the incoming message:
	 *   'buf' contains the GIOP Reply message header and
	 *   results between buf->rpos() and buf->wpos(). You must
	 *   not change buf->rpos() and the data contained in 'buf'
	 *   before buf->rpos(). You can change buf->wpos() and the
	 *   data between buf->rpos() and buf->wpos().
	 */

	cout << "client: input message ("
	     << buf->wpos() - buf->rpos()
	     << " bytes)" << endl;

	// simple decryption ...
	for (CORBA::ULong i = buf->rpos(); i < buf->wpos(); ++i)
	    (buf->buffer())[i] ^= 0x37;

	return Interceptor::INVOKE_CONTINUE;
    }

    Interceptor::Status
    before_unmarshal (Interceptor::LWRequest_ptr req,
		      CORBA::Environment_ptr env)
    {
	/*
	 * what you can do here:
	 * - answer the request yourself by filling in out parameters
	 *   or raising an exception (using env parameter) and returning
	 *   INVOKE_ABORT
	 */
	CORBA::String_var s = req->operation();
	cout << "client: before unmarshal for: " << s.in() << endl;
	return Interceptor::INVOKE_CONTINUE;
    }

    Interceptor::Status
    finish_request (Interceptor::LWRequest_ptr req,
		    CORBA::Environment_ptr env)
    {
	/*
	 * what you can do here:
	 * - get the service context list from the reply
	 * - modify the out/inout parameters of the request
	 * - remove piggybacked extra values from the reply
	 * - answer the request yourself by filling in out parameters
	 *   or raising an exception (using env parameter) and returning
	 *   INVOKE_ABORT
	 */
	CORBA::String_var s = req->operation();
	cout << "client: finish request for: " << s.in() << endl;
	return Interceptor::INVOKE_CONTINUE;
    }
};

int
main (int argc, char *argv[])
{
    MyClientInterceptor icept;
    icept.activate (0);

    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");
    assert (argc == 2);
    CORBA::Object_var obj = orb->bind ("IDL:HelloWorld:1.0", argv[1]);
    if (CORBA::is_nil (obj)) {
	cout << "cannot bind to " << argv[1] << endl;
	return 1;
    }
    HelloWorld_var hello = HelloWorld::_narrow (obj);
    hello->hello ();
    return 0;
}
