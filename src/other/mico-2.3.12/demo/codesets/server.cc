#define MICO_CONF_POA
#include <CORBA-SMALL.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif
#include "hello.h"


using namespace std;

class Hello_impl : virtual public POA_Hello {
public:
    char *sayHello (const char *s)
    {
	cout << s << endl;
	return CORBA::string_dup (s);
    }
};

int
main (int argc, char *argv[])
{
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);
    assert(!CORBA::is_nil(poa));
    PortableServer::POAManager_var mgr = poa->the_POAManager();
    Hello_impl * hi = new Hello_impl;
    Hello_var hi_ref = hi->_this();

    ofstream of ("server.ref");
    CORBA::String_var str = orb->object_to_string (hi_ref);
    of << str.in() << endl;
    of.close ();

    mgr->activate();
    orb->run ();
    return 0;
}
