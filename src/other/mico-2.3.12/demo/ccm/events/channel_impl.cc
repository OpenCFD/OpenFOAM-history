
#include <iostream.h>
#include "channel.h"
#include "message_impl.h"

class MyEventChannel_impl : virtual public MyEventChannel
{
private:
  CCM_EventChannel_Context_var _ctx;
  bool active;

public:
  MyEventChannel_impl ()
    {
      active = false;
    }
  
  void push_incoming (Message * msg)
    {
      if (active) {
	_ctx->push_outgoing (msg);
      }
    }

  void set_session_context (Components::SessionContext_ptr ctx)
    {
      _ctx = CCM_EventChannel_Context::_narrow (ctx);
    }

  void ccm_activate ()
    {
      cout << "Event Channel is active." << endl;
      active = true;
    }

  void ccm_passivate ()
    {
      cout << "Event Channel is passive." << endl;
      active = false;
    }

  void ccm_remove ()
    {
      cout << "Event Channel is being removed." << endl;
    }
};

class MyEventChannelHome_impl : virtual public CCM_EventChannelHome {
public:
  Components::EnterpriseComponent_ptr create ()
    {
      return new MyEventChannel_impl;
    }
};

extern "C" {
  Components::HomeExecutorBase_ptr
  create_EventChannelHome (CORBA::ORB_ptr orb)
  {
    CORBA::ValueFactoryBase_var f = new MessageFactory;
    orb->register_value_factory ("IDL:Message:1.0", f);
    return new MyEventChannelHome_impl;
  }
}
