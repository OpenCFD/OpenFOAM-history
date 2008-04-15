
#include "ex.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <unistd.h>
#include <signal.h>

class SomeIface_impl : virtual public POA_SomeIface {
public:
    void f ()
    {
	SomeExcept ex;
	ex.msg = (const char *)"foobar";
	mico_throw (ex);
    }
};

int main (int argc, char *argv[])
{
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);
    PortableServer::POAManager_var mgr = poa->the_POAManager();
    SomeIface_impl* servant = new SomeIface_impl;
    servant->_this();
    mgr->activate();
    orb->run ();
    return 0;
}
