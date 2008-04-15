#include "hello.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS

#ifdef _WIN32
#include <time.h>
#else
#include <sys/time.h>
#endif


using namespace std;

int
main (int argc, char *argv[])
{

  char addr[256];
  strcpy(addr, argv[2]);

  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");
  /*
  CORBA::Object_var bad_obj = orb->bind ("IDL:Hello:1.1", argv[1]);
  if (CORBA::is_nil (bad_obj)) {
    cout << "cannot bind to " << addr << endl;
 //   return 1;
  }
  */
  CORBA::Object_var obj = orb->bind ("IDL:Hello:1.0");
  if (CORBA::is_nil (obj)) {
     cout << "cannot bind to " << addr << endl;
     return 1;
  }
  Hello_var hello = Hello::_narrow(obj);
  hello->hello_world();
  hello->olleh();
  //  obj = orb->bind ("IDL:Hello:1.1", addr);
  //  if (CORBA::is_nil (obj))
  //    cout << "cannot bind to " << addr << endl;


  return 0;
}
