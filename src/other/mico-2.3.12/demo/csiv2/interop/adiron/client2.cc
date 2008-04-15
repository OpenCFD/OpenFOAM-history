#include "InterOpTest.h"

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

  /*
   * IOR is in hello.ref in the local directory
   */

  char pwd[256], uri[300];
  char peter_uri[300];
  sprintf (uri, "file://%s/ref", getcwd(pwd, 256));
  sprintf (peter_uri, "file://%s/peter.ref", getcwd(pwd, 256));
  
  cout << "geting Peter reference..." << flush;
  CORBA::Object_var pobj = orb->string_to_object(peter_uri);
  InterOpTest::Peter_var peter = InterOpTest::Peter::_narrow(pobj);
  if (CORBA::is_nil(peter)) {
      cout << "failed!" << endl;
      cout << "oops: could not locate Peter server" << endl;
      exit(2);
  }
  cout << "done." << endl;

  cout << "geting Johnson reference..." << flush;
  CORBA::Object_var obj = orb->string_to_object(uri);
  InterOpTest::Johnson_var johnson = InterOpTest::Johnson::_narrow(obj);

  if (CORBA::is_nil(johnson)) {
      cout << "failed!" << endl;
      cout << "oops: could not locate Johnson server" << endl;
      exit(1);
  }
  cout << "done." << endl;
  CORBA::String_var result;
  result = johnson->say_hello_from_user_password(peter, "@objectsecurity.com", "karel", "cobalt", 1);
  cout << "result: `" << result.in() << "'" << endl;
  return 0;
}
