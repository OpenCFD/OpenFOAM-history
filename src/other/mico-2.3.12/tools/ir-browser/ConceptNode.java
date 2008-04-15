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
import Node;


public class ConceptNode extends Node {
	private Vector inst;
	private long value;
        private boolean has_value;

	public ConceptNode( String aName, Vector theInst )
	{
		name = aName;
		inst = theInst;
		value = 0; //!!!
		has_value = false;
	}

	public ConceptNode( ConceptNode node )
	{
		name = node.name;
		inst = node.inst;
		value = node.value;
	}

	public Vector getInstances()
	{
		return inst;
	}

	public void addInstances( Vector newInst )
	{
            for( Enumeration e = newInst.elements(); e.hasMoreElements(); ) {
                inst.addElement( e.nextElement() );
            }
	}

	public void setValue( long aValue )
	{
	    value = aValue;
	    has_value = true;
	}

	public long getValue()
	{
	    return value;
	}

        public boolean hasValue()
        {
	    return has_value;
	}

	private String quote (String s)
	{
	    if (s.length() == 0)
		return "''";
	    int i;
	    for (i = 0; i < s.length(); ++i) {
		char c = s.charAt(i);
		if (!Character.isLetter (c) &&
		    !Character.isDigit (c) &&
		    c != '_')
		    break;
	    }
	    if (i == s.length())
		return s;
	    return "'" + s + "'";
	}

	public void print( StringBuffer ostr )
	{
            ostr.append( "[" + quote (name) );
            if( inst.size() != 0 ) {
                ostr.append( ":" );
                boolean first = true;
                for( Enumeration e = inst.elements(); e.hasMoreElements();) {
                    if( first )
                        first = false;
                    else
                        ostr.append( ", " );
	            ostr.append( quote ((String)e.nextElement()) );
                }
            } else {
                if( has_value ) {
                    ostr.append( ":" );
                    ostr.append( value );
                }
            }
            ostr.append( "]" );				
	}

	public boolean equals (Object o)
	{
            if (o == null || !(o instanceof ConceptNode) || !super.equals (o))
                return false;
            ConceptNode cn = (ConceptNode)o;

            if (has_value != cn.has_value)
                return false;
            if (has_value)
                return (value == cn.value);
            if (inst.size() != cn.inst.size())
                return false;
            for (int i = 0; i < inst.size(); ++i) {
                if (!inst.elementAt(i).equals (cn.inst.elementAt(i)))
                    return false;
            }
            return true;
        }
}



