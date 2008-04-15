
#include <coss/CosNaming.h>
#include <mico/CCMContainer.h>
#include "channel.h"
#include "producer.h"
#include "consumer.h"
#include "message_impl.h"


using namespace std;

int
main (int argc, char *argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
  CORBA::Object_var obj = orb->resolve_initial_references ("NameService");
  CosNaming::NamingContextExt_var nc =
    CosNaming::NamingContextExt::_narrow (obj);

  MICO::CCM::register_all_factories (orb);
  CORBA::ValueFactoryBase_var f = new MessageFactory;
  orb->register_value_factory ("IDL:Message:1.0", f);

  EventChannel_var channel;
  Consumer_var consumers[3];
  Producer_var producers[4];
  MessageConsumer_var mc;
  CORBA::ULong i;

  /*
   * ---
   */

  cout << "creating Event Channel ... " << flush;

  obj = nc->resolve_str ("EventChannelHome");
  EventChannelHome_var channel_home = EventChannelHome::_narrow (obj);

  channel = channel_home->create ();

  cout << "done." << endl;

  /*
   * ---
   */

  cout << "creating Consumers ... " << flush;

  obj = nc->resolve_str ("ConsumerHome");
  ConsumerHome_var consumer_home = ConsumerHome::_narrow (obj);

  for (i=0; i<3; i++) {
    cout << i << " " << flush;
    consumers[i] = consumer_home->create ();
  }

  cout << "done." << endl;

  /*
   * ---
   */

  cout << "subscribing Consumers ... " << flush;

  for (i=0; i<3; i++) {
    cout << i << " " << flush;
    mc = consumers[i]->get_consumer_msg ();
    channel->subscribe_outgoing (mc);
  }

  cout << "done." << endl;

  /*
   * ---
   */

  cout << "creating Producers ... " << flush;

  obj = nc->resolve_str ("ProducerHome");
  ProducerHome_var producer_home = ProducerHome::_narrow (obj);

  for (i=0; i<4; i++) {
    cout << i << " " << flush;
    producers[i] = producer_home->create ();
  }

  cout << "done." << endl;

  /*
   * ---
   */

  cout << "configuring Message ... " << flush;

  Message_var msg = new Message_impl;

  for (i=0; i<4; i++) {
    cout << i << " " << flush;
    char temp[256];
    sprintf (temp, "Producer %d", i);
    msg->text ((const char *) temp);
    producers[i]->myMessage (msg);
  }

  cout << "done." << endl;

  /*
   * ---
   */

  cout << "connecting Producers ... " << flush;

  mc = channel->get_consumer_incoming ();

  for (i=0; i<4; i++) {
    cout << i << " " << flush;
    producers[i]->subscribe_msg (mc);
  }

  cout << "done." << endl;

  /*
   * ---
   */

  cout << "Activating ... " << flush;

  channel->configuration_complete ();

  for (i=0; i<3; i++) {
    consumers[i]->configuration_complete ();
  }

  for (i=0; i<4; i++) {
    producers[i]->configuration_complete ();
  }

  cout << "done." << endl;

  /*
   * ---
   */

  cout << "Send some Events ... " << flush;

  for (i=0; i<4; i++) {
    producers[i]->run (i+1);
  }

  cout << "done." << endl;

  return 0;
}
