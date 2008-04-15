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

import DIIBoolean;
import DIIString;
import DIISequence;
import ConceptualGraph;

class NamedValue {
public static final int IN = 0;
public static final int OUT = 1;
public static final int INOUT = 2;
    
public int flags;
public DIIObject value;

public NamedValue (int f, DIIObject v)
    {
        flags = f;
        value = v;
    }
}


public class DIIRequest {
private String key, operation;
private Vector args;
private NamedValue result;

public DIIRequest (String k, String op)
    {
        key = k;
        operation = op;
        result = null;
        args = new Vector();
    }

public void addInArg (DIIObject o)
    {
        args.addElement (new NamedValue (NamedValue.IN, o));
    }

public void addOutArg (DIIObject o)
    {
        args.addElement (new NamedValue (NamedValue.OUT, o));
    }

public void addInOutArg (DIIObject o)
    {
        args.addElement (new NamedValue (NamedValue.INOUT, o));
    }

public void setResult (DIIObject o)
    {
        result = new NamedValue (NamedValue.OUT, o);
    }

private ConceptualGraph mkConNode (String name, String inst,
                                   ConceptualGraph wd,
                                   ConceptualGraph dt)
    {
        Vector v = new Vector();
        if (inst != null)
            v.addElement (inst);
        ConceptNode cn = new ConceptNode (name, v);
        return new ConceptualGraph (cn, wd, dt);
    }

private ConceptualGraph mkRelNode (String name,
                                   ConceptualGraph wd,
                                   ConceptualGraph dt)
    {
        RelationNode rn = new RelationNode (name);
        return new ConceptualGraph (rn, wd, dt);
    }

private ConceptualGraph findNode (ConceptualGraph cg, String name)
    {
        while (cg != null && cg.getNode().getName() != name)
            cg = cg.getDepth();
        return cg;
    }

private String getInst (ConceptualGraph cg)
    {
        return (String)((ConceptNode)cg.getNode()).getInstances().elementAt(0);
    }
    
private ConceptualGraph objToCg (DIIObject o)
    {
        if (o instanceof DIIString) {
            DIIString ds = (DIIString)o;
            return mkRelNode ("BASIC",
                              mkConNode ("STRING", ds.get(),
                                         null,
                                         null),
                              null);
        }
        if (o instanceof DIIBoolean) {
            DIIBoolean db = (DIIBoolean)o;
            return mkRelNode ("BASIC",
                              mkConNode ("BOOL", db.get() ? "TRUE" : "FALSE",
                                         null,
                                         null),
                              null);
        }
        if (o instanceof DIISequence) {
            DIISequence ds = (DIISequence)o;
	    Vector v = ds.get();

            ConceptualGraph cg = null;
	    for (int i = v.size(); --i >= 0; ) {
		Object el = v.elementAt(i);
		cg = mkRelNode ("ELEMENT",
				mkConNode ("VALUE", null,
					   objToCg ((DIIObject)v.elementAt(i)),
					   null),
				cg);
	    }
	    if (v.size() == 0) {
		cg = mkRelNode ("DUMMY",
				mkConNode ("VALUE", null,
					   objToCg (ds.elementType()),
					   null),
				null);
	    }
            return mkRelNode ("COMPLEX",
                              mkConNode ("SEQUENCE", null,
                                         cg,
                                         null),
                              null);
        }
        System.out.println ("objToCg: unsupported type");
        return null;
    }

private void cgToObj (DIIObject o, ConceptualGraph cg)
    {
        if (o instanceof DIIString) {
	    DIIString ds = (DIIString)o;
	    ds.set (getInst (cg.getWidth()));
            return;
        }
	if (o instanceof DIIBoolean) {
	    DIIBoolean db = (DIIBoolean)o;
	    db.set (getInst(cg.getWidth()).equals ("TRUE") ? true : false);
	    return;
	}
        if (o instanceof DIISequence) {
	    DIISequence ds = (DIISequence)o;
            Vector v = (Vector)ds.get();

            v.removeAllElements();

            cg = cg.getWidth().getWidth();
            for ( ; cg != null; cg = cg.getDepth()){
		if (cg.getNode().getName().equals ("ELEMENT")) {
		    DIIObject obj = ds.elementType().diiClone();
		    cgToObj (obj, cg.getWidth().getWidth());
		    v.addElement (obj);
		}
            }
            return;
        }
        System.out.println ("cgToObj: unsupported type");
    }
    
private void cgToNv (ConceptualGraph cg)
    {
        cg = cg.getWidth();
        for (int i = 0; cg != null; cg = cg.getDepth()) {
	    if (cg.getNode().getName().equals ("RESULT")) {
		if (result != null)
		    cgToObj (result.value, cg.getWidth().getWidth());
	    } else {
		NamedValue nv = (NamedValue)args.elementAt(i);
		if (nv.flags != NamedValue.IN)
		    cgToObj (nv.value, cg.getWidth().getWidth());
		++i;
	    }
        }
    }
    
private ConceptualGraph nvToCg ()
    {
        ConceptualGraph cg = null;
        for (int i = args.size(); --i >= 0; ) {
            String dir = "";
            NamedValue nv = (NamedValue)args.elementAt(i);

            switch (nv.flags) {
            case NamedValue.IN:
                dir = "IN_PARAM";
                break;
            case NamedValue.OUT:
                dir = "OUT_PARAM";
                break;
            case NamedValue.INOUT:
                dir = "INOUT_PARAM";
                break;
            }
            cg = mkRelNode (dir,
                            mkConNode ("NAME", "x",
                                       null,
                                       mkConNode ("VALUE", null,
                                                  objToCg (nv.value),
                                                  null)),
                            cg);
        }

        if (result != null) {
            cg = mkRelNode ("RESULT",
                            mkConNode ("VALUE", null,
                                       objToCg (result.value),
                                       null),
                            cg);
        }
        return mkConNode ("OPERATION", operation,
                          cg,
                          null);
    }
    
public void invoke ()
        throws DIIException
    {
        ConceptualGraph cg = nvToCg ();
        cg = GlobalVars.proxy.invoke (cg, key);
        cgToNv (cg);
    }
}
