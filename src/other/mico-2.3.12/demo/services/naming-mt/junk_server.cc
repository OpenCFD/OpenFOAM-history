



#include "Junk_impl.h"

#include <iostream>
#include <fstream>
using std::cout;
using std::cerr;
using std::endl;

#include <signal.h>
                                                                                
void sighandler (int sig)
{
  CORBA::ORB_var orb = ORB_holder::instance()->orb();
  if (!CORBA::is_nil (orb)) {
    orb->shutdown (TRUE);
  }
}
                                                                                
int
main ( int argc, char* argv[] )
{
    CORBA::ORB_var orb = CORBA::ORB::_nil ();
    signal (SIGINT, sighandler);
    signal (SIGTERM, sighandler);
                                                                                
    cout << "junk_server- startup" << endl;
    orb = CORBA::ORB_init(argc, argv, "mico-local-orb");
    ORB_holder::instance()->orb(orb);
    cout << "junk_server- got the orb" << endl;
    CORBA::Object_var poaobj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var root_poa = PortableServer::POA::_narrow(poaobj);
    assert( !CORBA::is_nil(root_poa) );
    PortableServer::POAManager_var root_mgr = root_poa->the_POAManager();
    cout << "junk_server- got the root_poa" << endl;

    JunkFactory_impl* junk_factory = new JunkFactory_impl( root_poa );

    root_mgr->activate();
    junk_factory->poa_mgr()->activate();
    cout << "junk_server- activated junk_factory poa_mgr" << endl;

    orb->run();
    root_poa->destroy(TRUE, TRUE);
    return 0;
}
