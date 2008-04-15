
#include <CORBA.h>
#include "derived_impl.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS


using namespace std;

CORBA::ORB_var orb = CORBA::ORB::_nil();

#ifdef HAVE_THREADS

class ORBRunner
    : public virtual MICOMT::Thread
{
public:
    void _run(void*)
    {
	try {
	    orb->run();
	} catch (CORBA::SystemException& ex) {
	    cerr << "caught: " << ex._repoid() << endl;
	}
    }
};

#endif // HAVE_THREADS

int main (int argc, char *argv[])
{
    orb = CORBA::ORB_init (argc, argv, "mico-local-orb");
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);
    assert(!CORBA::is_nil(poa));
    PortableServer::POAManager_var mgr = poa->the_POAManager();

    assert (argc == 2);
    obj = orb->bind ("IDL:base:1.0", argv[1]);
    assert( !CORBA::is_nil( obj ) );
    base_var client = base::_narrow (obj);
    assert( !CORBA::is_nil( client ) );
    derived_impl* serv = new derived_impl;
    derived_var test = serv->_this();

    mgr->activate();

#ifdef HAVE_THREADS
    ORBRunner* runner = new ORBRunner;
    runner->start();
#endif // HAVE_THREADS

    client->m( test );
    base_var ret_base = client->m();
    assert( !CORBA::is_nil( ret_base ) );
    derived_var ret_derived = derived::_narrow( ret_base );
    assert( !CORBA::is_nil( ret_derived ) );
}
