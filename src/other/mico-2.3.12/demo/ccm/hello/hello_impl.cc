
#include "hello.h"


using namespace std;

class HelloWorld_impl : virtual public CCM_HelloWorld {
private:
  CORBA::String_var _message;

public:
  HelloWorld_impl (const char * initial)
    {
      _message = initial;
    }

  void sayHello ()
    {
      cout << _message << endl;
    }

  void message (const char * val)
    {
      _message = CORBA::string_dup (val);
    }

  char * message ()
    {
      return CORBA::string_dup (_message);
    }
};

class HelloHome_impl : virtual public CCM_HelloHome {
private:
  CORBA::String_var _initial_message;

public:
  HelloHome_impl ()
    {
      _initial_message = CORBA::string_dup ("Hello World");
    }

  Components::EnterpriseComponent_ptr create ()
    {
      return new HelloWorld_impl (_initial_message);
    }

  void initial_message (const char * val)
    {
      _initial_message = CORBA::string_dup (val);
    }

  char * initial_message ()
    {
      return CORBA::string_dup (_initial_message);
    }
};

extern "C" {
  Components::HomeExecutorBase_ptr create_HelloHome ()
  {
    return new HelloHome_impl;
  }
}
