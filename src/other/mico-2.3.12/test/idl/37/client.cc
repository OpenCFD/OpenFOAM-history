
#include "double.h"

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
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

  char pwd[256], uri[300];
  sprintf (uri, "file://%s/test.ref", getcwd(pwd, 256));

  CORBA::Object_var obj = orb->string_to_object (uri);
  Test_var tt = Test::_narrow (obj);

  if (CORBA::is_nil (tt)) {
    cout << "oops: could not locate test server" << endl;
    exit (1);
  }
  double d = atof(argv[1]);
  cout.precision(16);
  cout << "passing: " << d << " to server..." << endl;
  tt->op(d);
  return 0;
}
