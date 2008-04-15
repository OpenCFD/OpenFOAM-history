
#ifndef __EVENT_IMPL_H__
#define __EVENT_IMPL_H__

#include "message.h"

class Message_impl :
  virtual public OBV_Message,
  virtual public CORBA::DefaultValueRefCountBase
{
public:
  Message_impl ();
};

class MessageFactory : public CORBA::ValueFactoryBase
{
public:
  CORBA::ValueBase * create_for_unmarshal ();
};

#endif
