
#include "ex.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS


using namespace std;

int main (int argc, char *argv[])
{
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");
    assert (argc == 2);
    CORBA::Object_var obj = orb->bind ("IDL:SomeIface:1.0", argv[1]);
    SomeIface_var client = SomeIface::_narrow (obj);

    try {
	client->f();
    } catch (SomeExcept_catch &ex) {
	cout << "SomeExcept: " << ex->msg.in() << endl;
    }
    return 0;
}
