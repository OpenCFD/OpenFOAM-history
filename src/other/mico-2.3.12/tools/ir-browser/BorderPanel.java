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

public class BorderPanel extends Panel {
private final static Insets insets = new Insets (4, 4, 4, 4);
private static final Color lightGray = new Color (230, 230, 230);
private static final Color darkGray = new Color (100, 100, 100);
public static final int RAISED = 1;
public static final int SUNKEN = 2;
public static final int RIDGE = 3;
private int type;    

public BorderPanel (int t)
    {
        type = t;
    }

public Insets getInsets ()
    {
        return insets;
    }

public void paint (Graphics g)
    {
        int wd = getSize().width;
        int ht = getSize().height;

        switch (type) {
        case RAISED:
            g.setColor (lightGray);
            g.drawLine (0, 0, wd-1, 0);
            g.drawLine (0, 1, wd-2, 1);
            g.drawLine (0, 2, 0, ht-1);
            g.drawLine (1, 2, 1, ht-2);
            g.setColor (darkGray);
            g.drawLine (2, ht-2, wd-1, ht-2);
            g.drawLine (1, ht-1, wd-1, ht-1);
            g.drawLine (wd-2, 2, wd-2, ht-2);
            g.drawLine (wd-1, 1, wd-1, ht-2);
            break;

        case SUNKEN:
            g.setColor (darkGray);
            g.drawLine (0, 0, wd-1, 0);
            g.drawLine (0, 1, wd-2, 1);
            g.drawLine (0, 2, 0, ht-1);
            g.drawLine (1, 2, 1, ht-2);
            g.setColor (lightGray);
            g.drawLine (2, ht-2, wd-1, ht-2);
            g.drawLine (1, ht-1, wd-1, ht-1);
            g.drawLine (wd-2, 2, wd-2, ht-2);
            g.drawLine (wd-1, 1, wd-1, ht-2);
            break;

        case RIDGE:
            g.setColor (lightGray);
            g.drawLine (1, 1, wd-2, 1);
            g.drawLine (1, 2, 1, ht-2);
            g.drawLine (wd-3, 3, wd-3, ht-3);
            g.drawLine (3, ht-3, wd-3, ht-3);

            g.setColor (darkGray);
            g.drawLine (2, 2, wd-3, 2);
            g.drawLine (2, 3, 2, ht-3);
            g.drawLine (2, ht-2, wd-2, ht-2);
            g.drawLine (wd-2, 2, wd-2, ht-2);
            break;
        }
    }
}
