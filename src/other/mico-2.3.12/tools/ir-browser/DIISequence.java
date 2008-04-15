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

public class DIISequence implements DIIObject {
private Vector val;
private DIIObject eltype;

public DIISequence (Vector v, DIIObject e)
    {
	val = v;
	eltype = e;
    }
    
public DIISequence (DIIObject e)
    {
	val = new Vector();
	eltype = e;
    }

public void set (Vector v)
    {
	val = v;
    }

public Vector get ()
    {
	return val;
    }

public DIIObject diiClone ()
    {
	return new DIISequence ((Vector)val.clone(), eltype);
    }

public Object toJava ()
    {
	Vector v = new Vector();
	for (int i = 0; i < val.size(); ++i)
	    v.addElement (((DIIObject)val.elementAt(i)).toJava());
	return v;
    }

public DIIObject elementType ()
    {
	return eltype;
    }
}
