
#include "callback.h"
#include <CORBA.h>
#include <iostream>
#include <fstream>
#include <unistd.h>


using namespace std;

CORBA::ORB_ptr orb = CORBA::ORB::_nil();

class Callback_impl
    : virtual public POA_Callback
{
public:
    void
    reply(const char* msg)
    { cout << "got: " << msg << endl; }
};

class ORBThread
    : virtual public MICOMT::Thread
{
public:
    void
    _run(void*)
    { orb->run(); }
};

int
main(int argc, char* argv[])
{
    orb = CORBA::ORB_init(argc, argv, "mico-local-orb");
    CORBA::Object_ptr o = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_ptr poa = PortableServer::POA::_narrow(o);
    assert(!CORBA::is_nil(poa));

    Callback_impl* servant = new Callback_impl;
    poa->activate_object(servant);

    CORBA::Object_ptr obj = poa->servant_to_reference(servant);
    Callback_ptr callback = Callback::_narrow(obj);

    PortableServer::POAManager_ptr manager = poa->the_POAManager();
    manager->activate();
    CORBA::release(manager);

    // create and start new thread in which will run orb
    // for receiving requests to the Callback object.
    ORBThread* main = new ORBThread;
    main->start();

    CORBA::Object_ptr obj2 = orb->bind("IDL:Sender:1.0");
    Sender_ptr sender = Sender::_narrow(obj2);
    assert(!CORBA::is_nil(sender));
    CORBA::String_var msg = "Hello";
    cout << "sending message ``" << msg << "'' to the server." << endl;
    sender->message(callback, msg);
  
    for (;;) {
	// we can do something in the infinite loop
#ifndef _WIN32
	sleep(1);
#else // _WIN32
	_sleep(1);
#endif // _WIN32
    }
    // or we can use main->wait() for waiting on main thread finish
    return 0;
}
