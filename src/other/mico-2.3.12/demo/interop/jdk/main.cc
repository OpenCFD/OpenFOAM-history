#include <iostream.h>
#include <fstream.h>
#include "Account.h"


class Account_impl : virtual public POA_Account
{
private:
    CORBA::Long _current_balance;

public:
    Account_impl()
    {
	_current_balance = 0;
    };
    void deposit( CORBA::ULong amount )
    {
	cout << "Deposit: " << amount << endl;
	_current_balance += amount;
    };
    void withdraw( CORBA::ULong amount )
    {
	cout << "Withdraw: " << amount << endl;
	_current_balance -= amount;
    };
    CORBA::Long balance()
    {
	cout << "Balance= " << _current_balance << endl;
	return _current_balance;
    };
};



int main( int argc, char *argv[] )
{
    // ORB initialization
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    // POA initialization
    CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
    PortableServer::POAManager_var mgr = poa->the_POAManager();
    
    Account_impl* server = new Account_impl;

    PortableServer::ObjectId_var oid = poa->activate_object (server);

    ofstream of( "account.ref" );
    CORBA::Object_var ref = poa->id_to_reference (oid.in());
    CORBA::String_var str = orb->object_to_string (ref.in());
    of << str.in() << endl;
    of.close();
    
    printf ("Running.\n");

    mgr->activate ();
    orb->run();

    return 0;
}
