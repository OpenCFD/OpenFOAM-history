#define FORCE_MARSHALLING

#define MICO_CONF_POA
#include <CORBA-SMALL.h>
#include "sequence.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#include <fstream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream.h>
#include <fstream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS


using namespace std;

class foo_impl : virtual public POA_foo
{
public:
  foo_impl()
  {
  };
  S* bar( const S& s1, S& s2, S_out s3 )
  {
    cout << "#4" << endl;
    cout << "ULong " << s1.x << endl;
    cout << "Seqence ";
    for( CORBA::ULong i = 0; i < s1.seq.length(); i++ )
      cout << s1.seq[ i ] << " ";
    cout << endl << endl;

    cout << "#5" << endl;
    cout << "ULong " << s2.x << endl;
    cout << "Seqence ";
    {
      for( CORBA::ULong i = 0; i < s2.seq.length(); i++ )
        cout << s2.seq[ i ] << " ";
    }
    cout << endl << endl;

    s2.x = 54321;
    s2.seq.length( 0 );
    
    s3 = new S;
    *s3 = s1;
    
    S* result = new S;
    *result = s1;
    return result;
  };
  SSeq* bar2( const SSeq& seq1, SSeq& seq2, SSeq_out seq3 )
  {
    CORBA::ULong i, j;
    
    cout << "#6" << endl;
    for( i = 0; i < seq1.length(); i++ ) {
      cout << "[" << i << "]" << endl;
      cout << "ULong " << seq1[ i ].x << endl;
      cout << "Seqence ";
      for( j = 0; j < seq1[ i ].seq.length(); j++ )
	cout << seq1[ i ].seq[ j ] << " ";
      cout << endl << endl;
    }

    cout << "#7" << endl;
    for( i = 0; i < seq2.length(); i++ ) {
      cout << "[" << i << "]" << endl;
      cout << "ULong " << seq2[ i ].x << endl;
      cout << "Seqence ";
      for( j = 0; j < seq2[ i ].seq.length(); j++ )
	cout << seq2[ i ].seq[ j ] << " ";
      cout << endl << endl;
    }

    seq2.length( 0 );
    
    seq3 = new SSeq;
    *seq3 = seq1;
    
    SSeq* result = new SSeq;
    *result = seq1;
    return result;
  };
  void bar3( const StringSeq& seq )
  {
    for( CORBA::ULong i = 0; i < seq.length(); i++ ) {
      cout << "<" << i << ">" << seq[ i ].in() << endl;
    }
    cout << endl;
  };
};


int main( int argc, char *argv[] )
{
  // ORB initialization
  CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );
  CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);
  PortableServer::POAManager_var mgr = poa->the_POAManager();

  foo_impl* server = new foo_impl;
  foo_var foo = server->_this();
  string ref;

  mgr->activate();

  if( argc >= 2 && *argv[ 1 ] == 's' ) {
    ofstream objid( "server.objid" );
    CORBA::String_var ref = orb->object_to_string( foo );
    objid << ref.in();
    objid.close();
    orb->run ();
    return 0;
  } else if( argc >= 2 && *argv[ 1 ] == 'c' ) {
    ifstream objid( "server.objid" );
    objid >> ref;
  } else if( argc >= 2 ) {
    cerr << "usage: " << argv[ 0 ] << "[c|s]" << endl;
    return 1;
  } else {
    CORBA::String_var s = orb->object_to_string( foo );
    ref = s;
  }
  
  // client side
#ifdef FORCE_MARSHALLING
  CORBA::Object_var obj2 = new CORBA::Object( new CORBA::IOR( ref.c_str() ) );
#else
  CORBA::Object_var obj2 = orb->string_to_object( ref.c_str() );
#endif
  foo_var client = foo::_narrow( obj2 );
  
  S s1;
  s1.x = 42;
  s1.seq.length( 3 );
  s1.seq[ 0 ] = 11;
  s1.seq[ 1 ] = 22;
  s1.seq[ 2 ] = 33;
  
  {
    S s2;
    s2.x = 11;
    s2.seq.length( 1 );
    s2.seq[ 0 ] = 55;
    
    S_var s3;
    S_var result;
    result = client->bar( s1, s2, s3 );
  
    cout << "#1" << endl;
    cout << "ULong " << s2.x << endl;
    cout << "Seqence ";
    for( CORBA::ULong i = 0; i < s2.seq.length(); i++ )
      cout << s2.seq[ i ] << " ";
    cout << endl << endl;
    
    cout << "#2" << endl;
    cout << "ULong " << s3->x << endl;
    cout << "Seqence ";
    {
      for( CORBA::ULong i = 0; i < s3->seq.length(); i++ )
        cout << s3->seq[ i ] << " ";
    }
    cout << endl << endl;
    
    cout << "#3" << endl;
    cout << "ULong " << result->x << endl;
    cout << "Seqence ";
    {
      for( CORBA::ULong i = 0; i < result->seq.length(); i++ )
        cout << result->seq[ i ] << " ";
    }
    cout << endl << endl;
  }
  
  {
    SSeq seq1;
    seq1.length( 1 );
    seq1[ 0 ] = s1;
    
    SSeq seq2;
    seq2.length( 1 );
    seq2[ 0 ].x = 55;
    seq2[ 0 ].seq.length( 2 );
    seq2[ 0 ].seq[ 0 ] = 66;
    seq2[ 0 ].seq[ 1 ] = 77;
    
    SSeq_var seq3;
    SSeq_var result;
    
    result = client->bar2( seq1, seq2, seq3 );

    cout << "#8" << endl;
    for( CORBA::ULong i = 0; i < seq2.length(); i++ ) {
      cout << "[" << i << "]" << endl;
      cout << "ULong " << seq2[ i ].x << endl;
      cout << "Seqence ";
      for( CORBA::ULong j = 0; j < seq2[ i ].seq.length(); j++ )
	cout << seq2[ i ].seq[ j ] << " ";
      cout << endl << endl;
    }

    cout << "#9" << endl;
    {
      for( CORBA::ULong i = 0; i < seq3->length(); i++ ) {
        cout << "[" << i << "]" << endl;
        cout << "ULong " << seq3[ i ].x << endl;
        cout << "Seqence ";
        for( CORBA::ULong j = 0; j < seq3[ i ].seq.length(); j++ )
	  cout << seq3[ i ].seq[ j ] << " ";
        cout << endl << endl;
      }
    }

    cout << "#10" << endl;
    {
      for( CORBA::ULong i = 0; i < result->length(); i++ ) {
        cout << "[" << i << "]" << endl;
        cout << "ULong " << result[ i ].x << endl;
        cout << "Seqence ";
        for( CORBA::ULong j = 0; j < result[ i ].seq.length(); j++ )
	  cout << result[ i ].seq[ j ] << " ";
        cout << endl << endl;
      }
    }
  }
  
  StringSeq strseq;
  strseq.length( 2 );
  strseq[ 0 ] = (const char *) "Hello";
  strseq[ 1 ] = (const char *) "World";
  client->bar3( strseq );
  CORBA::release( foo );
}
