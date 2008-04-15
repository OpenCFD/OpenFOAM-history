
#include "producer.h"
#include "message_impl.h"

class MyProducer_impl : virtual public MyProducer
{
private:
  CCM_Producer_Context_var _ctx;
  Message_var _mymsg;
  bool active;

public:
  MyProducer_impl ()
    {
      _mymsg = new Message_impl;
      _mymsg->text ((const char*) "Hello World!");
      active = false;
    }

  void myMessage (Message * newmsg)
    {
      CORBA::add_ref (newmsg);
      _mymsg = newmsg;
    }

  Message * myMessage ()
    {
      CORBA::add_ref (_mymsg);
      return _mymsg;
    }

  void run (CORBA::ULong count)
    {
      if (active) {
	for (CORBA::ULong i=0; i<count; i++) {
	  _ctx->push_msg (_mymsg);
	}
      }
    }

  void set_session_context (Components::SessionContext_ptr ctx)
    {
      _ctx = CCM_Producer_Context::_narrow (ctx);
    }

  void ccm_activate ()
    {
      if (!active) {
	active = true;
	run (1);
      }
    }

  void ccm_passivate ()
    {
      active = false;
    }

  void ccm_remove ()
    {
    }
};

class MyProducerHome_impl : virtual public CCM_ProducerHome {
public:
  Components::EnterpriseComponent_ptr create ()
    {
      return new MyProducer_impl;
    }
};

extern "C" {
  Components::HomeExecutorBase_ptr
  create_ProducerHome (CORBA::ORB_ptr orb)
  {
    CORBA::ValueFactoryBase_var f = new MessageFactory;
    orb->register_value_factory ("IDL:Message:1.0", f);
    return new MyProducerHome_impl;
  }
}
