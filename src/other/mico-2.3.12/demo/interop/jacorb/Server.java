/*  Copyright (C) Thomas Fricke 1998
 *
 *  This example has been derived from the simple Jacorb example1
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

import org.omg.CORBA.ORB;
import org.omg.CosNaming.*;
import java.io.*;

public class Server
{

	public static void main( String[] args )
	{
		try
		{
			// initialize ORB and BOA

			org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init();
			org.omg.CORBA.BOA boa = orb.BOA_init();

			// let the BOA create the server CORBA object

			org.omg.CORBA.Object server = 
				boa.create( new AccountImpl(),	
						"IDL:Account:1.0");

			// tell the BOA about it
			// (this named_obj_is_ready()-call is a proprietary
			// shortcut for calling boa.obj_is_ready() and
			// telling the name server to bind a name to
			// this object. 

			boa.obj_is_ready( server,"theFoo");
			//			boa.obj_is_ready( server);
			try {
			    BufferedWriter out
				= new BufferedWriter( new FileWriter( "account.ref" ) );
			    out.write(orb.object_to_string(server));
			    out.newLine();
			    out.close();
			} catch( IOException ex ) {
			    System.out.println( "Could not open file 'account.ref'" );
			    System.exit( -1 );
			}

			//			System.out.println(orb.object_to_string(server));
			// accept incoming requests

			boa.impl_is_ready();

		} catch (Exception e ){
			e.printStackTrace();
		}
	}
}
