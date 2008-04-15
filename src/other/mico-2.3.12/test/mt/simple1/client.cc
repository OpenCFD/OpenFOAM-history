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

#define THRS  20

doit *th[THRS];

char c[] = "+.*-#";

int main (int argc, char **argv);

MICOMT::CondVar *cond1;
MICOMT::CondVar *cond2;

void doit::_run(void *v) {

  char c = *(char *)v; 

  mt.lock();
  cond1->wait( mt );
  mt.unlock();

  for (int i = 0; i < 10; i++) {
      s->func( c );
      yield();
  }

  mt.lock();
  cond2->wait( mt );
  mt.unlock();

  for (int i = 0; i < loops; i++)
  {
    if (( i % 1000 ) == 0) 
      cout << i << endl;

    try
    {
      s->func( c );
    }
    catch (CORBA::Exception &ex)
    {
      cerr << "client: caught exception";
      ex._print (cerr);
      cerr << endl;
      return;
    }
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

   for ( int n = 0; n < 10; n++ ) {
     o = orb->bind ("IDL:simple:1.0");
     if (CORBA::is_nil (o))
       {
         cerr << "client: orb->bind (): to \"IDL:simple:1.0\" failed";
         cerr << "\nclient: is the server running?" << endl;
         exit (-1);
       }
   }

   cerr << "binding test done." << endl;
   s = simple::_narrow (o);
   if (CORBA::is_nil (s))
     {
       cerr << "client: couldn't _narrow () to \"IDL:simple:1.0\"" << endl;
       exit (-2);
     }
   
   cond1 = new MICOMT::CondVar;
   cond2 = new MICOMT::CondVar;

   loops = atoi (argv[1]);
   
   for ( i = 0; i < THRS; i++ ) {
     th[i] = new doit();
     th[i]->start( &c[i % 5] );
   }

   // wait for all threads to start up
   sleep(2);

   // do a couple of invokations to make sure the
   // cacheline are hot and all server-threads have been started
   cerr << "starting pre-invoke test." << endl;
   cond1->broadcast();

   // wait for all threads to be finisched
   sleep(5);

   cerr << "starting invoke test." << endl;

   CORBA::Long t1, t2;
   t1 = OSMisc::timestamp();                                                    

   cond2->broadcast();

   for ( i = 0; i < THRS; i++ ) {
     th[i]->wait();
     delete th[i];
   }
#ifdef FLUSH
   try
   {
      s->flush();
   }
   catch (CORBA::Exception &ex)
   {
      cerr << "client: caught exception";
      ex._print (cerr);
      cerr << endl;
   }
#endif // FLUSH
   cerr << "client: done" << endl;
   t2 = OSMisc::timestamp();

   double tt, pc;
   
   tt = (double)(t2 - t1);
   pc = (double)(t2 - t1)/ (loops*THRS);
   
   cerr << tt << " ms total time" << endl; 
   cerr << pc << " ms per call" << endl;     
   cout << loops << " " << pc << " " << tt << endl;

   return 0;
}
