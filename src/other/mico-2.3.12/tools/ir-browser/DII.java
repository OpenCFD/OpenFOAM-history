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
import java.applet.Applet;

import GlobalVars;
import CGEditor;
import IRBrowser;
import Proxy;
import CGTemplates;
import EntryBox;
import DIIFrame;
import IRFrame;

public class DII extends Applet implements Runnable, ActionListener {
private volatile boolean pleaseCreate;
private Thread windowThread;
private DIIFrame frame;
private Label label;
private Button button;

public void init ()
    {
        String version = System.getProperty ("java.version");

        GlobalVars.applet = this;
        GlobalVars.serverhost = getCodeBase().getHost();
        pleaseCreate = false;
        windowThread = null;
        frame = null;

        setLayout(new GridLayout(2,0));

        add (button = new Button ("Start IR browser"));
        add (label = new Label ("", Label.CENTER));

	button.addActionListener( this );

        validate ();
    }

public void start ()
    {
        if (windowThread == null) {
            windowThread = new Thread (this, "window thread");
            windowThread.start ();
        }
    }

public void stop ()
    {
	/*
	 * XXX the DCE window manager causes stop() to be called when the
	 *     user leaves the virtual root where the applet runs...
        if (windowThread != null) {
            label.setText ("applet stopped");
            windowThread.stop ();
            windowThread = null;
            GlobalVars.CloseAll ();
        }
	*/
    }

public synchronized void run ()
    {
        while (windowThread != null) {
            while (pleaseCreate == false) {
                try {
                    wait ();
                } catch (InterruptedException e) {}
            }
            try {
                button.setEnabled( false );

		IRFrame irframe = new IRFrame ();
		irframe.pack();
		irframe.init();
		irframe.show();

                label.setText ("");

                while (pleaseCreate == true) {
                    try {
                        wait ();
                    } catch (InterruptedException e) {}
                }
            } catch (DIIException e) {
                label.setText (e.getErrMsg());
                pleaseCreate = false;
            }
            button.setEnabled( true );
        }
    }

public synchronized void actionPerformed( ActionEvent ev )
    {
        if (ev.getSource() == button) {
            pleaseCreate = true;
            label.setText ("loading applet, please wait ...");
            notify ();
        }
    }

public synchronized void FrameClosed ()
    {
        pleaseCreate = false;
        notify ();
    }
}
