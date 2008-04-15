/*
 *  MICO --- a free CORBA implementation
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

import java.net.*;
import java.io.*;
import java.util.*;


class TransportLayer {
	Socket		 sock;
	OutputStream out;
	InputStream	 in;
	boolean		 connected;

	public TransportLayer( String host ) {
		connected = true;
		try {
			sock = new Socket( host, 3399 );
		} catch( UnknownHostException e ) {
			connected = false;
		} catch( IOException e ) {
			connected = false;
		}
		if( connected ) {
			try {
				out = sock.getOutputStream();
				in = sock.getInputStream();
				sendString( "Java-Host" );  //!!!
			} catch( IOException e ) {
				sock = null;
				connected = false;
			}
		}
	}

	public boolean connected() {
		return connected;
	}

	public String transmitString( String s ) {
		sendString( s );
		return receiveString();
	}

	void sendString( String s ) {
		long l;
		int b1, b2, b3, b4;

		if( !connected )
			return;

		l = s.length() + 1;
		b1 = (int) (( l >> 24 ) & 255);
		b2 = (int) (( l >> 16 ) & 255);
		b3 = (int) (( l >>  8 ) & 255);
		b4 = (int) (l & 255);

		try {
			out.write( b1 );
			out.write( b2 );
			out.write( b3 );
			out.write( b4 );
			for( int i = 0; i < s.length(); i++ ) {
				out.write( s.charAt( i ) );
			}
			out.write( 0 );
			out.flush();
		} catch( IOException e ) {
				sock = null;
				connected = false;
		}
	}

	String receiveString() {
		long l;
		long b1, b2, b3, b4;
		String err = "Lost connection to diiproxy!";

		if( !connected )
			return err;

		try {
			b1 = (long) in.read();
			b2 = (long) in.read();
			b3 = (long) in.read();
			b4 = (long) in.read();
			if( b1 == -1 || b2 == -1 || b3 == -1 || b4 == -1 ) {
				sock = null;
				connected = false;
				return err;
			}
			l = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
			StringBuffer s = new StringBuffer( (int) (l - 1) );
			for( int i = 0; i < l; i++ ) {
				byte b;
				b = (byte) in.read();
				if( b == 0 ) continue;
				s.append( (char) b );
			}
			return s.toString();
		} catch( IOException e ) {
				sock = null;
				connected = false;
		}
		return err;
	}
	public void disconnect () {
            try {
                sock.close ();
            } catch (IOException e) {
            }
	}
}
