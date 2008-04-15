#include <CORBA-SMALL.h>
#include "account.h"

int main( int argc, char *argv[] )
{
    // ORB initialization
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );
    CORBA::BOA_var boa = orb->BOA_init (argc, argv, "mico-local-boa");

    // client side
    assert (argc == 2);
    CORBA::ORB::ObjectTag_var tag = CORBA::ORB::string_to_tag ("foobar");
    /*
     * bind to an instance of type "IDL:Account:1.0" whose ReferenceData
     * equals "foobar".
     */
    CORBA::Object_var obj = orb->bind ("IDL:Account:1.0", tag, argv[1]);
    assert (!CORBA::is_nil (obj));
    Account_var client = Account::_narrow( obj );

    client->deposit( 100 );
    client->deposit( 100 );
    client->deposit( 100 );
    client->deposit( 100 );
    client->deposit( 100 );
    client->withdraw( 240 );
    client->withdraw( 10 );
    cout << "Balance is " << client->balance() << endl;

    return 0;
}
