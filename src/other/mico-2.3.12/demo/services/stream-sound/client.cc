// client.cc
// test der streams 
#include <iostream.h>
#include <fstream.h>
#include <CORBA-SMALL.h>
#include <coss/stream_impl.h>
#include <mico/template_impl.h>

#include "sound.h"

SunAudio radau;  // Audio device

class AsyncStream : public virtual ActiveCEP_impl
{
public: 
MICOStream::StreamData micro;

public:
  AsyncStream(){micro.length(4096);}

  void receive( const MICOStream::StreamData& chunk )
  {

    int _len = 4096; 

    if (chunk.length() > 1)  // sound data from server
      radau.write((void *) &(chunk[0]), chunk.length());

    micro.length(4096);

    _len = radau.read(&(micro[0]), 4096);  // data from microphone
    if (_len > 0){
      micro.length(_len);
      write_async(micro);   // send to client
    }
    else{
      micro.length(1);
      write(micro);
    }  
  }
};

int main(int argc, char *argv[])
{
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );
    CORBA::BOA_var boa = orb->BOA_init (argc, argv, "mico-local-boa");

    // instantiate active cep

    AsyncStream stream;

    // get passive peer
    char ref[1000];
    ifstream in("obj.ref");
    in >> ref;
    in.close();

    CORBA::Object_var obj = orb->string_to_object(ref);
    assert (!CORBA::is_nil (obj));
    MICOStream::PassiveCEP_var server = MICOStream::PassiveCEP::_narrow( obj );

    stream.connect(server->bind_addr());

    // initiate server send, so callbacks start...
       
    MICOStream::StreamData_var _start = new MICOStream::StreamData(1);
    _start->length(10);
    _start[0] = (CORBA::Octet) '0';
    stream.write(_start);

    radau.open();
    
    orb->run();

    return(0);
}
