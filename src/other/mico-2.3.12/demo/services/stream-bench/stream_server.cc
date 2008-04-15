// server.cc
#define MICO_CONF_IMR
#include <CORBA-SMALL.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <fstream>
#else
#include <iostream.h>
#include <fstream.h>
#endif
#include <coss/stream_impl.h>
#include <mico/template_impl.h>


using namespace std;

class AsyncStream : public virtual PassiveCEP_impl
{
  void receive(const MICOStream::StreamData& chunk){
    //   cout << "Receive" << endl; 
  }
};

int main(int argc, char *argv[])
{
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );
    CORBA::BOA_var boa = orb->BOA_init (argc, argv, "mico-local-boa");

    AsyncStream *server = new AsyncStream;

    // this will bind to a default inet address
    server->bind("");
    
    CORBA::String_var ref = orb->object_to_string(server);
    ofstream out("obj.ref");
    out << ref.in() << endl;
    out.close();

    boa->impl_is_ready (CORBA::ImplementationDef::_nil());
    orb->run();

    return 0;

}

