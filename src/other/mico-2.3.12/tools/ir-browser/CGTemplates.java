// -*- Java -*-
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

import java.util.*;
import java.lang.*;

import CGEditor;
import CGParser;
import DIIException;

public class CGTemplates {
private static final String templates = "[templates]-	->(menu)->[values:VALUE]-		->(entry)->[long]->(BASIC)->[LONG:0],		->(entry)->['unsigned long']->(BASIC)->[ULONG:0],		->(entry)->[short]->(BASIC)->[SHORT:0],		->(entry)->['unsigned short']->(BASIC)->[USHORT:0],		->(entry)->['boolean']->(BASIC)->[BOOL:TRUE],		->(entry)->['string']->(BASIC)->[STRING:'foobar'],		->(entry)->['objref']->(BASIC)->[OBJREF:'$0'],		->(entry)->[struct]->(COMPLEX)->[STRUCT],		->(entry)->[sequence]->(COMPLEX)->[SEQUENCE]		.,	->(menu)->[parameters:OPERATION]-		->(entry)->['input parameter']->(IN_PARAM)-			->[VALUE],			->[NAME:x]			.,		->(entry)->['output parameter']->(OUT_PARAM)-			->[VALUE],			->[NAME:x]			.,		->(entry)->['inout parameter']->(INOUT_PARAM)-			->[VALUE],			->[NAME:x]			.,		->(entry)->['result']->(RESULT)-			->[VALUE]			.		.,	->(menu)->[elements:SEQUENCE]-		->(entry)->['element']->(ELEMENT)->[VALUE],		->(entry)->['dummy']->(DUMMY)->[VALUE]		.,	->(menu)->[members:STRUCT]-		->(entry)->['member']->(MEMBER)-			->[VALUE],			->[NAME:x]			.		.	.";

public static void SetTemplates (CGEditor cgedit)
        throws DIIException
    {
        ConceptualGraph cg, mcg, ecg;
        CGParser p = new CGParser();

        try {
            cg = p.Parse (templates);
	    // loop over all menus
	    for (mcg = cg.getWidth(); mcg != null; mcg = mcg.getDepth()) {
		ConceptualGraph mnamecg = mcg.getWidth();
		ConceptNode mnamenode = (ConceptNode)mnamecg.getNode();

		// loop over all entries for current menu
		ecg = mnamecg.getWidth();
		for ( ; ecg != null; ecg = ecg.getDepth()) {
		    ConceptualGraph enamecg = ecg.getWidth();
		    ConceptNode enamenode = (ConceptNode)enamecg.getNode();

		    cgedit.AddTemplate (mnamenode.getName(), // menu name
					mnamenode.getInstances(), // after
					enamenode.getName(), // entry name
					enamecg.getWidth()); // template
		}
	    }
        } catch (Exception e) {
            throw new DIIException (1, "cannot parse templates");
        }
    }
}
