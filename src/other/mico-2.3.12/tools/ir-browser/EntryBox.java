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
import java.awt.event.*;

import GlobalVars;
import EntryBoxHandler;

class EntryBox extends Dialog implements ActionListener {
private TextField entry;
private Button ok, cancel;
private int button;
private EntryBoxHandler callback;
private Object callbackArg;

public EntryBox (String title, String caption, String txt)
    {
        this (title, caption, txt, null, null, GlobalVars.frame);
    }

public EntryBox (String title, String caption, String txt,
		 EntryBoxHandler cb, Object cbArg)
    {
        this (title, caption, txt, cb, cbArg, GlobalVars.frame);
    }

public EntryBox (String title, String caption, String txt,
		 EntryBoxHandler cb, Object cbArg,
		 Frame parent)
    {
        super (parent, title, true);

        button = 0;
	callback = cb;
	callbackArg = cbArg;

        Panel p1 = new Panel();
        Panel p2 = new Panel();

        add ("North", p1);
        add ("South", p2);

        p1.add ("West", new Label (caption));
        entry = new TextField (txt, 20);
        entry.selectAll ();
        p1.add ("West", entry);

        ok = new Button ("Ok");
        cancel = new Button ("Cancel");
        p2.add ("West", ok);
        p2.add ("West", cancel);

	ok.addActionListener( this );
	cancel.addActionListener( this );
	entry.addActionListener( this );

        Dimension d = getLayout().minimumLayoutSize(this);
        setSize (d.width, d.height);
    }

public String GetText ()
    {
        return entry.getText ();
    }

public int GetButton ()
    {
        return button;
    }
    
public void actionPerformed( ActionEvent e )
    {
        switch( e.getID() ) {
        case Event.ACTION_EVENT:
            if (e.getSource() == ok || e.getSource() == cancel ||
		   e.getSource() == entry ) {
                button = (e.getSource() == cancel) ? 1 : 0;
                dispose ();
		if (callback != null)
		    callback.entryBoxHandler (this, callbackArg);
            }
        }
    }
}
