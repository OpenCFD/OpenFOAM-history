// server.cc
#define MICO_CONF_IMR
#include <CORBA-SMALL.h>
#include <iostream.h>
#include <fstream.h>
#include <coss/stream_impl.h>
#include <mico/template_impl.h>

#include "sound.h"

SunAudio radau;

class AsyncStream : public virtual PassiveCEP_impl
{
public:
  MICOStream::StreamData micro;
  
public:
  AsyncStream(){micro.length(4096);}

  void receive( const MICOStream::StreamData& chunk )
  {

    int _len = 4096;
    if (chunk.length() > 1){ // not an empty packet
      radau.write( (void *) &(chunk[0]), chunk.length());  // to audio
    }
    micro.length(4096);

    _len = radau.read(&(micro[0]), 4096);  // get sound data from microphone

    if (_len > 0){
      // strange, local echo on sound device needed?!?
      radau.write( (void *) &(micro[0]), micro.length());
      micro.length(_len);
      write_async(micro);  // send data to client
    }
    else{   // send empty packet to keep callbacks bouncing
      micro.length(1);
      write(micro);
    }
  }

  void disconnect( void ){
    PassiveCEP_impl::disconnect();
    _boa()->deactivate_impl(CORBA::ImplementationDef::_nil() );
    _orb()->shutdown (TRUE);
  }

};
 


int main(int argc, char *argv[])
{
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );
    CORBA::BOA_var boa = orb->BOA_init (argc, argv, "mico-local-boa");

    AsyncStream *server = new AsyncStream;

    // this will bind to a default inet address
    server->bind("");
    
    radau.open();  // open sound devive

    CORBA::String_var ref = orb->object_to_string(server);
    ofstream out("obj.ref");
    out << ref << endl;
    out.close();

    boa->impl_is_ready (CORBA::ImplementationDef::_nil());
    orb->run ();

    return 0;

}

