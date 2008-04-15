#include "simple.h"
#include <CORBA.h>
#include <mico/os-misc.h>                                                      
#include <mico/util.h>                                                      
#include <pthread.h>
#include <sched.h>

class doit: public MICOMT::Thread {
public:
  void _run (void *arg);
};

simple_var s;
int loops;

#define THRS  2

doit *th[THRS];

char c[] = "+.*-#";

int main (int argc, char **argv);

void doit::_run(void *v) {

  char c = *(char *)v; 

  for (int i = 0; i < loops; i++)
    {
      if (( i % 1000 ) == 0) 
	MICODebug::instance()->printer() << i << endl;
      s->func( c );
      yield();
    }
}

int main (int argc, char **argv) {

  int i;

   CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");
   CORBA::Object_var o;

   if (argc < 2)
   {
      cerr << "Usage: client <iterations>" << endl;
      exit (0);
   }

   o = orb->bind ("IDL:simple:1.0");
   if (CORBA::is_nil (o))
     {
       cerr << "client: orb->bind (): to \"IDL:simple:1.0\" failed";
       cerr << "\nclient: is the server running?" << endl;
       exit (-1);
     }
   
   s = simple::_narrow (o);
   if (CORBA::is_nil (s))
     {
       cerr << "client: couldn't _narrow () to \"IDL:simple:1.0\"" << endl;
       exit (-2);
     }
   
   loops = atoi (argv[1]);
   CORBA::Long t1, t2;
   t1 = OSMisc::gettime();                                                    
   
   for ( i = 0; i < THRS; i++ ) {
     th[i] = new doit();
     th[i]->start( &c[i % 5] );
   }

   for ( i = 0; i < THRS; i++ ) {
     th[i]->wait();
     delete th[i];
   }
   s->flush();
   cout << "client: done" << endl;
   t2 = OSMisc::gettime();
   cout << (double)(t2 - t1) << " ms total time" << endl; 
   cout << (double)(t2 - t1)/ (loops*THRS) << " ms per call" << endl;     

   return 0;
}
