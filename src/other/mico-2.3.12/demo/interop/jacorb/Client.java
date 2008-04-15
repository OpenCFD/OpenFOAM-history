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
import java.lang.*;

public class Client
{

    public static void pm(Object o){
	java.lang.reflect.Method ms[]=o.getClass().getMethods();
	System.out.println(o.getClass().getName());
	for(int i=0; i< ms.length; i++){
	    System.out.print(ms[i].getReturnType().getName()+
			     " "+ms[i].getName()+"(");
	    java.lang.Class cs[]=ms[i].getParameterTypes();
	    for(int j=0; j<cs.length; j++)
		System.out.println("  "+cs[j].getName()+
				   ((j==cs.length-1)?"":","));
	    System.out.println(")");
	}
    }

    public static void main( String[] args )
    {

	try
	    { 
		// init the ORB
		 org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init();

		Account  account;
		

		org.omg.CORBA.Object o=orb.string_to_object(args[0]);
// 		pm(o);
// 		if(o==null)
// 		    System.out.println("o is null");		    
		account = AccountHelper.narrow(o);
// 		System.out.println("typeId: "+((jacorb.Orb.Stub)o).typeId());
// 		System.out.println("is_a: "+
// 				   ((jacorb.Orb.Stub)o)._is_a("IDL:Account:1.0"));

// 		if(account==null)
// 		    System.out.println("a is null");		    
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
		System.out.println("The balance is " + 
				   account.balance() );


	    } catch (jacorb.Orb.SystemException se){
		se.printStackTrace();
	    } // catch (Exception e){}
    }
}
