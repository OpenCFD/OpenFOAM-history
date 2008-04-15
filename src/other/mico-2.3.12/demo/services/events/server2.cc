#define MICO_CONF_POA
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

class Consumer_impl : virtual public POA_CosEventComm::PushConsumer {
public:
  Consumer_impl () {}

  void push (const CORBA::Any& data);
  void disconnect_push_consumer ();
};

void Consumer_impl::push (const CORBA::Any& data) {
  cout << "EventConsumer2: push () called !" << endl;
}

void Consumer_impl::disconnect_push_consumer () {
  cout << "EventConsumer2: disconnected !" << endl;
}

int main (int argc, char** argv)
{
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");
  CORBA::Object_var poa_obj = orb->resolve_initial_references("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow(poa_obj);
  assert(!CORBA::is_nil(poa));
  PortableServer::POAManager_var mgr = poa->the_POAManager();

  Consumer_impl* consumer_impl = new Consumer_impl ();
  CosEventComm::PushConsumer_var consumer = consumer_impl->_this();

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

  cerr << "(2) looking for EventChannel" << endl;
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
  CosEventChannelAdmin::ProxyPushSupplier_var proxy_supplier;

  cerr << "EventChannel found !" << endl;

  event_channel = CosEventChannelAdmin::EventChannel::_narrow (obj);
  assert (! CORBA::is_nil (event_channel));
  consumer_admin = event_channel->for_consumers ();
  assert (! CORBA::is_nil (consumer_admin));

  proxy_supplier = consumer_admin->obtain_push_supplier ();
  assert (! CORBA::is_nil (proxy_supplier));
  cerr << "(2) ProxyPushSupplier obtained !" << endl;

  proxy_supplier->connect_push_consumer (CosEventComm::PushConsumer::_duplicate (consumer));
  
  mgr->activate();
  orb->run ();
  return 0;
}
