/*
 *  MICO --- a free CORBA implementation
 *  Copyright (C) 1997-98 Kay Roemer & Arno Puder
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
 *
 *  Send comments and/or bug reports to:
 *                 mico@informatik.uni-frankfurt.de
 */

#include "hello.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif


using namespace std;

CORBA::ORB_var the_orb;

class SecureHello_impl : public virtual POA_SecureHello {
public:
    void hello ()
    {
        CORBA::Object_var o =
            the_orb->resolve_initial_references ("PrincipalCurrent");
        CORBA::PrincipalCurrent_var pc =
            CORBA::PrincipalCurrent::_narrow (o);
	CORBA::Principal_var princ = pc->get_principal();

	const char *s;
	CORBA::Any_var a;

	a = princ->get_property ("auth-method");
	a >>= s;
	cout << "authentication method: " << s << endl;

	a = princ->get_property ("peer-address");
	a >>= s;
	cout << "peer address: " << s << endl;

	a = princ->get_property ("ssl-cipher");
	a >>= s;
	cout << "cipher: " << s << endl;

	a = princ->get_property ("ssl-x509-issuer");
	a >>= s;
	cout << "issuer: " << s << endl;

	a = princ->get_property ("ssl-x509-subject");
	a >>= s;
	cout << "subject: " << s << endl;

	// ... or, to get individual fields of X509 names:

	a = princ->get_property ("ssl-x509-subject:CN");
	a >>= s;
	cout << "subject's name: " << s << endl;

	a = princ->get_property ("ssl-x509-subject:emailAddress");
	a >>= s;
	cout << "subject's email: " << s << endl;
    }
};

/*
 * a simple access checker using interceptors.
 */
class AccessChecker : public Interceptor::ServerInterceptor {
public:
    Interceptor::Status
    initialize_request (Interceptor::LWServerRequest_ptr req,
			CORBA::Environment_ptr env)
    {
	CORBA::Object_var obj = req->target ();
        CORBA::Object_var o =
            the_orb->resolve_initial_references ("PrincipalCurrent");
        CORBA::PrincipalCurrent_var pc =
            CORBA::PrincipalCurrent::_narrow (o);
	CORBA::Principal_var p = pc->get_principal();

	CORBA::Any_var a = p->get_property ("ssl-x509-subject:CN");
	const char *name;

	/*
	 * try changing "Roemer" to some other name to verify that
	 * the access check works ...
	 */
	if (!(a >>= name) || strcmp (name, "Roemer")) {
	    // permission denied ...
	    env->exception (new CORBA::NO_PERMISSION);
	    return Interceptor::INVOKE_ABORT;
	}
	// ok ...
	return Interceptor::INVOKE_CONTINUE;
    }
};

int
main (int argc, char *argv[])
{
    AccessChecker ac;
    ac.activate (0);

    the_orb = CORBA::ORB_init (argc, argv);
    CORBA::Object_var poaobj = the_orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
    PortableServer::POAManager_var mgr = poa->the_POAManager();

    /*
     * Create a SecureHello object
     */

    SecureHello_impl * hello = new SecureHello_impl;

    /*
     * Activate the Servant
     */

    PortableServer::ObjectId_var oid = poa->activate_object (hello);

    /*
     * Write reference to file
     */

    ofstream of ("sec_hello.ior");
    CORBA::Object_var ref = poa->id_to_reference (oid.in());
    CORBA::String_var str = the_orb->object_to_string (ref.in());
    of << str.in() << endl;
    of.close ();

    mgr->activate ();
    the_orb->run();

    /*
     * Shutdown (never reached)
     */

    poa->destroy (TRUE, TRUE);
    delete hello;

    return 0;
}
