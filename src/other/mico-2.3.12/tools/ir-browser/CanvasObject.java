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
import MyCanvas;

abstract public class CanvasObject extends Object {
private static long theId = 0;
protected long myid;
protected MyCanvas canvas;

public CanvasObject ()
    {
        myid = theId++;
    }

public long GetId ()
    {
        return myid;
    }

public void SetCanvas (MyCanvas c)
    {
        canvas = c;
    }

public void ObjectChanged (Rectangle oldbbox)
    {
        if (canvas != null)
            canvas.ObjectChanged (this, oldbbox);
    }

public void AutoUpdateOn ()
    {
        if (canvas != null)
            canvas.AutoUpdateOn ();
    }

public void AutoUpdateOff ()
    {
        if (canvas != null)
            canvas.AutoUpdateOff ();
    }

public Rectangle CloneBbox ()
    {
        Rectangle b = Bbox();
        return new Rectangle (b.x, b.y, b.width, b.height);
    }

    abstract public void Move (int xinc, int yinc);
    abstract public void MoveTo (int x, int y);
    abstract public boolean IsAt (int x, int y);
    abstract public void Draw (Graphics g);
    abstract public Rectangle Bbox ();
    abstract public Point GetPos ();
}
