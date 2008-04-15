#include "demo.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

using namespace std;

int
main (int argc, char *argv[])
{
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    char pwd[256], uri[300];
    sprintf (uri, "file://%s/demo.ref", getcwd(pwd, 256));

    CORBA::Object_var obj = orb->string_to_object (uri);
    Demo_var demo = Demo::_narrow (obj);

    if (CORBA::is_nil (demo)) {
	printf ("oops: could not locate Demo server\n");
	exit (1);
    }

    CORBA::Long timeout = argc > 1 ? atoi(argv[2]) : 0;

    cout << endl << demo->date(timeout) << endl << endl;

    return 0;
}
