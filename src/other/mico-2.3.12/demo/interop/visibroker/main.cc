#include <fstream.h>
#include "Account.h"


class Account_impl : virtual public Account_skel
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
    _current_balance += amount;
  };
  void withdraw( CORBA::ULong amount )
  {
    _current_balance -= amount;
  };
  CORBA::Long balance()
  {
    return _current_balance;
  };
};



int main( int argc, char *argv[] )
{
  // ORB initialization
  CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "mico-local-orb" );
  CORBA::BOA_var boa = orb->BOA_init (argc, argv, "mico-local-boa");

  Account_impl* server = new Account_impl;
  CORBA::String_var ref = orb->object_to_string( server );
  ofstream ostr( "account.ref" );
  ostr << ref;
  ostr.close();
  
  boa->impl_is_ready( CORBA::ImplementationDef::_nil() );
  orb->run ();

  return 0;
}
