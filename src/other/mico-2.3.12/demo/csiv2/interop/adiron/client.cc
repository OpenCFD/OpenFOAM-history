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
  sprintf (uri, "file://%s/ref", getcwd(pwd, 256));

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
  result = johnson->say_hello("Hello from MICO CSIv2 interoperability test!");
  cout << "result: `" << result.in() << "'" << endl;
  return 0;
}
