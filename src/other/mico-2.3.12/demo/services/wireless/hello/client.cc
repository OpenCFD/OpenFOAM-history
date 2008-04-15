#include "hello.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

using namespace std;

int
main (int argc, char *argv[])
{
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    /*
     * IOR is in hello.ref in the local directory
     */

    char pwd[256], uri[300];
    sprintf (uri, "file://%s/hello.ref", getcwd(pwd, 256));

    /*
     * Bind to Hello World server
     */

    CORBA::Object_var obj = orb->string_to_object (uri);
    HelloWorld_var hello = HelloWorld::_narrow (obj);

    if (CORBA::is_nil (hello)) {
	printf ("oops: could not locate HelloWorld server\n");
	exit (1);
    }

    cout << endl << hello->hello ("Jaakko") << endl << endl;

    try {
	hello->error("No particular reason");
    } catch (const HelloWorld::Goodbye &e) {
	cerr << "Invocation failed: " << e.reason << endl;
    }

    return 0;
}
