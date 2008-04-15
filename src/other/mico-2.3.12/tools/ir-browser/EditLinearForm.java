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

import java.awt.*;
import java.awt.event.*;
import java.util.*;

import AlertBox;
import CGParser;
import GlobalVars;
import CGEditor;
import View;


public class EditLinearForm extends Dialog implements View, ActionListener {
private Button done, commit;
private TextArea text;
private CGEditor cgedit;
private boolean noupdates;

public EditLinearForm (CGEditor edit)
    {
        super (GlobalVars.frame, "Edit Linear Form", false);

        setLayout (new GridBagLayout());

        noupdates = false;
        cgedit = edit;
        Panel p1 = new Panel();

        done = new Button ("Done");
        commit = new Button ("Commit");
        text = new TextArea (20, 60);
        text.setFont (new Font ("fixed", Font.PLAIN, 14));
        text.setEditable( true );
	
        p1.add ("West", done);
        p1.add ("West", commit);

	done.addActionListener( this );
	commit.addActionListener( this );

        GlobalVars.constrain (this, text, 0, 0, 1, 1,
                              GridBagConstraints.BOTH, 1.0, 1.0);
        GlobalVars.constrain (this, p1, 0, 1, 1, 1,
                              GridBagConstraints.BOTH, 1.0, 0.0);

        Update();
        cgedit.AddView (this);
    }

private void Update ()
    {
        ConceptualGraph cg = cgedit.GetCG ();
        if (cg != null) {
            StringBuffer ostr = new StringBuffer ();
            cg.print (ostr, 2);
            text.setText (ostr.toString());
        }
    }

private void Commit ()
    {
        CGParser p = new CGParser ();

        noupdates = true;
        try {
            cgedit.SetCG (p.Parse (text.getText()));
        } catch (Exception e) {
            AlertBox a = new AlertBox ("Parse Error", "Cannot parse graph");
            a.pack();
            a.setVisible( true );
        }
        noupdates = false;
    }

public void actionPerformed (ActionEvent ev)
    {
        if (ev.getSource() == done) {
            cgedit.DelView (this);
            dispose();
            GlobalVars.editlinform = null;
        } else if (ev.getSource() == commit)
            Commit();
    }

public void UpdateView (Object viewedObject)
    {
        if (!noupdates)
            Update();
    }
}
