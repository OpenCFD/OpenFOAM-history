#include "simple.h"
#include <CORBA.h>
#include <mico/os-misc.h>                                                      
#include <mico/util.h>                                                      
#include <pthread.h>
#include <sched.h>

class doit: public MICOMT::Thread {
public:
  MICOMT::Mutex mt;

  void _run (void *arg);
};

simple_var s;
int loops;

#define THRS  100

doit *th[THRS];

char c[] = "+.*-#";

int main (int argc, char **argv);

MICOMT::CondVar *cond;

void doit::_run(void *v) {

  char c = *(char *)v; 

  mt.lock();
  cond->wait( mt );
  mt.unlock();

  for (int i = 0; i < loops; i++)
    {
      if (( i % 1000 ) == 0) 
	//MICO::Logger::Stream (MICO_DEBUG_MODULE) << i << endl;
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

/*
   for ( int n = 0; n < 10; n++ ) {
     o = orb->bind ("IDL:simple:1.0");
     if (CORBA::is_nil (o))
       {
         cerr << "client: orb->bind (): to \"IDL:simple:1.0\" failed";
         cerr << "\nclient: is the server running?" << endl;
         exit (-1);
       }
   }

*/

   o = orb->string_to_object(
	"IOR:002020200000000f49444c3a73696d706c653a312e30002000000002564953010000004c000101200000000c31302e31302e31312e343700000071c2000000000000002700504d43000000000000000f49444c3a73696d706c653a312e3000200000000773696d706c650020000000000000000000000043000100200000000c31302e31302e31312e343700d71720200000002700504d43000000000000000f49444c3a73696d706c653a312e3000200000000773696d706c6500"
	);
   cout << "binding test done." << endl;
   s = simple::_narrow (o);
   if (CORBA::is_nil (s))
     {
       cerr << "client: couldn't _narrow () to \"IDL:simple:1.0\"" << endl;
       exit (-2);
     }

   loops = atoi (argv[1]);
   
   cond = new MICOMT::CondVar;

   for ( i = 0; i < THRS; i++ ) {
     th[i] = new doit();
     th[i]->start( &c[i % 5] );
   }

   sleep(10);
   cout << "starting invoke test." << endl;

   CORBA::Long t1, t2;
   t1 = OSMisc::gettime();                                                    
   
   cond->broadcast();

   for ( i = 0; i < THRS; i++ ) {
     th[i]->wait();
     delete th[i];
   }
//   s->flush();
   cout << "client: done" << endl;
   t2 = OSMisc::gettime();
   cout << (double)(t2 - t1) << " ms total time" << endl; 
   cout << (double)(t2 - t1)/ (loops*THRS) << " ms per call" << endl;     

   return 0;
}
