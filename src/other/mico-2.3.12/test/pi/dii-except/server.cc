#include "z.h"
#include "request.h"

#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else
#include <fstream.h>
#endif

using namespace std;

/*
 * Implementation of the Z
 */

class Z_impl : virtual public POA_Z
{
    void dii();
    char* stat();
};

void
Z_impl::dii()
{
    Z::E ex;
    ex._raise();
}

char*
Z_impl::stat()
{
    // Request
    CORBA::Request_var req = create_request( this->_this(), "dii" );
    req->invoke();
    
    return CORBA::string_dup( req->env()->exception()->_repoid() );
}

int
main (int argc, char *argv[])
{
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
    CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
    PortableServer::POAManager_var mgr = poa->the_POAManager();

    Z_impl* zi = new Z_impl;
    PortableServer::ObjectId_var oid = poa->activate_object( zi );

    // write reference
    ofstream of ("Z.ref");
    CORBA::Object_var ref = poa->id_to_reference (oid.in());
    CORBA::String_var str = orb->object_to_string (ref.in());
    of << str.in() << endl;
    of.close ();

    cout << "Running." << endl;
    mgr->activate ();
    orb->run();

    poa->destroy (TRUE, TRUE);
    delete zi;

    return 0;
}


