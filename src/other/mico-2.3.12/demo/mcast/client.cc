#include <CORBA-SMALL.h>
#include "hello.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif


using namespace std;

int
main (int argc, char *argv[])
{
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

    assert (argc == 2);
    CORBA::Object_var obj = orb->bind ("IDL:HelloWorld:1.0", argv[1]);
    if (CORBA::is_nil (obj)) {
	cout << "cannot bind to " << argv[1] << endl;
	return 1;
    }
    HelloWorld_var hello = HelloWorld::_narrow (obj);

    CORBA::String_var s;
    for (int i = 0; i < 10*70; ++i) {
	s = hello->hello();
	cout << s.in();
	if (!((i+1) % 70))
	    cout << endl;
    }
    return 0;
}
