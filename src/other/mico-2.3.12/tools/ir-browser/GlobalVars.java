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
import java.lang.*;
import java.util.*;
import java.net.*;
import java.io.*;

import DII;
import EditLinearForm;
import Proxy;

public class GlobalVars {
public static EditLinearForm editlinform = null;
public static DII applet = null;
public static Frame frame = null;
public static Proxy proxy = null;
public static TransportLayer transport = null;
public static String serverhost = null;
public static String thekey = null;

public static void openConn ()
  throws DIIException
  {
    if (transport == null) {
      transport = new TransportLayer (serverhost);
      if (!transport.connected()) {
        transport = null;
	throw new DIIException (1, "cannot connect to DII proxy");
      }
      proxy = new Proxy (transport);
    }
  }
  
public static void closeConn ()
  {
    if (transport != null)
      transport.disconnect();
    transport = null;
    proxy = null;
  }

public static void DisplayError (String txt)
    {
        AlertBox a = new AlertBox ("DII Error", txt);
        a.pack();
        a.show();
    }

public static void constrain (
    Container container, Component component,
    int grid_x, int grid_y,
    int grid_width, int grid_height,
    int fill, int anchor, double weight_x, double weight_y,
    int top, int left, int bottom, int right)
    {
        GridBagConstraints c = new GridBagConstraints();
        c.gridx = grid_x; c.gridy = grid_y;
        c.gridwidth = grid_width; c.gridheight = grid_height;
        c.fill = fill; c.anchor = anchor;
        c.weightx = weight_x; c.weighty = weight_y;
        if( top + bottom + left + right > 0 )
            c.insets = new Insets( top, left, bottom, right );
        ((GridBagLayout) container.getLayout()).setConstraints( component, c );
        container.add( component );
    }

public static void constrain (
    Container container, Component component,
    int gridx, int gridy, int gridw, int gridh,
    int fill,
    double wx, double wy)
    {
        constrain (container, component, gridx, gridy, gridw, gridh,
                   fill, GridBagConstraints.NORTHWEST, wx, wy,
                   0, 0, 0, 0);
    }
}
