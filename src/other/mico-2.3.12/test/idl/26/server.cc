
#include <CORBA.h>
#include <unistd.h>
#include <signal.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS

#include "base_impl.h"

int main (int argc, char *argv[])
{
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);
    assert(!CORBA::is_nil(poa));
    PortableServer::POAManager_var mgr = poa->the_POAManager();    

    base_impl* serv = new base_impl;
    serv->_this();
    mgr->activate();
    orb->run ();
    return 0;
}

