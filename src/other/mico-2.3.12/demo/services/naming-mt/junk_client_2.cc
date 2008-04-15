/* Program for verifying that NameService is working
*/
                                                                                
#include "Junk.h"
#include "JunkNC.h"

#include <coss/CosNaming.h>
                                                                                
#include <string>
#include <iostream>
#include <fstream>
using std::cerr;
using std::cout;
using std::endl;
                                                                                
int main(int argc, char* argv[] )
{
    int client_num = ::atoi(argv[1]);
    cout << "junk_client_2: Start - " << client_num << endl;
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var root_poa = PortableServer::POA::_narrow(obj);
    PortableServer::POAManager_var mgr = root_poa->the_POAManager();
    cout << "junk_client_2: Got the RootPOA" << endl;
                                                                                
    CORBA::Object_var nsobj =
    orb->resolve_initial_references ("NameService");
    CosNaming::NamingContext_var root_nc =
                                   CosNaming::NamingContext::_narrow (nsobj);
    if (CORBA::is_nil (root_nc)) {
        cerr << "oops, I cannot access the Naming Service!" << endl;
        exit (1);
    }
    cout << "junk_client_2: Got the NameService" << endl;

    //int ndepth = 4;
    //int nwidth = 3;
    //int njunk  = 2;
                                                                                
    std::string ior_s;
    std::ifstream in ("junk_factory.ior");
    in >> ior_s;
    in.close();
    obj = orb->string_to_object( ior_s.c_str() );
    JunkFactory_ptr junk_factory = JunkFactory::_narrow(obj);
    if ( CORBA::is_nil(junk_factory) ) {
        cerr << "junk_client_2: failed to narrow junk_factory" << endl;
        return 1;
    }

    JunkNC junk_nc( junk_factory, root_nc, client_num);
    CosNaming::Name name;
    int level = 0;
    int l;
    for ( l = 0; l < 50; l++) {
        if ( l%20 == 0) cout << "junk_client_2-" << client_num 
                                                       << "-" << l << endl;
        name.length(0);
        bool empty;
        if ( !junk_nc.traverse(name, root_nc.in(), level + 1, l, empty) ) {
            cerr << "junk_client_2-" << client_num 
                                   << "-traverse failed, l = " << l << endl;
            break;
        }
        if (empty ) {
            if ( !junk_nc.add_nc(root_nc, level, l) ) break;
        }
    }
    junk_nc.report(client_num );
    orb->destroy();
    return 0;
}
