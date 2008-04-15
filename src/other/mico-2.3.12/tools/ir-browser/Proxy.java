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

import java.util.*;
import ConceptualGraph;
import TransportLayer;
import parser;
import DIIException;


public class Proxy {
	private TransportLayer transportLayer;
	private parser theParser;

	public Proxy( TransportLayer theConnection )
	{
		transportLayer = theConnection;
	}

	private void executeCommand( String command )
	{
		String result;

		result = transportLayer.transmitString( command );
                try {
                    /* allocate a parser object */
                    theParser = new parser();

                    theParser.setToBeParsed(result);
                    theParser.parse();

                } catch (java.lang.Exception ex) {
                    System.err.println("Exception: " + ex.getMessage());
                    ex.printStackTrace();
                    System.exit(-1);
                }
	}

	private void checkForError()
		throws DIIException
	{
		// This function should check whether the last
		// command caused an error
                int errnum = theParser.getErrNum();
		if (errnum != 0) {
                    String errmsg = theParser.getErrMsg();
                    theParser.setError (0, "");
                    // Error occured. Throw an exception
                    throw (new DIIException(errnum, errmsg));
		}
	}

	public String getVersion()
		throws DIIException
	{
		executeCommand( "@version" );
		checkForError();
		return theParser.getAnswerString();
	}

	public void setVerbose( boolean flag )
		throws DIIException
	{
		if( flag )
			executeCommand( "@verbose-on" );
		else
			executeCommand( "@verbose-off" );
		checkForError();
	}

        public ConceptualGraph invoke( ConceptualGraph query, String key)
		throws DIIException
	{
		StringBuffer ostr;

		ostr = new StringBuffer();
		ostr.append( "@invoke " + key + " : ");
		query.print( ostr );
		executeCommand( ostr.toString() );
		checkForError();
		return theParser.getAnswerConceptualGraph();
	}

        public String bind( String obj)
		throws DIIException
	{
		StringBuffer ostr;

		ostr = new StringBuffer();
		ostr.append( "@bind '" + obj + "'");
		executeCommand( ostr.toString() );
		checkForError();
		return theParser.getAnswerKey();
	}

        public void release( String key)
		throws DIIException
	{
		StringBuffer ostr;

		ostr = new StringBuffer();
		ostr.append( "@release " + key);
		executeCommand( ostr.toString() );
		checkForError();
	}
}
