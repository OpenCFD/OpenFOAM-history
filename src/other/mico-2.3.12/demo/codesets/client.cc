// sander include <sys/time.h>
#include <CORBA-SMALL.h>
#include "hello.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef _WIN32
#include <direct.h>
#endif


using namespace std;

int
main (int argc, char *argv[])
{
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

    /*
     * IOR is in server.ref in the local directory
     */

    char pwd[256], uri[300];
    sprintf (uri, "file://%s/server.ref", getcwd(pwd, 256));

    CORBA::Object_var obj = orb->string_to_object (uri);
    if (CORBA::is_nil (obj)) {
	cout << "cannot bind to " << uri << endl;
	return 1;
    }
    Hello_var hello = Hello::_narrow (obj);

    CORBA::String_var s = hello->sayHello ("&ouml;&auml;&uuml;");
    cout << s.in() << endl;

    return 0;
}
