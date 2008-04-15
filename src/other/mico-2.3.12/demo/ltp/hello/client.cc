#include "hello.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef _WINDOWS
#include <direct.h>
#endif

using namespace std;

int
main (int argc, char *argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

  cerr << "ORB init" << endl;
  /*
   * IOR is in hello.ref in the local directory
   */

  char pwd[256], uri[300];
  sprintf (uri, "file://%s/hello.ref", getcwd(pwd, 256));

  /*
   * Bind to Hello World server
   */

  CORBA::Object_var obj = orb->string_to_object (uri);
  cerr << "orb->string_to_object" << endl;
  HelloWorld_var hello = HelloWorld::_narrow (obj);
  cerr << "narrow" << endl;

  if (CORBA::is_nil (hello)) {
    printf ("oops: could not locate HelloWorld server\n");
    exit (1);
  }

  cerr << "starting invocation" << endl;
  hello->hello ();
  cerr << "done" << endl;
  return 0;
}
