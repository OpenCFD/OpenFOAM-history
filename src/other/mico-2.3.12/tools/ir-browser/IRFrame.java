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

class IRFrame extends Frame implements ActionListener {
private IRBrowser irbrowse;
protected StatusLine status;
protected Label path;
protected TextArea description;
private MenuItem mnExit, mnArrange, mnReload;

public IRFrame ()
        throws DIIException
    {
	GlobalVars.openConn ();

	GlobalVars.frame = this;

	setTitle ("MICO IR Browser");

	MenuBar mbar = new MenuBar ();
	Menu editmenu = new Menu ("Edit");        
	editmenu.add (mnArrange = new MenuItem ("Arrange Icons"));
	editmenu.add (mnReload = new MenuItem ("Reload"));
	editmenu.addSeparator ();
	editmenu.add (mnExit = new MenuItem ("Exit"));

	mnArrange.addActionListener( this );
	mnReload.addActionListener( this );
	mnExit.addActionListener( this );

	mbar.add (editmenu);
	setMenuBar (mbar);
	
	setLayout( new BorderLayout() );

	Dimension size = Toolkit.getDefaultToolkit().getScreenSize();
	size.width = size.width / 3;
	size.height = size.height / 3;
	this.setSize( size );

	Panel p = new Panel();
	p.setLayout (new GridBagLayout());

	status = new StatusLine ();
	path = new Label ("");
	irbrowse = new IRBrowser (new Dimension (700, 700), true, this);
	ScrollPane sp = new ScrollPane();
	sp.setSize( 600, 400 );
	sp.add( irbrowse );

	GlobalVars.constrain (p, path, 0, 0, 1, 1,
			      GridBagConstraints.BOTH, 1.0, 0.0);
	GlobalVars.constrain (p, sp, 0, 1, 1, 1,
			      GridBagConstraints.BOTH, 1.0, 1.0);
	GlobalVars.constrain (p, status, 0, 2, 1, 1,
			      GridBagConstraints.BOTH, 1.0, 0.0);

	description = new TextArea();
	description.setFont( new Font( "fixed", Font.PLAIN, 14) );
	description.setEditable( false );
	add( "West", description );
	add( "Center", p );
    }

public void init ()
        throws DIIException
    {
	irbrowse.reload ();
    }

public void actionPerformed( ActionEvent ev )
    {
        if (ev.getSource() == mnArrange) {
	    irbrowse.Layout();
	} else if (ev.getSource() == mnReload) {
	    irbrowse.reload();
        } else if (ev.getSource() == mnExit) {
	    // dispose() kills netscape.
	    irbrowse.close ();
	    irbrowse = null;
	    setVisible( false );
	    GlobalVars.applet.FrameClosed();
	    GlobalVars.closeConn ();
        };
    }
}
