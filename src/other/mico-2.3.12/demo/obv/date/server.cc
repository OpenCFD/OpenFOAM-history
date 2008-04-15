/*
 * Calendar server
 */

#include <string.h>
#include "date.h"
#include "date_impl.h"
#include "calendar.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else
#include <fstream.h>
#endif


using namespace std;

class Calendar_impl : virtual public POA_Calendar
{
public:
  void insert (date *, const char *);
  CORBA::Boolean find_date (date *, CORBA::String_out);
  CORBA::Boolean find_note (const char *, date_out);

private:
  struct entry {
    date_var d;
    string s;
  };
  vector<entry> data;
};

void
Calendar_impl::insert (date * when, const char * what)
{
  entry e;
  CORBA::add_ref (when);
  e.d = when;
  e.s = what;
  data.push_back (e);
}

CORBA::Boolean
Calendar_impl::find_date (date * when, CORBA::String_out what)
{
  for (CORBA::ULong i=0; i<data.size(); i++) {
    if (data[i].d->equal (when)) {
      what = CORBA::string_dup (data[i].s.c_str());
      return TRUE;
    }
  }

  what = CORBA::string_dup ("not found");
  return FALSE;
}

CORBA::Boolean
Calendar_impl::find_note (const char * what, date_out when)
{
  for (CORBA::ULong i=0; i<data.size(); i++) {
    if (strstr (data[i].s.c_str(), what) != NULL) {
      CORBA::add_ref (data[i].d);
      when = data[i].d.in();
      return TRUE;
    }
  }

  when = 0;
  return FALSE;
}

int
main (int argc, char *argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

  /*
   * Register factories
   */

  date_factory * df = new date_factory;
  orb->register_value_factory ("IDL:date:1.0", df);

  /*
   * Initialize POA, and activate our calendar server
   */

  CORBA::Object_var po = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (po);
  PortableServer::POAManager_var mgr = poa->the_POAManager ();

  PortableServer::ServantBase_var cal = new Calendar_impl;
  CORBA::Object_var obj = cal->_this ();

  /*
   * Write reference to file
   */

  ofstream of ("Calendar.ref");
  CORBA::String_var str = orb->object_to_string (obj.in());
  of << str.in() << endl;
  of.close ();

  cout << "Running." << endl;
  mgr->activate ();
  orb->run ();

  /*
   * Shutdown (never reached)
   */

  poa->destroy (TRUE, TRUE);
  df->_remove_ref ();
  return 0;
}


