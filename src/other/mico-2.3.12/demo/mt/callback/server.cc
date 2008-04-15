
#include "callback.h"
#include <CORBA.h>
#include <iostream>
#include <fstream>
#include <unistd.h>


using namespace std;

CORBA::ORB_ptr orb = CORBA::ORB::_nil();
CORBA::String_var mess; // = CORBA::String::_nil();
Callback_ptr callback = Callback::_nil();

class Worker
    : public MICOMT::Thread
{
public:
    void _run(void*)
    {
	for (;;) {
	    cout << "sending reply ``" << mess << "'' to the client." << endl;
	    callback->reply(mess);
#ifndef _WIN32
	    sleep(1);
#else // _WIN32
	    _sleep(1);
#endif // _WIN32
	}
    }
};

// Implementation for interface Sender
class Sender_impl
    : virtual public POA_Sender
{
public:
    void message(Callback_ptr obj, const char* msg)
    {
	cout << "client send message ``" << msg << "''" << endl;
	callback = Callback::_duplicate(obj);
	mess = msg;
	Worker* worker = new Worker();
	worker->start();
    }
};

int
main(int argc, char* argv[])
{
    orb = CORBA::ORB_init(argc, argv, "mico-local-orb");
    CORBA::Object_ptr obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_ptr poa = PortableServer::POA::_narrow(obj);
    assert(!CORBA::is_nil(poa));

    Sender_impl* servant = new Sender_impl;
    PortableServer::ObjectId_var id = poa->activate_object(servant);

    PortableServer::POAManager_ptr manager = poa->the_POAManager();
    manager->activate();

    cout << "server started." << endl;
    orb->run();

    CORBA::release(manager);
    CORBA::release(poa);
    CORBA::release(orb);
}

