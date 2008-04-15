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

import java.lang.*;
import java.util.*;
import java.awt.*;

import DIIObject;
import DIISequence;
import DIIBoolean;
import DIIString;
import DIIRequest;

import ConceptualGraph;
import CGParser;

public class IRBrowserProxy {
private String key;

public IRBrowserProxy (String k)
    {
	key = k;
    }
    
public String getwd ()
	throws DIIException
    {
	DIIRequest req = new DIIRequest (key, "getwd");
	DIIString res = new DIIString();

	req.setResult (res);
	req.invoke ();
	return (String)res.toJava();
    }

public boolean back ()
	throws DIIException
    {
	DIIRequest req = new DIIRequest (key, "back");
	DIIBoolean result = new DIIBoolean();

	req.setResult (result);
	req.invoke();
	return ((Boolean)result.toJava()).booleanValue();
    }

public boolean cd (String _el)
	throws DIIException
    {
	DIIRequest req = new DIIRequest (key, "cd");
	DIIString el = new DIIString (_el);
	DIIBoolean res = new DIIBoolean ();

	req.addInArg (el);
	req.setResult (res);
	req.invoke ();

	return ((Boolean)res.toJava()).booleanValue();
    }

public Vector list ()
	throws DIIException
    {
	DIIRequest req = new DIIRequest (key, "list");
	DIISequence res = new DIISequence (new DIIString());
	
	req.setResult (res);
	req.invoke ();

	return (Vector)res.toJava();
    }

public String type (String _el)
	throws DIIException
    {
	DIIRequest req = new DIIRequest (key, "type");
	DIIString el = new DIIString (_el);
	DIIString result = new DIIString();

	req.addInArg (el);
	req.setResult (result);
	req.invoke();
	return (String)result.toJava();
    }

public String describe( String _el )
       throws DIIException
  {
    DIIRequest req = new DIIRequest( key, "describe" );
    DIIString el = new DIIString( _el );
    DIIString result = new DIIString();
    
    req.addInArg( el );
    req.setResult (result);
    req.invoke();
    return (String)result.toJava();
  }

public Vector types ()
	throws DIIException
    {
	DIIRequest req = new DIIRequest (key, "types");
	DIISequence res = new DIISequence (new DIIString());

	req.setResult (res);
	req.invoke ();

	return (Vector)res.toJava();
    }

public ConceptualGraph optemplate (String _el)
	throws DIIException
    {
	DIIRequest req = new DIIRequest (key, "optemplate");
	DIIString el = new DIIString (_el);
	DIIString res = new DIIString ();

	req.addInArg (el);
	req.setResult (res);
	req.invoke ();

	ConceptualGraph cg = null;
	try {
	    CGParser cgp = new CGParser();
	    cg = cgp.Parse ((String)res.toJava());
	} catch (Exception e) {
	    GlobalVars.DisplayError ("cg parsing failed");
	}
	return cg;
    }

public String repoid ()
	throws DIIException
    {
	DIIRequest req = new DIIRequest (key, "repoid");
	DIIString res = new DIIString();

	req.setResult (res);
	req.invoke ();
	return (String)res.toJava();
    }
}
