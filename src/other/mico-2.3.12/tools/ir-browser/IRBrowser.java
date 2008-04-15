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
import java.awt.event.*;
import java.io.*;
import java.applet.Applet;

import AlertBox;
import EntryBox;
import CanvasObject;
import CanvasIcon;
import MyCanvas;
import View;
import IRBrowserProxy;
import DIIFrame;
import IRFrame;

class CGIcon extends CanvasIcon {
protected IRBrowser browser;
protected static CGIcon has_focus = null;

public CGIcon (int x, int y, String txt, FontMetrics fm, IRBrowser canvas)
	throws IOException
    {
	super (x, y,
               null,
	       canvas,
	       txt, fm,
	       Color.white, Color.black);
	browser = canvas;
    }

public boolean can_be_opened()
  {
    // Default is "no"
    return false;
  }

public void clicked ()
  {
    if( !can_be_opened() )
      return;

    if( has_focus != null )
      has_focus.toggle_selection();
    this.toggle_selection();
    has_focus = this;
    browser.describe( "" );
    browser.cd( GetText() );
  }

public void get_focus()
  {
    if( has_focus == this )
      return;
    if( has_focus != null )
      has_focus.toggle_selection();
    this.toggle_selection();
    has_focus = this;
    browser.describe( GetText() );
  }

public void release_focus()
  {
    this.toggle_selection();
    has_focus = null;
  }
}

class IfaceIcon extends CGIcon {
private static Image icon = null;

public IfaceIcon (int x, int y, String txt, FontMetrics fm, IRBrowser canvas)
	throws IOException
    {
        super (x, y, txt, fm, canvas);
        if (icon == null) {
            icon = GlobalVars.applet.getImage (
                GlobalVars.applet.getDocumentBase(),
                "gifs/" + "interface.gif");
        }
        SetImage (icon);
    }

public boolean can_be_opened()
  {
    return true;
  }

}

class ModuleIcon extends CGIcon {
private static Image icon = null;

public ModuleIcon (int x, int y, String txt, FontMetrics fm, IRBrowser canvas)
	throws IOException
    {
        super (x, y, txt, fm, canvas);
        if (icon == null) {
            icon = GlobalVars.applet.getImage (
                GlobalVars.applet.getDocumentBase(),
                "gifs/" + "module.gif");
        }
        SetImage (icon);
    }

public boolean can_be_opened()
  {
    return true;
  }

}

class AttrIcon extends CGIcon {
private static Image icon = null;

public AttrIcon (int x, int y, String txt, FontMetrics fm, IRBrowser canvas)
	throws IOException
    {
        super (x, y, txt, fm, canvas);
        if (icon == null) {
            icon = GlobalVars.applet.getImage (
                GlobalVars.applet.getDocumentBase(),
                "gifs/" + "attribute.gif");
        }
        SetImage (icon);
    }
}

class AliasIcon extends CGIcon {
private static Image icon = null;

public AliasIcon (int x, int y, String txt, FontMetrics fm, IRBrowser canvas)
	throws IOException
    {
        super (x, y, txt, fm, canvas);
        if (icon == null) {
            icon = GlobalVars.applet.getImage (
                GlobalVars.applet.getDocumentBase(),
                "gifs/" + "alias.gif");
        }
        SetImage (icon);
    }
}

class ArrayIcon extends CGIcon {
private static Image icon = null;

public ArrayIcon (int x, int y, String txt, FontMetrics fm, IRBrowser canvas)
	throws IOException
    {
        super (x, y, txt, fm, canvas);
        if (icon == null) {
            icon = GlobalVars.applet.getImage (
                GlobalVars.applet.getDocumentBase(),
                "gifs/" + "array.gif");
        }
        SetImage (icon);
    }
}

class ConstIcon extends CGIcon {
private static Image icon = null;

public ConstIcon (int x, int y, String txt, FontMetrics fm, IRBrowser canvas)
	throws IOException
    {
        super (x, y, txt, fm, canvas);
        if (icon == null) {
            icon = GlobalVars.applet.getImage (
                GlobalVars.applet.getDocumentBase(),
                "gifs/" + "constant.gif");
        }
        SetImage (icon);
    }
}

class EnumIcon extends CGIcon {
private static Image icon = null;

public EnumIcon (int x, int y, String txt, FontMetrics fm, IRBrowser canvas)
	throws IOException
    {
        super (x, y, txt, fm, canvas);
        if (icon == null) {
            icon = GlobalVars.applet.getImage (
                GlobalVars.applet.getDocumentBase(),
                "gifs/" + "enum.gif");
        }
        SetImage (icon);
    }
}

class ExceptIcon extends CGIcon {
private static Image icon = null;

public ExceptIcon (int x, int y, String txt, FontMetrics fm, IRBrowser canvas)
	throws IOException
    {
        super (x, y, txt, fm, canvas);
        if (icon == null) {
            icon = GlobalVars.applet.getImage (
                GlobalVars.applet.getDocumentBase(),
                "gifs/" + "exception.gif");
        }
        SetImage (icon);
    }
}

class OpIcon extends CGIcon {
private static Image icon = null;

public OpIcon (int x, int y, String txt, FontMetrics fm, IRBrowser canvas)
	throws IOException
    {
        super (x, y, txt, fm, canvas);
        if (icon == null) {
            icon = GlobalVars.applet.getImage (
                GlobalVars.applet.getDocumentBase(),
                "gifs/" + "operation.gif");
        }
        SetImage (icon);
    }

public boolean can_be_opened()
  {
    return true;
  }

public void clicked ()
    {
      if( has_focus != null )
	has_focus.toggle_selection();
      this.toggle_selection();
      has_focus = this;
      browser.describe( GetText() );
      browser.invoke( GetText() );
    }
}

class PrimiIcon extends CGIcon {
private static Image icon = null;

public PrimiIcon (int x, int y, String txt, FontMetrics fm, IRBrowser canvas)
	throws IOException
    {
        super (x, y, txt, fm, canvas);
        if (icon == null) {
            icon = GlobalVars.applet.getImage (
                GlobalVars.applet.getDocumentBase(),
                "gifs/" + "primitive.gif");
        }
        SetImage (icon);
    }
}

class SeqIcon extends CGIcon {
private static Image icon = null;

public SeqIcon (int x, int y, String txt, FontMetrics fm, IRBrowser canvas)
	throws IOException
    {
        super (x, y, txt, fm, canvas);
        if (icon == null) {
            icon = GlobalVars.applet.getImage (
                GlobalVars.applet.getDocumentBase(),
                "gifs/" + "sequence.gif");
        }
        SetImage (icon);
    }
}

class StringIcon extends CGIcon {
private static Image icon = null;

public StringIcon (int x, int y, String txt, FontMetrics fm, IRBrowser canvas)
	throws IOException
    {
        super (x, y, txt, fm, canvas);
        if (icon == null) {
            icon = GlobalVars.applet.getImage (
                GlobalVars.applet.getDocumentBase(),
                "gifs/" + "string.gif");
        }
        SetImage (icon);
    }
}

class StructIcon extends CGIcon {
private static Image icon = null;

public StructIcon (int x, int y, String txt, FontMetrics fm, IRBrowser canvas)
	throws IOException
    {
        super (x, y, txt, fm, canvas);
        if (icon == null) {
            icon = GlobalVars.applet.getImage (
                GlobalVars.applet.getDocumentBase(),
                "gifs/" + "struct.gif");
        }
        SetImage (icon);
    }
}

class TypedefIcon extends CGIcon {
private static Image icon = null;

public TypedefIcon (int x, int y, String txt, FontMetrics fm, IRBrowser canvas)
	throws IOException
    {
        super (x, y, txt, fm, canvas);
        if (icon == null) {
            icon = GlobalVars.applet.getImage (
                GlobalVars.applet.getDocumentBase(),
                "gifs/" + "typedef.gif");
        }
        SetImage (icon);
    }
}

class UnionIcon extends CGIcon {
private static Image icon = null;

public UnionIcon (int x, int y, String txt, FontMetrics fm, IRBrowser canvas)
	throws IOException
    {
        super (x, y, txt, fm, canvas);
        if (icon == null) {
            icon = GlobalVars.applet.getImage (
                GlobalVars.applet.getDocumentBase(),
                "gifs/" + "union.gif");
        }
        SetImage (icon);
    }
}

class BackIcon extends CGIcon {
private static Image icon = null;

public BackIcon (int x, int y, String txt, FontMetrics fm, IRBrowser canvas)
	throws IOException
    {
        super (x, y, txt, fm, canvas);
        if (icon == null) {
            icon = GlobalVars.applet.getImage (
                GlobalVars.applet.getDocumentBase(),
                "gifs/" + "back.gif");
        }
        SetImage (icon);
    }

public boolean can_be_opened()
  {
    return true;
  }

}



public class IRBrowser extends MyCanvas
    implements EntryBoxHandler, MouseListener,
               MouseMotionListener, FocusListener, KeyListener {

private Vector icons;
private CGIcon curicon;
private int dragx, dragy;

private Vector viewers;

private IRFrame frame;
private DIIFrame diiframe;

private final static int stateIdle = 0;
private final static int stateDrag = 1;
private final static int stateCreate = 2;
private final static int stateEdit = 3;
private int state;

private final int layoutXDist = 20;
private final int layoutYDist = 20;

protected IRBrowserProxy irproxy;


public IRBrowser (Dimension prefsize, boolean editable, IRFrame f)
    {
	super (prefsize);
	frame = f;
	diiframe = null;
        state = stateIdle;
	curicon = null;
        viewers = new Vector();
	icons = new Vector();

	this.addMouseListener( this );
	this.addMouseMotionListener( this );
	this.addFocusListener( this );
	this.addKeyListener( this );

	try {
	    String key = GlobalVars.proxy.bind ("IDL:IRBrowser:1.0");
	    irproxy = new IRBrowserProxy (key);
	} catch (DIIException e) {
	    GlobalVars.DisplayError (e.getErrMsg());
	    return;
	}
    }

public void focusGained( FocusEvent e )
    {
    }

public void focusLost( FocusEvent e )
    {
      ShowStatus (-1, -1);
    }

public void keyPressed( KeyEvent e )
    {
      if (e.getKeyChar() == '\033')
	Abort ();
      else if (e.getKeyChar() == 'l')
	Layout ();
      else if (e.getKeyChar() == 'r')
	reload ();
    }

public void keyReleased( KeyEvent e )
    {
    }

public void keyTyped( KeyEvent e )
    {
    }

public void mouseClicked (MouseEvent e)
    {
    }

public void mouseEntered (MouseEvent e)
    {
    }

public void mouseExited (MouseEvent e)
    {
    }

public void mousePressed (MouseEvent e)
    {
      // Left mouse detection for JDK 1.3
      if( (e.getModifiers() & InputEvent.BUTTON1_MASK) != 0 )
	Select( e );
      // Left mouse detection for pre-JDK 1.3
      if( e.getModifiers() == 0 )
	Select( e );
      if( (e.getModifiers() & InputEvent.BUTTON3_MASK) != 0 )
	Pick( e );
    }

public void mouseReleased (MouseEvent e)
    {
      Drop( e );
    }

public void mouseDragged (MouseEvent e)
    {
      Drag( e );
    }

public void mouseMoved (MouseEvent e)
    {
      ShowStatus( e.getX(), e.getY() );
    }


private void ShowStatus (int x, int y)
    {
        String s = "";
        CGIcon icon = null;

        if (x >= 0 && y >= 0) {
            try {
                CanvasObject co = super.Find (x, y);
                if (co instanceof CGIcon) {
                    icon = (CGIcon)co;
                }
            } catch (NoSuchElementException ex) {}

            if (icon != null) {
		s = "Button-1: select";
		if( icon.can_be_opened() )
		  s = s + ", Button-3: open";
            }
        }
	frame.status.setStatus (s);
    }

private CGIcon createIcon (int x, int y, String name, String type)
	throws IOException
    {
	if (type.equals ("interface"))
	    return new IfaceIcon (x, y, name,
				  getFontMetrics (getFont()),
				  this);
	if (type.equals ("module"))
	    return new ModuleIcon (x, y, name,
				   getFontMetrics (getFont()),
				   this);
	if (type.equals ("attribute"))
	    return new AttrIcon (x, y, name,
				 getFontMetrics (getFont()),
				 this);
	if (type.equals ("operation"))
	    return new OpIcon (x, y, name,
			       getFontMetrics (getFont()),
			       this);
	if (type.equals ("alias"))
	    return new AliasIcon (x, y, name,
				  getFontMetrics (getFont()),
				  this);
	if (type.equals ("array"))
	    return new ArrayIcon (x, y, name,
				  getFontMetrics (getFont()),
				  this);
	if (type.equals ("constant"))
	    return new ConstIcon (x, y, name,
				  getFontMetrics (getFont()),
				  this);
	if (type.equals ("enum"))
	    return new EnumIcon (x, y, name,
				 getFontMetrics (getFont()),
				 this);
	if (type.equals ("exception"))
	    return new ExceptIcon (x, y, name,
				   getFontMetrics (getFont()),
				   this);
	if (type.equals ("primitive"))
	    return new PrimiIcon (x, y, name,
				  getFontMetrics (getFont()),
				  this);
	if (type.equals ("sequence"))
	    return new SeqIcon (x, y, name,
				getFontMetrics (getFont()),
				this);
	if (type.equals ("string"))
	    return new StringIcon (x, y, name,
				   getFontMetrics (getFont()),
				   this);
	if (type.equals ("struct"))
	    return new StructIcon (x, y, name,
				   getFontMetrics (getFont()),
				   this);
	if (type.equals ("typedef"))
	    return new TypedefIcon (x, y, name,
				    getFontMetrics (getFont()),
				    this);
	if (type.equals ("union"))
	    return new UnionIcon (x, y, name,
				  getFontMetrics (getFont()),
				  this);
	if (type.equals ("back"))
	    return new BackIcon (x, y, name,
				 getFontMetrics (getFont()),
				 this);
	System.out.println ("unknown type: " + type);
	return null;
    }

private void clear ()
    {
	for (int i = 0; i < icons.size(); ++i) {
	    RemoveFromCanvas ((CGIcon)icons.elementAt(i));
	}
	icons.removeAllElements();
    }

public void invoke (String name)
    {
        setCursor( new Cursor( Cursor.WAIT_CURSOR ) );
	try {
	    if (diiframe != null && diiframe.exited())
		diiframe = null;
	    if (diiframe != null) {
		diiframe.setCG (irproxy.optemplate (name));
	    } else {
		String repoid = irproxy.repoid();
		diiframe = new DIIFrame (repoid, irproxy.optemplate (name));
		diiframe.pack();
		diiframe.init();
		diiframe.show();
		diiframe.Bind (repoid);
	    }
	} catch (DIIException ex) {
	    GlobalVars.DisplayError (ex.getErrMsg());
	}
        setCursor( new Cursor( Cursor.DEFAULT_CURSOR ) );
    }

public void cd (String name)
  {
    // change into
    setCursor( new Cursor( Cursor.WAIT_CURSOR ) );
    try {
      if (irproxy.cd (name)) {
	if (diiframe != null) {
	  diiframe.exit ();
	  diiframe = null;
	}
	reload();
      }
    } catch (DIIException ex) {
      GlobalVars.DisplayError (ex.getErrMsg());
    }
    setCursor( new Cursor( Cursor.DEFAULT_CURSOR ) );
  }

public void describe( String name )
  {
    if( name.equals( ".." ) || name.equals( "" ) ) {
      frame.description.setText( "" );
    } else {
      setCursor( new Cursor( Cursor.WAIT_CURSOR ) );
      String descr = new String( "" );
      try {
	descr = irproxy.describe( name );
      } catch (DIIException ex) {
	GlobalVars.DisplayError (ex.getErrMsg());
      }
      frame.description.setText( descr );
      setCursor( new Cursor( Cursor.DEFAULT_CURSOR ) );
    }
  }

public void reload ()
    {
	AutoUpdateOff ();
	clear();
	try {
	    Vector names = irproxy.list();
	    Vector types = irproxy.types();
	    for (int i = 0; i < names.size(); ++i) {
		try {
		    CGIcon icon = createIcon (0, 0,
					      (String)names.elementAt(i),
					      (String)types.elementAt(i));
		    icons.addElement (icon);
		    Add (icon);
		} catch (IOException ex) {
		    GlobalVars.DisplayError ("cannot load icon");
		}
	    }
	    frame.path.setText ("Path: " + irproxy.getwd());
	} catch (DIIException ex) {
	    GlobalVars.DisplayError (ex.getErrMsg());
	}
	NotifyViewers();
	Layout();
	AutoUpdateOn ();
    }

public void close ()
    {
        if (diiframe != null) {
	    diiframe.exit ();
	    diiframe = null;
	}
    }

private void Pick (MouseEvent ev)
    {
	if (state != stateIdle)
	    return;

	CGIcon icon = null;
	try {
	    CanvasObject co = super.Find (ev.getX(), ev.getY());
	    if (co instanceof CGIcon) {
		icon = (CGIcon)co;
	    }
	} catch (NoSuchElementException ex) {}

	if (icon != null) {
	  icon.clicked ();
	}
    }

private void Select (MouseEvent ev)
  {
    if (state != stateIdle)
      return;

    CGIcon icon = null;
    try {
      CanvasObject co = super.Find (ev.getX(), ev.getY());
      if (co instanceof CGIcon) {
	icon = (CGIcon)co;
      }
    } catch (NoSuchElementException ex) {}

    if( icon != null )
      icon.get_focus();
  }

private void Drag (MouseEvent ev)
    {
	switch (state) {
	case stateDrag:
            curicon.Move (ev.getX() - dragx, ev.getY() - dragy);
            dragx = ev.getX();
            dragy = ev.getY();
	    break;

	case stateCreate:
	    break;
        }
    }

private void Drop (MouseEvent ev)
    {
	switch (state) {
	case stateDrag:
	    state = stateIdle;
	    CheckResize ();
	    break;

	case stateCreate:
            break;
	}
        ShowStatus (ev.getX(), ev.getY());
    }

private void Abort ()
    {
	switch (state) {
	case stateCreate:
	    state = stateIdle;
	    break;
	}
        state = stateIdle;
    }

public void entryBoxHandler (EntryBox e, Object arg)
    {
	if (state != stateEdit)
	    return;
	state = stateIdle;
	if (e.GetButton() == 0) {
	    NotifyViewers();
	}
    }

protected void Edit (String txt)
    {
	state = stateEdit;
        EntryBox editor = new EntryBox ("Edit Node", "Enter node:", txt,
					this, null);
        editor.pack();
        editor.show();
    }

private void RemoveFromCanvas (CGIcon icon)
    {
	super.Remove (icon);
    }

public void Layout ()
    {
	int x, y, maxht;
	x = layoutXDist;
	y = layoutYDist;
	maxht = 0;

	Dimension sz = getSize();

	AutoUpdateOff ();
	for (int i = 0; i < icons.size(); ++i) {
	    CGIcon icon = (CGIcon)icons.elementAt (i);
	    Rectangle bbox = icon.Bbox();
	    if (x > 0 && x + bbox.width > sz.width) {
		y += maxht + layoutYDist;
		maxht = 0;
		x = layoutXDist;
	    }
	    icon.MoveTo (x, y);
	    x += bbox.width + layoutXDist;
	    if (bbox.height > maxht)
		maxht = bbox.height;
	}
	CheckResize ();
	AutoUpdateOn ();
    }

public void AddView (View v)
    {
        viewers.addElement (v);
    }

public void DelView (View v)
    {
        viewers.removeElement (v);
    }

private void NotifyViewers ()
    {
        for (int i = 0; i < viewers.size(); ++i)
            ((View)viewers.elementAt(i)).UpdateView (this);
    }
}


