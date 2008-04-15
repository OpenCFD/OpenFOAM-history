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
import java.io.*;
import java.awt.*;
import java.awt.image.*;
import CanvasObject;

public class CanvasIcon extends CanvasObject implements ImageObserver {
private Rectangle pos;
private Color tcol, bgcol;
private String text;
private FontMetrics fm;
private Image img;
private Component canvas;

private void CalcSize ()
    {
	int iwd = 0;
	int iht = 0;
	if (img != null) {
	    iwd = img.getWidth (this);
	    iht = img.getHeight (this);
	}
	int twd = fm.stringWidth (text) + 4;
	int tht = fm.getHeight () + 4;
        pos.setSize (Math.max (iwd, twd), iht+tht+2);
    }

public CanvasIcon (int x0, int y0,
		   Image img,
		   Component canvas,
		   String text,
		   FontMetrics fm,
		   Color bgcol, Color tcol)
	throws IOException
    {
	this.img = img;
        this.fm = fm;
        this.text = text;
        this.bgcol = bgcol;
        this.tcol = tcol;
	this.canvas = canvas;

	LoadImage ();

        pos = new Rectangle (x0, y0, 0, 0);
        CalcSize ();
    }

public void toggle_selection()
  {
    Color tmp;

    tmp = tcol;
    tcol = bgcol;
    bgcol = tmp;
    Rectangle obbox = CloneBbox();
    CalcSize ();
    ObjectChanged (obbox);
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

private void LoadImage ()
	throws IOException
    {
	if (img != null) {
	    MediaTracker tracker = new MediaTracker (canvas);
	    tracker.addImage (img, 0);
	    do {
		try {
		    tracker.waitForAll ();
		} catch (InterruptedException ex) {
		}
	    } while (tracker.statusAll (false) == MediaTracker.LOADING);

	    if (tracker.statusAll (false) != MediaTracker.COMPLETE)
		throw new IOException();
	}
    }
    
public void SetImage (Image nimg)
	throws IOException
    {
        Rectangle obbox = CloneBbox();
	img = nimg;
	LoadImage ();
        CalcSize ();
        ObjectChanged (obbox);
    }
    
public Image getImage ()
    {
	return img;
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
	int iwd = 0;
	int iht = 0;
	if (img != null) {
	    iwd = img.getWidth (this);
	    iht = img.getHeight (this);
	}
	int twd = fm.stringWidth (text) + 4;
	int tht = fm.getHeight () + 4;

	g.drawImage (img, pos.x + (pos.width - iwd)/2, pos.y, this);
        g.setColor (bgcol);
        g.fillRect (pos.x + (pos.width - twd)/2, pos.y + iht + 1, twd, tht);
        g.setColor (tcol);
        g.setFont (fm.getFont());
        g.drawString (text,
		      pos.x + (pos.width - twd)/2 + 2,
		      pos.y + iht + 1 + fm.getAscent () + 2);
    }

public boolean imageUpdate (Image img, int flags, int x, int y, int wd, int ht)
    {
	return true;
    }
}
