
#include "consumer.h"
#include "message_impl.h"


using namespace std;

class MyConsumer_impl : virtual public CCM_Consumer
{
public:
  void push_msg (Message * msg)
    {
      cout << "Consumer: got message `" << msg->text() << "'" << endl;
    }
};

class MyConsumerHome_impl : virtual public CCM_ConsumerHome {
public:
  Components::EnterpriseComponent_ptr create ()
    {
      return new MyConsumer_impl;
    }
};

extern "C" {
  Components::HomeExecutorBase_ptr
  create_ConsumerHome (CORBA::ORB_ptr orb)
  {
    CORBA::ValueFactoryBase_var f = new MessageFactory;
    orb->register_value_factory ("IDL:Message:1.0", f);
    return new MyConsumerHome_impl;
  }
}
