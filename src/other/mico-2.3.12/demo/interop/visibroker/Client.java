/*
 *  MICO --- a CORBA 2.0 implementation
 *  Copyright (C) 1997 Kay Roemer & Arno Puder
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
 *  Send comments and/or bug reports to:
 *                 mico@informatik.uni-frankfurt.de
 */

import java.io.*;
import Account;

public class Client {

  public static void main( String[] args ) {
    // Initialize the ORB.
    org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init( args, null );

    String ref = null;

    try {
      DataInputStream in =
	new DataInputStream( new FileInputStream( "account.ref" ) );
      ref = in.readLine();
    } catch( IOException ex ) {
      System.out.println( "Could not open file 'account.ref'" );
      System.exit( -1 );
    }

    org.omg.CORBA.Object obj = orb.string_to_object( ref );
    Account account = AccountHelper.narrow( obj );

    account.deposit( 700 );
    account.withdraw( 700 );
    account.deposit( 700 );
    account.withdraw( 700 );
    account.deposit( 700 );
    account.withdraw( 700 );
    account.deposit( 700 );
    account.withdraw( 700 );
    account.deposit( 700 );
    account.withdraw( 250 );
    System.out.println("The balance is " + account.balance() );
  }

}
