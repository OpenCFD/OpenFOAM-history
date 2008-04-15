
#ifdef __COMO__
#pragma no_pch
#endif // __COMO__
#include <CORBA.h>
#include <string.h>
#include <mico/template_impl.h>

#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif


using namespace std;

void
usage (const char * argv0)
{
  printf ("usage:\n");
  printf ("  %s               -- reads IOR from stdin\n", argv0);
  printf ("  %s IOR           -- reads IOR from command line\n", argv0);
  printf ("  %s repoid [tag]  -- binds to repoid using tag\n", argv0);
}

int
main (int argc, char *argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

  if (argc > 1 && *argv[1] == '-') {
    usage (argv[0]);
    return 1;
  }

  CORBA::Object_var obj;

  if (argc == 1) {
    char ref[2345];
    cin >> ref;
    if (!ref[0]) {
      return 0;
    }

#ifdef HAVE_EXCEPTIONS
    try {
#endif
      obj = orb->string_to_object (ref);
#ifdef HAVE_EXCEPTIONS
    }
    catch (CORBA::SystemException_catch & sex) {
      obj = CORBA::Object::_nil ();
    }
#endif

    if (CORBA::is_nil (obj)) {
      printf ("error: illegal object reference: `%s'\n", ref);
      exit (1);
    }
  }
  else if (argc > 1 && strncmp (argv[1], "IDL", 3) == 0) {
    CORBA::ORB::ObjectTag_var tag = new CORBA::ORB::ObjectTag();

    if (argc == 3) {
      tag = CORBA::ORB::string_to_tag (argv[2]);
    }

    obj = orb->bind (argv[1], tag.in());

    if (CORBA::is_nil (obj)) {
      if (argc == 3) {
	printf ("error: cannot bind to `%s' using tag `%s'\n",
		argv[1], argv[2]);
      }
      else {
	printf ("error: cannot bind to `%s'\n", argv[1]);
      }
      exit (1);
    }
  }
  else if (argc == 2) {
#ifdef HAVE_EXCEPTIONS
    try {
#endif
      obj = orb->string_to_object (argv[1]);
#ifdef HAVE_EXCEPTIONS
    }
    catch (CORBA::SystemException_catch & sex) {
      obj = CORBA::Object::_nil ();
    }
#endif

    if (CORBA::is_nil (obj)) {
      printf ("error: illegal object reference: `%s'\n", argv[1]);
      exit (1);
    }

    if (!obj->_ior()) {
      printf ("error: locality-constrained object: `%s'\n", argv[1]);
      exit (1);
    }
  }
  else {
    usage (argv[0]);
    exit (1);
  }

  cout << endl;
  obj->_ior()->print (cout);
  cout << endl;
  return 0;
  
}
