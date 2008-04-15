
#include "message_impl.h"

Message_impl::Message_impl ()
{
  text ((const char *) "");
}

CORBA::ValueBase *
MessageFactory::create_for_unmarshal ()
{
  return new Message_impl;
}

