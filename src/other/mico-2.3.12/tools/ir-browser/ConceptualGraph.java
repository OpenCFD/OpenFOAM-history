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
import RelationNode;
import ConceptNode;


public class ConceptualGraph {
	private Node node;
	private ConceptualGraph width;
	private ConceptualGraph depth;
	private int tag;

	public ConceptualGraph( Node aNode, ConceptualGraph theWidth,
                                ConceptualGraph theDepth )
	{
		node = aNode;
		width = theWidth;
		depth = theDepth;
	}

	public void setNode( Node aNode )
	{
		node = aNode;
	}

	public Node getNode()
	{
		return node;
	}

	public void setWidth( ConceptualGraph theWidth )
	{
		width = theWidth;
	}

	public void setDepth( ConceptualGraph theDepth )
	{
		depth = theDepth;
	}

	public ConceptualGraph getWidth()
	{
		return width;
	}

	public ConceptualGraph getDepth()
	{
		return depth;
	}

	public void print( StringBuffer ostr, int incr )
	{
		prettyPrint( ostr, 0, incr );
	}

	public void print( StringBuffer ostr )
	{
		prettyPrint( ostr, -1, -1 );
	}

	private void prettyPrint( StringBuffer ostr, int indent, int incr )
	{
		node.print( ostr );
		if( width != null ) {
                    if( width.getDepth() != null ) {
                        ostr.append( "-" );
                        indent = printSpaces( ostr, indent, incr );
                    }
                    ostr.append( "->" );
                    width.prettyPrint( ostr, indent, incr );
                }
		int new_incr = 0;
                if( width != null && width.getDepth() != null ) {
                    ostr.append( "." );
                    new_incr = -incr;
		}
                if( depth != null ) {
                    ostr.append( "," );
                    indent = printSpaces( ostr, indent, new_incr );
                    ostr.append( "->" );
                    depth.prettyPrint( ostr, indent, incr );
                }
	}

	private int printSpaces( StringBuffer ostr, int indent, int incr )
	{
		if( indent == -1 )
			return indent;
		indent += incr;
		int num = indent;
		ostr.append( "\n" );
		while( num-- != 0 )
			ostr.append( " " );
		return indent;
	}

	public boolean equals (Object o)
	{
            if (o == null || !(o instanceof ConceptualGraph))
                return false;

            ConceptualGraph cg1, cg2;
            cg2 = (ConceptualGraph)o;
            if (!node.equals (cg2.node))
                return false;

            cg1 = getWidth();
            cg2 = cg2.getWidth();
            while (cg1 != null && cg2 != null) {
                if (!cg1.equals (cg2))
                    return false;
                cg1 = cg1.getDepth();
                cg2 = cg2.getDepth();
            }
            return (cg1 == null && cg2 == null);
        }
}




