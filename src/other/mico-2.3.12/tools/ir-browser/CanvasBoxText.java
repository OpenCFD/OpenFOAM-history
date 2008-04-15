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
import CanvasObject;

public class CanvasBoxText extends CanvasObject {
private Rectangle pos;
private Color bgcol, fgcol, tcol;
private String text;
private FontMetrics fm;

private void CalcSize ()
    {
        pos.setSize (fm.stringWidth (text) + 6, fm.getHeight () + 6);
    }

public CanvasBoxText (int x0, int y0,
                      String text,
                      FontMetrics fm,
                      Color fgcol, Color bgcol, Color tcol)
    {
        this.fm = fm;
        this.text = text;
        this.fgcol = fgcol;
        this.bgcol = bgcol;
        this.tcol = tcol;
        pos = new Rectangle (x0, y0, 0, 0);
        CalcSize ();
    }

public void SetText (String ntext)
    {
        Rectangle obbox = CloneBbox();
        text = ntext;
        CalcSize ();
        ObjectChanged (obbox);
    }

public String GetText ()
    {
        return text;
    }

public void Move (int xinc, int yinc)
    {
        Rectangle obbox = CloneBbox();
        pos.translate (xinc, yinc);
        ObjectChanged (obbox);
    }

public void MoveTo (int x, int y)
    {
        Rectangle obbox = CloneBbox();
        pos.setLocation (x, y);
        ObjectChanged (obbox);
    }

public boolean IsAt (int x, int y)
    {
        return pos.contains (x, y);
    }

public Rectangle Bbox ()
    {
        return pos;
    }

public Point GetPos ()
    {
        return new Point (pos.x, pos.y);
    }

public void Draw (Graphics g)
    {
        g.setColor (bgcol);
        g.fillRect (pos.x, pos.y, pos.width, pos.height);
        g.setColor (fgcol);
        g.drawRect (pos.x, pos.y, pos.width-1, pos.height-1);
        g.setColor (tcol);
        g.setFont (fm.getFont());
        g.drawString (text, pos.x+2, pos.y+3+fm.getAscent ());
    }
}
