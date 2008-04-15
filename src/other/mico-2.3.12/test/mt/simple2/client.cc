#include "simple.h"
#include <CORBA.h>
#include <mico/os-misc.h>                                                      
#include <mico/util.h>                                                      
#include <pthread.h>
#include <sched.h>

class doit1: public MICOMT::Thread {
public:
  void _run (void *arg);
};

class doit2: public MICOMT::Thread {
public:
  void _run (void *arg);
};

simple1_var s1;
simple2_var s2;
int loops;

#define THRS  2

//doit *th[THRS];
doit1 *th1;
doit2 *th2;

char c[] = "+.*-#";

int main (int argc, char **argv);


void doit2::_run(void *v) {

  char c = *(char *)v; 

  for (int i = 0; i < loops; i++)
    {
      if (( i % 1000 ) == 0) 
	MICODebug::instance()->printer() << i << endl;
      s1->func( c );
      yield();
    }
}

void doit1::_run(void *v) {

  char c = *(char *)v; 

  for (int i = 0; i < loops; i++)
    {
      if (( i % 1000 ) == 0) 
	MICODebug::instance()->printer() << i << endl;
      s2->func( c );
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
     o = orb->bind ("IDL:simple1:1.0");
     if (CORBA::is_nil (o))
       {
         cerr << "client: orb->bind (): to \"IDL:simple1:1.0\" failed";
         cerr << "\nclient: is the server running?" << endl;
         exit (-1);
       }
   }
   s1 = simple1::_narrow (o);
   if (CORBA::is_nil (s1))
     {
       cerr << "client: couldn't _narrow () to \"IDL:simple1:1.0\"" << endl;
       exit (-2);
     }

   o = orb->bind ("IDL:simple2:1.0");
   if (CORBA::is_nil (o))
     {
       cerr << "client: orb->bind (): to \"IDL:simple12:1.0\" failed";
       cerr << "\nclient: is the server running?" << endl;
       exit (-1);
     }
   s2 = simple2::_narrow (o);
   if (CORBA::is_nil (s2))
     {
       cerr << "client: couldn't _narrow () to \"IDL:simple2:1.0\"" << endl;
       exit (-2);
     }
   
   loops = atoi (argv[1]);
   
   CORBA::Long t1, t2;
   t1 = OSMisc::gettime();                                                    
   
   th1 = new doit1();
   th1->start( &c[0] );

   th2 = new doit2();
   th2->start( &c[1] );

   th1->wait();
   delete th1;

   th2->wait();
   delete th2;

   s1->flush();
   s2->flush();
   cout << "client: done" << endl;
   t2 = OSMisc::gettime();
   cout << (double)(t2 - t1) << " ms total time" << endl; 
   cout << (double)(t2 - t1)/ loops << " ms per call" << endl;     

   return 0;
}
