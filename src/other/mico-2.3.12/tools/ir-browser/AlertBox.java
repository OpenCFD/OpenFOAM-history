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
import BorderPanel;

class AlertBox extends Dialog implements ActionListener {
private Button ok;
private int button;

public AlertBox (String title, String txt)
    {
        this (title, txt, GlobalVars.frame);
    }

public AlertBox (String title, String txt, Frame parent)
    {
        super (parent, title, true);

        button = 0;

        Panel p1 = new BorderPanel (BorderPanel.RIDGE);
        Panel p2 = new Panel();

        Label img = new Label ("!");
        img.setFont (new Font ("Halvetica", Font.PLAIN, 24));

        Label text = new Label (txt);
        ok = new Button ("Ok");

	ok.addActionListener( this );

        add ("North", p1);
        add ("South", p2);
        p1.add ("West", img);
        p1.add ("East", text);
        p2.add ("Center", ok);
    }

public int GetButton ()
    {
        return button;
    }
    
public void actionPerformed (ActionEvent e)
    {
        if (e.getSource() == ok) {
            button = 0;
            dispose ();
        }
    }
}
