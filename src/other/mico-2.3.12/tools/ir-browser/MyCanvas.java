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
import CanvasObject;

public class MyCanvas extends Canvas {
private Vector objs;
private Dimension prefsize;
private CanvasHandler handler;

private Image offscreen;
private Dimension offscreensize;

private Rectangle dirty;
private int autoupdate;

public MyCanvas (Dimension prefsize)
    {
        this.prefsize = prefsize;
        this.offscreen = null;
        this.dirty = null;
        this.autoupdate = 0;

        setSize (prefsize);
        objs = new Vector ();
    }

public void Add (CanvasObject obj)
    {
        objs.addElement (obj);
        obj.SetCanvas (this);
        CheckResize ();
        Redraw (getGraphics(), obj.Bbox());
    }

public void Remove (CanvasObject co)
        throws NoSuchElementException
    {
        if (!objs.removeElement (co)) {
            throw new NoSuchElementException ();
        } else {
            co.SetCanvas (null);
            CheckResize ();
            Redraw (getGraphics(), co.Bbox());
        }
    }

public void RemoveAll ()
    {
        objs.removeAllElements();
        CheckResize ();
        repaint();
    }

public void ToTop (CanvasObject co)
        throws NoSuchElementException
    {
        Restack (co, Size());
    }

public void ToBottom (CanvasObject co)
        throws NoSuchElementException
    {
        Restack (co, 0);
    }

public void Restack (CanvasObject co, int idx)
        throws NoSuchElementException
    {
        if (!objs.removeElement (co)) {
            throw new NoSuchElementException ();
        } else {
            if (idx >= objs.size())
                objs.addElement (co);
            else
                objs.insertElementAt (co, idx);
            Redraw (getGraphics(), co.Bbox());
        }
    }

public CanvasObject ObjectAt (int idx)
        throws ArrayIndexOutOfBoundsException
    {
        return (CanvasObject)objs.elementAt (idx);
    }

public int Size ()
    {
        return objs.size();
    }
    
public CanvasObject Find (int x, int y)
        throws NoSuchElementException
    {
        for (int i = objs.size()-1; i >= 0; --i) {
            CanvasObject co = (CanvasObject)objs.elementAt(i);
            if (co.IsAt (x, y))
                return co;
        }
        throw new NoSuchElementException ();
    }

public CanvasObject Find (long objid)
        throws NoSuchElementException
    {
        for (int i = objs.size()-1; i >= 0; --i) {
            CanvasObject co = (CanvasObject)objs.elementAt(i);
            if (co.GetId() == objid)
                return co;
        }
        throw new NoSuchElementException ();
    }

private boolean CheckResizeOffscreen ()
    {
        Dimension d = getSize();
        if (offscreen == null ||
            d.width  != offscreensize.width ||
            d.height != offscreensize.height) {

            Image nos = createImage (d.width, d.height);
            Graphics nog = nos.getGraphics();
            nog.setColor (getBackground());
            nog.fillRect (0, 0, d.width, d.height);
            if (offscreen != null)
                nos.getGraphics().drawImage (offscreen, 0, 0, null);
            offscreen = nos;
            offscreensize = d;
            return true;
        }
        return false;
    }

public void DoRedraw (Graphics og, Rectangle bbox)
    {
        CheckResizeOffscreen ();
        Graphics g = offscreen.getGraphics ();

        g.setColor (getBackground());
        g.fillRect (bbox.x, bbox.y, bbox.width, bbox.height);

        for (int i = 0; i < objs.size(); ++i) {
            CanvasObject co = (CanvasObject)objs.elementAt (i);
            if (bbox.intersects (co.Bbox ()))
                co.Draw(g);
        }
        og.clipRect (bbox.x, bbox.y, bbox.width, bbox.height);
        og.drawImage (offscreen, 0, 0, null);
    }

public void Redraw (Graphics g, Rectangle bbox)
    {
        if (autoupdate >= 0) {
            DoRedraw (g, bbox);
        } else {
            if (dirty == null) {
                dirty = new Rectangle (bbox.x, bbox.y,
                                       bbox.width, bbox.height);
            } else {
                dirty.add (bbox);
            }
        }
    }

public void ObjectChanged (CanvasObject co, Rectangle oldbbox)
    {
        Rectangle newbbox = co.Bbox();
        Rectangle unionbbox = newbbox.union(oldbbox);
        long area1 = unionbbox.width * unionbbox.height;
        long area2 = oldbbox.width * oldbbox.height +
            newbbox.width * newbbox.height;

        if (area1 > 4 * area2) {
            Redraw (getGraphics(), oldbbox);
            Redraw (getGraphics(), newbbox);
        } else {
            Redraw (getGraphics(), unionbbox);
        }
    }

public Rectangle Bbox ()
    {
        if (objs.size() == 0)
            return new Rectangle (0, 0, 0, 0);

        Rectangle bbox = ((CanvasObject)objs.elementAt(0)).CloneBbox();
        for (int i = objs.size()-1; i >= 1; --i) {
            bbox.add (((CanvasObject)objs.elementAt(i)).Bbox());
        }
        return bbox;
    }

public void CheckResize ()
    {
	// XXX does not work if Bbox.x < 0 or Bbox.y < 0 ...

        Rectangle b = Bbox();
        Dimension newd, d = getSize();

        newd = new Dimension (d.width, d.height);

        if (b.x + b.width  > d.width  || 2*(b.x + b.width)  < d.width)
	  newd.width = Math.max (b.x + b.width + 50, prefsize.width);
        if (b.y + b.height > d.height || 2*(b.y + b.height) < d.height)
	  newd.height = Math.max (b.y + b.height + 50, prefsize.height);

        if (newd.width != d.width || newd.height != d.height)
            setSize (newd);
    }

public void AutoUpdateOn ()
    {
        if (++autoupdate == 0) {
            if (dirty != null) {
                DoRedraw (getGraphics(), dirty);
                dirty = null;
            }
        }
    }

public void AutoUpdateOff ()
    {
        --autoupdate;
    }

public void update (Graphics g)
    {
        if (CheckResizeOffscreen ())
            DoRedraw (g, new Rectangle (0, 0, getSize().width,
					getSize().height));
        else {
            g.clipRect (0, 0, getSize().width, getSize().height);
            g.drawImage (offscreen, 0, 0, null);
        }
    }

public void paint (Graphics g)
    {
        update (g);
    }

public Dimension getPreferredSize ()
    {
        return prefsize;
    }

public Dimension getMinimumSize ()
    {
        return prefsize;
    }
}
