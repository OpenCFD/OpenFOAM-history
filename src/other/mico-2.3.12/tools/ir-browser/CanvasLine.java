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

public class CanvasLine extends CanvasObject {
protected Rectangle pos;
protected Point head, tail;
protected Color col;

protected void CalcSize ()
    {
        pos.setBounds (head.x, head.y, 1, 1);
        pos.add (tail);
        pos.setSize (pos.width+1, pos.height+1);
    }

public CanvasLine (int x0, int y0, int x1, int y1, Color col)
    {
        pos = new Rectangle ();
        this.col = col;
        head = new Point (x1, y1);
        tail = new Point (x0, y0);
        CalcSize ();
    }

public void Move (int xinc, int yinc)
    {
        Rectangle obbox = CloneBbox();
        head.translate (xinc, yinc);
        tail.translate (xinc, yinc);
        CalcSize ();
        ObjectChanged (obbox);
    }

public void MoveTo (int x, int y)
    {
        Rectangle obbox = CloneBbox();
        head.translate (x - tail.x, y - tail.y);
        tail.move (x, y);
        CalcSize ();
        ObjectChanged (obbox);
    }

public void MoveHead (int xinc, int yinc)
    {
        Rectangle obbox = CloneBbox();
        head.translate (xinc, yinc);
        CalcSize();
        ObjectChanged (obbox);
    }

public void MoveHeadTo (int x, int y)
    {
        Rectangle obbox = CloneBbox();
        head.move (x, y);
        CalcSize();
        ObjectChanged (obbox);
    }

public void MoveTail (int xinc, int yinc)
    {
        Rectangle obbox = CloneBbox();
        tail.translate (xinc, yinc);
        CalcSize();
        ObjectChanged (obbox);
    }

public void MoveTailTo (int x, int y)
    {
        Rectangle obbox = CloneBbox();
        tail.move (x, y);
        CalcSize();
        ObjectChanged (obbox);
    }

public Point GetHead ()
    {
        return head;
    }

public Point GetTail ()
    {
        return tail;
    }

public Point GetPos ()
    {
        return tail;
    }
    
public boolean IsAt (int x, int y)
    {
        if (tail.x == head.y && tail.y == head.y)
            return (tail.x == x && tail.y == y);
        if (!pos.contains (x, y))
            return false;

        // |head-tail|
        float arrowlen = ((head.x - tail.x) * (head.x - tail.x) +
                          (head.y - tail.y) * (head.y - tail.y));

        // |((x,y),tail)|
        float a = ((x - tail.x) * (x - tail.x) +
                   (y - tail.y) * (y - tail.y));

        // (((x,y)-tail) * (head-tail)) ^ 2
        // ---------------------------------
        //           |(head-tail)|
        float b = ((x - tail.x) * (head.x - tail.x) +
                   (y - tail.y) * (head.y - tail.y));
        b = b * b / arrowlen;

        return (a - b <= 1);
    }

public Rectangle Bbox ()
    {
        return pos;
    }

public void Draw (Graphics g)
    {
        g.setColor (col);
        g.drawLine (tail.x, tail.y, head.x, head.y);
    }
}
