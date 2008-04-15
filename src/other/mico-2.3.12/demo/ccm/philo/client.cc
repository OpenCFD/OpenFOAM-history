#include <mico/CCM.h>
#include <mico/CCMContainer.h>
#include <coss/CosNaming.h>
#include "philo.h"


using namespace std;

int
main (int argc, char *argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
  MICO::CCM::register_all_factories (orb);
  CORBA::Object_var obj = orb->resolve_initial_references ("NameService");
  CosNaming::NamingContextExt_var nc =
    CosNaming::NamingContextExt::_narrow (obj);

  CORBA::ULong i=3;

  if (argc >= 2 && *argv[1] > '1' && *argv[1] < '9') {
    if ((i = atoi (argv[1])) == 0) {
      i = 3;
    }
  }

  cout << "creating Observer ... " << flush;

  DiningPhilosophers::StatusInfoConsumer_var obco =
    DiningPhilosophers::StatusInfoConsumer::_nil ();

  try {
    obj = nc->resolve_str ("ObserverHome");
  } catch (...) {
    obj = CORBA::Object::_nil ();
  }

  if (!CORBA::is_nil (obj)) {
    DiningPhilosophers::ObserverHome_var obh =
      DiningPhilosophers::ObserverHome::_narrow (obj);
    DiningPhilosophers::Observer_var ob = obh->create ();
    obco = ob->get_consumer_info ();
    ob->configuration_complete ();
    cout << "done." << endl;
  }
  else {
    cout << "No Observer Home found." << endl;
  }

  cout << "Looking for TkObserver ... " << flush;

  DiningPhilosophers::StatusInfoConsumer_var obgui =
    DiningPhilosophers::StatusInfoConsumer::_nil ();

  try {
    obj = nc->resolve_str ("TkObserverHome");
  } catch (...) {
    obj = CORBA::Object::_nil ();
  }

  if (!CORBA::is_nil (obj)) {
    DiningPhilosophers::ObserverHome_var obh =
      DiningPhilosophers::ObserverHome::_narrow (obj);
    DiningPhilosophers::Observer_var ob = obh->create ();
    obgui = ob->get_consumer_info ();
    ob->configuration_complete ();
    cout << "done." << endl;
  }
  else {
    obgui = DiningPhilosophers::StatusInfoConsumer::_nil ();
  }

  if (!CORBA::is_nil (obgui)) {
    cout << "found." << endl;
  }
  else {
    cout << "not found." << endl;
  }

  cout << "creating Forks ... " << flush;

  obj = nc->resolve_str ("ForkHome");

  DiningPhilosophers::ForkHome_var csh =
    DiningPhilosophers::ForkHome::_narrow (obj);
  DiningPhilosophers::ForkManager_var fm;
  DiningPhilosophers::Fork_var* f = new DiningPhilosophers::Fork_var[i];

  for (CORBA::ULong j1=0; j1<i; j1++) {
    fm = csh->create ();
    f[j1] = fm->provide_the_fork ();
    fm->configuration_complete ();
  }

  cout << "done." << endl;
  cout << "creating Philosophers ... " << flush;

  obj = nc->resolve_str ("PhilosopherHome");

  DiningPhilosophers::PhilosopherHome_var ph =
    DiningPhilosophers::PhilosopherHome::_narrow (obj);
  DiningPhilosophers::Philosopher_var* p = new DiningPhilosophers::Philosopher_var[i];

  for (CORBA::ULong j2=0; j2<i; j2++) {
    p[j2] = ph->create ();
  }

  cout << "done." << endl;
  cout << "configuring Philosophers ... " << flush;

  const char * philosophers[] = {
    "Socrates", "Plato", "Aristotle", "Pythagoras", "Zeno",
    "Heraclitus", "Democritus", "Descartes", "Kant", "Protagoras"
  };

  for (CORBA::ULong j3=0; j3<i; j3++) {
    if (j3 < 10) {
      p[j3]->name (philosophers[j3]);
    }
    if (!CORBA::is_nil (obco)) {
      p[j3]->subscribe_info (obco);
    }
    if (!CORBA::is_nil (obgui)) {
      p[j3]->subscribe_info (obgui);
    }
  }

  for (CORBA::ULong j4=0; j4<i; j4++) {
    p[j4]->connect_left (f[j4]);
    p[j4]->connect_right (f[(j4+1)%i]);
  }

  cout << "done." << endl;

  cout << "activating Philosophers ... " << flush;

  for (CORBA::ULong j5=0; j5<i; j5++) {
    p[j5]->configuration_complete ();
  }

  cout << "done." << endl;
  return 0;
}
