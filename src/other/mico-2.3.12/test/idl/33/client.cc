
#include <val_impl.h>

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

  ValFactory_impl* fact = new ValFactory_impl;
  orb->register_value_factory("IDL:Val:1.0", fact);

  char pwd[256], uri[300];
  sprintf (uri, "file://%s/test.ref", getcwd(pwd, 256));

  CORBA::Object_var obj = orb->string_to_object (uri);
  Test_var tt = Test::_narrow (obj);

  if (CORBA::is_nil (tt)) {
    cout << "oops: could not locate test server" << endl;
    exit (1);
  }
  Val* p1 = new Val_impl;
  Val* p2 = new Val_impl;
  Val* p3 = NULL;
//    const int max = 100000;
//    for (int i = 0; i < max; i++) {
  p1->num(1);
  p2->num(2);
  Val* res = tt->op(p1, p2, p3);
  assert(p2->num() == 3);
  assert(p3->num() == 5);
  assert(res->num() == 10);
  CORBA::remove_ref(p3);
  CORBA::remove_ref(res);
//    }
  return 0;
}
