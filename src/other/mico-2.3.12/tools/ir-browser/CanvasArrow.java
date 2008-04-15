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
import CanvasLine;

public class CanvasArrow extends CanvasLine {
private boolean drawarrow;
private Point a1, a2, a3;

private final static int alen = 6;
private final static double sin_angle = Math.sin (3.1415/7);
private final static double cos_angle = Math.cos (3.1415/7);

protected void CalcSize ()
    {
	if (a1 == null)
	    // our constructor is not finished yet
	    return;

	super.CalcSize ();

	double len = Math.sqrt ((head.x - tail.x) * (head.x - tail.x) +
				(head.y - tail.y) * (head.y - tail.y));
	if (len < 2*alen) {
	    drawarrow = false;
	} else {
	    drawarrow = true;

	    a2.x = (head.x + tail.x + 1) / 2;
	    a2.y = (head.y + tail.y + 1) / 2;

	    double x = (tail.x - head.x) * alen / len;
	    double y = (tail.y - head.y) * alen / len;
	    a1.x = (int)(  x * cos_angle - y * sin_angle + a2.x + 0.5);
	    a1.y = (int)(  x * sin_angle + y * cos_angle + a2.y + 0.5);
	    a3.x = (int)(  x * cos_angle + y * sin_angle + a2.x + 0.5);
	    a3.y = (int)(- x * sin_angle + y * cos_angle + a2.y + 0.5);

	    // make bbox big enough to include arrow
	    pos.add (a1);
	    pos.add (a3);
            pos.setSize (pos.width+1, pos.height+1);
	}
    }

public CanvasArrow (int x0, int y0, int x1, int y1, Color col)
    {
	super (x0, y0, x1, y1, col);
	drawarrow = false;
	a1 = new Point (0, 0);
	a2 = new Point (0, 0);
	a3 = new Point (0, 0);
	CalcSize ();
    }

public void Draw (Graphics g)
    {
	g.setColor (col);
	g.drawLine (tail.x, tail.y, head.x, head.y);
	if (drawarrow) {
	    g.drawLine (a1.x, a1.y, a2.x, a2.y);
	    g.drawLine (a3.x, a3.y, a2.x, a2.y);
	}
    }
}
