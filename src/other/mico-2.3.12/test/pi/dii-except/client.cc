#include "z.h"
#include "request.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

using namespace std;

int
main (int argc, char *argv[])
{
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // locate object
      Z_var z;
      {
        char pwd[256], uri[300];
        sprintf (uri, "file://%s/Z.ref", getcwd(pwd, 256));

        CORBA::Object_var obj = orb->string_to_object (uri);
        z = Z::_narrow (obj);

        if (CORBA::is_nil (z)) {
	  cout << "oops: could not locate Z" << endl;
          exit (1);
        }
      }

      // Request
      CORBA::Request_var req = create_request( z, "dii" );
      req->invoke();
      CORBA::Exception* ex = req->env()->exception();
      printf( "Client DII exception: %s\n", ex->_repoid() );
      
      CORBA::UnknownUserException* uuex = 
      CORBA::UnknownUserException::_downcast(ex);
      const char *real_repoid = uuex->_except_repoid();
      printf( "Client DII real exception: %s\n", real_repoid );

      // The same but on server
      CORBA::String_var server_ex_repoid = z->stat();
      printf( "Server DII exception: %s\n", server_ex_repoid.in() );

      return 0;
}
