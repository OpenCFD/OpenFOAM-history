#define MICO_CONF_IMR
#include <CORBA-SMALL.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif
#include <coss/CosNaming.h>
#include <coss/CosEventComm.h>
#include <coss/CosEventChannelAdmin.h>


using namespace std;

int main (int argc, char** argv)
{
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

  CORBA::Object_var nsobj =
    orb->resolve_initial_references ("NameService");
  assert (! CORBA::is_nil (nsobj));
  
  CosNaming::NamingContext_var nc = 
    CosNaming::NamingContext::_narrow (nsobj);
  assert (! CORBA::is_nil (nc));

  CosNaming::Name name;
  name.length (1);
  name[0].id = CORBA::string_dup ("MyEventChannel");
  name[0].kind = CORBA::string_dup ("");

  cerr << "looking for EventChannel" << endl;
  CORBA::Object_var obj;

#ifdef HAVE_EXCEPTIONS
  try {
    obj = nc->resolve (name);
  }
  catch (...) {
    cerr << "Cannot find EventChannel !" << endl;
    return -1;
  }
#else
  obj = nc->resolve (name);
#endif
  
  CosEventChannelAdmin::EventChannel_var event_channel;
  CosEventChannelAdmin::ConsumerAdmin_var consumer_admin;
  CosEventChannelAdmin::ProxyPullSupplier_var proxy_supplier;

  event_channel = CosEventChannelAdmin::EventChannel::_narrow (obj);
  assert (! CORBA::is_nil (event_channel));
  cerr << "EventChannel found !" << endl;

  consumer_admin = event_channel->for_consumers ();
  assert (! CORBA::is_nil (consumer_admin));

  proxy_supplier = consumer_admin->obtain_pull_supplier ();
  assert (! CORBA::is_nil (proxy_supplier));
  cerr << "ProxyPullSupplier obtained !" << endl;

  CORBA::Any_var any = proxy_supplier->pull ();
  cerr << "EventConsumer3: pull() returned !" << endl;
}
