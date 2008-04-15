
#include "bench.h"
#include <CORBA.h>
#include <iostream>
#include <string>

bench_ptr __bench;
int __num;
int __delay;
int __thread_number;

#ifdef HAVE_THREADS

class Invoker
    : virtual public MICOMT::Thread
{
public:
    void _run(void*)
    {
	for (int __i=0; __i<__num; __i++) {
	    __bench->perform ();
	}
    }
};

#endif // HAVE_THREADS

int
main (int __argc, char* __argv[]) {
  
  CORBA::ORB_ptr __orb = CORBA::ORB_init (__argc, __argv, "mico-local-orb");
  
  cout << "__argv[0]: " << __argv[0] << endl;
  cout << "__argv[1]: " << __argv[1] << endl;
  cout << "__argv[2]: " << __argv[2] << endl;
  cout << "__argv[3]: " << __argv[3] << endl;
  cout << "__argv[4]: " << __argv[4] << endl;

  string __s = __argv[1];
  cout << "method: " << __s << endl;
  if (__s == "bind") {
    cerr << "binding..." << endl;
    CORBA::Object_ptr __obj = __orb->bind ("IDL:bench:1.0");
    __bench = bench::_narrow (__obj);
    cerr << "binded." << endl;
  }
  else if (__s == "ior") {
    string __ior;
    cin >> __ior;
    CORBA::Object_ptr __obj = __orb->string_to_object(__ior.c_str());
    __bench = bench::_narrow (__obj);
  }
  else {
    cerr << "unsupported binding method." << endl;
    exit(2);
  }
  if (CORBA::is_nil(__bench)) {
    cerr << "can't bind to the bench interface" << endl;
    exit(1);
  }
  __num = atoi (__argv[2]);
  cout << "num: " << __num << endl;
  __delay = atoi (__argv[3]);
  cout << "invoke delay: " << __delay << endl;
#ifdef HAVE_THREADS
  __thread_number = atoi (__argv[4]);
  cout << "number of threads: " << __thread_number << endl;
  if (__thread_number > 0) {
      Invoker* __thr_array[__thread_number];
      for (int __i=0; __i<__thread_number; __i++) {
	  __thr_array[__i] = new Invoker;
      }
      for (int __i=0; __i<__thread_number; __i++) {
	  __thr_array[__i]->start();
      }
      for (int __i=0; __i<__thread_number; __i++) {
	  __thr_array[__i]->wait();
      }
      //__bench->shutdown();
      __orb->destroy();
      return 0;
  }
#endif // HAVE_THREADS
  for (int __i=0; __i<__num; __i++) {
      try {
          __bench->perform ();
      } catch (...) {
          cout << ".";
      }
      //::sleep (__delay);
  }
  __bench->shutdown();
  __orb->destroy();
  return 0;
}
