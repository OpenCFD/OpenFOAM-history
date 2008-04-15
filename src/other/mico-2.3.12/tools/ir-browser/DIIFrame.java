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
import java.awt.*;
import java.awt.event.*;

import GlobalVars;
import CGEditor;
import IRBrowser;
import Proxy;
import CGTemplates;
import EntryBox;
import StatusLine;

class DIIFrame extends Frame implements EntryBoxHandler, ActionListener {
private EditLinearForm editlinform = null;
private CGEditor cgedit;
protected StatusLine status;
private String defrepoid;
private String thekey;
private ConceptualGraph defcg;
private boolean isexited = false;
private MenuItem mnArrGraph, mnLinForm, mnExit, mnNewGraph, mnInvoke,
    mnBind, mnRelease;

public DIIFrame (String repoid, ConceptualGraph template)
        throws DIIException
    {
	GlobalVars.openConn ();

        defrepoid = repoid;
	defcg = template;

        setTitle ("MICO Dynamic Method Invoker");

        MenuBar mbar = new MenuBar ();
        Menu editmenu = new Menu ("Edit");        
        editmenu.add (mnNewGraph = new MenuItem ("New graph"));        
        editmenu.add (mnArrGraph = new MenuItem ("Arrange graph"));
        editmenu.add (mnLinForm = new MenuItem ("Linear form of graph ..."));
        editmenu.addSeparator ();
        editmenu.add (mnExit = new MenuItem ("Exit"));

	mnNewGraph.addActionListener( this );
	mnArrGraph.addActionListener( this );
	mnLinForm.addActionListener( this );
	mnExit.addActionListener( this );

        Menu servmenu = new Menu ("Server");
        servmenu.add (mnBind = new MenuItem ("Bind ..."));
        servmenu.add (mnInvoke = new MenuItem ("Invoke"));
        servmenu.add (mnRelease = new MenuItem ("Release"));
	mnInvoke.setEnabled( false );
	mnRelease.setEnabled( false );

	mnBind.addActionListener( this );
	mnInvoke.addActionListener( this );
	mnRelease.addActionListener( this );

        mbar.add (editmenu);
        mbar.add (servmenu);
        setMenuBar (mbar);

	setLayout (new GridBagLayout());

	status = new StatusLine();
        cgedit = new CGEditor (new Dimension (700, 700), true, this);
        ScrollPane sp = new ScrollPane();
	sp.setSize( 600, 400 );
	sp.add( cgedit );

	GlobalVars.constrain (this, sp, 0, 0, 1, 1,
			      GridBagConstraints.BOTH, 1.0, 1.0);
	GlobalVars.constrain (this, status, 0, 1, 1, 1,
			      GridBagConstraints.BOTH, 1.0, 0.0);
    }
    
public void init ()
        throws DIIException
    {
        CGTemplates.SetTemplates (cgedit);
	cgedit.SetCG (defcg);
    }

public void actionPerformed (ActionEvent ev)
    {
        if (ev.getSource() == mnLinForm) {
	  editlinform = new EditLinearForm (cgedit);
	  editlinform.pack();
	  editlinform.show();
        } else if (ev.getSource() == mnArrGraph) {
            cgedit.Layout (null);
        } else if (ev.getSource() == mnExit) {
	    exit ();
        } else if (ev.getSource() == mnNewGraph) {
            cgedit.SetCG (null);
        } else if (ev.getSource() == mnBind) {
	    EntryBox e = new EntryBox ("DII - Bind", "repository id:",
				       defrepoid, this, null, this);
	    e.pack ();
	    e.show ();
        } else if (ev.getSource() == mnInvoke) {
	    Invoke();
        } else if (ev.getSource() == mnRelease) {
	    Release();
        }
    }

public void setCG (ConceptualGraph cg)
    {
	cgedit.SetCG (cg);
    }

public void entryBoxHandler (EntryBox e, Object arg)
    {
	if (e.GetButton() == 0) {
	    Bind (e.GetText());
	}
    }

public void Bind (String repoid)
    {
	try {
	    thekey = GlobalVars.proxy.bind (repoid);
	    status.setStatus ("Bind succeeded", 3000);
	    mnBind.setEnabled( false );
	    mnInvoke.setEnabled( true );
	    mnRelease.setEnabled( true );
	} catch (DIIException ex) {
	    GlobalVars.DisplayError (ex.getErrMsg());
	}
    }

private void Release ()
    {
        if (thekey != null) {
	    try {
		GlobalVars.proxy.release (thekey);
		status.setStatus ("Release succeeded", 3000);
	    } catch (DIIException e) {
		GlobalVars.DisplayError (e.getErrMsg());
	    }
	    thekey = null;
	    mnRelease.setEnabled( false );
	    mnInvoke.setEnabled( false );
	    mnBind.setEnabled( true );
	}
    }

private void Invoke ()
    {
        ConceptualGraph cg = cgedit.GetCG ();
        if (cg == null) {
            GlobalVars.DisplayError ("Enter a graph first");
	    return;
        }
        try {
	    ConceptualGraph rescg = GlobalVars.proxy.invoke (cg, thekey);
	    cgedit.SetCG (rescg);
	    status.setStatus ("Invocation succeeded", 3000);
        } catch (DIIException ex) {
            GlobalVars.DisplayError (ex.getErrMsg());
            return;
        }
    }

public void exit ()
    {
	// dispose() kills netscape.
	Release ();
	if (editlinform != null) {
	    editlinform.setVisible ( false );
	    editlinform = null;
	}
	setVisible( false );
	isexited = true;
    }

public boolean exited ()
    {
	return isexited;
    }
}
