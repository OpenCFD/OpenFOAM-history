#include "wstr.h"
#include <mico/util.h>

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
  TestWString_var tt = TestWString::_narrow (obj);

  if (CORBA::is_nil (tt)) {
    cout << "oops: could not locate test server" << endl;
    exit (1);
  }

  CORBA::WString_var ws1 = CORBA::wstring_dup(L"Hello");
  CORBA::WString_var ws2 = tt->echo(ws1);
  if (xwcscmp(ws1.in(), ws2.in()) == 0)
      cout << "Strings are the same" << endl;
  else
      cout << "ERROR: strings are different" << endl;
  return 0;
}
