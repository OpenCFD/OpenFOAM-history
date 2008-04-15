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
#include "req.h"


using namespace std;

class bench_impl : public virtual bench_skel
{
  void  receive( const transtype& data )
    {};
};

int main(int argc, char *argv[])
{
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );
    CORBA::BOA_var boa = orb->BOA_init (argc, argv, "mico-local-boa");

    bench_impl *server = new bench_impl;
    
    CORBA::String_var ref = orb->object_to_string(server);
    ofstream out("obj.ref");
    out << ref.in() << endl;
    out.close();

    boa->impl_is_ready (CORBA::ImplementationDef::_nil());
    orb->run ();

    return 0;

}

