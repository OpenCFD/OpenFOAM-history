/*  Copyright (C) Thomas Fricke 1998
 *
 *  This example has been derived from the simple Mico account example
 *  to demonstrate the interoperability of Mico and Jacorb.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include <fstream.h>
#include "Account.h"


class Account_impl : virtual public Account_skel
{
private:
  CORBA::Long _current_balance;

public:
    Account_impl (const CORBA::BOA::ReferenceData &refdata)
	: Account_skel (refdata)
    {
        _current_balance = 0;
    }

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

  CORBA::ORB::ObjectTag_var tag = CORBA::ORB::string_to_tag ("theFoo");
  Account_impl* server = new Account_impl(tag);
  CORBA::String_var ref = orb->object_to_string( server );
  ofstream ostr( "account.ref" );
  ostr << (const char *)ref;
  ostr.close();
  
  boa->impl_is_ready( CORBA::ImplementationDef::_nil() );
  orb->run();
  return 0;
}
